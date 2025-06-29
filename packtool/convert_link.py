import sys

def convert_link_to_ld(link_path, ld_path):
    entries = []

    # Read and parse .link file
    with open(link_path, "r") as f:
        for line in f:
            line = line.strip()
            if not line or ":" not in line:
                continue

            addr_str, name = line.split(":", 1)
            try:
                addr = int("0x" + addr_str, 16)  # Prepend 0x
            except ValueError:
                print(f"[!] Invalid address '{addr_str}'")
                continue

            entries.append((addr, name))

    # Write .ld script
    with open(ld_path, "w") as f:
        f.write("SECTIONS\n{\n")
        for addr, name in entries:
            f.write(f"  {name} = 0x{addr:08X};\n")
        f.write("}\n")

    print(f"[✓] Wrote linker script to: {ld_path}")


if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python convert_link_to_ld.py input.link output.ld")
        sys.exit(1)

    convert_link_to_ld(sys.argv[1], sys.argv[2])
