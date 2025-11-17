import struct

class PPCInstruction:
    def decode(word : int):
        """
        Normalize a 32-bit PPC instruction by stripping constants.
        Returns a 32-bit integer word:
        """
        opcode = (word >> 26) & 0x3F

        # masks
        MASK_OPCODE_RD_RA_RB = 0xFFE0F800  # opcode + rD + rA + rB
        MASK_OPCODE_RD_RA    = 0xFFE00000  # opcode + rD + rA
        MASK_BRANCH          = 0xFC000001  # opcode + link bit

        d_form_opcodes = {14, 32, 36, 37, 38, 40, 41}
        branch_opcodes = {16, 18, 19}

        if opcode in d_form_opcodes:
            return word & MASK_OPCODE_RD_RA
        elif opcode in branch_opcodes:
            return word & MASK_BRANCH
        else:
            return word & MASK_OPCODE_RD_RA_RB
    
class Function:
    def __init__(self, section_offset, size, data):
        self.section_offset = section_offset
        self.size = size
        self.data = data
        
    def normalize(self):
        """Convert raw PPC code into a normalized byte stream (opcode+regs only)."""
        out = bytearray()
        for i in range(0, self.size, 4):
            word, = struct.unpack_from(">I", self.data, i)  # big-endian
            normalized_instr = PPCInstruction.decode(word)
            # print(f"{normalized_instr:08X}")
            out += struct.pack(">I", normalized_instr)
        return bytes(out)

def get_funcs_in_section(section_bytes, section_size) -> list[Function]:

    BLR_OPCODE = 0x4E800020

    functions = []
    in_function = True
    function_start = 0

    for i in range(0, section_size, 4):
        word, = struct.unpack_from(">I", section_bytes, i)  # big-endian PPC instruction

        if not in_function:
            # Start a new function
            in_function = True
            function_start = i

        if word == BLR_OPCODE:
            # End of function
            in_function = False
            function_size = i + 4 - function_start
            function = Function(function_start, function_size, section_bytes[function_start : function_start + function_size])
            functions.append(function)

    # Catch the case where the last function has no explicit BLR (rare)
    if in_function:
        in_function = False
        function_size = i + 4 - function_start
        function = Function(function_start, function_size, section_bytes[function_start : function_start + function_size])
        functions.append(function)

    return functions
