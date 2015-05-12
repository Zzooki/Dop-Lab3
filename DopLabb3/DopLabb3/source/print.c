#include "env.h"
#include "print.h"

void PrintExp(expADT exp) {
    switch (ExpType(exp)) {
    case FuncExp:
        break;
    case IfExp:
        printf("if ");
        PrintExp(GetIfLHSExpression(exp));
        printf(" %c ", GetIfRelOp(exp));
        PrintExp(GetIfRHSExpression(exp));
        printf(" then ");
        PrintExp(GetIfThenPart(exp));

        expADT elsePart = GetIfElsePart(exp);
        if (elsePart) {
            printf(" else ");
            PrintExp(elsePart);
        }
        break;
    case CallExp:
        break;
    case ConstExp:
        printf("%d", ExpInteger(exp));
        break;
    case IdentifierExp:
        printf("%s", ExpIdentifier(exp));
        break;
    case CompoundExp:
        PrintExp(ExpLHS(exp));
        printf("%c", ExpOperator(exp));
        PrintExp(ExpRHS(exp));
        break;
    }
}

void PrintValue(valueADT value) {
    switch (ValueType(value)) {
    case FuncValue:
        printf("%s :: Function with one argument.\n",
               GetFuncValueFormalArg(value));
        break;
    case IntValue:
        printf("%d :: Int\n", GetIntValue(value));
        break;
    }
}
