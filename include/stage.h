#ifndef KAR_H_STAGE
#define KAR_H_STAGE

#include "datatypes.h"
#include "audio.h"
#include "obj.h"
#include "hurt.h"
#include "collision.h"

#define GRSTATECHANGE_NOANIM (1 << 2)

typedef enum GroundKind
{
    GRKIND_NUM,
} GroundKind;

typedef enum StageKind
{
    STAGEKIND_CITY1 = 9,
    STAGEKIND_DRAG1,
    STAGEKIND_DRAG2,
    STAGEKIND_DRAG3,
    STAGEKIND_DRAG4,
    STAGEKIND_AIRGLIDER,
    STAGEKIND_TARGETFLIGHT,
    STAGEKIND_HIGHJUMP,
    STAGEKIND_KIRBYMELEE1,
    STAGEKIND_KIRBYMELEE2,
    STAGEKIND_DESTRUCTIONDERBY1,
    STAGEKIND_DESTRUCTIONDERBY2,
    STAGEKIND_DESTRUCTIONDERBY3,
    STAGEKIND_DESTRUCTIONDERBY4,
    STAGEKIND_DESTRUCTIONDERBY5,
    STAGEKIND_SINGLERACE1,
    STAGEKIND_SINGLERACE2,
    STAGEKIND_SINGLERACE3,
    STAGEKIND_SINGLERACE4,
    STAGEKIND_SINGLERACE5,
    STAGEKIND_SINGLERACE6,
    STAGEKIND_SINGLERACE7,
    STAGEKIND_SINGLERACE8,
    STAGEKIND_SINGLERACE9,
    STAGEKIND_VSKINGDEDEDE,
    STAGEKIND_CITYFREE = 52,
    STAGEKIND_NUM = 59,
} StageKind;

typedef struct YakumonoParam
{
    union
    {
        struct
        {
            struct  // 0x0
            {
                void *x0;
                int target_num;
            } *param;
            u8 x4[0x12c];           // 0x4
            void **x130;            // 0x130,
        } *break_weak;
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
    int x78;              // 0x78
    int x7c;              // 0x7c
} YakumonoData;

typedef struct GrFogData
{
    u8 x0[0x1c];    // 0x0
    int fog_kind;   // 0x1c
} GrFogData;

typedef struct GrFogDesc
{
    int transition_frames;  // 0x0
    GXColor fog_color;      // 0x4
    float fog_near;         // 0x8
    float fog_far;          // 0xc
    GXColor map_color;      // 0x10
    GXColor sky_color;      // 0x14
} GrFogDesc;

typedef struct GrModelMotionAnim
{
    AnimJointDesc *anim_joint;
    MatAnimJointDesc *matanim_joint;
    int x8;
    int xc;
    int x10;
} GrModelMotionAnim;

typedef struct GrModelMotion  // exists in the stage file
{                             //
    GrModelMotionAnim anim[]; // variable amount of these depending on the stage
} GrModelMotion;              //

typedef struct StageData
{
    GroundKind ground_kind;     // 0x0
    u32 bgm_idx;                // 0x4
    u32 ura_bgm_idx;            // 0x8
    u32 bgm_change;             // 0xc
    u32 x10;                    // 0x10
    void *x14;                  // 0x14
    u32 max_ply;                // 0x18
    u32 start_pos_idx;          // 0x20
    u32 enemy_pos_idx;          // 0x24
    u32 item_pos_idx;           // 0x28
    u32 machine_pos_idx;        // 0x2c
    u32 x30;                    // 0x30
    u32 has_item;               // 0x34
    u32 has_yaku;               // 0x38
    u32 x3c;                    // 0x3c
    void *x40;                  // 0x40
    u32 x44;                    // 0x44
    u32 x48;                    // 0x48
    u32 has_kirby;              // 0x4c, requests kirby dat files preload, used for city trial
    u32 has_dedede;             // 0x50, probably just for VS king dedede
    u32 has_kirby_walk;         // 0x54, requests RdKirbyMotionWalk.dat preload
} StageData;

typedef struct stData // exists in the Stage.dat file
{
    StageData *stage_data;      // array of these with STAGEKIND_NUM elements
} stData;

typedef struct GrData // exists in the stage's main dat file
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
    struct
    {
        struct
        {
            HSD_FogDesc *desc;           // 0x0
        } *data;
        struct
        {
            GrFogDesc *desc_arr; 
            int num;                // 0x4
        } *colors;              // 0x4
    } *fog;                // 0x34
    int x38;               // 0x38
    int x3c;               // 0x3c
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
    u8 xc[0x15c];       // 0xc
    GOBJ *fog_gobj;     // 0x168
} GrObj;
typedef struct StageDesc
{
    char *data_file;             // 0x0
    int x4;                     // 0x4
    char *model_file;           // 0x8
    int xc;                     // 0xc
    int x10;                    // 0x10
} StageDesc;

static StageDesc *stc_stagedesc = (StageDesc *)(0x804a2ffc);
static GrData **stc_grdatalookup = (GrData **)(0x80557638); // indexed by gr_kind
static GrObj **stc_grobj = (GrObj **)(0x805dd0e0 + 0x5ec);

StageKind Gm_GetCurrentStageKind();
StageKind Gm_GetCurrentStageKind2();
GroundKind Gm_GetGrKindFromStageKind(StageKind stage_kind);

void Gr_StateChange(YakumonoData *yp, int state_idx, int anim_idx, int joint_idx, int flags, float start_frame, float anim_rate, float blend_rate);

AudioEmitter Map_AllocAudioEmitter(int index);
#endif