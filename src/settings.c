#include "text.h"
#include "useful.h"
#include "hsd.h"
#include "preload.h"
#include "game.h"
#include "scene.h"
#include "inline.h"
#include "audio.h"

#include <string.h>

#include "hoshi.h"
#include "settings.h"
#include "save.h"

#include "code_patch/code_patch.h"

static int stc_text_canvas_idx = -1;
static SettingsData stc_settings_data;
static HSD_Archive *MnSettings_archive = 0;
static MatAnimJointDesc *stc_option_matanimjointdesc = 0;
static MatAnimJointDesc *stc_preview_matanimjointdesc = 0;

static int deflicker_enabled = 0;
static int resolution_kind = 0;

static MenuDesc main_menu = {0};
static MenuDesc default_menu = {
    .name = "_mainmenu",
    .option_num = 1,
    .options = (OptionDesc[]){
        {
            .name = "View Credits",
            .kind = OPTKIND_SCENE,
            .major_idx = MJRKIND_AIR,
        },
    },
};

static GlobalMod settings_mod = {
    .data.name = "_settings",
    .data.menu_desc = &default_menu,
    .data.OnSaveInit = 0,
};

//////////////
// Settings //
//////////////

GlobalMod *Menu_GetMod()
{
    return &settings_mod;
}

