#ifndef HOSHIMOD_H
#define HOSHIMOD_H

#include "settings.h"

typedef struct ModDesc
{
    char *name;
    char *author;
    struct 
    {
        u16 major;
        u16 minor;
    } version;
    void *save_ptr;                         // read-only! hoshi will write this pointer during installation.
    int save_size;
    int affects_gameplay;                   // bool indicating whether or not any aspects of this mod impact game state
    OptionDesc *option_desc;
    void (*OnBoot)();
    void (*OnSceneChange)();
    void (*OnSaveInit)();
    void (*OnSaveLoaded)();
    void (*On3DLoadStart)();                // executes before the game is initialized
    void (*On3DLoadEnd)();                  // executes after the game is initialized (riders, machines, stage, etc are all instantiated)
    void (*On3DPause)(int pause_ply);
    void (*On3DUnpause)(int pause_ply);
    void (*On3DExit)();
    void (*OnMainMenuLoad)();
    void (*OnPlayerSelectLoad)();
    void (*OnFrameStart)();
    void (*OnFrameEnd)();
} ModDesc;


#pragma pack(push, 1)
typedef struct ModSaveBackup
{
    char name[20];
    struct
    {
        u16 major;
        u16 minor;
    } version;
    int save_size;
    u8 save_data[];
} ModSaveBackup;
#pragma pack(pop)

void *Hoshi_ImportMod(char *id, int version_major, int version_minor);
void Hoshi_ExportMod(void *export);

// the following functions were mainly implemented for replay determinism.
// gameplay altering functionality that is gated with save data variables
// need to be saved and restored.
int Hoshi_GetBackupSize();                      // use this to determine the size of the array passed into Hoshi_BackupModSave
int Hoshi_BackupModSave(u8 *save);              // returns the number of mods whose save data was backed up
void Hoshi_RestoreModSave(u8 *save, int num);   // 
#endif