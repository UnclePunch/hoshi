#include "os.h"
#include "inline.h"

#include <stdarg.h>
#include <string.h>

#include "fst.h"

/////////////////////////
// Private Definitions //
/////////////////////////
struct FSTFilePathBuild
{
    char *path;
    int buffer_size;
    int pos;
    int file_entrynum;
};

//////////////////////////
// Private Declarations //
//////////////////////////

void _FST_ForEachInFolder(int dir_entrynum, char *file_extension, int check_subdirectories, void (*file_cb)(int file_entrynum, void *args), void *args);
char *_FST_GetExtension(char *filename);

///////////////////////
// Private Functions //
///////////////////////

void _FST_ForEachInFolder(int dir_entrynum, char *file_extension, int check_subdirectories, void (*file_cb)(int file_entrynum, void *args), void *args)
{
    // ensure a valid entrynum was passed
    if (dir_entrynum == -1)
        return;

    FSTEntry *dir_entry = &(*stc_fst_entries)[dir_entrynum];

    // ensure its a folder
    if (!dir_entry->is_dir)
        return;

    // ensure it has contents
    if (dir_entry->u.dir.last_entrynum <= dir_entrynum)
        return;

    // OSReport("iterating through folder %s\n", &(*stc_fst_filenames)[dir_entry->filename_offset]);

    // for each entrynum in this directory
    for (int entrynum = dir_entrynum + 1; entrynum < dir_entry->u.dir.last_entrynum; entrynum++)
    {
        // OSReport("checking entrynum %d/%d\n", entrynum, dir_entry->u.dir.last_entrynum);
        FSTEntry *file_entry = &(*stc_fst_entries)[entrynum];

        // if its a folder
        if (file_entry->is_dir)
        {
            char *this_foldername = &(*stc_fst_filenames)[file_entry->filename_offset];

            // not checking subdirectories, skip past its contents
            if (!check_subdirectories)
            {
                entrynum = file_entry->u.dir.last_entrynum - 1;
                // OSReport("skipping folder %s\n", this_foldername);
            }
            else
            {
                // user isn't searching for a file extension, execute on folders too
                if (!file_extension)
                    file_cb(entrynum, args);

                // enter folder
                // OSReport("entering folder %s\n", this_foldername);
                _FST_ForEachInFolder(entrynum, file_extension, check_subdirectories, file_cb, args);

                // skip entries in folder, already iterated over them
                entrynum = file_entry->u.dir.last_entrynum - 1;
            }
        }
        else
        {
            // filter by extension?
            char *this_filename = &(*stc_fst_filenames)[file_entry->filename_offset];
            char *this_extension = _FST_GetExtension(this_filename);

            if (!file_extension ||                             // any file
                                                               // OR
                (file_extension &&                             // requesting to filter extension
                 this_extension &&                             // was able to find an extension
                 strcmp(this_extension, file_extension) == 0)) // they match
            {
                // OSReport("executing cb on %s\n", this_filename);

                // entry is a file, execute the callback
                file_cb(entrynum, args);
            }
            // else
            // OSReport("skipping %s, invalid extension\n", this_filename);
        }
    }

    return;
}
char *_FST_GetExtension(char *filename)
{
    static char out[32];

    // parse string
    int period_pos = -1;
    int cur = 0;
    while (1)
    {

        // find the final period in the string
        char this_char = filename[cur];

        // remember the position of the period
        if (this_char == '.')
            period_pos = cur;

        // end at the null terminator
        else if (this_char == '\0')
            break;

        cur++;
    }

    // if no period, use language extension
    if (period_pos == -1)
        return 0;
    else
        strcpy(out, &filename[period_pos]); // copy extension

    return out;
}
void _FST_BuildPathCallback(int entrynum, struct FSTFilePathBuild *path_build)
{

    FSTEntry *dir_entry = &(*stc_fst_entries)[entrynum];

    // ensure path buffer hasn't overflowed
    if (path_build->pos == -1)
        return;

    if ((!dir_entry->is_dir && entrynum == path_build->file_entrynum) ||                                                                 // is the exact file we're looking for
        (dir_entry->is_dir && dir_entry->u.dir.last_entrynum > entrynum && dir_entry->u.dir.last_entrynum >= path_build->file_entrynum)) // folder contains the file we're looking for
    {

        // get entry name
        FSTEntry *file_entry = &(*stc_fst_entries)[entrynum];
        char *entry_name = &(*stc_fst_filenames)[file_entry->filename_offset];

        int name_size = strlen(entry_name);

        // check for buffer overflow
        if (name_size + path_build->pos + 2 >= path_build->buffer_size)
        {
            path_build->pos = -1;
            return;
        }

        // copy folder name to path
        strcpy(&path_build->path[path_build->pos], entry_name);
        path_build->pos += name_size;

        // add slash to the end if directory
        if (dir_entry->is_dir)
        {
            path_build->path[path_build->pos] = '/';
            path_build->pos++;
        }
        // terminate string
        else
        {
            path_build->path[path_build->pos] = '\0';
            path_build->pos++;
        }
    }

    return;
}

//////////////////////
// Public Functions //
//////////////////////

void FST_ForEachInFolder(char *dir_name, char *file_extension, int check_subdirectories, void (*file_cb)(int file_entrynum, void *args), void *args)
{
    _FST_ForEachInFolder(DVDConvertPathToEntrynum(dir_name), file_extension, check_subdirectories, file_cb, args);
}
char *FST_GetFilenameFromEntrynum(int entrynum)
{

    FSTEntry *file_entry = &(*stc_fst_entries)[entrynum];
    return &(*stc_fst_filenames)[file_entry->filename_offset];
}
char *FST_GetFilePathFromEntrynum(int entrynum)
{
    static char full_path[128];

    struct FSTFilePathBuild path_build = {
        .path = full_path,
        .buffer_size = sizeof(full_path),
        .pos = 0,
        .file_entrynum = entrynum,
    };

    _FST_ForEachInFolder(0, 0, 1, (void (*)(int, void *))_FST_BuildPathCallback, &path_build);

    // buffer overflowed
    if (path_build.pos == -1)
    {
        OSReport("FST: buffer overflow!\n");
        return 0;
    }

    return full_path;
}