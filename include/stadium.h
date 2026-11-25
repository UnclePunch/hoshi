#ifndef KAR_H_STADIUM
#define KAR_H_STADIUM

#include "datatypes.h"

typedef enum StadiumKind
{
    STADIUMKIND_DRAG1,
    STADIUMKIND_DRAG2,
    STADIUMKIND_DRAG3,
    STADIUMKIND_DRAG4,
    STADIUMKIND_AIRGLIDER,
    STADIUMKIND_TARGETFLIGHT,
    STADIUMKIND_HIGHJUMP,
    STADIUMKIND_MELEE1,
    STADIUMKIND_MELEE2,
    STADIUMKIND_DESTRUCTION1,
    STADIUMKIND_DESTRUCTION2,
    STADIUMKIND_DESTRUCTION3,
    STADIUMKIND_DESTRUCTION4,
    STADIUMKIND_DESTRUCTION5,
    STADIUMKIND_SINGLERACE1,
    STADIUMKIND_SINGLERACE2,
    STADIUMKIND_SINGLERACE3,
    STADIUMKIND_SINGLERACE4,
    STADIUMKIND_SINGLERACE5,
    STADIUMKIND_SINGLERACE6,
    STADIUMKIND_SINGLERACE7,
    STADIUMKIND_SINGLERACE8,
    STADIUMKIND_SINGLERACE9,
    STADIUMKIND_VSKINGDEDEDE,
    STADIUMKIND_NUM,
} StadiumKind;

typedef enum StadiumGroup
{
    STADIUMGROUP_DRAGRACE,
    STADIUMGROUP_AIRGLIDER,
    STADIUMGROUP_TARGETFLIGHT,
    STADIUMGROUP_HIGHJUMP,
    STADIUMGROUP_MELEE,
    STADIUMGROUP_DESTRUCTION,
    STADIUMGROUP_SINGLERACE,
    STADIUMGROUP_VSKINGDEDEDE,
    STADIUMGROUP_NUM,
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