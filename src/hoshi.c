#include "text.h"
#include "os.h"
#include "hsd.h"
#include "preload.h"
#include "game.h"
#include "scene.h"
#include "memcard.h"
#include "inline.h"

#include <stdarg.h>
#include <string.h>

#include "hoshi.h"

#include "more_scenes.h"
#include "save.h"
#include "settings.h"
#include "more_preload.h"
#include "stack.h"
#include "hash.h"
#include "patch_misc.h"
#include "reloc/reloc.h"

// Hoshi
#include "export.h"
#include "screen_cam.h"
#include "wide.h"
#include "hoshi/func.h"
#include "hoshi/log.h"

// Lib
#include "code_patch/code_patch.h"
#include "fst/fst.h"

ModloaderData *stc_modloader_data;
GlobalMod *cur_mod_install = 0;

/////////////////////////////
//                         //
//       DOL Hooks         //
//                         //
/////////////////////////////

// runs every scene change, heap exists
void Hook_SceneChange()
{
    ScreenCam_Create();

    // loop through installed mods, run their scene change function
    for (int i = 0; i < stc_modloader_data->mod_num; i++)
    {
        GlobalMod *this_mod = &stc_modloader_data->mods[i];

        if (this_mod->desc->OnSceneChange)
            this_mod->desc->OnSceneChange();
    }
    
#ifdef LOG_LEVEL
    if (LOG_LEVEL > LOG_LEVEL_INFO)
    {
        // watermark
        // int canvas_idx = Text_CreateCanvas(1, 0, 0, 0, 0, 63, 0, 63);
        Text *t = Text_CreateText(1, ScreenCam_GetCanvasIdx());
        t->kerning = 1;
        t->use_aspect = 1;
        t->viewport_scale = (Vec2){0.5, 0.5};
        t->aspect = (Vec2){420, 32};
        t->viewport_color = (GXColor){0, 0, 0, 128};
        Text_AddSubtext(t, 0, 0, "KARDX Test Build " __DATE__);
    }
#endif


    return;
};
CODEPATCH_HOOKCREATE(0x8000678c, "", Hook_SceneChange, "", 0)

// runs upon entering main menu
void Hook_MainMenuLoad()
{
    // loop through installed mods, run their function
    for (int i = 0; i < stc_modloader_data->mod_num; i++)
    {
        GlobalMod *this_mod = &stc_modloader_data->mods[i];

        if (this_mod->desc->OnMainMenuLoad)
            this_mod->desc->OnMainMenuLoad();
    }

    return;
};
CODEPATCH_HOOKCREATE(0x80018994, "", Hook_MainMenuLoad, "", 0)

// runs before instantiating the 3d scene
void Hook_3DLoadStart()
{
    // loop through installed mods, run their function
    for (int i = 0; i < stc_modloader_data->mod_num; i++)
    {
        GlobalMod *this_mod = &stc_modloader_data->mods[i];

        if (this_mod->desc->On3DLoadStart)
            this_mod->desc->On3DLoadStart();
    }

    return;
};
CODEPATCH_HOOKCREATE(0x80014448, "", Hook_3DLoadStart, "", 0)

// runs after instantiating the 3d scene
void Hook_3DLoadEnd()
{
    // loop through installed mods, run their function
    for (int i = 0; i < stc_modloader_data->mod_num; i++)
    {
        GlobalMod *this_mod = &stc_modloader_data->mods[i];

        if (this_mod->desc->On3DLoadEnd)
            this_mod->desc->On3DLoadEnd();
    }

    return;
};
CODEPATCH_HOOKCREATE(0x80014d3c, "", Hook_3DLoadEnd, "", 0)

// runs upon pausing 3d scene
void Hook_3DPause(int pause_ply)
{
    // loop through installed mods, run their function
    for (int i = 0; i < stc_modloader_data->mod_num; i++)
    {
        GlobalMod *this_mod = &stc_modloader_data->mods[i];

        if (this_mod->desc->On3DPause)
            this_mod->desc->On3DPause(pause_ply);
    }

    return;
};
CODEPATCH_HOOKCREATE(0x80041160, "lbz 3,0x0830 (29)\n\t", Hook_3DPause, "", 0)

// runs upon unpausing 3d scene
void Hook_3DUnPause(int pause_ply)
{
    // loop through installed mods, run their function
    for (int i = 0; i < stc_modloader_data->mod_num; i++)
    {
        GlobalMod *this_mod = &stc_modloader_data->mods[i];

        if (this_mod->desc->On3DUnpause)
            this_mod->desc->On3DUnpause(pause_ply);
    }

    return;
};
CODEPATCH_HOOKCREATE(0x80113a30, "lis 3, 0x8053\n\t"
                                 "lbz 3, 0x6208 (3)\n\t",
                     Hook_3DUnPause, "", 0)

