// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
//
// #include "recurs.h"
// #include "read_from_file_in_buffer.h"
// #include "differentiation.h"
// #include "print_tree.h"
//
// char* s = NULL;
// int p = 0;
// Variables arrayVar;
// int size = 0;
//
// Node* GetG(const char* str)
// {
//     s = strdup(str);
//     p = 0;
//     Node* node = GetE();
//     if (s[p] != '\0') printf("ошибкаG\n");
//     return node;
// }
//
// Node* GetE()
// {
//
//     Node* node = GetT();
//     while (s[p] == '+' || s[p] == '-')
//     {
//         int old_p = p;
//         char op = s[p];
//         p++;
//         Node* node2 = GetT();
//
//         CREAT_NODE(operationNode);
//         switch (s[old_p]) {
//             case '+':
//                 InitializeNode(operationNode, OPERAT, ADD, NULL, NULL, NULL);
//                 break;
//             case '-':
//                 InitializeNode(operationNode, OPERAT, SUB, NULL, NULL, NULL);
//                 break;
//             default:
//                 printf("ошибкаE\n");
//         }
//
//         operationNode->left = node;
//         operationNode->right = node2;
//
//         node = operationNode;
//     }
//     return node;
// }
//
// Node* GetT()
// {
//     Node* node = GetP();
//
//     while (s[p] == '*' || s[p] == '/' || s[p] == '^')
//     {
//         int old_p = p;
//         char op = s[p];
//         p++;
//         Node* node2 = GetP();
//
//         CREAT_NODE(operationNode);
//         switch (s[old_p]) {
//             case '*':
//                 InitializeNode(operationNode, OPERAT, MUL, NULL, NULL, NULL);
//                 break;
//             case '/':
//                 InitializeNode(operationNode, OPERAT, DIV, NULL, NULL, NULL);
//                 break;
//             case '^':
//                 InitializeNode(operationNode, OPERAT, POW, NULL, NULL, NULL);
//                 break;
//             default:
//                 printf("ошибкаP\n");
//         }
//
//         operationNode->left = node;
//         operationNode->right = node2;
//         node = operationNode;
//     }
//     return node;
// }
//
// Node* GetP()
// {
//     Node* node = NULL;
//
//     if (s[p] == '(')
//     {
//         p++;
//         node = GetE();
//         while (s[p] == ' ') {
//             p++;
//         }
//         if (s[p] != ')') printf("ошибкаP\n");
//         p++;
//     }
//     else if ('a' <= s[p] && s[p] <= 'z')
//     {
//         node = GetO();
//         if (node->type == OPERAT)
//             node->left = GetP();
//     }
//     else
//     {
//         node = GetN();
//     }
//     return node;
// }
//
// Node* GetN()
// {
//     int old_p = p;
//     int val = 0;
//     while ('0' <= s[p] && s[p] <= '9')
//     {
//         val = val * 10 + s[p] - '0';
//         p++;
//         if (p <= old_p) printf("ошибкаN\n");
//     }
//
//     CREAT_NODE(node);
//     InitializeNode(node, INT, val, NULL, NULL, NULL);
//
//     return node;
// }
//
// #define SET_OPERATOR(op, OP, ...)                           \
//     if (5 <= OP && OP <= 13 && strcmp(token, op) == 0)      \
//     {                                                       \
//         InitializeNode(node, OPERAT, OP, NULL, NULL, NULL); \
//         return node;                                        \
//     }
//
// Node* GetO()
// {
//     char token[OP_LEN] = "";
//     int i = 0;
//     while ('a' <= s[p] && s[p] <= 'z')
//     {
//         token[i] = s[p];
//         i++;
//         p++;
//     }
//
//     CREAT_NODE(node);
//
//     #include "operation.dsl"
//     #undef SET_OPERATOR
//
//     int val = AddVariable(&arrayVar, token, 0);
//     InitializeNode(node, VAR, val, NULL, NULL, NULL);
//     return node;
// }
//
// void BuildTREEEE(char* filename)
// {
//     FILE* file = fopen(filename, "r");
//     if (file == nullptr)
//     {
//         printf("Ошибка при открытии файла.\n");
//     }
//
//     Buffer array = {NULL, 0};
//
//     GetFileSize(file, &array);
//     array.buffer = (char*)calloc(array.size + 1, sizeof(char));
//     ReadFileInBuffer(file, &array);
//     FillText(&array);
//
//     size = array.size;
//     arrayVar.capacity = CAPACITY;
//     arrayVar.size = ZERO;
//     arrayVar.data = (VariableData*)calloc(arrayVar.capacity, sizeof(VariableData));
//
//     Node* node = GetG(array.buffer);
//     Differ differ_before = {};
//     Tree tree = {};
//     Variables arrayV = {};
//     differ_before.tree = &tree;
//     differ_before.variables = &arrayV;
//
//     CtorRootAndVariebles(&differ_before);
//     differ_before.tree->rootTree = node;
//     differ_before.variables = &arrayVar;
//     printf("AAAA\n");
//     GenerateImage(&differ_before);
//     printf("AAAA\n");
//     PrintTreeToFileWithoutBrackets(node, &arrayVar);
//
//     double result = EvaluateExpression(differ_before.tree->rootTree, differ_before.variables);
//     printf("%lg\n", result);
// }

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
        int op = bufferS[pBuf].value;
        pBuf++;
        Node* node2 = GetT();

        CREAT_NODE(operationNode);
        switch (op) {
            case ADD:
                InitializeNode(operationNode, OPERAT, ADD, NULL, NULL, NULL);
                break;
            case SUB:
                InitializeNode(operationNode, OPERAT, SUB, NULL, NULL, NULL);
                break;
            default:
                printf("ошибкаE\n");
                break;
        }

        operationNode->left = node;
        operationNode->right = node2;

        node = operationNode;
    }
    return node;
}

