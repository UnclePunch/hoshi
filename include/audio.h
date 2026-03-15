#ifndef MEX_H_AUDIO
#define MEX_H_AUDIO

#include "structs.h"
#include "datatypes.h"

#define AXDRIVER_PIDMASK 0x0000003F // and with PID to get 

#define LBAUDIO_TRACK_AUTO_START     34 // evidenced @ 8005d684
#define LBAUDIO_TRACK_AUTO_END      383 // evidenced @ 8005d684
#define LBAUDIO_TRACK_AUTO_NUM      (LBAUDIO_TRACK_AUTO_END - LBAUDIO_TRACK_AUTO_START)

// FGMInstanceData flags
#define FGMINSTANCE_PRIORITY            0x2
#define FGMINSTANCE_VOLUME              0x4
#define FGMINSTANCE_CHANNEL_BALANCE     0x8
#define FGMINSTANCE_SET_1E              0x10
#define FGMINSTANCE_SET_PITCH           0x20
#define FGMINSTANCE_x40                 0x40
#define FGMINSTANCE_REVERB              0x80
#define FGMINSTANCE_END                 0x100
#define FGMINSTANCE_LOOP                0x200
#define FGMINSTANCE_END_LOOP            0x100000
#define FGMINSTANCE_START               0x40000000
#define FGMINSTANCE_END_PLAYBACK        0x80000000

typedef enum BGMKind
{
    BGM_LEGENDARYAIRRIDEMACHINE = 42,
    BGM_MENU = 60,
    BGM_NUM = 69,
} BGMKind;

typedef enum FGMGroup
{
    FGMGROUP_PERSIST,   // main, pokemon, nr_name, end
    FGMGROUP_ANNOUNCER, // nr_title, nr_select, nr_1p, nr_vs
    FGMGROUP_CHARACTER, // character ssms + null.ssm
    FGMGROUP_STAGE,     // stage ssms + last.ssm
    FGMGROUP_1P,        // ending, 1pend
} FGMGroup;

typedef enum FGM_Menu
{
    FGMMENU_CS_CANCEL = 0x10000,
    FGMMENU_CS_KETTEI,
    FGMMENU_CS_KETTEI_PRE,
    FGMMENU_CS_MV,
    FGMMENU_CS_BEEP1,
    FGMMENU_CS_DOOR_OPEN,
    FGMMENU_CS_DOOR_CLOSE,
    FGMMENU_CS_START_OK,
} FGM_Menu;

typedef enum FGM_Main
{
    FGMMAIN_START = 0x20000,
    FGMMAIN_INFO_TIMECOUNT1 = 0x20011,
    FGMMAIN_INFO_TIMECOUNT2,
} FGM_Main;

typedef enum FGM_Narrator_Select
{

    FGMNRSELECT_MULTIMAN = 30000,
    FGMNRSELECT_CHOOSECHAR = 30004,
    FGMNRSELECT_MELEE,
} FGM_Narrator_Select;

enum FGMID
{
    FGM_MAIN,
    FGM_POKEMON,
    FGM_NR_TITLE,
    FGM_NR_SELECT,
    FGM_NR_1P,
    FGM_NR_VS,
    FGM_CAPTAIN,
    FGM_CLINK,
    FGM_DK,
    FGM_DRMARIO,
    FGM_FALCO,
    FGM_FOX,
    FGM_GKOOPA,
    FGM_ICE,
    FGM_KIRBY,
    FGM_KOOPA,
    FGM_LINK,
    FGM_LUIGI,
    FGM_MARIO,
    FGM_MARS,
    FGM_MEWTWO,
    FGM_NESS,
    FGM_PEACH,
    FGM_PICHU,
    FGM_PIKACHU,
    FGM_PURIN,
    FGM_SAMUS,
    FGM_ZS,
    FGM_YOSHI,
    FGM_GW,
    FGM_GANON,
    FGM_EMBLEM,
    FGM_MHANDS,
    FGM_KIRBYTM,
    FGM_CASTLE,
    FGM_CORNERIA,
    FGM_GREATBAY,
    FGM_KONGO,
    FGM_MUTECITY,
    FGM_ONETT,
    FGM_ZEBES,
    FGM_GARDEN,
    FGM_KLAID,
    FGM_GREEN,
    FGM_VENOM,
    FGM_BIGBLUE,
    FGM_FOURSIDE,
    FGM_PUPUPU,
    FGM_PSTADIUM,
    FGM_1PADV,
    FGM_ENDING,
    FGM_NR_NAME,
    FGM_1PEND,
    FGM_LAST,
    FGM_END,
    FGM_NULL,
};

