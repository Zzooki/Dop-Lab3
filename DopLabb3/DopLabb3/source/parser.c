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

	if (isdigit(token[0]))
		return NewIdentifierExp(token);

    int intVal = StringToInteger(token);

    return NewIntegerExp(intVal);
}

expADT ReadC(scannerADT scanner) {
    expADT exp   = ReadF(scanner);
	string token = ReadToken(scanner);

	if (!StringEqual(token, "(")) {
		SaveToken(scanner, token);
		return exp;
	}

	expADT arg = ReadE(scanner);

	token = ReadToken(scanner);
	if (!StringEqual(token, ")"))
		Error("nix, det ska va en parentes");

    return NewCallExp(exp, arg);
}

expADT ReadT(scannerADT scanner) {
    expADT exp   = ReadC    (scanner);
    string token = ReadToken(scanner);

    //--------
    // C * T
    //--------
	if (StringEqual(token, "*")) return NewCompoundExp('*', exp, ReadT(scanner));
    
	//--------
    // C / T
    //--------
	if (StringEqual(token, "/")) return NewCompoundExp('/', exp, ReadT(scanner));
		
    //--------
    // C
    //--------
	SaveToken(scanner, token);

    return exp;
 
}

expADT ReadE(scannerADT scanner) {
    expADT exp   = ReadT    (scanner);
    string token = ReadToken(scanner);

    //--------
    // T + E
    //--------
	if (StringEqual(token, "+")) return NewCompoundExp('+', exp, ReadE(scanner));
    
	//--------
    // T - E
    //--------
	if (StringEqual(token, "-")) return NewCompoundExp('-', exp, ReadE(scanner));
		
    //--------
    // T
    //--------
	SaveToken(scanner, token);

    return exp;
}

expADT ParseExp(scannerADT scanner) {
	expADT exp;
	exp = ReadE(scanner);
	if (MoreTokensExist(scanner)){
		Error("ParseExp: %s unexpected!", ReadToken(scanner));
	}
	return(exp);
}
