import os
import xml.etree.ElementTree as ET

class RiivolutionPatch:
    def __init__(self, mem_offset, new_value, old_value):
        self.mem_offset = mem_offset
        self.new_value = new_value
        self.old_value = old_value

class RiivolutionMod:
    def __init__(self, game_id :str , mod_name : str):
        self.game_id = game_id
        self.mod_name = mod_name
        self.disc_folder = mod_name.replace(' ', '')

        # create the xml
        wiidisc = ET.Element("wiidisc", {"version": "1"})
        ET.SubElement(wiidisc, "id", {"game": "GKY"})
        options = ET.SubElement(wiidisc, "options")
        section = ET.SubElement(options, "section", {"name" : "Mod"})
        option = ET.SubElement(section, "option", {"name" : mod_name})
        enabled = ET.SubElement(option, "choice", {"name" : "Enabled"})

        # create the file replacement patch
        files_choice = ET.SubElement(enabled, "patch", {"id" : "files"})
        files_patch = ET.SubElement(wiidisc, "patch", {"id" : "files"})
        ET.SubElement(files_patch, "folder", {"external" : "/" + self.disc_folder, "recursive" : "true"})
        ET.SubElement(files_patch, "folder", {"external" : "/" + self.disc_folder, "disc" : "/", "create" : "true", "recursive" : "true"})

        self.root = wiidisc

    def addPatch(self, patch_name, binary_mem_offset, patches : list[RiivolutionPatch], file_name="payload.bin"):

        # add new patch name under enabled
        for enabled in self.root.findall(".//choice[@name='Enabled']"):
            ET.SubElement(enabled, "patch", {"id" : patch_name})

        # add new patch data
        elmnt_patch = ET.SubElement(self.root, "patch", {"id" : patch_name})
        ET.SubElement(elmnt_patch, "memory", {"valuefile" : "/" + self.disc_folder + "/" + file_name, "offset" : f"0x{binary_mem_offset:08x}"})
        for patch in patches:
            ET.SubElement(elmnt_patch, "memory", {"offset" : f"0x{patch.mem_offset:08x}", "value" : f"{patch.new_value:08x}", "original" : f"{patch.old_value:08x}"})

    def save(self, path, blob):
        indent(self.root)

        # create output path
        os.makedirs(path, exist_ok=True)

        # create xml folder
        xml_folder = os.path.join(path, "riivolution")
        os.makedirs(xml_folder, exist_ok=True)

        # create disc folder
        disc_folder = os.path.join(path, self.disc_folder)
        os.makedirs(disc_folder, exist_ok=True)

        # write blob out to file
        payload_path = os.path.join(disc_folder, "payload.bin")
        with open(payload_path, "wb") as f:
            f.write(blob)

        # create xml file
        xml_path = os.path.join(xml_folder, self.mod_name + ".xml")
        tree = ET.ElementTree(self.root)
        tree.write(xml_path, encoding="utf-8", xml_declaration=True)

def indent(elem, level=0):
    """Recursively add indentation for pretty printing."""
    i = "\n" + level*"    "
    if len(elem):
        if not elem.text or not elem.text.strip():
            elem.text = i + "    "
        for child in elem:
            indent(child, level+1)
        if not child.tail or not child.tail.strip():
            child.tail = i
    if level and (not elem.tail or not elem.tail.strip()):
        elem.tail = i

    return

# if __name__ == '__main__':
