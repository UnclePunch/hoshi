/*---------------------------------------------------------------------------*
    Handles lowest level save file operations.
        - allocates runtime save struct
        - creates save file on memcard
        - loads save file from memcard
        - returns runtime save file struct
 *---------------------------------------------------------------------------*/

#include "memcard.h"
#include "os.h"
#include "hsd.h"
#include "inline.h"
#include "game.h"
#include <string.h>
#include <stddef.h>

#include "save.h"
#include "hash.h"
#include "hoshi.h"

#include "code_patch/code_patch.h"

static char *save_name = "KAR-Plus";
static KARPlusSave *stc_karplus_save;
static int stc_karplus_save_hash;

/*---------------------------------------------------------------------*
Name:           KARPlusSave_OnSaveCreateOrLoad

Description:    Inserted at the end of vanilla memcard initialization.
                Calls KARPlusSave_CreateOrLoad.

Arguments:      none.

Returns:        none.

*---------------------------------------------------------------------*/
void KARPlusSave_OnSaveCreateOrLoad()
{

    OSReport("---------------------\n");
    OSReport("[hoshi] Initializing save...\n");
    OSReport("\n");

    // check if main save was created
    if (Memcard_GetState() == MEMCARD_SUCCESS && Memcard_GetSaveStatus() == CARDSAVE_EXIST)
    {
        // create/load custom save file
        int is_created = KARPlusSave_CreateOrLoad(); // create/load the save file

        // update hash
        stc_karplus_save_hash = _hash_32(stc_karplus_save, SAVE_SIZE);

        // enter a new scene and ask the user if they just created a custom save
        if (is_created)
        {
            ;
        }
    }
    // main save file wasn't created, we're not creating a KARPlus save file, init the mod save data here
    else
        Modloader_InitSaveData();

    OSReport("Finished initializing save.\n");
    OSReport("---------------------\n");
    OSReport("\n");

    return;
}
CODEPATCH_HOOKCREATE(0x80047834, "", KARPlusSave_OnSaveCreateOrLoad, "", 0)
CODEPATCH_HOOKCREATE(0x80047720, "", KARPlusSave_OnSaveCreateOrLoad, "", 0)

/*---------------------------------------------------------------------*
Name:           KARPlusSave_OnMainMenuEnter

Description:    Inserted at the end of vanilla main menu initialization.
                Determines whether or not to write out data to memcard.

Arguments:      none.

Returns:        none.

*---------------------------------------------------------------------*/
void KARPlusSave_OnMainMenuEnter()
{
    KARPlusSave_Write();
}
CODEPATCH_HOOKCREATE(0x800189a4, "", KARPlusSave_OnMainMenuEnter, "", 0)

/*---------------------------------------------------------------------*
Name:           KARPlusSave_OnNoCardInserted

Description:    Inserted at the end of vanilla main menu initialization.
                Determines whether or not to write out data to memcard.

Arguments:      none.

Returns:        none.

*---------------------------------------------------------------------*/
void KARPlusSave_OnNoSave()
{
    // check if no card inserted
    if (Memcard_GetState() == MEMCARD_ERROR)
    {
        // set as no save to skip prompt
        Memcard_SetSaveStatus(CARDSAVE_IGNORE);

        // init mod save data (default values)
        Modloader_InitSaveData();

        // skip all tutorial vids
        GameData *gd = Gm_GetGameData();
        gd->is_showed_tutorial_airride = 1;
        gd->is_showed_tutorial_topride = 1;
        gd->is_showed_tutorial_city = 1;

        /*
        025367BC 003BFFFF
        025369FC 003BFFFF
        025368d4 003BFFFF
        04536EE8 FFFFFFFF
        04536EEC 00000000
        */

        // unlock everything
        int *unlock_bitfield = (int *)0x80536EE8;
        unlock_bitfield[0] = -1;
        unlock_bitfield[1] = 0;

        // completed checklist
        int *airride_checklist = (int *)0x805367BC;
        memset(airride_checklist, -1, 0x78);
        int *topride_checklist = (int *)0x805369FC;
        memset(topride_checklist, -1, 0x78);
        int *city_checklist = (int *)0x805368d4;
        memset(city_checklist, -1, 0x78);

        MainMenu_InitAllVariables();
        FGM_LoadInGameBanks();
        Scene_SetNextMajor(MJRKIND_MENU);
        Scene_ExitMajor();
    }

    // otherwise card is inserted but has no save file, so continue to prompt
}
CODEPATCH_HOOKCREATE(0x80047718, "", KARPlusSave_OnNoSave, "", 0)

