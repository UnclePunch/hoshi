import os
import argparse
import sys
import struct
import hashlib
import time
from dolreader.dol import DolFile
from dolreader.section import TextSection
import elf_utils
from elf_utils import Section, RelocSection, Symbol, Reloc 
import ppc_utils
import reloc_utils 
from ppc_utils import Function
from sig_utils import SignatureLibrary, FunctionSignature
from riivolution_utils import RiivolutionMod, RiivolutionPatch

def branch_create(from_addr, to_addr, is_link):
    return 0x48000000 | ((to_addr - from_addr) & 0x03FFFFFC) | (1 if is_link == 1 else 0)

def align(num, align):
   return (num + (align-1)) & ~(align-1) # round up n bytes

def sign_extend_16(value: int) -> int:
    value &= 0xFFFF
    return value if value < 0x8000 else value - 0x10000

def instrs_to_addr(lis_instr : int, ori_instr : int):
    if ((lis_instr & 0xFC000000) >> 26 != 15):
        raise ValueError(f"address {lis_instr:08X} does not contain a valid \"lis\" instruction")
    if ((ori_instr & 0xFC000000) >> 26 != 14):
        raise ValueError(f"address {ori_instr:08X} does not contain a valid \"addi\" instruction")

    return ((lis_instr & 0xFFFF) << 16) + sign_extend_16(ori_instr & 0xFFFF)

def addr_to_instrs(address : int) -> tuple[int, int]:
    # determine instruction immediate values
    instr_imm_hi = address >> 16
    instr_imm_lo = address & 0x0000FFFF

    # convert to subi if low bits of the address are over 0x8000
    if address & 0x00008000:
        instr_imm_hi += 0x1
        instr_imm_lo = (instr_imm_lo - 0x10000) & 0xFFFF

    lis_instr = (0x3c600000 | instr_imm_hi)
    addi_instr = (0x38630000 | instr_imm_lo)

    return lis_instr, addi_instr

def find_func_by_signature(functions : list[Function], sigs : list[FunctionSignature]):
    for function in functions:
        hash = -1 # reset hash
        for sig in sigs:
            if function.size == sig.instruction_total * 4:
                # calculate hash if not yet calculated
                if hash == -1:
                    func_normalized = function.normalize()
                    hash = hashlib.sha1(func_normalized).hexdigest()

                    # dump normalized function
                    #with open(f"{section_address + function.section_offset:08X}.bin", "wb") as f:
                    #    f.write(func_normalized)

                if hash == sig.hash:
                    return function, sig, func_normalized          
    return None, None, None

def dol_search_for_heaplo_instructions(dol : DolFile):
    sig_lib = SignatureLibrary.from_json("sigs.json")

    console = dol_get_console(dol)
    if console == None:
        return None, None

    print(f"dol console: {console}")

    print(f"begin searching for OSInit in dol...")
    for i, section in enumerate(dol.textSections):
        functions = ppc_utils.get_funcs_in_section(section.data.read(), section.size)
        
        print(f" checking section [{i}] at 0x{section.address:08X} of size {section.size / 1024:.2f}kb with {len(functions)} functions")
        function, sig, func_normalized = find_func_by_signature(functions, sig_lib.groups[console])

        if sig is None:
            continue

        print(f"found OSInit at memory address 0x{section.address + function.section_offset:08X} with signature {sig.name}")

        # find instructions
        instruction_seq = [
            0x3C608000,  # lis r3
            0x38600000,  # addi r3, r3
            0x38000000,  # add 31
            0x54000000,  # mask
            #0x48000001   # func call
        ]
        pattern = b''.join(struct.pack(">I", w) for w in instruction_seq)
        func_offset = func_normalized.find(pattern, 0)

        if func_offset == -1:
            print(f"could not find instruction sequence in OSInit")
            continue

        print(f"found instruction sequence at function offset {func_offset:08x}\n")
        return section.address + function.section_offset + func_offset, section.address + function.section_offset + func_offset + 0x4

    print(f"OSInit not found.\n")
    return None, None
    