// runs upon exiting 3d scene
void Hook_3DExit()
{
    // loop through installed mods, run their function
    for (int i = 0; i < stc_modloader_data->mod_num; i++)
    {
        GlobalMod *this_mod = &stc_modloader_data->mods[i];

        if (this_mod->desc->On3DExit)
            this_mod->desc->On3DExit();
    }

    return;
};
CODEPATCH_HOOKCREATE(0x80015274, "", Hook_3DExit, "", 0)

// runs upon entering player select
void Hook_PlayerSelectLoad()
{
    // loop through installed mods, run their function
    for (int i = 0; i < stc_modloader_data->mod_num; i++)
    {
        GlobalMod *this_mod = &stc_modloader_data->mods[i];

        if (this_mod->desc->OnPlayerSelectLoad)
            this_mod->desc->OnPlayerSelectLoad();
    }

    return;
};
CODEPATCH_HOOKCREATE(0x8003b48c, "", Hook_PlayerSelectLoad, "", 0)

// runs every tick
void Hook_OnFrameStart()
{
    // loop through installed mods, run their function
    for (int i = 0; i < stc_modloader_data->mod_num; i++)
    {
        GlobalMod *this_mod = &stc_modloader_data->mods[i];

        if (this_mod->desc->OnFrameStart)
            this_mod->desc->OnFrameStart();
    }

    return;
};
CODEPATCH_HOOKCREATE(0x80006844, "", Hook_OnFrameStart, "", 0)

// runs every tick
void Hook_OnFrameEnd()
{
    // loop through installed mods, run their function
    for (int i = 0; i < stc_modloader_data->mod_num; i++)
    {
        GlobalMod *this_mod = &stc_modloader_data->mods[i];

        if (this_mod->desc->OnFrameEnd)
            this_mod->desc->OnFrameEnd();
    }

    return;
};
CODEPATCH_HOOKCREATE(0x80006a60, "", Hook_OnFrameEnd, "", 0)

////////////////////////////////////////////
//                                        //
//       Main program entrypoint.         //
//                                        //
////////////////////////////////////////////

void OnFileLoad(ModHeader *file)
{
    void *hoshi_data_start = *(void **)0x805de290;

    // replace hsd_memalloc for the duration of this callback
    // this allows us to make persistent allocations by simply calling hsd_memalloc
    int alloc_instr = *(int *)(HSD_MemAlloc);
    CODEPATCH_REPLACEFUNC(HSD_MemAlloc, MemAllocPersistent);
    int loadarchive_intr = *(int *)(Archive_LoadFile);
    CODEPATCH_REPLACEFUNC(Archive_LoadFile, Archive_LoadFilePersistent);

    // if (LOG_LEVEL >= LOG_LEVEL_DEBUG)
    //     (*stc_dblevel) = DB_DEVELOP;

    Patches_Apply();    // apply code patches
    Export_Init();      // init mod export/import 
    Scenes_Init();      // init scene expansion
    Preload_Init();     // init preload expansion
    Stack_Init();       // init stack print patch
    ScreenCam_Init();   // init screen camera
    KARPlusSave_Init(); // Init save data
    Wide_Init();        // init widescreen adjustments

    CODEPATCH_REPLACEFUNC(hash_32, _hash_32);         // install our hash function
    CODEPATCH_REPLACEFUNC(hash_32_str, _hash_32_str); // install our string hash function
    
    // alloc hoshi lookup data
    stc_modloader_data = HSD_MemAlloc(sizeof(*stc_modloader_data));

    stc_modloader_data->hoshi.mod_header = file;

    // count number of mod files on disc
    int mod_num = 0;
    FST_ForEachInFolder("/mods", ".bin", 0, (void (*)(int, void *))Mods_CountFile, &mod_num);
    LOG_DEBUG("found %d mods", mod_num);

    // if mods were found, load them
    if (mod_num > 0)
    {
        // allocate data for them
        stc_modloader_data->mods = HSD_MemAlloc(sizeof(*stc_modloader_data->mods) * mod_num);

        int tick = OSGetTick();

        // load each mod file and install them
        FST_ForEachInFolder("/mods", ".bin", 0, (void (*)(int, void *))Mods_LoadGlobal, 0);
        OSReport("\n");

        // count size of all mods
        int mods_size = 0;
        for (int i = 0; i < stc_modloader_data->mod_num; i++)
            mods_size += stc_modloader_data->mods[i].mod_header->relocs_offset;

        LOG_INFO("Installed %d mods (%.2fkb) in %.2fms", mod_num, BytesToKB(mods_size), MillisecondsSinceTick(tick));

        Preload_IncreasePersistentHeapSize(); // increase size of persistent heap to fit any new files
        OSReport("\n");
    }
    else
        stc_modloader_data->mod_num = 0;

    // insert callback hooks
    CODEPATCH_HOOKAPPLY(0x8000678c);
    CODEPATCH_HOOKAPPLY(0x80018994);
    CODEPATCH_HOOKAPPLY(0x80014448);
    CODEPATCH_HOOKAPPLY(0x80014d3c);
    CODEPATCH_HOOKAPPLY(0x8003b48c);
    CODEPATCH_HOOKAPPLY(0x80041160);
    CODEPATCH_HOOKAPPLY(0x80113a30);
    CODEPATCH_HOOKAPPLY(0x80015274);
    CODEPATCH_HOOKAPPLY(0x80006844);
    CODEPATCH_HOOKAPPLY(0x80006a60);
    
    Settings_Init(stc_modloader_data);
    MainMenu_ApplyPatches();

    // restore hsd_memalloc
    CODEPATCH_REPLACEINSTRUCTION(HSD_MemAlloc, alloc_instr);
    CODEPATCH_REPLACEINSTRUCTION(Archive_LoadFile, loadarchive_intr);

    // log out memory used by hoshi
    void *hoshi_data_end = (*(void **)0x805de290);
    int hoshi_data_size = ((int)hoshi_data_end - (int)hoshi_data_start) + OSRoundUp32B(file->relocs_offset);
    int remaining_size = *(int *)0x805de294 - *(int *)0x805de290;
    LOG_INFO("hoshi + mods using %.2fkb.", BytesToKB(hoshi_data_size));
    LOG_INFO("Remaining memory: %.2fkb.", BytesToKB(remaining_size));

    return;
}

