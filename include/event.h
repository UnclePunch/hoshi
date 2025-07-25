#ifndef KAR_H_EVENT
#define KAR_H_EVENT

#include "datatypes.h"
#include "stadium.h"

typedef enum EventKind
{
    EVKIND_DYNABLADE,
    EVKIND_TAC,
    EVKIND_METEOR,
    EVKIND_PILLAR,
    EVKIND_RUNAMOK,
    EVKIND_RESTORATIONAREA,
    EVKIND_RAILFIRE,
    EVKIND_SAMEITEM,
    EVKIND_LIGHTHOUSE,
    EVKIND_SECRETCHAMBER,
    EVKIND_PREDICTION,
    EVKIND_MACHINEFORMATION,
    EVKIND_UFO,
    EVKIND_BOUNCE,
    EVKIND_FOG,
    EVKIND_FAKEPOWERUPS,
    EVKIND_NUM,
} EventKind;

typedef struct EventCheckData
{
    struct
    {
        struct
        {
            int event_delay_min;                     // 0x0
            int event_delay_max;                     // 0x4
            int event_occur_chance;                  // 0x8
            int event_skip_chance;                   // 0xc
            u8 x10[0x8];                             // 0x10
            int prev_kind_num;                       // 0x18
            int x1c;                                 // 0x1c
            int x20;                                 // 0x20
            int x24;                                 // 0x24
            int x28;                                 // 0x28
            struct                                   // 0x2c, contains a weight for every event for every type of stadium
            {                                        //
                int weight[STGROUP_NUM][EVKIND_NUM]; //
            } *event_weights;                        //
        } *param;                                    //
    } *data;
    int state; // 0 = no event, 1 = event starting, 2 = event active, 3 = event ending
    EventKind cur_kind;
    int xc;
    int timer;             // 0x10, counts up
    int event_time;        // 0x14, event triggers when timer matches this number
    int prev_kind[10];     // 0x18, last 10 events that have occurred
    int prev_kind_num;     // 0x40, number of events that occurred this match
    u8 x44[0x40];          // 0x44
    EventKind reserve[16]; // 0x84
    int reserve_kind_num;  // 0xc4. max is 16 (Gr_EventGene_Reserve_Num)
} EventCheckData;

static GOBJ **stc_eventcheck_gobj = (GOBJ **)(0x805dd0e0 + 0x618);
static int *stc_event_machineformation_loadnum = (int *)(0x805dd0e0 + 0x750); // number of machines spawned for the machine formation event

#endif