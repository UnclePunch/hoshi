import os
import json
import hashlib
from ppc_utils import Function

class FunctionSignature:
    def __init__(self, name, hash_str, instruction_total):
        self.name = name
        self.hash = hash_str
        self.instruction_total = instruction_total

    def to_dict(self):
        return {
            "name": self.name,
            "hash": self.hash,
            "instruction_total": self.instruction_total,
        }

    @staticmethod
    def from_dict(d):
        return FunctionSignature(d["name"], d["hash"], d["instruction_total"])

    @staticmethod
    def from_bin(path):

        # read the bin file contents
        with open(path, "rb") as f:
            code = f.read()

        # create a function class from its data
        function = Function(0, len(code), code)
        
        # generate a signature
        sig_name =  os.path.splitext(os.path.basename(path))[0]
        binary = function.normalize()
        hash = hashlib.sha1(binary).hexdigest()

        # dump normalized binary
        #out_path = (os.path.basename(path))+ "_normalized.bin"
        #with open(out_path, "wb") as outf:
        #    outf.write(binary)

        return FunctionSignature(sig_name, hash, len(binary)//4)

class SignatureLibrary:
    def __init__(self):
        # store functions grouped by subdir name
        # e.g. { "wii": [FunctionSignature, ...], "gamecube": [...] }
        self.groups: dict[str, list] = {}

    def to_json(self, path):
        # collect all signatures grouped
        all_sigs = {
            group: [sig.to_dict() for sig in sigs]
            for group, sigs in self.groups.items()
        }

        with open(path, "w") as f:
            json.dump(all_sigs, f, indent=2)

    @staticmethod
    def from_folder(folder_path):
        sigs = SignatureLibrary()
        script_dir = os.path.dirname(os.path.abspath(__file__))
        root_path = os.path.join(script_dir, folder_path)

        # walk subdirs
        for root, dirs, files in os.walk(root_path):
            # group name = relative path from base
            rel = os.path.relpath(root, root_path)
            group_name = rel if rel != "." else "default"

            sigs.groups[group_name] = []

            for fname in files:
                if fname.lower().endswith(".bin"):
                    path = os.path.join(root, fname)
                    sig = FunctionSignature.from_bin(path)
                    sigs.groups[group_name].append(sig)

        return sigs

    @staticmethod
    def from_json(json_path):
        sigs = SignatureLibrary()
        script_dir = os.path.dirname(os.path.abspath(__file__))
        json_path = os.path.join(script_dir, json_path)

        with open(json_path, "r") as f:
            data: dict[str, list[dict]] = json.load(f)
            for group, items in data.items():
                sigs.groups[group] = [FunctionSignature.from_dict(sig) for sig in items]

        return sigs