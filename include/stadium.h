#ifndef KAR_H_STADIUM
#define KAR_H_STADIUM

#include "datatypes.h"

typedef enum StadiumKind
{
    STKIND_DRAG1,
    STKIND_DRAG2,
    STKIND_DRAG3,
    STKIND_DRAG4,
    STKIND_AIRGLIDER,
    STKIND_TARGETFLIGHT,
    STKIND_HIGHJUMP,
    STKIND_MELEE1,
    STKIND_MELEE2,
    STKIND_DESTRUCTION1,
    STKIND_DESTRUCTION2,
    STKIND_DESTRUCTION3,
    STKIND_DESTRUCTION4,
    STKIND_DESTRUCTION5,
    STKIND_SINGLERACE1,
    STKIND_SINGLERACE2,
    STKIND_SINGLERACE3,
    STKIND_SINGLERACE4,
    STKIND_SINGLERACE5,
    STKIND_SINGLERACE6,
    STKIND_SINGLERACE7,
    STKIND_SINGLERACE8,
    STKIND_SINGLERACE9,
    STKIND_VSKINGDEDEDE,
    STKIND_NUM,
} StadiumKind;

typedef enum StadiumGroup
{
    STGROUP_DRAGRACE,
    STGROUP_AIRGLIDER,
    STGROUP_TARGETFLIGHT,
    STGROUP_HIGHJUMP,
    STGROUP_MELEE,
    STGROUP_DESTRUCTION,
    STGROUP_SINGLERACE,
    STGROUP_VSKINGDEDEDE,
    STGROUP_NUM,
} StadiumGroup;

typedef struct StadiumResults
{
    u8 ply_placement[4];  // 0xbc8
    int xbcc;             // 0xbcc
    int ply_race_time[4]; // 0xbd0
    int xbe0;             // 0xbe0
    int xbe4;             // 0xbe4
    int xbe8;             // 0xbe8
    int xbec;             // 0xbec
    int xbf0[4];          // 0xbf0
    u8 xc00[4];           // 0xc00
    int ply_points[4];    // 0xc04, 0x40
    float ply_dist[4];    // 0xc14
} StadiumResults;

static u8 *stc_stadium_option_to_kind = (u8 *)0x80535a9c;

StadiumGroup Gm_GetStadiumGroupFromKind(StadiumKind st_kind);
StadiumKind Gm_GetCurrentStadiumKind();
StadiumGroup Gm_GetCurrentStadiumGroup();
int Gm_StadiumIsDefaultUnlocked(StadiumKind kind);
int Gm_StadiumIsUnlocked(StadiumKind kind);
int Gm_StadiumRoundNum();

#endif