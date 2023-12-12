#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "print_tree.h"
#include "differentiation.h"

void PrintInFileInfForm(Node* node, Variables* arrayVar)
{
    assert(node);

    FILE* file = fopen("./file/answer.txt", "a");
    if (file == NULL)
    {
        printf("Ошибка при открытии файла.\n");
        return;
    }

    PrintNodeInfForm(node, file, arrayVar);
    fprintf(file, "\n");
    fclose(file);
}

void PrintNodeInfForm(Node* node, FILE* file, Variables* arrayVar)
{
    assert(file);

    if (node == NULL) {
        fprintf(file, "nil ");
        return;
    }

    fprintf(file, "( ");

    PrintNodeInfForm(node->left, file, arrayVar);

    if (node->type == INT || node->type == CONST) {
        if (node->value == E_CONST) fprintf(file, "e ");
        else fprintf(file, "%.2lf ", node->value);
    }
    else if (node->type ==VAR) {
        char* operation = strdup(arrayVar->data[int(node->value)].name);
        fprintf(file, "%s ", operation);
    }
    else {
        char* operation = FromOperationToWord(node->value);
        fprintf(file, "%s ", operation);
    }

    PrintNodeInfForm(node->right, file, arrayVar);
    fprintf(file, ") ");
}

void PrintTreeLaTex(const char* expression, Node* node, Variables* arrayVar, Lines* text)
{
    assert(node);
    assert(arrayVar);

    FILE* file = fopen("./file/tex.tex", "a");
    if (file == NULL)
    {
        printf("Ошибка при открытии файла.\n");
        return;
    }

    int min_value = 0;
    int max_value = text->line_count - 1;
    int random_number = GenerateRandomNumber(min_value, max_value);

    fprintf(file, "$\n\\text{%s}$\n", text->text[random_number]);

    fprintf(file, "$$%s ", expression);
    PrintNodeTex(node, file, arrayVar);
    fprintf(file, "$$\n");

    fclose(file);
}

static void PrintDivisionTexNoPar(Node* node, FILE* file, Variables* arrayVar)
{
    fprintf(file, "\\frac ");

    fprintf(file, "{ ");
    PrintNodeTex(node->left, file, arrayVar);
    fprintf(file, "} ");

    fprintf(file, "{ ");
    PrintNodeTex(node->right, file, arrayVar);
    fprintf(file, "} ");
}

static void PrintDivisionTex(Node* node, FILE* file, Variables* arrayVar)
{
    fprintf(file, "\\frac ");

    fprintf(file, "{ ");
    node->flagDirection = LEFT;
    PrintNodeTex(node->left, file, arrayVar);
    fprintf(file, "} ");

    fprintf(file, "{ ");
    node->flagDirection = RIGHT;
    PrintNodeTex(node->right, file, arrayVar);
    fprintf(file, "} ");
}

static void PrintSpecialOperationTex(Node* node, FILE* file, Variables* arrayVar)
{
    char* operation = IssuesOperation(node);
    fprintf(file, "%s( ", operation);

    PrintNodeTex(node->left, file, arrayVar);
    fprintf(file, ") ");
}

static void PrintPowerOpTexNoPar(Node* node, FILE* file, Variables* arrayVar)
{
    if (node->right->type == INT && node->right->value == 0.5) {
        fprintf(file, "\\sqrt{ ");
        PrintNodeTex(node->left, file, arrayVar);
        fprintf(file, "} ");
    }
    else {
        if (node->left->left == NULL && node->left->right == NULL)
            PrintNodeTex(node->left, file, arrayVar);
        else {
            fprintf(file, "( ");
            PrintNodeTex(node->left, file, arrayVar);
            fprintf(file, ") ");
        }
        char* operation = IssuesOperation(node);
        fprintf(file, "%s ", operation);
        fprintf(file, "{ ");
        PrintNodeTex(node->right, file, arrayVar);
        fprintf(file, "} ");
    }
}

static void PrintPowerOpTex(Node* node, FILE* file, Variables* arrayVar)
{
    PrintNodeTex(node->left, file, arrayVar);
    char* operation = IssuesOperation(node);
    fprintf(file, "%s ", operation);
    fprintf(file, "{ ");
    PrintNodeTex(node->right, file, arrayVar);
    fprintf(file, "} ");
}

