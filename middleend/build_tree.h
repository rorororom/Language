#ifndef BUILD
#define BUILD

#include "../common/differentiation.h"

const int OP_LEN = 10;
const int NO_OP = -9999;

void AddNewId(int type, int val, char* name);
void CreatId();
int BuildTreeMiddleEnd(char* filename, Differ* differ_before);
Node* ParseInputTokens(Node* tokens);
Node* GetIf(Node* tokens);
Node* ExtractStatementBody(Node* tokens);
Node* GetExpressionWithPowerhile(Node* tokens);
Node* GetId(Node* tokens);
Node* GetAddition(Node* tokens);
Node* GetExpression(Node* tokens);
Node* GetTerm(Node* tokens);
Node* GetExpressionWithPower(Node* tokens);
Node* GetP(Node* tokens);
Node* GetNumber(Node* tokens);
Node* GetOparat(Node* tokens);
int GetNumFromMyLanguage();
void AddVar(char* name, Node* tokens);
void AddVariableVar(Variables* arrayVar, int type, int value, const char* name);
void GetOparatperatFromMyLanguage(Node* tokens);
void SkipSpaces();
void TokenInizial(Node* tokens);

#endif
