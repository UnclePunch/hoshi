#ifndef KAR_H_MENU
#define KAR_H_MENU

#include "datatypes.h"
#include "obj.h"
#include "scene.h"

typedef enum MainMenuTopMenuKind
{
    MAINMENU_TOPMENU_AIRRIDE,
    MAINMENU_TOPMENU_TOPRIDE,
    MAINMENU_TOPMENU_CITY,
    MAINMENU_TOPMENU_OPTIONS,
    MAINMENU_TOPMENU_LAN,
} MainMenuTopMenuKind;

typedef enum MainMenuSubmenuKind
{
    MAINMENU_SUBMENU_AIRRIDE,
    MAINMENU_SUBMENU_TOPRIDE,
    MAINMENU_SUBMENU_CITY,
    MAINMENU_SUBMENU_OPTIONS,
    MAINMENU_SUBMENU_AIRRIDE_FREERUN,
    MAINMENU_SUBMENU_TOPRIDE_FREERUN,
    MAINMENU_SUBMENU_AIRRIDE_RECORDS,
    MAINMENU_SUBMENU_TOPRIDE_RECORDS,
    MAINMENU_SUBMENU_CITY_RECORDS,
} MainMenuSubmenuKind;

typedef struct MainMenuData
{
    u8 input_lockout;                     // 0x30
    u8 x31;                               // 0x31
    u8 menu_name_tex_idx;                 // 0x32
    u8 is_in_submenu;                     // 0x33
    u8 top_menu;                          // 0x34
    u8 cursor_val[2];                     // 0x35
    u8 depth;                             // 0x37, either 0 or 1. used to get cursor value above
    MainMenuSubmenuKind submenu_kind : 8; // 0x38, 0 = air ride, 1 = top ride, 2 = city, 3 = options, etc
    MajorKind major_kind : 8;             // 0x39,
    int x3c;                              // 0x3c
    int x40;                              // 0x40
    int x44;                              // 0x44
    int x48;                              // 0x48
    u16 x4c;                              // 0x4c
    s16 soundtest_bgm_kind;               // 0x4e
} MainMenuData;

typedef struct MenuElementData
{
    int x0;            // 0x0
    int kind;          // 0x4
    u8 is_visible : 1; // 0x8, 0x80
    u8 x8_40 : 1;      // 0x8, 0x40
    u8 x8_20 : 1;      // 0x8, 0x20
    u8 x8_10 : 1;      // 0x8, 0x10
    u8 x8_08 : 2;      // 0x8, 0x08
    u8 x8_04 : 2;      // 0x8, 0x04
    u8 x8_02 : 1;      // 0x8, 0x02
    u8 x8_01 : 1;      // 0x8, 0x01
} MenuElementData;

typedef struct SoundTestDesc
{
    u8 bgm;
    u8 x1;
    u8 x2;
    u8 x3;
    u8 kind; // 0 = unk, 1 = play via bgm id
} SoundTestDesc;

static HSD_Archive **stc_MnSelplyAll_archive = (HSD_Archive **)(0x805dd0e0 + 0x6dc);
static SoundTestDesc *stc_soundtest_desc = (SoundTestDesc *)0x80496458; // 62 of these

GOBJ *MenuElement_Create(JOBJDesc *jobjdesc);
MenuElementData *MenuElement_AddData(GOBJ *menu_element_gobj, int element_kind);

#endif