void *MemAllocPersistent(int size)
{

    u8 **stc_heap_next = (u8 **)0x805de290;
    void *alloc = (void *)(*stc_heap_next); // next available address in "heap"

    // "alloc" data
    (*stc_heap_next) = (u8 *)((int)(*stc_heap_next) + OSRoundUp32B(size));

    // zero the data
    memset(alloc, 0, size);

    return alloc;
}
void MemFreePersistent(int size)
{
    u8 **stc_heap_next = (u8 **)0x805de290;
    void *alloc = (void *)(*stc_heap_next); // next available address in "heap"

    // "free" data
    (*stc_heap_next) = (u8 *)((int)(*stc_heap_next) - OSRoundUp32B(size));

    return;
}

volatile int archive_is_loaded = 0;
void Archive_LoadFilePersistentCallback(int unk, void *arg2)
{
    archive_is_loaded = 1;
}
HSD_Archive *Archive_LoadFilePersistent(char *filename)
{
    // check if it exists
    int entrynum = DVDConvertPathToEntrynum(filename);
    if (entrynum == -1)
       return 0;

    archive_is_loaded = 0;

    int size = File_GetSize(filename);
    void *buffer = HSD_MemAlloc(size);
    File_Read(entrynum, 0, buffer, OSRoundUp32B(size), 0x21, 1, Archive_LoadFilePersistentCallback, 0);

    while (!archive_is_loaded)
        ;

    HSD_Archive *archive = HSD_MemAlloc(sizeof(HSD_Archive));
    Archive_Init(archive, buffer, size);

    return archive;
}

void File_LoadCallback(int r3, void *arg2)
{
    int *is_loaded = (int *)arg2;
    *is_loaded = 1;
    return;
}
int File_LoadOffsetSync(int entrynum, void *file_buffer, int file_offset, int file_size)
{
    // load file
    volatile int is_loaded = 0;
    File_Read(entrynum, file_offset, file_buffer, OSRoundUp32B(file_size), 0x21, 1, File_LoadCallback, (void *)&is_loaded);

    // wait to load
    while (!is_loaded)
        ;

    return 1;
}

void Mods_CountFile(int entrynum, int *num)
{
    (*num)++;
    return;
}

