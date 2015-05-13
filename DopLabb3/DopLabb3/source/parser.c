//------------------------------------------------
// INCLUDES
//------------------------------------------------

#include "exp.h"
#include "parser.h"
#include "strlib.h"

//------------------------------------------------
// FUNCTIONS
//------------------------------------------------

expADT ReadF(scannerADT scanner) {
    string token = ReadToken(scanner);

    int intVal = StringToInteger(token);

    return NewIntegerExp(intVal);
}

expADT ReadC(scannerADT scanner) {
    expADT exp = ReadF(scanner);

    return exp;
}

expADT ReadT(scannerADT scanner) {
	/*
	Anropa ReadT. Om ingen T existerar anropas funktionen Error. Om en C existerar
	sparas motsvarande expADT.*/
	expADT exp = ReadC(scanner);
	string token = ReadToken(scanner);

	if (StringEqual(token, '*')){
		if (MoreTokensExist){
			return NewCompoundExp('*', exp, ReadT(scanner));

		}
		else Error("Invalid Expression!");		
	}
	if (StringEqual(token, '/')){
		if (MoreTokensExist){
			return NewCompoundExp('/', exp, ReadT(scanner));
		}
		else Error("Invalid Expression!");
	}
	else SaveToken(scanner, token);

	return(exp);
	 /*
	Om något finns kvar i scannern läs in nästa token från scannern och avgör om det är
	"*" eller "/". Om inte, lägg tillbaka inläst token till scannern och returnera den
	sparade expADT för C. Anropande funktion tar hand om det som återstår i scannern.
	Om "*" eller "/" återfanns, anropa ReadT rekursivt för att läsa denna, bygg upp
	motsvarande expADT med hjälp av T och den sparade C och returnera detta.*/

	/*
	Kontrollera om det finns någonting kvar i scannern. Om inte, returnera den sparade
	expADT för C.*/
 
}

expADT ReadE(scannerADT scanner) {
    expADT exp   = ReadT    (scanner);
    string token = ReadToken(scanner);

    //--------
    // T + E
    //--------
	if (StringEqual(token, "+")){
		if (MoreTokensExist){
			return NewCompoundExp('+', exp, ReadE(scanner));
		}
		else Error("Invalid Expression!");
	}
    //--------
    // T - E
    //--------
	else if (StringEqual(token, "-")){
		if (MoreTokensExist){
			return NewCompoundExp('-', exp, ReadE(scanner));
		}
		else Error("Invalid Expression!");
	}
		
    //--------
    // T
    //--------
    else
        SaveToken(scanner, token);

    return exp;
}

expADT ParseExp(scannerADT scanner) {
    return ReadE(scanner);
}
