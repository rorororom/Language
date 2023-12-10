#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "read_from_file_in_buffer.h"

void ReadFileInBuffer (FILE *fp, Buffer* array)
{
    assert (array != NULL);
    assert (array -> buffer != NULL);

    size_t nread = fread (array -> buffer, sizeof(char), array -> size, fp);
    assert(nread == array -> size);

    fclose (fp);
}

void GetFileSize (FILE* file, struct Buffer* array)
{
    assert (file != NULL);

    struct stat st = {};

    if (fstat(fileno(file), &st) == -1)
    {
        printf ("Error: could not stat the file");
    }

    array->size = static_cast<size_t>(st.st_size);

}

void FillText (struct Buffer* array)
{
    assert (array != NULL);
    assert (array -> buffer != NULL);


    for (size_t i = 0; i < array -> size; i++)
    {
        if (array -> buffer[i] == '\n')
        {
            array -> buffer[i] = '\0';
        }
    }

    if (array -> size > 0 && array -> buffer[array -> size - 1] != '\n')
    {
        array -> buffer[array -> size] = '\0';
    }
}

void LineCount (struct Buffer* array, struct Lines* lines)
{
    assert (array != NULL);
    assert (array -> buffer != NULL);
    for (size_t i = 0; i < array -> size; i++)
    {
        if (array -> buffer[i] == '\n')
            lines -> line_count++;
    }
}

void FillTextAndCountLine (struct Buffer* array, struct Lines* lines)
{
    assert (lines != NULL);
    assert (array != NULL);
    assert (lines -> text != NULL);
    assert (array -> buffer != NULL);

    size_t line1 = 0;
    lines -> text[line1++] = array -> buffer;

    for (size_t i = 0; i < array -> size; i++)
    {
        if (i < array->size && array->buffer[i] == '\n')
        {
            array -> buffer[i] = '\0';
            lines -> text[line1] = &array -> buffer[i + 1];
            line1++;
        }
    }

    if (array -> size > 0 && array -> buffer[array -> size - 1] != '\n')
    {
        array -> buffer[array -> size] = '\0';
    }
}

void ProcessFile(struct Buffer* array, struct Lines* lines)
{
    FILE *fp = fopen("./file/phrases.txt", "rb");

    GetFileSize(fp, array);
    array->buffer = (char*)calloc(array -> size + 1, sizeof(char));

    ReadFileInBuffer(fp, array);
    LineCount(array, lines);
    lines->text = (char**)calloc(lines->line_count + 1, sizeof(char*));
    FillTextAndCountLine(array, lines);

}