typedef enum AudioEmitterKind
{
    AUDIOEMITTER_NONE,
    AUDIOEMITTER_RIDER,
    AUDIOEMITTER_MACHINE,
    AUDIOEMITTER_WEAPON,
    AUDIOEMITTER_ENEMY,
    AUDIOEMITTER_ITEM,
    AUDIOEMITTER_MAP,
} AudioEmitterKind;

typedef enum AudioTrackOwner
{
    AUDIOTRACKOWNER_NONE,
    AUDIOTRACKOWNER_RIDER,
    AUDIOTRACKOWNER_MACHINE,
    AUDIOTRACKOWNER_WEAPON,
    AUDIOTRACKOWNER_ENEMY,
    AUDIOTRACKOWNER_ITEM,
    AUDIOTRACKOWNER_MAP,
} AudioTrackOwner;

typedef s32 AudioEmitter;
typedef u32 FGMInstance;        // this & SDA_13B8 = index in FGMInstanceData
typedef s32 PID;                // if this is -1, the FGM is not playing yet, still queued. AND with AXDRIVER_PIDMASK to access p_voices[] and to get VPB in ax_live->vpb[]                   

typedef struct _AXPBADDR
{
    u16 loopFlag;         // 0x0, one shot or looped sample (see above)
    u16 format;           // 0x2, sample format used (see above)
    u16 loopAddressHi;    // 0x4, Loop start
    u16 loopAddressLo;    // 0x6
    u16 endAddressHi;     // 0x8, Loop/sample end, including last sample played
    u16 endAddressLo;     // 0xa
    u16 currentAddressHi; // 0xc, Current playback position
    u16 currentAddressLo; // 0xe
} AXPBADDR;

typedef struct _AXPBMIX
{
    u16 vL;          // volume left
    u16 vDeltaL;     // volume ramp left
    u16 vR;          // volume right
    u16 vDeltaR;     // volume ramp right
    u16 vAuxAL;      // volume AUX A left
    u16 vDeltaAuxAL; // volume ramp AUX A left
    u16 vAuxAR;      // volume AUX A right
    u16 vDeltaAuxAR; // volume ramp AUX A right
    u16 vAuxBL;      // volume AUX B left
    u16 vDeltaAuxBL; // volume ramp AUX B left
    u16 vAuxBR;      // volume AUX B right
    u16 vDeltaAuxBR; // volume ramp AUX B right
    u16 vAuxBS;      // volume AUX B surround
    u16 vDeltaAuxBS; // volume ramp AUX B surround
    u16 vS;          // volume surround
    u16 vDeltaS;     // volume ramp surround
    u16 vAuxAS;      // volume AUX A surround
    u16 vDeltaAuxAS; // volume ramp AUX A surround
} AXPBMIX;

typedef struct _AXPBITD
{
    u16 flag;         // on or off for this voice
    u16 bufferHi;     // buffer in RAM
    u16 bufferLo;     // buffer in RAM
    u16 shiftL;       // phase shift samples left (current)
    u16 shiftR;       // phase shift samples right (current)
    u16 targetShiftL; // phase shift samples left (target)
    u16 targetShiftR; // phase shift samples right (target)
} AXPBITD;

typedef struct _AXPBUPDATE
{
    u16 updNum[5]; // number of updates per 1ms step
    u16 dataHi;    // location of update data in main memory
    u16 dataLo;
} AXPBUPDATE;

typedef struct _AXPBDPOP
{
    s16 aL;
    s16 aAuxAL;
    s16 aAuxBL;
    s16 aR;
    s16 aAuxAR;
    s16 aAuxBR;
    s16 aS;
    s16 aAuxAS;
    s16 aAuxBS;
} AXPBDPOP;

