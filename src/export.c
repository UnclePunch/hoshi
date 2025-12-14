/*---------------------------------------------------------------------------*
    Handles lowest level save file operations.
        - allocates runtime save struct
        - creates save file on memcard
        - loads save file from memcard
        - returns runtime save file struct
 *---------------------------------------------------------------------------*/

#include "os.h"
#include "hoshi.h"
#include "hoshi/log.h"
#include <string.h>

extern ModloaderData *stc_modloader_data;
extern GlobalMod *cur_mod_install;

void *_Hoshi_ImportMod(char *id, int version_major, int version_minor)
{
    for (int i = 0; i < stc_modloader_data->mod_num; i++)
    {
        GlobalMod *m = &stc_modloader_data->mods[i];
        if (strcmp(m->desc->name, id) == 0)   
        {
            // ensure a supported version is present
            if (version_major < m->desc->version.major)
            {
                OSReport("hoshi: Error importing mod \"%s\" Expected version %d.X but found version %d.%d.\n",
                            id,
                            version_major,
                            m->desc->version.major,
                            m->desc->version.minor);
                assert("0");
            }

            return m->export;
        }
    }

    LOG_WARN("hoshi: mod %s not installed!\n", id);

    return 0;
}
void _Hoshi_ExportMod(void *export)
{
    cur_mod_install->export = export;
    return;
}

