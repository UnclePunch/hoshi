#ifndef BOOTSCENE_H
#define BOOTSCENE_H

typedef struct BootScene
{
    MajorKind kind;
    u8 pri;
} BootScene;

void BootScene_Init();
void _BootScene_Set(MajorKind kind, u8 pri);
void BootScene_Apply();


#endif