/*---------------------------------------------------------------------*
Name:           KARPlusSave_Init

Description:    Allocates save file runtime struct. Places branches
                to save related functions in game memory.

Arguments:      none.

Returns:        none.

*---------------------------------------------------------------------*/
void KARPlusSave_Init()
{

    stc_karplus_save = HSD_MemAlloc(SAVE_SIZE); // alloc save data
    stc_karplus_save_hash = -1;

    // init save data
    stc_karplus_save->version_major = VERSION_MAJOR;
    stc_karplus_save->version_minor = VERSION_MINOR;
    stc_karplus_save->mod_num = 0;

    // install functions
    CODEPATCH_HOOKAPPLY(0x80047834);
    CODEPATCH_HOOKAPPLY(0x80047720);
    CODEPATCH_HOOKAPPLY(0x80047718);

    CODEPATCH_HOOKAPPLY(0x800189a4);
    CODEPATCH_REPLACEFUNC(KARPlus_WriteSave, KARPlusSave_Write);

    return;
}

/*---------------------------------------------------------------------*
Name:           KARPlusSave_CreateOrLoad

Description:    Handles creating and loading the custom save file (done
                automatically without asking user). Executes on boot up
                when loading the memcard prompt scene.

Arguments:      none.

Returns:        is_created bool.

*---------------------------------------------------------------------*/
int KARPlusSave_CreateOrLoad()
{
    s32 cardResult;
    CARDFileInfo fileInfo;
    CARDStat stat;

    int is_created = 0;

    // Mount the memory card
    cardResult = CARDMount(0, (void *)0x805a7e80, 0);
    if (cardResult != CARD_RESULT_READY)
    {
        OSReport("Failed to mount memory card.\n");
        return 0;
    }

    // check for existing file
    cardResult = CARDOpen(0, save_name, &fileInfo);

    // no exist
    if (cardResult == CARD_RESULT_NOFILE)
    {
        OSReport("No save file found, creating one...\n");

        // create save file
        cardResult = CARDCreate(0, save_name, SAVE_SIZE, &fileInfo);
        if (cardResult != CARD_RESULT_READY)
        {
            OSReport("Error creating save file.\n");
            CARDClose(&fileInfo);
            CARDUnmount(0);
            return 0;
        }

        OSReport("Save file created with size 0x%x\n", SAVE_SIZE);

        // initialize mods' save data
        Modloader_InitSaveData();

        // write to card
        cardResult = CARDWrite(&fileInfo, stc_karplus_save, SAVE_SIZE, 0);
        if (cardResult != CARD_RESULT_READY)
        {
            OSReport("Error initializing save file (result %d).\n", cardResult);
            CARDClose(&fileInfo);
            CARDUnmount(0);
            return 0;
        }

        OSReport("Created save file on card.\n");

        is_created = 1;
    }
    // exists
    else if (cardResult == CARD_RESULT_READY)
    {
        OSReport("Save file found.\n");

        // Get file status to retrieve size in blocks
        cardResult = CARDGetStatus(0, fileInfo.fileNo, &stat);
        if (cardResult != CARD_RESULT_READY)
        {
            OSReport("Failed to get file status.\n");
            CARDClose(&fileInfo);
            CARDUnmount(0);
            return 0;
        }

        // ensure this save file is the size we expect it to be
        if (stat.length != SAVE_SIZE)
        {
            OSReport("Error, expected file to be size %d, is %d\n", SAVE_SIZE, stat.length);
            CARDClose(&fileInfo);
            CARDUnmount(0);
            return 0;
        }

        OSReport("Loading it\n");

        cardResult = CARDRead(&fileInfo, stc_karplus_save, OSRoundUp512B(SAVE_SIZE), 0);
        if (cardResult != CARD_RESULT_READY)
        {
            OSReport("Failed to read save data.\n");
            CARDClose(&fileInfo);
            CARDUnmount(0);
            return 0;
        }

        // initialize mods' save data
        Modloader_InitSaveData();
    }

    CARDClose(&fileInfo);
    CARDUnmount(0);

    return is_created;
}

