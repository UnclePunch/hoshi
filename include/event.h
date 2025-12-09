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
            int delay_min;                     // 0x0
            int delay_max;                     // 0x4
            int occur_chance;                  // 0x8
            int skip_chance;                   // 0xc
            u8 x10[0x8];                             // 0x10
            int prev_kind_num;                       // 0x18
            int music_fadeout_frames;                // 0x1c, number of frames to fade out the music
            int x20;                                 // 0x20
            int x24;                                 // 0x24
            int x28;                                 // 0x28
            struct                                   // 0x2c, contains a weight for every event for every type of stadium
            {                                        //
                int arr[STGROUP_NUM][EVKIND_NUM]; //
            } *weights;                        //
            struct                                   // 0x2c, contains a weight for every event for every type of stadium
            {                                        //
                struct                                   // 0x2c, contains a weight for every event for every type of stadium
                {                                        //
                    u8 x0;                           //
                    u8 x1;                           //
                    u8 x2;                           //
                    u8 x3;                           //
                    u8 x4;                           //
                    u8 x5;                           //
                    u8 x6;                           //
                    u8 x7;                           //
                    u8 x8;                           //
                    u8 is_siren;                     // will play siren before starting this event
                    u8 xa;                           //
                    u8 xb;                           //
                } arr[EVKIND_NUM];                  //
            } *param;  
        } *event;                                    //
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

typedef struct EventGlobal
{
    u8 x0[0x42];
    u8 x42_80 : 1;          // 0x42, 0x80
    u8 x42_40 : 1;          // 0x42, 0x40
    u8 x42_20 : 1;          // 0x42, 0x20
    u8 is_song_playing : 1; // 0x42, 0x10
} EventGlobal;

typedef struct EventFunction
{
    void *x0;
    void *x4;
    void *x8;
    void *xc;
    int (*check)(EventCheckData *gp);
} EventFunction;

static GOBJ **stc_eventcheck_gobj = (GOBJ **)(0x805dd0e0 + 0x618);
static int *stc_event_machineformation_loadnum = (int *)(0x805dd0e0 + 0x750); // number of machines spawned for the machine formation event
static EventGlobal *stc_event_global = (EventGlobal *)0x80538088;
static EventFunction (*stc_event_function)[EVKIND_NUM] = (void *)0x804a5410;
#endif