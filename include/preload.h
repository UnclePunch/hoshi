#ifndef MEX_H_PRELOAD
#define MEX_H_PRELOAD

#include "structs.h"
#include "datatypes.h"
#include "game.h"

#define PRELOADFLAG_FTCOSTUME (1 << 0) // 0x01, fighter costume files
#define PRELOADFLAG_FTDAT (1 << 1)     // 0x02, main fighter PlX dat files
#define PRELOADFLAG_EFFECT (1 << 2)    // 0x04, effect files
#define PRELOADFLAG_FTANIM (1 << 3)    // 0x08, fighter animations
#define PRELOADFLAG_ALLOC (1 << 4)     // 0x10, special allocs like fod reflection
#define PRELOADFLAG_x20 (1 << 5)       // 0x20, unk
#define PRELOADFLAG_PERSIST (1 << 6)   // 0x40, IfAll / EfMnData / EfCoData / ItCo
#define PRELOADFLAG_x80 (1 << 7)       // 0x80, LbRb

typedef enum PreloadEntryLoadState // 0x0 of PreloadEntry
{
    PRELOADENTRYSTATE_NONE,
    PRELOADENTRYSTATE_QUEUED,
    PRELOADENTRYSTATE_LOADING,
    PRELOADENTRYSTATE_LOADED,
    PRELOADENTRYSTATE_ACCESSED,
} PreloadEntryLoadState;

typedef enum PreloadEntryInitKind // 0x3 of PreloadEntry
{
    PRELOADENTRYINITKIND_NOINIT,     // do not initialize the file when accessed
    PRELOADENTRYINITKIND_REQINIT,    // request the file be initialized upon being accessed
    PRELOADENTRYINITKIND_INITALIZED, // file has been initialized
} PreloadEntryInitKind;

typedef enum PreloadHeapKind // 0x2 of PreloadEntry
{
    PRELOADHEAPKIND_MAINDRAM,   // main DRAM heap
    PRELOADHEAPKIND_1,          //
    PRELOADHEAPKIND_CARDICON,   // memcard banner and icon files
    PRELOADHEAPKIND_GAMECOMMON, // IfAll, ItCommon, RdCommon, etc
    PRELOADHEAPKIND_ACTORS,     // game files like rider dat, animations, stage. also stage and special allocations (entrynum 2000+)
    PRELOADHEAPKIND_5,          //
    PRELOADHEAPKIND_ARAM,       // misc files in ARAM, transferred over DMA when needed. contents vary based on scene
} PreloadHeapKind;

typedef enum PreloadFileKind // 0x1 of PreloadEntry
{
    // these have not been updated for KAR
    PRELOADFILEKIND_ALLOC,   // special alloc, code @ 80017c7c will skip loading a file
    PRELOADFILEKIND_1,       // these will always result in an assertion?? 800180dc
    PRELOADFILEKIND_DEFAULT, // will only init the archive
    PRELOADFILEKIND_EFFECT,  // will init particle bank and get a symbol from it @ 80018110
    PRELOADFILEKIND_STAGE,   // will init get stage symbol and init its ptcl bank @ 80018130
    PRELOADFILEKIND_NUM,     // m-ex uses this as a custom file_kind. its main purpose to not be marked as unneeded every time the preload table is updated (cant preload arbitrary files in vanilla melee)
} PreloadFileKind;

struct PreloadHeapDesc //
{
    PreloadHeapKind kind;
    int location;
    int pri;
    int size;
};

typedef struct PreloadAlloc
{
    struct PreloadAlloc *next; // Offset 0x00
    void *start;               // Offset 0x04
    u32 size;                  // Offset 0x08
} PreloadAlloc;

struct PreloadHandle
{
    int x0;
    void *heap_start;
    void *heap_end;
    PreloadAlloc *alloc;
};

