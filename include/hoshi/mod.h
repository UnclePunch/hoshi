#ifndef HOSHIMOD_H
#define HOSHIMOD_H

#include "settings.h"
#include "credits.h"

typedef struct ModDesc
{
    char *name;
    char *author;
    struct 
    {
        u16 major;                          // increment this if your save data struct changes! does not apply to settings menu changes as they are hashed by name.
        u16 minor;
    } version;
    void *save_ptr;                         // read-only! hoshi will write this pointer during installation.
    int save_size;
    int affects_gameplay;                   // bool indicating whether or not any aspects of this mod impact game state
    OptionDesc *option_desc;
    ModCredit **credits;                    // null terminated ModCredit pointer array
    void (*OnBoot)();                       // executes when the mod is loaded into memory. calling HSD_MemAlloc here will allocate persistent memory throughout the runtime of the program
    void (*OnSceneChange)();                // executes every scene change, after the heap is setup but before the scene is initialized
    void (*OnSaveInit)();                   // use this to initialize the default values for any save data your mod contains (can no longer allocate persistent memory).
    void (*OnSaveLoaded)();                 // executes after all mods have been loaded into memory (can no longer allocate persistent memory)
    void (*On3DLoadStart)();                // executes before the game is initialized
    void (*On3DLoadEnd)();                  // executes after the game is initialized (riders, machines, stage, etc are all instantiated)
    void (*On3DPauseStart)(int pause_ply);  // executes before pausing the game (before hud is created)
    void (*On3DPauseEnd)(int pause_ply);    // executes after pausing the game (after hud is created)
    void (*On3DUnpause)(int pause_ply);     // executes before unpausing the game
    void (*On3DExit)();                     // executes upon leaving the 3D scene (any)
    void (*OnMainMenuLoad)();               // executes after the main menu is initialized and created
    void (*OnPlayerSelectLoad)();           // executes after the air ride/city trial player select scene is loaded and created
    void (*OnFrameStart)();                 // executes at the top of a frame (all scenes)
    void (*OnFrameEnd)();                   // executes at the bottom of a frame (all scenes)
    void (*OnShowHUD)();                    // executes when the HUD is displayed
    void (*OnHideHUD)();                    // executes when the HUD is hidden
    void (*OnReset)();                      // executes when the game is reset via the physical reset button
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
    struct
    {
        int menu_num;
        int user_size;
        u8 data[];
    } save;
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