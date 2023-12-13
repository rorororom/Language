#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdbool.h>

#include "../common/log_funcs.h"
#include "../common/const.h"

#include "compiler.h"

#define DEF_CMD(name, code, has_arg, action)                                            \
    if (source->textArray[cnt][0] == ':')                                               \
        {                                                                               \
            UpdateLabelPosition(source, &cnt);                                          \
        }                                                                               \
    else if (strcmp(source->textArray[cnt], #name) == 0)                                \
    {                                                                                   \
        cmd = code;                                                                     \
        if (has_arg)                                                                    \
        {                                                                               \
            argCode = ArgCode(source, &cmd, cnt);                                       \
            AddTwoElementsInCodeArray (source, &cnt, cmd, argCode, outputfile);         \
        }                                                                               \
        else {                                                                          \
            source->codeArray[source->position++] = cmd;                                \
            cnt++;                                                                      \
            fprintf(outputfile, "%d\n", code);                                          \
        }                                                                               \
    }

void GenerateCodeFromInput(struct Compiler *source, FILE* outputfile)
{
    assert(source != NULL);
    assert(outputfile != NULL);

    int value = 0, position = 0, argCode = 0, cmd = 0, cnt = 0;
    int numberStop = -1, registerFlag = 0;

    source->capacity = INITIAL_CAPACITY;

    source->codeArray = (int *)malloc(source->capacity * sizeof(int));
    if (source->codeArray == NULL)
    {
        fprintf(stderr, "Ошибка: Не удалось выделить память для codeArray\n");
    }

    InitializationUnknownMarks (source);
    InitializationLabels (source);

    while (cnt < source->words)
    {
        #include "../common/commands.h"

    }
    #undef DEF_CMD

    ResolveUnknownMarks (source);
}

int ArgCode(struct Compiler *source, int* cmd, int cnt)
{
    assert(source != NULL);
    assert(source->codeArray != NULL);

    int value = 0;

    if ('0' <= source->textArray[cnt + 1][0] && source->textArray[cnt + 1][0] <= '9')
    {
        return atoi(source->textArray[cnt + 1]);
    }
    else {
        if (source->textArray[cnt + 1][0] == ':')
        {
            if (source->labels[source->textArray[cnt + 1][1] - '0'] == -1)
            {
                source->unknownMarks[source->cntUnknownMarks++] = cnt + 1;
                source->unknownMarks[source->cntUnknownMarks++] = source->position + 1;
                return -100;
            }
            else
            {
                return source->labels[source->textArray[cnt + 1][1] - '0'];
            }
        }
        *cmd = *cmd | (1 << 5);
        return ReturnArg(source, cnt + 1);
    }
    return value;
}

int ReturnArg (struct Compiler *source, int i)
{
    assert(source != NULL);
    assert(source->codeArray != NULL);

    CHECK_REGISTER("RCX", RCX);
    CHECK_REGISTER("RAX", RAX);
    CHECK_REGISTER("RBX", RBX);

    else
    {
        printf ("Ошибка: неправильный аргумент = %s\n", source->textArray[i]);
        fprintf (LOG_FILE, "Ошибка: неправильный аргумент = %s\n", source->textArray[i]);
    }

    return 0;
}

int BinaryRecordind (struct Compiler* source)
{
    assert(source != NULL);
    assert(source->codeArray != NULL);

    FILE* file = fopen ("../assets/code2.bin", "wb");
    assert(file != nullptr);

    if (ValidationFile(file) != 0)
    {
        return 1;
    }

    size_t elements_written1 = fwrite (&source->position, sizeof (int), 1, file);
    if (elements_written1 != 1)
    {
        HANDLE_ERROR("Ошибка при записи в файл position");
    }

    size_t elements_written = fwrite (source->codeArray, sizeof (int), source->position, file);

    if (elements_written != source->position)
    {
        HANDLE_ERROR("Ошибка при записи в файл codeArray");
    }

    fclose (file);
    return 0;
}

int ValidationFile(FILE* file)
{
    if (file == NULL)
    {
        perror("Не удается открыть файл\n");
        return 1;
    }
    return 0;
}

void CommandPrintout (struct Compiler* source)
{
    assert(source != NULL);
    assert(source->codeArray != NULL);

    fprintf (LOG_FILE, "МАССИВ КОМАНД\n");
    for (int i = 0; i < source->position; i++)
    {
        fprintf (LOG_FILE, "%d - %d\n", i, source->codeArray[i]);
    }
}

void ResolveUnknownMarks (struct Compiler* source)
{
    int countWhile = 0;

    while (source->unknownMarks[countWhile] != -1)
    {
        int unknown = source->textArray[source->unknownMarks[countWhile]][1] - '0';
        countWhile++;
        source->codeArray[source->unknownMarks[countWhile]] = source->labels[unknown];

        countWhile++;
    }
}

void InitializationUnknownMarks (struct Compiler* source)
{
    assert(source != NULL);
    assert(source->codeArray != NULL);

    for (int i = 0; i < 10; i++)
    {
        source->unknownMarks[i] = -1;
    }
}

void InitializationLabels (struct Compiler* source)
{
    assert(source != NULL);
    assert(source->codeArray != NULL);

    for (int i = 0; i < MAX_SIZE_REG; i++)
    {
        source->labels[i] = -1;
    }
}

void EnsureCapacity(struct Compiler *source)
{
    assert(source != NULL);
    assert(source->codeArray != NULL);

    if (source->position >= source->capacity)
    {
        source->capacity *= 2;
        int *newCodeArray = (int *)realloc(source->codeArray, source->capacity * sizeof(int));
        if (newCodeArray == NULL) {
            fprintf(stderr, "Ошибка: Не удалось выделить память для codeArray\n");
            exit(1);
        }
        source->codeArray = newCodeArray;
    }
}

void PrintTextArray(struct Compiler *source)
{
    for (int i = 0; i < source->words; i++)
    {
        fprintf(LOG_FILE, "%d = %s and %d\n", i, source->textArray[i], strlen(source->textArray[i]));
    }
}

void UpdateLabelPosition(struct Compiler *source, int* cnt)
{
    if (source->labels[source->textArray[*cnt][1] - '0'] == -1)
    {
        source->labels[source->textArray[*cnt][1] - '0'] = source->position;
    }
    (*cnt)++;
}

void AddTwoElementsInCodeArray(struct Compiler *source, int *cnt, int cmd, int argCode, FILE *outputfile)
{
    fprintf(outputfile, "%d %d\n", cmd, argCode);
    source->codeArray[source->position++] = cmd;
    source->codeArray[source->position++] = argCode;
    *cnt += 2;
}
