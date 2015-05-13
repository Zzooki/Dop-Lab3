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
    expADT exp = ReadC(scanner);

    return exp;
}

expADT ReadE(scannerADT scanner) {
    expADT exp = ReadT(scanner);

    return exp;
}

expADT ParseExp(scannerADT scanner) {
    return ReadE(scanner);
}