typedef struct _AXPBVE
{
    u16 currentVolume; // .15 volume at start of frame
    s16 currentDelta;  // signed per sample delta delta
} AXPBVE;

typedef struct _AXPBADPCM
{
    u16 a[8][2];    // coef table a1[0],a2[0],a1[1],a2[1]....
    u16 gain;       // gain to be applied (0 for ADPCM, 0x0800 for PCM8/16)
    u16 pred_scale; // predictor / scale combination (nibbles, as in hardware)
    u16 yn1;        // y[n - 1]
    u16 yn2;        // y[n - 2]
} AXPBADPCM;

typedef struct _AXPBFIR
{
    u16 numCoefs; // reserved, keep zero
    u16 coefsHi;  // reserved, keep zero
    u16 coefsLo;  // reserved, keep zero
} AXPBFIR;

typedef struct _AXPBSRC
{
    // ratio = (f32)ratio * 0x10000;
    u16 ratioHi;            // sampling ratio, integer
    u16 ratioLo;            // sampling ratio, fraction
    u16 currentAddressFrac; // current fractional sample position
    u16 last_samples[4];    // last 4 input samples
} AXPBSRC;

typedef struct _AXPBADPCMLOOP
{
    u16 loop_pred_scale; // predictor / scale combination (nibbles, as in hardware)
    u16 loop_yn1;        // y[n - 1]
    u16 loop_yn2;        // y[n - 2]
} AXPBADPCMLOOP;

typedef struct _AXPB
{
    u16 nextHi;              // 0x0, pointer to next parameter buffer (MRAM)
    u16 nextLo;              // 0x2,
    u16 currHi;              // 0x4, pointer to this parameter buffer (MRAM)
    u16 currLo;              // 0x6
    u16 srcSelect;           // 0x8, Select type of sample rate conversion (none,4-tap,linear)
    u16 coefSelect;          // 0xa, Coef. to be used with 4-tap SRC
    u16 mixerCtrl;           // 0xc, Mixer control bits
    u16 state;               // 0xe, current state
    u16 type;                // 0x10, type of voice
    AXPBMIX mix;             // 0x12
    AXPBITD itd;             // 0x36
    AXPBUPDATE update;       // 0x44
    AXPBDPOP dpop;           // 0x52
    AXPBVE ve;               // 0x64
    AXPBFIR fir;             // 0x68
    AXPBADDR addr;           // 0x6E
    AXPBADPCM adpcm;         // 0x7E
    AXPBSRC src;             // 0xA6
    AXPBADPCMLOOP adpcmLoop; // 0xB4
    u16 pad[3];              // 0xBA, 32 byte alignment
} AXPB;

typedef struct _AXVPB
{
    void *next;  // 0x0, used in priority stacks
    void *prev;  // 0x4, used in priority stacks
    void *next1; // 0x8, used in callback stack
    // these are used in voice allocation
    u32 priority;    // 0xc, index to stack
    void *callback;  // 0x10, user callback for specified
    u32 userContext; // 0x14, user assigned context for callback
    // vars & flags for updating and sync PBs
    u32 index;           // 0x18, index of VPB in array
    u32 sync;            // 0x1c, bit mask for each PB item to sync
    u32 depop;           // 0x20, should depop voice
    u32 updateMS;        // 0x24, update current ms
    u32 updateCounter;   // 0x28, counter for n updates
    u32 updateTotal;     // 0x2c, bounds checking for update block
    u16 *updateWrite;    // 0x30, write pointer for PB updates
    u16 updateData[128]; // 0x34, data for PB updates
    void *itdBuffer;     // 0x134, pointer to ITD buffer
    AXPB pb;             // 0x138, write params to this PB 0x
} AXVPB;

typedef struct HPSHeader
{
    char magic[8];               // 0x0
    int freq;                    // 0x8
    int channel_num;             // 0xc
    u16 is_loop;                 // 0x10
    u16 format;                  // 0x12
    int sa;                      // 0x14
    int ea;                      // 0x18
    int ca;                      // 0x1c
    u16 coef[16];                // 0x20
    u16 gain;                    // 0x40
    u16 initial_predictor_scale; // 0x42
    u16 initial_sample_history1; // 0x44
    u16 initial_sample_history2; // 0x46
} HPSHeader;

