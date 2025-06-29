import sys
import subprocess
import re
import shutil
import os
import struct
import argparse

from elftools.elf.elffile import ELFFile

build_dir = "_build"
objdump="powerpc-eabi-objdump"

parser = argparse.ArgumentParser(description="Compile and pack C/ASM files into GC format.")
parser.add_argument("linked_obj", help="Linked object file")
parser.add_argument("-l", "--link", required=False, help="Needed to link .dol functions")
parser.add_argument("-m", "--modtype", required=True, help="Will populate a lookup with this mod type's symbols")
parser.add_argument("-o", "--output", default="output.modbin", help="Output MODBIN file path")

RELOC_TYPE_ENUM = {
    "R_PPC_REL24":     1,
    "R_PPC_ADDR32":    2,
    "R_PPC_ADDR16_HA": 3,
    "R_PPC_ADDR16_LO": 4,
    "R_PPC_REL32":     5,
}

def main():

    args = parser.parse_args()
    output_path = args.output
    lookup_symbols = load_modtype_symbols(args.modtype)

    # Create temp build directory
    os.makedirs(build_dir, exist_ok=True)

    # strip unused functions out of linked object
    linked_obj = strip_linked_obj(args.linked_obj, lookup_symbols)

    # Dump symbols and relocs
    symbols_txt = os.path.join(build_dir, "symbols.txt")
    with open(symbols_txt, "w") as f:
        subprocess.run([objdump, "-t", linked_obj], stdout=f)

    relocs_txt = os.path.join(build_dir, "relocs.txt")
    with open(relocs_txt, "w") as f:
        subprocess.run([objdump, "-r", linked_obj], stdout=f)

    all_sections = extract_sections_and_data(linked_obj)

    symbols = parse_symbols(symbols_txt)
    reloc_entries = parse_relocs(relocs_txt)
    
    #with open(os.path.join(build_dir, "log.txt"), "wt") as f:
    #    f.write("\nsymbol dictionary:\n\n") 
    #    for name, info in symbols.items():
    #        f.write(f"  {name} in {info['section']} at 0x{info['value']:X} ({info['size']} bytes)\n")
    #    f.write("\nreloc entries:\n\n");
    #    for r in reloc_entries:
    #        f.write(f"  {r['type']} at .{r['section']}+0x{r['offset']:X} -> {r['symbol']}\n")
            
    write_modbin(output_path, all_sections, reloc_entries, symbols, lookup_symbols)
 
     # cleanup compile files
    print(f"[*] Cleaning build directory: {build_dir}")
    shutil.rmtree(build_dir)

def strip_linked_obj(linked_obj, lookup_symbols):
    if not lookup_symbols:
        print(f"No symbols found for the chosen modtype.")

    # Build -u symbol args
    u_args = []
    for symbol in lookup_symbols:
        u_args += ["-u", symbol]

    linked_stripped = os.path.join(build_dir, "linked_stripped.o")
    cmd = ["powerpc-eabi-ld", "-r", "--gc-sections"] + u_args + ["-o", linked_stripped, linked_obj]
    subprocess.run(cmd, check=True)
    return linked_stripped

def extract_sections_and_data(file_path):
    with open(file_path, 'rb') as f:
        elf = ELFFile(f)

        sections = {}
        needed_sections = {".text", ".data", ".rodata", ".bss"}

        # Loop over all sections in the ELF file
        #print("[*] Sections and Data:")
        for section in elf.iter_sections():
            section_name = section.name
            if any(section_name.startswith(prefix) for prefix in needed_sections):
                section_data = section.data()

                # Store the section name and data
                sections[section_name] = {'data' : section_data, 'offset' : None}

                # Print section information
                #print(f"Section: {section_name}, Size: {len(section_data)} bytes")

        
        print(f"Extracted {len(sections)} sections")

        return sections
    
def parse_symbols(symbols_path):
    symbols = {}

    with open(symbols_path, "r") as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith("SYMBOL TABLE") or line.startswith("out/"):
                continue

            parts = line.split()
            if len(parts) < 5:
                continue  # Not enough parts to be useful

            # Handle both 5 and 6+ part lines
            if len(parts) == 5:
                value_str, binding_type, section, size_str, name = parts
                sym_type = None
            else:
                value_str, binding_type, sym_type, section, size_str, name = parts[:6]

            try:
                value = int(value_str, 16)
                size = int(size_str, 16)
            except ValueError:
                continue

            symbols[name] = {
                "section": section,
                "value": value,
                "size": size
            }

    seen_offsets = {}
    for name, info in symbols.items():
        sec = info["section"]
        key = (sec, info["value"])

        # Skip section name symbols or debug symbols
        if name == sec or name.endswith(".c") or sec == "*ABS*" and info["size"] == 0:
            continue

        if key in seen_offsets:
            print(f"[!] Warning: Duplicate {sec} offset 0x{info['value']:X} for {name} and {seen_offsets[key]}")
        else:
            seen_offsets[key] = name

    return symbols

def parse_relocs(relocs_path):
    entries = []
    current_section = None

    section_header = re.compile(r"RELOCATION RECORDS FOR \[(\.[^\]]+)\]:")
    reloc_line = re.compile(r"^([0-9a-fA-F]{8})\s+(\S+)\s+(\S+)$")

    with open(relocs_path, "r") as f:
        for line in f:
            line = line.strip()
            if not line:
                continue

            # Match section headers
            sec_match = section_header.match(line)
            if sec_match:
                current_section = sec_match.group(1)
                # print(f"entering section {current_section}")
                continue

            # Match relocation lines
            rel_match = reloc_line.match(line)
            if rel_match:
                offset = int(rel_match.group(1), 16)
                rtype = rel_match.group(2)
                symbol = rel_match.group(3)

                # print(f" adding reloc {rtype} in section {current_section} pointing to {symbol}")
                entries.append({
                    "section": current_section,
                    "offset": offset,
                    "type": rtype,
                    "symbol": symbol
                })


    print(f"Extracted {len(entries)} relocations")

    return entries

