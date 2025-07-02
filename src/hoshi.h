#ifndef MODLOADER_H
#define MODLOADER_H

#include "reloc/reloc.h"           // public header
#include "hoshi/settings.h"        // public header
#include "code_patch/code_patch.h" //

#define MODLOADER_VERSION_MAJOR 0
#define MODLOADER_VERSION_MINOR 5

typedef struct MenuDesc MenuDesc;
typedef struct MenuSave MenuSave;

typedef struct gbFunction
{
    char *name;
    char *author;
    char *version;
    int *save_size;
    MenuDesc *menu_desc;
    void (*OnBoot)(HSD_Archive *archive);
    void (*OnSceneChange)();
    void (*OnSaveInit)(void *save_ptr, int req_init);
    void (*On3DLoad)();
    void (*On3DPause)(int pause_ply);
    void (*On3DUnpause)(int pause_ply);
    void (*On3DExit)();
    void (*OnMainMenuLoad)();
    void (*OnPlayerSelectLoad)();
} gbFunction;

typedef struct GlobalMod
{
    gbFunction data;
    ModHeader *mex_function;
    void *archive;
    int entrynum;
    struct
    {
        int menu_size;
        MenuSave *menu_data;
        int user_size;
        void *user_data;
    } save;
} GlobalMod;

typedef struct ModloaderData
{
    struct
    {
        HSD_Archive *archive;
        ModHeader *mex_function;
    } hoshi;
    int mod_num;
    GlobalMod *mods;
} ModloaderData;

extern ModloaderData *stc_modloader_data;

void OnFileLoad(HSD_Archive *archive);
void *MemAllocPersistent(int size);
void OnSceneChange();
void OnSceneChange_Caller();
void Mods_CountFile(int entrynum, int *num);
void Mods_LoadFile_Callback(int r3, int *is_loaded);
void *Mods_LoadFile(int entrynum);
void Mods_LoadGlobal(int entrynum);
void Mods_LoadVS(int entrynum);
void Modloader_InitSaveData();
GlobalMod *Mods_GetFromName(char *name);
GlobalMod *Mods_GetFromMenuDesc(MenuDesc *desc);
int Mod_InitSaveData(GlobalMod *mod);
int _hash_32(const void *data, int size);

#endif