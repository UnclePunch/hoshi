#include "text.h"
#include "os.h"
#include "hsd.h"
#include "preload.h"
#include "scene.h"
#include "inline.h"
#include "audio.h"
#include "game.h"
#include <string.h>

#include "more_scenes.h"
#include "code_patch/code_patch.h"

MajorSceneDesc major_scene_descs[MJRKIND_NUM * 2] = {0}; // alloc major scenes
int major_scene_num = MJRKIND_NUM;                       // hoshi will be changing this
MinorSceneDesc minor_scene_descs[MNRKIND_NUM * 2] = {0}; // alloc minor scenes
int minor_scene_num = MNRKIND_NUM;                       // hoshi will be changing this

void Scenes_Init()
{
    Scenes_CopyVanilla();
    Scenes_ApplyPatches();
}

// Function Patches
void Gm_MajorPatch()
{
    // r27 = GmData
    // r28 = unk
    // r29 = major_scene_desc
    // r30 = GmData
    // r31 = major_scene_descs (yes again 80008378)

    OSClearReports();

    // not present in vanilla, im adding this to allow booting to scenes other than the title screen
    Scene_InitHeaps();

    int major_num = major_scene_num;
    GameData *gd = Gm_GetGameData();
    int *reboot_data = (int *)0x805dd540;

    // major scene loop
    while (1)
    {
        // update cur major
        gd->major_cur = gd->major_pending;

        // find this major
        MajorSceneDesc *this_major_desc = 0;
        for (int i = 0; i < major_num; i++)
        {
            if (major_scene_descs[i].major_id == gd->major_cur)
            {
                this_major_desc = &major_scene_descs[i];
                break;
            }
        }

        if (!this_major_desc)
        {
            OSReport("scene: couldn't find major %d\n", gd->major_cur);
            assert("0");
        }

        // set default next major
        gd->major_pending = this_major_desc->next_major_id;
        gd->request_major_exit = 0;

        // set initial minor
        Scene_SetNextMinor(this_major_desc->initial_minor_id);

        // run major enter func
        if (this_major_desc->cb_Enter)
            this_major_desc->cb_Enter();

        while (!gd->request_major_exit)
        {
            // exec minor scene
            Gm_Minor();

            // dont run on unplug memcard scene
            if ((Scene_GetCurrentMajor() == MJRKIND_CARD || Scene_GetCurrentMinor() != minor_scene_num) &&
                reboot_data[1] == 0 &&
                this_major_desc->cb_ExitMinor)
                this_major_desc->cb_ExitMinor();
        };

        // reboot?
        if (reboot_data[1] != 0)
            reboot_data[1] = 0;
    }
}
CODEPATCH_HOOKCREATE(0x8000836c, "", Gm_MajorPatch, "", 0)

// Minor Scene redirect ASM patches
// note: using r to have the compiler alloc a register isn't completely safe in this context, re-do this with a .s file?
void minor_scene_asm_1()
{

    asm volatile(
        "mr 31, %0\n"        //
        "lwz 0, 0x0 (%1)\n"  //
        "lis 12, %2@h\n"     //
        "ori 12, 12, %2@l\n" //
        "mtctr 12\n"         //
        "bctr\n"             //
        :
        : "r"(&minor_scene_descs), "r"(&minor_scene_num), "i"(0x80008978 + 0x4)
        : "ctr");
}
void minor_scene_asm_2()
{

    asm volatile(
        "mr 4, %0\n"         //
        "lwz 0, 0x0 (%1)\n"  //
        "lis 12, %2@h\n"     //
        "ori 12, 12, %2@l\n" //
        "mtctr 12\n"         //
        "bctr\n"             //
        :
        : "r"(&minor_scene_descs), "r"(&minor_scene_num), "i"(0x80008b1c + 0x4)
        : "ctr");
}
void minor_scene_asm_3()
{

    asm volatile(
        "mr 4, %0\n"         //
        "lwz 0, 0x0 (%1)\n"  //
        "lis 12, %2@h\n"     //
        "ori 12, 12, %2@l\n" //
        "mtctr 12\n"         //
        "bctr\n"             //
        :
        : "r"(&minor_scene_descs), "r"(&minor_scene_num), "i"(0x80008b90 + 0x4)
        : "ctr");
}
void minor_scene_asm_4()
{

    asm volatile(
        "mr 4, %0\n"         //
        "lwz 0, 0x0 (%1)\n"  //
        "lis 12, %2@h\n"     //
        "ori 12, 12, %2@l\n" //
        "mtctr 12\n"         //
        "bctr\n"             //
        :
        : "r"(&minor_scene_descs), "r"(&minor_scene_num), "i"(0x80008c04 + 0x4)
        : "ctr");
}
void minor_scene_asm_5()
{

    asm volatile(
        "mr 4, %0\n"         //
        "lwz 0, 0x0 (%1)\n"  //
        "lis 12, %2@h\n"     //
        "ori 12, 12, %2@l\n" //
        "mtctr 12\n"         //
        "bctr\n"             //
        :
        : "r"(&minor_scene_descs), "r"(&minor_scene_num), "i"(0x80008c70 + 0x4)
        : "ctr");
}
void minor_scene_asm_6()
{

    asm volatile(
        "mr 31, %0\n"        //
        "lwz 0, 0x0 (%1)\n"  //
        "lis 12, %2@h\n"     //
        "ori 12, 12, %2@l\n" //
        "mtctr 12\n"         //
        "bctr\n"             //
        :
        : "r"(&minor_scene_descs), "r"(&minor_scene_num), "i"(0x80008cc8 + 0x4)
        : "ctr");
}
void minor_scene_asm_7()
{

    asm volatile(
        "mr 4, %0\n"         //
        "lwz 0, 0x0 (%1)\n"  //
        "lis 12, %2@h\n"     //
        "ori 12, 12, %2@l\n" //
        "mtctr 12\n"         //
        "bctr\n"             //
        :
        : "r"(&minor_scene_descs), "r"(&minor_scene_num), "i"(0x80008d78 + 0x4)
        : "ctr");
}

