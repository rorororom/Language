#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "recurs.h"
#include "read_from_file_in_buffer.h"
#include "differentiation.h"
#include "print_tree.h"
#include "log_funcs.h"

#define BUF_V tokens[pBuf].value
#define BUF_T tokens[pBuf].type
#define BUF_L tokens[old_p].left
#define BUF_R tokens[old_p].right
#define BUF tokens[old_p]

int LINE_COUNT = 0;

char* s = NULL;
int p = 0;
Variables arrayVar;
int size = 0;
int pBuf = 0;

void AddNewId(int type, int val, char* name)
{
    arrayVar.data[arrayVar.size].type = type;
    arrayVar.data[arrayVar.size].value = val;
    arrayVar.data[arrayVar.size].name = name;
    arrayVar.size++;
}

void CreatId()
{
    AddNewId(OPERAT, SIN, "sin");
    AddNewId(OPERAT, COS, "cos");
    AddNewId(OPERAT, TAN, "tg");
    AddNewId(OPERAT, COT, "ctg");
    AddNewId(OPERAT, LN, "ln");
    AddNewId(OPERAT, ARCSIN, "arcsin");
    AddNewId(OPERAT, ARCCOS, "arccos");
    AddNewId(OPERAT, ARCCOT, "arctg");
    AddNewId(OPERAT, ARCTAN, "arcctg");
    AddNewId(OPERAT, IF, "if");
    AddNewId(OPERAT, EQ, "=");
}

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
    CreatId();

    for (int i = 0; i < arrayVar.size; i++)
    {
        printf("%s\n", arrayVar.data[i].name);
    }

    Node* tokens = (Node*)calloc(1000, sizeof(Node));
    printf("AAAAAA\n");
    for (int i = 0; i < array.size; i++)
    {
        printf("%d: %d\n", i, int(s[i]));
    }
    TokenInizial(tokens);
    for (int i = 0; i < pBuf; i++)
    {
        printf("%d: type = %d, values = %d\n", i, tokens[i].type, tokens[i].value);
    }
    pBuf = 0;
    Node* node = GetG(tokens);

    differ_before->tree->rootTree = node;
    differ_before->variables = &arrayVar;
    GenerateImage(differ_before);
    return 0;
}


Node* GetG(Node* tokens) {
    Node* node = NULL;
    fprintf(LOG_FILE, "я нахожусь в GetG, pbuf = %d, token = %d\n", pBuf, tokens[pBuf]);

    if (BUF_V == IF) {
        fprintf(LOG_FILE, "я зашел в условие == If, pbuf = %d, token = %d\n", pBuf, tokens[pBuf]);
        node = GetIf(tokens);
    } else if (BUF_V == WHILE) {
        fprintf(LOG_FILE, "я зашел в условие == While, pbuf = %d, token = %d\n", pBuf, tokens[pBuf]);
        node = GetWhile(tokens);
    } else {
        fprintf(LOG_FILE, "я зашел в условие ELSE, pbuf = %d, token = %d\n", pBuf, tokens[pBuf]);
        node = GetA(tokens);
    }

    if (BUF_V != END) printf("ошибкаG\n");
    return node;
}

Node* GetIf(Node* tokens) {
    Node* ifNode = &tokens[pBuf++];
    if (BUF_V != SCOBKA) {
        printf("Ошибка: ожидается '}' после блока While\n");
    }
    pBuf++;
    Node* condition = GetA(tokens);
    if (BUF_V != SCOBKA) {
        printf("Ошибка: ожидается '}' после блока While\n");
    }
    pBuf++;
    if (BUF_V != OPEN_BRACE) {
        printf("Ошибка: ожидается '{' после условия IF\n");
        return NULL;
    }

    pBuf++;  // Пропустить '{'

    ifNode->left = condition;
    ifNode->right = GetBody(tokens);

    return ifNode;
}

