#ifndef WRITE
#define WRITE

#include <stdlib.h>
#include "differentiation.h"

void PrintTreeToFileWithoutBrackets(char* filename, Node* node, Variables* arrayVar);
void PrintNode(Node* node, FILE* file, Variables* arrayVar);
void PrintIntNode(Node* node, FILE* file, Variables* arrayVar);
void PrintParentNorNull(Node* node, FILE* file, Variables* arrayVar);
void PrintParentNull(Node* node, FILE* file, Variables* arrayVar);

#endif
