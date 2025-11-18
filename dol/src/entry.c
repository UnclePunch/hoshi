#include "os.h"
#include "audio.h"
#include "hsd.h"
#include "reloc.h"

void **heap_lo_addr = (void **)(0x805dd0e0 + 0x11B0);

static char modloader_filename[] = "hoshi.bin";

void *AllocHeapLo(int size)
{
    void *alloc = *heap_lo_addr;
    (*heap_lo_addr) = (void *)((int)(*heap_lo_addr) + OSRoundUp32B(size));

    return alloc;
}
void FreeHeapLo(int size)
{
    void *alloc = *heap_lo_addr;
    (*heap_lo_addr) = (void *)((int)(*heap_lo_addr) - OSRoundUp32B(size));
}

void func()
{
    // set audio heap alloc and free callbacks (needed for file load callback)
    AudioHeap_SetAllocAndFree((void *)0x80059d28, (void *)0x80059d50);

    // ensure modloader file exists
    if (DVDConvertPathToEntrynum(modloader_filename) == -1)
    {
        OSReport("error: hoshi.bin not found!\n");
        return;
    }

    // load file
    ModHeader *buffer = AllocHeapLo(File_GetSize(modloader_filename));
    int out_size;
    File_LoadSync(modloader_filename, buffer, &out_size);

    // apply relocs
    reloc(buffer);

    // free relocs
    FreeHeapLo(buffer->relocs_num * sizeof(Reloc));

    // flush cache on the code
    void *code_ptr = (void*)((int)buffer + (int)buffer->code_offset);
    TRK_FlushCache(code_ptr, buffer->code_size);

    // get onBoot function from modloader
    void (*on_boot)(ModHeader *);
    get_func(buffer, (void **)&on_boot);

    // execute it
    on_boot(buffer);

    return;
}
