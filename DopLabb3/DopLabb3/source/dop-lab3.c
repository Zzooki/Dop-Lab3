#include "exception.h"
#include "eval.h"
#include "exp.h"
#include "parser.h"
#include "print.h"
#include "simpio.h"
#include "strlib.h"
#include "symtab.h"

#include <stdio.h>

typedef void(*commandFnT)(void);
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

main() {
    printf("FML Interpreter by c0debr0 & m1ss sWaG\n\n");
	printf("Type :help to get available commands!\n");

    scannerADT scanner = NewScanner();
    SetScannerSpaceOption(scanner, IgnoreSpaces);
    environmentADT globalEnv = NewEnvironment();


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

		if (s[0] == ':') {
			executeCommand(s + 1);
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

void initCommandTable(void){
	commandTable = NewSymbolTable();
	defineCommand("load", loadCmd);
	defineCommand("define", defineCmd);
	defineCommand("help", helpCmd);
	defineCommand("quit", quitCmd);
	defineCommand("type", quitCmd);
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

	entry = Lookup(commandTable, cmd);
	if (entry == UNDEFINED){
		printf("Undefined command: %s\n", cmd);
		return;
	}
	entry->fn();
}
void loadCmd(void){
	printf("load");
}
void defineCmd(void){
	printf("define");
}
void helpCmd(void){
	printf("The available commands are:\n:load\n:define\n:type\n:quit\n:swag");
}
void quitCmd(void){
	exit(EXIT_SUCCESS);
}

void swagCmd(void){
	printf("swag");
}
void typeCmd(void){
	printf("type");
}