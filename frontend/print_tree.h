#ifndef PRINT
#define PRINT

#include <stdlib.h>
#include "differentiation.h"

void Preamble();
void EndOfDocument();
void PrintStartProekt();

void PrintParentNorNullTex(Node* node, FILE* file, Variables* arrayVar);
void PrintNodeTex(Node* node, FILE* file, Variables* arrayVar);
void PrintParentNullTex(Node* node, FILE* file, Variables* arrayVar);
void PrintIntNodeTex(Node* node, FILE* file, Variables* arrayVar);

void PrintTreeLaTex(const char* expression, Node* node, Variables* arrayVar, Lines* text);

void PrintInFileInfForm(Node* node, Variables* arrayVar);
void PrintNodeInfForm(Node* node, FILE* file, Variables* arrayVar);

void PrintTreeToFileWithoutBrackets(Node* node, Variables* arrayVar);
void PrintNode(Node* node, FILE* file, Variables* arrayVar);
void PrintIntNode(Node* node, FILE* file, Variables* arrayVar);
void PrintParentNorNull(Node* node, FILE* file, Variables* arrayVar);
void PrintParentNull(Node* node, FILE* file, Variables* arrayVar);

void EndTex();
#endif
