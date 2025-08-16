#ifndef KAR_H_GAME
#define KAR_H_GAME

#include "audio.h"
#include "structs.h"
#include "datatypes.h"
#include "os.h"
#include "scene.h"
#include "obj.h"
#include "hsd.h"
#include "stadium.h"
#include "rider.h"
#include "machine.h"
#include "stage.h"
#include "event.h"
#include "item.h"
#include "menu.h"

//////////////////////
// Enum Definitions //
//////////////////////

typedef enum GameEntity
{
    GAMEENTITY_MACHINE = 16,
    GAMEENTITY_RIDER,
} GameEntity;

typedef enum GamePLink
{
    GAMEPLINK_SYS,
    GAMEPLINK_1,
    GAMEPLINK_CITYEVENTSPAWN,
    GAMEPLINK_3,
    GAMEPLINK_CITYITEMSPAWN,
    GAMEPLINK_CITYMACHINESPAWN,
    GAMEPLINK_6, // effect probably
    GAMEPLINK_STAGE,
    GAMEPLINK_YAKUMONO,
    GAMEPLINK_MACHINE,
    GAMEPLINK_RIDER,
    GAMEPLINK_11,
    GAMEPLINK_EVENTACTOR, // dyna blade, tac, meteor, etc
    GAMEPLINK_ITEM,       // anything in ItemKind
    GAMEPLINK_PROJECTILE, // bomb, plasma bullet, flame, firework etc
    GAMEPLINK_15,
    GAMEPLINK_16,
    GAMEPLINK_CAMWORLD,
    GAMEPLINK_18,
    GAMEPLINK_CAMHUD,
    GAMEPLINK_20,
    GAMEPLINK_CAMDEBUG,
    GAMEPLINK_AUDIO,
    GAMEPLINK_23,
    GAMEPLINK_24,
    GAMEPLINK_25,
    GAMEPLINK_HUD,
    GAMEPLINK_PAUSEHUD,
    GAMEPLINK_CARDCAM, // renders memcard save prompt
} GamePLink;

typedef enum PKind
{
    PKIND_HMN,
    PKIND_CPU,
    PKIND_2,
    PKIND_3,
    PKIND_NONE,
} PKind;

typedef enum CharacterKind
{
    CKIND_COMPACT,
    CKIND_WARP,
    CKIND_TURBO,
    CKIND_FORMULA,
    CKIND_SLICK,
    CKIND_SWERVE,
    CKIND_WAGON,
    CKIND_BULK,
    CKIND_SHADOW,
    CKIND_WINGED,
    CKIND_JET,
    CKIND_ROCKET,
    CKIND_WHEELIESCOOTER,
    CKIND_WHEELIEBIKE,
    CKIND_REXWHEELIE,
    CKIND_DRAGOON,
    CKIND_HYDRA,
    CKIND_FLIGHT,
    CKIND_DEDEDE,
    CKIND_METAKNIGHT,
} CharacterKind;

typedef enum AirRideMode
{
    AIRRIDEMODE_RACE,
    AIRRIDEMODE_TIME,
    AIRRIDEMODE_FREE,
} AirRideMode;

typedef enum CityMode
{
    CITYMODE_TRIAL,
    CITYMODE_STADIUM,
    CITYMODE_FREERUN,
} CityMode;

typedef enum GameMode
{
    GMMODE_AIRRIDE,
    GMMODE_TOPRIDE,
    GMMODE_CITYTRIAL,
    GMMODE_NUM,
} GameMode;

typedef enum GmIntroState
{
    GMINTRO_END,
    GMINTRO_1,
    GMINTRO_FLYOVER,
    GMINTRO_COUNTDOWN,
} GmIntroState;

typedef enum CityClearKind
{
    CITYCLEARKIND_0,
    CITYCLEARKIND_1,
    CITYCLEARKIND_COLOR_GREEN = 15,
    CITYCLEARKIND_COLOR_PURPLE,
    CITYCLEARKIND_COLOR_BROWN,
    CITYCLEARKIND_COLOR_WHITE,
    CITYCLEARKIND_BREAK5CPUMACHINES = 77, // 8004e398
    CITYCLEARKIND_NUM = 120,
} CityClearKind;

///////////////////////
// Struct Defintions //
///////////////////////

typedef struct TitleScreenData
{
    u8 state;      // 0xc
    u8 xd;         // 0xd
    u16 timer;     // 0xe
    int x10;       // 0x10
    u8 is_skip_op; // 0x14, bool to skip the opening movie when loading the title screen
    u8 x15;        // 0x15
    u8 x16;        // 0x16
    u8 x17;        // 0x17
} TitleScreenData;

