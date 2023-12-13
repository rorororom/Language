#ifndef CREATOR_CPU
#define CREATOR_CPU

#include "stack.h"
#include "../common/const.h"
#include <stdio.h>

#define CPU_VERIFY(source) {         \
    if (CpuVerify (source) > 0) {    \
        CPU_DUMP (source);           \
    }                               \
}

enum CpuErrors {
    ERROR_FILENAME_BIT        = 1,
    ERROR_OUTPUTFILE_BIT      = 1 << 2,
    ERROR_CODE_ARRAY_BIT      = 1 << 3,
    ERROR_CODE_REGISTER_BIT   = 1 << 4,
};

#define CONDITIONAL_JUMP(cmd_name, cmd_number, condition)   \
    DEF_CMD(cmd_name, cmd_number, 1,                        \
    {                                                       \
        int number1 = StackPop(&source->myStack);           \
        int number2 = StackPop(&source->myStack);           \
        if (condition)                                      \
        {                                                   \
            i++;                                            \
        }                                                   \
        else                                                \
        {                                                   \
            i = source->codeArray[i+1] - 1;                         \
        }                                                   \
    })

#define DEF_CMD(name, num, ...)                             \
        CMD_##name = num,

enum Commands
{
    #include "../common/commands.h"
};

#undef DEF_CMD

enum Register
{
    RAX = 1,
    RBX,
    RCX,
};
const int SIZEREGISTER = 4;

struct Cpu {
    int* codeArray;
    int position;
    int codeRegister[SIZEREGISTER];
    struct Stack myStack;
};

void CpuCtor (struct Cpu* source, struct Stack* myStack);
void CpuDtor (struct Cpu* source);
void CpuDump (struct Cpu* source);

int CpuVerify (struct Cpu* source);

#endif
