#include "inline.h"
#include "text.h"
#include "code_patch/code_patch.h"

#include "hoshi/log.h"

// Text Transparency
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

// Text Alignment
void Text_AlignSubtext(Text *t)
{
    static u8 align_cmds[] = {
        TEXTCMD_ALIGNLEFT,
        TEXTCMD_ALIGNCENTER,
        TEXTCMD_ALIGNRIGHT,
    };

    *(u8 *)t->alloc->next = align_cmds[t->align];                // write alignment command
    t->alloc->next = (TextHeapCell *)((u8 *)t->alloc->next + 1); // increment next pointer
}
CODEPATCH_HOOKCREATE(0x80450030, "mr 3, 28\n\t", Text_AlignSubtext, "", 0)
u8 *Text_SetTextAlign(Text *t, int idx)
{
    return Text_GetSubtext(t->text_start, idx);
}
CODEPATCH_HOOKCREATE(0x8045038c, "mr 3, 25\n\t"
                                 "mr 4, 12\n\t",
                     Text_SetTextAlign, "mr 7, 3\n\t", 0x804503d4)

// Text Commands in ASCII
int Text_CommandCheck(u8 *in, int *in_cur, u8 *out, int *out_cur)
{
    static u8 opcode_sizes[] = {
        -1,
        1,
        1,
        1,
        1,
        1,
        1,
        4,
        1,
        1,
        4,
        0,
        3,
        0,
        4,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
    };

    // get the next ascii character
    u8 val = in[*in_cur];

    // look for a text opcode in the ascii string
    if (val > TEXTCMD_TERMINATE && val < TEXTCMD_NUM)
    {
        int command_size = opcode_sizes[val];

        memcpy(&out[*out_cur], &in[*in_cur], command_size + 1);

        (*in_cur) += command_size;
        (*out_cur) += command_size + 1;
        return 1;
    }

    return 0;
}
CODEPATCH_HOOKCONDITIONALCREATE(0x8044fb54, "stwu	1, -40 (1)\n\t"
                                            "mflr 0\n\t"
                                            "stw 0, 44 (1)\n\t"
                                            "stw 3, 8 (1)\n\t"
                                            "stw 4, 12 (1)\n\t"
                                            "stw 5, 16 (1)\n\t"
                                            "stw 6, 20 (1)\n\t"
                                            "stw 7, 24 (1)\n\t"
                                            "stw 8, 28 (1)\n\t"
                                            "stw 9, 32 (1)\n\t"
                                            "stw 10, 36 (1)\n\t"
                                            "lwz 3, 12 (1)\n\t"    // in
                                            "addi 4, 1, 16\n\t"   // in cur
                                            "lwz 5, 8 (1)\n\t"    // out
                                            "addi 6, 1, 20\n\t",   // out_cur
                                            Text_CommandCheck, 
                                            "lwz 3, 8 (1)\n\t"
                                            "lwz 4, 12 (1)\n\t"
                                            "lwz 5, 16 (1)\n\t"
                                            "lwz 6, 20 (1)\n\t"
                                            "lwz 7, 24 (1)\n\t"
                                            "lwz 8, 28 (1)\n\t"
                                            "lwz 9, 32 (1)\n\t"
                                            "lwz 10, 36 (1)\n\t"
                                            "lwz 0, 44 (1)\n\t"
                                            "mtlr 0\n\t"
                                            "addi 1, 1, 40\n\t",
                                            0, 0x8044fea4)

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

// Text HSDObj
static HSD_ObjAllocData text_obj;
void TextHeap_Init()
{
    HSD_ObjAllocInit(&text_obj, 0xa0, 4);
}
CODEPATCH_HOOKCREATE(0x8044f634, "", TextHeap_Init, "", 0)
void *TextObj_Alloc(int size)
{
    return HSD_ObjAlloc(&text_obj);
}
void TextObj_Free(void *obj)
{
    HSD_ObjFree(&text_obj, obj);
}

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

    // subtext alignment
    CODEPATCH_REPLACEINSTRUCTION(0x8044ffa4, 0x38030000 | (17 + 1)); // add an extra byte for align command
    CODEPATCH_HOOKAPPLY(0x80450030);
    CODEPATCH_HOOKAPPLY(0x8045038c);
    CODEPATCH_REPLACEFUNC(0x80450774, Text_SetScale);

    // text ascii commands
    CODEPATCH_HOOKAPPLY(0x8044fb54);

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

    // Text HSDObj
    CODEPATCH_HOOKAPPLY(0x8044f634);
    CODEPATCH_REPLACECALL(0x8044f214, TextObj_Alloc);
    CODEPATCH_REPLACECALL(0x8044f0fc, TextObj_Free);
}