void *Mods_LoadFile(int entrynum)
{

    DVDFileInfo file_info;
    DVDFastOpen(entrynum, &file_info);
    int file_size = file_info.length;
    DVDClose(&file_info);

    // load mod
    ModHeader *file_buffer = HSD_MemAlloc(OSRoundUp32B(file_size));
    File_LoadOffsetSync(entrynum, file_buffer, 0, file_size);

    // ensure its a supported mod version
    if (file_buffer->version != HOSHI_VERSION) {

        OSClearReports();

        OSReport("Mod update required!\n%s targets hoshi v%d.\nCurrently installed: hoshi v%d.\n", 
                    FST_GetFilenameFromEntrynum(entrynum),
                    file_buffer->version, 
                    HOSHI_VERSION);
        assert("0");
    }

    // reloc and overload
    reloc(file_buffer);

    // free relocs
    int relocs_size = file_buffer->relocs_num * sizeof(Reloc);
    MemFreePersistent(relocs_size);
    LOG_INFO("Freeing relocs (%.2f kb)", BytesToKB(relocs_size));

    LOG_INFO("Loaded mod file: %s (0x%08x)", FST_GetFilenameFromEntrynum(entrynum), file_buffer);

    return file_buffer;
}

void Mods_LoadGlobal(int entrynum)
{
    // get this mod
    GlobalMod *this_mod = &stc_modloader_data->mods[stc_modloader_data->mod_num];

    this_mod->entrynum = entrynum;

    // load mod file
    ModHeader *file = Mods_LoadFile(entrynum); //

    // get mod's descriptor
    ModDesc *mod_desc;
    get_func(file, (void **)&mod_desc);
    this_mod->desc = mod_desc;

    // save ptr to mod_header so we can access debug data
    this_mod->mod_header = file;

    LOG_INFO("~~~~~~~~~~~~~~~~~~~~~");
    LOG_INFO("   Installing global mod...");
    LOG_INFO("   Name:\t\t%s", this_mod->desc->name);
    LOG_INFO("   Author:\t\t%s", this_mod->desc->author);
    LOG_INFO("   Version:\t\t%d.%d", this_mod->desc->version.major, this_mod->desc->version.minor);

    OSReport("\n");

    // set global pointer to the mod being installed
    cur_mod_install = this_mod;

    // exec init function
    if (this_mod->desc->OnBoot)
        this_mod->desc->OnBoot();

    OSReport("\n");

    LOG_INFO("Finished installing %s.", this_mod->desc->name);
    LOG_INFO("~~~~~~~~~~~~~~~~~~~~~\n");

    // inc
    stc_modloader_data->mod_num++;
}

GlobalMod *Mods_GetFromName(char *name)
{
    // each mod
    for (int mod_idx = 0; mod_idx < stc_modloader_data->mod_num; mod_idx++)
    {
        if (strcmp(stc_modloader_data->mods[mod_idx].desc->name, name) == 0)
            return &stc_modloader_data->mods[mod_idx];
    }

    return 0;
}

void Mods_SetDefaultSaveData()
{
    LOG_DEBUG("Setting default save values.");

    // init each mod's settings
    for (int mod_idx = 0; mod_idx < stc_modloader_data->mod_num; mod_idx++)
    {
        GlobalMod *mod = &stc_modloader_data->mods[mod_idx];
        Mod_SetDefaultSaveData(mod);
    }
}
void Mod_SetDefaultSaveData(GlobalMod *mod)
{
    LOG_DEBUG("~~~~~~~~~~~~~~~~~~~~~");
    LOG_DEBUG("%s:", mod->desc->name);

    // get save data sizes as dictated by the currently installed mod version
    int menu_size = 0, user_size = 0;
    Option_GetSaveSize(mod->desc->option_desc, &menu_size);

    if ((mod->desc->save_size) && (mod->desc->save_size) > 0)
        user_size = mod->desc->save_size;

    // if mod has save data
    if (menu_size > 0 || user_size > 0)
    {
        // alloc save
        KARPlusSave_Alloc(mod, menu_size, user_size);

        if (mod->save.menu_data)
            LOG_DEBUG("menu_data: %x (0x%x)",
                      mod->save.menu_data,
                      mod->save.menu_num * sizeof(MenuSave));

        if (mod->save.user_data)
            LOG_DEBUG("user_data: %x (0x%x)",
                      mod->save.user_data,
                      mod->save.user_size);

        Mod_CopyToSave(mod); // copy default menu settings to the save

        // update the mod's pointer to its save file data
        mod->desc->save_ptr = mod->save.user_data;

        if (mod->desc->OnSaveInit)
        {
            LOG_DEBUG("Exec save default func...", mod->desc->name);
            mod->desc->OnSaveInit();
            OSReport("\n");
            LOG_DEBUG("Done.", mod->desc->name);
        }
    }
    else
    {
        LOG_DEBUG("No save desc.", mod->desc->name);
    }

    LOG_DEBUG("~~~~~~~~~~~~~~~~~~~~~\n");
}