void Settings_Init(ModloaderData *mod_data)
{
    // build a menu at runtime consisting of the base settings and each active mods' settings

    // count number of mod settings
    int mod_settings_num = 0;
    for (int mod_idx = 0; mod_idx < mod_data->mod_num; mod_idx++)
    {
        if (mod_data->mods[mod_idx].data.menu_desc)
        {
            OSReport("Mod %s has settings menu\n", mod_data->mods[mod_idx].data.name);
            mod_settings_num++;
        }
    }

    // alloc option array
    OptionDesc *opt_arr = HSD_MemAlloc((sizeof(OptionDesc) * default_menu.option_num) + //
                                       (sizeof(OptionDesc) * mod_settings_num));        //
    main_menu.options = opt_arr;                                                        // store pointer

    // copy over mod options
    if (mod_settings_num > 0)
    {
        for (int mod_idx = 0; mod_idx < mod_data->mod_num; mod_idx++)
        {
            MenuDesc *mod_menu = mod_data->mods[mod_idx].data.menu_desc;
            if (mod_menu)
            {

                // determine this options position
                OptionDesc *this_opt = &opt_arr[main_menu.option_num];
                for (int i = 0; i < main_menu.option_num; i++)
                {
                }

                // if the menu only contains one scene option, put it directly on the main menu
                if (mod_menu->option_num == 1 &&
                    mod_menu->options[0].kind == OPTKIND_SCENE)
                {
                    *this_opt = mod_menu->options[0];
                }
                else // insert this menu as an option on the main menu
                {
                    this_opt->kind = OPTKIND_MENU;
                    this_opt->menu_ptr = mod_menu;
                }

                main_menu.option_num++;
            }
        }
    }

    // // install the credits scene
    // default_menu.options[0].major_idx = Credits_Init(); // install and set index

    // // copy the credits option
    // memcpy(&opt_arr[main_menu.option_num], &default_menu.options[0], sizeof(opt_arr[main_menu.option_num]));
    // main_menu.option_num++;

    return;
}
void Settings_Create()
{
    // init data
    stc_settings_data.ScMenValue_scene_models = Archive_GetPublicAddress(MnSettings_archive, "ScMenValue_scene_models");
    stc_settings_data.ScMenSelruleCursor_scene_models = Archive_GetPublicAddress(MnSettings_archive, "ScMenSelruleCursor_scene_models");
    stc_settings_data.ScMenSelruleFrame_scene_models = Archive_GetPublicAddress(MnSettings_archive, "ScMenSelruleFrame_scene_models");
    stc_settings_data.ScMenSelruleFrame2_scene_models = Archive_GetPublicAddress(MnSettings_archive, "ScMenSelruleFrame2_scene_models");
    stc_settings_data.ScMenTransition_scene_models = Archive_GetPublicAddress(MnSettings_archive, "ScMenTransition_scene_models");

    // GOBJ *p = GOBJ_EZCreator(38, 30, 0,
    //                          0, 0,
    //                          HSD_OBJKIND_JOBJ, ((JOBJSet **)Archive_GetPublicAddress(MnSettings_archive, "CullPlane_scene_models"))[0]->jobj,
    //                          0, 0,
    //                          JObj_GX, MODSETTINGS_GXLINK, 0);

    // create cursor
    GOBJ *c = GOBJ_EZCreator(38, 30, 0,
                             0, 0,
                             HSD_OBJKIND_JOBJ, stc_settings_data.ScMenSelruleCursor_scene_models[0]->jobj,
                             Cursor_Think, 2,
                             JObj_GX, MODSETTINGS_GXLINK, CURSOR_GXPRI + 1);
    JOBJ *cj = c->hsd_object;
    JObj_AddSetAnim(cj, 0, stc_settings_data.ScMenSelruleCursor_scene_models[0], 0, 1);
    stc_settings_data.cursor_gobj = c;

    MenuDesc *desc = &main_menu;
    desc->prev = 0;
    desc->cursor = 0;
    desc->scroll = 0;

    // create initial menu
    GOBJ *m = Menu_Create(desc);
    MenuData *mp = m->userdata;
    stc_settings_data.menu.cur_gobj = m;
    stc_settings_data.menu.prev_gobj = 0;

    // replace background's destroy function to destroy our stuff as well
    Gm_GetMenuData()->main.ScMenOpdelpanel_gobj->destructor_function = Settings_Destroy;

    Settings_UpdateCurrentMenu();
}
void Settings_Think()
{
    GOBJ *m = stc_settings_data.menu.cur_gobj;
    MenuData *mp = m->userdata;
    MenuDesc *desc = mp->desc;

    int down = Pad_GetDown(20);
    int rapid = Pad_GetRapidHeld(20);

    int max_onscreen_opt_num = desc->option_num;
    if (desc->option_num > OPTION_NUM)
        max_onscreen_opt_num = OPTION_NUM;

    if (rapid & PAD_BUTTON_UP)
    {
        int is_moved = 0;

        if (desc->cursor > 0)
        {
            desc->cursor--;
            is_moved = 1;
        }
        else if (desc->scroll > 0)
        {
            desc->scroll--;
            is_moved = 1;
        }
        else
        {
            desc->scroll = desc->option_num - max_onscreen_opt_num;
            desc->cursor = max_onscreen_opt_num - 1;
            is_moved = 1;
        }

        if (is_moved)
        {
            SFX_Play(FGMMENU_CS_MV);
            Settings_UpdateCurrentMenu();
        }
    }
    else if (rapid & PAD_BUTTON_DOWN)
    {
        int is_moved = 0;

        if (desc->cursor < max_onscreen_opt_num - 1)
        {
            desc->cursor++;
            is_moved = 1;
        }
        else if (desc->scroll < desc->option_num - OPTION_NUM)
        {
            desc->scroll++;
            is_moved = 1;
        }
        else
        {
            desc->cursor = 0;
            desc->scroll = 0;
            is_moved = 1;
        }

        if (is_moved)
        {
            SFX_Play(FGMMENU_CS_MV);
            Settings_UpdateCurrentMenu();
        }
    }

    OptionDesc *opt_desc = &desc->options[desc->cursor + desc->scroll];

    // left and right
    if (opt_desc->kind == OPTKIND_VALUE)
    {
        if (rapid & PAD_BUTTON_LEFT)
        {
            int is_moved = 0;

            if ((*opt_desc->val) > 0)
            {
                (*opt_desc->val)--;
                is_moved = 1;
            }
            else
            {
                (*opt_desc->val) = opt_desc->value_num - 1;
                is_moved = 1;
            }

            if (is_moved)
            {
                SFX_Play(FGMMENU_CS_MV);
                Settings_UpdateCurrentMenu();
            }
        }
        else if (rapid & PAD_BUTTON_RIGHT)
        {
            int is_moved = 0;

            if ((*opt_desc->val) < opt_desc->value_num - 1)
            {
                (*opt_desc->val)++;
                is_moved = 1;
            }
            else
            {
                (*opt_desc->val) = 0;
                is_moved = 1;
            }

            if (is_moved)
            {
                SFX_Play(FGMMENU_CS_MV);
                Settings_UpdateCurrentMenu();
            }
        }
    }

    // A and B
    if (down & (PAD_BUTTON_A | PAD_BUTTON_START))
    {
        if (opt_desc->kind == OPTKIND_MENU && opt_desc->menu_ptr)
        {

            opt_desc->menu_ptr->prev = desc;
            desc = opt_desc->menu_ptr;
            desc->cursor = 0;
            desc->scroll = 0;

            SFX_Play(FGMMENU_CS_KETTEI);
            Settings_EnterNewMenu(desc, TRANSKIND_ADVANCE);
        }
        else if (opt_desc->kind == OPTKIND_SCENE)
        {
            Scene_SetNextMajor(opt_desc->major_idx);
            Scene_ExitMajor();
            Scene_ExitMinor();

            SFX_Play(FGMMENU_CS_KETTEI);

            Menu_CopyAllModToSave(&main_menu);
            KARPlusSave_Write();
        }
    }
    else if (down & PAD_BUTTON_B)
    {
        MenuDesc *prev_desc = desc->prev;

        if (prev_desc)
        {
            desc->prev = 0;
            desc = prev_desc;
            Settings_EnterNewMenu(desc, TRANSKIND_REGRESS);
        }
        else
        {
            Settings_ReqDestroy();
            Menu_CopyAllModToSave(&main_menu);
            KARPlusSave_Write();
        }

        SFX_Play(FGMMENU_CS_CANCEL);
    }

    GOBJ *m_prev = stc_settings_data.menu.prev_gobj;
    if (m_prev)
    {
        MenuData *mp = m_prev->userdata;
        if (mp->is_remove)
        {
            JOBJ *j = JObj_GetIndex(m_prev->hsd_object, 1);

            if (j->aobj && j->aobj->flags == AOBJ_NO_ANIM)
            {
                GObj_Destroy(m_prev);
                stc_settings_data.menu.prev_gobj = 0;
            }
        }
    }
}

