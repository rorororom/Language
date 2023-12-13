#include <stdlib.h>
#include <assert.h>

#include "creator_cpu.h"
#include "../common/log_funcs.h"
#include "../common/const.h"

void CpuCtor(struct Cpu* source, struct Stack* myStack)
{
    assert(source != NULL);
    assert(myStack != NULL);

    source->myStack = *myStack;

    source->codeArray = (int*)calloc(MAXSIZE, sizeof(int));
    source->codeRegister[3] = {0};
}


void CpuDtor(struct Cpu* source) {
    if (source != NULL) {
        if (source->codeArray != NULL) {
            free(source->codeArray);
            source->codeArray = NULL;
        }

        StackDtor(&(source->myStack));

        source->position = 0;
        for (int i = 0; i < SIZEREGISTER; i++) {
            source->codeRegister[i] = 0;
        }
    }
}


int CpuVerify (struct Cpu* source)
{
    int cnt_errors = 0;
    if (source->codeArray == NULL)
    {
        cnt_errors = cnt_errors | ERROR_CODE_ARRAY_BIT;
    }
    if (source->codeRegister == NULL)
    {
        cnt_errors = cnt_errors | ERROR_CODE_REGISTER_BIT;
    }

    return cnt_errors;
}


void CpuDump(struct Cpu* source)
{
    fprintf(LOG_FILE, "CPU Dump:\n");

    for (int i = 0; i < source->position; i++)
    {
        fprintf(LOG_FILE, "%d = %d", i, source->codeArray[i]);
    }

    for (int i = 0; i < SIZEREGISTER; i++)
    {
        fprintf(LOG_FILE, "%d = %d", i, source->codeRegister[i]);
    }
}
