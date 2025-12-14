#ifndef HOSHIFUNC_H
#define HOSHIFUNC_H

#include "preload.h"
#include "scene.h"

MajorSceneDesc *Hoshi_GetMajorScenes();
u8 Hoshi_InstallMajorScene(MajorSceneDesc *);
MinorSceneDesc *Hoshi_GetMinorScenes();
u8 Hoshi_InstallMinorScene(MinorSceneDesc *);

int Hoshi_WriteSave();

int Hoshi_AddPreloadMenuFile(char *file_name);
int Hoshi_AddPreloadGameFile(char *file_name, PreloadHeapKind heap_kind);

void *Hoshi_ImportMod(char *id, int version_major, int version_minor);
void Hoshi_ExportMod(void *export);
#endif