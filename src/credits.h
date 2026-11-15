#ifndef CREDITS_H
#define CREDITS_H

#include "hoshi/mod.h"
#include "hoshi/credits.h"
#include "hoshi/func.h"

#define TEXT_GXLINK 0

#define CREDITTEXT_ENTITY 0
#define CREDITTEXT_PLINK 1

#define CREDIT_SPACING 30
#define CREDIT_MOD_SCALE 0.6
#define CREDIT_NAME_SCALE 0.8

#define CREDIT_MOD_SPACING (TEXTGLYPH_SIZE / 2)     // space after the mod name
#define CREDIT_ROLE_SPACING (TEXTGLYPH_SIZE / 3)    // space between roles
#define CREDIT_NAME_SPACING (0)
#define CREDIT_MOD_END_SPACING (TEXTGLYPH_SIZE)     // space between mods
#define CREDIT_FINAL_SPACING (TEXTGLYPH_SIZE * 3)

#define SCROLL_SPEED (0.6)

typedef enum CreditTextKind
{
    CREDITTEXT_MOD,
    CREDITTEXT_ROLE,
    CREDITTEXT_NAME,
    CREDITTEXT_NUM,
} CreditTextKind;

typedef enum CreditTextState
{
    CREDITSTATE_NORMAL,
    CREDITSTATE_FINALSTART,
    CREDITSTATE_FINALFADE,
    CREDITSTATE_FINALEND,
} CreditTextState;

typedef struct CreditTextData
{
    Text *t[CREDITTEXT_NUM];
    Vec2 pos;
    Vec2 text_cursor;
    CreditTextState state;
    int timer;
} CreditTextData;

void Credits_Install();

void CreditsMajor_Enter();
void CreditsMajor_Exit();

void CreditsMinor_Enter();
void CreditsMinor_Exit(void *data);
void CreditsMinor_Think();

GOBJ *CreditsText_Create();
void CreditsText_Init(GOBJ *g, ModCredit *credit, float x, float y);
void CreditsText_Destroy(CreditTextData *gp);
void CreditsText_Think(GOBJ *g);

void CreditsText_AddModCredits(GOBJ *g, ModDesc *mod_desc);
void CreditsText_AddString(GOBJ *g, CreditTextKind text_kind, GXColor color, float scale, float x, float y, char *fmt, ...);
void CreditsText_AddModName(GOBJ *g, char *s);
void CreditsText_AddRoleCredit(GOBJ *g, ModCredit *credit);
int CreditsText_IsNormalEnded();

#endif