static void PrintMultiplicationWithAddSubLeft(Node* node, FILE* file, Variables* arrayVar)
{
    fprintf(file, "( ");
    PrintNodeTex(node->left, file, arrayVar);
    fprintf(file, ") ");
    fprintf(file, "* ");
    PrintNodeTex(node->right, file, arrayVar);
}

static void PrintMultiplicationWithAddSubRight(Node* node, FILE* file, Variables* arrayVar)
{
    fprintf(file, "( ");
    PrintNodeTex(node->right, file, arrayVar);
    fprintf(file, ") ");
    fprintf(file, "* ");
    PrintNodeTex(node->right, file, arrayVar);
}

static void ProcessDefaultOperationTex(Node* node, FILE* file, Variables* arrayVar)
{
    node->flagDirection = LEFT;
    PrintNodeTex(node->left, file, arrayVar);

    if (node->value == MUL) {
        fprintf(file, "\\cdot ");
    }
    else {
        char* operation = IssuesOperation(node);
        fprintf(file, "%s ", operation);
    }

    node->flagDirection = RIGHT;
    PrintNodeTex(node->right, file, arrayVar);
}

void PrintParentNorNullTex(Node* node, FILE* file, Variables* arrayVar)
{
    int operationParent = 0;
    if (node->parent != NULL) {
        operationParent = CheckingPriorityOperation(node->parent->value) ;
        if (operationParent == ERROR_OP) printf("error operation\n");
    }

    int operationNowNode = CheckingPriorityOperation(node->value);
    if (operationNowNode == ERROR_OP) printf("error operation\n");

    if ((node->parent->flagDirection == RIGHT) &&
        (operationParent == operationNowNode) && (node->value == SUB)) {
        node->value = ADD;
    }

    if (node->value == DIV) {
        PrintDivisionTexNoPar(node, file, arrayVar);
    }
    else if (5 <= node->value && node->value <= 13) {
        PrintSpecialOperationTex(node, file, arrayVar);
    }
    else if (node->value == POW) {
        PrintPowerOpTexNoPar(node, file, arrayVar);
    }

    else if (node->value       == MUL  && (((node->left->value == ADD ||
             node->left->value == SUB) &&    node->left->type  == OPERAT))) {
        PrintMultiplicationWithAddSubLeft(node, file, arrayVar);
    }
    else if (node->value        == MUL  && (((node->right->value == ADD ||
             node->right->value == SUB) &&    node->right->type  == OPERAT))) {
        PrintMultiplicationWithAddSubRight(node, file, arrayVar);
    }
    else {
        ProcessDefaultOperationTex(node, file, arrayVar);
    }
}

void PrintNodeTex(Node* node, FILE* file, Variables* arrayVar)
{
    if (node == NULL) {
        return;
    }
    if (node->type == INT || node->type == VAR || node->type == CONST) {
        PrintIntNodeTex(node, file, arrayVar);
    }
    else {
        if (node->parent != NULL) {
            PrintParentNorNullTex(node, file, arrayVar);
        }
        else {
            PrintParentNullTex(node, file, arrayVar);
        }
    }
}

void PrintIntNodeTex(Node* node, FILE* file, Variables* arrayVar)
{
    PrintNodeTex(node->left, file, arrayVar);
    if (node->type == CONST) {
        fprintf(file, "e ");
    }
    else if (node->type == INT) {
        fprintf(file, "%lg ", node->value);
    }
    else if (node->type ==VAR) {
        char* nameVar = strdup(arrayVar->data[int(node->value)].name);
        fprintf(file, "%s ", nameVar);
    }
    PrintNodeTex(node->right, file, arrayVar);
}

void PrintParentNullTex(Node* node, FILE* file, Variables* arrayVar)
{
    if (node->value == DIV) {
        PrintDivisionTex(node, file, arrayVar);
    }
    else if (node->value == POW) {
        PrintPowerOpTex(node, file, arrayVar);
    }
    else if (SIN <= node->value && node->value <= ARCCOT) {
        PrintSpecialOperationTex(node, file, arrayVar);
    }
    else {
        ProcessDefaultOperationTex(node, file, arrayVar);
    }
}