typedef struct HPSChunkHeader
{
    int length;           // 0x0
    int length_minus_one; // 0x4
    int next;             // 0x8
    u16 initPS;           // 0xc
    u16 initsh1;          // 0xe
    u16 initsh2;          // 0x10
    u16 gain;             // 0x12
    int extra;            // 0x14
    int x18;              // 0x18
    int x1c;              // 0x1c
} HPSChunkHeader;

struct BGMData // this name sucks, dont know enough about it to rename it
{
    unsigned int unk : 26;
    unsigned int vpb_index : 6;
};

struct VPB
{
    PID pid;                // 0x0, internally "vID". if this is 0, its inactive?
    int x4;                 // 0x4
    u8 x8;                  // 0x8
    u8 x9_80 : 1;           // 0x9, 0x80, doesnt adjust volume when this is raised
    u8 x9_40 : 1;           // 0x9, 0x40
    u8 x9_20 : 1;           // 0x9, 0x20
    u8 x9_10 : 1;           // 0x9, 0x10
    u8 is_initializing : 1; // 0x9, 0x08, is raised when initalizing the hps load process @ 8038b76c, is lowered when the first chunk is played @ 8038b310
    u8 x9_04 : 1;           // 0x9, 0x04
    u8 pause : 1;           // 0x9, 0x02, will not update the sound on audio frames. if this is not set it inits x24 when playing the hps @ 8038b180
    u8 req_stop : 1;        // 0x9, 0x01, set to delete? set @ 804496d0.
    u8 axvpb_num;           // 0xa, number of present axvpb's at 0xc
    u8 sg;                  // 0xb, seems to be used to access the static volume data indexed by sg at 8059a178
    AXVPB *axvpb[2];        // 0xc
    s16 x14;                // 0x14
    float pitch_x18;        // 0x18, are combined together to set ratioHi of AXPBSRC @ 8038b244
    float pitch_x1c;        // 0x1c, are combined together to set ratioHi of AXPBSRC @ 8038b244
    u16 currentVolume;      // 0x20, name derived from r4 @ 8038b200
    u8 is_updated_prev;     // 0x22, raised when prev is set @ 8038b33c
    u8 x27;                 // 0x23
    VPB *prev;              // 0x24, is equal to the previous value of stc_bgm_vpb, before it was replaced with a ptr to this one
    float volume_x28;       // 0x28, is used to calculate x24 @ 8038b194
    u16 x2c;                // 0x2c, seems to be the amount the volume needs to be lowered before reaching 0? is decremented each audio tick after stopping a sound (8044aa94). when its fully muted it begins freeing the voice (8044ac50 r25 is the amount of volume left to reach 0)
    u8 adjust_queued;       // 0x2e, bool raised when a change is queued, like volume. gets lowered @ 8044acf4 after its adjusted
    u8 x2f;                 // 0x2f
    int *x30;               // 0x30, 0x8 of this pointer is hps entrynum
    float volume_x34;       // 0x34, is used to calculate x24 @ 8038b194
    float volume_x38;       // 0x38 (gets ignored completely?)
    u8 x3c[0x5c];           // 0x3c
    // 0x98
};

struct AXLive
{
    VPB voice_data[64];                  // 0x0
    u8 x1784[0xc0];                      // ?, some constants
    BGMData bgm_data_lookup[263];        // 0x1D80, array of BGMData corresponding to SFX that were played with an instance_slot param > 0 when calling SFX_PlayRaw. this is used to stop previous instances when playing the same sfx
    HPSChunkHeader hps_chunk_headers[3]; // 0x1c60, circular buffer of 3 most recent hps headers
};

typedef struct 
{
    int x0;
    int x4;
    int bank_num;
    int bank_index_start[];     // contains "bank_num" elements;
    // after the array above exists a pointer array to all scripts i believe
    // this is pointed to via r13 @ stc_fgm_script_data
} SEMFile;                      // is the contents of the .sem file. alloc'd in audio heap, pointed to via r13