void Settings_EnterNewMenu(MenuDesc *desc, MenuState trans_kind)
{

    // remove previous menu if it still exists
    if (stc_settings_data.menu.prev_gobj)
    {
        GObj_Destroy(stc_settings_data.menu.prev_gobj);
        stc_settings_data.menu.prev_gobj = 0;
    }

    // update menu pointers
    stc_settings_data.menu.prev_gobj = stc_settings_data.menu.cur_gobj;
    stc_settings_data.menu.cur_gobj = Menu_Create(desc);

    // animate them
    if (trans_kind != TRANSKIND_NONE)
    {
        // determine animations
        int cur_menu_anim_id, prev_menu_anim_id;
        if (trans_kind == TRANSKIND_ADVANCE)
        {
            cur_menu_anim_id = 0;
            prev_menu_anim_id = 1;
        }
        else if (trans_kind == TRANSKIND_REGRESS)
        {
            cur_menu_anim_id = 2;
            prev_menu_anim_id = 3;
        }

        Menu_AddTransitionAnim(stc_settings_data.menu.cur_gobj,
                               cur_menu_anim_id);

        Menu_AddTransitionAnim(stc_settings_data.menu.prev_gobj,
                               prev_menu_anim_id);

        // request to remove the previous menu gobj after its animation ends
        MenuData *mp = stc_settings_data.menu.prev_gobj->userdata;
        mp->is_remove = 1;
    }
}
void Settings_UpdateCurrentMenu()
{
    GOBJ *m = stc_settings_data.menu.cur_gobj;
    MenuData *mp = m->userdata;
    MenuDesc *desc = mp->desc;

    // destroy options jobjs
    JObj_RemoveAll(JObj_GetIndex(stc_settings_data.menu.cur_gobj->hsd_object, 2));

    // destroy all options text
    mp = stc_settings_data.menu.cur_gobj->userdata;
    for (int opt_idx = 0; opt_idx < GetElementsIn(mp->option_data); opt_idx++)
    {
        if (mp->option_data[opt_idx].name.text)
        {
            Text_Destroy(mp->option_data[opt_idx].name.text);
            mp->option_data[opt_idx].name.text = 0;
            mp->option_data[opt_idx].name.j = 0;
        }

        for (int i = 0; i < GetElementsIn(mp->option_data[opt_idx].value); i++)
        {
            if (mp->option_data[opt_idx].value[i].text)
            {
                Text_Destroy(mp->option_data[opt_idx].value[i].text);
                mp->option_data[opt_idx].value[i].text = 0;
                mp->option_data[opt_idx].value[i].j = 0;
            }
        }
    }

    // recreate options
    JOBJ *option_root = JObj_GetIndex(m->hsd_object, 1);
    for (int opt_idx = 0; opt_idx < mp->option_num; opt_idx++)
    {
        OptionDesc *this_opt_desc = &desc->options[desc->scroll + opt_idx];
        OptionData *this_opt_data = &mp->option_data[opt_idx];

        JOBJ *oj = Option_Create(this_opt_desc, this_opt_data);

        JObj_AddNext(option_root, oj);

        // update selected status
        switch (this_opt_desc->kind)
        {
        case (OPTKIND_VALUE):
        {
            JObj_AddSetAnim(oj, 0, this_opt_data->assets, (opt_idx == desc->cursor), 0);
            break;
        }
        case (OPTKIND_MENU):
        case (OPTKIND_SCENE):
        {
            JObj_AddSetAnim(oj, (opt_idx == desc->cursor), this_opt_data->assets, 0, 1);
            break;
        }
        }

        // move into position
        float height = 5;
        JObj_GetIndex(oj, 1)->trans.Y = 11 - (opt_idx * height);
    }
}
void Settings_Destroy(void *data)
{
    void (*ScMenOpDelPanel_Destroy)(void *data) = (void *)0x80138d04;

    if (stc_settings_data.menu.cur_gobj)
    {
        GObj_Destroy(stc_settings_data.menu.cur_gobj);
        stc_settings_data.menu.cur_gobj = 0;
    }
    if (stc_settings_data.cursor_gobj)
    {
        GObj_Destroy(stc_settings_data.cursor_gobj);
        stc_settings_data.cursor_gobj = 0;
    }
    if (stc_settings_data.transition_gobj)
    {
        GObj_Destroy(stc_settings_data.transition_gobj);
        stc_settings_data.transition_gobj = 0;
    }

    // original code
    ScMenOpDelPanel_Destroy(data);
}
void Settings_ReqDestroy()
{

    void (*MainMenu_AnimateOptions)(u8 r3, u8 r4) = (void *)0x801328a8;
    void (*MainMenu_SetCursor)(u8 depth, u8 cursor_val, u8 r5) = (void *)0x80132234;
    void (*MainMenu_ShowOptions)(u8 depth, u8 opt_num) = (void *)0x80132130;
    void (*MainMenu_AnimateOptionDeleteBackgroundLeave)() = (void *)0x8014a438;

    GameData *gd = Gm_GetGameData();

    gd->main_menu.x31 = stc_menu_select->menu_option_desc[gd->main_menu.submenu_kind].option[1].option_name_tex_idx;
    gd->main_menu.menu_name_tex_idx = stc_menu_select->menu_desc[gd->main_menu.submenu_kind].menu_name_tex_idx;

    MainMenu_AnimateOptionDeleteBackgroundLeave();

    // 800176f4
    MainMenu_ShowOptions(gd->main_menu.depth, stc_menu_select->menu_desc[gd->main_menu.submenu_kind].option_num);

    // set option frames 8001772c
    for (int i = 0; i < 6; i++)
    {
        void (*MainMenu_SetOptionName)(u8 depth, u8 index, u8 option_name_tex_idx, u8 kind) = (void *)0x80132150;
        void (*MainMenu_SetOptionUnk)(u8 depth, u8 index) = (void *)0x80132214;

        MainMenu_SetOptionName(gd->main_menu.depth,
                               i,
                               stc_menu_select->menu_option_desc[gd->main_menu.submenu_kind].option[i].option_name_tex_idx,
                               stc_menu_select->menu_option_desc[gd->main_menu.submenu_kind].option[i].kind);

        MainMenu_SetOptionUnk(gd->main_menu.depth, i);
    }

    gd->main_menu.input_lockout = 10; //
    gd->main_menu.is_in_submenu = 1;  // submenu ID

    MainMenu_SetCursor(gd->main_menu.depth,
                       gd->main_menu.cursor_val[gd->main_menu.depth],
                       stc_menu_select->menu_option_desc[gd->main_menu.submenu_kind].option[gd->main_menu.cursor_val[gd->main_menu.depth]].description_idx);
    MainMenu_AnimateOptions(gd->main_menu.depth, gd->main_menu.menu_name_tex_idx);
}

