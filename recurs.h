#ifndef RECURS
#define RECURS

#include "differentiation.h"

const int OP_LEN = 10;

int BuildTREEEE(char* filename, Differ* differ_before);
// void BuildTREEEE(char* filename);
void TokenInizial();
// Node* GetG(const char* str);
Node* GetG();
Node* GetE();
Node* GetN();
Node* GetT();
Node* GetP();
Node* GetW();
Node* GetO();
//void BuildTREEEE(char* filename);
#endif
