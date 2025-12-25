#ifndef MEX_H_SCENE
#define MEX_H_SCENE

#include "structs.h"
#include "datatypes.h"

// Scene Enums
enum SCENE_HEAP_KIND
{
    SCENEHEAPKIND_UNK0,
    SCENEHEAPKIND_UNK1,
    SCENEHEAPKIND_UNK2,
    SCENEHEAPKIND_UNK3, // shrinks main heap, enables fighter cache?
    SCENEHEAPKIND_UNK4,
};

typedef enum MinorKind
{
    MNRKIND_TITLESCREEN,           //
    MNRKIND_OPENING,               //
    MNRKIND_MAINMENU,              //
    MNRKIND_AIRRIDESETINGS,        //
    MNRKIND_4,                     //
    MNRKIND_CITYSETTINGS,          // City Trial Settings
    MNRKIND_AIRRIDEMAPSELECT,      //
    MNRKIND_7,                     //
    MNRKIND_AIRRIDEPLYSELECT,      //
    MNRKIND_9,                     //
    MNRKIND_CITYPLYSELECT,         // City Trial CSS
    MNRKIND_11,                    //
    MNRKIND_12,                    //
    MNRKIND_CITYRESULT,            //
    MNRKIND_14,                    //
    MNRKIND_15,                    //
    MNRKIND_16,                    //
    MNRKIND_STADIUMSPLASH,         //
    MNRKIND_3D,                    //
    MNRKIND_19,                    //
    MNRKIND_20,                    //
    MNRKIND_STADIUMSELECT,         //
    MNRKIND_MOVIE = 25,            //
    MNRKIND_AIRRIDECHECKLIST = 32, //
    MNRKIND_TOPRIDECHECKLIST,      //
    MNRKIND_CITYCHECKLIST,         //
    MNRKIND_AIRRIDERECORDS,        //
    MNRKIND_TOPRIDERECORDS,        //
    MNRKIND_CITYRECORDS,           //
    MNRKIND_CARD,                  //
    MNRKIND_DEBUGMENU,             //
    MNRKIND_40,                    //
    MNRKIND_NUM,                   // terminator
} MinorKind;

typedef enum MajorKind
{
    MJRKIND_0,           //
    MJRKIND_MOVIE,       //
    MJRKIND_TITLE,       // Title Screen
    MJRKIND_MENU,        //
    MJRKIND_AIR,         //
    MJRKIND_TOP,         //
    MJRKIND_CITY,        //
    MJRKIND_DEBUG3D,     // Air Ride Debug Scene
    MJRKIND_TOPDEBUG,    // Top Ride Debug Menu
    MJRKIND_ENDING = 10, // Credits
    MJRKIND_DEBUG = 16,  // Debug Menu
    MJRKIND_CARD = 18,   // Memcard Prompt
    MJRKIND_LAN,         // LAN
    MJRKIND_NUM,         //
} MajorKind;

struct MajorSceneDesc
{
    MajorKind major_id : 8;         // 0x0
    MajorKind next_major_id : 8;    // 0x1
    MinorKind initial_minor_id : 8; // 0x2
    void (*cb_Enter)();             // 0x4
    void (*cb_ExitMinor)();         // 0x8
};

struct MinorSceneDesc
{
    s8 idx;                                    // 0x00, MinorKind
    s8 x1;                                     // 0x01
    void (*cb_Load)();                         // 0x04
    void (*cb_Exit)(void *data);               // 0x08
    void (*cb_ThinkPreGObjProc)(void *data);   // 0x0C, executed first at @ 800068bc
    void (*cb_ThinkPostGObjProc)(void *data);  // 0x10, executed second after GObj proc's have been udpated @ 80006a4c
    void (*cb_ThinkPostGObjProc2)(void *data); // 0x14, executed third, directly after the previous (what is this for...) @ 80006a64
    void (*cb_ThinkPreRender)(void *data);     // 0x18, execute fourth, before rendering
    void (*cb_ThinkPostRender)(void *data);    // 0x1c, executed last, after all rendering
    int preload_kind;                          // 0x20. copied to Preload::kind
};

