#include "reloc.h"
#include "os.h"
#include "hoshi/log.h"

void reloc(ModHeader *header, Reloc *reloc_table)
{
    void *code_ptr = (void *)((int)header + (int)header->code_offset);

    for (int reloc_idx = 0; reloc_idx < header->relocs_num; reloc_idx++)
    {
        char cmd = reloc_table[reloc_idx].reloc_kind;
        void *symbol_ptr = (void *)reloc_table[reloc_idx].symbol_offset;
        void *instr_ptr = (void *)((int)code_ptr + reloc_table[reloc_idx].instr_offset);

        // convert symbol addr to mem
        if (!((int)symbol_ptr & 0x80000000))
            symbol_ptr = (void *)((int)symbol_ptr + (int)code_ptr);

        switch (cmd)
        {
        case R_PPC_REL24: // branch instr
        {
            int offset = (int)symbol_ptr - (int)instr_ptr; // get offset
            int branch_instr = *(int *)instr_ptr;          // get branch instr
            branch_instr |= offset & 0x03fffffc;           // insert offset into branch instr
            *(int *)instr_ptr = branch_instr;              // store back modified branch instr

            break;
        }
        case R_PPC_ADDR32: // static addr
        {
            *(int *)instr_ptr = (int)symbol_ptr;
            break;
        }
        case R_PPC_ADDR16_HA: // load addr (high half)
        case R_PPC_ADDR16_LO: // load addr (low half)
        {
            // check if the low half is signed
            if ((int)symbol_ptr & 0x8000)
            {
                // adjust this address to load a negative offset
                int high = ((int)symbol_ptr & 0xffff0000) >> 16;
                int low = ((int)symbol_ptr & 0x0000ffff);
                symbol_ptr = (int *)(((high + 1) << 16) | low);
            }

            if (cmd == R_PPC_ADDR16_HA) // high half
                *(unsigned short *)(instr_ptr) = ((int)symbol_ptr >> 16);
            else if (cmd == R_PPC_ADDR16_LO) // low half
                *(unsigned short *)(instr_ptr) = ((int)symbol_ptr & 0x0000ffff);

            break;
        }
        case R_PPC_ADDR16_HI : // idk
        {
            assert("R_PPC_ADDR16_HI detected, uh oh\n");
        }
        case R_PPC_REL32: // relative 32 bit
        {
            int offset = (int)symbol_ptr - (int)instr_ptr; // get offset
            *(int *)instr_ptr = offset;
            break;
        }
        }
    }

    // flush cache on code
    TRK_FlushCache(code_ptr, header->code_size);
}

void get_func(ModHeader *header, void **func_array)
{
    SymbolLookup *symbols = (SymbolLookup *)((int)header + (int)header->symbol_lookup_offset);
    void *code_ptr = (void *)((int)header + (int)header->code_offset);

    for (int i = 0; i < header->symbol_lookup_num; i++)
        func_array[symbols[i].function_idx] = (void *)((int)code_ptr + (int)symbols[i].function_ptr);

    return;
}