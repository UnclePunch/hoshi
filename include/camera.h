#ifndef KAR_H_CAM
#define KAR_H_CAM

#include "datatypes.h"

#define CM_CAMERA_MAX 32
typedef struct CameraParam
{
    Vec3 interest;         // 0x00
    Vec3 eye;              // 0x0c
    Vec3 up;               // 0x18
    float fov;             // 0x24
} CameraParam;

typedef struct CamInterest
{
    int x0;             // 0x00
    int x4;             // 0x04
    Vec3 pos;           // 0x08
    Vec3 pos_high;      // 0x14, this position is a little higher than the previous lol, its referenced when adjusting the camera position based on the c stick zoom value
    u8 x20[0x86];       // 0x20
    u16 ply;            // 0xa6
} CamInterest;

typedef struct CamData
{
    int kind;               // 0x0, 1 = normal, 9 = rail
    int x4;                 // 0x4
    int x8;                 // 0x8
    int xc;                 // 0xc
    int x10;                // 0x10
    CameraParam x14;        // 0x14  (copied from x138)
    u8 x3c[0x38];           // 0x3c
    u8 x74[0x10];           // 0x74
    u8 x84_80 : 1;          // 0x84
    float rotation_amt;     // 0x88
    float zoom_amt;         // 0x8c, max is 8.4
    float x90;              // 0x90
    CamInterest *target;    // 0x94, camera target. is 0x450 of riderdata?
    u8 x98[0x28];           // 0x98
    CameraParam xc0;        // 0xc0
    CameraParam xe8;        // 0xe8, gets copied directly from the cobj eye position @ 800b783c
    CameraParam x110;       // 0x110, final set of values? not sure
    CameraParam x138;       // 0x138, 
    u8 x160[0x20];
    Vec3 eye_pos;           // 0x180
    Vec3 interest_pos;      // 0x18c
    u8 x198[0x28];          // 0x198
    float x1c0;             // 0x1c0, rotation at least for when on foot
} CamData;

typedef struct PlayerCamData
{
    CamData *cam_data;      // 0x0
    COBJ *cobj;             // 0x4
    int x8;                 // 0x8
    u8 xc_80 : 1;           // 0xc, 0x80
    u8 xc_40 : 1;           // 0xc, 0x40
    u8 xc_20 : 1;           // 0xc, 0x20
    u8 xc_10 : 1;           // 0xc, 0x10
    u8 xd;                  // 0xd
    u8 xe;                  // 0xe
    u8 xf;                  // 0xf
    u8 x10[0x58];           // 0x10
    u16 ply;                // 0x68
    u16 view_index;         // 0x6a, for example if p1 and p4 are playing with 2 views, p4 would have 1 for this variable
    int x6c;                // 0x6c
    u8 controller_idx;      // 0x70
    u8 lod;                 // 0x71, level of detail this camera uses (1 = highest?), 0 = none?
    u8 x72;                 // 0x72
    u8 x73;                 // 0x73
} PlayerCamData;

typedef struct PlayerCamLookup
{
    u8 x0[0xac];                        // 0x0
    void *unk_data;                     // 0xac
    GOBJ *cam_gobjs[CM_CAMERA_MAX];     // 0xb0, has data PlayerCamData
    COBJ *cam_cobjs[CM_CAMERA_MAX];     // 0x130
    int x1b0[32];                       // 0x1b0
    int x230;                           // 0x230
    float *x234;                        // 0x234, unk struct array
    int x238;                           // 0x238
    u8 x23c[4];                         // 0x23c
    struct                              // 0x240
    {
        float normal;                   // 0x240
        float rail;                     // 0x244
    } ply_distance[4];
} PlayerCamLookup;

static PlayerCamLookup *stc_plycam_lookup = (PlayerCamLookup *)0x80557248; // array of 32, this is actually part of a larger struct

COBJ *PlyCam_GetCObj(int cam_index);

#endif