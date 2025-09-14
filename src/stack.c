#include "text.h"
#include "os.h"
#include "hsd.h"
#include "preload.h"
#include "scene.h"
#include "inline.h"
#include "audio.h"
#include "game.h"
#include <string.h>

#include "hoshi/log.h"

#include "stack.h"
#include "hoshi.h"
#include "reloc/reloc.h"
#include "code_patch/code_patch.h"

static StackLog stc_stack_log = {.num = 0};
static MEXDebug *stc_dol_debug = 0;
static char *iso_name;

void Stack_Init()
{
    Stack_ApplyPatches();

    // load iso name from opening.bnr
    iso_name = HSD_MemAlloc(0x20);
    File_Read(DVDConvertPathToEntrynum("opening.bnr"), 0x1860, iso_name, 0x20, 0x21, 1, 0, 0);
}

// Functions
void Stack_Print()
{
    OSReport("\n");

    // attempt to load dol symbol names
    if (DVDConvertPathToEntrynum("MxDb.dat") != -1 &&
        stc_preload_heaps_lookup->heap_arr[0].is_disabled == 0 &&
        OSCheckHeap(HSD_GetHeapID()) > File_GetSize("MxDb.dat"))
    {
        HSD_Archive *debug_archive = Archive_LoadFile("MxDb.dat");
        if (!debug_archive)
            OSReport("MxDb.dat not found, DOL symbols will not be identified\n\n");
        else
            stc_dol_debug = Archive_GetPublicAddress(debug_archive, "mexDebug");
    }

    OSReport("%s\n", iso_name);
    OSReport("hoshi v" STR(MODLOADER_VERSION_MAJOR) "." STR(MODLOADER_VERSION_MINOR) "\n");
    OSReport((char *)0x80507bd4); // STACK text
    OSReport(" LR Save:   Symbol\n");

    // output each function on the callstack
    for (int i = 0; i < stc_stack_log.num; i++)
    {
        // get symbol's name
        char *symbol_name = Stack_FindSymbolNameFromAddress(stc_stack_log.log[i].lr);
        if (!symbol_name)
            symbol_name = "N/A";

        // output it
        OSReport("%08X:   %.35s\n",
                 stc_stack_log.log[i].lr,
                 symbol_name);
    }

    // output segfault if it occurred
    if (stc_stack_log.exception_kind != -1)
    {
        void *(*Stack_PrintAddress)(int exception_kind, int dsisr, void *segfault_instr_addr, StackData *sp) = (void *)0x8043aa00;
        Stack_PrintAddress(stc_stack_log.exception_kind,
                           stc_stack_log.dsisr,
                           stc_stack_log.segfault_instr_addr,
                           stc_stack_log.stack_data);

        // get symbol's name
        char *symbol_name = Stack_FindSymbolNameFromAddress((void *)((int *)stc_stack_log.stack_data)[0x198 / 4]);
        if (!symbol_name)
            symbol_name = "N/A";

        OSReport("Symbol: %s\n", symbol_name);
    }

    void (*OSEndLog)() = (void *)0x8043a15c;
    OSEndLog();

    return;
}
CODEPATCH_HOOKCREATE(0x8043f844, "", Stack_Print, "", 0)

void Stack_Log(StackData *sp, int exception_kind, int dsisr, void *segfault_instr_addr)
{
    StackChain *c = sp->chain;

    stc_stack_log.stack_data = sp;
    stc_stack_log.exception_kind = exception_kind;
    stc_stack_log.dsisr = dsisr;
    stc_stack_log.segfault_instr_addr = segfault_instr_addr;

    for (int i = 0; i < STACK_MAX_DEPTH_LOG; i++)
    {
        // next chain exists and is a valid address
        if (!c || c > (0x80000000 | OSGetPhysicalMemSize()))
            break;

        // log pointers
        stc_stack_log.log[i].addr = c;
        stc_stack_log.log[i].back_chain = c->next;
        stc_stack_log.log[i].lr = c->lr_save;
        stc_stack_log.num++;

        c = c->next;
    }
}
CODEPATCH_HOOKCREATE(0x8007d8b4,
                     "mr 3, 29\n\t"
                     "mr 4, 28\n\t"
                     "mr 5, 31\n\t"
                     "mr 6, 30\n\t",
                     Stack_Log,
                     "",
                     0x8007d90c) // 0x8007d90c
CODEPATCH_HOOKCREATE(0x8007d790,
                     "mr 3, 31\n\t"
                     "li 4, -1\n\t"
                     "li 5, 0\n\t"
                     "li 6, 0\n\t",
                     Stack_Log,
                     "",
                     0x8007d7d4) // 0x8007d90c