typedef struct GameData
{
    int x0;                            // 0x0
    int x4;                            // 0x4
    int x8;                            // 0x8
    TitleScreenData title_data;        // 0xc
    int x18;                           // 0x18
    int x1c;                           // 0x1c
    int x20;                           // 0x20
    int x24;                           // 0x24
    u8 x28;                            // 0x28
    u8 x29;                            // 0x29
    u8 is_showed_tutorial_airride : 1; // 0x2a
    u8 is_showed_tutorial_topride : 1; // 0x2a
    u8 is_showed_tutorial_city : 1;    // 0x2a, 0x20
    u8 x2b;                            // 0x2b
    int x2c;                           // 0x2c
    MainMenuData main_menu;            // 0x30
    int x50;                           // 0x50
    int x54;                           // 0x54
    int x58;                           // 0x58
    int x5c;                           // 0x5c
    int x60;                           // 0x60
    int x64;                           // 0x64
    int x68;                           // 0x68
    int x6c;                           // 0x6c
    int x70;                           // 0x70
    int x74;                           // 0x74
    int x78;                           // 0x78
    int x7c;                           // 0x7c
    int x80;                           // 0x80
    int x84;                           // 0x84
    int x88;                           // 0x88
    int x8c;                           // 0x8c
    int x90;                           // 0x90
    int x94;                           // 0x94
    int x98;                           // 0x98
    int x9c;                           // 0x9c
    int xa0;                           // 0xa0
    int xa4;                           // 0xa4
    int xa8;                           // 0xa8
    int xac;                           // 0xac
    int xb0;                           // 0xb0
    int xb4;                           // 0xb4
    int xb8;                           // 0xb8
    int xbc;                           // 0xbc
    int xc0;                           // 0xc0
    int xc4;                           // 0xc4
    int xc8;                           // 0xc8
    int xcc;                           // 0xcc
    int xd0;                           // 0xd0
    int xd4;                           // 0xd4
    int xd8;                           // 0xd8
    int xdc;                           // 0xdc
    int xe0;                           // 0xe0
    int xe4;                           // 0xe4
    int xe8;                           // 0xe8
    int xec;                           // 0xec
    int xf0;                           // 0xf0
    int xf4;                           // 0xf4
    int xf8;                           // 0xf8
    int xfc;                           // 0xfc
    int x100;                          // 0x100
    int x104;                          // 0x104
    struct
    {
        u8 x108;      // 0x108
        u8 x109;      // 0x109
        u8 x10a;      // 0x10a
        u8 x10b;      // 0x10b
        int x10c;     // 0x10c
        int x110;     // 0x110
        int x114;     // 0x114
        int x118;     // 0x118
        int x11c;     // 0x11c
        int x120;     // 0x120
        int x124;     // 0x124
        int x128;     // 0x128
        int x12c;     // 0x12c
        u8 x130;      // 0x134
        u8 x131;      // 0x134
        u8 x132;      // 0x134
        u8 p_kind[4]; // 0x133
        u8 icon[4];   // 0x137
        u8 x13b;      // 0x13b
        int x13c;     // 0x13c
        int x140;     // 0x140
        int x144;     // 0x144
        int x148;     // 0x148
        int x14c;     // 0x14c
        int x150;     // 0x150
        int x154;     // 0x154
        u8 x158;      // 0x158
        u8 x159;      // 0x159
        u8 x15a;      // 0x15a
        u8 color[4];  // 0x15b
        u8 x15f;
    } airride_select_ply;
    int x160; // 0x160
    int x164; // 0x164
    int x168; // 0x168
    int x16c; // 0x16c
    int x170; // 0x170
    int x174; // 0x174
    int x178; // 0x178
    int x17c; // 0x17c
    int x180; // 0x180
    int x184; // 0x184
    int x188; // 0x188
    int x18c; // 0x18c
    int x190; // 0x190
    int x194; // 0x194
    int x198; // 0x198
    int x19c; // 0x19c
    int x1a0; // 0x1a0
    int x1a4; // 0x1a4
    int x1a8; // 0x1a8
    int x1ac; // 0x1ac
    int x1b0; // 0x1b0
    int x1b4; // 0x1b4
    int x1b8; // 0x1b8
    int x1bc; // 0x1bc
    int x1c0; // 0x1c0
    int x1c4; // 0x1c4
    int x1c8; // 0x1c8
    int x1cc; // 0x1cc
    struct
    {
        int x1d0;          // 0x1d0
        int x1d4;          // 0x1d4
        int x1d8;          // 0x1d8
        int x1dc;          // 0x1dc
        int x1e0;          // 0x1e0
        int x1e4;          // 0x1e4
        int x1e8;          // 0x1e8
        int x1ec;          // 0x1ec
        u8 x1f0;           // 0x1f0
        u8 ready_state[4]; // 0x1f1
        u8 x1f4;           // 0x1f5
        int x1f8;          // 0x1f8
        u8 x1fc;           // 0x1fc
        u8 icon[4];        // 0x1fd
        u8 x201;           // 0x201
        int x204;          // 0x204
        int x208;          // 0x208
        int x20c;          // 0x20c
        int x210;          // 0x210
        int x214;          // 0x214
        int x218;          // 0x218
        int x21c;          // 0x21c
        u8 x220;           // 0x220
        u8 color[4];       // 0x221
        u8 x225;           // 0x225
        int x228;          // 0x228
        int x22c;          // 0x22c
        int x230;          // 0x230
        struct
        {
            u8 x234;           // 0x234, total number of machines selectable
            u8 num;            // 0x235, total number of machines selectable
            u8 c_kind_arr[20]; // 0x236, 0x66, array of c_kind indices
        } machine_select;
        int x24c; // 0x24c
        int x250; // 0x250
        int x254; // 0x254
        int x258; // 0x258
    } city_select_ply;
    int x25c;                        // 0x25c
    int x260;                        // 0x260
    int x264;                        // 0x264
    int x268;                        // 0x268
    int x26c;                        // 0x26c
    int x270;                        // 0x270
    int x274;                        // 0x274
    int x278;                        // 0x278
    int x27c;                        // 0x27c
    int x280;                        // 0x280
    int x284;                        // 0x284
    int x288;                        // 0x288
    int x28c;                        // 0x28c
    int x290;                        // 0x290
    int x294;                        // 0x294
    int x298;                        // 0x298
    int x29c;                        // 0x29c
    int x2a0;                        // 0x2a0
    int x2a4;                        // 0x2a4
    int x2a8;                        // 0x2a8
    int x2ac;                        // 0x2ac
    int x2b0;                        // 0x2b0
    int x2b4;                        // 0x2b4
    int x2b8;                        // 0x2b8
    int x2bc;                        // 0x2bc
    int x2c0;                        // 0x2c0
    int x2c4;                        // 0x2c4
    int x2c8;                        // 0x2c8
    int x2cc;                        // 0x2cc
    int x2d0;                        // 0x2d0
    int x2d4;                        // 0x2d4
    int x2d8;                        // 0x2d8
    int x2dc;                        // 0x2dc
    int x2e0;                        // 0x2e0
    int x2e4;                        // 0x2e4
    int x2e8;                        // 0x2e8
    int x2ec;                        // 0x2ec
    int x2f0;                        // 0x2f0
    int x2f4;                        // 0x2f4
    int x2f8;                        // 0x2f8
    int x2fc;                        // 0x2fc
    int x300;                        // 0x300
    int x304;                        // 0x304
    int x308;                        // 0x308
    int x30c;                        // 0x30c
    int x310;                        // 0x310
    int x314;                        // 0x314
    int x318;                        // 0x318
    int x31c;                        // 0x31c
    int x320;                        // 0x320
    int x324;                        // 0x324
    int x328;                        // 0x328
    int x32c;                        // 0x32c
    int x330;                        // 0x330
    int x334;                        // 0x334
    int x338;                        // 0x338
    int x33c;                        // 0x33c
    int x340;                        // 0x340
    int x344;                        // 0x344
    int x348;                        // 0x348
    int x34c;                        // 0x34c
    int x350;                        // 0x350
    int x354;                        // 0x354
    int x358;                        // 0x358
    u8 x35c;                         // 0x35c
    AirRideMode airride_mode : 8;    // 0x35d
    u8 x35e;                         // 0x35e
    u8 x35f;                         // 0x35f
    int x360;                        // 0x360
    int x364;                        // 0x364
    int x368;                        // 0x368
    int x36c;                        // 0x36c
    int x370;                        // 0x370
    int x374;                        // 0x374
    int x378;                        // 0x378
    int x37c;                        // 0x37c
    int x380;                        // 0x380
    int x384;                        // 0x384
    int x388;                        // 0x388
    int x38c;                        // 0x38c
    int x390;                        // 0x390
    struct                           // 0x394
    {                                //
        u16 time_seconds;            // 0x394
        u8 menu_stadium_selection;   // 0x396, selected from settings, sub 1 to get StadiumGroup value
        u8 game_tempo : 2;           // 0x397, 0xC0 (1 = normal = 2 is slow)
        u8 events_enable : 1;        // 0x397, 0x20
        u8 x397_x10 : 1;             // 0x397, 0x10
        u8 x397_x08 : 1;             // 0x397, 0x08
        u8 x397_x04 : 1;             // 0x397, 0x04
        u8 x397_x02 : 1;             // 0x397, 0x02
        u8 x397_x01 : 1;             // 0x397, 0x01
        int x398 : 8;                // 0x398,
        CityMode mode : 8;           // 0x399, which mode was selected from the menu
        int x39a : 8;                // 0x39a,
        int scene : 8;               // 0x39b, 3 = player select screen, 4 = in game, 5 = properties graph, 6 = stadium splash, 7 = stadium, 8 = results screen
        int x39c;                    // 0x39c
        float ply_stats[4][9];       // 0x3a0, copied from playerblock @ 80040024
        int x430;                    // 0x430
        int x434;                    // 0x434
        int x438;                    // 0x438
        int x43c;                    // 0x43c
        int x440;                    // 0x440
        int x444;                    // 0x444
        int x448;                    // 0x448
        int x44c;                    // 0x44c
        int x450;                    // 0x450
        u8 is_bike[5];               // 0x454
        u8 machine_kind[5];          // 0x459
        u8 prev_stadium_kind[5];     // 0x45e
        int x464;                    // 0x464
        int x468;                    // 0x468
        int x46c;                    // 0x46c
        int x470;                    // 0x470
        int x474;                    // 0x474
        int x478;                    // 0x478
        int x47c;                    // 0x47c
        int x480;                    // 0x480
        int x484;                    // 0x484
        int x488;                    // 0x488
        int x48c;                    // 0x48c
        int x490;                    // 0x490
        int x494;                    // 0x494
        int x498;                    // 0x498
        int x49c;                    // 0x49c
        int x4a0;                    // 0x4a0
        int x4a4;                    // 0x4a4
        int x4a8;                    // 0x4a8
        int x4ac;                    // 0x4ac
        int x4b0;                    // 0x4b0
        int x4b4;                    // 0x4b4
        int x4b8;                    // 0x4b8
        int x4bc;                    // 0x4bc
        int x4c0;                    // 0x4c0
        int x4c4;                    // 0x4c4
        int x4c8;                    // 0x4c8
        int x4cc;                    // 0x4cc
        int x4d0;                    // 0x4d0
        int x4d4;                    // 0x4d4
        int x4d8;                    // 0x4d8
        int x4dc;                    // 0x4dc
        int x4e0;                    // 0x4e0
        int x4e4;                    // 0x4e4
        int x4e8;                    // 0x4e8
        int x4ec;                    // 0x4ec
        int x4f0;                    // 0x4f0
        int x4f4;                    // 0x4f4
        int x4f8;                    // 0x4f8
        int x4fc;                    // 0x4fc
        int x500;                    // 0x500
        int x504;                    // 0x504
        int x508;                    // 0x508
        int x50c;                    // 0x50c
        int x510;                    // 0x510
        int x514;                    // 0x514
        int x518;                    // 0x518
        int x51c;                    // 0x51c
        int x520;                    // 0x520
        int x524;                    // 0x524
        int x528;                    // 0x528
        int x52c;                    // 0x52c
        int x530;                    // 0x530
        int x534;                    // 0x534
        int x538;                    // 0x538
        int x53c;                    // 0x53c
        int x540;                    // 0x540
        int x544;                    // 0x544
        int x548;                    // 0x548
        int x54c;                    // 0x54c
        int x550;                    // 0x550
        int x554;                    // 0x554
        int x558;                    // 0x558
        int x55c;                    // 0x55c
        int x560;                    // 0x560
        int x564;                    // 0x564
        int x568;                    // 0x568
        int x56c;                    // 0x56c
        int x570;                    // 0x570
        int x574;                    // 0x574
        int x578;                    // 0x578
        int x57c;                    // 0x57c
        int x580;                    // 0x580
        int x584;                    // 0x584
        int x588;                    // 0x588
        int x58c;                    // 0x58c
        int x590;                    // 0x590
        int x594;                    // 0x594
        int x598;                    // 0x598
        int x59c;                    // 0x59c
        int x5a0;                    // 0x5a0
        int x5a4;                    // 0x5a4
        int x5a8;                    // 0x5a8
        u8 x5ac;                     // 0x5ac
        u8 stadium_kind;             // 0x5ad
        u8 x5ae;                     // 0x5ae
        u8 stadium_round;            // 0x5af
    } city;                          //
    int x5b0;                        // 0x5b0
    int x5b4;                        // 0x5b4
    int x5b8;                        // 0x5b8
    int x5bc;                        // 0x5bc
    int x5c0;                        // 0x5c0
    int x5c4;                        // 0x5c4
    int x5c8;                        // 0x5c8
    int x5cc;                        // 0x5cc
    int x5d0;                        // 0x5d0
    int x5d4;                        // 0x5d4
    int x5d8;                        // 0x5d8
    int x5dc;                        // 0x5dc
    int x5e0;                        // 0x5e0
    int x5e4;                        // 0x5e4
    int x5e8;                        // 0x5e8
    int x5ec;                        // 0x5ec
    int x5f0;                        // 0x5f0
    int x5f4;                        // 0x5f4
    int x5f8;                        // 0x5f8
    int x5fc;                        // 0x5fc
    int x600;                        // 0x600
    int x604;                        // 0x604
    int x608;                        // 0x608
    int x60c;                        // 0x60c
    int x610;                        // 0x610
    int x614;                        // 0x614
    int x618;                        // 0x618
    int x61c;                        // 0x61c
    int x620;                        // 0x620
    int x624;                        // 0x624
    int x628;                        // 0x628
    int x62c;                        // 0x62c
    int x630;                        // 0x630
    int x634;                        // 0x634
    int x638;                        // 0x638
    int x63c;                        // 0x63c
    int x640;                        // 0x640
    int x644;                        // 0x644
    int x648;                        // 0x648
    int x64c;                        // 0x64c
    int x650;                        // 0x650
    int x654;                        // 0x654
    int x658;                        // 0x658
    int x65c;                        // 0x65c
    int x660;                        // 0x660
    int x664;                        // 0x664
    int x668;                        // 0x668
    int x66c;                        // 0x66c
    int x670;                        // 0x670
    int x674;                        // 0x674
    int x678;                        // 0x678
    int x67c;                        // 0x67c
    int x680;                        // 0x680
    int x684;                        // 0x684
    int x688;                        // 0x688
    int x68c;                        // 0x68c
    int x690;                        // 0x690
    int x694;                        // 0x694
    int x698;                        // 0x698
    int x69c;                        // 0x69c
    int x6a0;                        // 0x6a0
    int x6a4;                        // 0x6a4
    int x6a8;                        // 0x6a8
    int x6ac;                        // 0x6ac
    int x6b0;                        // 0x6b0
    int x6b4;                        // 0x6b4
    int x6b8;                        // 0x6b8
    int x6bc;                        // 0x6bc
    int x6c0;                        // 0x6c0
    int x6c4;                        // 0x6c4
    int x6c8;                        // 0x6c8
    int x6cc;                        // 0x6cc
    int x6d0;                        // 0x6d0
    int x6d4;                        // 0x6d4
    int x6d8;                        // 0x6d8
    int x6dc;                        // 0x6dc
    int x6e0;                        // 0x6e0
    int x6e4;                        // 0x6e4
    int x6e8;                        // 0x6e8
    int x6ec;                        // 0x6ec
    int x6f0;                        // 0x6f0
    int x6f4;                        // 0x6f4
    int x6f8;                        // 0x6f8
    int x6fc;                        // 0x6fc
    int x700;                        // 0x700
    int x704;                        // 0x704
    int x708;                        // 0x708
    int x70c;                        // 0x70c
    int x710;                        // 0x710
    int x714;                        // 0x714
    int x718;                        // 0x718
    int x71c;                        // 0x71c
    int x720;                        // 0x720
    int x724;                        // 0x724
    int x728;                        // 0x728
    int x72c;                        // 0x72c
    int x730;                        // 0x730
    int x734;                        // 0x734
    int x738;                        // 0x738
    int x73c;                        // 0x73c
    int x740;                        // 0x740
    int x744;                        // 0x744
    int x748;                        // 0x748
    int x74c;                        // 0x74c
    int x750;                        // 0x750
    int x754;                        // 0x754
    int x758;                        // 0x758
    int x75c;                        // 0x75c
    int x760;                        // 0x760
    int x764;                        // 0x764
    int x768;                        // 0x768
    int x76c;                        // 0x76c
    int x770;                        // 0x770
    int x774;                        // 0x774
    int x778;                        // 0x778
    int x77c;                        // 0x77c
    int x780;                        // 0x780
    int x784;                        // 0x784
    int x788;                        // 0x788
    int x78c;                        // 0x78c
    int x790;                        // 0x790
    int x794;                        // 0x794
    int x798;                        // 0x798
    int x79c;                        // 0x79c
    int x7a0;                        // 0x7a0
    int x7a4;                        // 0x7a4
    int x7a8;                        // 0x7a8
    int x7ac;                        // 0x7ac
    int x7b0;                        // 0x7b0
    int x7b4;                        // 0x7b4
    int x7b8;                        // 0x7b8
    int x7bc;                        // 0x7bc
    int x7c0;                        // 0x7c0
    int x7c4;                        // 0x7c4
    int x7c8;                        // 0x7c8
    int x7cc;                        // 0x7cc
    int x7d0;                        // 0x7d0
    MajorKind major_cur : 8;         // 0x7d4
    MajorKind major_pending : 8;     // 0x7d5
    u8 request_major_exit;           // 0x7d6
    int x7d8;                        // 0x7d8
    int x7dc;                        // 0x7dc
    HSD_Update update;               // 0x7e0
    int x814;                        // 0x814
    int x818;                        // 0x818
    int x81c;                        // 0x81c
    int x820;                        // 0x820
    int x824;                        // 0x824
    int x828;                        // 0x828
    int x82c;                        // 0x82c
    u8 pause_ply;                    // 0x830, player who paused the game
    u8 pause_cursor;                 // 0x831, 0 = resume, 1 = start over, 2 = exit
    u8 intro_state;                  // 0x832
    u8 frames_in_second;             // 0x833
    int seconds_passed;              // 0x834
    int pause_delay;                 // 0x838
    int x83c;                        // 0x83c
    int x840;                        // 0x840
    int x844;                        // 0x844
    int x848;                        // 0x848
    int x84c;                        // 0x84c
    int x850;                        // 0x850
    int x854;                        // 0x854
    int x858;                        // 0x858
    int x85c;                        // 0x85c
    int x860;                        // 0x860
    int x864;                        // 0x864
    int x868;                        // 0x868
    int x86c;                        // 0x86c
    int x870;                        // 0x870
    int x874;                        // 0x874
    int x878;                        // 0x878
    int x87c;                        // 0x87c
    int x880;                        // 0x880
    int x884;                        // 0x884
    int x888;                        // 0x888
    int x88c;                        // 0x88c
    int x890;                        // 0x890
    int x894;                        // 0x894
    int x898;                        // 0x898
    int x89c;                        // 0x89c
    int x8a0;                        // 0x8a0
    int x8a4;                        // 0x8a4
    int x8a8;                        // 0x8a8
    int x8ac;                        // 0x8ac
    int x8b0;                        // 0x8b0
    int x8b4;                        // 0x8b4
    int x8b8;                        // 0x8b8
    int x8bc;                        // 0x8bc
    int x8c0;                        // 0x8c0
    int x8c4;                        // 0x8c4
    int x8c8;                        // 0x8c8
    int x8cc;                        // 0x8cc
    int x8d0;                        // 0x8d0
    int x8d4;                        // 0x8d4
    int x8d8;                        // 0x8d8
    int x8dc;                        // 0x8dc
    int x8e0;                        // 0x8e0
    int x8e4;                        // 0x8e4
    int x8e8;                        // 0x8e8
    int x8ec;                        // 0x8ec
    int x8f0;                        // 0x8f0
    int x8f4;                        // 0x8f4
    int x8f8;                        // 0x8f8
    int x8fc;                        // 0x8fc
    int x900;                        // 0x900
    int x904;                        // 0x904
    int x908;                        // 0x908
    int x90c;                        // 0x90c
    int x910;                        // 0x910
    int x914;                        // 0x914
    int x918;                        // 0x918
    int x91c;                        // 0x91c
    int x920;                        // 0x920
    int x924;                        // 0x924
    int x928;                        // 0x928
    int x92c;                        // 0x92c
    int x930;                        // 0x930
    int x934;                        // 0x934
    int x938;                        // 0x938
    int x93c;                        // 0x93c
    int x940;                        // 0x940
    int x944;                        // 0x944
    int x948;                        // 0x948
    int x94c;                        // 0x94c
    int x950;                        // 0x950
    int x954;                        // 0x954
    int x958;                        // 0x958
    int x95c;                        // 0x95c
    int x960;                        // 0x960
    int x964;                        // 0x964
    int x968;                        // 0x968
    int x96c;                        // 0x96c
    int x970;                        // 0x970
    int x974;                        // 0x974
    int x978;                        // 0x978
    int x97c;                        // 0x97c
    int x980;                        // 0x980
    int x984;                        // 0x984
    int x988;                        // 0x988
    int x98c;                        // 0x98c
    int x990;                        // 0x990
    int x994;                        // 0x994
    int x998;                        // 0x998
    int x99c;                        // 0x99c
    int x9a0;                        // 0x9a0
    int x9a4;                        // 0x9a4
    int x9a8;                        // 0x9a8
    int x9ac;                        // 0x9ac
    int x9b0;                        // 0x9b0
    int x9b4;                        // 0x9b4
    int x9b8;                        // 0x9b8
    int x9bc;                        // 0x9bc
    int x9c0;                        // 0x9c0
    int x9c4;                        // 0x9c4
    int x9c8;                        // 0x9c8
    int x9cc;                        // 0x9cc
    int x9d0;                        // 0x9d0
    int x9d4;                        // 0x9d4
    int x9d8;                        // 0x9d8
    int x9dc;                        // 0x9dc
    int x9e0;                        // 0x9e0
    int x9e4;                        // 0x9e4
    int x9e8;                        // 0x9e8
    int x9ec;                        // 0x9ec
    int x9f0;                        // 0x9f0
    int x9f4;                        // 0x9f4
    int x9f8;                        // 0x9f8
    int x9fc;                        // 0x9fc
    int xa00;                        // 0xa00
    int xa04;                        // 0xa04
    int xa08;                        // 0xa08
    int xa0c;                        // 0xa0c
    int xa10;                        // 0xa10
    int xa14;                        // 0xa14
    int xa18;                        // 0xa18
    int xa1c;                        // 0xa1c
    int xa20;                        // 0xa20
    int xa24;                        // 0xa24
    int xa28;                        // 0xa28
    int xa2c;                        // 0xa2c
    int xa30;                        // 0xa30
    int xa34;                        // 0xa34
    int destruction_derby_ko_num[4]; // 0xa38, indexed by ply
    int xa48;                        // 0xa48
    int xa4c;                        // 0xa4c
    int xa50;                        // 0xa50
    int xa54;                        // 0xa54
    int xa58;                        // 0xa58
    int xa5c;                        // 0xa5c
    int xa60;                        // 0xa60
    int xa64;                        // 0xa64
    int xa68;                        // 0xa68
    int xa6c;                        // 0xa6c
    int xa70;                        // 0xa70
    int xa74;                        // 0xa74
    int xa78;                        // 0xa78
    int xa7c;                        // 0xa7c
    int xa80;                        // 0xa80
    int xa84;                        // 0xa84
    int xa88;                        // 0xa88
    int xa8c;                        // 0xa8c
    int xa90;                        // 0xa90
    u8 city_kind;                    // 0xa94, 5 = main city trial. stadium modes are derived here @ 8004051c (0xE is destruction derby)
    u8 xa95;                         // 0xa95
    u8 xa96;                         // 0xa96
    u8 stage_kind;                   // 0xa97, StageKind
    u8 bgm_override;                 // 0xa98, when this is not 1, it plays it as the song id
    u8 is_always_ura_bgm;            // 0xa99
    u8 xa9a;                         // 0xa9a
    u8 xa9b;                         // 0xa9b
    int xa9c;                        // 0xa9c
    int xaa0;                        // 0xaa0
    u8 xaa4;                         // 0xaa4
    u8 xaa5;                         // 0xaa5
    u8 xaa6;                         // 0xaa6
    u8 xaa7_80 : 1;                  // 0xaa7, 0x80
    u8 xaa7_40 : 1;                  // 0xaa7, 0x40
    u8 is_play_music : 1;            // 0xaa7, 0x20
    u8 xaa7_10 : 1;                  // 0xaa7, 0x10
    u8 xaa7_08 : 1;                  // 0xaa7, 0x08
    u8 xaa7_04 : 1;                  // 0xaa7, 0x04
    u8 xaa7_02 : 1;                  // 0xaa7, 0x02
    u8 xaa7_01 : 1;                  // 0xaa7, 0x01
    int xaa8;                        // 0xaa8
    int xaac;                        // 0xaac
    int xab0;                        // 0xab0
    int xab4;                        // 0xab4
    int xab8;                        // 0xab8
    int xabc;                        // 0xabc
    int xac0;                        // 0xac0
    int xac4;                        // 0xac4
    struct                           // 0xac8
    {                                //
        u8 x0;                       // 0x00
        u8 rider_kind;               // 0x01
        u8 is_bike;                  // 0x02
        u8 machine_kind;             // 0x03
        u8 color;                    // 0x04
        u8 x5;                       // 0x05
        u8 ply;                      // 0x06
        u8 x7;                       // 0x07
        int x8;                      // 0x08
        int xc;                      // 0x0c
        int x10;                     // 0x10
        int x14;                     // 0x14
        int x18;                     // 0x18
        int x1c;                     // 0x1c
        int x20;                     // 0x20
        int x24;                     // 0x24
        int x28;                     // 0x28
        int x2c;                     // 0x2c
    } ply_data[4];                   //
    int xb88;                        // 0xb88
    int xb8c;                        // 0xb8c
    int xb90;                        // 0xb90
    int xb94;                        // 0xb94
    int xb98;                        // 0xb98
    int xb9c;                        // 0xb9c
    int xba0;                        // 0xba0
    int xba4;                        // 0xba4
    int xba8;                        // 0xba8
    int xbac;                        // 0xbac
    int xbb0;                        // 0xbb0
    int xbb4;                        // 0xbb4
    int xbb8;                        // 0xbb8
    int xbbc;                        // 0xbbc
    int xbc0;                        // 0xbc0
    int xbc4;                        // 0xbc4
    StadiumResults stadium_results;  // 0xbc8
    int xc24;                        // 0xc24
    int xc28;                        // 0xc28
    int xc2c;                        // 0xc2c
    int xc30;                        // 0xc30
    int xc34;                        // 0xc34
    int xc38;                        // 0xc38
    int xc3c;                        // 0xc3c
    int xc40;                        // 0xc40
    int xc44;                        // 0xc44
    int xc48;                        // 0xc48
    int xc4c;                        // 0xc4c
    int xc50;                        // 0xc50
    int xc54;                        // 0xc54
    int xc58;                        // 0xc58
    int xc5c;                        // 0xc5c
    int xc60;                        // 0xc60
    int xc64;                        // 0xc64
    int xc68;                        // 0xc68
    int xc6c;                        // 0xc6c
    int xc70;                        // 0xc70
    int xc74;                        // 0xc74
    int xc78;                        // 0xc78
    int xc7c;                        // 0xc7c
    int xc80;                        // 0xc80
    int xc84;                        // 0xc84
    int xc88;                        // 0xc88
    int xc8c;                        // 0xc8c
    int xc90;                        // 0xc90
    int xc94;                        // 0xc94
    int xc98;                        // 0xc98
    int xc9c;                        // 0xc9c
    int xca0;                        // 0xca0
    int xca4;                        // 0xca4
    int xca8;                        // 0xca8
    int xcac;                        // 0xcac
    int xcb0;                        // 0xcb0
    int xcb4;                        // 0xcb4
    int xcb8;                        // 0xcb8
    int xcbc;                        // 0xcbc
    int xcc0;                        // 0xcc0
    int xcc4;                        // 0xcc4
    int xcc8;                        // 0xcc8
    int xccc;                        // 0xccc
    int xcd0;                        // 0xcd0
    int xcd4;                        // 0xcd4
    int xcd8;                        // 0xcd8
    int xcdc;                        // 0xcdc
    int xce0;                        // 0xce0
    int xce4;                        // 0xce4
    int xce8;                        // 0xce8
    int xcec;                        // 0xcec
    int xcf0;                        // 0xcf0
    int xcf4;                        // 0xcf4
    int xcf8;                        // 0xcf8
    int xcfc;                        // 0xcfc
    int xd00;                        // 0xd00
    int xd04;                        // 0xd04
    int xd08;                        // 0xd08
    int xd0c;                        // 0xd0c
    int xd10;                        // 0xd10
    int xd14;                        // 0xd14
    int xd18;                        // 0xd18
    int xd1c;                        // 0xd1c
    int xd20;                        // 0xd20
    int xd24;                        // 0xd24
    int xd28;                        // 0xd28
    int xd2c;                        // 0xd2c
    int xd30;                        // 0xd30
    int xd34;                        // 0xd34
    int xd38;                        // 0xd38
    int xd3c;                        // 0xd3c
    int xd40;                        // 0xd40
    int xd44;                        // 0xd44
    int xd48;                        // 0xd48
    int xd4c;                        // 0xd4c
    int xd50;                        // 0xd50
    int xd54;                        // 0xd54
    int xd58;                        // 0xd58
    int xd5c;                        // 0xd5c
    int xd60;                        // 0xd60
    int xd64;                        // 0xd64
    int xd68;                        // 0xd68
    int xd6c;                        // 0xd6c
    int xd70;                        // 0xd70
    int xd74;                        // 0xd74
    int xd78;                        // 0xd78
    int xd7c;                        // 0xd7c
    int xd80;                        // 0xd80
    int xd84;                        // 0xd84
    int xd88;                        // 0xd88
    int xd8c;                        // 0xd8c
    int xd90;                        // 0xd90
    int xd94;                        // 0xd94
    int xd98;                        // 0xd98
    int xd9c;                        // 0xd9c
    int xda0;                        // 0xda0
    int xda4;                        // 0xda4
    int xda8;                        // 0xda8
    int xdac;                        // 0xdac
    int xdb0;                        // 0xdb0
    int xdb4;                        // 0xdb4
    int xdb8;                        // 0xdb8
    int xdbc;                        // 0xdbc
    int xdc0;                        // 0xdc0
    int xdc4;                        // 0xdc4
    int xdc8;                        // 0xdc8
    int xdcc;                        // 0xdcc
    int xdd0;                        // 0xdd0
    int xdd4;                        // 0xdd4
    int xdd8;                        // 0xdd8
    int xddc;                        // 0xddc
    int xde0;                        // 0xde0
    int xde4;                        // 0xde4
    int xde8;                        // 0xde8
    int xdec;                        // 0xdec
    int xdf0;                        // 0xdf0
    int xdf4;                        // 0xdf4
    int xdf8;                        // 0xdf8
    int xdfc;                        // 0xdfc
    int xe00;                        // 0xe00
    int xe04;                        // 0xe04
    int xe08;                        // 0xe08
    int xe0c;                        // 0xe0c
    int xe10;                        // 0xe10
    int xe14;                        // 0xe14
    int xe18;                        // 0xe18
    int xe1c;                        // 0xe1c
    int xe20;                        // 0xe20
    int xe24;                        // 0xe24
    int xe28;                        // 0xe28
    int xe2c;                        // 0xe2c
    int xe30;                        // 0xe30
    int xe34;                        // 0xe34
    int xe38;                        // 0xe38
    int xe3c;                        // 0xe3c
    int xe40;                        // 0xe40
    int xe44;                        // 0xe44
    int xe48;                        // 0xe48
    int xe4c;                        // 0xe4c
    int xe50;                        // 0xe50
    int xe54;                        // 0xe54
    int xe58;                        // 0xe58
    int xe5c;                        // 0xe5c
    int xe60;                        // 0xe60
    int xe64;                        // 0xe64
    int xe68;                        // 0xe68
    int xe6c;                        // 0xe6c
    int xe70;                        // 0xe70
    int xe74;                        // 0xe74
    int xe78;                        // 0xe78
    int xe7c;                        // 0xe7c
    int xe80;                        // 0xe80
    int xe84;                        // 0xe84
    int xe88;                        // 0xe88
    int xe8c;                        // 0xe8c
    int xe90;                        // 0xe90
    int xe94;                        // 0xe94
    int xe98;                        // 0xe98
    int xe9c;                        // 0xe9c
    int xea0;                        // 0xea0
    int xea4;                        // 0xea4
    int xea8;                        // 0xea8
    int xeac;                        // 0xeac
    int xeb0;                        // 0xeb0
    int xeb4;                        // 0xeb4
    int xeb8;                        // 0xeb8
    int xebc;                        // 0xebc
    int xec0;                        // 0xec0
    int xec4;                        // 0xec4
    int xec8;                        // 0xec8
    int xecc;                        // 0xecc
    GOBJ *clearchecker_gobj;         // 0xed0
    int xed4;                        // 0xed4
    int xed8;                        // 0xed8
    int xedc;                        // 0xedc
    int xee0;                        // 0xee0
    int xee4;                        // 0xee4
    int xee8;                        // 0xee8
    int xeec;                        // 0xeec
    int xef0;                        // 0xef0
    int xef4;                        // 0xef4
    int xef8;                        // 0xef8
    int xefc;                        // 0xefc
} GameData;

