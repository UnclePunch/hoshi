#include "scene.h"
#include "game.h"
#include "text.h"
#include "credits.h"
#include <string.h>
#include <stdarg.h>

#include "hoshi.h"

#include "hoshi/mod.h"
#include "hoshi/credits.h"
#include "hoshi/settings.h"
#include "hoshi/func.h"

extern ModloaderData *stc_modloader_data;
extern char *g_iso_name; // dependent on the stack module loading the iso name

static int is_mod_credits_done;
static int is_final_credit_done;
static int stc_canvas_idx;
static GOBJ *stc_last_credit;
static int next_mod_idx;
static int stc_pause_timer = 0;

ModCredit *hoshi_credits[] = {
    &(ModCredit) {
        .role = "Project Lead, Programmer",
        .names = (char *[]){ "UnclePunch", NULL },
    },
    NULL,
};

static COBJDesc stc_cobj_desc = (COBJDesc){
    .class_name = 0,
    .flags = 0,
    .projection_type = PROJ_ORTHO,
    .viewport_left = 0,
    .viewport_right = 640,
    .viewport_top = 0,
    .viewport_bottom = 480,
    .scissor_lr = 640,
    .scissor_tb = 480,
    .eye_desc = &(WOBJDesc){.pos = (Vec3){0, 0, 300}},
    .interest_desc = &(WOBJDesc){.pos = (Vec3){0, 0, 0}},
    .roll = 0,
    .vector = 0,
    .near = 0.1,
    .far = 1000,
    .projection_param.ortho.bottom = -240,
    .projection_param.ortho.top = 240,
    .projection_param.ortho.left = -320,
    .projection_param.ortho.right = 320,
};

MajorSceneDesc major_desc = {
    .major_id = -1,
    .next_major_id = 0,
    .initial_minor_id = -1,
    .cb_Enter = CreditsMajor_Enter,
    .cb_ExitMinor = CreditsMajor_Exit,
};
MinorSceneDesc minor_scene = {
    .idx = -1,
    .x1 = -1,
    .cb_Load = CreditsMinor_Enter,
    .cb_Exit = CreditsMinor_Exit,
    .cb_ThinkPreGObjProc = CreditsMinor_Think,
    .preload_kind = 0, // 4 will preserve menu file preloads
};

OptionDesc g_credits_optiondesc = {
    .name = "Credits",
    .description = "View credits.",
    .pri = MENUPRI_VERYLOW,
    .kind = OPTKIND_SCENE,
    .major_idx = -1,
};

void Credits_Install()
{
    minor_scene.idx = Hoshi_InstallMinorScene(&minor_scene);
    major_desc.initial_minor_id = minor_scene.idx;
    major_desc.major_id = Hoshi_InstallMajorScene(&major_desc);

    g_credits_optiondesc.major_idx = major_desc.major_id;
}

void CreditsMajor_Enter()
{
    return;
}
void CreditsMajor_Exit()
{
    return;
}

