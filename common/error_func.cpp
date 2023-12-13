#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "log_funcs.h"
#include "../backend2/stack.h"
#include "error_func.h"

void PrintStackErrors(int sum_errors)
{
    if (sum_errors & ERROR_SIZE_BIT)
    {
        fprintf(LOG_FILE, "Ошибка: Превышен размер стека\n");
    }

    if (sum_errors & ERROR_CAPACITY_BIT)
    {
        fprintf(LOG_FILE, "Ошибка: Ошибка вместимости стека (неположительная)\n");
    }

    if (sum_errors & ERROR_DATA_BIT)
    {
        fprintf(LOG_FILE, "Ошибка: Нулевой указатель на данные стека\n");
    }
}

int StackVerify(struct Stack* myStack)
{
    int sum_errors = 0;

    if (myStack->size > myStack->capacity)
    {
        printf("A\n");
        sum_errors = sum_errors | ERROR_SIZE_BIT;
    }

    if (myStack->capacity <= 0)
    {
        printf("AA\n");
        sum_errors = sum_errors | ERROR_CAPACITY_BIT;
    }

    if (myStack->data == NULL)
    {
        printf("AAA\n");
        sum_errors = sum_errors | ERROR_DATA_BIT;
    }

    if (sum_errors > 0)
    {
        PrintStackErrors(sum_errors);
    }

    return sum_errors;
}