typedef struct Game3dData
{
    u8 plyview_num;                               // 0x0
    s8 plyview_lookup[4];                         // 0x1, -1 for not present?
    HSD_SObjDesc *hud_sobj;                       // 0x8
    int xc;                                       // 0xc
    int x10;                                      // 0x10
    int x14;                                      // 0x14
    int x18;                                      // 0x18
    int x1c;                                      // 0x1c
    int x20;                                      // 0x20
    int x24;                                      // 0x24
    int x28;                                      // 0x28
    int x2c;                                      // 0x2c
    int x30;                                      // 0x30
    int x34;                                      // 0x34
    int x38;                                      // 0x38
    int x3c;                                      // 0x3c
    int x40;                                      // 0x40
    int x44;                                      // 0x44
    int x48;                                      // 0x48
    int x4c;                                      // 0x4c
    int x50;                                      // 0x50
    int x54;                                      // 0x54
    int x58;                                      // 0x58
    int x5c;                                      // 0x5c
    int x60;                                      // 0x60
    int x64;                                      // 0x64
    int x68;                                      // 0x68
    int x6c;                                      // 0x6c
    int x70;                                      // 0x70
    int x74;                                      // 0x74
    int x78;                                      // 0x78
    int x7c;                                      // 0x7c
    int x80;                                      // 0x80
    int x84;                                      // 0x84
    int x88;                                      // 0x88
    int x8c;                                      // 0x8c
    int x90;                                      // 0x90
    int x94;                                      // 0x94
    int x98;                                      // 0x98
    int x9c;                                      // 0x9c
    int xa0;                                      // 0xa0
    int xa4;                                      // 0xa4
    int xa8;                                      // 0xa8
    int xac;                                      // 0xac
    int xb0;                                      // 0xb0
    int xb4;                                      // 0xb4
    int xb8;                                      // 0xb8
    int xbc;                                      // 0xbc
    int xc0;                                      // 0xc0
    int xc4;                                      // 0xc4
    int xc8;                                      // 0xc8
    int xcc;                                      // 0xcc
    int xd0;                                      // 0xd0
    int xd4;                                      // 0xd4
    int xd8;                                      // 0xd8
    int xdc;                                      // 0xdc
    int xe0;                                      // 0xe0
    int xe4;                                      // 0xe4
    int xe8;                                      // 0xe8
    int xec;                                      // 0xec
    int xf0;                                      // 0xf0
    int xf4;                                      // 0xf4
    int xf8;                                      // 0xf8
    int xfc;                                      // 0xfc
    int x100;                                     // 0x100
    int x104;                                     // 0x104
    int x108;                                     // 0x108
    int x10c;                                     // 0x10c
    int x110;                                     // 0x110
    int x114;                                     // 0x114
    int x118;                                     // 0x118
    int x11c;                                     // 0x11c
    int x120;                                     // 0x120
    int x124;                                     // 0x124
    int x128;                                     // 0x128
    int x12c;                                     // 0x12c
    int x130;                                     // 0x130
    int x134;                                     // 0x134
    int x138;                                     // 0x138
    JOBJSet **ScInfPliconKirby_scene_models;      // 0x13c
    JOBJSet **ScInfPliconDedede_scene_models;     // 0x140
    JOBJSet **ScInfPliconMetaKnight_scene_models; // 0x144
    int x148;                                     // 0x148
    int x14c;                                     // 0x14c
    int x150;                                     // 0x150
    int x154;                                     // 0x154
    int x158;                                     // 0x158
    int x15c;                                     // 0x15c
    int x160;                                     // 0x160
    int x164;                                     // 0x164
    int x168;                                     // 0x168
    int x16c;                                     // 0x16c
    int x170;                                     // 0x170
    int x174;                                     // 0x174
    int x178;                                     // 0x178
    int x17c;                                     // 0x17c
    int x180;                                     // 0x180
    int x184;                                     // 0x184
    int x188;                                     // 0x188
    int x18c;                                     // 0x18c
    int x190;                                     // 0x190
    int x194;                                     // 0x194
    int x198;                                     // 0x198
    int x19c;                                     // 0x19c
    int x1a0;                                     // 0x1a0
    int x1a4;                                     // 0x1a4
    int x1a8;                                     // 0x1a8
    int x1ac;                                     // 0x1ac
    int x1b0;                                     // 0x1b0
    int x1b4;                                     // 0x1b4
    int x1b8;                                     // 0x1b8
    int x1bc;                                     // 0x1bc
    int x1c0;                                     // 0x1c0
    int x1c4;                                     // 0x1c4
    int x1c8;                                     // 0x1c8
    int x1cc;                                     // 0x1cc
    int x1d0;                                     // 0x1d0
    int x1d4;                                     // 0x1d4
    int x1d8;                                     // 0x1d8
    int x1dc;                                     // 0x1dc
    int x1e0;                                     // 0x1e0
    int x1e4;                                     // 0x1e4
    int x1e8;                                     // 0x1e8
    int x1ec;                                     // 0x1ec
    int x1f0;                                     // 0x1f0
    int x1f4;                                     // 0x1f4
    int x1f8;                                     // 0x1f8
    int x1fc;                                     // 0x1fc
    int x200;                                     // 0x200
    int x204;                                     // 0x204
    int x208;                                     // 0x208
    int x20c;                                     // 0x20c
    int x210;                                     // 0x210
    int x214;                                     // 0x214
    int x218;                                     // 0x218
    int x21c;                                     // 0x21c
    int x220;                                     // 0x220
    int x224;                                     // 0x224
    int x228;                                     // 0x228
    int x22c;                                     // 0x22c
    int x230;                                     // 0x230
    int x234;                                     // 0x234
    int x238;                                     // 0x238
    int x23c;                                     // 0x23c
    int x240;                                     // 0x240
    int x244;                                     // 0x244
    int x248;                                     // 0x248
    int x24c;                                     // 0x24c
    int x250;                                     // 0x250
    int x254;                                     // 0x254
    int x258;                                     // 0x258
    int x25c;                                     // 0x25c
    int x260;                                     // 0x260
    int x264;                                     // 0x264
    int x268;                                     // 0x268
    int x26c;                                     // 0x26c
    int x270;                                     // 0x270
    int x274;                                     // 0x274
    int x278;                                     // 0x278
    int x27c;                                     // 0x27c
    int x280;                                     // 0x280
    int x284;                                     // 0x284
    int x288;                                     // 0x288
    int x28c;                                     // 0x28c
    int x290;                                     // 0x290
    int x294;                                     // 0x294
    int x298;                                     // 0x298
    int x29c;                                     // 0x29c
    int x2a0;                                     // 0x2a0
    int x2a4;                                     // 0x2a4
    int x2a8;                                     // 0x2a8
    int x2ac;                                     // 0x2ac
    int x2b0;                                     // 0x2b0
    int x2b4;                                     // 0x2b4
    int x2b8;                                     // 0x2b8
    int x2bc;                                     // 0x2bc
    int x2c0;                                     // 0x2c0
    int x2c4;                                     // 0x2c4
    int x2c8;                                     // 0x2c8
    int x2cc;                                     // 0x2cc
    int x2d0;                                     // 0x2d0
    int x2d4;                                     // 0x2d4
    int x2d8;                                     // 0x2d8
    int x2dc;                                     // 0x2dc
    int x2e0;                                     // 0x2e0
    int x2e4;                                     // 0x2e4
    int x2e8;                                     // 0x2e8
    int x2ec;                                     // 0x2ec
    int x2f0;                                     // 0x2f0
    int x2f4;                                     // 0x2f4
    int x2f8;                                     // 0x2f8
    int x2fc;                                     // 0x2fc
    int x300;                                     // 0x300
    int x304;                                     // 0x304
    int x308;                                     // 0x308
    int x30c;                                     // 0x30c
    int x310;                                     // 0x310
    int x314;                                     // 0x314
    int x318;                                     // 0x318
    int x31c;                                     // 0x31c
    int x320;                                     // 0x320
    int x324;                                     // 0x324
    int x328;                                     // 0x328
    int x32c;                                     // 0x32c
    int x330;                                     // 0x330
    int x334;                                     // 0x334
    int x338;                                     // 0x338
    int x33c;                                     // 0x33c
    int x340;                                     // 0x340
    int x344;                                     // 0x344
    int x348;                                     // 0x348
    int x34c;                                     // 0x34c
    int x350;                                     // 0x350
    int x354;                                     // 0x354
    int x358;                                     // 0x358
    int x35c;                                     // 0x35c
    int x360;                                     // 0x360
    int x364;                                     // 0x364
    int x368;                                     // 0x368
    int x36c;                                     // 0x36c
    int x370;                                     // 0x370
    int x374;                                     // 0x374
    int x378;                                     // 0x378
    int x37c;                                     // 0x37c
    int x380;                                     // 0x380
    int x384;                                     // 0x384
    int x388;                                     // 0x388
    int x38c;                                     // 0x38c
    int x390;                                     // 0x390
    u16 time_seconds;                             // 0x394
    u8 menu_stadium_selection;                    // 0x396, selected from settings, sub 1 to get StadiumGroup value
    u8 game_tempo : 2;                            // 0x397, 0xC0 (1 = normal = 2 is slow)
    u8 events_enable : 1;                         // 0x397, 0x20
    u8 x397_x10 : 1;                              // 0x397, 0x10
    u8 x397_x08 : 1;                              // 0x397, 0x08
    u8 x397_x04 : 1;                              // 0x397, 0x04
    u8 x397_x02 : 1;                              // 0x397, 0x02
    u8 x397_x01 : 1;                              // 0x397, 0x01
    u8 x398;                                      // 0x398,
    CityMode city_mode : 8;                       // 0x399, which mode was selected from the menu
    int x39c;                                     // 0x39c
    int x3a0;                                     // 0x3a0
    int x3a4;                                     // 0x3a4
    int x3a8;                                     // 0x3a8
    int x3ac;                                     // 0x3ac
    int x3b0;                                     // 0x3b0
    int x3b4;                                     // 0x3b4
    int x3b8;                                     // 0x3b8
    int x3bc;                                     // 0x3bc
    int x3c0;                                     // 0x3c0
    int x3c4;                                     // 0x3c4
    int x3c8;                                     // 0x3c8
    int x3cc;                                     // 0x3cc
    int x3d0;                                     // 0x3d0
    int x3d4;                                     // 0x3d4
    int x3d8;                                     // 0x3d8
    int x3dc;                                     // 0x3dc
    int x3e0;                                     // 0x3e0
    int x3e4;                                     // 0x3e4
    int x3e8;                                     // 0x3e8
    int x3ec;                                     // 0x3ec
    int x3f0;                                     // 0x3f0
    int x3f4;                                     // 0x3f4
    int x3f8;                                     // 0x3f8
    int x3fc;                                     // 0x3fc
    int x400;                                     // 0x400
    int x404;                                     // 0x404
    int x408;                                     // 0x408
    int x40c;                                     // 0x40c
    int x410;                                     // 0x410
    int x414;                                     // 0x414
    int x418;                                     // 0x418
    int x41c;                                     // 0x41c
    int x420;                                     // 0x420
    int x424;                                     // 0x424
    int x428;                                     // 0x428
    int x42c;                                     // 0x42c
    int x430;                                     // 0x430
    int x434;                                     // 0x434
    int x438;                                     // 0x438
    int x43c;                                     // 0x43c
    int x440;                                     // 0x440
    int x444;                                     // 0x444
    int x448;                                     // 0x448
    int x44c;                                     // 0x44c
    int x450;                                     // 0x450
    u8 is_bike[5];                                // 0x454
    u8 machine_kind[5];                           // 0x459
    u8 prev_stadium_kind[5];                      // 0x45e
    int x464;                                     // 0x464
    int x468;                                     // 0x468
    int x46c;                                     // 0x46c
    int x470;                                     // 0x470
    int x474;                                     // 0x474
    int x478;                                     // 0x478
    int x47c;                                     // 0x47c
    int x480;                                     // 0x480
    int x484;                                     // 0x484
    int x488;                                     // 0x488
    int x48c;                                     // 0x48c
    int x490;                                     // 0x490
    int x494;                                     // 0x494
    int x498;                                     // 0x498
    int x49c;                                     // 0x49c
    int x4a0;                                     // 0x4a0
    int x4a4;                                     // 0x4a4
    int x4a8;                                     // 0x4a8
    int x4ac;                                     // 0x4ac
    int x4b0;                                     // 0x4b0
    int x4b4;                                     // 0x4b4
    int x4b8;                                     // 0x4b8
    int x4bc;                                     // 0x4bc
    int x4c0;                                     // 0x4c0
    int x4c4;                                     // 0x4c4
    int x4c8;                                     // 0x4c8
    int x4cc;                                     // 0x4cc
    int x4d0;                                     // 0x4d0
    int x4d4;                                     // 0x4d4
    int x4d8;                                     // 0x4d8
    int x4dc;                                     // 0x4dc
    int x4e0;                                     // 0x4e0
    int x4e4;                                     // 0x4e4
    int x4e8;                                     // 0x4e8
    int x4ec;                                     // 0x4ec
    int x4f0;                                     // 0x4f0
    int x4f4;                                     // 0x4f4
    int x4f8;                                     // 0x4f8
    int x4fc;                                     // 0x4fc
    int x500;                                     // 0x500
    int x504;                                     // 0x504
    int x508;                                     // 0x508
    int x50c;                                     // 0x50c
    int x510;                                     // 0x510
    int x514;                                     // 0x514
    int x518;                                     // 0x518
    int x51c;                                     // 0x51c
    int x520;                                     // 0x520
    int x524;                                     // 0x524
    int x528;                                     // 0x528
    int x52c;                                     // 0x52c
    int x530;                                     // 0x530
    int x534;                                     // 0x534
    int x538;                                     // 0x538
    int x53c;                                     // 0x53c
    int x540;                                     // 0x540
    int x544;                                     // 0x544
    int x548;                                     // 0x548
    int x54c;                                     // 0x54c
    int x550;                                     // 0x550
    int x554;                                     // 0x554
    int x558;                                     // 0x558
    int x55c;                                     // 0x55c
    int x560;                                     // 0x560
    int x564;                                     // 0x564
    int x568;                                     // 0x568
    int x56c;                                     // 0x56c
    int x570;                                     // 0x570
    int x574;                                     // 0x574
    int x578;                                     // 0x578
    int x57c;                                     // 0x57c
    int x580;                                     // 0x580
    int x584;                                     // 0x584
    int x588;                                     // 0x588
    int x58c;                                     // 0x58c
    int x590;                                     // 0x590
    int x594;                                     // 0x594
    int x598;                                     // 0x598
    int x59c;                                     // 0x59c
    int x5a0;                                     // 0x5a0
    int x5a4;                                     // 0x5a4
    int x5a8;                                     // 0x5a8
    u8 x5ac;                                      // 0x5ac
    u8 stadium_kind;                              // 0x5ad
    u8 x5ae;                                      // 0x5ae
    u8 x5af;                                      // 0x5af
    int x5b0;                                     // 0x5b0
    int x5b4;                                     // 0x5b4
    int x5b8;                                     // 0x5b8
    int x5bc;                                     // 0x5bc
    int x5c0;                                     // 0x5c0
    int x5c4;                                     // 0x5c4
    int x5c8;                                     // 0x5c8
    int x5cc;                                     // 0x5cc
    int x5d0;                                     // 0x5d0
    int x5d4;                                     // 0x5d4
    int x5d8;                                     // 0x5d8
    int x5dc;                                     // 0x5dc
    int x5e0;                                     // 0x5e0
    int x5e4;                                     // 0x5e4
    int x5e8;                                     // 0x5e8
    int x5ec;                                     // 0x5ec
    int x5f0;                                     // 0x5f0
    int x5f4;                                     // 0x5f4
    int x5f8;                                     // 0x5f8
    int x5fc;                                     // 0x5fc
    int x600;                                     // 0x600
    int x604;                                     // 0x604
    int x608;                                     // 0x608
    int x60c;                                     // 0x60c
    int x610;                                     // 0x610
    int x614;                                     // 0x614
    int x618;                                     // 0x618
    int x61c;                                     // 0x61c
    int x620;                                     // 0x620
    int x624;                                     // 0x624
    int x628;                                     // 0x628
    int x62c;                                     // 0x62c
    int x630;                                     // 0x630
    int x634;                                     // 0x634
    int x638;                                     // 0x638
    GOBJ *cityui_statchart_gobj[5];               // 0x63c, indexed by ply
    GOBJ *cityui_statbar_gobj[4][9];              // 0x650,
    int x6e0;                                     // 0x6e0
    int x6e4;                                     // 0x6e4
    int x6e8;                                     // 0x6e8
    int x6ec;                                     // 0x6ec
    int x6f0;                                     // 0x6f0
    int x6f4;                                     // 0x6f4
    int x6f8;                                     // 0x6f8
    int x6fc;                                     // 0x6fc
    int x700;                                     // 0x700
    int x704;                                     // 0x704
    int x708;                                     // 0x708
    int x70c;                                     // 0x70c
    int x710;                                     // 0x710
    int x714;                                     // 0x714
    int x718;                                     // 0x718
    int x71c;                                     // 0x71c
    int x720;                                     // 0x720
    int x724;                                     // 0x724
    int x728;                                     // 0x728
    int x72c;                                     // 0x72c
    int x730;                                     // 0x730
    int x734;                                     // 0x734
    int x738;                                     // 0x738
    int x73c;                                     // 0x73c
    int x740;                                     // 0x740
    int x744;                                     // 0x744
    int x748;                                     // 0x748
    int x74c;                                     // 0x74c
    int x750;                                     // 0x750
    int x754;                                     // 0x754
    int x758;                                     // 0x758
    int x75c;                                     // 0x75c
    int x760;                                     // 0x760
    int x764;                                     // 0x764
    int x768;                                     // 0x768
    int x76c;                                     // 0x76c
    int x770;                                     // 0x770
    int x774;                                     // 0x774
    int x778;                                     // 0x778
    int x77c;                                     // 0x77c
    int x780;                                     // 0x780
    int x784;                                     // 0x784
    int x788;                                     // 0x788
    int x78c;                                     // 0x78c
    int x790;                                     // 0x790
    int x794;                                     // 0x794
    int x798;                                     // 0x798
    int x79c;                                     // 0x79c
    int x7a0;                                     // 0x7a0
    int x7a4;                                     // 0x7a4
    int x7a8;                                     // 0x7a8
    int x7ac;                                     // 0x7ac
    int x7b0;                                     // 0x7b0
    int x7b4;                                     // 0x7b4
    int x7b8;                                     // 0x7b8
    int x7bc;                                     // 0x7bc
    int x7c0;                                     // 0x7c0
    int x7c4;                                     // 0x7c4
    int x7c8;                                     // 0x7c8
    int x7cc;                                     // 0x7cc
    int x7d0;                                     // 0x7d0
    int x7d4;                                     // 0x7d4
    int x7d8;                                     // 0x7d8
    int x7dc;                                     // 0x7dc
    int x7e0;                                     // 0x7e0
    int x7e4;                                     // 0x7e4
    int x7e8;                                     // 0x7e8
    int x7ec;                                     // 0x7ec
    int x7f0;                                     // 0x7f0
    int x7f4;                                     // 0x7f4
    int x7f8;                                     // 0x7f8
    int x7fc;                                     // 0x7fc
    int x800;                                     // 0x800
    int x804;                                     // 0x804
    int x808;                                     // 0x808
    int x80c;                                     // 0x80c
    int x810;                                     // 0x810
    int x814;                                     // 0x814
    int x818;                                     // 0x818
    int x81c;                                     // 0x81c
    int x820;                                     // 0x820
    int x824;                                     // 0x824
    int x828;                                     // 0x828
    int x82c;                                     // 0x82c
    u8 x830;                                      // 0x830
    u8 x831;                                      // 0x831
    u8 x832;                                      // 0x832
    u8 frames_in_second;                          // 0x833
    int seconds_passed;                           // 0x834
    int x838;                                     // 0x838
    int x83c;                                     // 0x83c
    int x840;                                     // 0x840
    int x844;                                     // 0x844
    int x848;                                     // 0x848
    int x84c;                                     // 0x84c
    int x850;                                     // 0x850
    int x854;                                     // 0x854
    int x858;                                     // 0x858
    int x85c;                                     // 0x85c
    int x860;                                     // 0x860
    int x864;                                     // 0x864
    int x868;                                     // 0x868
    int x86c;                                     // 0x86c
    int x870;                                     // 0x870
    int x874;                                     // 0x874
    int x878;                                     // 0x878
    int x87c;                                     // 0x87c
    int x880;                                     // 0x880
    int x884;                                     // 0x884
    int x888;                                     // 0x888
    int x88c;                                     // 0x88c
    int x890;                                     // 0x890
    int x894;                                     // 0x894
    int x898;                                     // 0x898
    int x89c;                                     // 0x89c
    int x8a0;                                     // 0x8a0
    int x8a4;                                     // 0x8a4
    int x8a8;                                     // 0x8a8
    int x8ac;                                     // 0x8ac
    int x8b0;                                     // 0x8b0
    int x8b4;                                     // 0x8b4
    int x8b8;                                     // 0x8b8
    int x8bc;                                     // 0x8bc
    int x8c0;                                     // 0x8c0
    int x8c4;                                     // 0x8c4
    int x8c8;                                     // 0x8c8
    int x8cc;                                     // 0x8cc
    int x8d0;                                     // 0x8d0
    int x8d4;                                     // 0x8d4
    int x8d8;                                     // 0x8d8
    int x8dc;                                     // 0x8dc
    int x8e0;                                     // 0x8e0
    int x8e4;                                     // 0x8e4
    int x8e8;                                     // 0x8e8
    int x8ec;                                     // 0x8ec
    int x8f0;                                     // 0x8f0
    int x8f4;                                     // 0x8f4
    int x8f8;                                     // 0x8f8
    int x8fc;                                     // 0x8fc
    int x900;                                     // 0x900
    int x904;                                     // 0x904
    int x908;                                     // 0x908
    int x90c;                                     // 0x90c
    int x910;                                     // 0x910
    int x914;                                     // 0x914
    int x918;                                     // 0x918
    int x91c;                                     // 0x91c
    int x920;                                     // 0x920
    int x924;                                     // 0x924
    int x928;                                     // 0x928
    int x92c;                                     // 0x92c
    int x930;                                     // 0x930
    int x934;                                     // 0x934
    int x938;                                     // 0x938
    int x93c;                                     // 0x93c
    int x940;                                     // 0x940
    int x944;                                     // 0x944
    int x948;                                     // 0x948
    int x94c;                                     // 0x94c
    int x950;                                     // 0x950
    int x954;                                     // 0x954
    int x958;                                     // 0x958
    int x95c;                                     // 0x95c
    int x960;                                     // 0x960
    int x964;                                     // 0x964
    int x968;                                     // 0x968
    int x96c;                                     // 0x96c
    int x970;                                     // 0x970
    int x974;                                     // 0x974
    int x978;                                     // 0x978
    int x97c;                                     // 0x97c
    int x980;                                     // 0x980
    int x984;                                     // 0x984
    int x988;                                     // 0x988
    int x98c;                                     // 0x98c
    int x990;                                     // 0x990
    int x994;                                     // 0x994
    int x998;                                     // 0x998
    int x99c;                                     // 0x99c
    int x9a0;                                     // 0x9a0
    int x9a4;                                     // 0x9a4
    int x9a8;                                     // 0x9a8
    int x9ac;                                     // 0x9ac
    int x9b0;                                     // 0x9b0
    int x9b4;                                     // 0x9b4
    int x9b8;                                     // 0x9b8
    int x9bc;                                     // 0x9bc
    int x9c0;                                     // 0x9c0
    int x9c4;                                     // 0x9c4
    int x9c8;                                     // 0x9c8
    int x9cc;                                     // 0x9cc
    int x9d0;                                     // 0x9d0
    int x9d4;                                     // 0x9d4
    int x9d8;                                     // 0x9d8
    int x9dc;                                     // 0x9dc
    int x9e0;                                     // 0x9e0
    int x9e4;                                     // 0x9e4
    int x9e8;                                     // 0x9e8
    int x9ec;                                     // 0x9ec
    int x9f0;                                     // 0x9f0
    int x9f4;                                     // 0x9f4
    int x9f8;                                     // 0x9f8
    int x9fc;                                     // 0x9fc
    int xa00;                                     // 0xa00
    int xa04;                                     // 0xa04
    int xa08;                                     // 0xa08
    int xa0c;                                     // 0xa0c
    int xa10;                                     // 0xa10
    int xa14;                                     // 0xa14
    int xa18;                                     // 0xa18
    int xa1c;                                     // 0xa1c
    int xa20;                                     // 0xa20
    int xa24;                                     // 0xa24
    int xa28;                                     // 0xa28
    int xa2c;                                     // 0xa2c
    int xa30;                                     // 0xa30
    int xa34;                                     // 0xa34
    int xa38;                                     // 0xa38
    int xa3c;                                     // 0xa3c
    int xa40;                                     // 0xa40
    int xa44;                                     // 0xa44
    int xa48;                                     // 0xa48
    int xa4c;                                     // 0xa4c
    int xa50;                                     // 0xa50
    int xa54;                                     // 0xa54
    int xa58;                                     // 0xa58
    int xa5c;                                     // 0xa5c
    int xa60;                                     // 0xa60
    int xa64;                                     // 0xa64
    int xa68;                                     // 0xa68
    int xa6c;                                     // 0xa6c
    int xa70;                                     // 0xa70
    int xa74;                                     // 0xa74
    int xa78;                                     // 0xa78
    int xa7c;                                     // 0xa7c
    int xa80;                                     // 0xa80
    int xa84;                                     // 0xa84
    int xa88;                                     // 0xa88
    int xa8c;                                     // 0xa8c
    int xa90;                                     // 0xa90
    int xa94;                                     // 0xa94
    int xa98;                                     // 0xa98
    int xa9c;                                     // 0xa9c
    int xaa0;                                     // 0xaa0
    int xaa4;                                     // 0xaa4
    int xaa8;                                     // 0xaa8
    int xaac;                                     // 0xaac
    int xab0;                                     // 0xab0
    int xab4;                                     // 0xab4
    int xab8;                                     // 0xab8
    int xabc;                                     // 0xabc
    int xac0;                                     // 0xac0
    int xac4;                                     // 0xac4
    int xac8;                                     // 0xac8
    int xacc;                                     // 0xacc
    int xad0;                                     // 0xad0
    int xad4;                                     // 0xad4
    int xad8;                                     // 0xad8
    int xadc;                                     // 0xadc
    int xae0;                                     // 0xae0
    int xae4;                                     // 0xae4
    int xae8;                                     // 0xae8
    int xaec;                                     // 0xaec
    int xaf0;                                     // 0xaf0
    int xaf4;                                     // 0xaf4
    int xaf8;                                     // 0xaf8
    int xafc;                                     // 0xafc
    int xb00;                                     // 0xb00
    int xb04;                                     // 0xb04
    int xb08;                                     // 0xb08
    int xb0c;                                     // 0xb0c
    int xb10;                                     // 0xb10
    int xb14;                                     // 0xb14
    int xb18;                                     // 0xb18
    int xb1c;                                     // 0xb1c
    int xb20;                                     // 0xb20
    int xb24;                                     // 0xb24
    int xb28;                                     // 0xb28
    int xb2c;                                     // 0xb2c
    int xb30;                                     // 0xb30
    int xb34;                                     // 0xb34
    int xb38;                                     // 0xb38
    int xb3c;                                     // 0xb3c
    int xb40;                                     // 0xb40
    int xb44;                                     // 0xb44
    GOBJ *plyview_pos_gobj;                       // 0xb48
    int xb4c;                                     // 0xb4c
    int xb50;                                     // 0xb50
    int xb54;                                     // 0xb54
    int xb58;                                     // 0xb58
    int xb5c;                                     // 0xb5c
    int xb60;                                     // 0xb60
    int xb64;                                     // 0xb64
    int xb68;                                     // 0xb68
    int xb6c;                                     // 0xb6c
    int xb70;                                     // 0xb70
    int xb74;                                     // 0xb74
    int xb78;                                     // 0xb78
    int xb7c;                                     // 0xb7c
    int xb80;                                     // 0xb80
    int xb84;                                     // 0xb84
    int xb88;                                     // 0xb88
} Game3dData;

