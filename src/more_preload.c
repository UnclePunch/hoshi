#include "text.h"
#include "os.h"
#include "hsd.h"
#include "preload.h"
#include "scene.h"
#include "inline.h"
#include "audio.h"
#include "game.h"
#include <string.h>

#include "more_preload.h"
#include "code_patch/code_patch.h"
#include "fst/fst.h"

static PreloadCustomGameFiles stc_custom_game_files = {0};
static char *stc_custom_menu_files[8] = {0};

void Preload_Init()
{
    Preload_ApplyPatches();
}

// installed to the game under the name KARPlus_X
// adds an arbitrary file to the preload table and is requested every scene
// change until its zeroed by the game
int Preload_AddCustomMenuFile(char *file_name)
{
    for (int i = 0; i < GetElementsIn(stc_custom_menu_files); i++)
    {
        if (stc_custom_menu_files[i] == 0)
        {
            stc_custom_menu_files[i] = file_name;
            OSReport("Preload: added menu file %s\n", file_name);
            return 1;
        }
        else if (strcmp(stc_custom_menu_files[i], file_name) == 0)
        {
            OSReport("Preload: menu file %s already exists\n", file_name);
            return 0;
        }
    }

    return 0;
}
int Preload_AddCustomGameFile(char *file_name)
{
    // exit if at max
    if (stc_custom_game_files.num > GetElementsIn(stc_custom_game_files.file))
    {
        OSReport("Preload: game file num over %d\n", GetElementsIn(stc_custom_game_files.file));
        return 0;
    }

    // ensure it isnt already present
    for (int i = 0; i < stc_custom_game_files.num; i++)
    {
        if (strcmp(stc_custom_game_files.file[i].name, file_name) == 0)
        {
            OSReport("Preload: game file %s already exists\n", file_name);
            return 0;
        }
    }

    // add
    stc_custom_game_files.file[stc_custom_game_files.num].name = file_name;
    stc_custom_game_files.file[stc_custom_game_files.num].kind = 2;
    stc_custom_game_files.file[stc_custom_game_files.num].preload_kind = 4;
    stc_custom_game_files.num++;

    OSReport("Preload: added game file %s\n", file_name);
    return 1;

    return 0;
}

// Adjusts the size of PRELOADHEAPKIND_GAMECOMMON
void Preload_IncreasePersistentHeapSize()
{
    PreloadHeapDesc *heap_descs = stc_preload_heap_descs;
    while (heap_descs->kind != 10)
    {
        if (heap_descs->kind == PRELOADHEAPKIND_GAMECOMMON)
        {
            int newfiles_total_size = 0;

            for (int i = 0; i < stc_custom_game_files.num; i++)
                newfiles_total_size += File_GetSize(stc_custom_game_files.file[i].name);

            newfiles_total_size = OSRoundUp32B(newfiles_total_size);

            OSReport("[hoshi] Persistent heap size: %.2fkb -> %.2fkb\n",
                     BytesToKB(heap_descs->size),
                     BytesToKB((heap_descs->size + newfiles_total_size)));

            heap_descs->size += newfiles_total_size;

            break;
        }

        heap_descs++;
    }
}

// runs during Preload_Update
void Preload_LoadCustomMenuFile()
{
    for (int i = 0; i < GetElementsIn(stc_custom_menu_files); i++)
    {
        if (stc_custom_menu_files[i])
        {
            Preload_CreateEntry(5,
                                stc_custom_menu_files[i],
                                6,
                                6,
                                0,
                                1,
                                100,
                                32);
        }
    }

    return;
}
CODEPATCH_HOOKCREATE(0x801317ec, "", Preload_LoadCustomMenuFile, "", 0)
void Preload_LoadCustomGameFile()
{
    Preload *p = stc_preload_table;

    // loop through all custom game files
    for (int i = 0; i < stc_custom_game_files.num; i++)
    {
        // if entry exists and it matches our current preload kind
        if (p->updated.is_cache_game_files) // game has raised flag to begin preloading common game files
        {
            Preload_CreateEntry(stc_custom_game_files.file[i].kind,
                                stc_custom_game_files.file[i].name,
                                PRELOADHEAPKIND_GAMECOMMON, // 4
                                PRELOADHEAPKIND_GAMECOMMON, // 4
                                0,
                                1,
                                2,     // 5
                                0x40); // 0x20
        }
    }

    return;
}
CODEPATCH_HOOKCREATE(0x80074e0c, "", Preload_LoadCustomGameFile, "", 0)

// runs in a few places: entering top level of main menu, title screen, etc
void Preload_ClearMenuEntries()
{
    // clear vanilla menu entries
    for (int i = 0; i < 23; i++)
    {
        if (stc_preload_menu_files[i])
        {
            OSReport("Preload: removing %s\n", FST_GetFilenameFromEntrynum(stc_preload_menu_files[i]));
            stc_preload_menu_files[i] = 0;
        }
    }

    // clear custom menu entries
    for (int i = 0; i < GetElementsIn(stc_custom_menu_files); i++)
    {
        if (stc_custom_menu_files[i])
        {
            OSReport("Preload: removing %s\n", stc_custom_menu_files[i]);
            stc_custom_menu_files[i] = 0;
        }
    }
}

void Preload_ApplyPatches()
{
    CODEPATCH_HOOKAPPLY(0x801317ec);                             // add hook to load custom preload entries
    CODEPATCH_HOOKAPPLY(0x80074e0c);                             // add load custom game file
    CODEPATCH_REPLACEFUNC(0x8013128c, Preload_ClearMenuEntries); // add hook to clear custom preload entries

    // install functions to add new preload entries
    CODEPATCH_REPLACEFUNC(KARPlus_AddPreloadMenuFile, Preload_AddCustomMenuFile);
    CODEPATCH_REPLACEFUNC(KARPlus_AddPreloadGameFile, Preload_AddCustomGameFile);

    return;
}
