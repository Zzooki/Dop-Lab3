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
	Om n�got finns kvar i scannern l�s in n�sta token fr�n scannern och avg�r om det �r
	"*" eller "/". Om inte, l�gg tillbaka inl�st token till scannern och returnera den
	sparade expADT f�r C. Anropande funktion tar hand om det som �terst�r i scannern.
	Om "*" eller "/" �terfanns, anropa ReadT rekursivt f�r att l�sa denna, bygg upp
	motsvarande expADT med hj�lp av T och den sparade C och returnera detta.*/

	/*
	Kontrollera om det finns n�gonting kvar i scannern. Om inte, returnera den sparade
	expADT f�r C.*/
 
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
