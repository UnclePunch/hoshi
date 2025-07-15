#ifndef KAR_H_STAGE
#define KAR_H_STAGE

#include "datatypes.h"
#include "obj.h"
#include "hurt.h"
#include "collision.h"

typedef enum GroundKind
{
    GRKIND_CITY1 = 9,
    GRKIND_DRAG1,
    GRKIND_DRAG2,
    GRKIND_DRAG3,
    GRKIND_DRAG4,
    GRKIND_AIRGLIDER,
    GRKIND_TARGETFLIGHT,
    GRKIND_HIGHJUMP,
    GRKIND_KIRBYMELEE1,
    GRKIND_KIRBYMELEE2,
    GRKIND_DESTRUCTIONDERBY1,
    GRKIND_DESTRUCTIONDERBY2,
    GRKIND_DESTRUCTIONDERBY3,
    GRKIND_DESTRUCTIONDERBY4,
    GRKIND_DESTRUCTIONDERBY5,
    GRKIND_SINGLERACE1,
    GRKIND_SINGLERACE2,
    GRKIND_SINGLERACE3,
    GRKIND_SINGLERACE4,
    GRKIND_SINGLERACE5,
    GRKIND_SINGLERACE6,
    GRKIND_SINGLERACE7,
    GRKIND_SINGLERACE8,
    GRKIND_SINGLERACE9,
    GRKIND_VSKINGDEDEDE,
    GRKIND_NUM,
} GroundKind;

typedef enum StageKind
{
    STAGEKIND_NUM,
} StageKind;

typedef struct MapData
{
    int x0; // 0x0
} MapData;

typedef struct GrData // exists in the stage file
{                     //
    struct
    {
        int x0;
    } *stage_param;
} GrData; //

typedef struct GrObj
{
    GOBJ *gobj;         // 0x0
    GroundKind gr_kind; // 0x4
    GrData *gr_data;    // 0x8
} GrObj;

static GrData **stc_grdatalookup = (GrData **)(0x80557638); // indexed by gr_kind
static GrObj **stc_grobj = (GrObj **)(0x805dd0e0 + 0x5ec);

StageKind Gm_GetCurrentStageKind();
GroundKind Gm_GetCurrentGrKind();
GroundKind Gm_GetGrKindFromStageKind(StageKind stage_kind);

#endif