Node* GetBody(Node* tokens)
{
    CREAT_NODE(whileBodyNode);
    InitializeNode(whileBodyNode, OPERAT, SEMICOLON, NULL, NULL, NULL);

    Node* currentNode = whileBodyNode;

    while (BUF_V != CLOSE_BRACE && BUF_V != END) {
        if (BUF_V == WHILE) {
            Node* nestedWhile = GetWhile(tokens);
            currentNode->left = nestedWhile;
        } else if (BUF_V == IF) {
            Node* nestedIf = GetIf(tokens);
            currentNode->left = nestedIf;
        } else {
            Node* statement = GetG(tokens);
            currentNode->left = statement;
        }

        pBuf++;

        if (BUF_V != CLOSE_BRACE && BUF_V != END) {
            CREAT_NODE(nextStatement);
            InitializeNode(nextStatement, OPERAT, SEMICOLON, NULL, NULL, NULL);
            currentNode->right = nextStatement;
            currentNode = nextStatement;
        }
    }
    return whileBodyNode;
}


Node* GetWhile(Node* tokens) {
    fprintf(LOG_FILE, "я нахожусь в GetWhile, pbuf = %d, token = %d\n", pBuf, tokens[pBuf]);
    Node* whileNode = &tokens[pBuf++];
    fprintf(LOG_FILE, "я собираюсь вызвать GetE, pbuf = %d, token = %d\n", pBuf, tokens[pBuf]);

    if (BUF_V != SCOBKA) {
        printf("Ошибка: ожидается '(' после блока While\n");
    }
    pBuf++;

    Node* condition = GetA(tokens);

    if (BUF_V != SCOBKA) {
        printf("Ошибка: ожидается ')' после блока While\n");
    }
    pBuf++;

    fprintf(LOG_FILE, "я после вызова GetE, pbuf = %d, token = %d\n", pBuf, tokens[pBuf]);

    if (BUF_V != OPEN_BRACE) {
        printf("Ошибка: ожидается '{' после условия While\n");
        return NULL;
    }

    pBuf++;  // Пропустить '{'

    whileNode->left = condition;
    whileNode->right = GetBody(tokens);

    return whileNode;
}


Node* GetId(Node* tokens)
{
    if (tokens[pBuf].type == VAR) {
        return &tokens[pBuf++];
    }
    else return NULL;
}

Node* GetA(Node* tokens)
{
    int old_p = pBuf;
    Node* node = GetId(tokens);
    if (node == NULL)
    {
        node = GetE(tokens);
    }
    else if (tokens[pBuf].value != EQ && tokens[pBuf].value != MORE && tokens[pBuf].value != LESS)
    {
        pBuf = old_p;
        node = GetE(tokens);
    }
    else {
        Node* main;
        if ((tokens[pBuf].value == EQ   ||
            tokens[pBuf].value == MORE  ||
            tokens[pBuf].value == LESS) &&
            tokens[pBuf].type == OPERAT)
        {
            main = &tokens[pBuf];
            pBuf++;
        }
        Node* node2 = GetE(tokens);
        Node* nodeL = node;
        node = main;
        node->left = nodeL;
        node->right = node2;
    }
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
    if (BUF_V == EQ) printf("ADFGBNKEPODVS\n"); \
    if (pBuf < NO_OP && SIN <= BUF_V && BUF_V <= EQ && OP == BUF_V) \
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
    if (SIN <= OP && OP <=LESS && strcmp(token, op) == 0)      \
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
            case '{':
                INIT(OPERAT, OPEN_BRACE);
                p++;
                break;
            case '}':
                INIT(OPERAT, CLOSE_BRACE);
                p++;
                break;
            case '\0':
                if (p < size) {
                    p++;
                    break;
                }
                else {
                    INIT(OPERAT, END);
                    p++;
                    break;
                }
            // case '\n':
            //     LINE_COUNT++;
            //     p++;
            //     printf("asdf\n");
            //     if (s[p] == '\0') p++;
            //     break;
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
                num = GetNUM();
                INIT(INT, num);
                break;
            case '+':
                INIT(OPERAT, ADD);
                p++;
                break;
            case '>':
                INIT(OPERAT, MORE);
                p++;
                break;
            case '<':
                INIT(OPERAT, LESS);
                p++;
                break;
            case '-':
                INIT(OPERAT, SUB);
                p++;
                break;
            case ';':
                INIT(OPERAT, SEMICOLON);
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
            case '=':
                INIT(OPERAT, EQ);
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
