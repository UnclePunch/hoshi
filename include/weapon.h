#ifndef MEX_H_WEAPON
#define MEX_H_WEAPON

#include "structs.h"
#include "datatypes.h"
#include "os.h"
#include "audio.h"
#include "hurt.h"
#include "trigger.h"

typedef enum WeaponPri
{
    WPPRI_0,
    WPPRI_ANIM,
    WPPRI_PHYS = 4,
    WPPRI_ENVCOLL,
    WPPRI_6,
    WPPRI_TRIGGER, // collect powerup collision, also frees sounds
    WPPRI_8,
    WPPRI_HITCOLL,
    WPPRI_DMGAPPLY,
    WPPRI_13 = 13,
    WPPRI_15 = 15,
} WeaponPri;

typedef enum WeaponKind
{
    WPKIND_SPITSMALL,
    WPKIND_SPITLARGE,
    WPKIND_2,
    WPKIND_3,
    WPKIND_BOMB,
    WPKIND_PLASMA1,
    WPKIND_PLASMA2,
    WPKIND_PLASMA3,
    WPKIND_PLASMA4,
    WPKIND_PLASMA5,
    WPKIND_PLASMA6,
    WPKIND_11,
    WPKIND_12,
    WPKIND_ICE,
    WPKIND_CRACKER,
    WPKIND_TIMEBOMB,
    WPKIND_GORDO,
} WeaponKind;

typedef struct WeaponDesc
{
    int x0;
} WeaponDesc;

typedef struct WeaponData
{
    int x0;                         // 0x0
    WeaponKind kind;                // 0x4
    GOBJ *creator_gobj;             // 0x8
    GOBJ *owner_gobj;               // 0xc
    int x10;                        // 0x10
    int x14;                        // 0x14
    int x18;                        // 0x18
    int x1c;                        // 0x1c
    int x20;                        // 0x20
    int state;                      // 0x24
    u8 x28[0x154];                  // 0x28
    DmgLog dmg_log;                 // 0x17c
} WeaponData;

GOBJ *Weapon_Create(WeaponDesc *desc);
void Weapon_StateChange(WeaponData *wp);

#endif