def bytes_pack(sections : list[Section]) -> bytearray:
    bytes = bytearray()
    print(f"begin packing byte array...")
    for section in sections:
        include_sections = [".text", ".rodata", ".data", ".sdata", ".bss"]
        if any(section.name.startswith(term) for term in include_sections):

            print(f" adding section {section.name}")

            bytes_offset = None

            # pad byte array
            padding = (4 - (len(bytes) % 4)) % 4
            bytes.extend(b'\x00' * padding)

            # append section data to byte array
            bytes_offset = len(bytes)
            bytes += section.data

            section.bytes_offset = bytes_offset

    print(f"done packing byte array.\n")

    return bytes

def dol_get_stack_addr(dol : DolFile, debug_stack_hi_instr_ptr, debug_stack_lo_instr_ptr) -> int:
    debug_stack_hi_instr = dol.read_uint32(debug_stack_hi_instr_ptr)
    debug_stack_lo_instr = dol.read_uint32(debug_stack_lo_instr_ptr)

    arena_lo_addr = instrs_to_addr(debug_stack_hi_instr, debug_stack_lo_instr)

    # arena_lo_addr = align(((debug_stack_hi_instr & 0xFFFF) << 16) | (debug_stack_lo_instr & 0xFFFF), 32)

    return arena_lo_addr

def dol_insert_bytes(dol, bytes, bytes_memory_address, debug_stack_hi_instr_ptr, debug_stack_lo_instr_ptr):
    section = dol.resolve_address(debug_stack_hi_instr_ptr)

    # adjust address
    adjusted_debug_stack_addr = align(bytes_memory_address + len(bytes), 32)

    # get instructions to write
    debug_stack_hi_instr, debug_stack_lo_instr = addr_to_instrs(adjusted_debug_stack_addr)

    # update instructions
    dol.write_uint32(debug_stack_hi_instr_ptr, debug_stack_hi_instr)
    dol.write_uint32(debug_stack_lo_instr_ptr, debug_stack_lo_instr)
    print(f"Injected {len(bytes) / 1024:.2f}kb.")
    print(f"Adjusted ArenaLo from 0x{bytes_memory_address:08X} to 0x{adjusted_debug_stack_addr:08X}.\n")

    bytes_section = TextSection(address=bytes_memory_address, data=bytes)
    dol.append_section(bytes_section)

def hooks_get(bytes : bytearray, bytes_memory_address : int, sections : list[Section], symbols : list[Symbol]) -> list[dict]:
    
    # create a dict to make accessing the HOOK_ and HOOKEND_ symbols faster
    symbol_dict = {this.name: this for this in symbols}
    hooks = []

    for symbol in symbols:
        if symbol.name.startswith("INJECT_"):
            hook_symbol_name = symbol.name.split("INJECT_")[1]

            hookstart_symbol = symbol_dict["HOOK_" + hook_symbol_name]
            hookend_symbol = symbol_dict["HOOKEND_" + hook_symbol_name]
            if (hookstart_symbol == None):
                raise ValueError(f"hook \"{hook_symbol_name}\" does not have symbol HOOK_" + {hook_symbol_name})
            elif (hookend_symbol == None):
                raise ValueError(f"hook \"{hook_symbol_name}\" does not have symbol HOOKEND_" + {hook_symbol_name})
            
            hookstart_section = sections[hookstart_symbol.section_index]
            hookstart_offset = hookstart_symbol.section_value
            hookend_section = sections[hookend_symbol.section_index]
            hookend_offset = hookend_symbol.section_value

            injection_address = symbol.section_value
            hookstart_address = bytes_memory_address + hookstart_section.bytes_offset + hookstart_offset
            hookend_address = bytes_memory_address + hookend_section.bytes_offset + hookend_offset

            # add to list of dicts
            hook_dict = {'name' : hook_symbol_name,
                         'injection_address' : injection_address,
                         'hookstart_address': hookstart_address,
                         'hookend_address' : hookend_address}
            hooks.append(hook_dict)

    return hooks

