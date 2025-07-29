#include "text.h"
#include "os.h"
#include "hsd.h"
#include "preload.h"
#include "scene.h"
#include "inline.h"
#include "audio.h"
#include "game.h"
#include <string.h>

#include "hoshi/log.h"

#include "more_preload.h"
#include "code_patch/code_patch.h"
#include "fst/fst.h"

static PreloadCustomGameFiles stc_custom_game_files = {0};
static char *stc_custom_menu_files[8] = {0};

void Preload_Init()
{
    Preload_ApplyPatches();
}

// installed to the game under the name Hoshi_X
// adds an arbitrary file to the preload table and is requested every scene
// change until its zeroed by the game
int Preload_AddCustomMenuFile(char *file_name)
{
    for (int i = 0; i < GetElementsIn(stc_custom_menu_files); i++)
    {
        if (stc_custom_menu_files[i] == 0)
        {
            stc_custom_menu_files[i] = file_name;
            LOG_INFO("Preload: added menu file %s", file_name);
            return 1;
        }
        else if (strcmp(stc_custom_menu_files[i], file_name) == 0)
        {
            LOG_WARN("Preload: menu file %s already exists", file_name);
            return 0;
        }
    }

    return 0;
}
int Preload_AddCustomGameFile(char *file_name, PreloadHeapKind heap_kind)
{
    // exit if at max
    if (stc_custom_game_files.num > GetElementsIn(stc_custom_game_files.file))
    {
        LOG_WARN("Preload: game file num over %d", GetElementsIn(stc_custom_game_files.file));
        return 0;
    }

    // ensure it isnt already present
    for (int i = 0; i < stc_custom_game_files.num; i++)
    {
        if (strcmp(stc_custom_game_files.file[i].name, file_name) == 0)
        {
            LOG_WARN("Preload: game file %s already exists in heap %d",
                     file_name,
                     stc_custom_game_files.file[i].heap_kind);
            return 0;
        }
    }

    // add
    stc_custom_game_files.file[stc_custom_game_files.num].name = file_name;
    stc_custom_game_files.file[stc_custom_game_files.num].file_kind = 2;
    stc_custom_game_files.file[stc_custom_game_files.num].preload_kind = 4;
    stc_custom_game_files.file[stc_custom_game_files.num].heap_kind = heap_kind;
    stc_custom_game_files.num++;

    LOG_INFO("Preload: added game file %s to heap %d", file_name, heap_kind);
    return 1;
}

// Adjusts the size of PRELOADHEAPKIND_GAMECOMMON
void Preload_IncreasePersistentHeapSize()
{
    PreloadHeapDesc *heap_descs = stc_preload_heap_descs;
    while (heap_descs->kind != 10)
    {
        if (heap_descs->kind == PRELOADHEAPKIND_STAY)
        {
            int newfiles_total_size = 0;

            for (int i = 0; i < stc_custom_game_files.num; i++)
            {
                if (stc_custom_game_files.file[i].heap_kind == PRELOADHEAPKIND_STAY)
                    newfiles_total_size += File_GetSize(stc_custom_game_files.file[i].name);
            }

            newfiles_total_size = OSRoundUp32B(newfiles_total_size);

            LOG_INFO("Persistent heap size: %.2fkb -> %.2fkb",
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
        int is_requested = 0;

        if (stc_custom_game_files.file[i].heap_kind == PRELOADHEAPKIND_STAY &&
            p->updated.is_cache_game_files) // game has raised flag to begin preloading common game files
        {
            is_requested = 1;
            Preload_CreateEntry(stc_custom_game_files.file[i].file_kind,
                                stc_custom_game_files.file[i].name,
                                PRELOADHEAPKIND_STAY, // 4
                                PRELOADHEAPKIND_STAY, // 4
                                0,
                                1,
                                2,     // 5
                                0x40); // 0x20
        }

        else if (stc_custom_game_files.file[i].heap_kind == PRELOADHEAPKIND_ALLM &&
                 p->updated.gr_kind != 59) // copied from 800748ec
        {
            is_requested = 1;
            Preload_CreateAllMEntry(stc_custom_game_files.file[i].name);
        }

        if (is_requested)
            LOG_DEBUG("Preloading %s", stc_custom_game_files.file[i].name);
    }

    return;
}
CODEPATCH_HOOKCREATE(0x80074e04, "", Preload_LoadCustomGameFile, "", 0)

// runs in a few places: entering top level of main menu, title screen, etc
void Preload_ClearMenuEntries()
{
    // clear vanilla menu entries
    for (int i = 0; i < 23; i++)
    {
        if (stc_preload_menu_files[i])
        {
            LOG_INFO("Preload: removing %s", FST_GetFilenameFromEntrynum(stc_preload_menu_files[i]));
            stc_preload_menu_files[i] = 0;
        }
    }

    // clear custom menu entries
    for (int i = 0; i < GetElementsIn(stc_custom_menu_files); i++)
    {
        if (stc_custom_menu_files[i])
        {
            LOG_INFO("Preload: removing %s", stc_custom_menu_files[i]);
            stc_custom_menu_files[i] = 0;
        }
    }
}

void Preload_ApplyPatches()
{
    CODEPATCH_HOOKAPPLY(0x801317ec);                             // add hook to load custom menu files
    CODEPATCH_HOOKAPPLY(0x80074e04);                             // add load custom game file
    CODEPATCH_REPLACEFUNC(0x8013128c, Preload_ClearMenuEntries); // add hook to clear custom preload entries

    // install functions to add new preload entries
    CODEPATCH_REPLACEFUNC(Hoshi_AddPreloadMenuFile, Preload_AddCustomMenuFile);
    CODEPATCH_REPLACEFUNC(Hoshi_AddPreloadGameFile, Preload_AddCustomGameFile);

    return;
}
