#ifndef SAVE_H
#define SAVE_H

#include "hoshi.h"

#define VERSION_MAJOR 1
#define VERSION_MINOR 0

#define SAVE_SIZE ((sizeof(KARPlusSave) + CARD_BLOCK_SIZE - 1) / CARD_BLOCK_SIZE) * CARD_BLOCK_SIZE

typedef enum DataKind
{
    DATAKIND_SETTINGS,
    DATAKIND_USER,
} DataKind;

typedef struct KARPlusSave
{
    u8 version_major;
    u8 version_minor;
    u8 mod_num;
    struct
    {
        int mod_hash;       // mod identifier
        struct              //
        {                   //
            int major : 15; //
            int minor : 16; //
        } version;          //
        int is_present : 1; // bool indicating the mod is currently in use
        struct
        {
            int offset : 16; // offset relative to KARPlusSave::data
            int size : 16;   // size of the data
        } menu_data;
        struct
        {
            int offset : 16; // offset relative to KARPlusSave::data
            int size : 16;   // size of the data
        } user_data;
    } metadata[50];
    u8 data[];
} KARPlusSave;

void KARPlusSave_Init();
int KARPlusSave_CheckModDataExists(GlobalMod *mod);
void KARPlusSave_GetModSaveData(GlobalMod *mod, void **out_menu_data, int *out_menu_data_size, void **out_user_data, int *out_user_data_size);
void *KARPlusSave_Alloc(GlobalMod *mod, int menu_size, int user_size);
int KARPlusSave_CreateOrLoad();
KARPlusSave *KARPlusSave_Get();
int KARPlusSave_Write();
int KARPlusSave_VerifySize(GlobalMod *mod, int menu_size, int user_size);
int KARPlusSave_CheckFreeData();
#endif