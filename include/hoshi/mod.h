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
    void (*OnFrame)();
} ModDesc;

void *Hoshi_ImportMod(char *id, int version_major, int version_minor);
void Hoshi_ExportMod(void *export);

#endif