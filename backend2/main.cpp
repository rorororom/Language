#include <stdio.h>
#include <stdlib.h>

#include "stack.h"
#include "creator_cpu.h"
#include "cpu.h"
#include "../common/log_funcs.h"
#include "../common/const.h"

int main()
{
    OpenLogFile("LOGE.log", "w");

    Stack myStack = {};
    StackCtor(&myStack);
    //STACK_DUMP(&myStack);

    struct Cpu source = {};
    CpuCtor(&source, &myStack);
    CPU_DUMP(&source);

    int position = 0;
    FILE* file = fopen("../assets/code2.bin", "rb");

    if (file == NULL)
    {
        HANDLE_ERROR("Ошибка при открытии файла");
    }

    if (fread(&source.position, sizeof(int), 1, file) != 1)
    {
        fclose(file);
        HANDLE_ERROR("Ошибка при чтении числа position из файла");
    }

    printf("position = %d\n", source.position);

    if (fread(source.codeArray, sizeof(int), source.position, file) != source.position)
    {
        fclose(file);
        free(source.codeArray);
        HANDLE_ERROR("Ошибка при чтении массива codeArray из файла");
    }

    fclose(file);
    //STACK_DUMP(&myStack);
    CommandPrintout(&source);

    ProcesscodeArray(&source);
    //printf("size = %d\n", source.myStack.size);
    printf ("answer = %d\n", source.codeRegister[2]);
    //printf("aaa\n");
//     //CPU_DUMP(&source);  // Добавлен вывод состояния Cpu после обработки.
//
//     CpuDtor(&source);  // Вызываем деструктор Cpu.
//     StackDtor(&myStack);  // Вызываем деструктор Stack.

    return 0;
}