GOBJ *Menu_Create(MenuDesc *desc)
{
    // create menu gobj
    GOBJ *m = GOBJ_EZCreator(38, MENU_PLINK, 0,
                             sizeof(MenuData), Menu_Destroy,
                             HSD_OBJKIND_JOBJ, 0,
                             Menu_Think, 0,
                             JObj_GX, MODSETTINGS_GXLINK, OPTION_GXPRI);

    MenuData *mp = m->userdata;

    // attach menu to background
    JObj_AttachPositionRotation(m->hsd_object,
                                JObj_GetIndex(Gm_GetMenuData()->main.ScMenOpdelpanel_gobj->hsd_object, 1));

    // add extra level to menu jobj to animate transitions
    JOBJ *mj = JObj_Alloc();
    JObj_AddNext(m->hsd_object, mj);

    // determine number of visible options at a time
    int opt_num = desc->option_num;
    if (desc->option_num > OPTION_NUM)
        opt_num = OPTION_NUM;

    mp->option_num = opt_num;
    mp->desc = desc;

    // create options
    for (int opt_idx = 0; opt_idx < opt_num; opt_idx++)
    {
        OptionDesc *this_opt_desc = &desc->options[desc->scroll + opt_idx];
        OptionData *this_opt_data = &mp->option_data[opt_idx];

        JOBJ *oj = Option_Create(this_opt_desc, this_opt_data);

        JObj_AddNext(mj, oj);

        // update selected status
        switch (this_opt_desc->kind)
        {
        case (OPTKIND_VALUE):
        {
            JObj_AddSetAnim(oj, 0, this_opt_data->assets, (opt_idx == desc->cursor), 0);
            break;
        }
        case (OPTKIND_MENU):
        {
            JObj_AddSetAnim(oj, (opt_idx == desc->cursor), this_opt_data->assets, 0, 1);
            break;
        }
        case (OPTKIND_SCENE):
        }

        // move into position
        float height = 5;
        JObj_GetIndex(oj, 1)->trans.Y = 11 - (opt_idx * height);
    }

    return m;
}
void Menu_Destroy(MenuData *mp)
{
    for (int opt_idx = 0; opt_idx < GetElementsIn(mp->option_data); opt_idx++)
    {
        Text_Destroy(mp->option_data[opt_idx].name.text);

        for (int i = 0; i < GetElementsIn(mp->option_data[opt_idx].value); i++)
        {
            if (mp->option_data[opt_idx].value[i].text)
                Text_Destroy(mp->option_data[opt_idx].value[i].text);
        }
    }

    HSD_Free(mp);
}
void Menu_Think(GOBJ *m)
{
    MenuData *mp = m->userdata;
    JOBJ *mj = m->hsd_object;

    JObj_SetMtxDirtySub(mj);

    JObj_AnimAll(mj);
}