struct MinorScene
{
    s8 minor_id;        // is -1 for last entry
    u8 heap_kind;       // heap behavior, (2)
    void *minor_prep;   // inits data for this minor (major exclusive)
    void *minor_decide; // decides next minor scene
    u8 minor_kind;      // index for a re-useable list of scene functions. contains a load, think, and leave function.
    void *load_data;    // points to static data used throughout this minor. other minors may use the same pointer to exchange data between minors
    void *unload_data;  // points to static data used throughout this minor. other minors may use the same pointer to exchange data between minors
};

struct SceneInfo
{
    u8 major_curr; // 0x0
    u8 major_next; // 0x1
    u8 major_prev; // 0x2
    u8 minor_curr; // 0x3
    u8 minor_next; // 0x4
    u8 minor_prev; // 0x5
};

struct ScMenuCommon
{
    u8 x0;                  // 0x0
    HSD_SObjDesc *sobj;     // 0x4
    GOBJ *cam_gobj;         // 0x8
    int xc;                 // 0xc
    int x10;                // 0x10
    int x14;                // 0x14
    int x18;                // 0x18
    int x1c;                // 0x1c
    int x20;                // 0x20
    int x24;                // 0x24
    struct
    {
        u8 canvas_idx;          // 0x28
        Text *description_text; // 0x2c
        int x30;                // 0x30
        int x34;                // 0x34
        int x38;                // 0x38
        int x3c;                // 0x3c
        struct
        {
            Text *x0;           // 0x40
            Text *x4;           // 0x44
        } ply_machine_description[4];
    } text;
    u8 x54[0x298];              // 0x60
    struct
    {
        GOBJ *menu_name;            // 0x2f8
        u8 x2fc[0x7C];              // 0x2fc
        GOBJ *ScMenOpdelpanel_gobj; // 0x378
        u8 x37c[0x144];             // 0x37c
    } main;
    struct
    {
        HSD_SObjDesc *sobj;                       // 0x4c0
        JOBJSet *ScMenSelplyBg_scene_models;      // 0x4c4
        GOBJ *background_gobj;                    // 0x4c8
        JOBJSet *ScMenSelplyPanel_scene_models;   // 0x4cc
        GOBJ *panel_gobj;                         // 0x4d0
        JOBJSet *ScMenSelplyScore_scene_models;   // 0x4d4
        GOBJ *score_gobj;                         // 0x4d8
        JOBJSet *ScMenSelplyCursor0_scene_models; // 0x4dc
        GOBJ *cursor0_gobj[4];                    // 0x4e0
        JOBJSet *ScMenSelplyBoard_scene_models;   // 0x4f0
        GOBJ *board_gobj;                         // 0x4f4
        JOBJSet *ScMenSelplyHandi_scene_models;   // 0x4f8
        struct                                    // 0x4fc
        {
            GOBJ *handicap;
            GOBJ *cpu_level;
        } bar_gobj[4];
        JOBJSet *ScMenSelplySicon_scene_models;   // 0x51c
        GOBJ *sicon_gobj[20];                     // 0x520
        JOBJSet *ScMenSelplySicon2_scene_models;  // 0x570
        GOBJ *sicon2_gobj[4];                     // 0x574
        JOBJSet *ScMenSelplyCpos_scene_models;    // 0x584
        GOBJ *cpos_gobj;                          // 0x588
        JOBJSet *ScMenSelplyIpos_scene_models;    // 0x58c
        GOBJ *ipos_gobj;                          // 0x590
        JOBJSet *ScMenSelplyCursor1_scene_models; // 0x594
        GOBJ *cursor1_gobj[4];                    // 0x598
        JOBJSet *ScMenSelplyCursor2_scene_models; // 0x5a8
        GOBJ *cursor2_gobj[4];                    // 0x5ac
        JOBJSet *ScMenSelplyCursor3_scene_models; // 0x5bc
        GOBJ *cursor3_gobj[4];                    // 0x5c0
        JOBJSet *ScMenSelplyCursor5_scene_models; // 0x5d0
        GOBJ *cursor5_gobj[4];                    // 0x5d4
        JOBJSet *ScMenSelplyWin_scene_models;     // 0x5e4
        GOBJ *win_gobj[4];                        // 0x5e8
        JOBJSet *ScMenSelplyPstart_scene_models;  // 0x5f8
        GOBJ *start_gobj;                         // 0x5fc
        JOBJSet *ScMenSelplyEntry_scene_models;   // 0x600
        GOBJ *entry_gobj[4];                      // 0x604
        JOBJSet *ScMenSelplyCursor6_scene_models; // 0x614
        GOBJ *cursor6_gobj[4];                    // 0x618
        JOBJSet *ScMenSelplyHnum_scene_models;    // 0x628
        GOBJ *hnum_gobj[8];                       // 0x62c
        JOBJSet *ScMenSelplyAbtn_scene_models;    // 0x64c
        GOBJ *abtn_gobj[4];                       // 0x650
    } airride_select;
    u8 x660[0x14c];
    // place padding here
    struct
    {
        HSD_SObjDesc *sobj;                              // 0x7ac
        JOBJSet **ScMenSelplyBgCt_scene_models;          // 0x7b0
        GOBJ *background_gobj;                           // 0x7b4
        JOBJSet **ScMenSelplyPanelCt_scene_models;       // 0x7b8
        GOBJ *panel_gobj;                                // 0x7bc
        JOBJSet **x7c0;                                  // 0x7c0
        JOBJSet **x7c4;                                  // 0x7c4
        JOBJSet **ScMenSelplyCursor0Ct_scene_models;     // 0x7c8
        GOBJ *cursor0_gobj[4];                           // 0x7cc
        JOBJSet **ScMenSelplyCursorBoardCt_scene_models; // 0x7dc
        GOBJ *board_gobj;                                // 0x7e0
        JOBJSet **ScMenSelplyHandiCt_scene_models;       // 0x7e4
        struct                                           // 0x7e8
        {
            GOBJ *handicap;
            GOBJ *cpu_level;
        } bar_gobj[4];
        u8 x808[0x54];                                   // 0x808
        JOBJSet **ScMenSelplySicon2Ct_scene_models;      // 0x85c
        GOBJ *sicon2_gobj[4];                            // 0x860
        JOBJSet **ScMenSelplyCpos2Ct_scene_models;       // 0x870
        GOBJ *cpos_gobj;                                 // 0x874
        JOBJSet **ScMenSelplyIpos2Ct_scene_models;       // 0x878
        GOBJ *ipos_gobj;                                 // 0x87c
        JOBJSet **ScMenSelplyCursor1_scene_models;       // 0x880
        GOBJ *cursor1_gobj[4];                           // 0x884
        void *x894;                                      // 0x894
        void *x898;                                      // 0x898
        void *x89c;                                      // 0x89c
        JOBJSet **x8a0;                                  // 0x8a0
        GOBJ *x8a4;                                      // 0x8a4
        JOBJSet *ScMenSelplyCursor3_scene_models;        // 0x8a8
        GOBJ *cursor3_gobj[4];                           // 0x8ac
        JOBJSet **ScMenSelplyCursor5_scene_models;       // 0x8bc
        GOBJ *cursor5_gobj[4];                           // 0x8c0
        JOBJSet **ScMenSelplyWinCt_scene_models;         // 0x8d0
        GOBJ *window_gobj[4];                             // 0x8d4
        int x8e4;                                        // 0x8e4
        int x8e8;                                        // 0x8e8
        JOBJSet **ScMenSelplyEntryCt_scene_models;       // 0x8ec
        GOBJ *entry_gobj[4];                             // 0x8f0
        int ScMenSelplyCursor6_scene_models;             // 0x900
        GOBJ *cursor6_gobj[4];                           // 0x904
    } city_select;                                       //
    int x914;                                            // 0x914
    int x918;                                            // 0x918
    int x91c;                                            // 0x91c
    int x920;                                            // 0x920
    int x924;                                            // 0x924
    int x928;                                            // 0x928
    int x92c;                                            // 0x92c
    int x930;                                            // 0x930
    int x934;                                            // 0x934
    int x938;                                            // 0x938
    int x93c;                                            // 0x93c
    int x940;                                            // 0x940
    int x944;                                            // 0x944
    int x948;                                            // 0x948
    int x94c;                                            // 0x94c
    int x950;                                            // 0x950
    int x954;                                            // 0x954
    int x958;                                            // 0x958
    int x95c;                                            // 0x95c
    int x960;                                            // 0x960
    int x964;                                            // 0x964
    int x968;                                            // 0x968
    int x96c;                                            // 0x96c
    int x970;                                            // 0x970
    int x974;                                            // 0x974
    int x978;                                            // 0x978
    int x97c;                                            // 0x97c
    int x980;                                            // 0x980
    int x984;                                            // 0x984
    int x988;                                            // 0x988
    int x98c;                                            // 0x98c
    int x990;                                            // 0x990
    int x994;                                            // 0x994
    int x998;                                            // 0x998
    int x99c;                                            // 0x99c
    int x9a0;                                            // 0x9a0
    struct                                               //
    {                                                    //
        JOBJSet **Kicon_jobjset;                         // 0x9a4
    } stadium;                                           //
    int x9a8;                                            // 0x9a8
    int x9ac;                                            // 0x9ac
    int x9b0;                                            // 0x9b0
    int x9b4;                                            // 0x9b4
    int x9b8;                                            // 0x9b8
    int x9bc;                                            // 0x9bc
    int x9c0;                                            // 0x9c0
    int x9c4;                                            // 0x9c4
    int x9c8;                                            // 0x9c8
    int x9cc;                                            // 0x9cc
    int x9d0;                                            // 0x9d0
    int x9d4;                                            // 0x9d4
    int x9d8;                                            // 0x9d8
    int x9dc;                                            // 0x9dc
    int x9e0;                                            // 0x9e0
    int x9e4;                                            // 0x9e4
    int x9e8;                                            // 0x9e8
    int x9ec;                                            // 0x9ec
    int x9f0;                                            // 0x9f0
    int x9f4;                                            // 0x9f4
    int x9f8;                                            // 0x9f8
    struct                                               //
    {                                                    //
        JOBJSet **Kicon_jobjset;                         // 0x9fc
        int xa00;                                        // 0xa00
        int xa04;                                        // 0xa04
        int xa08;                                        // 0xa08
        int xa0c;                                        // 0xa0c
        int xa10;                                        // 0xa10
        int xa14;                                        // 0xa14
        int xa18;                                        // 0xa18
        int xa1c;                                        // 0xa1c
        int xa20;                                        // 0xa20
        int xa24;                                        // 0xa24
        int xa28;                                        // 0xa28
        int xa2c;                                        // 0xa2c
        int xa30;                                        // 0xa30
        int xa34;                                        // 0xa34
        int xa38;                                        // 0xa38
        int xa3c;                                        // 0xa3c
        int xa40;                                        // 0xa40
        int xa44;                                        // 0xa44
        int xa48;                                        // 0xa48
        int xa4c;                                        // 0xa4c
        int xa50;                                        // 0xa50
        int xa54;                                        // 0xa54
        int xa58;                                        // 0xa58
        int xa5c;                                        // 0xa5c
        int xa60;                                        // 0xa60
        int xa64;                                        // 0xa64
        int xa68;                                        // 0xa68
        int xa6c;                                        // 0xa6c
        int xa70;                                        // 0xa70
        int xa74;                                        // 0xa74
        int xa78;                                        // 0xa78
        int xa7c;                                        // 0xa7c
        int xa80;                                        // 0xa80
        int xa84;                                        // 0xa84
        int xa88;                                        // 0xa88
        int xa8c;                                        // 0xa8c
        int xa90;                                        // 0xa90
        int xa94;                                        // 0xa94
        int xa98;                                        // 0xa98
        int xa9c;                                        // 0xa9c
        int xaa0;                                        // 0xaa0
        int xaa4;                                        // 0xaa4
        int xaa8;                                        // 0xaa8
        int xaac;                                        // 0xaac
        int xab0;                                        // 0xab0
        int xab4;                                        // 0xab4
        int xab8;                                        // 0xab8
        int xabc;                                        // 0xabc
        int xac0;                                        // 0xac0
        int xac4;                                        // 0xac4
        int xac8;                                        // 0xac8
        int xacc;                                        // 0xacc
        int xad0;                                        // 0xad0
        int xad4;                                        // 0xad4
        int xad8;                                        // 0xad8
        int xadc;                                        // 0xadc
    } airride_results;                                   //
    int xae0;                                            // 0xae0
    int xae4;                                            // 0xae4
    int xae8;                                            // 0xae8
    int xaec;                                            // 0xaec
    int xaf0;                                            // 0xaf0
    int xaf4;                                            // 0xaf4
    int xaf8;                                            // 0xaf8
    int xafc;                                            // 0xafc
    int xb00;                                            // 0xb00
    int xb04;                                            // 0xb04
    int xb08;                                            // 0xb08
    int xb0c;                                            // 0xb0c
    int xb10;                                            // 0xb10
    int xb14;                                            // 0xb14
    int xb18;                                            // 0xb18
    int xb1c;                                            // 0xb1c
    int xb20;                                            // 0xb20
    int xb24;                                            // 0xb24
    int xb28;                                            // 0xb28
    int xb2c;                                            // 0xb2c
    int xb30;                                            // 0xb30
    int xb34;                                            // 0xb34
    int xb38;                                            // 0xb38
    int xb3c;                                            // 0xb3c
    int xb40;                                            // 0xb40
    int xb44;                                            // 0xb44
    int xb48;                                            // 0xb48
    int xb4c;                                            // 0xb4c
    int xb50;                                            // 0xb50
    int xb54;                                            // 0xb54
    int xb58;                                            // 0xb58
    int xb5c;                                            // 0xb5c
    int xb60;                                            // 0xb60
    int xb64;                                            // 0xb64
    int xb68;                                            // 0xb68
    int xb6c;                                            // 0xb6c
    int xb70;                                            // 0xb70
    int xb74;                                            // 0xb74
    int xb78;                                            // 0xb78
    int xb7c;                                            // 0xb7c
    int xb80;                                            // 0xb80
    int xb84;                                            // 0xb84
    int xb88;                                            // 0xb88
    int xb8c;                                            // 0xb8c
    int xb90;                                            // 0xb90
    int xb94;                                            // 0xb94
    int xb98;                                            // 0xb98
    int xb9c;                                            // 0xb9c
    int xba0;                                            // 0xba0
    int xba4;                                            // 0xba4
    int xba8;                                            // 0xba8
    int xbac;                                            // 0xbac
    int xbb0;                                            // 0xbb0
    int xbb4;                                            // 0xbb4
    int xbb8;                                            // 0xbb8
    int xbbc;                                            // 0xbbc
    int xbc0;                                            // 0xbc0
    int xbc4;                                            // 0xbc4
    int xbc8;                                            // 0xbc8
    int xbcc;                                            // 0xbcc
    int xbd0;                                            // 0xbd0
    int xbd4;                                            // 0xbd4
    int xbd8;                                            // 0xbd8
    int xbdc;                                            // 0xbdc
    int xbe0;                                            // 0xbe0
    int xbe4;                                            // 0xbe4
    int xbe8;                                            // 0xbe8
    int xbec;                                            // 0xbec
    int xbf0;                                            // 0xbf0
    int xbf4;                                            // 0xbf4
    int xbf8;                                            // 0xbf8
    int xbfc;                                            // 0xbfc
    int xc00;                                            // 0xc00
    int xc04;                                            // 0xc04
    int xc08;                                            // 0xc08
    int xc0c;                                            // 0xc0c
    int xc10;                                            // 0xc10
    int xc14;                                            // 0xc14
    int xc18;                                            // 0xc18
    int xc1c;                                            // 0xc1c
    int xc20;                                            // 0xc20
    int xc24;                                            // 0xc24
    int xc28;                                            // 0xc28
    int xc2c;                                            // 0xc2c
    int xc30;                                            // 0xc30
    int xc34;                                            // 0xc34
    int xc38;                                            // 0xc38
    int xc3c;                                            // 0xc3c
    int xc40;                                            // 0xc40
    int xc44;                                            // 0xc44
    int xc48;                                            // 0xc48
    int xc4c;                                            // 0xc4c
    int xc50;                                            // 0xc50
    int xc54;                                            // 0xc54
    int xc58;                                            // 0xc58
    int xc5c;                                            // 0xc5c
    int xc60;                                            // 0xc60
    int xc64;                                            // 0xc64
    int xc68;                                            // 0xc68
    int xc6c;                                            // 0xc6c
    int xc70;                                            // 0xc70
    int xc74;                                            // 0xc74
    int xc78;                                            // 0xc78
    struct
    {
        JOBJSet **Kicon_jobjset; // 0xc7c
        int xc80;                // 0xc80
        int xc84;                // 0xc84
        int xc88;                // 0xc88
        int xc8c;                // 0xc8c
        int xc90;                // 0xc90
        int xc94;                // 0xc94
        int xc98;                // 0xc98
        int xc9c;                // 0xc9c
        int xca0;                // 0xca0
        int xca4;                // 0xca4
        int xca8;                // 0xca8
        int xcac;                // 0xcac
        int xcb0;                // 0xcb0
        int xcb4;                // 0xcb4
        int xcb8;                // 0xcb8
        int xcbc;                // 0xcbc
        int xcc0;                // 0xcc0
        int xcc4;                // 0xcc4
        GOBJ *board_gobj;        // 0xcc8
        int xccc;                // 0xccc
        GOBJ *board2_gobj;       // 0xcd0
        int xcd4;                // 0xcd4
        GOBJ *board4_gobj;       // 0xcd8
    } city_results;              //
    int xcdc;                    // 0xcdc
    int xce0;                    // 0xce0
    int xce4;                    // 0xce4
    int xce8;                    // 0xce8
    int xcec;                    // 0xcec
    int xcf0;                    // 0xcf0
    int xcf4;                    // 0xcf4
    int xcf8;                    // 0xcf8
    int xcfc;                    // 0xcfc
    int xd00;                    // 0xd00
    int xd04;                    // 0xd04
    int xd08;                    // 0xd08
    int xd0c;                    // 0xd0c
    int xd10;                    // 0xd10
    int xd14;                    // 0xd14
    int xd18;                    // 0xd18
    int xd1c;                    // 0xd1c
    int xd20;                    // 0xd20
    int xd24;                    // 0xd24
    int xd28;                    // 0xd28
    int xd2c;                    // 0xd2c
    int xd30;                    // 0xd30
    int xd34;                    // 0xd34
    int xd38;                    // 0xd38
    int xd3c;                    // 0xd3c
    int xd40;                    // 0xd40
    int xd44;                    // 0xd44
    int xd48;                    // 0xd48
    int xd4c;                    // 0xd4c
    int xd50;                    // 0xd50
    int xd54;                    // 0xd54
    int xd58;                    // 0xd58
    int xd5c;                    // 0xd5c
    int xd60;                    // 0xd60
    int xd64;                    // 0xd64
    int xd68;                    // 0xd68
    int xd6c;                    // 0xd6c
    int xd70;                    // 0xd70
    int xd74;                    // 0xd74
    int xd78;                    // 0xd78
    int xd7c;                    // 0xd7c
    int xd80;                    // 0xd80
    int xd84;                    // 0xd84
    int xd88;                    // 0xd88
    int xd8c;                    // 0xd8c
    int xd90;                    // 0xd90
    int xd94;                    // 0xd94
    int xd98;                    // 0xd98
    int xd9c;                    // 0xd9c
    int xda0;                    // 0xda0
    int xda4;                    // 0xda4
    int xda8;                    // 0xda8
    int xdac;                    // 0xdac
    int xdb0;                    // 0xdb0
    int xdb4;                    // 0xdb4
    int xdb8;                    // 0xdb8
    int xdbc;                    // 0xdbc
    int xdc0;                    // 0xdc0
    int xdc4;                    // 0xdc4
    int xdc8;                    // 0xdc8
    int xdcc;                    // 0xdcc
    int xdd0;                    // 0xdd0
    int xdd4;                    // 0xdd4
    int xdd8;                    // 0xdd8
    int xddc;                    // 0xddc
    int xde0;                    // 0xde0
    int xde4;                    // 0xde4
    int xde8;                    // 0xde8
    int xdec;                    // 0xdec
    int xdf0;                    // 0xdf0
    int xdf4;                    // 0xdf4
    int xdf8;                    // 0xdf8
    int xdfc;                    // 0xdfc
    int xe00;                    // 0xe00
    int xe04;                    // 0xe04
    int xe08;                    // 0xe08
    int xe0c;                    // 0xe0c
    int xe10;                    // 0xe10
    int xe14;                    // 0xe14
    int xe18;                    // 0xe18
    int xe1c;                    // 0xe1c
    int xe20;                    // 0xe20
    int xe24;                    // 0xe24
    int xe28;                    // 0xe28
    int xe2c;                    // 0xe2c
    int xe30;                    // 0xe30
    int xe34;                    // 0xe34
    int xe38;                    // 0xe38
    int xe3c;                    // 0xe3c
    int xe40;                    // 0xe40
    int xe44;                    // 0xe44
    int xe48;                    // 0xe48
    int xe4c;                    // 0xe4c
    int xe50;                    // 0xe50
    int xe54;                    // 0xe54
    int xe58;                    // 0xe58
    int xe5c;                    // 0xe5c
    int xe60;                    // 0xe60
    int xe64;                    // 0xe64
    int xe68;                    // 0xe68
    int xe6c;                    // 0xe6c
    int xe70;                    // 0xe70
    int xe74;                    // 0xe74
    int xe78;                    // 0xe78
    int xe7c;                    // 0xe7c
    int xe80;                    // 0xe80
    int xe84;                    // 0xe84
    int xe88;                    // 0xe88
    int xe8c;                    // 0xe8c
    int xe90;                    // 0xe90
    int xe94;                    // 0xe94
    int xe98;                    // 0xe98
    int xe9c;                    // 0xe9c
    int xea0;                    // 0xea0
    int xea4;                    // 0xea4
    int xea8;                    // 0xea8
    int xeac;                    // 0xeac
    GOBJ *ScMenTitleBg_gobj;     // 0xeb0
    int xeb4;                    // 0xeb4
    GOBJ *ScMenTitleFg_gobj;     // 0xeb8
    int xebc;                    // 0xebc
    int xec0;                    // 0xec0
    int xec4;                    // 0xec4
    int xec8;                    // 0xec8
    int xecc;                    // 0xecc
    int xed0;                    // 0xed0
    int xed4;                    // 0xed4
    int xed8;                    // 0xed8
    int xedc;                    // 0xedc
    int xee0;                    // 0xee0
    int xee4;                    // 0xee4
    int xee8;                    // 0xee8
    int xeec;                    // 0xeec
    int xef0;                    // 0xef0
    int xef4;                    // 0xef4
    int xef8;                    // 0xef8
    int xefc;                    // 0xefc
    u8 xf00[0x348];              // 0xf00
    struct
    {
        JOBJSet **Kicon_jobjset; // 0x1248
    } bestrap;
};

