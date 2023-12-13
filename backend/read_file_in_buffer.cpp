#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/stat.h>

#include "read_file_in_buffer.h"
#include "compiler.h"

int MainFuncReadFile(struct Compiler *source) {
    assert(source != NULL);
    FILE *fp = fopen(source->file, "r");
    if (fp == NULL) {
        perror("1Error opening file");
        return -1;
    }
    source->size = GetFileSize(source);
    source->buffer = (char *)calloc(source->size + 1, sizeof(char));
    if (source->buffer == NULL) {
        perror("2Error allocating memory");
        fclose(fp);
        return -1;
    }
    ReadFileInBuffer(fp, source);

    word_count (source);
    fill_text_and_count_word (source);

    fclose(fp);

    return 0;
}

int GetFileSize(struct Compiler *source) {
    assert(source != NULL);

    struct stat st = {};
    if (stat(source->file, &st) == -1) {
        perror("Error: could not stat the file");
        return -1;
    }
    return st.st_size;
}

void ReadFileInBuffer(FILE *fp, struct Compiler *source)
{
    assert(source != NULL);
    assert(source->buffer != NULL);
    assert(source->size >= 0);

    size_t nread = fread(source->buffer, sizeof(char), source->size, fp);
    if (nread != source->size) {
        perror("Error reading file");
    }
}

void word_count(struct Compiler *source)
{
    assert(source != NULL);
    assert(source->buffer != NULL);

    for (int i = 0; i < source->size; i++) {
        if (source->buffer[i] == '\n' || source->buffer[i] == ' ') {
            source->words++;
        }
    }
}

void fill_text_and_count_word(struct Compiler *source)
{
    assert(source != NULL);
    assert(source->buffer != NULL);
    assert(source->textArray == NULL);

    source->textArray = (char **)calloc(source->words, sizeof(char *));
    if (source->textArray == NULL) {
        perror("Error allocating memory");
        return;
    }

    source->textArray[0] = source->buffer;
    int current_word = 1;
    for (int i = 0; i < source->size; i++) {
        if (source->buffer[i] == '\n' || source->buffer[i] == ' ') {
            source->buffer[i] = '\0';
            source->textArray[current_word] = &(source->buffer[i + 1]);
            current_word++;
        }
    }

    if (source->size > 0 && source->buffer[source->size - 1] != '\n') {
        source->buffer[source->size] = '\0';
    }
}