JOBJ *Option_Create(OptionDesc *desc, OptionData *op)
{

    JOBJSet *set;
    if (desc->kind == OPTKIND_VALUE)
        set = stc_settings_data.ScMenSelruleFrame_scene_models[0];
    else if (desc->kind == OPTKIND_MENU || desc->kind == OPTKIND_SCENE)
        set = stc_settings_data.ScMenSelruleFrame2_scene_models[0];

    JOBJ *oj = JObj_LoadJoint(set->jobj);

    op->kind = desc->kind;
    op->assets = set;

    JObj_AddSetAnim(oj, 0, set, 0, 0);

    if (desc->kind == OPTKIND_VALUE)
    {
        // create values
        for (int val_idx = 0; val_idx < GetElementsIn(op->value); val_idx++)
        {
            JOBJ *val_j = JObj_LoadJoint(stc_settings_data.ScMenValue_scene_models[0]->jobj);
            val_j->trans.X = 2;

            JObj_AddNext(JObj_GetIndex(oj, 1), val_j);

            // create value
            Text *text = Text_CreateText(0, stc_text_canvas_idx);
            text->kerning = 1;
            text->align = 1;
            text->use_aspect = 1;
            text->color = (GXColor){0, 0, 0, 255};
            text->aspect = (Vec2){230, 32};
            text->gobj->gx_cb = OptionText_GX;
            text->is_depth_compare = 1;
            Text_AddSubtext(text, 0, -15, desc->value_names[*desc->val]);
            op->value[val_idx].text = text;
            op->value[val_idx].j = JObj_GetIndex(val_j, 2);
        }
    }

    // get option specific values
    int name_joint_idx;
    char *opt_name;
    switch (desc->kind)
    {
    case (OPTKIND_VALUE):
    {
        name_joint_idx = 6;
        opt_name = desc->name;
        break;
    }
    case (OPTKIND_MENU):
    {
        name_joint_idx = 11;
        opt_name = desc->menu_ptr->name;
        break;
    }
    case (OPTKIND_SCENE):
    {
        name_joint_idx = 11;
        opt_name = desc->name;
        break;
    }
    }

    // create option name text
    Text *text = Text_CreateText(0, stc_text_canvas_idx);
    text->kerning = 1;
    text->align = 1;
    text->use_aspect = 1;
    text->color = (GXColor){0, 0, 0, 255};
    text->aspect = (Vec2){190, 32};
    text->gobj->gx_cb = OptionText_GX;
    text->is_depth_compare = 1;
    Text_AddSubtext(text, 0, -15, opt_name);
    op->name.text = text;
    op->name.j = JObj_GetIndex(oj, name_joint_idx);

    return oj;
}
void OptionText_GX(GOBJ *g, int pass)
{
    if (pass != 2)
        return;

    COBJ *c = COBJ_GetCurrent();
    Text *t = g->userdata;

    JOBJ *j = 0;

    // painstakingly search for this text pointer
    GOBJ *m = (*stc_gobj_lookup)[MENU_PLINK];
    while (m)
    {
        if (m->entity_class == 38)
        {
            MenuData *mp = m->userdata;

            // find this text on the lookup
            for (int i = 0; i < mp->option_num; i++)
            {
                OptionData *op = &mp->option_data[i];

                if (op->name.text == t)
                {
                    j = op->name.j;
                    break;
                }
                else if (op->value[0].text == t)
                {
                    j = op->value[0].j;
                    break;
                }
            }

            if (j)
                break;
        }

        m = m->next;
    }

    if (!j)
    {
        OSReport("no joint found for text %p\n", t);
        assert("text_joint");
    }

    JObj_SetupMtxSub(j);

    // use jobj matrix
    Mtx backup_mtx;
    PSMTXCopy(&c->view_mtx, &backup_mtx);
    PSMTXConcat(&c->view_mtx, &j->rotMtx, &c->view_mtx);

    // render text
    Text_GX(g, pass);

    // restore cobj mtx
    PSMTXCopy(&backup_mtx, &c->view_mtx);
}
void Cursor_Think(GOBJ *g)
{
    MenuData *mp = stc_settings_data.menu.cur_gobj->userdata;

    // move cursor
    JOBJ *cursor_j = g->hsd_object;
    OptionData *sel_op = &mp->option_data[mp->desc->cursor];
    if (sel_op->kind == OPTKIND_VALUE)
    {
        JOBJ *oj = sel_op->value[0].j;
        JObj_SetupMtxSub(oj);
        HSD_MtxGetRotation(&oj->rotMtx, (Vec3 *)&cursor_j->rot);
        HSD_MtxGetTranslate(&oj->rotMtx, &cursor_j->trans);
    }
    else
        cursor_j->trans.Y = 100;
    JObj_SetMtxDirtySub(cursor_j);

    JObj_AnimAll(g->hsd_object);
    return;
}