def resolve_symbol_offset(symbol_name, symbols, all_sections):
    """
    Resolves a symbol name to an offset into the packed data.
    
    Returns:
      - offset (int), or None if symbol is unresolved
    """
    symbol_info = symbols.get(symbol_name)
    if symbol_info:
        section = symbol_info["section"]

        if section == "*ABS*":
            return symbol_info["value"]
        elif section in all_sections:
            return all_sections[section]['offset'] + symbol_info["value"]
        else:
            print(f"[!] Unknown section '{section}' in symbol table for '{symbol_name}'")
            return None

    # Fallback: section+offset pseudo-symbol
    if "+" in symbol_name or "-" in symbol_name:
        multiplier = 1 if "+" in symbol_name else -1
        operator = "+" if "+" in symbol_name else "-"
        try:
            base_section, offset_str = symbol_name.split(operator, 1)
            offset = int(offset_str, 16)
            # print(f"symbol {symbol_name} -> {base_section} has operator in it, multiplying {offset:x} by {multiplier}")

            if base_section in all_sections:
                return all_sections[base_section]['offset'] + (offset * multiplier)
            else:
                print(f"[!] Unknown base section '{base_section}' in pseudo-symbol '{symbol_name}'")
                return None
        except ValueError:
            print(f"[!] Invalid pseudo-symbol format: '{symbol_name}'")
            return None

    return None

def load_modtype_symbols(modtype, modtype_dir="modtypes"):
    SCRIPT_DIR = os.path.dirname(os.path.realpath(__file__))

    path = os.path.join(SCRIPT_DIR, modtype_dir, f"{modtype}.txt")
    if not os.path.isfile(path):
        raise FileNotFoundError(f"Mod type '{modtype}' not found in {modtype_dir}/")
    
    with open(path, "r") as f:
        symbols = [line.strip() for line in f if line.strip() and not line.startswith("#")]
    
    return symbols

def write_modbin(output_path, all_sections, reloc_entries, symbols, lookup_symbols):
    
    # Pack all sections together
    packed_data = bytearray()

    for key, entry in all_sections.items():
        entry['offset'] = len(packed_data)
        packed_data += entry['data']
        if len(packed_data) % 4 != 0:
            packed_data += bytes(4 - (len(packed_data) % 4))
        # print(f"{key:<18} {len(entry['data']):>6x} bytes @ offset {entry['offset']:>6x}")
        
    packed_size = len(packed_data)

    # create reloc data
    encoded_relocs = bytearray()
    for reloc in reloc_entries:
        reloc_type = RELOC_TYPE_ENUM.get(reloc["type"])
        if reloc_type is None:
            print(f"[!] Unknown reloc type: {reloc['type']}")
            continue

        # Instruction offset
        instr_section = f"{reloc['section']}"
        instr_offset = all_sections[instr_section]['offset'] + reloc["offset"]

        # Symbol offset
        target_offset = resolve_symbol_offset(reloc["symbol"], symbols, all_sections)
        # print(f" encoding reloc for section {instr_section} with target offset {target_offset:x}")
        if target_offset is None:
            print(f"[!] Symbol {reloc['symbol']} not found")
            continue

        if instr_offset > 0xFFFFFF:
            raise ValueError(f"Instruction offset too large: 0x{instr_offset:X}")

        header = (reloc_type << 24) | (instr_offset & 0xFFFFFF)
        encoded_relocs += struct.pack(">I I", header, target_offset)

    # create lookup for this mods symbols
    present_lookup_symbols = bytearray()
    symbol_idx = 0
    for symbol_name in lookup_symbols:
        symbol_offset = resolve_symbol_offset(symbol_name, symbols, all_sections)
        if symbol_offset is not None:
            present_lookup_symbols += struct.pack(">I I", symbol_idx, symbol_offset)
            print(f" [X] {symbol_name}")
        else:
            print(f" [ ] {symbol_name}")
        symbol_idx+=1

    with open(output_path, "wb") as f:
        # Header
        f.write(b"GCMB")                                                                # Magic
        f.write(struct.pack(">B", 1))                                                   # Version
        f.write(b"\x00\x00\x00")                                                        # Reserved
        f.write(struct.pack(">I", 0x20))                                                # symbol lookup file pointer
        f.write(struct.pack(">I", len(present_lookup_symbols) // 8))                    # symbol lookup size
        f.write(struct.pack(">I", 0x20 + len(present_lookup_symbols)))                  # Packed section file pointer
        f.write(struct.pack(">I", packed_size))                                         # Packed section size
        f.write(struct.pack(">I", 0x20 + packed_size + len(present_lookup_symbols)))    # symbol lookup file pointer
        f.write(struct.pack(">I", len(encoded_relocs) // 8))                            # Relocation count

        # lookup symbols
        f.write(present_lookup_symbols)

        # Data
        f.write(packed_data)
        
        # write reloc data
        f.write(encoded_relocs)
        

    print(f"Wrote {packed_size} bytes of packed data to {output_path}")

if __name__ == "__main__":
    main()
