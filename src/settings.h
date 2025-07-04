#ifndef MODSETTINGS_H
#define MODSETTINGS_H

#include "hoshi/settings.h" // public headers

#define SETTINGS_DEBUG 1

#define MENU_PLINK 31
#define MODSETTINGS_GXLINK 4
#define OPTION_GXPRI 1
#define TEXT_GXPRI 2
#define CURSOR_GXPRI 2

#define OPTION_NUM 5

#if SETTINGS_DEBUG == 1
#define LOG(...) OSReport(__VA_ARGS__)
#else
#define LOG(...) (sizeof(void *))
#endif

typedef enum MenuState
{
    TRANSKIND_NONE,
    TRANSKIND_ADVANCE,
    TRANSKIND_REGRESS,
} MenuState;

typedef struct SettingsData
{
    JOBJSet **ScMenValue_scene_models;
    JOBJSet **ScMenSelruleFrame_scene_models;
    JOBJSet **ScMenSelruleFrame2_scene_models;
    JOBJSet **ScMenSelruleCursor_scene_models;
    JOBJSet **ScMenTransition_scene_models;
    struct
    {
        GOBJ *cur_gobj;
        GOBJ *prev_gobj;
    } menu;
    GOBJ *cursor_gobj;
    GOBJ *transition_gobj;
    Text *description_text;
} SettingsData;

typedef struct OptionData
{
    OptionKind kind;
    JOBJSet *assets;
    struct
    {
        Text *text;
        JOBJ *j;
    } name;
    struct
    {
        Text *text;
        JOBJ *j;
    } value[1];
} OptionData;

typedef struct MenuData
{
    MenuDesc *desc;
    int is_remove;
    int option_num;
    OptionData option_data[OPTION_NUM];
} MenuData;

void MainMenu_ApplyPatches();

void Settings_Init(ModloaderData *mod_data);
int Settings_SortCallback(const void *a, const void *b);
void Settings_UpdateCurrentMenu();
void Settings_EnterNewMenu(MenuDesc *desc, MenuState trans_kind);
void Settings_ReqDestroy();
void Settings_Destroy(void *);
void Cursor_Think(GOBJ *g);
JOBJ *Option_Create(OptionDesc *desc, OptionData *op);
GOBJ *Menu_Create(MenuDesc *desc);
void Menu_CreateOptions(GOBJ *m);
void Menu_Think(GOBJ *m);
void Menu_Destroy(MenuData *mp);
void OptionText_GX(GOBJ *g, int pass);
void Menu_AddTransitionAnim(GOBJ *m, int anim_id);
void Menu_GetSaveSize(MenuDesc *desc, int *size);
void Option_GetSaveSize(OptionDesc *desc, int *size);

void Option_CopyFromSave(GlobalMod *mod, char *menu_name, OptionDesc *desc);
void Menu_CopyFromSave(GlobalMod *mod, char *menu_name, MenuDesc *desc);
void Option_CopyToSave(GlobalMod *mod, char *menu_name, OptionDesc *desc);
void Menu_CopyToSave(GlobalMod *mod, char *menu_name, MenuDesc *desc);
u16 Option_Hash(char *menu_name, char *option_name);

void swap_bytes(u8 *a, u8 *b, size_t size);
void qsort(u8 *base, size_t n, size_t size, int (*cmp)(const void *, const void *));
#endif