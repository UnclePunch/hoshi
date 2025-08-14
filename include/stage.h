#ifndef KAR_H_STAGE
#define KAR_H_STAGE

#include "datatypes.h"
#include "obj.h"
#include "hurt.h"
#include "collision.h"

#define GRSTATECHANGE_NOANIM (1 << 2)

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

typedef struct YakumonoParam
{
    union
    {
        struct
        {
            int joint_idx;
            int x4;
            int x8;
            int xc;
            int x10;
            int start_anim_idx;    // 0x14
            int active_anim_idx;   // 0x18
            int end_anim_idx;      // 0x1c
            int inactive_anim_idx; // 0x20
        } *lighthouse;
        struct
        {
            int x0;
            int x4;
        } *other;
    };
} YakumonoParam;

typedef struct YakumonoData
{
    GOBJ *gobj;           // 0x0
    int kind;             // 0x4
    YakumonoParam *param; // 0x8
    u8 xc[0x68];          // 0xc
    int state;            // 0x74
} YakumonoData;

typedef struct GrModelMotion // exists in the stage file
{                            //
    struct
    {
        AnimJointDesc *anim_joint;
        MatAnimJointDesc *matanim_joint;
        int x8;
        int xc;
        int x10;
    } anim[];
} GrModelMotion; //

typedef struct GrData // exists in the stage file
{                     //
    int flags;        // 0x0
    struct
    {
        int x0;
        float machine_accel;
        float scale;
        float gravity_unk;
        Vec3 gravity_force;
        int fog_flags;
    } *stage_node;         // 0x4
    int x8;                // 0x8
    void *lights;          // 0xc
    GrModelMotion *motion; // 0x10, pointer placed at runtime
    void *spline;          // 0x14
    void *pos_data;        // 0x18
    int x1c;               // 0x1c
    int x20;               // 0x1c
    int x24;               // 0x1c
    int x28;               // 0x1c
    int x2c;               // 0x1c
    int x30;               // 0x1c
    int x34;               // 0x1c
    int x38;               // 0x1c
    int x3c;               // 0x1c
    struct
    {
        YakumonoParam *data;
        int num;
    } *yakumono; // 0x40
} GrData;        //

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

void Gr_StateChange(YakumonoData *yp, int state_idx, int anim_idx, int joint_idx, int flags, float start_frame, float anim_rate, float blend_rate);

#endif