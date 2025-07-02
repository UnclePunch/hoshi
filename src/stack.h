#ifndef STACK_H
#define STACK_H

#define STACK_MAX_DEPTH_LOG 16

typedef struct StackChain StackChain;

struct StackChain
{
    StackChain *next;
    void *lr_save;
};

typedef struct StackData
{
    void *x0;
    StackChain *chain;
} StackData;

typedef struct StackLog
{
    int num;
    struct
    {
        void *addr;
        void *back_chain;
        void *lr;
    } log[STACK_MAX_DEPTH_LOG];
    StackData *stack_data;
    int exception_kind;
    int dsisr;
    void *segfault_instr_addr;
} StackLog;

void Stack_Init();
void Stack_ApplyPatches();
char *Stack_FindSymbolNameFromAddress(void *lr);

#endif