typedef struct ScMenuSelect
{
    struct
    {
        u8 preview_jobj;    // 0x0, displayed when hovered over start game button
        u8 description_idx; // 0x1
        u8 menu_color_idx;  // 0x2
    } mode_button_desc[5];
    u8 xf;
    struct
    {
        u8 menu_name_tex_idx; // 0x0
        u8 option_num;        // 0x1
        u8 x2;                // 0x2
    } menu_desc[9];
    u8 x2b;
    struct
    {
        struct
        {
            u8 option_name_tex_idx; // 0x0
            u8 kind;                // 0x1, 0 = blinking, 1 = rounded
            u8 description_idx;     // 0x2
            u8 x3;                  // 0x3
        } option[6];
    } menu_option_desc[9];
} ScMenuSelect;

// Air Ride Menu
typedef struct AirRideSelectMenuData
{
    struct
    {
        int x0;                 // 0x0
        int x4;                 // 0x4
        int x8;                 // 0x8
        u8 p_kind;              // 0xc
        u8 color_idx;           // 0xd
        u8 xe;                  // 0xe
        JOBJ *active_j;         // 0x10
        JOBJ *inactive_j;       // 0x14
        JOBJ *cpu_level_text_j; // 0x18
        JOBJ *level_bar_pos_j;  // 0x1c
        JOBJ *handicap_text_j;  // 0x20
        int x24;                // 0x24
        int x28;                // 0x28
        int x2c;                // 0x2c
        int x30;                // 0x30
        int x34;                // 0x34
    } ply[4];
} AirRideSelectMenuData;
typedef struct AirRideSelectCursor0Data
{
    int x0;           // 0x0
    int x4;           // 0x4
    int x8;           // 0x8
    JOBJ *outer_jobj; // 0xc
    int x10;          // 0x10
    u8 x14;           // 0x14
    u8 ply;           // 0x15
    u8 x16;           // 0x16
    u8 color;         // 0x17
    int x18;          // 0x18
    u8 x1c;           // 0x1c
    u8 x1d;           // 0x1d

} AirRideSelectCursor0Data;
typedef struct AirRideSelectCursor1Data
{
    int x0;                      // 0x0
    int x4;                      // 0x4
    int x8;                      // 0x8
    JOBJ *player_indicator_jobj; // 0xc
    JOBJ *background_jobj;       // 0x10
    JOBJ *arrow_jobj;            // 0x14
    u8 ply;                      // 0x18
    u8 x19;                      // 0x19
    u8 x1a;                      // 0x1a
    u8 x1b;                      // 0x1b
    u8 color_idx;                // 0x1c
} AirRideSelectCursor1Data;
typedef struct AirRideSelectCursor6Data
{
    int x0;                      // 0x0
    int x4;                      // 0x4
    int x8;                      // 0x8
    JOBJ *motion_jobj;           // 0xc
    JOBJ *background_jobj;       // 0x10
    JOBJ *player_indicator_jobj; // 0x14
    u8 ply;                      // 0x18
    u8 x19;                      // 0x19
    u8 x1a;                      // 0x1a
    u8 color_idx;                // 0x1b
    u8 x1c;                      // 0x1c
    u8 timer;                    // 0x1d
} AirRideSelectCursor6Data;