void Menu_AddTransitionAnim(GOBJ *m, int anim_id)
{
    void (*JObj_ApplySetAnim)(JOBJ *j, int anim_id, JOBJSet *set) = (void *)0x80055a30;

    JOBJ *mj = JObj_GetIndex(m->hsd_object, 1);

    JObj_RemoveAnim(mj);
    JObj_AddAnim(mj, stc_settings_data.ScMenTransition_scene_models[0]->animjoint[anim_id], 0, 0);
    // JObj_ApplySetAnim(mj, anim_id, stc_settings_data.ScMenTransition_scene_models[0]);
    AOBJ_ReqAnim(mj->aobj, 0);
    mj->aobj->framerate = 1;
    JObj_Anim(mj);
}
void Menu_GetSaveSize(MenuDesc *desc, int *size)
{
    // recursively iterate through menus and count the amount of options

    if (!desc)
        return;

    for (int opt_idx = 0; opt_idx < desc->option_num; opt_idx++)
    {
        // process each option
        switch (desc->options[opt_idx].kind)
        {
        case (OPTKIND_MENU):
        {
            Menu_GetSaveSize(desc->options[opt_idx].menu_ptr, size);
            break;
        }
        case (OPTKIND_VALUE):
        {
            (*size) += 3;
            break;
        }
        }
    }

    return;
}