/*---------------------------------------------------------------------*
Name:           KARPlusSave_Write

Description:    Writes the save data to the memory card syncronously.

Arguments:      none.

Returns:        cardResult.

*---------------------------------------------------------------------*/
int KARPlusSave_Write()
{

    // check if save contents were modified
    int cur_hash = _hash_32(stc_karplus_save, SAVE_SIZE);
    if (cur_hash != stc_karplus_save_hash)
    {
        stc_karplus_save_hash = cur_hash;

        // to-do: put this on a thread and make operations async
        if (Memcard_GetSaveStatus() == CARDSAVE_NOEXIST)
            return CARD_RESULT_NOCARD;

        s32 cardResult;
        CARDFileInfo fileInfo;
        CARDStat stat;

        // Mount the memory card
        cardResult = CARDMount(0, (void *)0x805a7e80, 0);
        if (cardResult != CARD_RESULT_READY)
        {
            OSReport("Failed to mount memory card.\n");
            return cardResult;
        }

        // check for existing file
        cardResult = CARDOpen(0, save_name, &fileInfo);

        if (cardResult != CARD_RESULT_READY)
        {
            OSReport("Failed to open save file.\n");
            CARDClose(&fileInfo);
            CARDUnmount(0);
            return cardResult;
        }

        // write save data
        cardResult = CARDWrite(&fileInfo, stc_karplus_save, SAVE_SIZE, 0);
        if (cardResult != CARD_RESULT_READY)
        {
            OSReport("Error updating save file.\n");
            CARDClose(&fileInfo);
            CARDUnmount(0);
            return cardResult;
        }

        CARDClose(&fileInfo);
        CARDUnmount(0);
        return cardResult;
    }

    return CARD_RESULT_READY;
}

/*---------------------------------------------------------------------*
Name:           KARPlusSave_Alloc

Description:    Returns a ptr to save data

Arguments:      hash - unique identifier for this mod
                size - size of the alloc

Returns:        ptr to mod's user_data.

*---------------------------------------------------------------------*/
void *KARPlusSave_Alloc(GlobalMod *mod, int menu_size, int user_size)
{
    int mod_hash = _hash_32_str(mod->data.name);

    if (stc_karplus_save->mod_num > GetElementsIn(stc_karplus_save->metadata))
    {
        // to-do, free something and ensure it gets alloc'd
        OSReport("Save: mod_num over %d\n", GetElementsIn(stc_karplus_save->metadata));
        return 0;
    }

    // copy mod hash
    stc_karplus_save->metadata[stc_karplus_save->mod_num].mod_hash = mod_hash;

    // get data offset
    int menu_offset;
    if (stc_karplus_save->mod_num == 0)
        menu_offset = (int)&stc_karplus_save->data - (int)stc_karplus_save;
    else
    {
        menu_offset = stc_karplus_save->metadata[stc_karplus_save->mod_num - 1].user_data.offset +
                      stc_karplus_save->metadata[stc_karplus_save->mod_num - 1].user_data.size;
    }
    int user_offset = menu_offset + menu_size;

    stc_karplus_save->metadata[stc_karplus_save->mod_num].is_present = 1; // set mod as present

    // init menu
    stc_karplus_save->metadata[stc_karplus_save->mod_num].menu_data.offset = menu_offset;
    stc_karplus_save->metadata[stc_karplus_save->mod_num].menu_data.size = menu_size;
    if (menu_size > 0)
        memset(&stc_karplus_save->data[menu_offset], -1, menu_size); // fill with -1's to indicate unused space

    // init user
    stc_karplus_save->metadata[stc_karplus_save->mod_num].user_data.offset = user_offset;
    stc_karplus_save->metadata[stc_karplus_save->mod_num].user_data.size = user_size;

    // get mem ptr
    void *save_data_ptr = 0;
    if (user_size > 0)
        save_data_ptr = (void *)((int)stc_karplus_save + user_offset);

    OSReport("alloc'd mod %s index %d hash 0x%x metadata @ %p\n",
             mod->data.name,
             stc_karplus_save->mod_num,
             stc_karplus_save->metadata[stc_karplus_save->mod_num].mod_hash,
             &stc_karplus_save->metadata[stc_karplus_save->mod_num]);

    stc_karplus_save->mod_num++;

    // save pointers to mod data
    mod->save.menu_data = (MenuSave *)(&((u8 *)&stc_karplus_save->data)[menu_offset]);
    mod->save.menu_num = menu_size / sizeof(MenuSave);
    mod->save.user_data = (MenuSave *)(&((u8 *)&stc_karplus_save->data)[user_offset]);
    mod->save.user_size = user_size;

    return save_data_ptr;
}