typedef struct GameClearData
{
    u8 x0[0x7c];
    struct
    {
        u8 x0_80 : 1;       //
        u8 x0_40 : 1;       //
        u8 x0_20 : 1;       //
        u8 x0_10 : 1;       //
        u8 has_reward : 1;  // 0x08, checked and assigned @ 8017e008. function gets clear_kind from reward_kind
        u8 is_unlocked : 1; // 0x04, raised after displaying the unlocked animation
        u8 x0_02 : 1;       //
        u8 is_new : 1;      // 0x01, raised when the match ends after unlocking it
    } clear[120];
} GameClearData;

typedef struct gmDataAll
{
    struct
    {
        int x0;
    } *x0;
    struct // 0x4
    {
        int x0;                  // unk
        int weights[STKIND_NUM]; // 0x4, indexed by StadiumKind
    } *stadium_weights;
    struct // 0x8
    {
        u8 city_kind; // 0x0
        u8 gr_kind;   // 0x1
        u8 x2;        // 0x2
        u8 x3;        // 0x3
        u8 x4;        // 0x4
        u8 x5;        // 0x5
    } *stadium_desc;  // array of these, STKIND_NUM
} gmDataAll;

typedef struct CharacterDesc
{
    u8 rider_kind;
    u8 is_bike;
    u8 machine;
} CharacterDesc;

