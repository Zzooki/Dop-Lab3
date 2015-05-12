#include "env.h"
#include "print.h"

void PrintExp(expADT exp) {
    switch (ExpType(exp)) {
    case FuncExp:
    case IfExp:
    case CallExp:
        break;
    case ConstExp:
        printf("%d\n", ExpInteger(exp));
        break;
    case IdentifierExp:
        break;
    case CompoundExp:
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
