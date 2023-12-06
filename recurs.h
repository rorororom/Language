#ifndef RECURS
#define RECURS

#include "differentiation.h"

const int OP_LEN = 10;
const int NO_OP = -9999;

int BuildTREEEE(char* filename, Differ* differ_before);
// void BuildTREEEE(char* filename);
void TokenInizial(Node* tokens);
// Node* GetG(const char* str);
Node* GetG(Node* tokens);
Node* GetE(Node* tokens);
Node* GetN(Node* tokens);
Node* GetT(Node* tokens);
Node* GetP(Node* tokens);
Node* GetW(Node* tokens);
Node* GetO(Node* tokens);
//void BuildTREEEE(char* filename);
#endif
