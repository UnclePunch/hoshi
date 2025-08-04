#include "text.h"
#include "os.h"
#include "hsd.h"
#include "preload.h"
#include "game.h"
#include "scene.h"
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
#include "hoshi/log.h"

// Lib
#include "code_patch/code_patch.h"
#include "fst/fst.h"

ModloaderData *stc_modloader_data;

/////////////////////////////
//                         //
//       DOL Hooks         //
//                         //
/////////////////////////////

// runs every scene change, heap exists
void Hook_SceneChange()
{
    // loop through installed mods, run their scene change function
    for (int i = 0; i < stc_modloader_data->mod_num; i++)
    {
        GlobalMod *this_mod = &stc_modloader_data->mods[i];

        if (this_mod->data.OnSceneChange)
            this_mod->data.OnSceneChange();
    }

#ifdef LOG_LEVEL
    if (LOG_LEVEL > LOG_LEVEL_INFO)
    {
        // watermark
        int canvas_idx = Text_CreateCanvas(1, 0, 0, 0, 0, 63, 0, 63);
        Text *t = Text_CreateText(1, canvas_idx);
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

        if (this_mod->data.OnMainMenuLoad)
            this_mod->data.OnMainMenuLoad();
    }

    return;
};
CODEPATCH_HOOKCREATE(0x80018994, "", Hook_MainMenuLoad, "", 0)

// runs upon entering 3d scene
void Hook_3DLoad()
{
    // loop through installed mods, run their function
    for (int i = 0; i < stc_modloader_data->mod_num; i++)
    {
        GlobalMod *this_mod = &stc_modloader_data->mods[i];

        if (this_mod->data.On3DLoad)
            this_mod->data.On3DLoad();
    }

    return;
};
CODEPATCH_HOOKCREATE(0x80014d3c, "", Hook_3DLoad, "", 0)

// runs upon pausing 3d scene
void Hook_3DPause(int pause_ply)
{
    // loop through installed mods, run their function
    for (int i = 0; i < stc_modloader_data->mod_num; i++)
    {
        GlobalMod *this_mod = &stc_modloader_data->mods[i];

        if (this_mod->data.On3DPause)
            this_mod->data.On3DPause(pause_ply);
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

        if (this_mod->data.On3DUnpause)
            this_mod->data.On3DUnpause(pause_ply);
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

        if (this_mod->data.On3DExit)
            this_mod->data.On3DExit();
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

        if (this_mod->data.OnPlayerSelectLoad)
            this_mod->data.OnPlayerSelectLoad();
    }

    return;
};
CODEPATCH_HOOKCREATE(0x8003b48c, "", Hook_PlayerSelectLoad, "", 0)

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

    // if (LOG_LEVEL >= LOG_LEVEL_DEBUG)
    //     (*stc_dblevel) = DB_DEVELOP;

    Patches_Apply();    // apply code patches
    Scenes_Init();      // init scene expansion
    Preload_Init();     // init preload expansion
    Stack_Init();       // init stack print patch
    KARPlusSave_Init(); // Init save data

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
        {
            DVDFileInfo file_info;
            DVDFastOpen(stc_modloader_data->mods[i].entrynum, &file_info);
            mods_size += file_info.length;
            DVDClose(&file_info);
        }
        LOG_INFO("Installed %d mods (%.2fkb) in %.2fms", mod_num, BytesToKB(mods_size), MillisecondsSinceTick(tick));

        Preload_IncreasePersistentHeapSize(); // increase size of persistent heap to fit any new files
        OSReport("\n");
    }
    else
        stc_modloader_data->mod_num = 0;

    // insert callback hooks
    CODEPATCH_HOOKAPPLY(0x8000678c);
    CODEPATCH_HOOKAPPLY(0x80018994);
    CODEPATCH_HOOKAPPLY(0x80014d3c);
    CODEPATCH_HOOKAPPLY(0x8003b48c);
    CODEPATCH_HOOKAPPLY(0x80041160);
    CODEPATCH_HOOKAPPLY(0x80113a30);
    CODEPATCH_HOOKAPPLY(0x80015274);

    Settings_Init(stc_modloader_data);
    MainMenu_ApplyPatches();

    // restore hsd_memalloc
    CODEPATCH_REPLACEINSTRUCTION(HSD_MemAlloc, alloc_instr);

    // log out memory used by hoshi
    void *hoshi_data_end = (*(void **)0x805de290);
    int hoshi_data_size = ((int)hoshi_data_end - (int)hoshi_data_start) + OSRoundUp32B(File_GetSize("hoshi.bin"));
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

