#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>

#include "read_from_file_in_buffer.h"
#include "differentiation.h"
#include "log_funcs.h"
#include "print_tree.h"

int indexInBufer = 0;

#define N_TYPE (*nowNode)->type
#define N_LEFT (*nowNode)->left
#define N_RIGHT (*nowNode)->right
#define N_L_TYPE N_LEFT->type
#define N_L_VALUE N_LEFT->value
#define N_R_TYPE N_RIGHT->type
#define N_R_VALUE ((*nowNode)->right->value)
#define N_VALUE ((*nowNode)->value)
#define NODE_INT(value) NewNode(INT, value, NULL, NULL)
#define N_R_PARENT (*nowNode)->right->parent
#define N_L_PARENT (*nowNode)->left->parent
#define N_PARENT (*nowNode)->parent

#define NN_TYPE nowNode->type
#define NN_LEFT nowNode->left
#define NN_RIGHT nowNode->right
#define NN_L_TYPE NN_LEFT->type
#define NN_L_VALUE NN_LEFT->value
#define NN_R_TYPE NN_RIGHT->type

#define _ADD(left, right) NewNode(OPERAT, ADD, left, right)
#define _MUL(left, right) NewNode(OPERAT, MUL, left, right)
#define _SUB(left, right) NewNode(OPERAT, SUB, left, right)
#define _DIV(left, right) NewNode(OPERAT, DIV, left, right)
#define _IN(left, right) NewNode(OPERAT, IN, left, right)
#define _POW(base, exponent) NewNode(OPERAT, POW, base, exponent)
#define _COS(expr) NewNode(OPERAT, COS, expr, NULL)
#define _SIN(expr) NewNode(OPERAT, SIN, expr, NULL)
#define COPYL Copy(nowNode->left)
#define COPYR Copy(nowNode->right)
#define DIFL  Dif(nowNode->left)
#define DIFR  Dif(nowNode->right)

//static void InitializeNode(Node* node, int type, int value, Node* left, Node* right, Node* parent);
static void PrintNodeDump(FILE* dotFile, Node* node, Variables* arrayVar, const char* fillColor);
static void DestroyNode(Node* node);

void CtorRootAndVariebles(Differ* differ)
{
    assert(differ);

    CREAT_NODE(newNode);
    InitializeNode(newNode, ZERO, ZERO, NULL, NULL, NULL);
    differ->tree->rootTree = newNode;

    differ->variables->capacity = CAPACITY;
    differ->variables->size = ZERO;
    differ->variables->data = (VariableData*)calloc(differ->variables->capacity, sizeof(VariableData));
}

void TreeAndVarieblesDtor(Differ* differ)
{
    assert(differ);
    assert(differ->tree);
    assert(differ->variables);

    DestroyNode(differ->tree->rootTree);
    differ->tree->rootTree = nullptr;

    for (size_t i = 0; i < differ->variables->capacity; ++i)
    {
        if (differ->variables->data->name) free(differ->variables->data->name);

        differ->variables->data[i].name = nullptr;
        differ->variables->data[i].value = ZERO;
    }

    free(differ->variables->data);
    differ->variables->data     = nullptr;
    differ->variables->size     = ZERO;
    differ->variables->capacity = ZERO;
}

static void DestroyNode(Node* node)
{
    assert(node);

    if (node->left != nullptr) {
        DestroyNode(node->left);
    }
    if (node->right != nullptr) {
        DestroyNode(node->right);
    }

    free(node);
}

void BuildTreeFromFile(const char* filename, Differ* differ)
{
    assert(differ);
    assert(filename);

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

    CREAT_NODE(newNode);
    newNode = ReadFromBufferInf(&array, NULL, differ->variables);
    indexInBufer = 0;
    differ->tree->rootTree = newNode;

    if (differ->tree->rootTree == NULL)
    {
        printf("Файл пустой или содержит только пробелы. Дерево не заполнено.\n");
    }
}


