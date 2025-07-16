//====================================================================================================
/*
 * Description:
 * This library injects code to a .dol address.
 *
 * Usage Instructions:
 * - Include "lib/code_patch/code_patch.c" in your code.
 * - ???
 * - profit
 */
//===================================================================================================

#include "os.h"
#include "code_patch.h"

void _CodePatch_Apply(PatchDesc *patch_desc)
{

    return;

    while (patch_desc->patch_kind != -1)
    {
        int *dol_addr = (int *)(0x80000000 | patch_desc->addr);

        switch (patch_desc->patch_kind)
        {
        case (CODEPATCHKIND_HOOK):
        {
            _CodePatch_HookApply(dol_addr, patch_desc->func_ptr);
            break;
        }
        case (CODEPATCHKIND_REPLACEINSTR):
        {
            _CodePatch_OverwriteApply(dol_addr, patch_desc->instruction);
            break;
        }
        case (CODEPATCHKIND_REPLACECALL):
        {
            _CodePatch_ReplaceCall(dol_addr, patch_desc->func_ptr);
            break;
        }
        case (CODEPATCHKIND_REPLACEFUNC):
        {
            _CodePatch_ReplaceFunc(dol_addr, patch_desc->func_ptr);
            break;
        }
        }

        patch_desc++;
    }
}

// asm patch
void _CodePatch_HookApply(int *dol_addr, void *hook_func)
{
    HookKind hook_kind = ((int *)hook_func)[0];
    int *hook_exit_ptr = &((int *)hook_func)[((((int *)hook_func)[1] & 0x03ffffff) / 4) + 1];
    int *hook_exec_start_ptr = &((int *)hook_func)[2]; // header is 2 lines, begin executing on the third instruction

    int *instr_orig_ptr;
    int *instr_return_ptr;
    int *instr_alt_return_ptr;

    switch (hook_kind)
    {
    case (HOOKKIND_NORMAL):
    {
        instr_orig_ptr = &hook_exit_ptr[0];
        instr_return_ptr = &hook_exit_ptr[1];
        instr_alt_return_ptr = 0;
        break;
    }
    case (HOOKKIND_CONDITIONAL):
    {
        instr_orig_ptr = &hook_exit_ptr[2];
        instr_return_ptr = &hook_exit_ptr[3];
        instr_alt_return_ptr = &hook_exit_ptr[4];
        break;
    }
    }

    // skip if already applied
    if ((_SHIFTR(*instr_return_ptr, 26, 6)) == 0x12)
        return;

    // get orig instruction
    int orig_instr = *dol_addr;

    // modify the original instruction if it is a branch
    if ((_SHIFTR(orig_instr, 26, 6)) == 0x12) // adjust if its a branch instruction
    {
        int dest_addr = (int)dol_addr + (orig_instr & ~(0xfc000001));                                   // get branch's destination addr
        orig_instr = (orig_instr & (0xfc000001)) | ((dest_addr - (int)instr_orig_ptr) & ~(0xfc000000)); // adjust branch instruction to be offset from asm hook
    }

    *instr_orig_ptr = orig_instr;                                                                       // copy original store instruction to asm hook
    *dol_addr = BRANCH_CREATE(dol_addr, hook_exec_start_ptr, 0);                                        // place branch in dol to asm hook
    (*instr_return_ptr = 0x48000000 | (((*instr_return_ptr - (int)instr_return_ptr))) & ~(0xfc000000)); // create an instruction to branch back to hook site

    // conditional specific stuff
    if (hook_kind == HOOKKIND_CONDITIONAL)                                                                              // copy duplicate original instruction to asm hook for alt return branch
        (*instr_alt_return_ptr = 0x48000000 | (((*instr_alt_return_ptr - (int)instr_alt_return_ptr))) & ~(0xfc000000)); // create an instruction to branch to alt return site

    TRK_FlushCache(dol_addr, 4);   // flush cache
    TRK_FlushCache(hook_func, 32); // flush cache
}

void _CodePatch_OverwriteApply(int *dol_addr, int instr)
{
    *dol_addr = instr;
    TRK_FlushCache(dol_addr, 4); // flush cache
}
void _CodePatch_ReplaceCall(int *dol_addr, void *func_addr)
{
    *dol_addr = BRANCH_CREATE(dol_addr, func_addr, 1);
    TRK_FlushCache(dol_addr, 4); // flush cache
}
void _CodePatch_ReplaceFunc(int *dol_addr, void *func_addr)
{
    *dol_addr = BRANCH_CREATE(dol_addr, func_addr, 0);
    TRK_FlushCache(dol_addr, 4); // flush cache
}
