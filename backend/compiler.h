#ifndef COMPILER_H
#define COMPILER_H

#include "../common/const.h"
#include "../common/differentiation.h"

const int INITIAL_CAPACITY = 1;

struct Compiler {
    char file[MAXLEN];
    char *buffer;
    char **textArray;
    int *codeArray;
    int size;
    int words;
    int unknownMarks[MAX_SIZE_REG];
    int cntUnknownMarks;
    int labels[MAX_SIZE_REG];
    int position;
    int capacity;
};

#define DEF_CMD(name, num, ...)                             \
        CMD_##name = num,

#define CHECK_REGISTER(register_str, register_val)          \
if (strcmp(source->textArray[i], register_str) == 0)        \
    return register_val

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
            i = codeArray[i+1] - 1;                         \
        }                                                   \
    })

#define HANDLE_ERROR(message)                               \
    perror(message);                                        \
    fprintf(LOG_FILE, message);                             \
    fclose(file);                                           \
    return 1;


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

void GenerateCodeFromInput(struct Compiler *source, FILE* outputfile);
int ReturnArg (struct Compiler *source, int i);
int ArgCode(struct Compiler *source, int* RegisterFlag, int cnt);
void ResolveUnknownMarks (struct Compiler* source);
//void ReverseCompiler (FILE* inputFile, FILE* outputFile);

void CommandPrintout (struct Compiler *source);
void PrintTextArray(struct Compiler *source);

void UpdateLabelPosition(struct Compiler *source, int* cnt);
void AddTwoElementsInCodeArray(struct Compiler *source, int *cnt, int cmd, int argCode, FILE *outputfile);

int BinaryRecordind (struct Compiler *source);

void InitializationLabels (struct Compiler* source);
void InitializationUnknownMarks (struct Compiler* source);

int ValidationFile(FILE* file);
#endif
