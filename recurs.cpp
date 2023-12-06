#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "recurs.h"
#include "read_from_file_in_buffer.h"
#include "differentiation.h"
#include "print_tree.h"

#define BUF_V tokens[pBuf].value
#define BUF_T tokens[pBuf].type
#define BUF_L tokens[old_p].left
#define BUF_R tokenss[old_p].right
#define BUF tokens[old_p]

int LINE_COUNT = 0;

char* s = NULL;
int p = 0;
Variables arrayVar;
int size = 0;
int pBuf = 0;

int BuildTREEEE(char* filename, Differ* differ_before)
{
    FILE* file = fopen(filename, "r");
    if (file == nullptr) {
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

    Node* tokens = (Node*)calloc(1000, sizeof(Node));
    TokenInizial(tokens);
    pBuf = 0;
    Node* node = GetG(tokens);

    differ_before->tree->rootTree = node;
    differ_before->variables = &arrayVar;
    GenerateImage(differ_before);
    return 0;
}

Node* GetG(Node* tokens)
{
    pBuf = 0;
    Node* node = GetE(tokens);

    if (BUF_V != END) printf("ошибкаG\n");
    return node;
}

Node* GetE(Node* tokens)
{
    Node* node = GetT(tokens);

    while (BUF_V == ADD || BUF_V == SUB) {
        int old_p = pBuf;
        pBuf++;
        Node* node2 = GetT(tokens);

        BUF_L = node;
        BUF_R = node2;
        node = &BUF;
    }
    return node;
}

Node* GetT(Node* tokens)
{
    Node* node = GetW(tokens);

    while (BUF_V == MUL || BUF_V == DIV || BUF_V == POW) {
        int old_p = pBuf;
        pBuf++;
        Node* node2 = GetW(tokens);

        BUF_L = node;
        BUF_R = node2;
        node = &BUF;
    }
    return node;
}

Node* GetW(Node* tokens)
{
    Node* node = GetP(tokens);

    while (BUF_V == POW) {
        int old_p = pBuf;
        pBuf++;
        Node* node2 = GetP(tokens);

        BUF_L = node;
        BUF_R = node2;
        node = &BUF;
    }
    return node;
}

Node* GetP(Node* tokens)
{
    Node* node = NULL;

    if (BUF_V == SCOBKA) {
        pBuf++;
        node = GetE(tokens);
        if (BUF_V != SCOBKA) printf("ошибкаP\n");
        pBuf++;
    }
    else if (BUF_T == OPERAT || BUF_T == VAR) {
        node = GetO(tokens);
        if (node->type == OPERAT)
            node->left = GetP(tokens);
    }
    else {
        node = GetN(tokens);
    }
    return node;
}

Node* GetN(Node* tokens)
{
    return &tokens[pBuf++];
}

#define SET_OPERATOR(op, OP, ...)                           \
    if (pBuf < NO_OP && SIN <= BUF_V && BUF_V <= ARCCOT && OP == BUF_V) \
    {                                                       \
        return &tokens[pBuf++];                                        \
    }


Node* GetO(Node* tokens)
{
    #include "operation.dsl"
    #undef SET_OPERATOR

    return &tokens[pBuf++];
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
    if (SIN <= OP && OP <= ARCCOT && strcmp(token, op) == 0)      \
    {                                                       \
        return OP;                                          \
    }

#define INIT(type, val) InitializeNode(&tokens[pBuf++], type, val, NULL, NULL, NULL);

int GetOPERAT(Node* tokens)
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
    InitializeNode(&tokens[pBuf++], VAR, valVAR, NULL, NULL, NULL);
    return NO_OP;
}

void TokenInizial(Node* tokens)
{
    int num = 0;
    while(p <= size)
    {
        switch(s[p])
        {
            case '(':
                INIT(OPERAT, SCOBKA);
                p++;
                break;
            case ')':
                INIT(OPERAT, SCOBKA);
                p++;
                break;
            case '\0':
                INIT(OPERAT, END);
                p++;
                break;
            case '\n':
                LINE_COUNT++;
                p++;
                break;
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
                num = GetNUM();
                INIT(INT, num);
                break;
            case '+':
                INIT(OPERAT, ADD);
                p++;
                break;
            case '-':
                INIT(OPERAT, SUB);
                p++;
                break;
            case '*':
                INIT(OPERAT, MUL);
                p++;
                break;
            case '^':
                INIT(OPERAT, POW);
                p++;
                break;
            case '/':
                INIT(OPERAT, DIV);
                p++;
                break;
            default:
            {
                if ('a' <= s[p] && s[p] <= 'z') {
                    int valOP = GetOPERAT(tokens);
                    if (valOP != NO_OP) {
                        INIT(OPERAT, valOP);
                        break;
                    }
                }
                else p++;
                break;
            }
        }
    }
}
