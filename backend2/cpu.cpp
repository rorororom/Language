#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "creator_cpu.h"
#include "cpu.h"
#include "../common/log_funcs.h"
#include "../common/const.h"
#include "stack.h"

#define DEF_CMD(name, num, has_arg, code)                     \
    case CMD_##name:                                          \
        code                                                  \
        i++;                                                  \
        break;

void ProcesscodeArray(struct Cpu* source) {
    assert(source != NULL);
    assert(source->codeArray != NULL);

    int value = 0, code_arg = 0, startCode = 0, code = 0;
    int i = 0, number1 = 0, number2 = 0, exponent = 0, base = 0, result = 0;

    while (i < source->position)
    {
        startCode = source->codeArray[i];
        code = startCode & 31;

        //printf ("%d\n", startCode);
        // printf ("%d\n", code);

        if (startCode == -1)
        {
            break;
        }

        switch (code) {
            #include "../common/commands.h"

            default:
                printf("Неизвестный код операции: %d\n", code);
                i++;
                break;
        }
    }
}

int return_arg (struct Cpu* source, int code)
{
    assert (source != NULL);

    switch (code)
    {
        case RAX:
            return source->codeRegister[1];
            break;
        case RBX:
            return source->codeRegister[2];
            break;
        case RCX:
            return source->codeRegister[3];
            break;
        default:
            printf("Неизвестный регистр: %d\n", code);
            break;
    }

    return 0;
}

int CheckStackSizeForOperation (struct Stack* myStack)
{
    assert (myStack != NULL);

    if (GetSizeStack(myStack) < 2)
    {
        printf("Ошибка: недостаточно элементов в стеке для выполнения операции.\n");
        return 0;
    }
    return 1;
}

void CommandPrintout(struct Cpu* source)
{
    assert(source != NULL);
    assert(source->codeArray != NULL);

    fprintf(LOG_FILE, "МАССИВ КОМАНД\n");
    for (int i = 0; i < source->position; i++) {
        fprintf(LOG_FILE, "%d - %d\n", i, source->codeArray[i]);
    }
}

void PopArg (struct Cpu* source, int code)
{
    switch (code)
    {
        case RAX:
            source->codeRegister[1] = StackPop(&source->myStack);
            printf("rbx = %d\n", source->codeRegister[1] );
            break;
        case RBX:
            source->codeRegister[2] = StackPop(&source->myStack);
            printf("rbx = %d\n", source->codeRegister[2] );
            break;
        case RCX:
            source->codeRegister[3] = StackPop(&source->myStack);
            break;
        default:
            printf("!!!!!Неизвестный регистр: %d\n", code);
            break;
    }
}


// void ArifmeticCommand (struct Cpu* source, int code)
// {
//     assert (source != NULL);
//
//     int number1 = 0, number2 = 0;
//
//     if (CheckStackSizeForOperation(&source->myStack))
//     {
//         number1 = StackPop(&source->myStack);
//         number2 = StackPop(&source->myStack);
//     }
// }
