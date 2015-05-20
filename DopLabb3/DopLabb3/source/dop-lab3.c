#include "exception.h"
#include "exp.h"
#include "parser.h"
#include "simpio.h"
#include "strlib.h"

#include <stdio.h>

main() {
    scannerADT scanner = NewScanner();

    while (TRUE) {
        printf("\n> ");
        string s = GetLine();

        if (StringLength(s)==0)
            continue;

        if (s[0] == ':') {
            // Det är ett kommando, hantera det här.
            continue;
        }

        SetScannerString(scanner, s);

        expADT exp = NULL;
        try {
        exp = ParseExp(scanner);

        except(ANY)
            printf("Error: %s\n", (string)GetExceptionValue());
        } endtry;

        if (exp)
            PrintExp(exp);

        free(s);
    }

    FreeScanner(scanner);

    system("pause");
}
