#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "../common/log_funcs.h"
#include "stack.h"

#define FILEF (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define CAPACITY 1000
#define UP_KOEF 2
#define DOWN_KOEF 0.5

void StackCtor(struct Stack* myStack) {
    myStack->capacity = CAPACITY;
    myStack->size = 0;
    myStack->data = (int*)malloc(sizeof(int) * myStack->capacity);
}

void StackPush(struct Stack* myStack, int value) {
    if (myStack->size >= myStack->capacity) {
        myStack->capacity *= UP_KOEF;
        myStack->data = (int*)realloc(myStack->data, sizeof(int) * myStack->capacity);
    }
    myStack->data[myStack->size++] = value;
}

int StackPop(struct Stack* myStack) {
    assert(myStack->size > 0);

    int value = myStack->data[--myStack->size];
    return value;
}

void StackDump (struct Stack* myStack, const char *file, int line, const char *function)
{
    fprintf (LOG_FILE, "\nTime is %s\n", __TIME__);
    fprintf (LOG_FILE, "I'm stackDump called from %s (%d) %s\n", function, line, file);
    fprintf (LOG_FILE, "Stack[%p] \"myStack\" from %s in function - %s.\n\n", myStack->data, FILEF,  function);

    fprintf (LOG_FILE, "--------------------------------------------------------------------------\n");
    fprintf (LOG_FILE, "Struct:\n");
    fprintf (LOG_FILE, "\tsize = %d\n", myStack->size);
    fprintf (LOG_FILE, "\tcapacity = %d\n", myStack->capacity);
    fprintf (LOG_FILE, "\tадрес data[] = %p\n", myStack->data);

    int now_size = myStack->size;
    for (int i = 0; i < now_size; i++)
    {
        fprintf (LOG_FILE, "\tdata[%d] = %d\n", i, myStack->data[i]);
    }
    fprintf (LOG_FILE, "--------------------------------------------------------------------------\n");
}


void StackDtor(struct Stack* myStack) {
    free(myStack->data);
    myStack->data = NULL;
    myStack->capacity = 0;
    myStack->size = 0;
}

void StackRealloc(struct Stack* myStack, float koef_capacity) {
    if (myStack->size < (myStack->capacity) / DOWN_KOEF) {
        myStack->capacity *= DOWN_KOEF;
        myStack->data = (int*)realloc(myStack->data, sizeof(int) * myStack->capacity);
    }
}

int GetSizeStack (struct Stack* myStack)
{
    return myStack->size;
}

int StackTop(const struct Stack* myStack)
{
    if (myStack->size > 0)
    {
        return myStack->data[myStack->size - 1];
    }
    else
    {
        printf("Стек пуст. Невозможно получить верхний элемент.\n");
        exit(1);
    }
}
