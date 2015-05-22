#include "exception.h"
#include "eval.h"
#include "exp.h"
#include "parser.h"
#include "print.h"
#include "simpio.h"
#include "strlib.h"

#include <stdio.h>

main() {
    printf("FML Interpreter by c0debr0 & m1ss sWaG\n\n");

    scannerADT scanner = NewScanner();
    SetScannerSpaceOption(scanner, IgnoreSpaces);
    environmentADT globalEnv = NewEnvironment();

    string swag = "func (n) { 10*n }";
    SetScannerString(scanner, swag);
    expADT swagExp = ParseExp(scanner);
    DefineIdentifier(globalEnv, "swag", swagExp, globalEnv);

    while (TRUE) {
        printf("> ");
        string s = GetLine();

        if (StringLength(s)==0)
            continue;

        if (s[0] == ':') {
            // Det �r ett kommando, hantera det h�r.
            continue;
        }

        SetScannerString(scanner, s);

        expADT exp = NULL;
        try {
            exp = ParseExp(scanner);

            if (exp) {
                valueADT val = Eval(exp, globalEnv);
                if (val)
                    PrintValue(val);
            }

        except(ANY)
            printf("Error: %s\n", (string)GetExceptionValue());
        } endtry;

        free(s);
    }

    FreeScanner(scanner);

    system("pause");
}
