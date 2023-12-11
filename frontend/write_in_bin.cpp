#include <stdio.h>
#include <stdlib.h>

#include "differentiation.h"

Variables arrayVar;

void WriteInBinFile(Differ* differ)
{
    assert(differ);

    FILE* file = fopen("./file/code2.bin", "w");
    if (file == NULL)
    {
        printf("Ошибка при открытии файла.\n");
        return;
    }

    PrintNode(differ->tree->rootTree, file);
    fclose(file);
}

void PrintNode(Node* node, FILE* file, Variables* arrayVar)
{
    if (node == NULL) return;
    if (node->type == INT || node->type == VAR)
        PrintIntNode(node, file, arrayVar);
    else {
        PrintParentNull(node, file, arrayVar);
    }
}

void PrintIntNode(Node* node, FILE* file, Variables* arrayVar)
{
    PrintNode(node->left, file, arrayVar);
    if (node->type == VAR) {
        char* nameValue = strdup(arrayVar->data[int(node->value)].name);
        fprintf(file, "POP RAX\n ", nameValue);
    }
    else {
        fprintf(file, "PUSH %d\n", node->value);
    }
    PrintNode(node->right, file, arrayVar);
}

void PrintParentNorNull(Node* node, FILE* file, Variables* arrayVar)
{
    if (node->value == IF || node->value == WHILE)
    {
        fprintf(file, ":1\n");
        PrintNode(node->left);
        fprintf(file, "");
    }
    else
    {
        node->flagDirection = LEFT;
        PrintNode(node->left, file, arrayVar);
        char* operation = "";
        if (node->value == POW)
            operation = IssuesOperation1(node);
        else
            operation = IssuesOperation(node);

        fprintf(file, "%s ", operation);

        node->flagDirection = RIGHT;
        PrintNode(node->right, file, arrayVar);
    }

    if (operationParent > operationNowNode || ((node->parent->flagDirection == RIGHT) &&
       (operationParent == operationNowNode) && (node->value == DIV)))
    {
        fprintf(file, ") ");
    }
}