// City Trial Menu
typedef struct CitySelectMenuData
{
    struct
    {
        int x0;                 // 0x0
        int x4;                 // 0x4
        int x8;                 // 0x8
        u8 p_kind;              // 0xc
        u8 color_idx;           // 0xd
        u8 xe;                  // 0xe
        JOBJ *active_j;         // 0x10
        JOBJ *inactive_j;       // 0x14
        JOBJ *cpu_level_text_j; // 0x18
        JOBJ *level_bar_pos_j;  // 0x1c
        JOBJ *handicap_text_j;  // 0x20
        int x24;                // 0x24
        int x28;                // 0x28
        int x2c;                // 0x2c
        int x30;                // 0x30
        int x34;                // 0x34
    } ply[4];
} CitySelectMenuData;
typedef struct CitySelectCursor0Data
{
    int x0;           // 0x0
    int x4;           // 0x4
    int x8;           // 0x8
    JOBJ *outer_jobj; // 0xc
    int x10;          // 0x10
    u8 x14;           // 0x14
    u8 ply;           // 0x15
    u8 x16;           // 0x16
    u8 color;         // 0x17
    int x18;          // 0x18
    u8 x1c;           // 0x1c
    u8 x1d;           // 0x1d

} CitySelectCursor0Data;
typedef struct CitySelectCursor1Data
{
    int x0;                      // 0x0
    int x4;                      // 0x4
    int x8;                      // 0x8
    JOBJ *player_indicator_jobj; // 0xc
    JOBJ *background_jobj;       // 0x10
    JOBJ *arrow_jobj;            // 0x14
    u8 ply;                      // 0x18
    u8 x19;                      // 0x19
    u8 x1a;                      // 0x1a
    u8 x1b;                      // 0x1b
    u8 color_idx;                // 0x1c
} CitySelectCursor1Data;
typedef struct CitySelectCursor6Data
{
    int x0;                      // 0x0
    int x4;                      // 0x4
    int x8;                      // 0x8
    JOBJ *motion_jobj;           // 0xc
    JOBJ *background_jobj;       // 0x10
    JOBJ *player_indicator_jobj; // 0x14
    u8 ply;                      // 0x18
    u8 x19;                      // 0x19
    u8 x1a;                      // 0x1a
    u8 color_idx;                // 0x1b
    u8 x1c;                      // 0x1c
    u8 timer;                    // 0x1d
} CitySelectCursor6Data;

