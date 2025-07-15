#ifndef KAR_H_HUD
#define KAR_H_HUD

#include "datatypes.h"
#include "obj.h"

typedef struct HudPliconData
{
    int x0;  // 0x0
    int x4;  // 0x4
    int x8;  // 0x8
    int xc;  // 0xc
    int x10; // 0x10
    int x14; // 0x14
    int x18; // 0x18
    int ply; // 0x1c
} HudPliconData;

typedef struct HudHpBarData
{
    int x0;           // 0x00
    int x4;           // 0x04
    u8 ply : 4;       // 0x08
    int xc;           // 0x0c
    int x10;          // 0x1c
    int x14;          // 0x1c
    int x18;          // 0x1c
    int x1c;          // 0x1c
    int x20;          // 0x2c
    int x24;          // 0x2c
    int x28;          // 0x2c
    int x2c;          // 0x2c
    int x30;          // 0x3c
    int x34;          // 0x3c
    int x38;          // 0x3c
    int x3c;          // 0x3c
    int x40;          // 0x4c
    int x44;          // 0x4c
    int x48;          // 0x4c
    JOBJ *kirby_jobj; // 0x4c
} HudHpBarData;

typedef struct HudMapIconData
{
    int x0;  // 0x00
    int x4;  // 0x04
    int x8;  // 0x08
    int xc;  // 0x0c
    int x10; // 0x10
    int x14; // 0x14
    int x18; // 0x1c
    int x1c; // 0x1c
    int ply; // 0x20
} HudMapIconData;

typedef struct HUDCityStatBarData
{
    int x0;            // 0x00
    int x4;            // 0x04
    int x8;            // 0x08
    int xc;            // 0x0c
    int x10;           // 0x10
    int x14;           // 0x14
    int x18;           // 0x1c
    int x1c;           // 0x1c
    int ply;           // 0x20
    JOBJ *bar_j;       // 0x24
    JOBJ *num_right_j; // 0x28
    JOBJ *num_left_j;  // 0x2c
    JOBJ *sign_j;      // 0x30
} HUDCityStatBarData;

typedef struct HUDElementData
{
    int x0;            // 0x0
    int x4;            // 0x4
    u8 x8_80 : 1;      // 0x8, 0x80
    u8 x8_40 : 1;      // 0x8, 0x40
    u8 x8_20 : 1;      // 0x8, 0x20
    u8 x8_10 : 1;      // 0x8, 0x10
    u8 ply : 2;        // 0x8, 0x0c
    u8 is_visible : 1; // 0x8, 0x02
    u8 x8_01 : 1;      // 0x8, 0x01
    int xc;            //
    int x10;           //
} HUDElementData;

static HSD_Archive **stc_if_all_archive = (HSD_Archive **)(0x805dd0e0 + 0x690);

void CityHUD_CreateStatChart(int ply, int ply2);
void CityHUD_CreateStatBar(int ply, int ply2, int stat_kind);

void HUD_PauseCreate();
void HUD_PauseDestroy();
GOBJ *HUD_CreateElement(int ply, JOBJDesc *j);
void HUD_GXLink(GOBJ *g, int pass);
void HUD_AddElementData(GOBJ *g, int size, int ply, int ply2);
void HUD_UpdateElement(JOBJ *j, int frame);

HSD_Archive **Gm_GetIfAllCityArchive();   // IfAll1c, contains common city trial specific graphics (timer, ready, pause, etc)
HSD_Archive **Gm_GetIfAllScreenArchive(); // IfAll1Xs, contains player related HUD that needs to be scaled down based on screen number

#endif