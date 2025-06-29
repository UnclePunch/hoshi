#ifndef MEX_H_MEX
#define MEX_H_MEX

#include "structs.h"
#include "datatypes.h"
#include "obj.h"
#include "hsd.h"

/*** Structs ***/

typedef struct MEXInstructionTable
{
    u32 cmd : 8;          // 0x0
    u32 code_offset : 24; // 0x01
    uint reloc_offset;    // 0x4
} MEXInstructionTable;

typedef struct MEXFunctionTable
{
    int index;        // 0x0
    uint code_offset; // 0x4
} MEXFunctionTable;

typedef struct MEXDebugSymbol
{
    uint code_offset_start; // 0x0
    uint code_offset_end;   // 0x4
    char *symbol;           // 0x8
} MEXDebugSymbol;

typedef struct MEXDebug
{
    int symbol_num;         // 0x18
    MEXDebugSymbol *symbol; // 0x1c
} MEXDebug;

typedef struct MEXFunctionLookup
{
    int num;
    void *function[30];
} MEXFunctionLookup;

typedef struct MEXFunction
{
    u8 *code;                                     // 0x0
    MEXInstructionTable *instruction_reloc_table; // 0x4
    int instruction_reloc_table_num;              // 0x8
    MEXFunctionTable *func_reloc_table;           // 0xC
    int func_reloc_table_num;                     // 0x10
    int code_size;                                // 0x14
    MEXDebug debug;                               // 0x18
} MEXFunction;

static MEXFunctionLookup **stc_mexfunction_lookup = (MEXFunctionLookup **)0x804dfad8;

/*** Functions ***/
HSD_Archive *MEX_LoadRelArchive(char *file, void *functions, char *symbol);
int bp();

static void MEX_RelocRelArchive(MEXFunction *func)
{
    // delocate
    for (int instr_idx = 0; instr_idx < func->instruction_reloc_table_num; instr_idx++)
    {
        u8 cmd = func->instruction_reloc_table[instr_idx].cmd;
        int *func_ptr = (int *)func->instruction_reloc_table[instr_idx].code_offset;
        int *code_ptr = (int *)func->instruction_reloc_table[instr_idx].reloc_offset;

        // convert rel addr to mem
        if (!((int)func_ptr & 0x80000000))
            func_ptr = (int *)((int)func_ptr + (int)func->code);

        // convert code addr to mem
        if (!((int)code_ptr & 0x80000000))
            code_ptr = (int *)((int)code_ptr + (int)func->code);

        switch (cmd)
        {
        case 1: // static addr
        {

            (*func_ptr) = (int)code_ptr;
            break;
        }
        case 4: // load addr (low half)
        case 6: // load addr (high half)
        {
            // check if the low half is signed
            if ((int)code_ptr & 0x8000)
            {
                // adjust this address to load a negative offset
                int high = ((int)code_ptr & 0xffff0000) >> 16;
                int low = ((int)code_ptr & 0x0000ffff);
                code_ptr = (int *)(((high + 1) << 16) | low);
            }

            if (cmd == 4) // low half
                *(u16 *)(func_ptr) = ((int)code_ptr & 0x0000ffff);
            else if (cmd == 6) // high half
                *(u16 *)(func_ptr) = ((int)code_ptr >> 16);

            break;
        }
        case 10: // branch instr
        {
            int offset = (int)code_ptr - (int)func_ptr; // get offset
            int branch_instr = (*func_ptr);             // get branch instr
            branch_instr |= offset & 0x03fffffc;        // insert offset into branch instr
            (*func_ptr) = branch_instr;                 // store back modified branch instr

            break;
        }
        case 26: // relative 32 bit
        {
            int offset = (int)code_ptr - (int)func_ptr; // get offset
            (*func_ptr) = offset;
            break;
        }
        }
    }
}

static void MEX_InitRELDAT(HSD_Archive *archive, char *symbol_name, void **return_func_array)
{
    MEXFunction *mex_function = Archive_GetPublicAddress(archive, symbol_name);

    if (mex_function)
    {
        // OSReport("relocating %d instructions and %d functions in MEX function %s @ %x\n", mex_function->instruction_reloc_table_num, mex_function->func_reloc_table_num, symbol_name, mex_function);
        MEX_RelocRelArchive(mex_function); // reloc instructions

        // reloc function pointers
        for (int i = 0; i < mex_function->func_reloc_table_num; i++)
        {
            MEXFunctionTable *this_func = &mex_function->func_reloc_table[i];
            return_func_array[this_func->index] = (void *)&mex_function->code[this_func->code_offset];
        }

        // flush cache on code section
        TRK_FlushCache(mex_function->code, mex_function->code_size);
    }

    return;
}

#endif