#include "text.h"
#include "os.h"
#include "hsd.h"

#include "hoshi/wide.h"
#include "wide.h"

#include "code_patch/code_patch.h"

float Wide_GetMult()
{
    return (*stc_cobj_aspect) / ORIG_ASPECT;
}

float Wide_AdjustPos(int proj, WideAlign align, COBJ *c)
{
    if (_fabs((*stc_cobj_aspect) - ORIG_ASPECT) < 0.001f)
        return 0.0f;

    float adjust_ratio = ((*stc_cobj_aspect) - ORIG_ASPECT) / ORIG_ASPECT;
    float mult = (align == WIDEALIGN_LEFT) ? -1 : 1;

    switch (proj)
    {
        case PROJ_PERSPECTIVE:
        {
            float width_half;
            // if (c)
            // {
            //     CamBounds bounds;
            //     width_half = Wide_GetOriginalWidth(c) / 2;
            // }
            // else
                width_half = (ORIG_WIDTH / 2); // use precomputed value

            return width_half * adjust_ratio * mult;
        }
        case PROJ_ORTHO:
        {
            return adjust_ratio * (640/2) * mult;
        }
    }
}

void Wide_Init()
{
    CODEPATCH_REPLACEFUNC(Hoshi_GetWideMult, Wide_GetMult);
    CODEPATCH_REPLACEFUNC(Hoshi_AdjustWidePos, Wide_AdjustPos);
}