typedef struct PlayerData
{
    u8 x0[0x90c];
} PlayerData;

typedef struct LegendaryPieceData           // 80ae2cec
{                                           //
    void *x0;                               // 0x0
    struct                                  //
    {                                       //
        u8 x0;                              // 0x0
        float item_spawn_match_progress[3]; // 0x4
        int item_kind[3];                   // 0x10
        int x1c[3];                         // 0x1c
        int x28[3];                         // 0x28
        u8 req_spawn : 1;                   // 0x38, 0x80
        u8 is_enabled : 1;                  // 0x38, 0x40
    } machine[2];
} LegendaryPieceData; //

typedef struct grBoxGeneObj              // r13 + 0x608
{                                        //
    struct                               //
    {                                    //
        u8 it_kind[ITKIND_NUM - 1];      // 0x0
        u8 chance[ITKIND_NUM - 1];       // 0x44
        u8 num;                          // 0x88
    } item_group_spawn[BOXKIND_NUM];     // is used for sky drops and box breaks
    u8 sameitem_it_kind[ITKIND_NUM - 1]; // 0x19b
    u8 sameitem_chance[ITKIND_NUM - 1];  // 0x1df
    u8 sameitem_num;                     // 0x223
    u8 unk_it_kind[40];                  // 0x224
    u8 unk_chance[40];                   // 0x24c
    u8 unk_num;                          // 0x274
    int x278;                            // 0x278
} grBoxGeneObj;

