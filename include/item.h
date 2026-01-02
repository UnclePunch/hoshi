#ifndef MEX_H_ITEM
#define MEX_H_ITEM

#include "structs.h"
#include "datatypes.h"
#include "os.h"
#include "trigger.h"

typedef enum ItemPri
{
    ITPRI_0,
    ITPRI_ANIM,
    ITPRI_PHYS = 4,
    ITPRI_ENVCOLL,
    ITPRI_6,
    ITPRI_TRIGGER, // collect powerup collision, also frees sounds
    ITPRI_8,
    ITPRI_HITCOLL,
    ITPRI_DMGAPPLY,
    ITPRI_13 = 13,
    ITPRI_15 = 15,
} ItemPri;


typedef enum BoxKind
{
    BOXKIND_ALL = -1,
    BOXKIND_BLUE,
    BOXKIND_GREEN,
    BOXKIND_RED,
    BOXKIND_NUM,
} BoxKind;

typedef enum BoxSize
{
    BOXSIZE_SMALL,  // spawns 1 item @ 80250bcc
    BOXSIZE_MEDIUM, // spawns 2 items @ 80250bb4
    BOXSIZE_LARGE,  // spawns 4 item @ 80250bc4
} BoxSize;

typedef enum ItemType
{
    ITTYPE_NULL = -1, //
    ITTYPE_PATCH,     //
    ITTYPE_QUICKFIX,  //
    ITTYPE_COPY,      //
    ITTYPE_NUM,       //
} ItemType;

typedef enum ItemGroup
{
    ITGROUP_ALL = -1, //
    ITGROUP_BAD,      //
    ITGROUP_GOOD,     //
    ITGROUP_FAKE,     //
    ITGROUP_NUM,      //
} ItemGroup;

typedef enum ItemKind
{
    ITKIND_BOXBLUE,
    ITKIND_BOXGREEN,
    ITKIND_BOXRED,
    ITKIND_ACCEL,
    ITKIND_ACCELDOWN,
    ITKIND_TOPSPEED,
    ITKIND_TOPSPEEDDOWN,
    ITKIND_OFFENSE,
    ITKIND_OFFENSEDOWN,
    ITKIND_DEFENSE,
    ITKIND_DEFENSEDOWN,
    ITKIND_TURN,
    ITKIND_TURNDOWN,
    ITKIND_GLIDE,
    ITKIND_GLIDEDOWN,
    ITKIND_CHARGE,
    ITKIND_CHARGEDOWN,
    ITKIND_WEIGHT,
    ITKIND_WEIGHTDOWN,
    ITKIND_HP,
    ITKIND_ALLUP,
    ITKIND_SPEEDMAX,
    ITKIND_SPEEDMIN,
    ITKIND_OFFENSEMAX,
    ITKIND_DEFENSEMAX,
    ITKIND_CHARGEMAX,
    ITKIND_CHARGENONE,
    ITKIND_CANDY,
    ITKIND_COPYBOMB,
    ITKIND_COPYFIRE,
    ITKIND_COPYICE,
    ITKIND_COPYSLEEP,
    ITKIND_COPYTIRE,
    ITKIND_COPYBIRD,
    ITKIND_COPYPLASMA,
    ITKIND_COPYTORNADO,
    ITKIND_COPYSWORD,
    ITKIND_COPYSPIKE,
    ITKIND_COPYMIC,
    ITKIND_FOODMAXIMTOMATO,
    ITKIND_FOODENERGYDRINK,
    ITKIND_FOODICECREAM,
    ITKIND_FOODRICEBALL,
    ITKIND_FOODCHICKEN,
    ITKIND_FOODCURRY,
    ITKIND_FOODRAMEN,
    ITKIND_FOODOMELET,
    ITKIND_FOODHAMBURGER,
    ITKIND_FOODSUSHI,
    ITKIND_FOODHOTDOG,
    ITKIND_FOODAPPLE,
    ITKIND_FIREWORKS,
    ITKIND_PANICSPIN,
    ITKIND_TIMEBOMB,
    ITKIND_GORDO,
    ITKIND_HYDRA1,
    ITKIND_HYDRA2,
    ITKIND_HYDRA3,
    ITKIND_DRAGOON1,
    ITKIND_DRAGOON2,
    ITKIND_DRAGOON3,
    ITKIND_ACCELFAKE,
    ITKIND_TOPSPEEDFAKE,
    ITKIND_OFFENSEFAKE,
    ITKIND_DEFENSEFAKE,
    ITKIND_TURNFAKE,
    ITKIND_GLIDEFAKE,
    ITKIND_CHARGEFAKE,
    ITKIND_WEIGHTFAKE,
    ITKIND_NUM,
} ItemKind;