// Saving Functions
void _Menu_CopyFromSave(MenuDesc *desc, MenuSave *save, int save_num)
{
    for (int opt_idx = 0; opt_idx < desc->option_num; opt_idx++)
    {
        // process each option
        switch (desc->options[opt_idx].kind)
        {
        case (OPTKIND_MENU):
        {
            _Menu_CopyFromSave(desc->options[opt_idx].menu_ptr, save, save_num);
            break;
        }
        case (OPTKIND_VALUE):
        {
            // hash this option name
            u16 opt_hash = Menu_HashOption(desc, &desc->options[opt_idx]);

            // find it in the save data
            for (int i = 0; i < save_num; i++)
            {
                // copy save value
                if (save[i].hash == opt_hash)
                {
                    LOG("copying val %d from save for option %s\n", save[i].val, desc->options[opt_idx].name);
                    *desc->options[opt_idx].val = save[i].val;
                    break;
                }
                else if (save[i].hash == (u16)-1)
                {
                    LOG("hash not found\n", save[i].val, desc->options[opt_idx].name, "?");
                    break;
                }
            }

            break;
        }
        }
    }
}
void _Menu_CopyToSave(MenuDesc *desc, MenuSave *save, int save_num)
{
    for (int opt_idx = 0; opt_idx < desc->option_num; opt_idx++)
    {
        // process each option
        switch (desc->options[opt_idx].kind)
        {
        case (OPTKIND_MENU):
        {
            _Menu_CopyToSave(desc->options[opt_idx].menu_ptr, save, save_num);
            break;
        }
        case (OPTKIND_VALUE):
        {
            // hash this option name
            u16 opt_hash = Menu_HashOption(desc, &desc->options[opt_idx]);

            // find it in the save data
            for (int i = 0; i < save_num; i++)
            {
                // copy save value
                if (save[i].hash == opt_hash)
                {
                    LOG(" %s hash found, copying value %d to save.\n",
                        desc->options[opt_idx].name,
                        *desc->options[opt_idx].val);
                    save[i].val = *desc->options[opt_idx].val;
                    break;
                }
                else if (save[i].hash == (u16)-1)
                {
                    // free space, insert it
                    LOG(" %s hash created, copying value %d to save.\n", desc->options[opt_idx].name, *desc->options[opt_idx].val);
                    save[i].hash = opt_hash;
                    save[i].val = *desc->options[opt_idx].val;
                    break;
                }
            }
            break;
        }
        }
    }
}
void Menu_CopyFromSave(GlobalMod *mod)
{

    if (!mod || !mod->data.menu_desc)
        return;

    // get this mods settings data
    if (!mod->save.menu_data)
        return;

    _Menu_CopyFromSave(mod->data.menu_desc,
                       mod->save.menu_data,
                       mod->save.menu_size / sizeof(MenuSave));
}
void Menu_CopyToSave(GlobalMod *mod)
{
    if (!mod || !mod->data.menu_desc)
        return;

    // get this mods settings data
    if (!mod->save.menu_data)
        return;

    _Menu_CopyToSave(mod->data.menu_desc,
                     mod->save.menu_data,
                     mod->save.menu_size / sizeof(MenuSave));
}
void Menu_CopyAllModToSave(MenuDesc *desc)
{
    // // base settings
    // LOG("copying base settings to save\n");
    // Menu_CopyToSave(&settings_mod);

    // for each menu
    for (int opt_idx = 0; opt_idx < desc->option_num; opt_idx++)
    {
        if (desc->options[opt_idx].kind == OPTKIND_MENU)
        {
            GlobalMod *mod = Mods_GetFromMenuDesc(desc->options[opt_idx].menu_ptr);
            if (mod)
            {
                LOG("%s:\n", mod->data.name);
                Menu_CopyToSave(mod);
            }
            else
            {
            }
        }
    }
}
u16 Menu_HashOption(MenuDesc *menu_desc, OptionDesc *opt_desc)
{
    int menu_name_len = strlen(menu_desc->name);
    int opt_name_len = strlen(opt_desc->name);

    char *full_name = HSD_MemAlloc(menu_name_len + opt_name_len + 1);
    memcpy(full_name, menu_desc->name, menu_name_len);
    memcpy(full_name + menu_name_len, opt_desc->name, opt_name_len);
    full_name[menu_name_len + opt_name_len] = '\0';

    u16 hash = hashstr_16(full_name);

    HSD_Free(full_name);

    return hash;
}

///////////////////////
// Main Menu Patches //
///////////////////////