void InitializeNode(Node* node, int type, int value, Node* left, Node* right, Node* parent)
{
    assert(node);

    node->value = value;
    node->type = type;
    node->flagDirection = ZERO;
    node->left = left;
    node->right = right;
    node->parent = parent;
}

static void SetNumericValue(Node* node, const char* value)
{
    int number = atoi(value);
    node->type = INT;
    node->value = number;
}

static void SetNegativeNumericValue(Node* node, const char* value)
{
    int number = atoi(value + 1);
    node->type = INT;
    node->value = -1 * number;
}

static void SetEConstantValue(Node* node)
{
    node->type = CONST;
    node->value = E_CONST;
}

static void SetVariableValue(Node* node, const char* value, Variables* arrayVar)
{
    double answer = GetVariableIdByName(arrayVar, value);
    if (answer == -1)
    {
        node->type = VAR;
        node->value = AddVariable(arrayVar, value, 0);
    }
    else
    {
        node->type = VAR;
        node->value = answer;
    }
}

#define SET_OPERATOR(op, OP, ...) else if (strcmp(value, op) == 0) { node->value = OP; node->type = OPERAT; }

void SetNodeTypeAndValue(Node* node, char* value, Variables* arrayVar)
{
    assert(node);
    assert(arrayVar);
    assert(value);

    if (('0' <= value[0] && value[0] <= '9'))
        SetNumericValue(node, value);
    else if (value[0] == '-')
        SetNegativeNumericValue(node, value);
    else if (value[0] == 'e')
        SetEConstantValue(node);

    #include "operation.dsl"
    #undef SET_OPERATOR

    else
        SetVariableValue(node, value, arrayVar);
}

int AddVariable(Variables* arrayVar, const char*  variableName, const double variableValue)
{
    assert(arrayVar);
    assert(variableName);

    arrayVar->data[arrayVar->size].name  = strdup(variableName);

    if (arrayVar->data[arrayVar->size].name == nullptr)
        return -1;

    arrayVar->data[arrayVar->size].value = variableValue;
    arrayVar->size++;

    return arrayVar->size - 1;
}

int GetVariableIdByName(Variables* arrayVar, const char* variableName)
{
    assert(arrayVar);
    assert(variableName);

    for (size_t i = 0; i < arrayVar->size; ++i)
    {
        if (strcmp(arrayVar->data[i].name, variableName) == 0)
            return (int)i;
    }

    return -1;
}

void GenerateImage(Differ* differ)
{
    assert(differ);
    assert(differ->tree);
    assert(differ->variables);

    FILE* dotFile = fopen("grapth.dot", "w");
    if (dotFile) {
        fprintf(dotFile, "digraph tree {\n");
        fprintf(dotFile, "\tnode [shape=Mrecord, style=filled,\
                            fillcolor=\"#bba6cd\", color=\"#552d7b\"];\n");

        PrintNodeDump(dotFile, differ->tree->rootTree, differ->variables, "#d5a1a7");

        fprintf(dotFile, "}\n");
        fclose(dotFile);
    }
    else {
        printf("Ошибка при открытии файла graph.dot\n");
    }

    CreateNewGraph();
}