void Preamble()
{
    FILE* file = fopen("./file/tex.tex", "a");
    if (file == NULL) {
        printf("Ошибка при открытии файла.\n");
        return;
    }

//     fprintf(file, R"(
// documentclass{article}
// usepackage[a4paper]{geometry}
// geometry{top=1.41cm, bottom=1.41cm, left=1.41cm, right=1.41cm, marginparwidth=1.75cm}
// )");

    fprintf(file, "\\documentclass{article}\n");
    fprintf(file, "\\usepackage[a4paper]{geometry}\n");
    fprintf(file, "\\geometry{top=1.41cm, bottom=1.41cm, left=1.41cm, right=1.41cm, marginparwidth=1.75cm}\n");

    fprintf(file, "\\usepackage[T2A]{fontenc}\n");
    fprintf(file, "\\usepackage[utf8]{inputenc}\n");
    fprintf(file, "\\usepackage[english, russian]{babel}\n");

    fprintf(file, "\\usepackage{amsmath}\n");
    fprintf(file, "\\usepackage{graphicx}\n");
    fprintf(file, "\\usepackage[colorlinks=true, allcolors=blue]{hyperref}\n");
    fprintf(file, "\\usepackage{amsfonts}\n");
    fprintf(file, "\\usepackage{amssymb}\n");
    fprintf(file, "\\usepackage{seqsplit}\n");
    fprintf(file, "\\usepackage[dvipsnames]{xcolor}\n");
    fprintf(file, "\\usepackage{enumitem}\n");
    fprintf(file, "\\usepackage{algorithm}\n");
    fprintf(file, "\\usepackage{algpseudocode}\n");
    fprintf(file, "\\usepackage{algorithmicx}\n");
    fprintf(file, "\\usepackage{mathalfa}\n");
    fprintf(file, "\\usepackage{mathrsfs}\n");
    fprintf(file, "\\usepackage{dsfont}\n");
    fprintf(file, "\\usepackage{caption,subcaption}\n");
    fprintf(file, "\\usepackage{wrapfig}\n");
    fprintf(file, "\\usepackage[stable]{footmisc}\n");
    fprintf(file, "\\usepackage{indentfirst}\n");
    fprintf(file, "\\usepackage{rotating}\n");
    fprintf(file, "\\usepackage{pdflscape}\n");

    fprintf(file, "\\usepackage{MnSymbol,wasysym}\n");
    fprintf(file, "\\title{<<To differentiate or not to differentiate? That is the question.>>}\n");
    fprintf(file, "\\author{Ромашкина Мария Б05-332}\n");

    fprintf(file, "\\begin{document}\n");
    fprintf(file, "\\maketitle\n");
    fprintf(file, "\\newpage\n");

    fclose(file);
}

void EndOfDocument()
{
    FILE* file = fopen("./file/tex.tex", "a");
    if (file == NULL) {
        printf("Ошибка при открытии файла.\n");
        return;
    }

    fprintf(file, "\\end{document}\n");
    fclose(file);
}

void PrintStartProekt()
{
    FILE* file = fopen("./file/tex.tex", "a");
    if (file == NULL) {
        printf("Ошибка при открытии файла.\n");
        return;
    }

    fprintf(file, "We believe that in a world full of changes and surprises, the ability to distinguish,\
                   analyze, and highlight key moments is a true art. Differentiation is like a reliable map in the labyrinth of life,\
                   helping us navigate the turbulent reality. Imagine this: you stand on top of a mountain,\
                   and your knowledge of differential equations is like wings that allow you to soar freely in the air of change.\
                   It's not just mathematics; it's the ability to see beauty in structures and relationships. And if someone ever asks you,\
                   to differentiate or not to differentiate, you will definitely know the answer -\
                   let's unleash our potential and highlight the key moments in this exciting journey called Life.\n");
    fprintf(file, "\n");
    fprintf(file, "But, for now, while you are young...\
                   Party and enjoy yourself because youth will never come back,\
                   but knowing how to take derivatives, you can always Google that)\n");
    fprintf(file, "\n");

    fclose(file);
}

void EndTex()
{
    FILE* file = fopen("./file/tex.tex", "a");
    if (file == NULL) {
        printf("Ошибка при открытии файла.\n");
        return;
    }

    fprintf(file, "\\newpage");
    fprintf(file, "\\begin{thebibliography}{9}");
    fprintf(file, "\\bibitem{lukashov}");
    fprintf(file, "Lukashov, Alexey Leonidovich.");
    fprintf(file, "\\end{thebibliography}");

    fclose(file);
}