void Mods_OnLoadSaveData()
{
    // init each mod's settings
    for (int mod_idx = 0; mod_idx < stc_modloader_data->mod_num; mod_idx++)
    {
        GlobalMod *mod = &stc_modloader_data->mods[mod_idx];
        Mod_OnLoadSaveData(mod);
    }

    Menu_ExecAllOptionChange();

    LOG_INFO("Audio Heap: %.2fkb / %.2fkb.", BytesToKB(OSCheckHeap(0)), BytesToKB((*__OSHeapTable)[0].size));

    OSClearReports();
}
int Mod_OnLoadSaveData(GlobalMod *mod)
{
    LOG_INFO("~~~~~~~~~~~~~~~~~~~~~");
    LOG_INFO("%s:", mod->desc->name);

    int req_init;

    // skip save file verification if no memcard is present, we can assume the defaults we created are valid.
    if (Memcard_GetSaveStatus() == CARDSAVE_IGNORE)
    {
        if (mod->desc->OnSaveLoaded)
        {
            LOG_INFO("Exec save loaded func...\n", mod->desc->name);
            mod->desc->OnSaveLoaded();
            OSReport("\n");
            LOG_INFO("Done.", mod->desc->name);
        }

        req_init = 0;
    }
    else
    {

        // get save data sizes as dictated by the currently installed mod version
        int menu_size = 0, user_size = 0;
        Option_GetSaveSize(mod->desc->option_desc, &menu_size);

        if (mod->desc->save_size > 0)
            user_size = mod->desc->save_size;

        // if mod has save data
        if (menu_size > 0 || user_size > 0)
        {
            int is_exists = KARPlusSave_CheckModDataExists(mod);
            void *mod_user_data;

            // check if save data exists on card
            if (is_exists)
            {
                LOG_INFO("Save data found.", mod->desc->name);

                // re-alloc if current version of the mod demands more storage
                req_init = KARPlusSave_VerifySize(mod, menu_size, user_size);
            }
            else
            {
                LOG_INFO("Save data created.", mod->desc->name);

                // alloc save
                KARPlusSave_Alloc(mod, menu_size, user_size);
                req_init = 1;
            }

            if (mod->save.menu_data)
                LOG_DEBUG("menu_data: %x (0x%x)",
                          mod->save.menu_data,
                          mod->save.menu_num * sizeof(MenuSave));

            if (mod->save.user_data)
                LOG_DEBUG("user_data: %x (0x%x)",
                          mod->save.user_data,
                          mod->save.user_size);

            if (!req_init)
                Mod_CopyFromSave(mod); // copy saved menu settings to the mod
            else
                Mod_CopyToSave(mod); // copy default menu settings to the save

            // update the mod's pointer to its save file data
            mod->desc->save_ptr = mod->save.user_data;
        }
        else
        {
            LOG_INFO("No save data.", mod->desc->name);
            req_init = 0;
        }

        if (mod->desc->OnSaveLoaded)
        {
            LOG_INFO("Exec save loaded func...\n", mod->desc->name);
            mod->desc->OnSaveLoaded();
            OSReport("\n");
            LOG_INFO("Done.", mod->desc->name);
        }
    }

    LOG_INFO("~~~~~~~~~~~~~~~~~~~~~");
    OSReport("\n");

    return req_init;
}

void Mod_CopyFromSave(GlobalMod *mod)
{
    if (mod->desc->option_desc)
    {
        OptionDesc *this_option = mod->desc->option_desc;

        switch (this_option->kind)
        {
        case (OPTKIND_VALUE):
        {
            Option_CopyFromSave(mod, "", this_option);
            break;
        }
        case (OPTKIND_MENU):
        {
            if (this_option->menu_ptr)
                Menu_CopyFromSave(mod, this_option->name, this_option->menu_ptr);

            break;
        }
        }
    }
}
void Mod_CopyToSave(GlobalMod *mod)
{
    if (mod->desc->option_desc)
    {
        OptionDesc *this_option = mod->desc->option_desc;
        switch (this_option->kind)
        {
        case (OPTKIND_VALUE):
        {
            Option_CopyToSave(mod, "", this_option);
            break;
        }
        case (OPTKIND_MENU):
        {
            if (this_option->menu_ptr)
                Menu_CopyToSave(mod, this_option->name, this_option->menu_ptr);

            break;
        }
        }
    }
}
void Mod_CopyAllToSave()
{
    // for each mod
    for (int mod_idx = 0; mod_idx < stc_modloader_data->mod_num; mod_idx++)
    {
        GlobalMod *mod = &stc_modloader_data->mods[mod_idx];
        Mod_CopyToSave(mod);
    }
}