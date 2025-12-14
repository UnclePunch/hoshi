#ifndef HOSHI_PRIV_H
#define HOSHI_PRIV_H

#include "reloc/reloc.h"
#include "hoshi/mod.h"
#include "hoshi/settings.h"
#include "code_patch/code_patch.h" //

#define MODLOADER_VERSION_MAJOR 1
#define MODLOADER_VERSION_MINOR 3

/*
    hoshi Version History:
        - 1.0 - introductory
        - 1.1 - split the save callback into 2 separate callbacks
        - 1.2 - add onFrame callback
        - 1.3 - implement ModDesc
*/

#define MOD_FILE_VERSION 2

typedef struct MenuDesc MenuDesc;
typedef struct MenuSave MenuSave;

typedef struct GlobalMod
{
    ModDesc *desc;
    ModHeader *mod_header;
    int entrynum;
    struct
    {
        int menu_num;
        MenuSave *menu_data;
        int user_size;
        void *user_data;
    } save;
    void *export;               // pointer to mod's exported data
} GlobalMod;

typedef struct ModloaderData
{
    struct
    {
        ModHeader *mod_header;
    } hoshi;
    int mod_num;
    GlobalMod *mods;
} ModloaderData;

extern ModloaderData *stc_modloader_data;

void OnFileLoad(ModHeader *file);
void *MemAllocPersistent(int size);
void OnSceneChange();
void OnSceneChange_Caller();
void Mods_CountFile(int entrynum, int *num);
void Mods_LoadFile_Callback(int r3, int *is_loaded);
void *Mods_LoadFile(int entrynum);
void Mods_LoadGlobal(int entrynum);
void Mods_LoadVS(int entrynum);
GlobalMod *Mods_GetFromName(char *name);

void Mods_SetDefaultSaveData();
void Mod_SetDefaultSaveData(GlobalMod *mod);
void Mods_OnLoadSaveData();
int Mod_OnLoadSaveData(GlobalMod *mod);

int _hash_32(const void *data, int size);

void Mod_CopyFromSave(GlobalMod *mod);
void Mod_CopyToSave(GlobalMod *mod);
void Mod_CopyAllToSave();

#endif