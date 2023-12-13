#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "../common/differentiation.h"
#include "../common/print_tree.h"
#include "../common/log_funcs.h"
#include "../common/write_in_file.h"

#include "build_tree.h"

void Print(Node* node, FILE* file, Variables* arrayVar);
void PrintAs(char* filename, Node* node, Variables* arrayVar);
void PrintInt(Node* node, FILE* file, Variables* arrayVar);
void PrintParent(Node* node, FILE* file, Variables* arrayVar);
char* IssuesOp(Node* node);

void TransformAndEvaluate(Differ* differ)
{
    int changeCount = 0;
    do {
        changeCount = 0;
        TransformationNode(&differ->tree->rootTree, &changeCount, differ->variables, differ->tree);
        GenerateImage(differ);
    } while (changeCount > 0);
}


int main()
{
    OpenLogFile("LOGE1111.txt", "w");
    Differ differ_before = {};
    Tree tree = {};
    Variables array = {};
    differ_before.tree = &tree;
    differ_before.variables = &array;

    CtorRootAndVariebles(&differ_before);
    BuildTreeMiddleEnd("../file/code2.txt", &differ_before);
    GenerateImage(&differ_before);
    SetParentPointers(differ_before.tree->rootTree, NULL);
    TransformAndEvaluate(&differ_before);
    PrintTreeToFileWithoutBrackets("../file/code3.txt", differ_before.tree->rootTree, differ_before.variables);
    PrintAs("../file/defInf.txt", differ_before.tree->rootTree, differ_before.variables);
}

void PrintAs(char* filename, Node* node, Variables* arrayVar)
{
    assert(node);

    FILE* file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Ошибка при открытии файла.\n");
        return;
    }

    Print(node, file, arrayVar);
    fprintf(file, "OUT\n");
    fprintf(file, "HLT\n");
    fclose(file);
}

void Print(Node* node, FILE* file, Variables* arrayVar)
{
    if (node == NULL) return;
    if (node->type == OPERAT && node->value == SEMICOLON)
    {
        Print(node->left, file, arrayVar);
        Print(node->right, file, arrayVar);
    }
    else if (node->type == INT || node->type == VAR)
        PrintInt(node, file, arrayVar);
    else {
        PrintParent(node, file, arrayVar);
    }
}

void PrintInt(Node* node, FILE* file, Variables* arrayVar)
{
    if (node->type == VAR) {
        char* token = "";
        if (node->value == 0) token = "RAX";
        else if (node->value == 1) token = "RBX";
        else if (node->value == 2) token = "RCX";
        if (node->parent->type == OPERAT && node->parent->value == EQ)
            fprintf(file, "POP %s\n", token);
        else
            fprintf(file, "PUSH %s\n", token);
    }
    else {
        fprintf(file, "PUSH %d\n", node->value);
    }
}

int lable[100];
int CNT = 1;

void PrintParent(Node* node, FILE* file, Variables* arrayVar)
{
    if (node->type == OPERAT && node->value == EQ) {
        Print(node->right, file, arrayVar);
        Print(node->left, file, arrayVar);
    }
    else if (node->type == OPERAT && (node->value == LESS || node->value == MORE)) {
        lable[node->value] = CNT;
        CNT++;
        Print(node->right, file, arrayVar);
        Print(node->left, file, arrayVar);
        char* token = "";
        if (node->value == MORE) token = "JA";
        else if (node->value == LESS) token = "JB";
        fprintf(file, "%s :%d\n", token, lable[node->value]);
    }
    else if (node->value == WHILE || node->value == IF)
    {
        lable[node->value] = CNT;
        CNT++;
        fprintf(file, ":%d\n", lable[node->value]);
        node->flagDirection = LEFT;
        Print(node->left, file, arrayVar);
        node->flagDirection = RIGHT;
        Print(node->right, file, arrayVar);
        fprintf(file, "JMP :%d\n", lable[node->value]);
        fprintf(file, ":%d\n", lable[node->left->value]);
    }
    else
    {
        node->flagDirection = LEFT;
        Print(node->left, file, arrayVar);

        char* operation = "";
        operation = IssuesOp(node);

        node->flagDirection = RIGHT;
        Print(node->right, file, arrayVar);
        fprintf(file, "%s\n", operation);
    }
}

char* IssuesOp(Node* node)
{
    if (node->value == ADD) return "ADD";
    if (node->value == SUB) return "SUB";
    if (node->value == DIV) return "DIV";
    if (node->value == MUL) return "MUL";
    if (node->value == POW) return "POW";
    else return "AAAA";
}