MajorSceneDesc *Scenes_GetMajorScenes()
{
    return major_scene_descs;
}
MinorSceneDesc *Scenes_GetMinorScenes()
{
    return minor_scene_descs;
}
MajorKind Scenes_InstallMajorScene(MajorSceneDesc *scene_desc)
{
    scene_desc->major_id = major_scene_num;
    memcpy(&major_scene_descs[major_scene_num], scene_desc, sizeof(*scene_desc));

    major_scene_num++;

    return scene_desc->major_id;
}
MinorKind Scenes_InstallMinorScene(MinorSceneDesc *scene_desc)
{
    scene_desc->idx = minor_scene_num;
    memcpy(&minor_scene_descs[minor_scene_num], scene_desc, sizeof(*scene_desc));

    minor_scene_num++;

    return scene_desc->idx;
}

// Functions
void Scenes_CopyVanilla()
{
    memcpy(&major_scene_descs, stc_major_scene_desc, sizeof(MajorSceneDesc) * MJRKIND_NUM); // copy all vanilla scenes to our relocated struct
    memset(stc_major_scene_desc, -1, sizeof(MajorSceneDesc) * MJRKIND_NUM);                 // zero it all out to ensure no reads are occuring

    memcpy(&minor_scene_descs, stc_minor_scene_desc, sizeof(MinorSceneDesc) * MNRKIND_NUM); // copy all vanilla scenes to our relocated struct
    memset(stc_minor_scene_desc, -1, sizeof(MinorSceneDesc) * MNRKIND_NUM);                 // zero it all out to ensure no reads are occuring
}
void Scenes_ApplyPatches()
{
    /*
    minor scene array accessed @
    80008978
    80008b1c
    80008b90
    80008c04
    80008c70
    80008cc8
    80008d78

    major scene array accessed @
    80008374
    */

    // replace Gm_Major func
    CODEPATCH_HOOKAPPLY(0x8000836c);

    static int branch_table[][2] = {
        {0x80008978, (int)&minor_scene_asm_1},
        {0x80008b1c, (int)&minor_scene_asm_2},
        {0x80008b90, (int)&minor_scene_asm_3},
        {0x80008c04, (int)&minor_scene_asm_4},
        {0x80008c70, (int)&minor_scene_asm_5},
        {0x80008cc8, (int)&minor_scene_asm_6},
        {0x80008d78, (int)&minor_scene_asm_7},
        {(int)KARPlus_GetMajorScenes, (int)&Scenes_GetMajorScenes},
        {(int)KARPlus_GetMinorScenes, (int)&Scenes_GetMinorScenes},
        {(int)KARPlus_InstallMajorScene, (int)&Scenes_InstallMajorScene},
        {(int)KARPlus_InstallMinorScene, (int)&Scenes_InstallMinorScene},
    };

    for (int i = 0; i < GetElementsIn(branch_table); i++)
        CODEPATCH_REPLACEFUNC(branch_table[i][0], branch_table[i][1]);

    return;
}
