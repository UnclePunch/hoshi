#ifndef KAR_H_RIDER
#define KAR_H_RIDER

#include "datatypes.h"
#include "obj.h"
#include "hurt.h"
#include "collision.h"
#include "machine.h"
#include "camera.h"
#include "trigger.h"

typedef enum RiderKind
{
    RDKIND_KIRBY,
    RDKIND_DEDEDE,
    RDKIND_METAKNIGHT,
    RDKIND_NUM,
} RiderKind;

typedef enum RiderPri
{
    RDPRI_0,
    RDPRI_ANIM,
    RDPRI_INPUT,
    RDPRI_3,
    RDPRI_PHYS,
    RDPRI_ENVCOLL,
    RDPRI_6,
    RDPRI_7,
    RDPRI_8,
    RDPRI_HITCOLL,
    RDPRI_DMGAPPLY,
    RDPRI_13 = 13,
    RDPRI_15 = 15,
} RiderPri;

typedef enum CopyKind
{
    COPYKIND_NONE = -1,
    COPYKIND_FIRE = 0,
    COPYKIND_WHEEL,
    COPYKIND_SLEEP,
    COPYKIND_SWORD,
    COPYKIND_BOMB,
    COPYKIND_PLASMA,
    COPYKIND_NEEDLE,
    COPYKIND_MIC,
    COPYKIND_ICE,
    COPYKIND_TORNADO,
    COPYKIND_BIRD,
    COPYKIND_NUM,
} CopyKind;

typedef struct rdDataKirby
{
    void *attr; // 0x0
    struct
    {
        JOBJDesc *jobjdesc;
        MatAnimJointDesc *matanimjointdesc;
        void *high_poly_table;
        void *mid_poly_table;
        void *low_poly_table;
        void *texture_table;
    } *model;  // 0x4
    void *x8;  // 0x8
    void *xc;  // 0xc
    void *x10; // 0x10
    struct
    {
        float radius;  // 0x0, size of kirby's collision sphere
        float x4;      // 0x4
        float radius2; // 0x8, radius again?
        float xc;      // 0xc
    } *coll;           // 0x14
    struct
    {
        int bone_idx; // 0x0,
        int x4;       // 0x4
        float radius; // 0x8, radius
        Vec3 offset;  // 0xC
    } *jostle;        // 0x18, sphere that detects walking nudge collision
} rdDataKirby;

