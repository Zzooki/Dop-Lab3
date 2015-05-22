#include "exception.h"
#include "eval.h"
#include "exp.h"
#include "parser.h"
#include "print.h"
#include "simpio.h"
#include "strlib.h"
#include "symtab.h"

#include <stdio.h>

typedef void(*commandFnT)(string);
typedef struct{
	commandFnT fn;
}*commandEntryT;
static symtabADT commandTable;

void initCommandTable(void);
void defineCommand(string cmd, commandFnT fn);
void executeCommand(string cmd);
void loadCmd(void);
void defineCmd(void);
void helpCmd(void);
void typeCmd(void);
void quitCmd(void);
void swagCmd(void);

environmentADT globalEnv;
scannerADT scanner;


main() {
    printf("FML Interpreter by c0debr0 & m1ss sWaG\n\n");
	printf("Type :help to get available commands!\n");

    scanner = NewScanner();
    SetScannerSpaceOption(scanner, IgnoreSpaces);

    globalEnv = NewEnvironment();
	initCommandTable();

    //string swag = "func (n) { 10*n }";
    //SetScannerString(scanner, swag);
    //expADT swagExp = ParseExp(scanner);
    //DefineIdentifier(globalEnv, "swag", swagExp, globalEnv);

    while (TRUE) {
        printf("> ");
        string s = GetLine();

        if (StringLength(s)==0)
            continue;
        try {
            if (s[0] == ':') {
                executeCommand(s + 1);
            }
            else {

                SetScannerString(scanner, s);

                expADT exp = NULL;

                exp = ParseExp(scanner);

                if (exp) {
                    valueADT val = Eval(exp, globalEnv);
                    if (val)
                        PrintValue(val);
                }
            }

        except(ANY)
            printf("Error: %s\n", (string)GetExceptionValue());
        printf("Line: %s\n", GetScannerString(scanner));
        } endtry;

        free(s);
    }

    FreeScanner(scanner);

    system("pause");
}

void initCommandTable(void){
	commandTable = NewSymbolTable();
    defineCommand("l", loadCmd);
    defineCommand("load", loadCmd);
    defineCommand("d", defineCmd);
    defineCommand("define", defineCmd);
    defineCommand("h", helpCmd);
    defineCommand("help", helpCmd);
    defineCommand("quit", quitCmd);
    defineCommand("q", quitCmd);
    defineCommand("type", typeCmd);
    defineCommand("t", typeCmd);
	defineCommand("swag", swagCmd);
}
void defineCommand(string cmd, commandFnT fn){
	commandEntryT entry;
	entry = New(commandEntryT);

	entry->fn = fn;
	Enter(commandTable, cmd, entry);

}
void executeCommand(string cmd){
	commandEntryT entry;

    string arg = NULL;
    for (int i = 0; i < strlen(cmd); i++) {
        char c = cmd[i];
        if (c == ' ') {
            cmd[i] = '\0';
            arg = cmd + i +1;
            break;
        }
    }

	entry = Lookup(commandTable, cmd);
	if (entry == UNDEFINED){
		printf("Undefined command: %s\n", cmd);
		return;
	}

	entry->fn(arg);
}
void loadCmd(string s) {
	FILE *newfile;
	string expression;
	newfile = fopen(s, "r");
	if (newfile == NULL) Error("Cannot find file, please swag it again!\n");
    string s2 = NULL;
	while ((expression = ReadLine(newfile)) != NULL)
	{
        if (expression[0] == '#' || expression[0] == '\0') continue;

        bool hasBrace = FALSE;
        for (int i = 0; i < strlen(expression); i++) {
            if (expression[i] == '{') {
                hasBrace = TRUE;
            }
            if (expression[i] == '}') {
                hasBrace = FALSE;
            }
        }

        if (!hasBrace) {
            s2 = expression;
            defineCmd(s2);
        }
        else {
            hasBrace = FALSE;
            s2 = expression;
            while (TRUE) {
                expression = ReadLine(newfile);
                if (expression[0] == '#' || expression[0] == '\0') continue;

                for (int i = 0; i < strlen(expression); i++) {
                    if (expression[i] == '}') {
                        hasBrace = TRUE;
                    }
                }

                s2 = Concat(Concat(s2, " "), expression);

                if (hasBrace) {
                    defineCmd(s2);
                    break;

                }
            }
        }

	}
}
void defineCmd(string s) {
    printf("%s\n", s);
    SetScannerString(scanner, s);

    string ident = ReadToken(scanner);

    if (isdigit(ident[0])) {
        Error("Expected identifier");
    }

    string assign = ReadToken(scanner);
    if (!StringEqual(assign, "="))
        Error("Expected =");

    expADT exp = ParseExp(scanner);

    environmentADT ctx = NewClosure(globalEnv);
    DefineIdentifier(globalEnv, ident, exp, ctx);
}
void helpCmd(string s) {
	printf("The available commands are:\n:load\n:define\n:type\n:quit\n:swag");
}
void quitCmd(string s) {
	exit(EXIT_SUCCESS);
}

void swagCmd(string s) {
	FILE *newfile;
	string expression, filename;
	filename = "swagfile.txt";
	newfile = fopen(filename, "r");
	printf("\n");
	if (newfile == NULL) Error("Cannot find file, please swag it again!\n");

	while ((expression = ReadLine(newfile)) != NULL)
	{
		printf("%s\n",expression);
	}
}
void typeCmd(string s){
    SetScannerString(scanner, s);

    valueADT val = Eval(ParseExp(scanner), globalEnv);

    if (ValueType(val) == IntValue) {
        int intVal = GetIntValue(val);

        if (intVal >= 0)
            printf("%d :: Int\n", intVal);
        else
            printf("%d :: Negative Swag Int\n", intVal);
    }
    else {
        printf("%s :: Function\n", s);
    }
}
