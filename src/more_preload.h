#ifndef MOREPRELOAD_H
#define MOREPRELOAD_H

typedef struct PreloadCustomGameFiles
{
    int num;
    struct
    {
        char *name;                //
        PreloadFileKind file_kind; //
        int preload_kind;          // dictates during which scenes this file will be cached
        PreloadHeapKind heap_kind; // which heap the file will be loaded to
    } file[32];
} PreloadCustomGameFiles;

void Preload_Init();
void Preload_ApplyPatches();
void Preload_IncreasePersistentHeapSize();

#endif