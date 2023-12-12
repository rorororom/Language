#include "differentiation.h"
#include "recurs.h"
#include "log_funcs.h"
#include "write_in_file.h"

void TransformAndEvaluate(Differ* differ)
{
    int changeCount = 0;
    do {
        changeCount = 0;
        TransformationNode(&differ->tree->rootTree, &changeCount, differ->variables, differ->tree);
        GenerateImage(differ);
    } while (changeCount > 0);
}

void FuncFromFile(char* filename)
{
    OpenLogFile("LOGE1111.txt", "w");
    Differ differ_before = {};
    Tree tree = {};
    Variables array = {};
    differ_before.tree = &tree;
    differ_before.variables = &array;

    CtorRootAndVariebles(&differ_before);

    BuildTREEEE("../file/defInf4.txt", &differ_before);

//     int result = 0;
//     differ_before.variables->data[0].value = 0;
//     result = EvaluateExpression(differ_before.tree->rootTree, differ_before.variables);
//
//     printf("answer = %d\n", result);
//
//     Differ differ_after = {};
//     Tree treeDif = {};
//     Variables arrayDif = {};
//     differ_after.tree = &treeDif;
//     differ_after.variables = &arrayDif;
//
//     CtorRootAndVariebles(&differ_after);
//     differ_after.variables = differ_before.variables;
//
//     differ_after.tree->rootTree = Dif(differ_before.tree->rootTree);
    SetParentPointers(differ_before.tree->rootTree, NULL);
    TransformAndEvaluate(&differ_before);
    PrintTreeToFileWithoutBrackets(differ_before.tree->rootTree, differ_before.variables);
//     GenerateImage(&differ_after);
//     differ_after.tree->rootTree->parent = NULL;
//
//     result = EvaluateExpression(differ_after.tree->rootTree, differ_after.variables);
//     printf("answer = %d\n", result);

    // TreeAndVarieblesDtor(&differ_after);
    // TreeAndVarieblesDtor(&differ_before);
}

int main()
{
    FuncFromFile("./file/defInf4.txt");
}
