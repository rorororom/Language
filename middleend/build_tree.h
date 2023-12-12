#ifndef BUILD
#define BUILD

#include "../common/differentiation.h"

const int OP_LEN = 10;
const int NO_OP = -9999;

void AddNewId(int type, int val, char* name);
void CreatId();
int BuildTreeMiddleEnd(char* filename, Differ* differ_before);
Node* GetG(Node* tokens);
Node* GetIf(Node* tokens);
Node* GetBody(Node* tokens);
Node* GetWhile(Node* tokens);
Node* GetId(Node* tokens);
Node* GetA(Node* tokens);
Node* GetE(Node* tokens);
Node* GetT(Node* tokens);
Node* GetW(Node* tokens);
Node* GetP(Node* tokens);
Node* GetN(Node* tokens);
Node* GetO(Node* tokens);
int GetNUM();
void AddVar(char* name, Node* tokens);
void AddVariableVar(Variables* arrayVar, int type, int value, const char* name);
void GetOPERAT(Node* tokens);
void SkipSpaces();
void TokenInizial(Node* tokens);

#endif
