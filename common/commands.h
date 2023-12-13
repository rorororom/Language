DEF_CMD(PUSH,   1, 1,
    {
        if((startCode & (1 << 5)) != 0)
        {
            i++;
            int code_arg = source->codeArray[i];
            StackPush(&source->myStack, return_arg(source, code_arg));
        }
        else
        {
            i++;
            value = source->codeArray[i];
            StackPush(&source->myStack, value);
        }
    })
DEF_CMD(SUB,    2, 0,
    {
        if (CheckStackSizeForOperation(&source->myStack))
        {
            number1 = StackPop(&source->myStack);
            number2 = StackPop(&source->myStack);
            value = number2 - number1;
            StackPush(&source->myStack, value);
        }
    })
DEF_CMD(ADD,    3, 0,
    {
        if (CheckStackSizeForOperation(&source->myStack))
        {
            number1 = StackPop(&source->myStack);
            number2 = StackPop(&source->myStack);
            value = number2 + number1;
            StackPush(&source->myStack, value);
        }
    })
DEF_CMD(MUL,    4, 0,
    {
        if (CheckStackSizeForOperation(&source->myStack))
        {
            number1 = StackPop(&source->myStack);
            number2 = StackPop(&source->myStack);
            value = number2 * number1;
            StackPush(&source->myStack, value);
        }
    })
DEF_CMD(DIV,    5, 0,
    {
        if (CheckStackSizeForOperation(&source->myStack))
        {
            number1 = StackPop(&source->myStack);
            number2 = StackPop(&source->myStack);

            if (number1 == 0)
            {
                printf("Ошибка: деление на ноль недопустимо.\n");
                StackPush(&source->myStack, number2);
                StackPush(&source->myStack, number1);
            }
            else
            {
                value = number2 / number1;
                StackPush(&source->myStack, value);
            }
        }
    })
DEF_CMD(SQRT,   6, 0,
    {
        if (CheckStackSizeForOperation(&source->myStack))
        {
            number1 = StackPop(&source->myStack);
            value = number1 * 0.5;
            StackPush(&source->myStack, value);
        }
    })
DEF_CMD(SIN,    7, 0,
    {
        if (CheckStackSizeForOperation(&source->myStack))
        {
            number1 = StackPop(&source->myStack);
            value = sin(number1);
            StackPush(&source->myStack, value);
        }
    })
DEF_CMD(COS,    8, 0,
    {
        if (CheckStackSizeForOperation(&source->myStack))
        {
            number1 = StackPop(&source->myStack);
            value = cos(number1);

            StackPush(&source->myStack, value);
        }
    })
DEF_CMD(IN,     9, 0,
    {
        printf("Введите число\n");
        scanf("%d", &value);
        StackPush(&source->myStack, value);
        Elem_t topValue = StackTop(&source->myStack);
        printf("cc= Top value on the stack: %d\n", topValue);
    })
DEF_CMD(OUT,   10, 0,
    {
        if (GetSizeStack(&source->myStack) > 0)
            printf("answer = %d\n", StackPop(&source->myStack));
    })
DEF_CMD(POP,   11, 1,
    {
        if((startCode & (1 << 5)) != 0)
        {
            i++;
            code_arg = source->codeArray[i];
            PopArg(source, code_arg);
        }
        else
        {
            i++;
            value = source->codeArray[i];
            PopArg(source, value);
        }
    })
DEF_CMD(MATAN, 12, 0,
    {
        system ("say \" МНЕ НЕ НРАВИТСЯ МАТАН, КОМУ ОН ВООБЩЕ МОЖЕТ НРАВИТСЯ? ТОТ, КОМУ НРАВИТСЯ МАТАН - СТРАННЫЙ\" ");
    })
DEF_CMD(TG,    13, 0,
    {
        value = sin(StackPop(&source->myStack));
        StackPush(&source->myStack, value);
    })
DEF_CMD(CTG,   14, 0,
    {
        number1 = StackPop(&source->myStack);
        value = cos(number1) /sin(number1);
        StackPush(&source->myStack, value);
    })
DEF_CMD(POW,   15, 0,
    {
        exponent = StackPop(&source->myStack);
        base = StackPop(&source->myStack);
        result = 1;
        for (int i = 0; i < exponent; i++)
        {
            result *= base;
        }
        StackPush(&source->myStack, result);
    })
DEF_CMD(CAT,   18, 0,
    {
        printf(" /\\_/\\  \n");
        printf("( o.o ) \n");
        printf(" > ^ <  \n");
        printf("  / | \\  \n");
        printf(" /  |  \\ \n");
        printf("  / \\   \n");
        printf(" /   \\  \n");
    })
DEF_CMD(SLEAP, 19, 0,
    {
        system ("say \" ВОТ БЛИН, ПОРА СПАТЬ\" ");
    })
DEF_CMD(LOVE,  20, 0,
    {
        system ("say \" Я ТЕБЯ ЛЮБЛЮ\" ");
    })
DEF_CMD(DOG,   21, 0,
    {
        printf("   / \\__\n");
        printf("  (    @\\___\n");
        printf("  /         O\n");
        printf(" /   (_____/\n");
        printf(" /_____/   U\n");
    })
DEF_CMD(HLT,   -1, 0,
    {;})
DEF_CMD(JMP,   22, 1,
{
    int jump_to = source->codeArray[i+1];
    i = jump_to - 1;
})

CONDITIONAL_JUMP(JA, 23, number1 > number2)
CONDITIONAL_JUMP(JAE, 24, number1 >= number2)
CONDITIONAL_JUMP(JB, 25, number1 < number2)
CONDITIONAL_JUMP(JBE, 26, number1 <= number2)
CONDITIONAL_JUMP(JE, 27, number1 == number2)
CONDITIONAL_JUMP(JNE, 28, number1 != number2)

DEF_CMD(PRINT, 31, 0,
    {printf ("answer = %d\n", StackPop(&source->myStack));})
