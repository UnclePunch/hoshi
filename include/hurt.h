#ifndef KAR_H_HURT
#define KAR_H_HURT

#include "datatypes.h"
#include "obj.h"

typedef enum HurtKind
{
    HURTKIND_RIDER,
    HURTKIND_1,
    HURTKIND_MACHINE,
    HURTKIND_3,
    HURTKIND_POWERUP,
    HURTKIND_5,
    HURTKIND_STAGE,
} HurtKind;

typedef struct HurtData
{
    HurtKind kind;       // x0
    void *desc;          // x4, hurtdesc?
    int xc_obj_kind;     // x8, stages use 2, rider and machines use 4,
    void *xc_obj;        // xc
    int x10;             // x10
    void *x14;           // x14
    void *x18;           // x18
    int hitcoll_log_idx; // 0x1c, index in HitCollData::log
    int x20;             // x20
    float kb_mag;        // 0x24
    float dmg_taken;     // 0x28
    float x2c;           // 0x2c
    int x30;             // 0x30
    int x34;             //
    int x38;             //
    int x3c;             //
    int x40;             //
    int x44;             //
    Vec3 coll_pos;       // 0x48
    int flags;           // 0x54
    int x58;             // 0x58
    int x5c;             // 0x5c
    int x60;             // 0x60
    int x64;             // 0x64
    void *x68_obj;       // 0x68
    u8 x6c[0x20];        // 0x6c
    struct
    {
        int kind;         // 0x88, 0 = vuln, 2 = intang
        int x8c;          // 0x8c
        int x90;          // 0x90
        int intang_timer; // 0x94
        int invuln_timer; // 0x94
    } vuln;
} HurtData;

typedef struct HitCollData
{
    int x0; // 0x0
    int x4; // 0x4
    u8 x8;  // 0x8
    u8 x9;  // 0x9
    u8 xa;  // 0xa
    u8 xb;  // 0xb
    struct
    {
        HurtData *victim_hurt_data; // 0xc
        void *x10;                  // 0x10
        void *x14;                  // 0x14
        Vec3 coll_pos;              // 0x18
        float dmg;                  // 0x24
    } log[20];
    int coll_num;        // 0x23c. amount of collisions found against this hurtbox
    HurtData *hurt_data; // 0x240. hurt data we are checking for hit collisions against
} HitCollData;

typedef struct DmgLog //
{                     //
    int xbac;         // 0xbac, 0x0
    int xbb0;         // 0xbb0, 0x4
    int xbb4;         // 0xbb4, 0x8
    int xbb8;         // 0xbb8, 0xC
    int xbbc;         // 0xbbc, 0x10
    int xbc0;         // 0xbc0, 0x14
    u16 xbc4;         // 0xbc4, 0x18
    u16 xbc6;         // 0xbc6, 0x1a
    int attacker_ply; // 0xbc8, 0x1c
} DmgLog;             //

static HitCollData *stc_hitcolldata = (HitCollData *)0x80559bf4;

HurtData *HurtData_Create(void *desc, HurtKind kind, int obj1_kind, int obj2_kind, int obj3_kind);
void HurtData_GiveIntangibility(HurtData *hurt, int timer);

#endif