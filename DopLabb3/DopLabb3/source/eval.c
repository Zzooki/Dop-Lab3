/*
 * File: eval.c
 * ------------
 * This file implements the eval.h interface.
 */

#include <stdio.h>
#include "genlib.h"
#include "strlib.h"
#include "exp.h"
#include "eval.h"
#include "symtab.h"

/* Private function prototypes */

static valueADT EvalCompound(expADT exp);

/* Exported entries */

valueADT Eval(expADT exp, environmentADT env)
{
    /*
    FuncExp,
    IfExp,
    CallExp,
    ConstExp,
    IdentifierExp,
    CompoundExp
    */


    switch (ExpType(exp)) {
    case IfExp: {

    }

    case FuncExp: {
        // fasen gör man nu då
    }

    case ConstExp: {
        return NewIntegerValue(ExpInteger(exp, env));
    }

    case IdentifierExp: {
        return (GetIdentifierValue(env, ExpIdentifier(exp, env)));

    }

    case CompoundExp: {
        return (EvalCompound(exp, env));

    }
    }
}


/* Private functions */

static valueADT EvalCompound(expADT exp, environmentADT env) {
    char op;
    valueADT lhs, rhs;

    op = ExpOperator(exp);
    if (op == '=') {
        rhs = Eval(ExpRHS(exp), env);
        string name = ExpIdentifier(ExpLHS(exp));
        DefineIdentifier(env, name, rhs, env);
        return (rhs);
    }

    lhs = Eval(ExpLHS(exp), env);
    rhs = Eval(ExpRHS(exp), env);

    switch (op) {
      case '+': return NewIntegerValue(GetIntValue(lhs) + GetIntValue(rhs));
      case '-': return NewIntegerValue(GetIntValue(lhs) - GetIntValue(rhs));
      case '*': return NewIntegerValue(GetIntValue(lhs) * GetIntValue(rhs));
      case '/': return NewIntegerValue(GetIntValue(lhs) / GetIntValue(rhs));
      default:  Error("Illegal operator");
    }
}