static void PrintNodeDump(FILE* dotFile, Node* node, Variables* arrayVar, const char* fillColor)
{
    assert(dotFile);
    assert(node);
    assert(fillColor);

    if (node == NULL) return;

    if (node->type == INT) {
        fprintf(dotFile, "%d [shape=record, style=\"filled,rounded\", color=\"#552d7b\",\
                          fillcolor=\"%s\", fontsize=14, label=\" %d \"];\n",
                          node, fillColor, node->value);
    }
    else if (node->type == OPERAT || node->type == CONST) {
        char* operation = IssuesOperation(node);
        fprintf(dotFile, "%d [shape=record, style=\"filled,rounded\", color=\" #0000ff \",\
                          fillcolor=\"%s\", fontsize=14, label=\" %s \"];\n",
                          node, fillColor, operation);
    }
    else if(node->type == VAR) {
        char* value = strdup(arrayVar->data[int(node->value)].name);
        fprintf(dotFile, "%d [shape=record, style=\"filled,rounded\", color=\"#FF0000\",\
                          fillcolor=\"%s\", fontsize=14, label=\" %s \"];\n",
                          node, fillColor, value);
    }
    if (node->left != NULL) {
        fprintf(dotFile, "\t%d -> %d;\n", node, node->left);
        PrintNodeDump(dotFile, node->left, arrayVar, "#6495ed");
    }

    if (node->right != NULL){
        fprintf(dotFile, "\t%d -> %d;\n", node, node->right);
        PrintNodeDump(dotFile, node->right, arrayVar, "#bba6cd");
    }
}

#define SET_OPERATOR(op, OP, simbol, sim, ...) case OP: return sim;

char* IssuesOperation1(Node* node)
{
    if (node->type == CONST) return "e";
    switch(int(node->value))
    {
        #include "operation.dsl"
        #undef SET_OPERATOR
        default:
            return ".";
    }
}

#define SET_OPERATOR(op, OP, simbol, ...) case OP: return simbol;

char* IssuesOperation(Node* node)
{
    if (node->type == CONST) return "e";
    switch(int(node->value))
    {
        #include "operation.dsl"
        #undef SET_OPERATOR
        default:
            return ".";
    }
}

void GenerateGraphImage()
{
    char command[MAX_LEN] = "";
    sprintf(command, "dot -Tpng /Users/aleksandr/Desktop/differentiation/grapth.dot\
                     -o /Users/aleksandr/Desktop/differentiation/file.png");
    system(command);
}

#define SET_OPERATOR(op, OP, ...) case OP: return op;

char* FromOperationToWord(int operation)
{
    switch(operation)
    {
        #include "operation.dsl"
        #undef SET_OPERATOR
        default:
            return ".";
    }
}

char* GetWord(Buffer* array)
{
    assert(array);

    while (indexInBufer < array->size && isspace(array->buffer[indexInBufer])) {indexInBufer++;}
    int cnt = 0;
    while (indexInBufer + cnt < array->size && !isspace(array->buffer[indexInBufer + cnt])) {cnt++;}

    char* token = (char*)malloc(cnt + 1);
    memcpy(token, array->buffer + indexInBufer, cnt);
    token[cnt] = '\0';

    indexInBufer = indexInBufer + cnt;

    return token;
}

static int FastPower(int baseValue, int exponent)
{
    int result = 1;

    while (exponent > 0) {
        if (exponent % 2 == 1) {
            result *= baseValue;
            exponent--;
        }

        baseValue *= baseValue;
        exponent /= 2;
    }
    return result;
}

#define RAD_TO_DEG(angle) ((angle) * 180.0 / M_PI)
#define SET_OPERATOR(op, OP, simbol, sim, evaluate, ...) case OP: evaluate

double EvaluateExpression(Node* node, Variables* arrayVar)
{
    assert(arrayVar);
    if (node == NULL)
        return 0;
    if (node->type == INT)
        return node->value;
    else if (node->type ==VAR)
        return arrayVar->data[int(node->value)].value;

    double rightValue = 0;
    double leftValue = 0;
    if (5 <= int(node->value) && int(node->value) <= 13 || node->right == NULL) {
        leftValue = EvaluateExpression(node->left, arrayVar);
    }
    else {
        leftValue = EvaluateExpression(node->left, arrayVar);
        rightValue = EvaluateExpression(node->right, arrayVar);
    }

    switch (int(node->value)) {
        #include "operation.dsl"
        #undef SET_OPERATOR
        default:
            printf("Некорректная операция\n");
            return 0;
    }
}

