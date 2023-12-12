#include <stdlib.h>

#include "../frontend/differentiation.h"
#include "../frontend/print_tree.h"
#include "../frontend/log_funcs.h"

#include "build_tree.h"

char* s = NULL;
int p = 0;
Variables arrayVar;
int size = 0;
int pBuf = 0;

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
    BuildTreeMiddleEnd("../file/defInf4.txt", &differ_before);
    SetParentPointers(differ_before.tree->rootTree, NULL);
    TransformAndEvaluate(&differ_before);
    //PrintTreeToFileWithoutBrackets(differ_before.tree->rootTree, differ_before.variables);
}