typedef struct ItemCommonAttr
{
    int x0;
    int x4;
    int x8;
    int xc;
    int x10;
    int x14;
    int x18;
    BoxKind box_kind; // 0x1c, type of box this item spawns from
    int x20;
    int x24;
    struct
    {
        int x0;
        int x4;
        ItemGroup group;
    } *x28;
} ItemCommonAttr;

typedef struct itData
{
    ItemCommonAttr *attr;       // 0x0
    struct
    {
        int x0;
    } *unique_attr;             // 0x4
    struct
    {
        JOBJ *j;
        int x4;
    } *model;                   // 0x8
    struct
    {
        AnimJointDesc *joint_anim;
        MatAnimJointDesc *mat_anim;
        void *script;
        int joint_num;
    } *anim_data;               // 0xc
    struct
    {
        HurtDesc *desc;         // 0x0
        int num;                // 0x4
    } *hurt;                    // 0x10
    TriggerDesc *trigger;       // 0x14
} itData;


typedef struct ItemCommonParam
{
    float scale;           // 0x0
    float x4;              // 0x4
    float x8;              // 0x8
    float shadow_scale;    // 0xc
    float x10;             // 0x10
    float x14;             // 0x14
    float x18;             // 0x18
    float x1c;             // 0x1c
    int x20;               // 0x20
    int lifetime_min;      // 0x24
    int lifetime_variance; // 0x28, added onto lifetime_min
    int x2c;               // 0x2c
    int x30;               // 0x30
    int x34;               // 0x34
    int x38;               // 0x38
    int x3c;               // 0x3c
    int x40;               // 0x40
    int x44;                // 0x44
    int x48;               // 0x48
} ItemCommonParam;

typedef struct itCommonDataAll
{
    ItemCommonParam *param;
    struct
    {
        float x0;
    } *x4;              // placed at runtime
    itData *itData;     // placed at runtime
} itCommonDataAll;

typedef struct ItemFallDesc
{
    float match_progress; // time
    int item_max;         // maximum amount of items present
    int spawn_time_min;   // min
    int spawn_time_max;   // max
} ItemFallDesc;

