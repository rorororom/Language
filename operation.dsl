SET_OPERATOR("add", ADD, "+", "+",
    {
        return leftValue + rightValue;
    },
    {
        return _ADD(DIFL, DIFR);
    })

SET_OPERATOR("sub", SUB, "-", "-",
    {
        return leftValue - rightValue;
    },
    {
        return _SUB(DIFL, DIFR);
    })

SET_OPERATOR("mul", MUL, "*", "*",
    {
        return leftValue * rightValue;
    },
    {
        return _ADD(_MUL(DIFL, COPYR), _MUL(COPYL, DIFR));
    })

SET_OPERATOR("div", DIV, "/", "/",
    {
        if (rightValue == 0)
        {
            printf("делить на ноль нельзя (запрещено на территории РФ с 23:00 до 07:00\
                    по четным дням согласно ст. УК 000-00 от 00.00.0000 г.)\n");
            return 0;
        }
        else return leftValue / rightValue;
    },
    {
        return _DIV(_SUB(_MUL(DIFL, COPYR),
               _MUL(COPYL, DIFR)),
               _POW(COPYR, NODE_INT(2)));
    })
SET_OPERATOR("pow", POW, "^", "**",
    {
        return FastPower((int)leftValue, (int)rightValue);
    },
    {
        if (NN_L_TYPE == VAR || NN_L_TYPE == OPERAT)
        {
            return _MUL(_MUL(COPYR, _POW(COPYL, _SUB(COPYR, NODE_INT(1)))), DIFL);
        }
        else if ((NN_L_TYPE == CONST || NN_L_TYPE == INT) && NN_L_TYPE == VAR)
        {
            return _MUL(_POW(COPYL, COPYR), NewNode(OPERAT, LN, nowNode->left, NULL));
        }
        else
        {
            return NODE_INT(0);
        }
    })

SET_OPERATOR("sin", SIN, "sin", "sin",
    {
        return sin(leftValue);
    },
    {
        return _MUL(_COS(COPYL), DIFL);
    })

SET_OPERATOR("cos", COS, "cos", "cos",
    {
        return cos(leftValue);
    },
    {
        return _MUL(_MUL(NODE_INT(-1), _SIN(COPYL)), DIFL);
    })

SET_OPERATOR("tg", TAN, "tg", "tg",
    {
        return tan(leftValue);
    },
    {
        return _MUL(_DIV(NODE_INT(1), _POW(_COS(COPYL), NODE_INT(2))), DIFL);
    })

SET_OPERATOR("ctg", COT, "ctg", "ctg",
    {
        return 1 / tan(leftValue);
    },
    {
        return _MUL(_DIV(DIFL, _POW(_SIN(COPYL), NODE_INT(2))), NODE_INT(-1));
    })

SET_OPERATOR("ln", LN, "ln", "ln",
    {
        return log(leftValue);
    },
    {
        return _MUL(_DIV(NODE_INT(1), COPYL), DIFL);
    })

SET_OPERATOR("arcsin", ARCSIN, "arcsin", "arcsin",
    {
        return RAD_TO_DEG(asin(leftValue));
    },
    {
        return _DIV(DIFL, _POW(_SUB(NODE_INT(1),
               _POW(COPYL, NODE_INT(2))), NODE_INT(0.5)));
    })

SET_OPERATOR("arccos", ARCCOS, "arccos", "arccos",
    {
        return RAD_TO_DEG(acos(leftValue));
    },
    {
        return _DIV(_MUL(  NODE_INT(-1), DIFL), _POW(_SUB(  NODE_INT(1),
               _POW(COPYL, NODE_INT(2))), NODE_INT(0.5)));
    })

SET_OPERATOR("arctan", ARCTAN, "arctan", "arctan",
    {
        return RAD_TO_DEG(atan(leftValue));
    },
    {
        return _DIV(DIFL, _ADD(NODE_INT(1), _POW(COPYL, NODE_INT(2))));
    })

SET_OPERATOR("arccot", ARCCOT, "arccot", "arccot",
    {
        return RAD_TO_DEG(atan(1 / leftValue));
    },
    {
        return _DIV(_MUL(  NODE_INT(-1), DIFL), _ADD(NODE_INT(1),
               _POW(COPYL, NODE_INT(2))));
    })