void CreditsMinor_Enter()
{
    GOBJ *cam_g = GOBJ_EZCreator(0, 0, 0, 
                                0, 0, 
                                HSD_OBJKIND_COBJ, &stc_cobj_desc,
                                0, 0,
                                CObjThink_Common, 0, 0);
    cam_g->cobj_links = (1 << TEXT_GXLINK);

    // init text
    stc_canvas_idx = Text_CreateCanvas(0, -1, 0, 0, 0, TEXT_GXLINK, 0, -1);
    stc_last_credit = 0;
    is_mod_credits_done = 0;
    is_final_credit_done = 0;
    next_mod_idx = 0;

    GOBJ *g;
    CreditTextData *gp;

    // create mod name text
    g = CreditsText_Create();
    gp = g->userdata;
    gp->t[CREDITTEXT_MOD]->aspect = (Vec2){500, 32};
    CreditsText_AddString(g, CREDITTEXT_MOD, (GXColor){187, 187, 187, 255}, 0.6f, 0, 0, "- Credits -");
    CreditsText_AddString(g, CREDITTEXT_MOD, (GXColor){255, 255, 255, 255}, 0.8f, 0, 10, g_iso_name);

    // move text to the center of the screen
    gp->pos.Y = -gp->text_cursor.Y / 2;

    stc_pause_timer = (1 * 60);  // 3.75 seconds to sync with legendary theme

    // display hoshi credits
    g = CreditsText_Create();
    gp = g->userdata;
    gp->t[CREDITTEXT_MOD]->aspect = (Vec2){500, 32};
    CreditsText_AddString(g, CREDITTEXT_MOD, (GXColor){187, 187, 187, 255}, 0.6f, 0, 0, "hoshi");
    CreditsText_AddString(g, CREDITTEXT_MOD, (GXColor){187, 187, 187, 255}, 0.8f, 0, 10, "Created and Programmed by");
    CreditsText_AddString(g, CREDITTEXT_MOD, (GXColor){255, 255, 255, 255}, 0.8f, 0, 10, "UnclePunch");
    gp->text_cursor.Y += CREDIT_MOD_END_SPACING;
    gp->pos.Y = -stc_cobj_desc.projection_param.ortho.bottom;

    // hoshi_credits

    BGM_Play(BGM_FLOATISLANDS);
    return;
}
void CreditsMinor_Exit(void *data)
{
    if (Scene_GetDirection() == PAD_BUTTON_B)
    {
        GameData *gd = Gm_GetGameData();
        gd->main_menu.top_menu = MAINMENU_TOPMENU_OPTIONS;
        gd->main_menu.is_in_submenu = 1;
        gd->main_menu.cursor_val[0] = 4;
        gd->main_menu.cursor_val[1] = 0;

        Scene_SetNextMajor(MJRKIND_MENU);
        Scene_ExitMajor();
    }

    return;
}
void CreditsMinor_Think()
{
    // decrement pause timer if exists
    if (stc_pause_timer > 0)
        stc_pause_timer--;

    // check to create next credit
    if (!is_mod_credits_done && stc_pause_timer == 0 && stc_last_credit)
    {
        CreditTextData *gp = stc_last_credit->userdata;

        // check if bottom of last credit is X units from the bottom of the screen
        float bottom_y = -(gp->t[CREDITTEXT_MOD]->trans.Y + gp->text_cursor.Y);
        if (bottom_y >= stc_cobj_desc.projection_param.ortho.bottom + CREDIT_SPACING)
        {
            // get next mod with credits
            ModDesc *desc = 0;
            while (next_mod_idx < stc_modloader_data->mod_num)
            {
                desc = stc_modloader_data->mods[next_mod_idx++].desc;

                if (desc->credits)
                    break;
            }

            if (desc && desc->credits)
                CreditsText_AddModCredits(CreditsText_Create(), desc);
            else
            {
                // check if bottom of last credit is X units from the bottom of the screen
                if (bottom_y >= stc_cobj_desc.projection_param.ortho.bottom + CREDIT_FINAL_SPACING)
                {
                    is_mod_credits_done = 1;

                    // display final message
                    GOBJ *final_g = CreditsText_Create();
                    CreditTextData *final_gp = final_g->userdata;
                    final_gp->t[CREDITTEXT_MOD]->aspect = (Vec2){500, 32};
                    CreditsText_AddString(final_g, CREDITTEXT_MOD, (GXColor){255,255,255, 255}, 0.8f, 0, 10, "Thanks for playing\x81\x49");
                    final_gp->pos.Y = -stc_cobj_desc.projection_param.ortho.bottom;
                    final_gp->state = CREDITSTATE_FINALSTART;
                }

            }
        }
    }
    
    // check to exit via inputs
    if (Pad_GetDown(20) & (PAD_BUTTON_A | PAD_BUTTON_B | PAD_BUTTON_START))
    {
        BGM_Stop();
        SFX_Play(FGMMENU_CS_KETTEI);
        Scene_SetDirection(PAD_BUTTON_B);
        Scene_ExitMinor();
        return;
    }
    else if (is_final_credit_done)
    {
        Scene_SetDirection(PAD_BUTTON_B);
        Scene_ExitMinor();
        return;
    }

    return;
}

