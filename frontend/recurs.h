#ifndef RECURS
#define RECURS

#include "../common/differentiation.h"

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
Node* GetA(Node* tokens);
Node* GetW(Node* tokens);
Node* GetO(Node* tokens);
Node* GetIf(Node* tokens);
Node* GetOp(Node* tokens);
Node* GetBody(Node* tokens);
//void BuildTREEEE(char* filename);
#endif

// Node* GetWhile(Node* tokens) {
//     fprintf(LOG_FILE, "я нахожусь в GetWhile, pbuf = %d, token = %d\n", pBuf, tokens[pBuf]);
//     Node* whileNode = &tokens[pBuf++];
//     fprintf(LOG_FILE, "я собираюсь вызвать GetE, pbuf = %d, token = %d\n", pBuf, tokens[pBuf]);
//
//     fprintf(LOG_FILE, "я после вызова GetE, pbuf = %d, token = %d\n", pBuf, tokens[pBuf]);
//
//     if (BUF_V != OPEN_BRACE) {
//         printf("Ошибка: ожидается '{' после условия While\n");
//         return NULL;
//     }
//     pBuf++;  // Пропустить '{'
//
//     Node* uslovie = GetBody(tokens);
//
//     if (BUF_V != CLOSE_BRACE) {
//         printf("Ошибка: ожидается '{' после условия While\n");
//         return NULL;
//     }
//     //pBuf++;
//
//      if (BUF_V != SCOBKA) {
//         printf("Ошибка: ожидается '(' после блока While\n");
//     }
//     pBuf++;
//
//     Node* condition = GetA(tokens);
//
//     if (BUF_V != SCOBKA) {
//         printf("Ошибка: ожидается ')' после блока While\n");
//     }
//     pBuf++;
//
//     whileNode->left = condition;
//     whileNode->right = uslovie;
//
//     return whileNode;
// }
