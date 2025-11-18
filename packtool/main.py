import sys
import re
import subprocess
import shutil
import os
import struct
import argparse

import elf_utils
from elf_utils import Section, RelocSection, Symbol, Reloc 

build_dir = "_build"

parser = argparse.ArgumentParser(description="Compile and pack C/ASM files into GC format.")
parser.add_argument("linked_obj", help="Linked object file")
parser.add_argument("-l", "--link", required=False, help="Needed to link .dol functions")
parser.add_argument("-m", "--modtype", required=True, help="Will populate a lookup with this mod type's symbols")
parser.add_argument("-o", "--output", default="output.bin", help="Output BIN file path")

def main():

    args = parser.parse_args()
    output_path = args.output
    lookup_symbols = load_modtype_symbols(args.modtype)
    linked_obj = args.linked_obj

    # Create temp build directory
    os.makedirs(build_dir, exist_ok=True)

    # # strip unused functions out of linked object
    linked_obj = strip_linked_obj(args.linked_obj, lookup_symbols)
    
    # compiler_name = get_compiler_info(linked_obj)
    
    all_sections, symbols, relocs = elf_utils.extract(linked_obj)

    write_modbin(output_path, all_sections, relocs, symbols, lookup_symbols)
 
    # cleanup compile files
    shutil.rmtree(build_dir)

def msys_to_win(path: str) -> str:
    """Convert MSYS/Unix paths to Windows paths only when running on Windows."""
    if os.name != "nt":
        # Not Windows → return as-is
        return path

    # On Windows → convert using cygpath
    result = subprocess.run(
        ["cygpath", "-w", path],
        capture_output=True,
        text=True,
        check=True
    )
        
    return result.stdout.strip()

def strip_linked_obj(linked_obj, lookup_symbols):
    if not lookup_symbols:
        print(f"No symbols found for the chosen modtype.")

    ld_path_unix = os.path.join(os.environ["DEVKITPPC"], "bin", "powerpc-eabi-ld")
    ld_path = msys_to_win(ld_path_unix)

    print(f"{ld_path}")

    # Build -u symbol args
    u_args = []
    for symbol in lookup_symbols:
        u_args += ["-u", symbol]

    linked_stripped = os.path.join(build_dir, "linked_stripped.o")
    cmd = [ld_path, "-r", "--gc-sections"] + u_args + ["-o", linked_stripped, linked_obj]
    subprocess.run(cmd, check=True)
    return linked_stripped

def get_compiler_info(obj_path):
    with open(obj_path, 'rb') as f:
        elf = ELFFile(f)

        # Look for .comment section
        comment_section = elf.get_section_by_name('.comment')
        if comment_section is None:
            return None

        # Read the raw bytes and split on null terminators
        data = comment_section.data()
        comments = data.split(b'\x00')

        # Decode strings and filter non-empty ones
        return [entry.decode('utf-8', errors='ignore') for entry in comments if entry]

def load_modtype_symbols(modtype, modtype_dir="modtypes"):
    SCRIPT_DIR = os.path.dirname(os.path.realpath(__file__))

    path = os.path.join(SCRIPT_DIR, modtype_dir, f"{modtype}.txt")
    if not os.path.isfile(path):
        raise FileNotFoundError(f"Mod type '{modtype}' not found in {modtype_dir}/")
    
    with open(path, "r") as f:
        symbols = [line.strip() for line in f if line.strip() and not line.startswith("#")]
    
    return symbols

def resolve_symbol_offset(symbol_name, symbols, all_sections):
    """
    Resolves a symbol name to an offset into the packed data.
    
    Returns:
      - offset (int), or None if symbol is unresolved
    """

    offset = None

    # first check for a psuedo symbol with an operator and offset
    if "+" in symbol_name or "-" in symbol_name:
        operator = "+" if "+" in symbol_name else "-"
        multiplier = 1 if operator == "+" else -1
        # print(f"resolving pseudo symbol {symbol_name}")
        symbol_name, offset_str = symbol_name.split(operator, 1)
        offset = int(offset_str, 16) * multiplier

    symbol_info = symbols.get(symbol_name)
    if symbol_info:
        section = symbol_info["section"]
        # print(f"resolving symbol {symbol_name} in section {section}")

        if section == "*ABS*":
            return symbol_info["value"]
        elif section in all_sections:
            # print(f"FOUND in all sections")
            if offset is None:
                offset = symbol_info["value"]
            return all_sections[section]['offset'] + offset
        else:
            print(f"[!] Unknown section '{section}' in symbol table for '{symbol_name}'")
            return None
        

    return None

