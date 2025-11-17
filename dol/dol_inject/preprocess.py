import sys
import os
import re

def process_file(infile, outfile):
    filename = os.path.splitext(infile)[0].replace("/", "_")
    hook_label = f"HOOK_{filename}"
    hookend_label = f"HOOKEND_{filename}"
    inject_label = f"INJECT_{filename}"

    with open(infile, "r") as f:
        lines = f.readlines()

    out_lines = []

    # Look for a hexadecimal address in a comment on the first line
    if lines:
        match = re.search(r'0x[0-9A-Fa-f]+', lines[0])
        if match:
            addr = match.group(0)
            out_lines.append(f".set {inject_label}, {addr}\n")


    # Add HOOK_ label
    out_lines.append(f"{hook_label}:\n")

    # Original content
    out_lines.extend(lines)

    # Ensure end label is on a new line
    out_lines.append(f"\n{hookend_label}:\n")
    out_lines.append("\t.long 0x0\n")
    out_lines.append("\t.long 0x0\n")

    os.makedirs(os.path.dirname(outfile), exist_ok=True)

    with open(outfile, "w") as f:
        f.writelines(out_lines)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python append_instr.py <input.S> <output.S.tmp>")
        sys.exit(1)
    process_file(sys.argv[1], sys.argv[2])
