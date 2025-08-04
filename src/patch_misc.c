#include "inline.h"
#include "text.h"
#include "code_patch/code_patch.h"

#include "hoshi/log.h"

// Text
void Text_AlphaCopy(Text *t)
{
    t->temp.color.a = t->color.a;
    return;
}
void Text_AlphaDraw(Text *t)
{
    GXSetTevAlphaIn(1, 7, 7, 7, 1);
    GXSetTevColor(1, &t->temp.color);
    return;
}
CODEPATCH_HOOKCREATE(0x80451fe4, "mr 3, 27\n\t", Text_AlphaCopy, "", 0)
CODEPATCH_HOOKCREATE(0x804524c8, "mr 3, 27\n\t", Text_AlphaDraw, "", 0)

// MemAlloc Assert
void *MemAlloc_Error(void *addr, int size)
{
    if (!addr)
    {
        int heap_index = HSD_GetHeapID();
        LOG_ERROR("Could not alloc %.2fkb / %.2fkb from heap %d",
                  BytesToKB(size),
                  BytesToKB(OSCheckHeap(heap_index)),
                  heap_index);
    }

    return addr;
}
CODEPATCH_HOOKCREATE(0x804101e0, "mr 4, 31\n\t", MemAlloc_Error, "", 0)

// FGM Load
void *MainMenu_LoadFGM()
{
    // In vanilla, these were loading upon entering the title screen.
    // This worked because you had to pass through the title screen to
    // get to the game modes (which need the files), but we want to support
    // booting to the main menu so we're moving it here.

    // void *buffer = (void *)0x8056f280;
    // int buffer_size = OSRoundUp32B(0x8057b160 - 0x8056f280);
    // DCFlushRange(buffer, buffer_size);
    // DCInvalidateRange(buffer, buffer_size);
    FGM_LoadInGameBanks();
}
CODEPATCH_HOOKCREATE(0x80015bd8, "", MainMenu_LoadFGM, "", 0)

void Patches_Apply()
{
    // add text transparency functionality
    CODEPATCH_HOOKAPPLY(0x80451fe4);
    CODEPATCH_HOOKAPPLY(0x804524c8);

    // parts of the game disregard the alpha component of the text color so this updates it to 255
    static GXColor *text_colors = (GXColor *)0x805e62f8;
    for (int i = 0; i < 28; i++)
        text_colors[i].a = 255;

    // more....
    static GXColor *text_colors2 = (GXColor *)0x805e03ac;
    static u16 misc_colors_offsets[] = {
        (0x805e03ac - 0x805e03ac) / 4,
        (0x805e0420 - 0x805e03ac) / 4,
        (0x805e0460 - 0x805e03ac) / 4,
        (0x805e0480 - 0x805e03ac) / 4,
        (0x805e0490 - 0x805e03ac) / 4,
        (0x805e049c - 0x805e03ac) / 4,
        (0x805e04a8 - 0x805e03ac) / 4,
    };
    for (int i = 0; i < GetElementsIn(misc_colors_offsets); i++)
        text_colors2[misc_colors_offsets[i]].a = 255;

    // remove main menu input lockout
    CODEPATCH_REPLACEINSTRUCTION(0x80018278, 0x48000010);
    u8 *menu_mode_appear_timers = (u8 *)0x805d6670;
    for (int i = 0; i < 5; i++)
        menu_mode_appear_timers[i] /= 2;

    // remove pause input lockout
    CODEPATCH_REPLACEINSTRUCTION(0x80040ea8, 0x48000010);

    // enter city trial with 1p
    CODEPATCH_REPLACEINSTRUCTION(0x80037a04, 0x41800078);

    // MemAlloc
    CODEPATCH_HOOKAPPLY(0x804101e0);

    // FGM Load
    CODEPATCH_HOOKAPPLY(0x80015bd8);
}
