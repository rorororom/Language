#ifndef RECURS
#define RECURS

#include "../common/differentiation.h"

const int OP_LEN = 10;
const int NO_OP = -9999;


int BuildTREEEE(char* filename, Differ* differ_before);
// void BuildTREEEE(char* filename);
void TokenInizial(Node* tokens);
// Node* ParseInputTokens(const char* str);
Node* ParseInputTokens(Node* tokens);
Node* GetExpression(Node* tokens);
Node* GetNumber(Node* tokens);
Node* GetTerm(Node* tokens);
Node* GetP(Node* tokens);
Node* GetAddition(Node* tokens);
Node* GetExpressionWithPower(Node* tokens);
Node* GetOparat(Node* tokens);
Node* GetIf(Node* tokens);
Node* GetOparatp(Node* tokens);
Node* ExtractStatementBody(Node* tokens);
//void BuildTREEEE(char* filename);
#endif

// Node* GetExpressionWithPowerhile(Node* tokens) {
//     fprintf(LOG_FILE, "я нахожусь в GetExpressionWithPowerhile, pbuf = %d, token = %d\n", pBuf, tokens[pBuf]);
//     Node* whileNode = &tokens[pBuf++];
//     fprintf(LOG_FILE, "я собираюсь вызвать GetExpression, pbuf = %d, token = %d\n", pBuf, tokens[pBuf]);
//
//     fprintf(LOG_FILE, "я после вызова GetExpression, pbuf = %d, token = %d\n", pBuf, tokens[pBuf]);
//
//     if (BUF_V != OPEN_BRACE) {
//         printf("Ошибка: ожидается '{' после условия While\n");
//         return NULL;
//     }
//     pBuf++;  // Пропустить '{'
//
//     Node* uslovie = ExtractStatementBody(tokens);
//
//     if (BUF_V != CLOSE_BRACE) {
//         printf("Ошибка: ожидается '{' после условия While\n");
//         return NULL;
//     }
//     //pBuf++;
//
//      if (BUF_V != BRACKET) {
//         printf("Ошибка: ожидается '(' после блока While\n");
//     }
//     pBuf++;
//
//     Node* condition = GetAddition(tokens);
//
//     if (BUF_V != BRACKET) {
//         printf("Ошибка: ожидается ')' после блока While\n");
//     }
//     pBuf++;
//
//     whileNode->left = condition;
//     whileNode->right = uslovie;
//
//     return whileNode;
// }
