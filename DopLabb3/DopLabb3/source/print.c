//------------------------------------------------
// INCLUDES
//------------------------------------------------

#include "env.h"
#include "genlib.h"
#include "print.h"

//------------------------------------------------
// FUNCTIONS
//------------------------------------------------

// Skriver ut ett uttrycktstr�d till k�llkod. Vi kan anv�nda funktionen f�r att
// verifiera att parsning fungerar korrekt.
void PrintExp(expADT exp) {
    exptypeT expType = ExpType(exp);

    // Beroende p� vad vi har f�r typ av uttryck, s� skriver vi ut det p� olika
    // s�tt. Se de olika case-satserna med tillh�rande kommentarer nedan.

    switch (expType) {

    //--------------------------------------------
    // func(<arg>) { <exp> }
    //
    // Ex: func(x) { ... }
    //--------------------------------------------
    case FuncExp: {
        string funcFormalArg = GetFuncFormalArg(exp);
        expADT funcBodyExp   = GetFuncBody     (exp);

        printf("func (%s) { ", funcFormalArg);

        if (funcBodyExp)
            PrintExp(funcBodyExp);

        printf(" }");

        break;
    }

    //--------------------------------------------
    // if <lhsExp> <relOp> <rhsExp>
    //     then <thenExp>
    //    [else <elseExp>]
    //
    // Ex: if x < 10 then ...
    //
    //     if x = y then ... else ...
    //--------------------------------------------
    case IfExp: {
        char   relOp   = GetIfRelOp        (exp);
        expADT lhsExp  = GetIfLHSExpression(exp);
        expADT rhsExp  = GetIfRHSExpression(exp);
        expADT thenExp = GetIfThenPart     (exp);
        expADT elseExp = GetIfElsePart     (exp);

        printf("if ");

        PrintExp(lhsExp);

        printf(" %c ", relOp);

        PrintExp(rhsExp);

        printf(" then ");

        PrintExp(thenExp);

        if (elseExp) {
            printf(" else ");

            PrintExp(elseExp);
        }

        break;
    }

      
    //--------------------------------------------
    // <func>(<arg>)
    //
    // Ex: succ(5)
    //--------------------------------------------
    case CallExp: {
        expADT argExp  = GetCallActualArg(exp);
        expADT callExp = GetCallExp      (exp);

        printf("\nkolla om detta st�mmer lol\n");

        PrintExp(callExp);

        printf("(");

        PrintExp(argExp);

        printf(")");

        break;
    }

    //--------------------------------------------
    // <heltal>
    //
    // Ex: 1337
    //--------------------------------------------
    case ConstExp: {
        int val = ExpInteger(exp);

        printf("%d", val);

        break;
    }

    //--------------------------------------------
    // <variabel>
    //
    // Ex: x
    //
    //     myVar
    //--------------------------------------------
    case IdentifierExp: {
        string ident = ExpIdentifier(exp);

        printf("%s", ident);

        break;
    }

    //--------------------------------------------
    // <lhsExp> <op> <rhsExp>
    //
    // Ex: x = 5
    //
    //     3 * y
    //--------------------------------------------
    case CompoundExp: {
        char   op     = ExpOperator(exp);
        expADT lhsExp = ExpLHS     (exp);
        expADT rhsExp = ExpRHS     (exp);

        PrintExp(lhsExp);

        printf(" %c ", op);

        PrintExp(rhsExp);

        break;
    }


    } // switch (expType)
}

void PrintValue(valueADT value) {
    // Ingen vet vad den h�r funktionen �r till f�r. Den verkar ha tappat sitt
    // syfte. Rykten g�r om att den k�nner sig sjukt ensam och utanf�r. Kan
    // n�gon vara v�nlig och anropa den? Tack p� f�rhand.
	// Haha s�t kommentar! 

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
