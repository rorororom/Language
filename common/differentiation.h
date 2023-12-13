#ifndef DIFFER
#define DIFFER

#include <stdlib.h>
#include "read_from_file_in_buffer.h"

#define CREAT_NODE(node) Node* node = (Node*)malloc(sizeof(Node))

const int MAX_LEN = 256;
const int MAX_VARIABLES = 10;
const int CAPACITY = 100;
const int ZERO = 0;
const double E_CONST = 2.71;
const int ERROR_OP = -100;

typedef struct Node {
    int type;
    int value;
    Node* left;
    Node* right;
    Node* parent;
    int flagDirection;
} Node;

typedef struct Tree {
    Node* rootTree;
    int size;
} Tree;

typedef struct VariableData {
    char* name;
    int value;
    int type;
} VariableData;

typedef struct Variables {
    VariableData* data;
    int capacity;
    int size;
} Variables;

typedef struct Differ {
    Tree* tree;
    Variables* variables;
} Differ;

enum OPERATION{
    OperError = -6,
    BRACKET,
    OPEN_BRACE,
    CLOSE_BRACE,
    SLASH,
    END,
    SUB = 0,
    DIV,
    ADD,
    MUL,
    POW,
    SIN = 5,
    COS = 6,
    TAN = 7,
    COT = 8,
    LN = 9,
    ARCSIN = 10,
    ARCCOS = 11,
    ARCTAN = 12,
    ARCCOT = 13,
    IF = 14,
    EQ = 15,
    SEMICOLON = 16,
    WHILE,
    MORE,
    LESS
};

enum TYPE{
    INT,
    OPERAT,
    VAR,
    CONST
};

enum DIRECTION{
    LEFT,
    RIGHT
};

void CtorRootAndVariebles(Differ* differ);
void TreeAndVarieblesDtor(Differ* differ);
void ClearFile(const char* filename);
void DifferOperat(Lines* text);
void PrintFunc(const char* expression, Node* node, Variables* arrayVar, FILE* file);

void InitializeNode(Node* node, int type, int value, Node* left, Node* right, Node* parent);

Node* NewNode(int type, double value, Node* left, Node* right);
void SetParentPointers(Node* node, Node* parent);
Node* Copy(Node* nowNode);
Node* Dif(Node* nowNode);
void TransformationNode(Node** nowNode, int* changeCount, Variables* arrayVar, Tree* treeDif);

int GetVariableIdByName(Variables* arrayVar, const char* variableName);
int AddVariable(Variables* arrayVar, const char*  variableName, const int variableValue);

Node* ReadFromBufferInf(Buffer* array, Node* currentNode, Variables* arrayVar);

void BuildTreeFromFile(const char* filename, Differ* differ);

//Node* ReadFromBuffer(Buffer* array, Node* currentNode);
char* GetWord(Buffer* array);

//Node* BuildTreeFromBuffer(struct Buffer* array, int* index, Variables* arrayVar);
void SetNodeTypeAndValue(Node* node, char* value, Variables* arrayVar);

char* IssuesOperation(Node* node);
char* IssuesOperation1(Node* node);
void GenerateImage(Differ* differ);
void GenerateGraphImage();

double EvaluateExpression(Node* node, Variables* arrayVar);

int CheckingPriorityOperation(int operation);

char* FromOperationToWord(int operation);

int GenerateRandomNumber(int min, int max);

void CreateNewGraph();

//VariableData* findOrCreateVariable(Variables* arrayVar, const char* name);

#endif
