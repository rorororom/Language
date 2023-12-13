#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../common/log_funcs.h"
#include "../common/const.h"

#include "read_file_in_buffer.h"
#include "compiler.h"

int main ()
{
    OpenLogFile ("LOGE.log", "w");

    struct Compiler source = {
        "../assets/commands.txt",
        NULL,
        NULL,
        NULL,
        0,
        0,
        NULL,
        0,
        NULL,
        0
    };

    MainFuncReadFile(&source);

    PrintTextArray(&source);

    FILE* outputfile = fopen ("../assets/machine_code.txt", "w");
    if (ValidationFile(outputfile) != 0)
    {
        return 1;
    }

    int* codeRegister = (int*)calloc (MAX_SIZE_REG, sizeof (char));

    GenerateCodeFromInput (&source, outputfile);

    CommandPrintout (&source);

    BinaryRecordind (&source);

    fclose (outputfile);
    return 0;
}
