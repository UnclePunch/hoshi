#ifndef KAR_H_TRIGGER
#define KAR_H_TRIGGER

#include "datatypes.h"
#include "obj.h"

typedef enum TriggerStatus
{
    TRIGGERSTATUS_INACTIVE,       // debug code skips drawing this type
    TRIGGERSTATUS_1,
    TRIGGERSTATUS_2,
    TRIGGERSTATUS_ACTIVE,         //
} TriggerStatus;

typedef struct TriggerDesc
{
    int joint_idx;
    int x4;
    float scale;
    Vec3 offset;
} TriggerDesc;

typedef struct TriggerData
{
    void *x0;               // x0, hurtdesc?
    TriggerStatus state;    // x4
    int x8;                 //
    int xc;                 //
    int x10;                //
    int x14;                //
    int x18;                //
    float scale;            // 0x1c
    int x20;                //
    int x24;                //
    int x28;                //
    int x2c;                //
    int x30;                //
    int x34;                //
    int x38;                //
    int x3c;                //
    int x40;                //
    int x44;                //
    int x48;                //
    int x4c;                //
    int x50;                //
    int x54;                //
    int x58;                //
    int x5c;                //
} TriggerData;

#endif