typedef struct RiderData
{
    int x0;                               // 0x0
    RiderKind kind;                       // 0x4
    u8 ply;                               // 0x8
    u8 x9;                                // 0x9
    u8 color_idx;                         // 0xa
    u8 xb;                                // 0xb
    MachineKind starting_machine_idx : 8; // 0xc
    int x10;                              // 0x10
    int x14;                              // 0x14
    rdDataKirby *rdDataKirby;             // 0x18
    int state_idx;                        // 0x1c
    int x20;                              // 0x20
    int state_frame;                      // 0x24
    int x28;                              // 0x28
    int x2c;                              // 0x2c
    int x30;                              // 0x30
    int x34;                              // 0x34
    int x38;                              // 0x38
    int x3c;                              // 0x3c
    int x40;                              // 0x40
    int x44;                              // 0x44
    int x48;                              // 0x48
    int x4c;                              // 0x4c
    int x50;                              // 0x50
    int x54;                              // 0x54
    int x58;                              // 0x58
    int x5c;                              // 0x5c
    int x60;                              // 0x60
    int x64;                              // 0x64
    int x68;                              // 0x68
    int x6c;                              // 0x6c
    int x70;                              // 0x70
    int x74;                              // 0x74
    int x78;                              // 0x78
    int x7c;                              // 0x7c
    int x80;                              // 0x80
    int x84;                              // 0x84
    int x88;                              // 0x88
    int x8c;                              // 0x8c
    int x90;                              // 0x90
    int x94;                              // 0x94
    int x98;                              // 0x98
    int x9c;                              // 0x9c
    int xa0;                              // 0xa0
    int xa4;                              // 0xa4
    int xa8;                              // 0xa8
    int xac;                              // 0xac
    int xb0;                              // 0xb0
    int xb4;                              // 0xb4
    int xb8;                              // 0xb8
    int xbc;                              // 0xbc
    int xc0;                              // 0xc0
    int xc4;                              // 0xc4
    int xc8;                              // 0xc8
    int xcc;                              // 0xcc
    int xd0;                              // 0xd0
    int xd4;                              // 0xd4
    int xd8;                              // 0xd8
    int xdc;                              // 0xdc
    int xe0;                              // 0xe0
    int xe4;                              // 0xe4
    int xe8;                              // 0xe8
    int xec;                              // 0xec
    int xf0;                              // 0xf0
    int xf4;                              // 0xf4
    int xf8;                              // 0xf8
    int xfc;                              // 0xfc
    int x100;                             // 0x100
    int x104;                             // 0x104
    int x108;                             // 0x108
    int x10c;                             // 0x10c
    int x110;                             // 0x110
    int x114;                             // 0x114
    int x118;                             // 0x118
    int x11c;                             // 0x11c
    int x120;                             // 0x120
    int x124;                             // 0x124
    int x128;                             // 0x128
    int x12c;                             // 0x12c
    int x130;                             // 0x130
    int x134;                             // 0x134
    int x138;                             // 0x138
    int x13c;                             // 0x13c
    int x140;                             // 0x140
    int x144;                             // 0x144
    int x148;                             // 0x148
    int x14c;                             // 0x14c
    int x150;                             // 0x150
    int x154;                             // 0x154
    int x158;                             // 0x158
    int x15c;                             // 0x15c
    int x160;                             // 0x160
    int x164;                             // 0x164
    int x168;                             // 0x168
    int x16c;                             // 0x16c
    int x170;                             // 0x170
    int x174;                             // 0x174
    int x178;                             // 0x178
    int x17c;                             // 0x17c
    int x180;                             // 0x180
    int x184;                             // 0x184
    int x188;                             // 0x188
    int x18c;                             // 0x18c
    int x190;                             // 0x190
    int x194;                             // 0x194
    int x198;                             // 0x198
    int x19c;                             // 0x19c
    int x1a0;                             // 0x1a0
    int x1a4;                             // 0x1a4
    int x1a8;                             // 0x1a8
    int x1ac;                             // 0x1ac
    int x1b0;                             // 0x1b0
    int x1b4;                             // 0x1b4
    int x1b8;                             // 0x1b8
    int x1bc;                             // 0x1bc
    int x1c0;                             // 0x1c0
    int x1c4;                             // 0x1c4
    int x1c8;                             // 0x1c8
    int x1cc;                             // 0x1cc
    int x1d0;                             // 0x1d0
    int x1d4;                             // 0x1d4
    int x1d8;                             // 0x1d8
    int x1dc;                             // 0x1dc
    int x1e0;                             // 0x1e0
    int x1e4;                             // 0x1e4
    int x1e8;                             // 0x1e8
    int x1ec;                             // 0x1ec
    int x1f0;                             // 0x1f0
    int x1f4;                             // 0x1f4
    int x1f8;                             // 0x1f8
    int x1fc;                             // 0x1fc
    int x200;                             // 0x200
    int x204;                             // 0x204
    int x208;                             // 0x208
    int x20c;                             // 0x20c
    int x210;                             // 0x210
    int x214;                             // 0x214
    int x218;                             // 0x218
    int x21c;                             // 0x21c
    int x220;                             // 0x220
    int x224;                             // 0x224
    int x228;                             // 0x228
    int x22c;                             // 0x22c
    int x230;                             // 0x230
    int x234;                             // 0x234
    int x238;                             // 0x238
    int x23c;                             // 0x23c
    int x240;                             // 0x240
    int x244;                             // 0x244
    int x248;                             // 0x248
    int x24c;                             // 0x24c
    int x250;                             // 0x250
    int x254;                             // 0x254
    int x258;                             // 0x258
    int x25c;                             // 0x25c
    int x260;                             // 0x260
    int x264;                             // 0x264
    int x268;                             // 0x268
    int x26c;                             // 0x26c
    int x270;                             // 0x270
    int x274;                             // 0x274
    int x278;                             // 0x278
    int x27c;                             // 0x27c
    int x280;                             // 0x280
    int x284;                             // 0x284
    int x288;                             // 0x288
    int x28c;                             // 0x28c
    int x290;                             // 0x290
    int x294;                             // 0x294
    int x298;                             // 0x298
    int x29c;                             // 0x29c
    int x2a0;                             // 0x2a0
    int x2a4;                             // 0x2a4
    int x2a8;                             // 0x2a8
    int x2ac;                             // 0x2ac
    int x2b0;                             // 0x2b0
    int x2b4;                             // 0x2b4
    int x2b8;                             // 0x2b8
    int x2bc;                             // 0x2bc
    DOBJ *dobj_lookup_arr;                // 0x2c0
    int x2c4;                             // 0x2c4
    int x2c8;                             // 0x2c8
    int x2cc;                             // 0x2cc
    int x2d0;                             // 0x2d0
    int x2d4;                             // 0x2d4
    int x2d8;                             // 0x2d8
    Vec3 self_vel;                        // 0x2dc
    int x2e8;                             // 0x2e8
    int x2ec;                             // 0x2ec
    int x2f0;                             // 0x2f0
    int x2f4;                             // 0x2f4
    int x2f8;                             // 0x2f8
    int x2fc;                             // 0x2fc
    Vec3 pos;                             // 0x300
    int x30c;                             // 0x30c
    int x310;                             // 0x310
    int x314;                             // 0x314
    int x318;                             // 0x318
    int x31c;                             // 0x31c
    int x320;                             // 0x320
    Vec3 forward;                         // 0x324, forward movement vector
    Vec3 up;                              // 0x330, up vector
    Vec3 x33c;                            // 0x33c
    int x348;                             // 0x348
    int x34c;                             // 0x34c
    int x350;                             // 0x350
    int x354;                             // 0x354
    int x358;                             // 0x358
    int x35c;                             // 0x35c
    int x360;                             // 0x360
    int x364;                             // 0x364
    int x368;                             // 0x368
    int x36c;                             // 0x36c
    int x370;                             // 0x370
    int x374;                             // 0x374
    int x378;                             // 0x378
    int x37c;                             // 0x37c
    int x380;                             // 0x380
    int x384;                             // 0x384
    int x388;                             // 0x388
    void *jump_param;                     // 0x38c
    HurtData *hurt_data;                  // 0x390
    struct
    {
        Vec2 lstick;        // 0x394
        Vec2 lstick_prev;   // 0x39c
        Vec2 x3a4;          // 0x3a4
        Vec2 rstick;        // 0x3ac
        Vec2 rstick_prev;   // 0x3b4
        Vec2 x3bc;          // 0x3bc
        float trigger;      // 0x3c4
        float trigger_prev; // 0x3c8
        float x3cc;         // 0x3cc
        float x3d0;         // 0x3d0
        int x3d4;           // 0x3d4
        int held;           // 0x3d8
        int x3dc;           // 0x3dc
        int x3e0;           // 0x3e0
        int down;           // 0x3e4
        int x3e8;           // 0x3e8
        s8 stickX;          // 0x3ec, byte. used for replays i think
        s8 stickY;          // 0x3ed, byte. used for replays i think
        int x3f0;           // 0x3f0
    } input;
    GOBJ *machine_gobj;        // 0x3f4
    GOBJ *x3f8;                // 0x3f8
    int x3fc;                  // 0x3fc
    int x400;                  // 0x400
    int x404;                  // 0x404
    int x408;                  // 0x408
    int x40c;                  // 0x40c
    int x410;                  // 0x410
    int x414;                  // 0x414
    int x418;                  // 0x418
    int x41c;                  // 0x41c
    int x420;                  // 0x420
    int x424;                  // 0x424
    u8 x428;                   // 0x428
    u8 x429;                   // 0x429
    struct                     // 0x42a
    {                          //
        u8 x0;                 //
        u8 cur_mat_index;      // material index currently being used for this model part (changes if wing kirby or fire)
        u8 original_mat_index; // material index that describes the original color
    } model_part[3];           // probably more of these
    u8 x433;                   // 0x433
    int x434;                  // 0x434
    int x438;                  // 0x438
    int x43c;                  // 0x43c
    int x440;                  // 0x440
    int x444;                  // 0x444
    int x448;                  // 0x448
    int x44c;                  // 0x44c
    CamInterest *x450;       // 0x450
    CopyKind copy_kind;        // 0x454
    int x458;                  // 0x458
    int x45c;                  // 0x45c
    int x460;                  // 0x460
    int x464;                  // 0x464
    int x468;                  // 0x468
    int x46c;                  // 0x46c
    int x470;                  // 0x470
    int x474;                  // 0x474
    int x478;                  // 0x478
    int x47c;                  // 0x47c
    int x480;                  // 0x480
    int x484;                  // 0x484
    int x488;                  // 0x488
    int x48c;                  // 0x48c
    int x490;                  // 0x490
    int x494;                  // 0x494
    int x498;                  // 0x498
    int x49c;                  // 0x49c
    int x4a0;                  // 0x4a0
    int x4a4;                  // 0x4a4
    int x4a8;                  // 0x4a8
    int x4ac;                  // 0x4ac
    int x4b0;                  // 0x4b0
    int x4b4;                  // 0x4b4
    int x4b8;                  // 0x4b8
    int x4bc;                  // 0x4bc
    int x4c0;                  // 0x4c0
    int x4c4;                  // 0x4c4
    int x4c8;                  // 0x4c8
    AudioSource audio_source;  // 0x4cc
    int audio_track;           // 0x4d0
    int x4d4;                  // 0x4d4
    int x4d8;                  // 0x4d8
    int x4dc;                  // 0x4dc
    int x4e0;                  // 0x4e0
    int x4e4;                  // 0x4e4
    int x4e8;                  // 0x4e8
    int x4ec;                  // 0x4ec
    int x4f0;                  // 0x4f0
    int x4f4;                  // 0x4f4
    int x4f8;                  // 0x4f8
    int x4fc;                  // 0x4fc
    int x500;                  // 0x500
    int x504;                  // 0x504
    int x508;                  // 0x508
    int x50c;                  // 0x50c
    int x510;                  // 0x510
    int x514;                  // 0x514
    int x518;                  // 0x518
    int x51c;                  // 0x51c
    int x520;                  // 0x520
    int x524;                  // 0x524
    int x528;                  // 0x528
    int x52c;                  // 0x52c
    int x530;                  // 0x530
    int x534;                  // 0x534
    int x538;                  // 0x538
    int x53c;                  // 0x53c
    int x540;                  // 0x540
    int x544;                  // 0x544
    int x548;                  // 0x548
    int x54c;                  // 0x54c
    int x550;                  // 0x550
    int x554;                  // 0x554
    int x558;                  // 0x558
    int x55c;                  // 0x55c
    int x560;                  // 0x560
    int x564;                  // 0x564
    int x568;                  // 0x568
    int x56c;                  // 0x56c
    int x570;                  // 0x570
    int x574;                  // 0x574
    int x578;                  // 0x578
    int x57c;                  // 0x57c
    int x580;                  // 0x580
    int x584;                  // 0x584
    int x588;                  // 0x588
    int x58c;                  // 0x58c
    int x590;                  // 0x590
    int x594;                  // 0x594
    int x598;                  // 0x598
    int x59c;                  // 0x59c
    int x5a0;                  // 0x5a0
    int x5a4;                  // 0x5a4
    int x5a8;                  // 0x5a8
    int x5ac;                  // 0x5ac
    int x5b0;                  // 0x5b0
    int x5b4;                  // 0x5b4
    int x5b8;                  // 0x5b8
    int x5bc;                  // 0x5bc
    int x5c0;                  // 0x5c0
    int x5c4;                  // 0x5c4
    int x5c8;                  // 0x5c8
    int x5cc;                  // 0x5cc
    int x5d0;                  // 0x5d0
    int x5d4;                  // 0x5d4
    int x5d8;                  // 0x5d8
    int x5dc;                  // 0x5dc
    int x5e0;                  // 0x5e0
    int x5e4;                  // 0x5e4
    int x5e8;                  // 0x5e8
    int x5ec;                  // 0x5ec
    int x5f0;                  // 0x5f0
    int x5f4;                  // 0x5f4
    int x5f8;                  // 0x5f8
    int x5fc;                  // 0x5fc
    int x600;                  // 0x600
    int x604;                  // 0x604
    int x608;                  // 0x608
    int x60c;                  // 0x60c
    int x610;                  // 0x610
    int x614;                  // 0x614
    int x618;                  // 0x618
    int x61c;                  // 0x61c
    int x620;                  // 0x620
    int x624;                  // 0x624
    int x628;                  // 0x628
    int x62c;                  // 0x62c
    int x630;                  // 0x630
    int x634;                  // 0x634
    int x638;                  // 0x638
    int x63c;                  // 0x63c
    int x640;                  // 0x640
    int x644;                  // 0x644
    int x648;                  // 0x648
    int x64c;                  // 0x64c
    int x650;                  // 0x650
    int x654;                  // 0x654
    int x658;                  // 0x658
    int x65c;                  // 0x65c
    int x660;                  // 0x660
    int x664;                  // 0x664
    int x668;                  // 0x668
    GOBJ *shadow_gobj;         // 0x66c
    CollData *coll_data;       // 0x670
    TriggerData trigger;       // 0x674
    int x6d4;                  // 0x6d4
    int x6d8;                  // 0x6d8
    int x6dc;                  // 0x6dc
    int x6e0;                  // 0x6e0
    int x6e4;                  // 0x6e4
    int x6e8;                  // 0x6e8
    int x6ec;                  // 0x6ec
    int x6f0;                  // 0x6f0
    int x6f4;                  // 0x6f4
    int x6f8;                  // 0x6f8
    int x6fc;                  // 0x6fc
    int x700;                  // 0x700
    int x704;                  // 0x704
    int x708;                  // 0x708
    int x70c;                  // 0x70c
    int x710;                  // 0x710
    int x714;                  // 0x714
    int x718;                  // 0x718
    int x71c;                  // 0x71c
    int x720;                  // 0x720
    int x724;                  // 0x724
    int x728;                  // 0x728
    int x72c;                  // 0x72c
    int x730;                  // 0x730
    int x734;                  // 0x734
    int x738;                  // 0x738
    int x73c;                  // 0x73c
    int x740;                  // 0x740
    float x744;                // 0x744
    float x748;                // 0x748
    union {                    // 0x74C
        struct {
            float WEIGHT;
            float BOOST;
            float TOP_SPEED;
            float TURN;
            float CHARGE;
            float GLIDE;
            float OFFENSE;
            float DEFENSE;
            float HP;
        };
        float values[9];
    } stats;
    int x768;                  // 0x768
    int x76c;                  // 0x76c
    int x770;                  // 0x770
    int x774;                  // 0x774
    int x778;                  // 0x778
    int x77c;                  // 0x77c
    int x780;                  // 0x780
    int x784;                  // 0x784
    int x788;                  // 0x788
    int x78c;                  // 0x78c
    int x790;                  // 0x790
    int x794;                  // 0x794
    int x798;                  // 0x798
    int x79c;                  // 0x79c
    int x7a0;                  // 0x7a0
    int x7a4;                  // 0x7a4
    int x7a8;                  // 0x7a8
    int x7ac;                  // 0x7ac
    int x7b0;                  // 0x7b0
    struct                     //
    {                          //
        void (*anim)(GOBJ *);  // 0x7b4
        void (*iasa)(GOBJ *);  // 0x7b8
        void (*phys)(GOBJ *);  // 0x7bc
        void (*coll)(GOBJ *);  // 0x7c0
        void (*x7c4)(GOBJ *);  // 0x7c4, runs per frame from gobj proc 8018f7b0
        void (*x7c8)(GOBJ *);  // 0x7c8, runs per frame from gobj proc 8018fc10
        void (*x7cc)(GOBJ *);  // 0x7cc, runs per frame from gobj proc 8018e9f0
        void (*x7d0)(GOBJ *);  // 0x7d0
        void (*x7d4)(GOBJ *);  // 0x7d4
        void (*x7d8)(GOBJ *);  // 0x7d8
    } cb;
    int x7dc;                           // 0x7dc
    int x7e0;                           // 0x7e0
    int x7e4;                           // 0x7e4
    int x7e8;                           // 0x7e8
    int x7ec;                           // 0x7ec
    int x7f0;                           // 0x7f0
    int x7f4;                           // 0x7f4
    int x7f8;                           // 0x7f8
    int x7fc;                           // 0x7fc
    int x800;                           // 0x800
    int x804;                           // 0x804
    int x808;                           // 0x808
    int x80c;                           // 0x80c
    int x810;                           // 0x810
    int x814;                           // 0x814
    int x818;                           // 0x818
    int x81c;                           // 0x81c
    u8 x820;                            // 0x820
    u8 x821;                            // 0x821
    u8 x822;                            // 0x822
    u8 x823;                            // 0x823
    u8 x824;                            // 0x824
    u8 x825;                            // 0x825
    u8 x826_80 : 1;                     // 0x826
    u8 x826_40 : 1;                     // 0x826
    u8 x826_20 : 1;                     // 0x826
    u8 x826_10 : 1;                     // 0x826
    u8 x826_08 : 1;                     // 0x826
    u8 x826_04 : 1;                     // 0x826
    u8 x826_02 : 1;                     // 0x826
    u8 is_walk_after_dead : 1;          // 0x826
    u8 x827;                            // 0x827
    int x828;                           // 0x828
    int x82c;                           // 0x82c
    int x830;                           // 0x830
    int x834;                           // 0x834
    int x838;                           // 0x838
    int x83c;                           // 0x83c
    int x840;                           // 0x840
    int x844;                           // 0x844
    int x848;                           // 0x848
    int x84c;                           // 0x84c
    int x850;                           // 0x850
    int x854;                           // 0x854
    int x858;                           // 0x858
    int x85c;                           // 0x85c
    int x860;                           // 0x860
    int x864;                           // 0x864
    int x868;                           // 0x868
    int x86c;                           // 0x86c
    int x870;                           // 0x870
    int x874;                           // 0x874
    int x878;                           // 0x878
    int x87c;                           // 0x87c
    int x880;                           // 0x880
    int x884;                           // 0x884
    int x888;                           // 0x888
    int x88c;                           // 0x88c
    int x890;                           // 0x890
    int x894;                           // 0x894
    int x898;                           // 0x898
    int x89c;                           // 0x89c
    int x8a0;                           // 0x8a0
    int x8a4;                           // 0x8a4
    int x8a8;                           // 0x8a8
    int x8ac;                           // 0x8ac
    int x8b0;                           // 0x8b0
    int x8b4;                           // 0x8b4
    int x8b8;                           // 0x8b8
    int x8bc;                           // 0x8bc
    int x8c0;                           // 0x8c0
    int x8c4;                           // 0x8c4
    int x8c8;                           // 0x8c8
    int x8cc;                           // 0x8cc
    int x8d0;                           // 0x8d0
    int x8d4;                           // 0x8d4
    int x8d8;                           // 0x8d8
    int x8dc;                           // 0x8dc
    int x8e0;                           // 0x8e0
    int x8e4;                           // 0x8e4
    int x8e8;                           // 0x8e8
    int x8ec;                           // 0x8ec
    int x8f0;                           // 0x8f0
    int x8f4;                           // 0x8f4
    int x8f8;                           // 0x8f8
    int x8fc;                           // 0x8fc
    int x900;                           // 0x900
    int x904;                           // 0x904
    int x908;                           // 0x908
    int x90c;                           // 0x90c
    int x910;                           // 0x910
    int x914;                           // 0x914
    int x918;                           // 0x918
    int copy_timer;                     // 0x91c
    int x920;                           // 0x920
    int x924;                           // 0x924
    int x928;                           // 0x928
    int x92c;                           // 0x92c
    void (*cb_copy_input)(RiderData *); // 0x930
    int x934;                           // 0x934
    int x938;                           // 0x938
    int x93c;                           // 0x93c
    int x940;                           // 0x940
    int x944;                           // 0x944
    int x948;                           // 0x948
    int x94c;                           // 0x94c
    int x950;                           // 0x950
    int x954;                           // 0x954
    int x958;                           // 0x958
    int x95c;                           // 0x95c
    int x960;                           // 0x960
    int x964;                           // 0x964
    int x968;                           // 0x968
    int x96c;                           // 0x96c
    int x970;                           // 0x970
    int x974;                           // 0x974
    int x978;                           // 0x978
    int x97c;                           // 0x97c
    int x980;                           // 0x980
    int x984;                           // 0x984
    int x988;                           // 0x988
    int x98c;                           // 0x98c
    int x990;                           // 0x990
    int x994;                           // 0x994
    int x998;                           // 0x998
    int x99c;                           // 0x99c
    int x9a0;                           // 0x9a0
    int x9a4;                           // 0x9a4
    int x9a8;                           // 0x9a8
    int x9ac;                           // 0x9ac
    int x9b0;                           // 0x9b0
    int x9b4;                           // 0x9b4
    int x9b8;                           // 0x9b8
    int x9bc;                           // 0x9bc
    int x9c0;                           // 0x9c0
    int x9c4;                           // 0x9c4
    int jumps_used;                     // 0x9c8
    u8 is_fullhop : 1;                  // 0x9cc 0x80
    u8 x9cd;                            // 0x9cd
    u8 x9ce;                            // 0x9ce
    u8 x9cf;                            // 0x9cf
    int x9d0;                           // 0x9d0
    int x9d4;                           // 0x9d4
    int x9d8;                           // 0x9d8
    int x9dc;                           // 0x9dc
    int x9e0;                           // 0x9e0
    int x9e4;                           // 0x9e4
    int x9e8;                           // 0x9e8
    int x9ec;                           // 0x9ec
    int x9f0;                           // 0x9f0
    int x9f4;                           // 0x9f4
    int x9f8;                           // 0x9f8
    int x9fc;                           // 0x9fc
    int xa00;                           // 0xa00
    int xa04;                           // 0xa04
    int xa08;                           // 0xa08
    int xa0c;                           // 0xa0c
    int xa10;                           // 0xa10
    int xa14;                           // 0xa14
    int x;                              // 0xa18
    int xa1c;                           // 0xa1c
    int xa20;                           // 0xa20
    int xa24;                           // 0xa24
    int xa28;                           // 0xa28
    int xa2c;                           // 0xa2c
    int xa30;                           // 0xa30
    int xa34;                           // 0xa34
    int xa38;                           // 0xa38
    int xa3c;                           // 0xa3c
    int xa40;                           // 0xa40
    int xa44;                           // 0xa44
    int xa48;                           // 0xa48
    int xa4c;                           // 0xa4c
    int xa50;                           // 0xa50
    int xa54;                           // 0xa54
    int xa58;                           // 0xa58
    int xa5c;                           // 0xa5c
    int xa60;                           // 0xa60
    int xa64;                           // 0xa64
    int xa68;                           // 0xa68
    int xa6c;                           // 0xa6c
    int xa70;                           // 0xa70
    int xa74;                           // 0xa74
    int xa78;                           // 0xa78
    int xa7c;                           // 0xa7c
    int xa80;                           // 0xa80
    int xa84;                           // 0xa84
    int xa88;                           // 0xa88
    int xa8c;                           // 0xa8c
    int xa90;                           // 0xa90
    int xa94;                           // 0xa94
    int xa98;                           // 0xa98
    int xa9c;                           // 0xa9c
    int xaa0;                           // 0xaa0
    int xaa4;                           // 0xaa4
    int xaa8;                           // 0xaa8
    int xaac;                           // 0xaac
    int xab0;                           // 0xab0
    int xab4;                           // 0xab4
    int xab8;                           // 0xab8
    int xabc;                           // 0xabc
    int xac0;                           // 0xac0
} RiderData;

static rdDataKirby **stc_rdDataKirby = (rdDataKirby **)0x80559fa8;

void Rider_RespawnEnter(RiderData *);
void Rider_GiveAbility(RiderData *, CopyKind);
int Rider_CheckUnableAbility(RiderData *); // checks if the rider can receive an ability?
void Rider_AbilityRemoveModel(RiderData *);
void Rider_AbilityRemoveUnk(RiderData *);
void Rider_LoseAbilityState_Enter(RiderData *);
void Rider_GiveIntangibility(RiderData *, int time);
void Rider_GiveInvincibility(RiderData *, int time);
int Rider_IsOnMachine(RiderData *);
int Rider_IsMachineDead(RiderData *);       // can only be called between the RDPRI_HITCOLL and RDPRI_DMGAPPLY priority.
void Rider_DropPatches(RiderData *, float stat_array[9], int drop_mode); // drop_mode=0,1,2 mode 0 drops a random individual patch, mode 1 drop many patches behind the player, mode 2 drops many patches in front of the player. Mode 1/2 drop all ups if stats are high enough

AudioSource Rider_AllocAudioSource(int index);

#endif