typedef struct grBoxGeneInfo // r13 + 0x610
{                            //
    int x0;                  // 0x0
    int x4;                  // 0x4
    struct                   // 0x8
    {                        //
        void *x0;            // 0x0
        ItemFallDesc *desc;  // 0x4
        int desc_num;        // 0x8
    } *fall_timer_desc;      //
    struct                   // 0xc, referred to as item lots data @ 800ec068?
    {
        struct // 0x0
        {
            u8 blue_small_chance;
            u8 blue_medium_chance;
            u8 blue_large_chance;
            u8 green_small_chance;
            u8 green_medium_chance;
            u8 green_large_chance;
            u8 red_small_chance;
            u8 red_medium_chance;
            u8 red_large_chance;
        } *box_spawn_chances;
        struct // 0x4
        {
            int it_kind;
            u16 fall_chance[STGROUP_NUM];
            u16 same_item_chance;
            u16 unk_chance;
        } *item_spawn;
        int item_spawn_num; // 0x8
        struct              // 0xc, 2 of these, one for each legendary machine
        {
            u8 chance_to_spawn; // out of 100
            u8 x1;
            u8 x2;
            u8 x3;
            u8 x4;
            u8 x5;
            u8 x6;
        } *legendary_pieces; //
        struct               // 0x10
        {
            ItemKind it_kind;
            u16 chance[EVKIND_NUM]; //
            u16 chance_unk1;        // 0x24, gets stored to unk_chance in grBoxGeneObj
        } *event_spawn;
        int event_spawn_num; // 0x14
        struct               // 0x18
        {
            int it_kind;        // 0x0
            u16 chance_misc;    // 0x4, volcano walls, breaking the star pole, dyna blade (hitting and leaving)
            u16 chance_tac;     // 0x6, hitting tac
            u16 chance_meteor;  // 0x8, drops after a meteor explodes
            u16 chance_pilar;   // 0xA, busting the event pillar
            u16 chance_chamber; // 0xC, secret chamber,
            u16 chance_ufo;     // 0xE, ufo
        } *x18;
        int x18_num;                  // 0x1c
        int x20;                      // 0x20
        int x24;                      // 0x24
        int x28;                      // 0x28
        int x2c;                      // 0x2c
        int x30;                      // 0x30
        int x34;                      // 0x34
        int x38;                      // 0x38
        int x3c;                      // 0x3c
        int x40;                      // 0x40
        int x44;                      // 0x44
        int x48;                      // 0x48
        int x4c;                      // 0x4c
        int x50;                      // 0x50
        int x54;                      // 0x54
        int x58;                      // 0x58
        int x5c;                      // 0x5c
        int x60;                      // 0x60
        int x64;                      // 0x64
        int x68;                      // 0x68
        int x6c;                      // 0x6c
        int x70;                      // 0x70
        int x74;                      // 0x74
        int x78;                      // 0x78
        int x7c;                      // 0x7c
        int x80;                      // 0x80
        int x84;                      // 0x84
        int x88;                      // 0x88
        int x8c;                      // 0x8c
        int x90;                      // 0x90
        int x94;                      // 0x94
        int x98;                      // 0x98
        int x9c;                      // 0x9c
    } *item_desc;                     //
    int x10;                          // 0x10, referred to as item lots data @ 800ec068?
    int x14;                          // 0x14, referred to as item lots data @ 800ec068?
    int x18;                          // 0x18
    int item_area_pos_num;            // 0x1c, number of item areas
    int cur_max_items;                // 0x20
    int x24;                          // 0x24
    int total_num;                    // 0x28
    int spawn_timer;                  // 0x2c
    int x30;                          // 0x30
    ItemKind same_item_it_kind;       // 0x34, item used when EVKIND_SAMEITEM is active
    int event_active_flags;           // 0x38 (or'd with EventKind)
    ItemFallDesc cur_event_fall_desc; // 0x3c
    int x4c;                          // 0x4c
    int x50;                          // 0x50
    int x54;                          // 0x54
    int x58;                          // 0x58
    int x5c;                          // 0x5c
    int x60;                          // 0x60
    int x64;                          // 0x64
    int x68;                          // 0x68
    int x6c;                          // 0x6c
    int x70;                          // 0x70
    int x74;                          // 0x74
    int x78;                          // 0x78
    int x7c;                          // 0x7c
    int x80;                          // 0x80
    int x84;                          // 0x84
    int x88;                          // 0x88
    int x8c;                          // 0x8c
    int x90;                          // 0x90
    int x94;                          // 0x94
    int x98;                          // 0x98
    int x9c;                          // 0x9c
    int xa0;                          // 0xa0
    int xa4;                          // 0xa4
    int xa8;                          // 0xa8
    int xac;                          // 0xac
    int xb0;                          // 0xb0
    int xb4;                          // 0xb4
    int xb8;                          // 0xb8
    int xbc;                          // 0xbc
    int xc0;                          // 0xc0
    int xc4;                          // 0xc4
    int xc8;                          // 0xc8
    int xcc;                          // 0xcc
    int xd0;                          // 0xd0
    int xd4;                          // 0xd4
    int xd8;                          // 0xd8
    int xdc;                          // 0xdc
    int xe0;                          // 0xe0
    int xe4;                          // 0xe4
    int xe8;                          // 0xe8
    int xec;                          // 0xec
    int xf0;                          // 0xf0
    int xf4;                          // 0xf4
    int xf8;                          // 0xf8
    int xfc;                          // 0xfc
    int x100;                         // 0x100
    int x104;                         // 0x104
    int x108;                         // 0x108
    int x10c;                         // 0x10c
    int x110;                         // 0x110
    int x114;                         // 0x114
    int x118;                         // 0x118
    int x11c;                         // 0x11c
    int x120;                         // 0x120
    int x124;                         // 0x124
    int x128;                         // 0x128
    int x12c;                         // 0x12c
    int x130;                         // 0x130
    int x134;                         // 0x134
    int x138;                         // 0x138
    int x13c;                         // 0x13c
    int x140;                         // 0x140
    int x144;                         // 0x144
    int x148;                         // 0x148
    int x14c;                         // 0x14c
    int x150;                         // 0x150
    int x154;                         // 0x154
    int x158;                         // 0x158
    int x15c;                         // 0x15c
    int x160;                         // 0x160
    int x164;                         // 0x164
    int x168;                         // 0x168
    int x16c;                         // 0x16c
    int x170;                         // 0x170
    int x174;                         // 0x174
    int x178;                         // 0x178
    int x17c;                         // 0x17c
    int x180;                         // 0x180
    int x184;                         // 0x184
    int x188;                         // 0x188
    int x18c;                         // 0x18c
    int x190;                         // 0x190
    int x194;                         // 0x194
    int x198;                         // 0x198
    int x19c;                         // 0x19c
    int x1a0;                         // 0x1a0
    int x1a4;                         // 0x1a4
    int x1a8;                         // 0x1a8
    int x1ac;                         // 0x1ac
    int x1b0;                         // 0x1b0
    int x1b4;                         // 0x1b4
    int x1b8;                         // 0x1b8
    int x1bc;                         // 0x1bc
    int x1c0;                         // 0x1c0
    int x1c4;                         // 0x1c4
    int x1c8;                         // 0x1c8
    int x1cc;                         // 0x1cc
    int x1d0;                         // 0x1d0
    int x1d4;                         // 0x1d4
    int x1d8;                         // 0x1d8
    int x1dc;                         // 0x1dc
    int x1e0;                         // 0x1e0
    int x1e4;                         // 0x1e4
    int x1e8;                         // 0x1e8
    int x1ec;                         // 0x1ec
    int x1f0;                         // 0x1f0
    int x1f4;                         // 0x1f4
    int x1f8;                         // 0x1f8
    int x1fc;                         // 0x1fc
    int x200;                         // 0x200
    int x204;                         // 0x204
    int x208;                         // 0x208
    int x20c;                         // 0x20c
    int x210;                         // 0x210
    int x214;                         // 0x214
    int x218;                         // 0x218
    int x21c;                         // 0x21c
    int x220;                         // 0x220
    int x224;                         // 0x224
    int x228;                         // 0x228
    int x22c;                         // 0x22c
    int x230;                         // 0x230
    int x234;                         // 0x234
    int x238;                         // 0x238
    int x23c;                         // 0x23c
    int x240;                         // 0x240
    int x244;                         // 0x244
    int x248;                         // 0x248
    int x24c;                         // 0x24c
    int x250;                         // 0x250
    int x254;                         // 0x254
    int x258;                         // 0x258
    int x25c;                         // 0x25c
    int x260;                         // 0x260
    int x264;                         // 0x264
    int x268;                         // 0x268
    int x26c;                         // 0x26c
    int x270;                         // 0x270
    int x274;                         // 0x274
    int x278;                         // 0x278
    int x27c;                         // 0x27c
    int x280;                         // 0x280
    int x284;                         // 0x284
    int x288;                         // 0x288
    int x28c;                         // 0x28c
    int timer_minutes;                // 0x290
    int timer_seconds_in_minute;      // 0x294
    int timer_subseconds;             // 0x298, out of 100
    int match_subseconds_left;        // 0x29c,
    int match_initial_subseconds;     // 0x2a0, initial time at the beginning of the game
    float match_progress;             // 0x2a4, goes from 0 -> 1
    int flags_x2a8;                   // 0x2a8, 0x40 = is_match_intro
    int x2ac;                         // 0x2ac
    int x2b0;                         // 0x2b0
    int x2b4;                         // 0x2b4
    int x2b8;                         // 0x2b8
    int x2bc;                         // 0x2bc
    int x2c0;                         // 0x2c0
    int x2c4;                         // 0x2c4
    int x2c8;                         // 0x2c8
} grBoxGeneInfo;

