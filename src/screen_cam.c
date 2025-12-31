/*---------------------------------------------------------------------------*
    Handles lowest level save file operations.
        - allocates runtime save struct
        - creates save file on memcard
        - loads save file from memcard
        - returns runtime save file struct
 *---------------------------------------------------------------------------*/

#include "os.h"
#include "obj.h"
#include "text.h"
#include "inline.h"
#include "hoshi.h"
#include "hoshi/screen_cam.h"
#include <string.h>

int canvas_idx;

void ScreenCam_Create()
{
    // create ortho text canvas
    int canvas_idx = Text_CreateCanvas(1, 0, 0, 0, 0, HOSHI_SCREENCAM_GXLINK, 0, 63);

    // // create ortho screen cam
    // GOBJ *g = GOBJ_EZCreator(0, 0, 0,
    //                 0, 0,
    //                 HSD_OBJKIND_COBJ, (COBJDesc *)0x805096a0,
    //                 0, 0,
    //                 0, HOSHI_SCREENCAM_GXLINK, 63);
    // COBJ *c = g->hsd_object;
    // CObj_SetOrtho(c, 0, -480, 0, 640);
}

int ScreenCam_GetCanvasIdx()
{
    return canvas_idx;
}

void ScreenCam_Init()
{
    CODEPATCH_REPLACEFUNC(Hoshi_GetScreenCanvasIndex, ScreenCam_GetCanvasIdx); // install function to get screen canvas idx 
}