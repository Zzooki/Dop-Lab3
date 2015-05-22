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


main() {
    printf("FML Interpreter by c0debr0 & m1ss sWaG\n\n");
	printf("Type :help to get available commands!\n");

    scannerADT scanner = NewScanner();
    SetScannerSpaceOption(scanner, IgnoreSpaces);

    globalEnv = NewEnvironment();
	initCommandTable();

    string swag = "func (n) { 10*n }";
    SetScannerString(scanner, swag);
    expADT swagExp = ParseExp(scanner);
    DefineIdentifier(globalEnv, "swag", swagExp, globalEnv);

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
        } endtry;

        free(s);
    }

    FreeScanner(scanner);

    system("pause");
}

void initCommandTable(void){
	commandTable = NewSymbolTable();
	defineCommand("load", loadCmd);
	defineCommand("define", defineCmd);
	defineCommand("help", helpCmd);
	defineCommand("quit", quitCmd);
	defineCommand("type", typeCmd);
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
	if (newfile = NULL) Error("Cannot find file, please swag it again!\n");
	while (expression = ReadLine(newfile != EOF))
	{
		if (!expression[0] == '#') defineCmd(expression);
	}
}
void defineCmd(string s) {
	
}
void helpCmd(string s) {
	printf("The available commands are:\n:load\n:define\n:type\n:quit\n:swag");
}
void quitCmd(string s) {
	exit(EXIT_SUCCESS);
}

void swagCmd(string s) {
	printf("swag");
}
void typeCmd(string s){
    scannerADT scanner = NewScanner();
    SetScannerSpaceOption(scanner, IgnoreSpaces);

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