typedef struct MnResultsCityBoard4Data
{
    u8 x0[0xc]; //
    struct
    {
        u8 xc;          // 0xc
        u8 color_idx;   // 0xd
        int x10;        // 0x10
        JOBJ *score_j;  // 0x14
        void *x18;      //
        void *x1c;      //
        void *x20;      //
        JOBJ *border_j; // 0x24
        JOBJ *plynum_j; // 0x28
        u8 x2c[0x58];   //
    } ply[4];
} MnResultsCityBoard4Data;

typedef struct BoxData
{
    u8 x0[0x148];        // 0x0
    HurtData *hurt_data; // 0x148
} BoxData;

typedef struct BGMDesc
{
    BGMKind bgm;
    int x4;
    char *path; // relative to /audio
    int xc;
} BGMDesc;

typedef struct PlyViewPosData
{
    int x0;                     // 0x0
    int x4;                     // 0x4
    int x8;                     // 0x8
    int xc;                     // 0xc
    int x10;                    // 0x10
    Vec3 plyview_center_pos[4]; // 0x14
} PlyViewPosData;

//////////////////////
// Static Variables //
//////////////////////

static LegendaryPieceData **stc_legendary_piece_data = (LegendaryPieceData **)(0x805dd0e0 + 0x600);
static grBoxGeneObj **stc_grBoxGeneObj = (grBoxGeneObj **)(0x805dd0e0 + 0x608);
static grBoxGeneInfo **stc_grBoxGeneInfo = (grBoxGeneInfo **)(0x805dd0e0 + 0x610);
static itCommonDataAll **stc_it_common_data = (itCommonDataAll **)(0x805dd0e0 + 0x7f0);
static gmDataAll **stc_gmdataall = (gmDataAll **)(0x805dd0e0 + 0x494);
static int *stc_city_machine_num = (int *)(0x805dd0e0 + 0x754); //
static u8 *stc_city_starting_machine = (u8 *)0x80495816;
static PlayerData *stc_playerdata = (PlayerData *)0x8055a9f0; // 4 of these
static u8 *stc_clear_num = (u8 *)0x805d51d0;                  // array here indexed by GMMODE
static u8 **stc_reward_lookup = (u8 **)0x805d51d0;            // array here indexed by GMMODE then RewardKind