char *Mod_SearchForSymbol(ModHeader *mod_header, void *addr)
{
    // skip mod if no debug data is present
    if (mod_header->debug_symbol.num == 0)
        return 0;

    void *code_ptr = (void *)((u32)mod_header + (u32)mod_header->code_offset);

    // skip mod if address exists outside mods code region
    if ((u32)addr < (u32)code_ptr || (u32)addr >= ((u32)code_ptr + mod_header->code_size))
        return 0;

    DebugSymbolLookup *symbol_lookup = (DebugSymbolLookup *)((u32)mod_header + (u32)mod_header->debug_symbol.lookup_offset);
    u8 *symbol_arr = (u8 *)((u32)mod_header + (u32)mod_header->debug_symbol.names_offset);

    // binary search to find the code section the addr lies in
    int symbol_idx_min = 0;
    int symbol_idx_max = mod_header->debug_symbol.num - 1;
    int symbol_idx;
    while (symbol_idx_min <= symbol_idx_max)
    {
        symbol_idx = symbol_idx_min + (symbol_idx_max - symbol_idx_min) / 2;

        u32 symbol_addr_start = (u32)code_ptr + symbol_lookup[symbol_idx].code_offset;
        u32 symbol_addr_end = symbol_addr_start + symbol_lookup[symbol_idx].code_size;

        if ((u32)addr >= symbol_addr_start && (u32)addr < symbol_addr_end)              // symbol found
            return (char *)(&symbol_arr[symbol_lookup[symbol_idx].symbol_name_offset]); //
        else if ((u32)addr > symbol_addr_end)                                           // symbol exists after this one
            symbol_idx_min = symbol_idx + 1;
        else if ((u32)addr < symbol_addr_start) // symbol exists prior to this one
            symbol_idx_max = symbol_idx - 1;
    }

    return 0;
}
char *Stack_FindSymbolNameFromAddress(void *lr)
{
    char *symbol_name;

    // LOG_DEBUG("Searching for %p", lr);
    // LOG_DEBUG("Checking Modloader code...", lr);

    // check hoshi code
    symbol_name = Mod_SearchForSymbol(stc_modloader_data->hoshi.mod_header, lr);
    if (symbol_name)
        return symbol_name;

    // check each installed mod
    for (int mod_idx = 0; mod_idx < stc_modloader_data->mod_num; mod_idx++)
    {
        ModHeader *mod_header = stc_modloader_data->mods[mod_idx].mod_header;

        // LOG_DEBUG("Checking %s code...", stc_modloader_data->mods[mod_idx].data.name);

        char *symbol_name = Mod_SearchForSymbol(mod_header, lr);

        if (symbol_name)
            return symbol_name;
    }

    // check dol
    if (stc_dol_debug && (u32)lr >= 0x80003100 && (u32)lr < 0x80535300) // hardcoded bounds, look into detecting regions from dol
    {
        // LOG_DEBUG("Checking dol code...");

        // binary search to find the code section the addr lies in
        int symbol_idx_min = 0;
        int symbol_idx_max = stc_dol_debug->symbol_num - 1;
        int symbol_idx;
        while (symbol_idx_min <= symbol_idx_max)
        {
            symbol_idx = symbol_idx_min + (symbol_idx_max - symbol_idx_min) / 2;

            u32 symbol_addr_start = stc_dol_debug->symbol[symbol_idx].code_offset_start;
            u32 symbol_addr_end = stc_dol_debug->symbol[symbol_idx].code_offset_end;

            if ((u32)lr >= symbol_addr_start && (u32)lr <= symbol_addr_end) // symbol found
                return stc_dol_debug->symbol[symbol_idx].symbol;            //
            else if ((u32)lr > symbol_addr_end)                             // symbol exists after this one
                symbol_idx_min = symbol_idx + 1;                            //
            else if ((u32)lr < symbol_addr_start)                           // symbol exists prior to this one
                symbol_idx_max = symbol_idx - 1;                            //
        }
    }

    return 0;
}

void Stack_ApplyPatches()
{
    CODEPATCH_REPLACEINSTRUCTION(0x8043ff60, 0x60000000); // skip setting exception handler
    CODEPATCH_REPLACEINSTRUCTION(0x8043f848, 0x48000268); // skip over pad check on crash
    CODEPATCH_REPLACEINSTRUCTION(0x8043fd58, 0x60000000); // skip unknown XFB call that gets stuck?

    CODEPATCH_REPLACEINSTRUCTION(0x8043fd40, 0x60000000); // skip osreport end so stack dump can add to it

    // inject code to log stack information
    CODEPATCH_HOOKAPPLY(0x8007d8b4);
    CODEPATCH_HOOKAPPLY(0x8007d790);

    // inject code to output stack information in newly created thread
    // (safe to load a file there)
    CODEPATCH_HOOKAPPLY(0x8043f844);

    return;
}
