#ifndef MOREPRELOAD_H
#define MOREPRELOAD_H

typedef struct PreloadCustomGameFiles
{
    int num;
    struct
    {
        char *name;                     //
        PreloadHeapKind heap_kind : 8;  // which heap the file will be loaded to
        PreloadFileKind file_kind : 8;  //
        u16 pri;                        // higher = loaded sooner
        int preload_kind;               // dictates during which scenes this file will be cached
    } file[32];
} PreloadCustomGameFiles;

void Preload_Init();
void Preload_ApplyPatches();
void Preload_IncreasePersistentHeapSize();

#endif