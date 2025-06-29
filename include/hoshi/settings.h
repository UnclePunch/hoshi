#ifndef HOSHISETTINGS_H
#define HOSHISETTINGS_H

typedef enum OptionKind
{
    OPTKIND_VALUE,
    OPTKIND_MENU,
    OPTKIND_SCENE,
    OPTKIND_NUM,
} OptionKind;

typedef enum MenuState
{
    TRANSKIND_NONE,
    TRANSKIND_ADVANCE,
    TRANSKIND_REGRESS,
} MenuState;

typedef struct MenuDesc MenuDesc;

typedef struct OptionDesc
{
    char *name;
    u16 pri;
    OptionKind kind : 16;
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
    u16 option_num;
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