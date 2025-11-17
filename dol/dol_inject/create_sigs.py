import os
import struct
from ppc_utils import Function 
from sig_utils import SignatureLibrary


if __name__ == "__main__":
    folder = "binary" 

    sigs = SignatureLibrary.from_folder("binary")
    sigs.to_json("sigs.json")

    #with open("OSInitSigs.txt", "w") as f:
        #for fname, sig in signatures.items():
            #f.writelines(f"{fname}: {sig.hash} {sig.instruction_total}" + '\n')
                
            #out_path = (os.path.basename(fname))+ "_normalized.bin"
            #with open(out_path, "wb") as outf:
            #    outf.write(binary)

for group, funcs in sigs.groups.items():
    print(f"Group: {group}")
    for sig in funcs:
        print(f"  {sig.name}: {sig.hash}, {sig.instruction_total} instructions")