struct PreloadHeap //
{
    int heap_index;        // 0x0, is a heap ID returned by OSCreateHeap, is = -1 if the heap doesnt use OSCreateHeap
    PreloadHandle *handle; // 0x4, name evidenced by assert @ 80014f10
    void *addr_start;      // 0x8
    int size;              // 0xC
    int pri;               // 0x10. 0 will allocate using OSCreateHeap, 1 will create heap using 80014e24. 2 = use main heap AND handles?? has something to do with how the heap is built. is referencedwhen freeing a preloaded file. if this is 1 it will properly free up the preload entry
    int req_disable;       // 0x14, temp variable used in Preload_ResetHeaps
    int is_disabled;       // 0x18,
};

struct PreloadHeapLookup //
{
    // these dont seem to change for the entirety of runtime
    // i guess they're referenced when rebuilding heaps between scenes
    void *dram_addr_start; // 0x0
    void *dram_addr_end;   // 0x4
    void *aram_addr_start; // 0x8
    void *aram_addr_end;   // 0xC

    PreloadHeap heap_arr[10]; // 0x10, indexed by PreloadHeapKind
};

struct PreloadChar
{
    int kind;
    u8 costume;
};

struct PreloadAllocData
{
    PreloadAllocData *next;
    void *alloc; // 0x4
    int size;    // 0x8
};

typedef enum PreloadFileGroup
{
    PRELOADFILEGROUP_0,
    PRELOADFILEGROUP_NUM,
} PreloadFileGroup;

struct PreloadEntryDesc //
{
    PreloadFileKind file_kind : 8; // 0x0 PreloadFileKind
    char *file_name;               // 0x4, file name
    int x8;                        // 0x8, same as last arg in Preload_CreateEntryByEntrynum
};

struct PreloadEntry // scene changes wait for asyncronous disc reads to finish if they are logged as a PreloadEntry!
{
    PreloadEntryLoadState load_state : 8; // 0x0, 0 = none, 1 = queued, 2 = loading, 3 = loaded, 4 = persistently loaded (IfAll, etc)
    PreloadFileKind file_kind : 8;        // 0x1, PreloadFileKind, alters the initialization behavior when the file is accessed with Preload_GetArchive/Preload_GetArchiveByEntrynum.its the value @ 0x0 in the tables relocated at m-ex injection 800186a8. 0 is used by special allocs (wont actually load anything, will auto mark as loaded), 2 is used by fighter files and ifall/itco/lbrb. 3 is used by efcodata and efmndata. 4 is used by stage files
    PreloadHeapKind heap_kind : 8;        // 0x2, PreloadHeapKind
    PreloadEntryInitKind init_kind : 8;   // 0x3, 0, do not init archive, 1 = init archive when accessed with Preload_GetArchive/Preload_GetArchiveByEntrynum. 0 = fighter anims/special allocs (dont initialize), 1 = fighter files/itco/ifall/efco/efmn (queued for initialization), 2 = lbrb/ifall/stage (initialized)
    u8 flags;                             // 0x4, indicates the purpose of the file. referenced when waiting for files using Preload_CheckIfFilesAreLoading. costume files are 0x1, plX files are 0x2, effect files are 0x4, anim files are 0x8, stage files and allocs are 0x10, IfAll/EfMnData/EfCoData/ItCo are 0x40, LbRb is 0x80
    u16 file_entry_num;                   // 0x6, if this is greater than 2000 its a special allocation (like fountain reflection (2001) or pokemon stadium transformation (200?))
    s16 status;                           // 0x8, is -9999 when marked for unload, 9999 when needed
    int file_size;                        // 0xc
    PreloadAllocData *file_data;          // 0x10, this gets allocated @ 80017c00
    PreloadAllocData *header_data;        // 0x14
    int x18;                              // 0x18
    int x1c;                              // 0x1c
    int x20;                              // 0x20
    int x24;                              // 0x24
};