static BGMDesc *stc_bgm_desc = (BGMDesc *)0x80498750;

///////////////
// Functions //
///////////////

TitleScreenData *TitleScreen_GetData();
GameData *Gm_GetGameData();
Game3dData *Gm_Get3dData();
GameClearData *Gm_GetGameClearData();

void CityTrial_DecideStadium();

void Gm_HidePauseHUD();
void Gm_ShowHUD();

int Gm_IsInCity();
CharacterDesc *Character_GetDesc(CharacterKind ckind);

GmIntroState Gm_GetIntroState();
CityMode Gm_GetCityMode();
float Ply_GetCityStatNum(int ply, int stat_idx, int unk);
GOBJ *Ply_GetRiderGObj(int ply);
GOBJ *Ply_GetMachineGObj(int ply);
int Ply_GetColor(int ply);
int Ply_CheckIfHMN(int ply);
int Ply_CheckIfCPU(int ply);
int Ply_GetViewIndex(int ply);
RiderKind Ply_GetRiderKind2(int ply);
PKind Ply_GetPKind(int ply);
RiderKind Ply_GetRiderKind(int ply);
void Ply_SetRiderKind(RiderKind kind);
void Ply_SetMachineKind(int ply, MachineKind kind);
int Ply_GetMachineKind(int ply);
void Ply_SetMachineIsBike(int ply, int is_bike);
int Ply_GetMachineIsBike(int ply);
void Ply_AddDeath(int ply, DmgLog *dmg_log, int is_bike, MachineKind machine_kind);
void Ply_SetHP(int ply, float hp);

int Gm_GetPlyViewNum();
ItemGroup Gm_GetItemGroup(ItemKind it_kind);
int ClearChecker_GetClearKindFromRewardKind(GameMode gm, int clear_kind);
int ClearChecker_GetRewardNum(GameMode gm);
int ClearChecker_CheckUnlocked(GameMode gm, int clear_kind);
u8 ClearChecker_GetClearData(GameMode gm, int clear_kind);
int Gm_IsGrKindCity(GroundKind gr_kind);
int Gm_IsDestructionDerby();
void CitySelect_Cursor6Update(int ply, int color_idx);
void AirRideSelect_Cursor6Update(int ply, int color_idx);
int Gm_GetPauseKind(int type);
void Gm_Pause(int pause_kind);
void Gm_Resume(int pause_kind);
void Gm_PlayPauseSFX();
void Gm_PauseAllSFX();
void Gm_ResumeAllSFX();

void Gm_SetCameraNormal();
int Gm_IsDamageEnabled();
int Hoshi_WriteSave();
int hash_32(const void *data, int size);
int hash_32_str(const void *data);
void Gm_LoadGroundFGMBank(GroundKind gr_kind); //
#endif