int CheckingPriorityOperation(int operation)
{
    if (operation == ADD    || operation == SUB) return 0;
    if (operation == MUL    || operation == DIV    || operation == POW)         return 1;
    if (operation == SIN    || operation == COS    || operation == COT    ||
        operation == TAN    || operation == LN     || operation == ARCSIN ||
        operation == ARCCOS || operation == ARCTAN || operation == ARCCOT) return 2;
    else return ERROR_OP;
}

Node* ReadFromBufferInf(Buffer* array, Node* currentNode, Variables* arrayVar)
{
    char* token = GetWord(array);

    if (strcmp(token, "(") == 0) {
        CREAT_NODE(newNode);
        newNode->left = ReadFromBufferInf(array, newNode, arrayVar);

        token = GetWord(array);
        SetNodeTypeAndValue(newNode, token, arrayVar);

        newNode->parent = currentNode;

        newNode->right = ReadFromBufferInf(array, newNode, arrayVar);
        token = GetWord(array);
        return newNode;
    }
    else if (strcmp(token, "nil") == 0) {
        return NULL;
    }
}

#define SET_OPERATOR(op, OP, simbol, sim, evaluate, dif) case OP: dif

Node* Dif(Node* nowNode)
{
    if (nowNode->type == INT || nowNode->type == CONST)
        return NewNode(INT, 0, NULL, NULL);
    if (nowNode->type == VAR)
        return NewNode(INT, 1, NULL, NULL);

    switch(int(nowNode->value))
    {
        #include "operation.dsl"
        #undef SET_OPERATOR
        default:
            return NULL;
    }
}

Node* NewNode(int type, double value, Node* left, Node* right)
{
    CREAT_NODE(node);
    node->type = type;
    node->value = value;
    node->left = left;
    node->right = right;

    return node;
}

Node* Copy(Node* nowNode)
{
    if (nowNode == NULL) return NULL;

    CREAT_NODE(newNode);
    newNode->type = nowNode->type;
    newNode->value = nowNode->value;
    newNode->left = Copy(nowNode->left);
    newNode->right = Copy(nowNode->right);

    return newNode;
}

static void ReplaceNodeWithAnswer(Node** nowNode, int* changeCount,
                                  Variables* arrayVar, Tree* treeDif, Lines* text, int answer)
{
    Node* newNode = NewNode(INT, answer, NULL, NULL);
    newNode->parent = N_PARENT;
    *nowNode = newNode;
    (*changeCount)++;
    CreateNewGraph();
    PrintTreeLaTex("f'(x) = ", treeDif->rootTree, arrayVar, text);
}

static void ReplaceNodeWithZero(Node** nowNode, int* changeCount,
                                Variables* arrayVar, Tree* treeDif, Lines* text)
{
    Node* newNode = NewNode(INT, 0, NULL, NULL);
    newNode->parent = N_PARENT;
    *nowNode = newNode;
    (*changeCount)++;
    PrintTreeLaTex("f'(x) = ", treeDif->rootTree, arrayVar, text);
}

static void ReplaceNodeWithOne(Node** nowNode, int* changeCount,
                               Variables* arrayVar, Tree* treeDif, Lines* text)
{
    Node* newNode = NewNode(INT, 1, NULL, NULL);
    newNode->parent = N_PARENT;
    *nowNode = newNode;
    (*changeCount)++;
    PrintTreeLaTex("f'(x) = ", treeDif->rootTree, arrayVar, text);
}

static void ReplaceNodeWithRightChild(Node** nowNode, int* changeCount,
                                      Variables* arrayVar, Tree* treeDif, Lines* text)
{
    N_R_PARENT = N_L_PARENT;
    *nowNode = N_RIGHT;
    (*changeCount)++;
    PrintTreeLaTex("f'(x) = ", treeDif->rootTree, arrayVar, text);
}

static void ReplaceNodeWithLeftChild(Node** nowNode, int* changeCount,
                                     Variables* arrayVar, Tree* treeDif, Lines* text)
{
    N_L_PARENT = N_PARENT;
    *nowNode = N_LEFT;
    (*changeCount)++;
    PrintTreeLaTex("f'(x) = ", treeDif->rootTree, arrayVar, text);
}