/*---------------------------------------------------------------------*
Name:           KARPlusSave_VerifySize

Description:    Returns a ptr to save data

Arguments:      hash - unique identifier for this mod
                size - size of the alloc

Returns:        bool - if the save size changed.

*---------------------------------------------------------------------*/
int KARPlusSave_VerifySize(GlobalMod *mod, int menu_size, int user_size)
{
    int is_size_changed = 0;
    int mod_hash = _hash_32_str(mod->data.name);
    int new_size = menu_size + user_size;

    // find this save
    for (int i = 0; i < stc_karplus_save->mod_num; i++)
    {
        if (mod_hash == stc_karplus_save->metadata[i].mod_hash)
        {
            int saved_size = stc_karplus_save->metadata[i].menu_data.size + stc_karplus_save->metadata[i].user_data.size;

            if (new_size > saved_size)
            {
                OSReport("Current version save is larger (0x%08x / 0x%08x).\n",
                         new_size,
                         saved_size);

                int free_size = KARPlusSave_CheckFreeData();
                int shift_amt = new_size - saved_size;

                // resize
                if (free_size >= new_size)
                {
                    OSReport("Resizing save...\n");

                    // shift other data
                    for (int j = stc_karplus_save->mod_num - 1; j > i; j--)
                    {
                        // shift data
                        int data_size_old = stc_karplus_save->metadata[j].menu_data.size + stc_karplus_save->metadata[j].user_data.size;
                        u8 *data_ptr_old = &stc_karplus_save->data[stc_karplus_save->metadata[j].menu_data.offset];
                        memmove(data_ptr_old + shift_amt, data_ptr_old, data_size_old);

                        // update metadata
                        stc_karplus_save->metadata[j].menu_data.offset += shift_amt;
                        stc_karplus_save->metadata[j].user_data.offset += shift_amt;
                    }

                    // fill new menu data with -1
                    int old_menusave_size = stc_karplus_save->metadata[i].menu_data.size;
                    if (menu_size > old_menusave_size)
                    {
                        OSReport("Nulling additional menu saves.\n");
                        memset(&stc_karplus_save->data[stc_karplus_save->metadata[i].menu_data.offset + old_menusave_size], -1, menu_size - old_menusave_size); // fill with -1's to indicate unused space
                    }

                    // update metadata
                    stc_karplus_save->metadata[i].menu_data.size = menu_size;
                    stc_karplus_save->metadata[i].user_data.size = user_size;

                    // re-init data?
                    is_size_changed = 1;
                }
                // unable to resize
                else
                {
                    OSReport("not enough free space to resize (0x%08x / 0x%08x)\n", new_size, free_size);
                }
            }
            else if (new_size < saved_size)
            {
                OSReport("Current version save is smaller (0x%08x / 0x%08x).\n",
                         new_size,
                         saved_size);
                OSReport("Resizing save...\n");

                int shift_amt = new_size - saved_size;

                // shift other data
                for (int j = i + 1; j > stc_karplus_save->mod_num; j++)
                {
                    // shift data
                    int data_size_old = stc_karplus_save->metadata[j].menu_data.size + stc_karplus_save->metadata[j].user_data.size;
                    u8 *data_ptr_old = &stc_karplus_save->data[stc_karplus_save->metadata[j].menu_data.offset];
                    memmove(data_ptr_old + shift_amt, data_ptr_old, data_size_old);

                    // update metadata
                    stc_karplus_save->metadata[j].menu_data.offset += shift_amt;
                    stc_karplus_save->metadata[j].user_data.offset += shift_amt;
                }

                // fill all menu data with -1 if menu size changed
                int old_menusave_size = stc_karplus_save->metadata[i].menu_data.size;
                if (menu_size < old_menusave_size)
                {
                    OSReport("Nulling all menu saves.\n");
                    memset(&stc_karplus_save->data[stc_karplus_save->metadata[i].menu_data.offset], -1, menu_size); // fill with -1's to indicate unused space
                }

                // update metadata
                stc_karplus_save->metadata[i].menu_data.size = menu_size;
                stc_karplus_save->metadata[i].user_data.size = user_size;

                // re-init data?
                is_size_changed = 1;
            }

            int menu_data_offset = stc_karplus_save->metadata[i].menu_data.offset;
            int user_data_offset = stc_karplus_save->metadata[i].user_data.offset;

            // save pointers
            mod->save.menu_data = (MenuSave *)(&((u8 *)&stc_karplus_save->data)[menu_data_offset]);
            mod->save.menu_num = menu_size / sizeof(MenuSave);
            mod->save.user_data = (MenuSave *)(&((u8 *)&stc_karplus_save->data)[user_data_offset]);
            mod->save.user_size = user_size;

            OSReport("verified mod %s index %d hash 0x%x metadata @ %p\n",
                     mod->data.name,
                     i,
                     stc_karplus_save->metadata[i].mod_hash,
                     &stc_karplus_save->metadata[i]);

            break;
        }
    }

    return is_size_changed;
}