def dol_apply_hooks(dol : DolFile, bytes : bytearray, hooks : list[dict]):
    # copy original instruction to hook
    #dol.write_uint32(hookend_address, dol.read_uint32(injection_address))
    #hookstart_address = bytes_memory_address + hookstart_section.bytes_offset + hookstart_offset
    #dol.insert_branch(hookstart_address, symbol.section_value)

    print(f"Applying hooks to elf code...")

    for hook in hooks:
        hook_name = hook['name']
        injection_address = hook['injection_address']
        hookstart_address = hook['hookstart_address']
        hookend_address = hook['hookend_address']

        # place branch to hook
        dol.insert_branch(hookstart_address, injection_address)
        #print(f"  placing branch from 0x{injection_address:08x} to 0x{hookstart_address:08x}")

        # branch back to original code
        dol.insert_branch(injection_address + 0x4, hookend_address)
        #print(f"  placing return branch from 0x{hookstart_address:08x} to 0x{injection_address + 0x4:08x}")

        print(f" Applied hook at 0x{injection_address:08X} for {hook_name}")

    print(f"Done!\n")

def xml_create(dir_path : str, game_id : str, mod_name : str, dol : DolFile, lis_instr_ptr : int, addi_instr_ptr :int, blob : bytearray, bytes_memory_addr :int, hooks : list[dict]):
    adjusted_osarenalo_addr = align(bytes_memory_addr + len(blob), 32)
    lis_instr, addi_instr = addr_to_instrs(adjusted_osarenalo_addr)

    mod_xml = RiivolutionMod(game_id, mod_name)
    patches = []

    # create patch to adjust OSArenaLo
    lis_patch = RiivolutionPatch(lis_instr_ptr,
                                lis_instr,
                                dol.read_uint32(lis_instr_ptr))
    addi_patch = RiivolutionPatch(addi_instr_ptr,
                                addi_instr,
                                dol.read_uint32(addi_instr_ptr))
    patches.append(lis_patch)
    patches.append(addi_patch)

    # create patches for each hook
    for hook in hooks:
        injection_addr = hook['injection_address']
        hookstart_address = hook['hookstart_address']
        hookend_address = hook['hookend_address']
        to_elf = RiivolutionPatch(injection_addr,
                                branch_create(injection_addr, hookstart_address, 0),
                                dol.read_uint32(injection_addr))
        to_dol = RiivolutionPatch(hookend_address,
                                branch_create(hookend_address, injection_addr + 0x4, 0),
                                0)
        patches.append(to_elf)
        patches.append(to_dol)

    mod_xml.addPatch(patch_name="code_patch",
                binary_mem_offset=bytes_memory_addr,
                patches=patches)
    
    mod_xml.save(dir_path, blob)

    print(f"Created Riivolution folder for {mod_name}.")

def dol_get_console(dol : DolFile) -> str:
    for i, section in enumerate(dol.dataSections):
        data = section.data.read()
        if b"Dolphin OS" in data:
            return 'gc'
        if b"Revolution OS" in data:
            return 'wii'
    return None