typedef struct ItemData
{
    int x0;                     // 0x0
    int x4;                     // 0x4
    int x8;                     // 0x8
    int xc;                     // 0xc
    int x10;                    // 0x10
    int x14;                    // 0x14
    GOBJ *shadow_gobj;          // 0x18
    ItemKind kind;              // 0x1c
    int x20;                    // 0x20
    int unk_group;              // 0x24, is referenced when defining shadow size. non boxes use a smaller shadow size in ItCommon
    JOBJDesc *jobjdesc;         // 0x28
    itData *itData;             // 0x2c
    int exist_index;            // 0x30, this item is the nth to exist
    int x34;                    // 0x34
    int x38;                    // 0x38
    int x3c;                    // 0x3c
    int x40;                    // 0x40
    int lifetime;               // 0x44
    int x48;                    // 0x48
    int state;                  // 0x4c
    int x50;                    // 0x50
    int x54;                    // 0x54
    int x58;                    // 0x58
    int x5c;                    // 0x5c
    int x60;                    // 0x60
    int x64;                    // 0x64
    float state_frame;          // 0x68
    int x6c;                    // 0x6c
    int x70;                    // 0x70
    int x74;                    // 0x74
    int x78;                    // 0x78
    int x7c;                    // 0x7c
    int x80;                    // 0x80
    int x84;                    // 0x84
    int x88;                    // 0x88
    int x8c;                    // 0x8c
    int x90;                    // 0x90
    int x94;                    // 0x94
    int x98;                    // 0x98
    int x9c;                    // 0x9c
    int xa0;                    // 0xa0
    int xa4;                    // 0xa4
    int xa8;                    // 0xa8
    float scale;                // 0xac
    float alpha;                // 0xb0
    float alpha_addend;         // 0xb4, is added to alpha each frame, for box at least 
    Vec3 xb8;                   // 0xb8, queued velocity? gets added to vel
    Vec3 vel;                   // 0xc4
    Vec3 xd0;                   // 0xd0
    Vec3 pos;                   // 0xdc
    Vec3 xe8;                   // 0xe8
    int xf4;                    // 0xf4
    int xf8;                    // 0xf8
    int xfc;                    // 0xfc
    Vec3 x100;                  // 0x100
    int x10c;                   // 0x10c
    int x110;                   // 0x110
    int x114;                   // 0x114
    int x118;                   // 0x118
    int x11c;                   // 0x11c
    int x120;                   // 0x120
    int x124;                   // 0x124
    int x128;                   // 0x128
    int x12c;                   // 0x12c
    int x130;                   // 0x130
    int x134;                   // 0x134
    Vec3 down;                  // 0x138, like an up vector but down, points in the direction of gravity?
    int x144;                   // 0x144
    int x148;                   // 0x148
    int x14c;                   // 0x14c
    int x150;                   // 0x150
    int x154;                   // 0x154
    int x158;                   // 0x158
    int x15c;                   // 0x15c
    int x160;                   // 0x160
    int x164;                   // 0x164
    int x168;                   // 0x168
    int x16c;                   // 0x16c
    int x170;                   // 0x170
    int x174;                   // 0x174
    int x178;                   // 0x178
    int x17c;                   // 0x17c
    int x180;                   // 0x180
    int x184;                   // 0x184
    int x188;                   // 0x188
    int x18c;                   // 0x18c
    int x190;                   // 0x190
    int x194;                   // 0x194
    int x198;                   // 0x198
    int x19c;                   // 0x19c
    int x1a0;                   // 0x1a0
    CollData *coll_data;        // 0x1a4, if 0, game uses raycasting?
    struct                      // 0x1a8, items use point collision
    {
        int raycast_idx;        // 0x1a8, this id represents when the raycast was performed?
        Vec3 land_pos;          // 0x1ac, calculated ahead of time once
    } point_coll;               //
    int x1b8;                   // 0x1b8
    int x1bc;                   // 0x1bc
    int x1c0;                   // 0x1c0
    float x1c4;                 // 0x1c4, is the value returned by 800ceb18
    Vec3 x1c8;                  // 0x1c8, multiplied with queued velocity @ 0xb8 when a box is landing. another down vector?
    int x1d4;                   // 0x1d4
    int x1d8;                   // 0x1d8
    int x1dc;                   // 0x1dc
    int x1e0;                   // 0x1e0
    int x1e4;                   // 0x1e4
    int x1e8;                   // 0x1e8
    int x1ec;                   // 0x1ec
    int x1f0;                   // 0x1f0
    int x1f4;                   // 0x1f4
    int x1f8;                   // 0x1f8
    int x1fc;                   // 0x1fc
    int x200;                   // 0x200
    int x204;                   // 0x204
    int x208;                   // 0x208
    int x20c;                   // 0x20c
    int x210;                   // 0x210
    int x214;                   // 0x214
    int x218;                   // 0x218
    int x21c;                   // 0x21c
    int x220;                   // 0x220
    int x224;                   // 0x224
    int x228;                   // 0x228
    int x22c;                   // 0x22c
    int x230;                   // 0x230
    int x234;                   // 0x234
    int x238;                   // 0x238
    int x23c;                   // 0x23c
    int audio_prox;             // 0x240, 8005de3c returns this.
    int audio_track;            // 0x244, stored @ 80256b5c when bouncing item sound plays
    int audio_unk;              // 0x248
    int bounce_num;             // 0x24c, incremented when bouncing @ 80255a70
    TriggerData trigger;        // 0x250
    int x2b0;                   // 0x2b0
    int x2b4;                   // 0x2b4
    int x2b8;                   // 0x2b8
    int x2bc;                   // 0x2bc
    int x2c0;                   // 0x2c0
    int x2c4;                   // 0x2c4
    int x2c8;                   // 0x2c8
    int x2cc;                   // 0x2cc
    int x2d0;                   // 0x2d0
    int x2d4;                   // 0x2d4
    int x2d8;                   // 0x2d8
    int x2dc;                   // 0x2dc
    int x2e0;                   // 0x2e0
    int x2e4;                   // 0x2e4
    int x2e8;                   // 0x2e8
    int x2ec;                   // 0x2ec
    int x2f0;                   // 0x2f0
    int x2f4;                   // 0x2f4
    int x2f8;                   // 0x2f8
    int x2fc;                   // 0x2fc
    int x300;                   // 0x300
    int x304;                   // 0x304
    int x308;                   // 0x308
    int x30c;                   // 0x30c
    int x310;                   // 0x310
    int x314;                   // 0x314
    int x318;                   // 0x318
    int x31c;                   // 0x31c
    int x320;                   // 0x320
    int x324;                   // 0x324
    int x328;                   // 0x328
    int x32c;                   // 0x32c
    int x330;                   // 0x330
    int x334;                   // 0x334
    int x338;                   // 0x338
    int x33c;                   // 0x33c
    int x340;                   // 0x340
    int x344;                   // 0x344
    int x348;                   // 0x348
    int x34c;                   // 0x34c
    int x350;                   // 0x350
    int x354;                   // 0x354
    int x358;                   // 0x358
    int x35c;                   // 0x35c
    int x360;                   // 0x360
    int x364;                   // 0x364
    int x368;                   // 0x368
    int x36c;                   // 0x36c
    int x370;                   // 0x370
    int x374;                   // 0x374
    int x378;                   // 0x378
    int x37c;                   // 0x37c
    int x380;                   // 0x380
    int x384;                   // 0x384
    int x388;                   // 0x388
    int x38c;                   // 0x38c
    int x390;                   // 0x390
    u16 time_seconds;           // 0x394
    u8 stadium_kind;            // 0x396
    u8 game_tempo : 2;          // 0x397, 0xC0 (1 = normal = 2 is slow)
    u8 events_enable : 1;       // 0x397, 0x20
    u8 x397_x10 : 1;            // 0x397, 0x10
    u8 x397_x08 : 1;            // 0x397, 0x08
    u8 x397_x04 : 1;            // 0x397, 0x04
    u8 x397_x02 : 1;            // 0x397, 0x02
    u8 x397_x01 : 1;            // 0x397, 0x01
    int x398;                   // 0x398
    int x39c;                   // 0x39c
    int x3a0;                   // 0x3a0
    int x3a4;                   // 0x3a4
    int x3a8;                   // 0x3a8
    int x3ac;                   // 0x3ac
    int x3b0;                   // 0x3b0
    int x3b4;                   // 0x3b4
    int x3b8;                   // 0x3b8
    int x3bc;                   // 0x3bc
    int x3c0;                   // 0x3c0
    int x3c4;                   // 0x3c4
    int x3c8;                   // 0x3c8
    int x3cc;                   // 0x3cc
    int x3d0;                   // 0x3d0
    int x3d4;                   // 0x3d4
    int x3d8;                   // 0x3d8
    int x3dc;                   // 0x3dc
    int x3e0;                   // 0x3e0
    int x3e4;                   // 0x3e4
    int x3e8;                   // 0x3e8
    int x3ec;                   // 0x3ec
    int x3f0;                   // 0x3f0
    int x3f4;                   // 0x3f4
    int x3f8;                   // 0x3f8
    int x3fc;                   // 0x3fc
    int x400;                   // 0x400
    int x404;                   // 0x404
    int x408;                   // 0x408
    int x40c;                   // 0x40c
    int x410;                   // 0x410
    int x414;                   // 0x414
    int x418;                   // 0x418
    int x41c;                   // 0x41c
    int x420;                   // 0x420
    int x424;                   // 0x424
    int x428;                   // 0x428
    int x42c;                   // 0x42c
    int x430;                   // 0x430
    int x434;                   // 0x434
    int x438;                   // 0x438
    int x43c;                   // 0x43c
    int x440;                   // 0x440
    int x444;                   // 0x444
    int x448;                   // 0x448
    int x44c;                   // 0x44c
    int x450;                   // 0x450
    int x454;                   // 0x454
    int x458;                   // 0x458
    int x45c;                   // 0x45c
    int x460;                   // 0x460
    int x464;                   // 0x464
    int x468;                   // 0x468
    int x46c;                   // 0x46c
    int x470;                   // 0x470
    int x474;                   // 0x474
    int x478;                   // 0x478
    int x47c;                   // 0x47c
    int x480;                   // 0x480
    int x484;                   // 0x484
    int x488;                   // 0x488
    int x48c;                   // 0x48c
    int x490;                   // 0x490
    int x494;                   // 0x494
    int x498;                   // 0x498
    int x49c;                   // 0x49c
    int x4a0;                   // 0x4a0
    int x4a4;                   // 0x4a4
    int x4a8;                   // 0x4a8
    int x4ac;                   // 0x4ac
    int x4b0;                   // 0x4b0
    int x4b4;                   // 0x4b4
    int x4b8;                   // 0x4b8
    int x4bc;                   // 0x4bc
    int x4c0;                   // 0x4c0
    int x4c4;                   // 0x4c4
    int x4c8;                   // 0x4c8
    int x4cc;                   // 0x4cc
    int x4d0;                   // 0x4d0
    int x4d4;                   // 0x4d4
    int x4d8;                   // 0x4d8
    int x4dc;                   // 0x4dc
    int x4e0;                   // 0x4e0
    int x4e4;                   // 0x4e4
    int x4e8;                   // 0x4e8
    int x4ec;                   // 0x4ec
    int x4f0;                   // 0x4f0
    int x4f4;                   // 0x4f4
    int x4f8;                   // 0x4f8
    int x4fc;                   // 0x4fc
    int x500;                   // 0x500
    int x504;                   // 0x504
    int x508;                   // 0x508
    int x50c;                   // 0x50c
    int x510;                   // 0x510
    int x514;                   // 0x514
    int x518;                   // 0x518
    int x51c;                   // 0x51c
    int x520;                   // 0x520
    int x524;                   // 0x524
    int x528;                   // 0x528
    int x52c;                   // 0x52c
    int x530;                   // 0x530
    int x534;                   // 0x534
    int x538;                   // 0x538
    int x53c;                   // 0x53c
    int x540;                   // 0x540
    int x544;                   // 0x544
    int x548;                   // 0x548
    int x54c;                   // 0x54c
    int x550;                   // 0x550
    int x554;                   // 0x554
    int x558;                   // 0x558
    int x55c;                   // 0x55c
    int x560;                   // 0x560
    int x564;                   // 0x564
    int x568;                   // 0x568
    int x56c;                   // 0x56c
    int x570;                   // 0x570
    int x574;                   // 0x574
    int x578;                   // 0x578
    int x57c;                   // 0x57c
    int x580;                   // 0x580
    int x584;                   // 0x584
    int x588;                   // 0x588
    int x58c;                   // 0x58c
    int x590;                   // 0x590
    int x594;                   // 0x594
    int x598;                   // 0x598
    int x59c;                   // 0x59c
    int x5a0;                   // 0x5a0
    int x5a4;                   // 0x5a4
    int x5a8;                   // 0x5a8
    int x5ac;                   // 0x5ac
    int x5b0;                   // 0x5b0
    int x5b4;                   // 0x5b4
    int x5b8;                   // 0x5b8
    int x5bc;                   // 0x5bc
    int x5c0;                   // 0x5c0
    int x5c4;                   // 0x5c4
    int x5c8;                   // 0x5c8
    int x5cc;                   // 0x5cc
    int x5d0;                   // 0x5d0
    int x5d4;                   // 0x5d4
    int x5d8;                   // 0x5d8
    int x5dc;                   // 0x5dc
    int x5e0;                   // 0x5e0
    int x5e4;                   // 0x5e4
    int x5e8;                   // 0x5e8
    int x5ec;                   // 0x5ec
    int x5f0;                   // 0x5f0
    int x5f4;                   // 0x5f4
    int x5f8;                   // 0x5f8
    int x5fc;                   // 0x5fc
    int x600;                   // 0x600
    int x604;                   // 0x604
    int x608;                   // 0x608
    int x60c;                   // 0x60c
    int x610;                   // 0x610
    int x614;                   // 0x614
    int x618;                   // 0x618
    int x61c;                   // 0x61c
    int x620;                   // 0x620
    int x624;                   // 0x624
    int x628;                   // 0x628
    int x62c;                   // 0x62c
    int x630;                   // 0x630
    int x634;                   // 0x634
    int x638;                   // 0x638
    int x63c;                   // 0x63c
    int x640;                   // 0x640
    int x644;                   // 0x644
    int x648;                   // 0x648
    int x64c;                   // 0x64c
    int x650;                   // 0x650
    int x654;                   // 0x654
    int x658;                   // 0x658
    int x65c;                   // 0x65c
    int x660;                   // 0x660
    int x664;                   // 0x664
    int x668;                   // 0x668
    int x66c;                   // 0x66c
    int x670;                   // 0x670
    int x674;                   // 0x674
    int x678;                   // 0x678
    int x67c;                   // 0x67c
    int x680;                   // 0x680
    int x684;                   // 0x684
    int x688;                   // 0x688
    int x68c;                   // 0x68c
    int x690;                   // 0x690
    int x694;                   // 0x694
    int x698;                   // 0x698
    int x69c;                   // 0x69c
    int x6a0;                   // 0x6a0
    int x6a4;                   // 0x6a4
    int x6a8;                   // 0x6a8
    int x6ac;                   // 0x6ac
    int x6b0;                   // 0x6b0
    int x6b4;                   // 0x6b4
    int x6b8;                   // 0x6b8
    int x6bc;                   // 0x6bc
    int x6c0;                   // 0x6c0
    int x6c4;                   // 0x6c4
    int x6c8;                   // 0x6c8
    int x6cc;                   // 0x6cc
    int x6d0;                   // 0x6d0
    int x6d4;                   // 0x6d4
    int x6d8;                   // 0x6d8
    int x6dc;                   // 0x6dc
    int x6e0;                   // 0x6e0
    int x6e4;                   // 0x6e4
    int x6e8;                   // 0x6e8
    int x6ec;                   // 0x6ec
    int x6f0;                   // 0x6f0
    int x6f4;                   // 0x6f4
    int x6f8;                   // 0x6f8
    int x6fc;                   // 0x6fc
    int x700;                   // 0x700
    int x704;                   // 0x704
    int x708;                   // 0x708
    int x70c;                   // 0x70c
    int x710;                   // 0x710
    int x714;                   // 0x714
    int x718;                   // 0x718
    int x71c;                   // 0x71c
    int x720;                   // 0x720
    int x724;                   // 0x724
    int x728;                   // 0x728
    int x72c;                   // 0x72c
    int x730;                   // 0x730
    int x734;                   // 0x734
    int x738;                   // 0x738
    int x73c;                   // 0x73c
    int x740;                   // 0x740
    int x744;                   // 0x744
    int x748;                   // 0x748
    int x74c;                   // 0x74c
    int x750;                   // 0x750
    int x754;                   // 0x754
    int x758;                   // 0x758
    int x75c;                   // 0x75c
    int x760;                   // 0x760
    int x764;                   // 0x764
    int x768;                   // 0x768
    int x76c;                   // 0x76c
    int x770;                   // 0x770
    int x774;                   // 0x774
    int x778;                   // 0x778
    int x77c;                   // 0x77c
    int x780;                   // 0x780
    int x784;                   // 0x784
    int x788;                   // 0x788
    int x78c;                   // 0x78c
    int x790;                   // 0x790
    int x794;                   // 0x794
    int x798;                   // 0x798
    int x79c;                   // 0x79c
    int x7a0;                   // 0x7a0
    int x7a4;                   // 0x7a4
    int x7a8;                   // 0x7a8
    int x7ac;                   // 0x7ac
    int x7b0;                   // 0x7b0
    int x7b4;                   // 0x7b4
    int x7b8;                   // 0x7b8
    int x7bc;                   // 0x7bc
    int x7c0;                   // 0x7c0
    int x7c4;                   // 0x7c4
    int x7c8;                   // 0x7c8
    int x7cc;                   // 0x7cc
    int x7d0;                   // 0x7d0
    int x7d4;                   // 0x7d4
    int x7d8;                   // 0x7d8
    int x7dc;                   // 0x7dc
    int x7e0;                   // 0x7e0
    int x7e4;                   // 0x7e4
    int x7e8;                   // 0x7e8
    int x7ec;                   // 0x7ec
    int x7f0;                   // 0x7f0
    int x7f4;                   // 0x7f4
    int x7f8;                   // 0x7f8
    int x7fc;                   // 0x7fc
    int x800;                   // 0x800
    int x804;                   // 0x804
    int x808;                   // 0x808
    int x80c;                   // 0x80c
    int x810;                   // 0x810
    int x814;                   // 0x814
    int x818;                   // 0x818
    int x81c;                   // 0x81c
    int x820;                   // 0x820
    int x824;                   // 0x824
    int x828;                   // 0x828
    int x82c;                   // 0x82c
    u8 x830;                    // 0x830
    u8 x831;                    // 0x831
    u8 x832;                    // 0x832
    u8 frames_in_second;        // 0x833
    int seconds_passed;         // 0x834
    int x838;                   // 0x838
    int x83c;                   // 0x83c
    int x840;                   // 0x840
    int x844;                   // 0x844
    int x848;                   // 0x848
    int x84c;                   // 0x84c
    int x850;                   // 0x850
    int x854;                   // 0x854
    int x858;                   // 0x858
    int x85c;                   // 0x85c
    int x860;                   // 0x860
    int x864;                   // 0x864
    int x868;                   // 0x868
    int x86c;                   // 0x86c
    int x870;                   // 0x870
    int x874;                   // 0x874
    int x878;                   // 0x878
    int x87c;                   // 0x87c
    int x880;                   // 0x880
    int x884;                   // 0x884
    int x888;                   // 0x888
    int x88c;                   // 0x88c
    int x890;                   // 0x890
    int x894;                   // 0x894
    int x898;                   // 0x898
    int x89c;                   // 0x89c
    int x8a0;                   // 0x8a0
    int x8a4;                   // 0x8a4
    int x8a8;                   // 0x8a8
    int x8ac;                   // 0x8ac
    int x8b0;                   // 0x8b0
    int x8b4;                   // 0x8b4
    int x8b8;                   // 0x8b8
    int x8bc;                   // 0x8bc
    int x8c0;                   // 0x8c0
    int x8c4;                   // 0x8c4
    int x8c8;                   // 0x8c8
    int x8cc;                   // 0x8cc
    int x8d0;                   // 0x8d0
    int x8d4;                   // 0x8d4
    int x8d8;                   // 0x8d8
    int x8dc;                   // 0x8dc
    int x8e0;                   // 0x8e0
    int x8e4;                   // 0x8e4
    int x8e8;                   // 0x8e8
    int x8ec;                   // 0x8ec
    int x8f0;                   // 0x8f0
    int x8f4;                   // 0x8f4
    int x8f8;                   // 0x8f8
    int x8fc;                   // 0x8fc
    int x900;                   // 0x900
    int x904;                   // 0x904
    int x908;                   // 0x908
    int x90c;                   // 0x90c
    int x910;                   // 0x910
    int x914;                   // 0x914
    int x918;                   // 0x918
    int x91c;                   // 0x91c
    int x920;                   // 0x920
    int x924;                   // 0x924
    int x928;                   // 0x928
    int x92c;                   // 0x92c
    int x930;                   // 0x930
    int x934;                   // 0x934
    int x938;                   // 0x938
    int x93c;                   // 0x93c
    int x940;                   // 0x940
    int x944;                   // 0x944
    int x948;                   // 0x948
    int x94c;                   // 0x94c
    int x950;                   // 0x950
    int x954;                   // 0x954
    int x958;                   // 0x958
    int x95c;                   // 0x95c
    int x960;                   // 0x960
    int x964;                   // 0x964
    int x968;                   // 0x968
    int x96c;                   // 0x96c
    int x970;                   // 0x970
    int x974;                   // 0x974
    int x978;                   // 0x978
    int x97c;                   // 0x97c
    int x980;                   // 0x980
    int x984;                   // 0x984
    int x988;                   // 0x988
    int x98c;                   // 0x98c
    int x990;                   // 0x990
    int x994;                   // 0x994
    int x998;                   // 0x998
    int x99c;                   // 0x99c
    int x9a0;                   // 0x9a0
    int x9a4;                   // 0x9a4
    int x9a8;                   // 0x9a8
    int x9ac;                   // 0x9ac
    int x9b0;                   // 0x9b0
    int x9b4;                   // 0x9b4
    int x9b8;                   // 0x9b8
    int x9bc;                   // 0x9bc
    int x9c0;                   // 0x9c0
    int x9c4;                   // 0x9c4
    int x9c8;                   // 0x9c8
    int x9cc;                   // 0x9cc
    int x9d0;                   // 0x9d0
    int x9d4;                   // 0x9d4
    int x9d8;                   // 0x9d8
    int x9dc;                   // 0x9dc
    int x9e0;                   // 0x9e0
    int x9e4;                   // 0x9e4
    int x9e8;                   // 0x9e8
    int x9ec;                   // 0x9ec
    int x9f0;                   // 0x9f0
    int x9f4;                   // 0x9f4
    int x9f8;                   // 0x9f8
    int x9fc;                   // 0x9fc
    int xa00;                   // 0xa00
    int xa04;                   // 0xa04
    int xa08;                   // 0xa08
    int xa0c;                   // 0xa0c
    int xa10;                   // 0xa10
    int xa14;                   // 0xa14
    int xa18;                   // 0xa18
    int xa1c;                   // 0xa1c
    int xa20;                   // 0xa20
    int xa24;                   // 0xa24
    int xa28;                   // 0xa28
    int xa2c;                   // 0xa2c
    int xa30;                   // 0xa30
    int xa34;                   // 0xa34
    int xa38;                   // 0xa38
    int xa3c;                   // 0xa3c
    int xa40;                   // 0xa40
    int xa44;                   // 0xa44
    int xa48;                   // 0xa48
    int xa4c;                   // 0xa4c
    int xa50;                   // 0xa50
    int xa54;                   // 0xa54
    int xa58;                   // 0xa58
    int xa5c;                   // 0xa5c
    int xa60;                   // 0xa60
    int xa64;                   // 0xa64
    int xa68;                   // 0xa68
    int xa6c;                   // 0xa6c
    int xa70;                   // 0xa70
    int xa74;                   // 0xa74
    int xa78;                   // 0xa78
    int xa7c;                   // 0xa7c
    int xa80;                   // 0xa80
    int xa84;                   // 0xa84
    int xa88;                   // 0xa88
    int xa8c;                   // 0xa8c
    int xa90;                   // 0xa90
    int xa94;                   // 0xa94
    int xa98;                   // 0xa98
    int xa9c;                   // 0xa9c
    int xaa0;                   // 0xaa0
    int xaa4;                   // 0xaa4
    int xaa8;                   // 0xaa8
    int xaac;                   // 0xaac
    int xab0;                   // 0xab0
    int xab4;                   // 0xab4
    int xab8;                   // 0xab8
    int xabc;                   // 0xabc
    int xac0;                   // 0xac0
    int xac4;                   // 0xac4
    int xac8;                   // 0xac8
    int xacc;                   // 0xacc
    int xad0;                   // 0xad0
    int xad4;                   // 0xad4
    int xad8;                   // 0xad8
    int xadc;                   // 0xadc
    int xae0;                   // 0xae0
    int xae4;                   // 0xae4
    int xae8;                   // 0xae8
    int xaec;                   // 0xaec
    int xaf0;                   // 0xaf0
    int xaf4;                   // 0xaf4
    int xaf8;                   // 0xaf8
    int xafc;                   // 0xafc
    int xb00;                   // 0xb00
    int xb04;                   // 0xb04
    int xb08;                   // 0xb08
    int xb0c;                   // 0xb0c
    int xb10;                   // 0xb10
    int xb14;                   // 0xb14
    int xb18;                   // 0xb18
    int xb1c;                   // 0xb1c
    int xb20;                   // 0xb20
    int xb24;                   // 0xb24
    int xb28;                   // 0xb28
    int xb2c;                   // 0xb2c
    int xb30;                   // 0xb30
    int xb34;                   // 0xb34
    int xb38;                   // 0xb38
    int xb3c;                   // 0xb3c
    int xb40;                   // 0xb40
    int xb44;                   // 0xb44
    int xb48;                   // 0xb48
    int xb4c;                   // 0xb4c
    int xb50;                   // 0xb50
    int xb54;                   // 0xb54
    int xb58;                   // 0xb58
    int xb5c;                   // 0xb5c
    int xb60;                   // 0xb60
} ItemData;

typedef struct ItemParam2
{
    int max_items; // 0x0
} ItemParam2;

static ItemCommonParam **stc_item_param = (ItemCommonParam **)(0x805dd0e0 + 0x7E8);
static ItemParam2 **stc_item_param2 = (ItemParam2 **)(0x805dd0e0 + 0x7EC);

ItemKind Gm_GetRandomItem(BoxKind box_kind, ItemGroup group, int spawn_flags); // group: -1 = sky, 0 = blue box, 1 = green box, 2 = red box. r4 = -1 = everything, 0 = down only, 1 = up only. spawn_flags: 0x1 = ?, 0x2 = patch, 0x4 = box,
GOBJ *Item_Create(void *spawn_desc);
ItemCommonAttr *Item_GetCommonAttr(ItemKind it_kind);

AudioSource Item_AllocAudioSource(int index);
#endif