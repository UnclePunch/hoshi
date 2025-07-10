#ifndef HOSHI_RELOC_H
#define HOSHI_RELOC_H

#include "structs.h"
#include "datatypes.h"

typedef struct ModHeader
{
    char magic[4];
    char version;
    int symbol_lookup_offset;
    int symbol_lookup_num;
    int code_offset;
    int code_size;
    int relocs_offset;
    int relocs_num;
    struct
    {
        int lookup_offset;
        int names_offset;
        int num;
    } debug_symbol;
} ModHeader;

typedef struct SymbolLookup
{
    int function_idx;
    void (*function_ptr)();
} SymbolLookup;

typedef struct DebugSymbolLookup
{
    int symbol_name_offset;
    int code_offset;
    int code_size;
} DebugSymbolLookup;

typedef struct Reloc
{
    int reloc_kind : 8;
    int instr_offset : 24;
    int symbol_offset;
} Reloc;

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

void reloc(ModHeader *header);
void get_func(ModHeader *header, void **func_array);

#endif