#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "recurs.h"
#include "read_from_file_in_buffer.h"
#include "differentiation.h"
#include "print_tree.h"

char* s = NULL;
int p = 0;
Variables arrayVar;
int size = 0;
Node* bufferS;
int pBuf = 0;

int BuildTREEEE(char* filename, Differ* differ_before)
{
    FILE* file = fopen(filename, "r");
    if (file == nullptr)
    {
        printf("Ошибка при открытии файла.\n");
    }

    Buffer array = {NULL, 0};

    GetFileSize(file, &array);
    array.buffer = (char*)calloc(array.size + 1, sizeof(char));
    ReadFileInBuffer(file, &array);
    FillText(&array);
    s = array.buffer;
    p = 0;

    size = array.size;
    arrayVar.capacity = CAPACITY;
    arrayVar.size = ZERO;
    arrayVar.data = (VariableData*)calloc(arrayVar.capacity, sizeof(VariableData));

    bufferS = (Node*)calloc(1000, sizeof(Node));
    TokenInizial();
    printf("a = %d\n", pBuf);
    for (int i = 0; i < pBuf; i++)
    {
        printf("%d: type = %d, values = %d\n", i, bufferS[i].type, bufferS[i].value);
    }
    pBuf = 0;
    Node* node = GetG();
    printf("a = %d\n", pBuf);
    differ_before->tree->rootTree = node;
    differ_before->variables = &arrayVar;
    GenerateImage(differ_before);
//     PrintTreeToFileWithoutBrackets(node, &arrayVar);
//
//     double result = EvaluateExpression(differ_before->tree->rootTree, differ_before->variables);
//     printf("%lg\n", result);
    return 0;
}



Node* GetG()
{
    pBuf = 0;
    Node* node = GetE();
    if (bufferS[pBuf].value != END) printf("ошибкаG\n");
    printf("type = %d, values = %d\n", bufferS[pBuf].type, bufferS[pBuf].value);
    return node;
}

Node* GetE()
{
    Node* node = GetT();
    while (bufferS[pBuf].value == ADD || bufferS[pBuf].value == SUB) {
        int old_p = pBuf;
        pBuf++;
        Node* node2 = GetT();

        bufferS[old_p].left = node;
        bufferS[old_p].right = node2;
        node = &bufferS[old_p];
    }
    return node;
}

Node* GetT()
{
    Node* node = GetW();

    while (bufferS[pBuf].value == MUL || bufferS[pBuf].value == DIV || bufferS[pBuf].value == POW) {
        int old_p = pBuf;
        pBuf++;
        Node* node2 = GetW();

        bufferS[old_p].left = node;
        bufferS[old_p].right = node2;
        node = &bufferS[old_p];
    }
    return node;
}

Node* GetW()
{
    Node* node = GetP();
    while (bufferS[pBuf].value == POW) {
        int old_p = pBuf;
        pBuf++;
        Node* node2 = GetP();

        bufferS[old_p].left = node;
        bufferS[old_p].right = node2;
        node = &bufferS[old_p];
    }
    return node;
}

Node* GetP()
{
    Node* node = NULL;

    if (bufferS[pBuf].value == SCOBKA) {
        pBuf++;
        node = GetE();
        if (bufferS[pBuf].value != SCOBKA) printf("ошибкаP\n");
        pBuf++;
    }
    else if (bufferS[pBuf].type == OPERAT || bufferS[pBuf].type == VAR) {
        node = GetO();
        if (node->type == OPERAT)
            node->left = GetP();
    }
    else {
        node = GetN();
    }
    return node;
}

Node* GetN()
{
    return &bufferS[pBuf++];
}

#define SET_OPERATOR(op, OP, ...)                           \
    if (pBuf < 1000 && 5 <= bufferS[pBuf].value && bufferS[pBuf].value <= 13 && OP == bufferS[pBuf].value) \
    {                                                       \
        return &bufferS[pBuf++];                                        \
    }


Node* GetO()
{
    #include "operation.dsl"
    #undef SET_OPERATOR

    return &bufferS[pBuf++];
}

int GetNUM()
{
    int val = 0;
    while ('0' <= s[p] && s[p] <= '9') {
        val = val * 10 + s[p] - '0';
        p++;
    }
    return val;
}

#define SET_OPERATOR(op, OP, ...)                           \
    if (5 <= OP && OP <= 13 && strcmp(token, op) == 0)      \
    {                                                       \
        return OP;                                          \
    }

int GetOPERAT()
{
    char token[OP_LEN] = "";
    int i = 0;
    while ('a' <= s[p] && s[p] <= 'z') {
        token[i] = s[p];
        i++;
        p++;
    }

    #include "operation.dsl"
    #undef SET_OPERATOR

    int valVAR = AddVariable(&arrayVar, token, 0);
    InitializeNode(&bufferS[pBuf++], VAR, valVAR, NULL, NULL, NULL);
    return -1000;
}

void TokenInizial()
{
    int num = 0;
    while(p <= size)
    {
        switch(s[p])
        {
            case '(':
                InitializeNode(&bufferS[pBuf++], OPERAT, SCOBKA, NULL, NULL, NULL);
                p++;
                break;
            case ')':
                InitializeNode(&bufferS[pBuf++], OPERAT, SCOBKA, NULL, NULL, NULL);
                p++;
                break;
            case '\0':
                InitializeNode(&bufferS[pBuf++], OPERAT, END, NULL, NULL, NULL);
                p++;
                break;
            case '\n':
                InitializeNode(&bufferS[pBuf++], OPERAT, SLASH, NULL, NULL, NULL);
                p++;
                break;
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
                num = GetNUM();
                InitializeNode(&bufferS[pBuf++], INT, num, NULL, NULL, NULL);
                break;
            case '+':
                InitializeNode(&bufferS[pBuf++], OPERAT, ADD, NULL, NULL, NULL);
                p++;
                break;
            case '-':
                InitializeNode(&bufferS[pBuf++], OPERAT, SUB, NULL, NULL, NULL);
                p++;
                break;
            case '*':
                InitializeNode(&bufferS[pBuf++], OPERAT, MUL, NULL, NULL, NULL);
                p++;
                break;
            case '^':
                InitializeNode(&bufferS[pBuf++], OPERAT, POW, NULL, NULL, NULL);
                p++;
                break;
            case '/':
                InitializeNode(&bufferS[pBuf++], OPERAT, DIV, NULL, NULL, NULL);
                p++;
                break;
            default:
            {
                if ('a' <= s[p] && s[p] <= 'z') {
                    int valOP = GetOPERAT();
                    if (valOP != -1000) {
                        InitializeNode(&bufferS[pBuf++], OPERAT, valOP, NULL, NULL, NULL);
                        break;
                    }
                }
                else p++;
                break;
            }
        }
    }
}