GOBJ *CreditsText_Create()
{
    // create credit gobj
    GOBJ *g = GOBJ_EZCreator(CREDITTEXT_ENTITY, CREDITTEXT_PLINK, 0,
                             sizeof(CreditTextData), CreditsText_Destroy,
                             HSD_OBJKIND_NONE, 0,
                             CreditsText_Think, 0,
                             0, 0, 0);

    CreditTextData *gp = g->userdata;

    // create text objects
    Text *t;
    for (int i = 0; i < CREDITTEXT_NUM; i++)
    {
        t = Text_CreateText(0, stc_canvas_idx);
        t->kerning = 1;
        t->use_aspect = 1;
        gp->t[i] = t;
    }

    // init text objects
    t = gp->t[CREDITTEXT_MOD];
    t->align = TEXTALIGN_CENTER;
    t->aspect = (Vec2){300, 32};
    t->scale = (Vec2){CREDIT_MOD_SCALE, CREDIT_MOD_SCALE};
    t->color = (GXColor){187, 187, 187, 255};
    
    t = gp->t[CREDITTEXT_ROLE];
    t->align = TEXTALIGN_RIGHT;
    t->aspect = (Vec2){230, 32};
    t->scale = (Vec2){CREDIT_NAME_SCALE, CREDIT_NAME_SCALE};
    t->color = (GXColor){187, 187, 187, 255};
    t->trans.X = -TEXTGLYPH_SIZE / 2;

    t = gp->t[CREDITTEXT_NAME];
    t->align = TEXTALIGN_LEFT;
    t->aspect = (Vec2){230, 32};
    t->scale = (Vec2){CREDIT_NAME_SCALE, CREDIT_NAME_SCALE};
    t->color = (GXColor){255, 255, 255, 255};
    t->trans.X = TEXTGLYPH_SIZE / 2;
    
    gp->pos = (Vec2){0, 0};
    gp->state = CREDITSTATE_NORMAL;
    
    stc_last_credit = g;

    return g;
}
void CreditsText_Destroy(CreditTextData *gp)
{
    for (int i = 0; i < CREDITTEXT_NUM; i++)
        Text_Destroy(gp->t[i]);

    HSD_Free(gp);
}
void CreditsText_Think(GOBJ *g)
{
    CreditTextData *gp = g->userdata;

    // update text logic
    if (stc_pause_timer == 0)
    {
        switch (gp->state)
        {
            case (CREDITSTATE_NORMAL):
            {
                gp->pos.Y -= SCROLL_SPEED;

                // check to destroy
                if (gp->pos.Y + gp->text_cursor.Y <= -stc_cobj_desc.projection_param.ortho.top)
                    GObj_Destroy(g);
                
                break;
            }
            case (CREDITSTATE_FINALSTART):
            {
                float y_center = (gp->pos.Y + (gp->text_cursor.Y / 2));

                // scroll logic
                if (y_center > 0)
                    gp->pos.Y -= SCROLL_SPEED;
                else
                {
                    // wait for last credit to be destroyed before fading out
                    if (CreditsText_IsNormalEnded())
                    {
                        gp->state = CREDITSTATE_FINALFADE;
                        gp->timer = 60;
                        
                        void (*BGM_FadeOut)(int dir, int frames, int bgm_slot) = (void *)0x8044576c;
                        BGM_FadeOut(0, 60 * 5, 1);
                    }
                }

                break;
            }
            case (CREDITSTATE_FINALFADE):
            {
                gp->timer--;

                // lerp fade out
                int alpha = lerp(255, 0, 1.0 - ((float)gp->timer / 60.0f));

                for (int i = 0; i < CREDITTEXT_NUM; i++)
                    gp->t[i]->color.a = alpha;

                if (gp->timer == 0)
                {
                    gp->state = CREDITSTATE_FINALEND;
                    gp->timer = 60;
                }

                break;
            }
            case (CREDITSTATE_FINALEND):
            {          
                if (--gp->timer == 0)
                {
                    GObj_Destroy(g);
                    is_final_credit_done = 1;
                }

                break;
            }
        }
    }
    
    // update position
    for (int i = 0; i < CREDITTEXT_NUM; i++)
        gp->t[i]->trans.Y = gp->pos.Y;

    return;
}
void CreditsText_AddModCredits(GOBJ *g, ModDesc *mod_desc)
{
    CreditTextData *gp = g->userdata;

    char *mod_name = mod_desc->name;
    ModCredit **mod_credits = mod_desc->credits;

    CreditsText_AddModName(g, mod_name);

    for (ModCredit **credit = mod_credits; *credit; credit++)
        CreditsText_AddRoleCredit(g, *credit);

    gp->text_cursor.Y += CREDIT_MOD_END_SPACING;

    // move it to the bottom of the screen
    gp->pos.Y = -stc_cobj_desc.projection_param.ortho.bottom;
}
void CreditsText_AddModName(GOBJ *g, char *s)
{
    CreditTextData *gp = g->userdata;
    Text *t = gp->t[CREDITTEXT_MOD];
    
    // add text
    float y_pos = gp->text_cursor.Y - (TEXTGLYPH_SIZE - (TEXTGLYPH_SIZE * CREDIT_MOD_SCALE));
    Text_AddSubtext(t, gp->text_cursor.X, y_pos, s);

    // update text cursor
    gp->text_cursor.Y += (CREDIT_MOD_SPACING + TEXTGLYPH_SIZE) * CREDIT_MOD_SCALE;
}
void CreditsText_AddRoleCredit(GOBJ *g, ModCredit *credit)
{
    CreditTextData *gp = g->userdata;
    Text *t_role = gp->t[CREDITTEXT_ROLE];
    Text *t_name = gp->t[CREDITTEXT_NAME];
    
    // first add the role
    float y_pos = gp->text_cursor.Y - (TEXTGLYPH_SIZE - (TEXTGLYPH_SIZE * CREDIT_NAME_SCALE));
    Text_AddSubtext(t_role, gp->text_cursor.X, y_pos, credit->role);

    // next add all the names
    for (char **name = credit->names; *name; name++)
    {    
        float y_pos = gp->text_cursor.Y - (TEXTGLYPH_SIZE - (TEXTGLYPH_SIZE * CREDIT_NAME_SCALE));
        Text_AddSubtext(t_name, gp->text_cursor.X, y_pos, *name);
        gp->text_cursor.Y += (CREDIT_NAME_SPACING + TEXTGLYPH_SIZE) * CREDIT_NAME_SCALE; // update text cursor
    }
    
    // space between roles
    gp->text_cursor.Y += CREDIT_ROLE_SPACING * CREDIT_NAME_SCALE;
}
void CreditsText_AddString(GOBJ *g, CreditTextKind text_kind, GXColor color, float scale, float x, float y, char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    CreditTextData *gp = g->userdata;
    Text *t = gp->t[text_kind];
    
    // format string
    char buf[256];
    vsnprintf(buf, sizeof(buf), fmt, args);

    // add text
    t->scale = (Vec2){scale, scale};
    t->color = color;
    float y_pos = (y * scale) + gp->text_cursor.Y - (TEXTGLYPH_SIZE - (TEXTGLYPH_SIZE * scale));
    Text_AddSubtext(t, gp->text_cursor.X, y_pos, buf);

    // update text cursor
    gp->text_cursor.Y += (y + TEXTGLYPH_SIZE) * scale;
}
int CreditsText_IsNormalEnded()
{
    int is_ended = 1;

    for (GOBJ *g = (*stc_gobj_lookup)[CREDITTEXT_PLINK]; g; g = g->next)
    {
        if (g->entity_class != CREDITTEXT_ENTITY)
            continue;

        CreditTextData *gp = g->userdata;

        if (gp->state == CREDITSTATE_NORMAL)
        {
            is_ended = 0;
            break;
        }
    }

    return is_ended;
}