static void ProcessRightNull(Node** nowNode, int* changeCount,
                             Variables* arrayVar, Tree* treeDif, Lines* text)
{
    if (N_L_VALUE == 0 && N_L_TYPE == INT && N_VALUE == MUL)
        ReplaceNodeWithZero(nowNode, changeCount, arrayVar, treeDif, text);

    else if (int(N_VALUE) == LN && N_L_TYPE == CONST)
        ReplaceNodeWithOne(nowNode, changeCount, arrayVar, treeDif, text);
}

static void ProcessIntNodes(Node** nowNode, int* changeCount,
                            Variables* arrayVar, Tree* treeDif, Lines* text)
{
    int answer = EvaluateExpression(*nowNode, arrayVar);
    ReplaceNodeWithAnswer(nowNode, changeCount, arrayVar, treeDif, text, answer);
}

static void ProcessLeftOrRightInt(Node** nowNode, int* changeCount,
                                  Variables* arrayVar, Tree* treeDif, Lines* text)
{
    if ((N_L_VALUE == 1  &&  N_L_TYPE == INT && (N_VALUE == MUL || N_VALUE == POW)) ||
        (N_L_VALUE == 0  && (N_VALUE  == ADD ||  N_VALUE == SUB)))
        ReplaceNodeWithRightChild(nowNode, changeCount, arrayVar, treeDif, text);

    else if (N_R_VALUE == 1 &&  N_R_TYPE == INT && (N_VALUE == MUL || N_VALUE == POW) ||
            (N_R_VALUE == 0 && (N_VALUE  == ADD ||  N_VALUE == SUB)))
        ReplaceNodeWithLeftChild(nowNode, changeCount, arrayVar, treeDif, text);

    else if (((N_R_VALUE == 0 && N_R_TYPE == INT)  ||
              (N_L_VALUE == 0 && N_L_TYPE == INT)) && N_VALUE == MUL)
        ReplaceNodeWithZero(nowNode, changeCount, arrayVar, treeDif, text);
}

static void ProcessPowAndLeftPow(Node** nowNode, int* changeCount,
                                 Variables* arrayVar, Tree* treeDif, Lines* text)
{
    int num1 = N_R_VALUE;
    int num2 = (*nowNode)->left->right->value;
    int answer = num1 * num2;
    ReplaceNodeWithAnswer(nowNode, changeCount, arrayVar, treeDif, text, answer);
}

void TransformationNode(Node** nowNode, int* changeCount, Variables* arrayVar, Tree* treeDif, Lines* text)
{
    if (*nowNode == NULL) return;

    if (N_TYPE == OPERAT) {
        if (N_RIGHT == NULL) {
            ProcessRightNull(nowNode, changeCount, arrayVar, treeDif, text);
            return;
        }

        if (N_LEFT && N_RIGHT && (N_L_TYPE == INT && N_R_TYPE == INT))
            ProcessIntNodes(nowNode, changeCount, arrayVar, treeDif, text);

        if (N_LEFT && N_RIGHT && (N_L_TYPE == INT || N_R_TYPE == INT))
            ProcessLeftOrRightInt(nowNode, changeCount, arrayVar, treeDif, text);

        if (N_TYPE == POW && N_L_VALUE == POW && N_L_TYPE == OPERAT)
            ProcessPowAndLeftPow(nowNode, changeCount, arrayVar, treeDif, text);
    }

    TransformationNode(&N_LEFT, changeCount, arrayVar, treeDif, text);
    TransformationNode(&N_RIGHT, changeCount, arrayVar, treeDif, text);
}

static int imageCounter = 0;

