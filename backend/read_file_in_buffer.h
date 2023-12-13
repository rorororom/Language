#ifndef REED_H
#define REED_H

#define LEN 256

int MainFuncReadFile(struct Compiler *source);
void ReadFileInBuffer(FILE *fp, struct Compiler *source);
int GetFileSize(struct Compiler *source);
void word_count(struct Compiler *source);
void fill_text_and_count_word(struct Compiler *source);

#endif
