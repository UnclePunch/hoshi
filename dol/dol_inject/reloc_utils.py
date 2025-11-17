import struct
from elf_utils import Section, Reloc, RelocSection, Symbol

# PowerPC ELF relocation constants
R_PPC_NONE      = 0
R_PPC_ADDR32    = 1    # 32-bit absolute
R_PPC_ADDR16_LO = 4   # lower 16 bits
R_PPC_ADDR16_HI = 5   # upper 16 bits
R_PPC_ADDR16_HA = 6   # upper 16 bits + adjust
R_PPC_REL24     = 10   # 24-bit relative branch
R_PPC_REL32     = 26   # 32-bit address

is_debug_reloc = 0
def reloc_sections(bytes : bytearray, memory_base_address : int, sections : list[Section], relocs : list[RelocSection], symbols : list[Symbol]):
    for section in relocs:
        reloc_section_idx = section.target_section_index
        reloc_section = sections[reloc_section_idx]

        # ensure this section exists in our bytearray
        if reloc_section.bytes_offset == None:
            continue

        if is_debug_reloc:
            print(f"Reloc section ({section.name}) targetting section #{reloc_section_idx} ({reloc_section.name})")
            print(f"Reloc entries:\n")

        for i, reloc in enumerate(section.reloc_data):
            reloc_type = reloc.type
            patch_section_offset = reloc.section_offset
            target_symbol_idx = reloc.symbol_index
            target_symbol : Symbol = symbols[target_symbol_idx]
            target_addend = reloc.addend
            target_symbol_type = target_symbol.type
            target_symbol_section_idx =  target_symbol.section_index
            target_symbol_section_value =  target_symbol.section_value

            target_section_offset = None
            reloc_symbol_section = None
            if target_symbol_section_idx != 'SHN_ABS':
                reloc_symbol_section = sections[target_symbol_section_idx]

            # STT_SECTION seems to use reloc addend to hold the offset of the section
            if target_symbol_type == 'STT_SECTION':
                target_section_offset = target_addend
            # while everything else uses the section value from the target symbol
            else:
                target_section_offset = target_symbol_section_value
                    
            
            if is_debug_reloc:
                print(f"[Reloc #{i}]")
                print(f"Section to Patch: {reloc_section.name}")
                print(f"Offset to Patch: 0x{patch_section_offset:08X}")
                print(f"Relocation Type: {reloc_type}")
                print(f"Symbol Index: {target_symbol_idx}")
                print(f"Symbol Name: {target_symbol.name}")
                print(f"Symbol Type: {target_symbol_type}")
                if target_symbol_section_idx != 'SHN_ABS':
                    print(f"Target Section: {reloc_symbol_section.name}")
                    print(f"Target Blob Offset: 0x{reloc_symbol_section.bytes_offset:08X}")
                    print(f"Target Section Offset: 0x{target_section_offset:08X}")
                    print(f"Target Section Value: 0x{target_symbol_section_value:08X}")
                else:
                    print(f"Symbol Value: 0x{target_symbol_section_value:08X}")
                print(f"")

            patch_offset = reloc_section.bytes_offset + patch_section_offset
            if target_symbol_section_idx == 'SHN_ABS':
                patch_data = target_symbol_section_value
            else:
                patch_data = reloc_symbol_section.bytes_offset + target_section_offset

            reloc_apply(bytes, memory_base_address, patch_offset, patch_data, reloc_type, target_symbol_section_idx)
def reloc_apply(bytes_arr, memory_base_address, patch_offset, patch_data, reloc_type, target_symbol_section_idx):
    """
    Apply relocation patch to a bytearray at patch_offset.
    """
    if is_debug_reloc:
        print(f"Patching offset {patch_offset:08X} with data {patch_data:08X} of reloc type {reloc_type} and symbol idx {target_symbol_section_idx}")

    # adjust file addresses to mem addresses
    patch_offset_memory = memory_base_address + patch_offset
    if target_symbol_section_idx != 'SHN_ABS':
        patch_data_memory = memory_base_address + patch_data
    else:
        patch_data_memory = patch_data

    if reloc_type == R_PPC_ADDR32:
        # 32-bit absolute pointer
        original = struct.unpack(">I", bytes_arr[patch_offset:patch_offset+4])[0]
        bytes_arr[patch_offset:patch_offset+4] = struct.pack(">I", patch_data_memory)
        if is_debug_reloc:
            print(f"Original value: {original:08X}")
            print(f"Patched value: {patch_data_memory:08X}")

    elif reloc_type == R_PPC_ADDR16_HI:
        # Upper 16 bits
        original = struct.unpack(">H", bytes_arr[patch_offset:patch_offset+2])[0]
        high16 = (patch_data_memory >> 16) & 0xFFFF
        bytes_arr[patch_offset:patch_offset+2] = struct.pack(">H", high16)
        if is_debug_reloc:
            print(f"Original HI16: {original:04X}")
            print(f"Patched HI16: {high16:04X}")

    elif reloc_type == R_PPC_ADDR16_HA:
        # Upper 16 bits + adjust
        original = struct.unpack(">H", bytes_arr[patch_offset:patch_offset+2])[0]
        low16 = patch_data_memory & 0xFFFF
        high16 = ((patch_data_memory >> 16) + (1 if low16 & 0x8000 else 0)) & 0xFFFF
        bytes_arr[patch_offset:patch_offset+2] = struct.pack(">H", high16)
        if is_debug_reloc:
            print(f"Original HA16: {original:04X}")
            print(f"Patched HA16: {high16:04X}")

    elif reloc_type == R_PPC_ADDR16_LO:
        # Lower 16 bits
        original = struct.unpack(">H", bytes_arr[patch_offset:patch_offset+2])[0]
        low16 = patch_data_memory & 0xFFFF
        bytes_arr[patch_offset:patch_offset+2] = struct.pack(">H", low16)
        if is_debug_reloc:
            print(f"Original LO16: {original:04X}")
            print(f"Patched LO16: {low16:04X}")

    elif reloc_type == R_PPC_REL24:
        # Branch instruction
        original_instr = struct.unpack(">I", bytes_arr[patch_offset:patch_offset+4])[0]
        # Compute relative offset in words
        offset = (patch_data_memory - patch_offset_memory)
        # Keep opcode and link bit
        opcode = original_instr
        patched_instr = opcode | (offset & 0x03FFFFFC)
        bytes_arr[patch_offset:patch_offset+4] = struct.pack(">I", patched_instr)
        if is_debug_reloc:
            print(f"Creating branch instruction from {patch_offset_memory:08X} -> {patch_data_memory:08X}")
            print(f"offset: {offset:08X}")
            print(f"Patched branch: {patched_instr:08X}")

    elif reloc_type == R_PPC_REL32:
        # 32-bit PC-relative / signed 32-bit relocation
        original = struct.unpack(">i", bytes_arr[patch_offset:patch_offset+4])[0]
        # compute signed 32-bit relative offset: target - place
        offset = int(patch_data_memory) - int(patch_offset_memory)
        # pack
        bytes_arr[patch_offset:patch_offset+4] = struct.pack(">i", offset)
        if is_debug_reloc:
            print(f"Original value: {original & 0xFFFFFFFF:08X}")
            print(f"Patched REL32: {offset & 0xFFFFFFFF:08X} (target {patch_data_memory:08X} place {patch_offset_memory:08X})")

    else:
        raise ValueError(f"Unknown relocation type: {reloc_type}")
