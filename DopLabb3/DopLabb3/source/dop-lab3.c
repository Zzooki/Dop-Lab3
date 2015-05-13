#include "exp.h"
#include "parser.h"

#include <stdio.h>

main() {
    /*expADT lhs = NewIntegerExp(5);
    expADT rhs = NewIntegerExp(9);
    expADT ifPart = NewCompoundExp('=', NewIdentifierExp("snabel"), NewIntegerExp(1337));
    expADT ifExp = NewIfExp(lhs, '=', rhs, ifPart, NULL);

    PrintExp(ifExp);*/

    scannerADT scanner = NewScanner();
    SetScannerString(scanner, "2+1337");
    expADT exp = ParseExp(scanner);
    PrintExp(exp);
    FreeScanner(scanner);

    printf("\n\n");
    system("pause");
}
