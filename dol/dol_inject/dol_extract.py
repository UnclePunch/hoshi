import os
import sys
from pathlib import Path
from pyisotools.iso import GamecubeISO

if __name__ == '__main__':
    if len(sys.argv) != 3:
        print("Usage: python dol_extract.py <iso_folder_path> <out_path>")
        sys.exit(1)

    iso_folder = sys.argv[1]
    out_path = sys.argv[2]

    for root, dirs, files in os.walk(iso_folder):
        for fname in files:
            if fname.lower().endswith("iso"):
                iso_path = Path(root) / fname
                dol_path = Path(out_path) / (iso_path.stem + ".dol")
                dol_path.parent.mkdir(parents=True, exist_ok=True)
                print(f"iso: {iso_path} | dol: {dol_path}")
                
                try:
                    iso = GamecubeISO.from_iso(iso_path)
                except Exception as e:
                    print(f"Skipping non-GameCube ISO: {iso_path} ({e})")
                    continue

                print(f"saving dol from {iso_path} to {dol_path}")

                with open(dol_path, "wb") as f:
                    iso.dol.save(f)

