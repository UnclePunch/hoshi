
#include "scene.h"
#include "game.h"
#include "boot_scene.h"
#include <string.h>

#include "hoshi/func.h"
#include "hoshi/log.h"
#include "code_patch/code_patch.h"

BootScene g_boot_scene = {.kind = MJRKIND_MOVIE, .pri = 0};

void _BootScene_Set(MajorKind kind, u8 pri)
{
    if (pri >= g_boot_scene.pri)
    {
        LOG_INFO("setting boot scene to %d with pri %d", kind, pri);
        g_boot_scene.pri = pri;
        g_boot_scene.kind = kind;
    }
}

void BootScene_Apply()
{
    MajorKind major_kind = g_boot_scene.kind;
    GameData *gd = Gm_GetGameData();

    // init main menu variables just in case exiting our scene requires it?
    MainMenu_InitAllVariables();

    // perform some per scene initialization
    switch (major_kind)
    {
    case (MJRKIND_MOVIE):
    {
        major_kind = MJRKIND_TITLE;
        break;
    }
    case (MJRKIND_TITLE):
    {
        // gd->titlescreen_is_skip_op = 1;

        // adjust instruction to skip opening movie upon leaving the memcard scene
        CODEPATCH_REPLACEINSTRUCTION(0x80047748, 0x38600001);
        CODEPATCH_REPLACEINSTRUCTION(0x80047784, 0x38600001);

        break;
    }
    case (MJRKIND_MENU):
    {
        // do something to fix the text in top left
        break;
    }
    case (MJRKIND_CITY):
    {
        // set city trial flag and skip tutorial video
        gd->main_menu.is_in_submenu = 1;
        gd->main_menu.top_menu = MAINMENU_TOPMENU_CITY;
        gd->main_menu.submenu_kind = MAINMENU_TOPMENU_CITY;
        gd->main_menu.major_kind = major_kind;
        gd->city.mode = CITYMODE_TRIAL;
        gd->is_showed_tutorial_city = 1;

        // debug code to boot into city trial
        if (0)
        {
            static PlayerDesc hmn_desc = {
                .p_kind = PKIND_HMN,
                .rider_kind = RDKIND_KIRBY,
                .is_bike = 0,
                .machine_kind = VCKIND_COMPACT,
                .color = 0,
                .rumble = 1,
                .ply = 0,
                .x7 = -1,
                .cpu_level = 8,
                .x9 = -1,
                .xa = 0,
                .xb = 0,
                .xc = 0,
                .x10 = 0,
                .x14 = 0,
                .x18 = 0,
                .x1c = 0,
                .x20 = 0,
                .x24 = 0,
                .x28 = 0,
                .x2c = 0,
            };
            memcpy(&gd->ply_desc[0], &hmn_desc, sizeof(PlayerDesc));
            gd->ply_view_desc[0].flag = PLYCAM_ON;
            gd->time_seconds = 5 * 60;
            gd->city.x398 = 1;
        }

        // gd->city.mode = CITYMODE_STADIUM;
        break;

    }
    case (MJRKIND_AIR):
    {
        // set air ride mode and skip tutorial video
        gd->main_menu.is_in_submenu = 1;
        gd->main_menu.top_menu = MAINMENU_TOPMENU_AIRRIDE;
        gd->main_menu.submenu_kind = MAINMENU_TOPMENU_AIRRIDE;
        gd->main_menu.major_kind = major_kind;
        gd->airride_mode = AIRRIDEMODE_RACE;
        gd->is_showed_tutorial_airride = 1;
        break;
    }
    case (MJRKIND_LAN):
    {
        gd->main_menu.top_menu = MAINMENU_TOPMENU_LAN;
        break;
    }
    }

    // change scene
    Scene_SetNextMajor(major_kind);

}

void BootScene_Init()
{
    // disable save scene code from changing scene
    CODEPATCH_REPLACEINSTRUCTION(0x80047754, 0x60000000);
    CODEPATCH_REPLACEINSTRUCTION(0x800477f4, 0x60000000);

    CODEPATCH_REPLACEFUNC(Hoshi_SetBootScene, _BootScene_Set);
}