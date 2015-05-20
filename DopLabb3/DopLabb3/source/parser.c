//------------------------------------------------
// INCLUDES
//------------------------------------------------

#include "exp.h"
#include "parser.h"
#include "strlib.h"

expADT ParseExp(scannerADT scanner);
expADT ReadE(scannerADT scanner);
expADT ReadT(scannerADT scanner);
expADT ReadC(scannerADT scanner);
expADT ReadF(scannerADT scanner);

//------------------------------------------------
// FUNCTIONS
//------------------------------------------------

expADT ReadF(scannerADT scanner) {
    string token = ReadToken(scanner);
	char relOp;
	expADT elsePart, ifPart;
	string identifier;

	if (StringEqual(token, "(")){
		expADT arg = ReadE(scanner);
		token = ReadToken(scanner);
		if (!StringEqual(token, ")"))
			Error("Syntax fel!");
		return(arg);
	}
	if (StringEqual(ConvertToLowerCase(token), "if")){
		expADT lhs = ReadE(scanner);
		token = ReadToken(scanner);
		if (StringEqual(token, "="))
			relOp = '=';
		else if (StringEqual(token, "<"))
				relOp = '<';
		else if(StringEqual(token, ">"))
			relOp = '>';
		else
			Error("Either =, < or >");
		expADT rhs = ReadE(scanner);
		token = ReadToken(scanner);
		if (StringEqual(ConvertToLowerCase(token), "then")){
			ifPart = ReadE(scanner);
		}
		else Error("Then?");
		token = ReadToken(scanner);
		if (StringEqual(ConvertToLowerCase(token), "else")){
			elsePart = ReadE(scanner);
		}
		else{
			Error("Missing else!!");
			
		}
		return(NewIfExp(lhs, relOp, rhs, ifPart, elsePart));
			
	}
	if (StringEqual(ConvertToLowerCase(token), "func")){
		token = ReadToken(scanner);
		if (StringEqual(token, "(")){
			identifier = ReadToken(scanner);
			token = ReadToken(scanner);
			if (!StringEqual(token, ")")) 
				Error("Syntax fel!");
			token = ReadToken(scanner);
			if (StringEqual(token, "{")){
				expADT body = ReadE(scanner);
				token = ReadToken(scanner);
				if (!StringEqual(token, "}"))
					Error("Missing }!");
				return(NewFuncExp(identifier, body));
			}
			else Error("Missing {!");
		}
		else Error("Missing (!");
	}

	if (!isdigit(token[0]))
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
