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
} ModHeader;

typedef struct SymbolLookup
{
    int function_idx;
    void (*function_ptr)();
} SymbolLookup;

typedef struct Reloc
{
    int reloc_kind : 8;
    int instr_offset : 24;
    int symbol_offset;
} Reloc;

void reloc(ModHeader *header);
void get_func(ModHeader *header, void **func_array);