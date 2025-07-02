#ifndef MOREPRELOAD_H
#define MOREPRELOAD_H

typedef struct PreloadCustomGameFiles
{
    int num;
    struct
    {
        char *name;           //
        PreloadFileKind kind; //
        int preload_kind;     // dictates during which scenes this file will be cached
    } file[32];
} PreloadCustomGameFiles;

void Preload_Init();
void Preload_ApplyPatches();
void Preload_IncreasePersistentHeapSize();

#endif