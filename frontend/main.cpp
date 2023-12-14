#include "../common/differentiation.h"
#include "recurs.h"
#include "../common/log_funcs.h"
#include "../common/write_in_file.h"

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

    SetParentPointers(differ_before.tree->rootTree, NULL);
    PrintTreeToFileWithoutBrackets("../file/code2.txt", differ_before.tree->rootTree, differ_before.variables);
    TreeAndVarieblesDtor(&differ_before);
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

    BuildTREEEE("../file/defInf4.txt", &differ_before);

    SetParentPointers(differ_before.tree->rootTree, NULL);
    PrintTreeToFileWithoutBrackets("../file/code2.txt", differ_before.tree->rootTree, differ_before.variables);

    TreeAndVarieblesDtor(&differ_before);
}
