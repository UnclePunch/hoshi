#ifndef LIB_H_CODEPATCH
#define LIB_H_CODEPATCH

void _CodePatch_HookApply(int *dol_addr, void *hook_func);
void _CodePatch_OverwriteApply(int *dol_addr, int instr);
void _CodePatch_ReplaceCall(int *dol_addr, void *func_addr);
void _CodePatch_ReplaceFunc(int *dol_addr, void *func_addr);

#define BRANCH_CREATE(src, dst, is_link) (0x48000000 | (is_link ? 1 : 0) | (((int)dst - (int)src) & ~(0xfc000000)))

#define STACKFRAME_CREATE   "stwu	1, -40 (1)\n\t"     \
                            "mflr 0\n\t"                \
                            "stw 0, 44 (1)\n\t"         \

#define STACKFRAME_DESTROY  "lwz 0, 44 (1)\n\t"         \
                            "mtlr 0\n\t"                \
                            "addi	1, 1, 40 \n\t"      \

#define STACKFRAME_BACKUPVOLATILE   "stwu	1, -40 (1)\n\t"     \
                                    "mflr 0\n\t"                \
                                    "stw 0, 44 (1)\n\t"         \
                                    "stw 3, 8 (1)\n\t"          \
                                    "stw 4, 12 (1)\n\t"         \
                                    "stw 5, 16 (1)\n\t"         \
                                    "stw 6, 20 (1)\n\t"         \
                                    "stw 7, 24 (1)\n\t"         \
                                    "stw 8, 28 (1)\n\t"         \
                                    "stw 9, 32 (1)\n\t"         \
                                    "stw 10, 36 (1)\n\t"        \

#define STACKFRAME_RESTOREVOLATILE  "lwz 3, 8 (1)\n\t"          \
                                    "lwz 4, 12 (1)\n\t"         \
                                    "lwz 5, 16 (1)\n\t"         \
                                    "lwz 6, 20 (1)\n\t"         \
                                    "lwz 7, 24 (1)\n\t"         \
                                    "lwz 8, 28 (1)\n\t"         \
                                    "lwz 9, 32 (1)\n\t"         \
                                    "lwz 10, 36 (1)\n\t"        \
                                    "lwz 0, 44 (1)\n\t"         \
                                    "mtlr 0\n\t"                \
                                    "addi	1, 1, 40 \n\t"      \

typedef enum HookKind
{
    HOOKKIND_NORMAL,
    HOOKKIND_CONDITIONAL,
} HookKind;

#define CODEPATCH_HOOKCREATE(_dol_addr, _prologue, _func, _epilogue, _exit_addr)                                            \
    void hook_##_dol_addr()                                                                                                 \
    {                                                                                                                       \
                                                                                                                            \
        __asm__ volatile(                                                                                                   \
                                                                                                                            \
            ".long %2\n\t"                          /* type of hook */                                                      \
            "b ptr_orig_instr" #_dol_addr "\n\t"    /* glorified pointer to the end of the injection (is not executed) */   \
            "b ptr_exit_addr" #_dol_addr "\n\t"     /* glorified pointer to the end of the injection (is not executed) */   \
            _prologue                               /* execute instructions passed into the macro */                        \
            "bl %0\n\t"                             /* call C function */                                                   \
            _epilogue                               /* execute instructions passed into the macro */                        \
            "ptr_orig_instr" #_dol_addr ":\n\t"     /* label pointing to */                                                 \
            ".long 0\n\t"                           /* null, replaced with the instruction we clobber to branch here */     \
            "ptr_exit_addr" #_dol_addr ":\n\t"      /* label indicating the end of the injection */                         \
            ".long %1\n\t"                          /* pointer to our return location, relocated to a branch at runtime */  \
            :                                                                                                               \
            : "i"(_func), "i"((_exit_addr) ? _exit_addr : (_dol_addr + 0x4)), "i"(HOOKKIND_NORMAL));                        \
    }

