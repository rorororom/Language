#include <stdlib.h>

#include "../common/differentiation.h"
#include "../common/print_tree.h"
#include "../common/log_funcs.h"
#include "../common/write_in_file.h"

#include "build_tree.h"

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
    PrintTreeToFileWithoutBrackets(differ_before.tree->rootTree, differ_before.variables);
}
