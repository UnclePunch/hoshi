#ifndef MEX_H_TEXT
#define MEX_H_TEXT

#include "inline.h"
#include "structs.h"
#include "datatypes.h"
#include "obj.h"
#include "gx.h"

/*** Structs ***/

typedef enum TextCmdOpcode
{
    TEXTCMD_TERMINATE,
    TEXTCMD_1,
    TEXTCMD_2,
    TEXTCMD_3,
    TEXTCMD_4,
    TEXTCMD_5,
    TEXTCMD_6,
    TEXTCMD_POS,
    TEXTCMD_8,
    TEXTCMD_9,
    TEXTCMD_10,
    TEXTCMD_11,
    TEXTCMD_COLOR,
    TEXTCMD_13,
    TEXTCMD_SCALE,
    TEXTCMD_15,
    TEXTCMD_NUM,
} TextCmdOpcode;

struct SISData
{
    u8 *image_data_arr;   // array of I4 image data for characters, stride is (32*32) / 2
    u8 *kerning_data_arr; // array of kerning data for characters, stride is 0x2, kerning is at 0x0
};

struct TextCanvas
{
    TextCanvas *next; // 0x0
    GOBJ *cam_gobj;   // 0x4,
    u16 size;         // 0x8 data remaining after this alloc?
    u16 sis_idx;      // 0xa sis group this canvas belongs to
    u8 p_link;        // 0xC
    u8 xd;            // 0xD
    u8 gx_link;       // 0xE
    u8 gx_pri;        // 0xF
};

struct Text
{
    Vec3 trans;                               // 0x0-0xC, affects entire viewport
    Vec2 aspect;                              // 0xC-0x14 CHANGE THIS TO BOUND, affects entire viewport
    float scissor_top;                        // 0x14, is_scissor must be enabled
    float scissor_bot;                        // 0x18, is_scissor must be enabled
    float scissor_left;                       // 0x1c, is_scissor must be enabled
    float scissor_right;                      // 0x20, is_scissor must be enabled
    Vec2 viewport_scale;                      // 0x24-0x2C, scales entire viewport including text (was orignally scale)
    GXColor viewport_color;                   // 0x2c, if the alpha value is != 0, uses an additional GXSetTevAlphaIn call @ 803a87f8 if enabled. background is the size of the aspect
    GXColor color;                            // 0x30
    Vec2 scale;                               // 0x34-0x3C scales just the text. (was originally stretch)
    float x3c;                                // 0x3c
    float x40;                                // 0x40
    u16 x44;                                  // 0x44
    u16 x46;                                  // 0x46
    u8 use_aspect;                            // 0x48, will fit the text to the bounds defined in aspect
    u8 kerning;                               // 0x49
    u8 align;                                 // 0x4a
    u8 x4b;                                   //
    u8 is_depth_compare;                      // 0x4C, if set to 1, updates the depth buffer when rendering, uses GX_LEQUAL (803a85a0)
    u8 hidden;                                // 0x4D
    u8 is_scissor;                            // 0x4E, crops the text within the bounding box 803a88e8
    u8 sis_id;                                // 0x4F, id of the premade text file to use
    void *x50;                                // some buffer alloc'd @ 803a5ba0
    GOBJ *gobj;                               // 0x54
    void (*render_callback)(GOBJ *text_gobj); // 0x58, read at 803a878c
    u8 *text_start;                           // 0x5C, start parsing text data at this ptr
    u8 *text_end;                             // 0x60, stops parsing text data at this ptr
    struct                                    //
    {                                         //
        int x0;                               //
        void *x4;                             //
        int size;                             // size of the text alloc
    } *allocInfo;                             // 0x64
    void *x68;                                // 0x68, some alloc used for dialogue?
    u16 x6c;                                  // 0x6c, flags of some kind
    u16 x6e;                                  // 0x6e
    struct
    {
        float x70;            // 0x70
        float x74;            // 0x74
        float x78;            // 0x78
        float x7c;            // 0x7c
        float x80;            // 0x80
        float x84;            // 0x84
        float x88;            // 0x88
        GXColor color;        // 0x8c, temp color storage when processing text opcodes. 803a8d7c
        u16 x90;              // 0x90
        u16 x92;              // 0x92
        int x94;              // 0x94
        int char_display_num; // 0x98, how many characters to display
        u8 use_aspect;        // 0x9c, use_aspect
        u8 kerning;           // 0x9d, kerning
        u8 align;             // 0x9e, align value, checked per character, updates the float @ x70
        u8 x9f;               // 0x9f
        int x100;             // 0x100
    } temp;
};

