#ifndef HOSHISETTINGS_H
#define HOSHISETTINGS_H

#include "scene.h"

typedef enum OptionKind
{
    OPTKIND_VALUE,
    OPTKIND_MENU,
    OPTKIND_SCENE,
    OPTKIND_NUM,
} OptionKind;

typedef enum MenuPriority
{
    MENUPRI_VERYHIGH,
    MENUPRI_HIGH,
    MENUPRI_NORMAL,
    MENUPRI_LOW,
    MENUPRI_VERYLOW,
} MenuPriority;

typedef struct MenuDesc MenuDesc;

typedef struct OptionDesc
{
    char *name;
    char *description;
    OptionKind kind : 16;
    MenuPriority _pri : 16; /// ignore, used by hoshi at runtime
    union
    {
        struct
        {
            int *val;
            int value_num;
            char **value_names;
            void (*on_change)(int val);
        };
        struct
        {
            MenuDesc *menu_ptr;
        };
        struct
        {
            MajorKind major_idx;
        };
    };

} OptionDesc;

struct MenuDesc
{
    char *name;
    char *description;
    int option_num : 16;
    MenuPriority pri : 16;
    OptionDesc *options;
    MenuDesc *prev;
    u16 cursor;
    u16 scroll;
};
#pragma pack(push, 1) // Align to 1-byte boundaries
typedef struct MenuSave
{
    u16 hash;
    u8 val;
} MenuSave;
#pragma pack(pop)

#endif