typedef struct FGMInstanceData
{
    FGMInstanceData *prev;      // 0x0
    FGMInstanceData *next;      // 0x4
    int flags;                  // 0x8. 0xFF seems to be some kind of bitfield related to vpb? not sure. this loops through and checks for bits in it 80441730
    FGMInstance instance;       // 0xc, index in the FGMInstanceData array
    PID pid;                    // 0x10, as evidenced by assert @ 80442538 and 80441274
    u32 sfx_id;                 // 0x14, what was passed into SFX_Play. internally fid
    u8 x18;                     // 0x18
    u8 priority;                // 0x19
    u16 audio_track;            // 0x1a. is r6 of SFX_Play
    u8 x1c;                     // 0x1c, seems to be limited between 2 and 29? @ 804428b4
    u8 x1d;                     // 0x1d
    u8 x1e;                     // 0x1e
    u8 x1f;                     // 0x1f
    u16 x20;                    // 0x20
    u16 x22;                    // 0x22, 
    u16 x24;                    // 0x24
    u16 x26;                    // 0x26
    u8 x28;                     // 0x28
    u8 pan;                     // 0x29
    u16 x2a;                    // 0x2a
    u8 x2c;                     // 0x2c
    u8 x2d;                     // 0x2d
    u8 x2e;                     // 0x2e
    u8 x2f;                     // 0x2f
    u8 x30;                     // 0x30
    u8 x31;                     // 0x31
    u16 x32;                    // 0x32
    u16 x34;                    // 0x34
    u16 x36;                    // 0x36
    u8 x38;                     // 0x38
    u8 x39;                     // 0x39
    u8 x3a;                     // 0x3a
    u8 x3b;                     // 0x3b
    u8 x3c;                     // 0x3c
    u8 sg;                      // 0x3d. "sound generator". r7 of SFX_Play, 0x42 of AudioEmitterData
    u8 x3e;                     // 0x3e
    u8 x3f;                     // 0x3f
    void *script_data;          // 0x40
    int x44;                    // 0x44
} FGMInstanceData;

/*
sfx_play notes:
    r20 = bank + sfxid
    r21 = volume
    r22 = pan
    r23 = unk passed in as r6
    r24 = unk passed in as r7
    r25 = unk passed in as r8
    r26 = unk passed in as r9
    r27 = audio_track. indexes data in audio_track_unk and must be under audio_track_num
    r28 = audio_voice. AudioEmitterData x2fc array index. is 0x42 of AudioEmitterData. checks if its between 0 and 63 8044273c
    r29 = script index
    r30 = FGMInstanceData

    r11 = bank_num

summary of findings:
    there are 64 sound generators, meaning 64 concurrent sounds can be played at once.
    these are referred to as "sg" in kar's code. they are marked in use with the array in Audio3D->sg.
    AudioEmitterData and FGMInstanceData contain indices to these

    AudioEmitter_Alloc -> AudioEmitter_Play -> AudioEmitter_GetSoundGenerator -> SFX_Play

    AudioEmitterData->sg, search through all FGMInstanceData to find a matching sg, then
    gets SDA_13AC[FGMInstanceData->instance.index] (which is the same struct?? lol) then
    uses the pid field to modify the sound. this goes through ax_live


documenting the very many IDs i have come across:

sg:
    - many structs contain this. FGMInstanceData, AudioEmitterData, VPB
    - allocation status is governed by Audio3D sg_status.
    - i believe its main purpose is to link AudioEmitterData to live FGMInstances.
        they both contain sg fields and the game loops through active FGMInstanceDatas
        and compares the sg fields before modifying the actual sounds.
    - there are also a few struct arrays indexed by sg.
        8059a178 - volume
pid:
    - exists in the FGMInstance.
    - used to access its FGMInstance via the p_voices array, and also
        used to access its VPB pointer

track:
    - 

vpb ptr:
    - linked list of these are pointed to at 0x1424(r13)
    - code starting @ 8044a800 runs every audio tick and loops
        through all of the VPB's. it gets its sg's volume 
        data @ 8059a178 and updates the volume, typically using
        AXSetVoiceVeDelta. this is as low level as it gets i think

fgm_instance:
    - exists in FGMInstance.
    - is generated using the sfx_id and a value at 0x13B0(r13) that 
        increments with every new sfx. 

The story so far:
    1. AudioEmitter_Play - creates an AudioEmitterData entry in the
        static array. stores audio track and gets a sg with 8005d6dc.
    2. SFX_Play - takes the sfx id, volume, pan, sg, and creates an
        FGMInstance with all of those as fields.
    3. AudioTick - executes the sfx script and creates the sound. a
        pid is allocated and its used to access the FGMInstance from 
        p_voices array but more importantly it ties to a VPB in AXLive->voices
    0. AudioEmitter_Update - calculates distance and updates volume 
        with 8044c284. this function updates the sg volume 
        array @ 8059a178. 
    0. AudioEmitter_SetVoiceParams - compares new params to existing ones
        and adjusts the sg with the new params if they've changed.
            8006039c - here r24 is an "is_stop" bool. it seems to be 
            raised when volume was above 10 and drops below 10
            800603e4 - here r25 is an "is_play" bool. it seems to be
            raised when volume was below 10 and is now above
    0. FGM_GetSoundGenerator @ 80443d8c:
        loops through all sg's to look for one not in use. an sg in
        use is characterized by an FGMInstance which shares the sg
        AND if the VPB associated with the FGMInstance's pid has flags
        x9_01 || is_initializing == 0 && any axvpb->vpb.state == 0

    0. audio tick runs and @ 8044a800 it loops through all of the VPB's. 
        it gets the VPB's sg and accesses the volume data @ 8059a178 
        and updates the volume for the AXVPB, typically using 
        AXSetVoiceVeDelta. this is as low level as it gets i think

*/