def roundup32(x: int) -> int:
    return ((x + 31) // 32) * 32

def bytes_pack(sections : list[Section]) -> bytearray:
    bytes = bytearray()
    print(f"begin packing byte array...")
    for section in sections:
        include_sections = [".text", ".rodata", ".data", ".sdata", ".bss"]
        if any(section.name.startswith(term) for term in include_sections):

            bytes_offset = None

            # pad byte array
            padding = (4 - (len(bytes) % 4)) % 4
            bytes.extend(b'\x00' * padding)

            print(f" adding section {section.name} at offset 0x{len(bytes):08x}")

            # append section data to byte array
            bytes_offset = len(bytes)
            bytes += section.data

            section.bytes_offset = bytes_offset

    print(f"done packing byte array.\n")

    return bytes

def write_modbin(output_path, all_sections: list[Section], reloc_sections : list[RelocSection], symbols : list[Symbol], lookup_symbols):
    
    # Pack all sections together
    packed_data = bytes_pack(all_sections)
    packed_size = len(packed_data)

    # create debug symbol bytearrays. one for lookup data and one for the symbol data
    func_lookup_data = bytearray()
    func_name_data = bytearray()
    func_num = 0
    for symbol in symbols:
        if symbol.type == 'STT_FUNC':
            symbol_name_offset = len(func_name_data)
            symbol_size = all_sections[symbol.section_index].size
            func_lookup_data += struct.pack(">I I I", symbol_name_offset, all_sections[symbol.section_index].bytes_offset, symbol_size)
            func_name_data += symbol.name.encode('ascii') + b'\x00'
            func_num += 1
            # print(f"{key:<18} starts at offset {all_sections[symbol.section_index].bytes_offset:X} with size {all_sections[symbol.section_index].size:X}")

    # create reloc data
    encoded_relocs = bytearray()
    for section in reloc_sections:
        reloc_section_idx = section.target_section_index
        reloc_section = all_sections[reloc_section_idx]

        # ensure this section exists in our bytearray
        if reloc_section.bytes_offset == None:
            continue

        for i, reloc in enumerate(section.reloc_data):
            reloc_type = reloc.type
            patch_section_offset = reloc.section_offset
            target_symbol_idx = reloc.symbol_index
            target_symbol = symbols[target_symbol_idx]
            target_addend = reloc.addend
            target_symbol_type = target_symbol.type
            target_symbol_section_idx =  target_symbol.section_index
            target_symbol_section_value =  target_symbol.section_value

            target_section_offset = None
            reloc_symbol_section = None
            if target_symbol_section_idx != 'SHN_ABS':
                reloc_symbol_section = all_sections[target_symbol_section_idx]

            # STT_SECTION seems to use reloc addend to hold the offset of the section
            if target_symbol_type == 'STT_SECTION':
                target_section_offset = target_addend
            # while everything else uses the section value from the target symbol
            else:
                target_section_offset = target_symbol_section_value
                    
            if 0:
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

            if reloc.type is None:
                print(f"[!] Unknown reloc type: {reloc.type}")
                continue

            patch_offset = reloc_section.bytes_offset + patch_section_offset
            if target_symbol_section_idx == 'SHN_ABS':
                patch_data = target_symbol_section_value
            else:
                patch_data = reloc_symbol_section.bytes_offset + target_section_offset

            if patch_offset > 0xFFFFFF:
                raise ValueError(f"Instruction offset too large: 0x{patch_offset:X}")

            header = (reloc.type << 24) | (patch_offset & 0xFFFFFF)
            encoded_relocs += struct.pack(">I I", header, patch_data)    

    # create lookup for this mods symbols
    symbol_dict = {this.name: this for this in symbols} # create a dict to make accessing symbols faster
    present_lookup_symbols = bytearray()
    for i, symbol_name in enumerate(lookup_symbols):
        symbol = symbol_dict.get(symbol_name)
        if symbol is not None and isinstance(symbol.section_index, int):
            present_lookup_symbols += struct.pack(">I I", i, all_sections[symbol.section_index].bytes_offset)
            print(f" [X] {symbol_name}")
        else:
            print(f" [ ] {symbol_name}")
    
    # Define the binary header format (big endian)
    HEADER_FMT = ">4sB3xIIIIIIII I"
    HEADER_SIZE = struct.calcsize(HEADER_FMT)

    MAGIC   = b"GCMB"
    VERSION = 2

    # Sizes
    lookup_size_bytes = len(present_lookup_symbols)
    reloc_size_bytes  = len(encoded_relocs)
    func_lookup_size = len(func_lookup_data)
    func_names_size = len(func_name_data)

    lookup_count = lookup_size_bytes // 8
    reloc_count  = reloc_size_bytes // 8

    # Offsets
    off_lookup      = HEADER_SIZE
    off_packed      = roundup32(off_lookup + lookup_size_bytes)
    off_func_lookup = off_packed + packed_size
    off_func_name   = off_func_lookup + func_lookup_size 
    off_reloc       = roundup32(off_func_name + func_names_size)

    print(f"off_lookup 0x{off_lookup:08x}")
    print(f"off_packed 0x{off_packed:08x}")
    print(f"off_func_lookup 0x{off_func_lookup:08x}")
    print(f"off_func_name 0x{off_func_name:08x}")
    print(f"off_reloc 0x{off_reloc:08x}")

    # Build header block
    header = struct.pack(
        HEADER_FMT,
        MAGIC,
        VERSION,
        off_lookup,
        lookup_count,
        off_packed,
        packed_size,
        off_reloc,
        reloc_count,
        off_func_lookup,
        off_func_name,
        func_num,
    )

    # Write the final binary
    with open(output_path, "wb") as f:
        f.write(header)

        f.seek(off_lookup)
        f.write(present_lookup_symbols)

        f.seek(off_packed)
        f.write(packed_data)

        f.seek(off_func_lookup)
        f.write(func_lookup_data)

        f.seek(off_func_name)
        f.write(func_name_data)    

        f.seek(off_reloc)
        f.write(encoded_relocs)  
        print(f"Wrote {f.tell()} bytes of data to {output_path}")

if __name__ == "__main__":
    main()