/*---------------------------------------------------------------------*
Name:           KARPlusSave_CheckModDataExists

Description:    Returns a bool if a mod's save data exists

Arguments:      mod_name - unique mod name string.

Returns:        bool, isExists

*---------------------------------------------------------------------*/
int KARPlusSave_CheckModDataExists(GlobalMod *mod)
{
    int mod_hash = _hash_32(mod->data.name, strlen(mod->data.name));

    for (int i = 0; i < stc_karplus_save->mod_num; i++)
    {
        if (mod_hash == stc_karplus_save->metadata[i].mod_hash)
            return 1;
    }

    return 0;
}

/*---------------------------------------------------------------------*
Name:           KARPlusSave_CheckFreeData

Description:    Returns the amount of free memory in the data section

Arguments:      none.

Returns:        Amount of free memory in the data section.

*---------------------------------------------------------------------*/
int KARPlusSave_CheckFreeData()
{
    int size_used = 0;

    for (int i = 0; i < stc_karplus_save->mod_num; i++)
        size_used += stc_karplus_save->metadata[i].user_data.size + stc_karplus_save->metadata[i].menu_data.size;

    return SAVE_SIZE - offsetof(KARPlusSave, data) - size_used;
}

/*---------------------------------------------------------------------*
Name:           KARPlusSave_Get

Description:    Returns a ptr to the city settings save data.

Arguments:      none.

Returns:        KARPlusSave ptr.

*---------------------------------------------------------------------*/
KARPlusSave *KARPlusSave_Get()
{
    return stc_karplus_save;
}