typedef struct
{
    AudioEmitterKind kind;   // 0x0, 0x33c
    int identifier;         // 0x4, for items, this is exist_index. machines will be kind==player and this will be 5. riders will use ply here
    int state;              // 0x8, 0 = free, 1 = free and has a sg assigned, 3 = unk @ 8005e0a8
    Vec3 xc;                // 0xc
    Vec3 x18;               // 0x18
    Vec3 x24;               // 0x24
    float range;            // 0x30
    int x34;                // 0x34 
    float distance;         // 0x38
    int x3c;                // 0x3c
    s16 x40;                // 0x40, is also a sound generator, not sure when this one is used
    s16 sg;                 // 0x42, "sound generator". is used to match with FGMInstances that use this data. this function determines/gets it 8005d6dc 
    u16 volume;             // 0x44, doesnt play if this is under 10? 8005f558
    u16 pitch;              // 0x46
    u16 pan;                // 0x48,
    u16 spa;                // 0x4a, spatial attenuation? higher = more positional based?
    u16 x4c;                // 0x4c
    u16 x4e;                // 0x4e
    u16 x50;                // 0x50
    u16 pri;                // 0x52
    u8 is_disable : 1;      // 0x54 0x80, gates logic @ 8005efc8. only used for debugging i think
    u8 is_paused : 1;       // 0x54 0x40, not sure but sounds are unconditionally played with the params specified when this is raised @ 80060168
    u8 is_playing : 1;      // 0x54 0x20
    u8 x54_10 : 1;          // 0x54 0x10, gates logic @ 8005efec
    u8 x54_08 : 1;          // 0x54 0x08
    u8 x54_04 : 1;          // 0x54 0x04
    u8 x54_02 : 1;          // 0x54 0x02
    u8 x54_01 : 1;          // 0x54 0x01
    u8 x55_80 : 1;          // 0x55 0x80
    u8 x55_40 : 1;          // 0x55 0x40
    u8 x55_20 : 1;          // 0x55 0x20
    u8 x55_10 : 1;          // 0x55 0x10
    u8 x55_08 : 1;          // 0x55 0x08
    u8 x55_04 : 1;          // 0x55 0x04
    u8 x55_02 : 1;          // 0x55 0x02
    u8 x55_01 : 1;          // 0x55 0x01
    struct                          // 0x58
    {
        int sfx_id;                 // 0x58, is -1 when null
        FGMInstance fgm_instance;   // 0x5c, is -1 when null
        int audio_track;            // 0x60,
        int is_queued : 1;          // 0x64
    } sounds[4];                    // 
    int x98;                        // 0x98
    int x9c;                        // 0x9c
    int xa0;                        // 0xa0
    int xa4;                        // 0xa4
    int xa8;                        // 0xa8
    int xac;                        // 0xac
    int xb0;                        // 0xb0
    int xb4;                        // 0xb4
} AudioEmitterData;