void CreateNewGraph()
{
    char filename[100] = "";
    sprintf(filename, "grath_%04d.png", imageCounter);

    char command[1000] = "";
    sprintf(command, "dot -Tpng /Users/aleksandr/Desktop/language/grapth.dot -o /Users/aleksandr/Desktop/language/grapth/%s", filename);
    system(command);

    char path[100] = "/Users/aleksandr/Desktop/language/grapth/";

    imageCounter++;

    fprintf(LOG_FILE, "<div style=\"display: block; margin-bottom: 20px;\">");
    fprintf(LOG_FILE, "<img src=\"%s%s\" alt=\"Graph Image\">", path, filename);
    fprintf(LOG_FILE, "</div>\n");
}

void SetParentPointers(Node* node, Node* parent)
{
    if (node == NULL) return;

    node->parent = parent;

    SetParentPointers(node->left, node);
    SetParentPointers(node->right, node);
}

void ClearFile(const char* filename)
{
    FILE* file = fopen(filename, "w");
    if (file != NULL) {
        fclose(file);
    }
}

int GenerateRandomNumber(int min, int max)
{
    int randomNumber = rand() % (max - min + 1) + min;

    return randomNumber;
}

// Node* ReadFromBuffer(Buffer* array, Node* currentNode)
// {
//     char* token = GetWord(array);
//     if (strcmp(token, "(") == 0)
//     {
//         CREAT_NODE(newNode);
//         token = GetWord(array);
//         SetNodeTypeAndValue(newNode, token);
//
//         newNode->parent = currentNode;
//
//         newNode->left = ReadFromBuffer(array, newNode);
//         newNode->right = ReadFromBuffer(array, newNode);
//         token = GetWord(array);
//         return newNode;
//     }
//     else if (strcmp(token, "nil") == 0)
//         return NULL;
//
//     return NULL;
// }

#define DifOper(OP, op)                         \
    differ.tree->rootTree->value = OP;          \
    node1 = Dif(differ.tree->rootTree);         \
    SetParentPointers(node1, NULL);             \
    PrintFunc(op, node1, differ.variables, file);

void DifferOperat(Lines* text)
{
    FILE* file = fopen("./file/tex.tex", "a");
    if (file == NULL)
    {
        printf("Ошибка при открытии файла.\n");
        return;
    }

    fprintf(file, "If someone asks you why you're so tired:\
                   Differentiation doesn't take vacations. It just goes to infinity.\n");
    fprintf(file, "The table of derivatives we will be using:\n");

    Differ differ = {};
    Tree tree = {};
    Variables array = {};
    differ.tree = &tree;
    differ.variables = &array;
    CtorRootAndVariebles(&differ);
    differ.variables->data[0].name = "x";
    differ.variables->data[0].value = 0;
    differ.variables->size++;

    CREAT_NODE(node);
    InitializeNode(node, OPERAT, SIN, NULL, NULL, NULL);
    CREAT_NODE(leftNode);
    InitializeNode(leftNode, VAR, 0, NULL, NULL, NULL);
    differ.tree->rootTree = node;

    leftNode->parent = differ.tree->rootTree;
    differ.tree->rootTree->left = leftNode;

    Node* node1 = Dif(differ.tree->rootTree);
    SetParentPointers(node1, NULL);
    //PrintFunc("sin(x)' = ", node1, differ.variables, file);

    DifOper(COS,    "cos(x)' = ");
    DifOper(TAN,    "tg(x)' = ");
    DifOper(COT,    "cot(x)' = ");
    DifOper(ARCSIN, "arcsin(x)' = ");
    DifOper(ARCCOS, "arccos(x)' = ");
    DifOper(ARCTAN, "arctg(x)' = ");
    DifOper(ARCCOT, "arccot(x)' = ");
    DifOper(LN,     "ln(x)' = ");

    fclose(file);
}

void PrintFunc(const char* expression, Node* node, Variables* arrayVar, FILE* file)
{
    assert(node);
    assert(arrayVar);

    fprintf(file, "$$%s ", expression);
    PrintNodeTex(node, file, arrayVar);
    fprintf(file, "$$\n");;
}
