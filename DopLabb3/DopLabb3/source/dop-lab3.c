#include "exp.h"

#include <stdio.h>

main() {
    expADT lhs = NewIntegerExp(5);
    expADT rhs = NewIntegerExp(9);
    expADT ifPart = NewCompoundExp('=', NewIdentifierExp("snabel"), NewIntegerExp(1337));
    expADT ifExp = NewIfExp(lhs, '=', rhs, ifPart, NULL);

    PrintExp(ifExp);
    printf("\n\n");
    system("pause");
}