// Text Commands
typedef struct TextCmdPos
{
    u16 x;
    u16 y;
} TextCmdPos;
typedef struct TextCmdColor
{
    u8 r;
    u8 g;
    u8 b;
} TextCmdColor;
typedef struct TextCmdScale
{
    u16 x;
    u16 y;
} TextCmdScale;

static u8 *Text_GetSubtext(u8 *text_data, int idx)
{
    int cur_idx = 0;

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
    };

    while (1)
    {
        u8 opcode = text_data[0];

        if (opcode == 0)
            return 0;

        else if (opcode == 7)
        {
            if (cur_idx == idx)
                return text_data;

            cur_idx++;
        }

        if (opcode >= GetElementsIn(opcode_sizes))
            text_data++;
        else
            text_data += opcode_sizes[opcode];

        text_data++;
    }
}
static void Text_SetColor(Text *text, int idx, GXColor *col)
{

    u8 *subtext = Text_GetSubtext(text->text_start, idx);
    subtext[6] = col->r;
    subtext[7] = col->g;
    subtext[8] = col->b;
}
static void Text_SetScale(Text *text, int idx, float x, float y)
{
    u8 *subtext = Text_GetSubtext(text->text_start, idx);
    TextCmdScale *scale = (TextCmdScale *)&subtext[10];
    scale->x = x * 256;
    scale->y = y * 256;
}
static void Text_GetWidthAndHeight(Text *t, float *width, float *height)
{
    void _Text_DetermineWidthHeight(u8 * text_data, Text * text, float *width, float *height); // note: text_data must not start at the 7 opcode, skip past it by passing &text->text_start[5]

    // update temp variables that only the gx cb update
    t->temp.x78 = t->x3c;
    t->temp.x7c = t->x40;
    t->temp.use_aspect = t->use_aspect;
    t->temp.kerning = t->kerning;
    t->temp.align = t->align;

    _Text_DetermineWidthHeight(&t->text_start[5], t, width, height);
}

/*** Functions ***/
int Text_CreateCanvas(int sis_idx, int no_create_cam_gobj, int gobj_entityclass, int gobj_plink, int gobj_ppriority, int gxlink, int gxpri, int cobj_gxpri); // the optional gobj and cobj_gxlink are used to create a cobj as well. set gobj
Text *Text_CreateText(int sis_idx, int canvas_idx);
Text *Text_CreateTextManual(int sis_idx, int canvas_idx, float pos_x, float pos_y, float pos_z, float limit_x, float limit_y);
void Text_Destroy(Text *text);
int Text_AddSubtext(Text *text, float xPos, float yPos, char *string, ...);
void Text_SetScale(Text *text, int subtext, float x, float y);
// void Text_SetColor(Text *text, int subtext, GXColor *color);
void Text_SetPosition(Text *text, int subtext, float x, float y);
void Text_SetText(Text *text, int subtext, char *string, ...);
u8 *Text_Alloc(int size);
void Text_Free(u8 *alloc);
void Text_DestroyAlloc(u8 *alloc);
void Text_DestroyAllAlloc(Text *text);
int Text_ConvertASCIIToShiftJIS(char *out, char *in);
void Text_GX(GOBJ *gobj, int pass);
void Text_LoadSisFile(int index, char *filename, char *symbol);
int Text_SetSisText(Text *text, int text_index);
void Text_FinalizeSisText(Text *text, int text_index);
void Text_DestroyAllSisCanvas(int sis_id);
void Text_DestroyCanvas(TextCanvas *);
void Text_InitSisHeap(int size);
void Text_DestroySisHeap();

/*** Variables ***/
// Text data
static int *stc_textheap_size = (int *)0x805de558;
static TextCanvas **stc_textheap_start = (TextCanvas **)0x805de55c;
static TextCanvas **stc_textheap_next = (TextCanvas **)0x805de560;

// Text GObj's data
static Text **stc_text_first = (Text **)0x805de568;

// Text canvas
static TextCanvas **stc_textcanvas_first = (TextCanvas **)0x805de56c;

// Sis Library
static HSD_Archive **stc_sis_archives = (HSD_Archive **)0x00000000; // array of sis file archive pointers
static SISData **stc_sis_data = (SISData **)0x00000000;             // array of currently loaded sis data, indexed by sis_id

#endif