struct PreloadTable
{
    int x0;                  // 0x0
    int is_cache_game_files; // 0x4
    int is_cache_A2_files;   // 0x8, not sure what these are for, not regular dat files...
    int is_cache_menus;      // 0xc, checked @ 80074908 and will cache game menus flagged in the stc_preload_menu_files array
    int x10;                 // 0x10
    int update_num;          // 0x14, used to know how up to date the cache is. incremented when Preload::heap_kind != MinorSceneDesc::heap_kind OR when calling Preload_Invalidate
    GroundKind gr_kind;      // 0x18, checked @ 800748ec
    int x1c;                 // 0x1c, checked @ 800748e0
    int x20;                 // 0x20, -1 to disable entirely? 800748d8
    int is_cache_alloc1;     // 0x24, alloc id 2001, not sure what uses this
    int is_cache_alloc2;     // 0x28, alloc id 2002, not sure what uses this
    u8 x30[0x80 - 0x2c];     //
};

struct Preload
{
    int kind;               // 0x0, copied from current MinorScene struct, affects the behavior @ 80074128
    PreloadTable queued;    // 0x4, modify this to request files to preload
    PreloadTable updated;   // 0x84
    PreloadEntry entry[80]; // 0x104
};

static Preload *stc_preload_table = (Preload *)0x80550f68;
static int *stc_preload_menu_files = (int *)0x80558818;
static PreloadEntryDesc *stc_preload_entry_descs = (PreloadEntryDesc *)0x80489648; // 82 of these
static PreloadHeapLookup *stc_preload_heaps_lookup = (PreloadHeapLookup *)0x80537f40;
static PreloadHeapDesc *stc_preload_heap_descs = (PreloadHeapDesc *)0x80497ed0; // kind 10 is the terminator

/*** Functions ***/
void Preload_Update();                                                                                                                                                       // checks for new files to preload and begins loading them synchronously
void Preload_Invalidate();                                                                                                                                                   // invalidates cache, this is usually used from scene prep on CSS's
Preload *Preload_GetTable();                                                                                                                                                 // gets preload table
PreloadEntry *Preload_CreateEntry(PreloadFileKind file_kind, char *filename, int r5, PreloadHeapKind heap_kind, int file_size, int is_init_archive, int r9, int flags);      // creates a preload entry for any given file/ (file_kind is 0 for stuff like fod reflection, r5 = 4, r9 = 7) 80017740
PreloadEntry *Preload_CreateEntryByEntrynum(PreloadFileKind file_kind, int entrynum, int r5, int heap_kind, int file_size, int is_init_archive, int r9, int flags, int r11); // creates a preload entry for any given file/ (file_kind is 0 for stuff like fod reflection, r5 = 4, r9 = 7) 80017740
void Preload_RequestLoadEntry(int preload_entry_index);                                                                                                                      //
void Preload_FreeEntry(int preload_entry_index);                                                                                                                             // 800174e8
HSD_Archive *Preload_GetArchive_Init_ReturnSymbols(int unk, char *filename, void *symbol_return, ...);                                                                       // variadic args are symbol strings, terminate the variadic args with a 0
HSD_Archive *Preload_GetArchiveByEntrynum(int entrynum);                                                                                                                     // will wait if the file isnt loaded yet
int Preload_CheckFileStatus(int entrynum);                                                                                                                                   // 0 = not loading, 1 = loading, 2 = loaded
int Preload_CheckIfFilesAreLoading(int preload_flags);                                                                                                                       // use PRELOADFLAG_ definitions
void Preload_FreeUnneededInitializedArchivesInHeap(PreloadHeapKind heap_kind);                                                                                               // arg is heap_kind in PreloadEntry
void Preload_Wait(int preload_flags);
void *Preload_AllocFromHeap(PreloadHeapKind heap_kind, int size);
void Preload_FreeToHeap(void *ptr, int size);
void Preload_SetGrKind(GroundKind);

int KARPlus_AddPreloadMenuFile(char *file_name);
int KARPlus_AddPreloadGameFile(char *file_name);
#endif