typedef struct Audio3D
{
    int x0;                         // 0x80538088, not sure, initialized num? seems to always be 1
    int largest_ssm_sizes[9];       // indexed by group?
    struct
    {
        // 0 is min, 255 is max.
        s16 x34;                    // 0x34
        s16 bgm_prev;               // 0x36
        s16 fgm_prev;               // 0x38
        s16 x3a;                    // 0x3a, BGM stop sets this to 255
        s16 bgm2_prev;              // 0x3c, idk, educated guess
        s16 x3e;                    // 0x3e
        u8 fgm_cur;                 // 0x40
        u8 bgm_cur;                 // 0x41
        u8 is_fgm_mute : 1;         // 
        u8 is_bgm_mute : 1;         // 
        u8 x42_20 : 1;              // 
        u8 x42_10 : 1;              // playing a secondary song raises this. BGM stop lowers this flag
        u8 x42_08 : 1;              // BGM stop lowers this flag
    } volume;
    struct
    {
        u8 status[LBAUDIO_TRACK_AUTO_NUM];      // 0x43, 0 = F, free?. 1 = R, released?. 2 = S, stay?. 3 = M, mixing? used by riders and machines. 4 = O, off? seems everything up to LBAUDIO_TRACK_AUTO_START is reserved
        u8 x1a0;                                // 
        u8 owner[LBAUDIO_TRACK_AUTO_NUM];       // AudioTrackOwner
        u8 x2f8[0xC];                           // unk
    } tracks;
    u8 sg_status[64];                   // 0x2ff, 0x80538387. 0 = free, 1 in use. 0-2 are reserved. index to this is stored to 0x42 of AudioEmitterData its looped through @ 8005d720
    AudioEmitterData sources[512];       // 0x33c, 0x805383c4. indexed by audio_3d
    u8 x1733C[0x6D4];                   // 8054F3C4
    int x17a10;                         // 8054fa98
    int x17a14;                         // 8054fa9c
    int x17a18;                         // 8054fba0
    int x17a1c;                         // 8054fba4, is limited to 300 @ 800614f8
    int x17a20;                         // 8054fba8
    int tick_num;                       // 8054fbac, increments every time 800614a8 runs
} Audio3D;

static u16 **stc_audio_track_unk = (u16 **)0x805de440;      // 0x1360(r13), 
static int *stc_audio_track_num = (int *)0x805de444;        // 0x1364(r13), number of audio tracks in audio_track_unk. determined by 80059e50 
static int *stc_audio_track_blacklist = (int *)0x805de450;  // 0x1370(r13), bitfield. (1 << audio_track) is AND'd with this value, if the flag is present it does not create the sfx

static Audio3D *audio_3d_data = (Audio3D *)0x80538088;   // indexed by value returned from Audio_AllocSource()

static PID *stc_bgm_pid = (PID *)0x80508bc8;                            // 0 = ?, 1 = main song, 2 = secondary song (event) is -1 when nothing is playing
static FGMInstanceData **p_voices = (FGMInstanceData **)0x8058e298;         // FGMInstance pointers that correspond to each pid. they are stored when the sfx script plays a sound @ 80441298
static AXLive *ax_live = (AXLive *)0x80597660;
static VPB **stc_vpb_adjust_queue = (VPB **)0x805de504;                 // 0x1424(r13), function 8044adb4 adds VPB's to this linked list and adjusts them next audio frame