#define CODEPATCH_HOOKCONDITIONALCREATE(_dol_addr, _prologue, _func, _epilogue, _exit_addr, _alt_epilogue, _exit_addr_alt)                                                  \
    void hook_##_dol_addr()                                                                                                                                                 \
    {                                                                                                                                                                       \
                                                                                                                                                                            \
        __asm__ volatile(                                                                                                                                                   \
                                                                                                                                                                            \
            ".long %2\n\t"                          /* type of hook */                                                                                                      \
            "b ptr_orig_instr" #_dol_addr "\n\t"    /* glorified pointer to the end of the injection (is not executed) */                                                   \
            "b ptr_exit_addr" #_dol_addr "\n\t"     /* glorified pointer to the end of the injection (is not executed) */                                                   \
            "b ptr_alt_exit_addr" #_dol_addr "\n\t" /* glorified pointer to the end of the injection (is not executed) */                                                   \
            _prologue                               /* execute instructions passed into the macro */                                                                        \
            "bl %0\n\t"                             /* call C function */                                                                                                   \
            "cmpwi 3, 0\n\t"                        /* check return value of our function */                                                                                \
            _epilogue                               /* execute instructions passed into the macro */                                                                        \
            "bne alt_exit_" #_dol_addr "\n\t"      /* if func returns 1, exit to alt addr */                                                                                \
            "ptr_orig_instr" #_dol_addr ":\n\t"     /* label pointing to */                                                                                                 \
            ".long 0\n\t"                           /* null, replaced with the instruction we clobber to branch here */                                                     \
            "ptr_exit_addr" #_dol_addr ":\n\t"      /* label indicating the end of the injection */                                                                         \
            ".long %1\n\t"                          /* pointer to our return location, relocated to a branch at runtime */                                                  \
            "alt_exit_" #_dol_addr ":\n\t"          /**/                                                                                                                    \
            _alt_epilogue                           /* execute instructions passed into the macro */                                                                        \
            "ptr_alt_exit_addr" #_dol_addr ":\n\t"  /* label indicating the end of the injection */                                                                         \
            ".long %3\n\t"                          /* pointer to our alternate return location, relocated to a branch at runtime */                                        \
            :                                                                                                                                                               \
            : "i"(_func), "i"((_exit_addr) ? _exit_addr : (_dol_addr + 0x4)), "i"(HOOKKIND_CONDITIONAL), "i"((_exit_addr_alt) ? _exit_addr_alt : (_dol_addr + 0x4)));       \
    }

#define CODEPATCH_HOOKAPPLY(_dol_addr) _CodePatch_HookApply((int *)_dol_addr, hook_##_dol_addr)
#define CODEPATCH_REPLACEINSTRUCTION(_dol_addr, _instr) _CodePatch_OverwriteApply((int *)_dol_addr, _instr)
#define CODEPATCH_REPLACECALL(_dol_addr, _func) _CodePatch_ReplaceCall((int *)_dol_addr, (void *)_func)
#define CODEPATCH_REPLACEFUNC(_dol_addr, _func) _CodePatch_ReplaceFunc((int *)_dol_addr, (void *)_func)

typedef enum CodePatchKind
{
    CODEPATCHKIND_HOOK,
    CODEPATCHKIND_REPLACEINSTR,
    CODEPATCHKIND_REPLACECALL,
    CODEPATCHKIND_REPLACEFUNC,
} CodePatchKind;

typedef struct PatchDesc
{
    CodePatchKind patch_kind : 4;
    int addr : 28;
    union
    {
        void *func_ptr;
        int instruction;
    };
} PatchDesc;

#define CODEPATCHDESC_HOOK(_dol_addr) {.patch_kind = CODEPATCHKIND_HOOK, .addr = (0x80000000 | (int)_dol_addr), .func_ptr = hook_##_dol_addr}

#endif