void Mods_CountFile(int entrynum, int *num)
{
    (*num)++;
    return;
}
void Mods_LoadFile_Callback(int r3, int *is_loaded)
{
    (*is_loaded) = 1;
    return;
}
void *Mods_LoadFile(int entrynum)
{

    // load this file
    DVDFileInfo file_info;
    DVDFastOpen(entrynum, &file_info);
    int file_size = file_info.length;
    DVDClose(&file_info);
    void *file_buffer = HSD_MemAlloc(OSRoundUp32B(file_size));

    // load file
    volatile int is_loaded = 0;
    File_Read(entrynum, 0, file_buffer, OSRoundUp32B(file_size), 0x21, 1, Mods_LoadFile_Callback, (void *)&is_loaded);

    // wait to load
    while (!is_loaded)
        ;

    LOG_INFO("Loaded mod file: %s (0x%08x)", FST_GetFilenameFromEntrynum(entrynum), file_buffer);

    return file_buffer;
}

void Mods_LoadGlobal(int entrynum)
{
    // get this mod
    GlobalMod *this_mod = &stc_modloader_data->mods[stc_modloader_data->mod_num];

    this_mod->entrynum = entrynum;

    // load file
    ModHeader *file = Mods_LoadFile(entrynum); //

    // reloc and overload
    reloc(file);
    get_func(file, (void **)&this_mod->data);

    // save ptr to mexFunction so we can access debug data
    this_mod->mod_header = file;

    LOG_INFO("~~~~~~~~~~~~~~~~~~~~~");
    LOG_INFO("   Installing global mod...");
    LOG_INFO("   Name:\t\t%s", this_mod->data.name);
    LOG_INFO("   Author:\t\t%s", this_mod->data.author);
    LOG_INFO("   Version:\t\t%s", this_mod->data.version);

    OSReport("\n");

    // exec init function
    if (this_mod->data.OnBoot)
        this_mod->data.OnBoot(file);

    OSReport("\n");

    LOG_INFO("Finished installing %s.", this_mod->data.name);
    LOG_INFO("~~~~~~~~~~~~~~~~~~~~~\n");

    // inc
    stc_modloader_data->mod_num++;
}

GlobalMod *Mods_GetFromName(char *name)
{
    // each mod
    for (int mod_idx = 0; mod_idx < stc_modloader_data->mod_num; mod_idx++)
    {
        if (strcmp(stc_modloader_data->mods[mod_idx].data.name, name) == 0)
            return &stc_modloader_data->mods[mod_idx];
    }

    return 0;
}

void Mods_InitSaveData()
{
    int req_write = 0;

    // // init default settings
    // Mod_InitSaveData(Menu_GetMod());

    // init each mod's settings
    for (int mod_idx = 0; mod_idx < stc_modloader_data->mod_num; mod_idx++)
    {
        GlobalMod *mod = &stc_modloader_data->mods[mod_idx];

        if (Mod_InitSaveData(mod))
            req_write = 1;
    }

    Menu_ExecAllOptionChange();

    // // if a save file was initialized, write it out to card asap
    // if (req_write)
    // {
    //     LOG_DEBUG("a save needed to be init'd, writing to memcard");
    //     KARPlusSave_Write();
    // }

    LOG_INFO("Audio Heap: %.2fkb / %.2fkb.", BytesToKB(OSCheckHeap(0)), BytesToKB((*__OSHeapTable)[0].size));

    // clear OSReports
    OSClearReports();
}
int Mod_InitSaveData(GlobalMod *mod)
{
    LOG_INFO("~~~~~~~~~~~~~~~~~~~~~");
    LOG_INFO("%s:", mod->data.name);

    int req_init;

    // get save data sizes as dictated by the currently installed mod version
    int menu_size = 0, user_size = 0;
    Option_GetSaveSize(mod->data.option_desc, &menu_size);

    if ((mod->data.save_size != 0) && (*mod->data.save_size) > 0)
        user_size = *mod->data.save_size;

    // if mod has save data
    if (menu_size > 0 || user_size > 0)
    {
        int is_exists = KARPlusSave_CheckModDataExists(mod);
        void *mod_user_data;

        // check if save data exists on card
        if (is_exists)
        {
            LOG_INFO("Save data found.", mod->data.name);

            // re-alloc if current version of the mod demands more storage
            req_init = KARPlusSave_VerifySize(mod, menu_size, user_size);
        }
        else
        {
            LOG_INFO("Save data created.", mod->data.name);

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

        if (mod->data.OnSaveInit)
        {
            LOG_INFO("Exec save init func...\n", mod->data.name);
            mod->data.OnSaveInit(mod->save.user_data, req_init);
            OSReport("\n");
            LOG_INFO("Done.", mod->data.name);
        }
    }
    else
    {
        LOG_INFO("No save data.", mod->data.name);
        req_init = 0;
    }

    LOG_INFO("~~~~~~~~~~~~~~~~~~~~~\n");

    return req_init;
}

void Mod_CopyFromSave(GlobalMod *mod)
{
    if (mod->data.option_desc)
    {
        OptionDesc *this_option = mod->data.option_desc;

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
    if (mod->data.option_desc)
    {
        OptionDesc *this_option = mod->data.option_desc;
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