static void **stc_fgm_script_data = (void **)0x805de460;        // 0x1380(r13)
static int *stc_fgm_script_num = (int *)0x805de464;             // 0x1384(r13) number of sem scripts?
static int **stc_fgm_bank_start_script = (int **)0x805de468;    // 0x1388(r13), array of ints containing the first script index in the bank
static int *stc_fgm_bank_num = (int *)0x805de46c;               // 0x138C(r13) seems to be the total number of banks on disc

static int *stc_fgm_instance_activenum = (int *)0x805de454;                     // 0x1374(r13), 
static FGMInstanceData **stc_fgm_data_start = (FGMInstanceData **)0x805de480;   // 0x13A0(r13), points to the first FGMInstanceData, used for iterating through
static FGMInstanceData **stc_fgm_data_next = (FGMInstanceData **)0x805de484;    // 0x13A4(r13), points to the next free FGMInstanceData @ 80442780
static int *stc_fgm_tick = (int *)0x805de488;                                   // 0x13A8(r13) how many times fgm audio has been updated (incremented @ 804422cc)
static FGMInstanceData **stc_fgm_data_unk = (FGMInstanceData **)0x805de48c;     // 0x13AC(r13) 
static int *stc_fgm_instance_totalnum = (int *)0x805de490;                      // 0x13B0(r13) how many times an SFX has been created
static int *stc_fgm_instance_mask = (int *)0x805de494;                          // 0x13B4(r13), AND'd with FGMInstance to get index
static int *stc_fgm_unk = (int *)0x805de498;                                    // 0x13B8(r13)

static BGMKind *stc_bgmkind_cur_playing = (BGMKind *)0x8054f508;         //

// look into 0x1458, 0x1430. used by HPS_AcquireVoice

int SFX_Play(int sfxID);                                                        // will play a sound effect with max priority (works when game is paused)
int SFX_PlayMenuSFX(int sfxID);                                                 //
int SFX_PlayRaw(int sfx, int volume, int pan, int audio_track, int sg);         // sg is 0x42 of AudioEmitterData any audio_track other than 0 will remember the current instance and destroy it if another is requested to play with that slot
int SFX_PlayCommon(int sfxID);
int SFX_PlayCrowd(int sfxID);
void SFX_StopCrowd();

void AudioHeap_SetAllocAndFree(void *alloc_func, void *free_func);
void Audio_ResetCache(int group_index);
void Audio_QueueFileLoad(int group_index, u64 ssm_index);
void Audio_UpdateCache();
void Audio_RequestSSMLoad(int ssm_id);
void Audio_SyncLoadAll();
void BGM_DecideMenuBGM();
int BGM_GetMenuBGM();
void BGM_PlayFile(char *filename, int volume, int pan, int stream_index);
void BGM_Play(int hpsID);
void BGM_Stop();
void BGM_Pause();
void BGM_Resume();
void BGM_LowerVolume();
void BGM_RaiseVolume();
int BGM_GetPreferredVolume();            // 0 - > 127, derived from sound options in main menu
void BGM_SetVolume(int volume, int unk); // 0 - > 127, unk is 1
int FGM_CheckActive(u32 fgm_id);
void FGM_Stop(u32 fgm_id);
void FGM_StopAll();
int FGM_CheckEnded(FGMInstance instance);
int FGM_SetVolume(u32 sfxid, u8 volume);
int FGM_SetPanning(u32 sfxid, u8 panning);
void FGM_ResumeKind(int kind); //
void FGM_PauseKind(int kind);  // pausing in-game pauses kinds 5,6,7,8
void FGM_LoadInGameBanks();

AudioEmitter AudioEmitter_Alloc(AudioEmitterKind kind, int idx); // 
void AudioEmitter_Free(AudioEmitter source); //
void AudioEmitter_SetPosition(AudioEmitter source, Vec3 *pos, float unk); // 
void AudioEmitter_Init(AudioEmitter source); // is called before playing. inits distance value
int AudioEmitter_CheckUnk(AudioEmitter source);
void AudioEmitter_Play(int sfx, int audio_track, AudioEmitter source); // you can play multiple sounds per track and prox?
int AudioTrack_Alloc(); // 
void AudioTrack_Free(int audio_track); // 

AXVPB *AXAcquireVoice(u32 priority, void *callback, u32 userContext);

#endif