/*** Static Variables ***/
static ScMenuCommon *stc_scene_menu_common = (ScMenuCommon *)0x80558788;
static MajorSceneDesc *stc_major_scene_desc = (MajorSceneDesc *)0x80495058;
static MinorSceneDesc *stc_minor_scene_desc = (MinorSceneDesc *)0x80495154;
static ScMenuSelect *stc_menu_select = (ScMenuSelect *)0x804962b0;

/*
Scene Change Notes

MinorThink: run Scene_ExitMinor to execute Scene_Decide
SceneDecide: run either Scene_SetNextMinor to enter another minor, OR Scene_SetNextMajor then Scene_ExitMajor to enter another major
*/

void Gm_Minor();
MajorKind Scene_GetCurrentMajor();
MinorKind Scene_GetCurrentMinor();
void Scene_SetNextMajor(int major_id);  // run this in scene decide!
void Scene_ExitMajor();                 // run this to cause a major scene change, usually ran in scene decide!
void Scene_SetNextMinor(int minor_id);  // run this in scene decide!
void Scene_ExitMinor();                 // run this to cause a minor scene change, usually ran in scene think!
void Scene_SetDirection(int direction); // usually the button used to change scene
int Scene_GetDirection();               // usually the button used to change scene
void Scene_InitHeaps();                 //
ScMenuCommon *Gm_GetMenuData();         // 0x80558788
void MainMenu_InitAllVariables();
#endif
