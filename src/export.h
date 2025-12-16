#ifndef EXPORT_H
#define EXPORT_H

#include "hoshi.h"

void *_Hoshi_ImportMod(char *id);
void _Hoshi_ExportMod(void *export);
void Export_Init();
#endif