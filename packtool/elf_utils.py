import sys
import subprocess
try:
    from elftools.elf.elffile import ELFFile
    from elftools.elf.relocation import RelocationSection
    from elftools.elf.sections import SymbolTableSection
except ImportError:
    subprocess.check_call([sys.executable, "-m", "pip", "install", "pyelftools"])
    from elftools.elf.elffile import ELFFile
    from elftools.elf.relocation import RelocationSection
    from elftools.elf.sections import SymbolTableSection


class Reloc:
    def __init__(self, section_offset, type, addend, symbol_index):
        self.section_offset = section_offset                    # offset in section to patch
        self.type = type                                        # type of reloc
        self.symbol_index = symbol_index                        # index of symbol to point to
        self.addend = addend                                    # offset in the section it is pointing to 

    def __repr__(self):
        return f"<Reloc section_offset=0x{self.section_offset:08X} type={self.type} symbol_index={self.symbol_index}>"

class Section:
    def __init__(self, name, size, bytes_offset, data=None):
        self.name = name                                        # section name (".text", ".data", etc.)
        self.size = size                                        # section size
        self.bytes_offset = bytes_offset                        # offset in the packed blob
        self.data = data                                        # raw bytes, or None

    def __repr__(self):
        offset_string = "None" if self.bytes_offset == None else f"0x{self.bytes_offset:08X}"
        return f"<Section {self.name} size=0x{self.size:08X} bytes_offset=" + offset_string + ">"

class RelocSection:
    def __init__(self, name, target_section_index, reloc_data : list[Reloc]):
        self.name = name                                    # section name
        self.target_section_index = target_section_index    #
        self.reloc_data = reloc_data                        #

    def __repr__(self):
        return f"<RelocSection {self.name} target_section_index=0x{self.target_section_index} reloc_data={self.reloc_data}>"

class Symbol:
    def __init__(self, name, type, section_index, section_value):
        self.name = name                        # Symbol name
        self.type = type                        # 
        self.section_index = section_index      # the section index this symbol exists in OR SHN_ABS if the symbol is absolute
        self.section_value = section_value      # offset in section OR memory address if the symbol is absolute

    def __repr__(self):
        section_index_string = "Absolute" if self.section_index == 'SHN_ABS' else f"{self.section_index}"
        return f"<Symbol {self.name} section_index=" + section_index_string + " section_value=0x" + f"{self.section_value:08X}>"

def extract(elf_path) -> tuple[list[Section], list[Symbol], list[RelocSection]]:

    symbols = []
    sections = []
    relocs = []

    # Load the ELF file
    with open(elf_path, 'rb') as f:
        elf = ELFFile(f)

        # get symbol table section
        symtab = elf.get_section_by_name('.symtab')

        # snag symbols
        for symbol in symtab.iter_symbols():
            this = Symbol(
                name=symbol.name,
                type=symbol.entry['st_info']['type'],
                section_index=symbol['st_shndx'],
                section_value=symbol['st_value'],
            )
            symbols.append(this)

        # snag sections and relocations if applicable
        for section in elf.iter_sections():
            if isinstance(section, RelocationSection):
                new_section = RelocSection(
                    name=section.name,
                    target_section_index=section['sh_info'],
                    reloc_data= []
                )

                # copy relocs into section
                for reloc in section.iter_relocations():
                    new_reloc = Reloc(
                        section_offset=reloc['r_offset'],
                        type=reloc['r_info_type'],
                        symbol_index=reloc['r_info_sym'],
                        addend=reloc['r_addend'],
                    )
                    new_section.reloc_data.append(new_reloc)

                relocs.append(new_section)
           
            this = Section(
                name=section.name,
                size=len(section.data()),
                bytes_offset=None,
                data=section.data(),
            )
            sections.append(this)

    return sections, symbols, relocs