Node* GetT()
{
    Node* node = GetW();

    while (bufferS[pBuf].value == MUL || bufferS[pBuf].value == DIV || bufferS[pBuf].value == POW) {
        int op = bufferS[pBuf].value;
        pBuf++;
        Node* node2 = GetW();

        CREAT_NODE(operationNode);
        switch (op) {
            case MUL:
                InitializeNode(operationNode, OPERAT, MUL, NULL, NULL, NULL);
                break;
            case DIV:
                InitializeNode(operationNode, OPERAT, DIV, NULL, NULL, NULL);
                break;
            case POW:
                InitializeNode(operationNode, OPERAT, POW, NULL, NULL, NULL);
                break;
            default:
                printf("ошибкаP\n");
        }

        operationNode->left = node;
        operationNode->right = node2;
        node = operationNode;
    }
    return node;
}

Node* GetW()
{
    Node* node = GetP();
    while (bufferS[pBuf].value == POW) {
        int op = bufferS[pBuf].value;
        pBuf++;
        Node* node2 = GetP();

        CREAT_NODE(operationNode);
        switch (op) {
            case POW:
                InitializeNode(operationNode, OPERAT, POW, NULL, NULL, NULL);
                break;
            default:
                printf("ошибкаW\n");
        }

        operationNode->left = node;
        operationNode->right = node2;
        node = operationNode;
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
        //pBuf++;
        node = GetO();
        if (node->type == OPERAT)
            node->left = GetP();
    }
    else {
        //pBuf++;
        node = GetN();
    }
    return node;
}

Node* GetN()
{
    CREAT_NODE(node);
    InitializeNode(node, INT, bufferS[pBuf].value, NULL, NULL, NULL);
    pBuf++;

    return node;
}

#define SET_OPERATOR(op, OP, ...)                           \
    if (pBuf < 1000 && 5 <= bufferS[pBuf].value && bufferS[pBuf].value <= 13 && OP == bufferS[pBuf].value) \
    {                                                       \
        InitializeNode(node, OPERAT, bufferS[pBuf].value, NULL, NULL, NULL); \
        pBuf++;                                             \
        return node;                                        \
    }


Node* GetO()
{
    CREAT_NODE(node);

    #include "operation.dsl"
    #undef SET_OPERATOR

    InitializeNode(node, VAR, bufferS[pBuf].value, NULL, NULL, NULL);
    pBuf++;
    return node;
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
