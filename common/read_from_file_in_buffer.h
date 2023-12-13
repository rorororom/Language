#ifndef READ
#define READ

#include <stdio.h>

struct Buffer {
    char *buffer;
    size_t size;
};

struct Lines {
    char **text;
    size_t line_count;
};

void GetFileSize (FILE* file, struct Buffer* array);
void ReadFileInBuffer (FILE *fp, struct Buffer* array);
void FillText (struct Buffer* array);
void WordCount(struct Compiler *source);

void ProcessFile(struct Buffer* array, struct Lines* lines);
void FillTextAndCountLine (struct Buffer* array, struct Lines* lines);
void LineCount (struct Buffer* array, struct Lines* lines);

#endif