void MainMenu_OnLoad()
{
    HSD_Archive *archive_MnAll = *(HSD_Archive **)0x805dd7c8;
    MnSettings_archive = Archive_LoadFile("MnSettings.dat");

    if (!MnSettings_archive)
        return;

    // create a sane cobj where NO_ZUPDATE is not enabled
    GOBJ *c = GOBJ_EZCreator(40, 17, 0,
                             0, 0,
                             HSD_OBJKIND_COBJ, stc_scene_menu_common->sobj->cobjdesc[0],
                             0, 0,
                             CObjThink_Common, 0, 5);
    c->cobj_links = (1ULL << MODSETTINGS_GXLINK);

    // create a text canvas in the 3d world
    stc_text_canvas_idx = Text_CreateCanvas(0, -1, 41, 17, 0, MODSETTINGS_GXLINK, TEXT_GXPRI, -1);

    // get texture and anim assets
    stc_option_matanimjointdesc = Archive_GetPublicAddress(MnSettings_archive, "option_matanim_joint");
    stc_preview_matanimjointdesc = Archive_GetPublicAddress(MnSettings_archive, "preview_matanim_joint");

    // copy vanilla textures to custom animation
    JOBJSet **mencursor1 = Archive_GetPublicAddress(archive_MnAll, "ScMenCursor1_scene_models");
    MatAnimDesc *mencursor1_matanimdesc = Matanimjoint_GetMatAnimDescByIndex(mencursor1[0]->matanimjoint[0], 7, 0);
    MatAnimDesc_CopyTextures(stc_option_matanimjointdesc->matanim, mencursor1_matanimdesc);

    // copy vanilla textures to custom animation
    JOBJSet **menmark = Archive_GetPublicAddress(archive_MnAll, "ScMenMark_scene_models");
    MatAnimDesc *menmark_matanimdesc = Matanimjoint_GetMatAnimDescByIndex(menmark[0]->matanimjoint[0], 10, 0);
    MatAnimDesc_CopyTextures(stc_preview_matanimjointdesc->matanim, menmark_matanimdesc);
}
CODEPATCH_HOOKCREATE(0x80131e3c, "", MainMenu_OnLoad, "", 0)

void MainMenu_OnCreateOption(GOBJ *g)
{
    TOBJ *t = JObj_GetIndex(g->hsd_object, 7)->dobj->mobj->tobj;

    // add texanim
    TOBJ_AddAnim(t, stc_option_matanimjointdesc->matanim->texture_anim);

    return;
}
CODEPATCH_HOOKCREATE(0x8014911c, "mr 3, 29\n\t", MainMenu_OnCreateOption, "", 0)

void MainMenu_OnCreateMenMark(GOBJ *g)
{
    MOBJ *m = JObj_GetIndex(g->hsd_object, 10)->dobj->mobj;

    // add anim
    MOBJ_AddAnim(m, stc_preview_matanimjointdesc->matanim);

    return;
}
CODEPATCH_HOOKCREATE(0x80149bdc, "mr 3, 29\n\t", MainMenu_OnCreateMenMark, "", 0)

void OptionsMenu_OnPressA()
{
    void (*MainMenu_AnimateOptionsBack)(u8 r3) = (void *)0x801484a4;
    void (*MainMenu_AnimateMenuNameAdvance)(u8 frame_idx) = (void *)0x80146fbc;
    void (*MainMenu_CreateOptionDeleteBackground)() = (void *)0x8014a584;

    GameData *gd = Gm_GetGameData();

    gd->main_menu.input_lockout = 10; // input lockout timer
    gd->main_menu.is_in_submenu = 6;  // submenu ID

    MainMenu_AnimateOptionsBack(gd->main_menu.depth);
    MainMenu_AnimateMenuNameAdvance(40);
    MainMenu_CreateOptionDeleteBackground();

    Settings_Create();
}
CODEPATCH_HOOKCREATE(0x80016818, "mr 3, 29\n\t", OptionsMenu_OnPressA, "b 0x8\n\t", 0x80016d88)

void OptionsMenu_PerFrame()
{
    Settings_Think();
}
CODEPATCH_HOOKCREATE(0x800182c0, "mr 3, 31\n\t", OptionsMenu_PerFrame, "b 0x8\n\t", 0x800182f0)

void MainMenu_ApplyPatches()
{
    // inject @ 80149118 and replace the matanim with a custom one,
    // insert new texture on frame 44 (option idx 4 for menu 4)

    stc_menu_select->menu_desc[3].option_num = 5;
    stc_menu_select->menu_option_desc[3].option[4].description_idx = 3;
    stc_menu_select->menu_option_desc[3].option[4].option_name_tex_idx = 44;
    stc_menu_select->menu_option_desc[3].option[4].kind = 1;

    CODEPATCH_REPLACEINSTRUCTION(0x80016814, 0x41800238);
    CODEPATCH_REPLACEINSTRUCTION(0x800182bc, 0x41800028);

    CODEPATCH_HOOKAPPLY(0x80016818);
    CODEPATCH_HOOKAPPLY(0x800182c0);

    CODEPATCH_HOOKAPPLY(0x8014911c);
    CODEPATCH_HOOKAPPLY(0x80131e3c);
    CODEPATCH_HOOKAPPLY(0x80149bdc);
}