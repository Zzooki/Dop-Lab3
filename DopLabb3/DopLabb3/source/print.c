#include "env.h"
#include "print.h"

void PrintExp(expADT exp) {
    switch (ExpType(exp)) {
    case ConstExp:
        printf("%d\n", ExpInteger(exp));
        break;
    }
}

void PrintValue(valueADT value) {
    switch (ValueType(value)) {
    case FuncValue:
        printf("NEPP!!!!\n");
        break;
    case IntValue:
        printf("%d::Int\n", GetIntValue(value));
        break;
    }
}
