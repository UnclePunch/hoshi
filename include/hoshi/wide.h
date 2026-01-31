#ifndef HOSHIWIDE_H
#define HOSHIWIDE_H

#include "datatypes.h"

// this is the width of the original perspective screen camera
#define ORIG_WIDTH (60.68)
#define ORIG_HEIGHT (48.36)
#define ORIG_ASPECT (1.255375)

typedef enum WideAlign
{
    WIDEALIGN_LEFT,
    WIDEALIGN_RIGHT,
} WideAlign;

// c - optional cobj arg. will reference this to determine width. otherwise will be relative to 4:3 fullscreen width.
float Hoshi_AdjustWidePos(int proj, WideAlign align, COBJ *c);
float Hoshi_GetWideMult();

#endif