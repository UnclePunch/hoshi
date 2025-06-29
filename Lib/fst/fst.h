#ifndef FST_H
#define FST_H

#include <stdarg.h>

void FST_ForEachInFolder(char *dir_name, char *file_extension, int check_subdirectories, void (*file_cb)(int file_entrynum, void *args), void *args);
char *FST_GetFilenameFromEntrynum(int entrynum);
char *FST_GetFilePathFromEntrynum(int entrynum);

#endif