def analyze_dol_folder(dol_folder_path):
    import os
    from pathlib import Path

    results = {}

    for root, dirs, files in os.walk(dol_folder_path):
        for fname in files:
            dol_path = Path(root) / fname
            with open(dol_path, 'rb') as f:
                dol = DolFile(f)
                print(f"checking {dol_path} ({dol_get_console(dol)}):")
                lis_instr_ptr, addi_instr_ptr = dol_search_for_heaplo_instructions(dol)

                if lis_instr_ptr is not None and addi_instr_ptr is not None:
                    success = True
                    lis_instr = dol.read_uint32(lis_instr_ptr)
                    addi_instr = dol.read_uint32(addi_instr_ptr)
                    arena_lo_addr = instrs_to_addr(lis_instr, addi_instr)
                else:
                    success = False
                    arena_lo_addr = -1


                result = {'success' : success ,
                          'arena_lo_address' :  arena_lo_addr,         
                }
                results[fname] = result

                print(f"\n")

    success_num = 0
    for fname, dict in results.items():
        if dict['success']:
            print(f"✓ 0x{dict['arena_lo_address']:08X} - {fname}")
            success_num += 1
        else:
            print(f" X {fname}")

    print(f"Identified OSInit in {success_num}/{len(results)}")

def parse_args():
    parser = argparse.ArgumentParser(description="Inject payload into DOL and/or generate XML.")

    # Required inputs
    parser.add_argument("-e", "--elf", required=True,
                        help="Path to input ELF file")
    parser.add_argument("-d", "--dol-in", required=True,
                        help="Path to input DOL file")

    # Optional outputs
    parser.add_argument("-o", "--dol-out",
                        help="Path to output DOL file")
    parser.add_argument("--riiv-out",
                        help="Path to output Riivolution files")
    parser.add_argument("--game-id",
                        help="Game ID (e.g. GKY for Kirby Air Ride)")
    parser.add_argument("--mod-name",
                        help="The name Riivolution will use to identify this mod)")

    args = parser.parse_args()

    # Validate: at least one output
    if not args.dol_out and not args.riiv_out:
        parser.error("At least one output (-o or --riiv-out) must be specified.")

    # Validate: if --riiv-out is used, require --game-id and --mod-name
    if args.riiv_out and (not args.game_id or not args.mod_name):
        parser.error("--game-id and --mod-name are required when --riiv-out is specified.")

    return args

if __name__ == '__main__':

    start = time.perf_counter()

    args = parse_args()
    print(args)

    # extract relevant data from elf
    sections, symbols, relocs = elf_utils.extract(args.elf)

    # create a byte array of code and data
    blob = bytes_pack(sections)

    #print(f"{symbols}\n")
    #print(f"{sections}\n")
    #print(f"{relocs}\n")

    with open(args.dol_in, 'rb') as f:
        dol = DolFile(f)

        # look for OSInit
        debug_stack_hi_instr_ptr, debug_stack_lo_instr_ptr = dol_search_for_heaplo_instructions(dol)

        if debug_stack_hi_instr_ptr == None or debug_stack_lo_instr_ptr == None:
            raise ValueError(f"could not find OSInit in dol")

        # determine where the heap starts by interpreting the instructions that load the address
        bytes_memory_address = dol_get_stack_addr(dol, debug_stack_hi_instr_ptr, debug_stack_lo_instr_ptr)

        # reloc code
        reloc_utils.reloc_sections(blob, bytes_memory_address, sections, relocs, symbols)

        # get list of hooks
        hooks = hooks_get(blob, bytes_memory_address, sections, symbols)

        # create xml patch
        if args.riiv_out is not None:
            xml_create(args.riiv_out,
                    args.game_id,   
                    args.mod_name,   
                    dol, 
                    debug_stack_hi_instr_ptr, 
                    debug_stack_lo_instr_ptr, 
                    blob,
                    bytes_memory_address,
                    hooks)

        if args.dol_out is not None:
            # insert into dol
            dol_insert_bytes(dol, blob, bytes_memory_address, debug_stack_hi_instr_ptr, debug_stack_lo_instr_ptr)

            # apply hooks to dol and blob
            dol_apply_hooks(dol, blob, hooks)   # apply branches to and from blob

            # write out to file
            with open(args.dol_out, "wb") as f:
                dol.save(f)

    end = time.perf_counter()
    print(f"Completed in {end - start:.3f} seconds.")

    # write blob out to file
    #with open("blob.bin", "wb") as f:
        #f.write(blob)