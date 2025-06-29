#ifndef TEXTJOINT_H
#define TEXTJOINT_H

#include "obj.h"

#define TJ_DEBUGWIDTH 0

#define TEXTJOINT_LOOKUPNUM 64

typedef struct TextJointLookup
{
    int num;
    struct
    {
        Text *text;
        JOBJ *joint;
    } entry[TEXTJOINT_LOOKUPNUM];
} TextJointLookup;

void TextJoint_Init(int canvas_idx);
void TextJoint_Cleanup();
Text *TextJoint_Create(JOBJ *j, int child_idx, int canvas_idx, char *string, int align_kind, float width, float height, GXColor *color);
void TextJoint_Destroy(Text *text);
void TextJoint_Think();
void TextJoint_GX(GOBJ *g, int pass);
JOBJ *TextJoint_FindJointFromText(Text *t);
float TextJoint_GetWidth(Text *t);
#endif