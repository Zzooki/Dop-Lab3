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

static valueADT EvalCompound(expADT exp, environmentADT env);

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
        expADT lhs = GetIfLHSExpression(exp);
        expADT rhs = GetIfRHSExpression(exp);
        char relOp = GetIfRelOp(exp);
        expADT ifPart = GetIfThenPart(exp);
        expADT elsePart = GetIfElsePart(exp);

        valueADT val1 = Eval(lhs, env);
        valueADT val2 = Eval(rhs, env);

        if (relOp == '=') {
            if (GetIntValue(val1) == GetIntValue(val2))
                return Eval(ifPart, env);
            return Eval(elsePart, env);
        }
        else if (relOp == '<') {
            if (GetIntValue(val1) < GetIntValue(val2))
                return Eval(ifPart, env);
            return Eval(elsePart, env);
        }
        else if (relOp == '>') {
            if (GetIntValue(val1) > GetIntValue(val2))
                return Eval(ifPart, env);
            return Eval(elsePart, env);
        }
    }

    case FuncExp: {
        // fasen g�r man nu d�
        break;
    }

    case ConstExp: {
        return NewIntegerValue(ExpInteger(exp));
    }

    case IdentifierExp: {
        return (GetIdentifierValue(env, ExpIdentifier(exp)));

    }

    case CompoundExp: {
        return (EvalCompound(exp, env));

    }
    }

    return NULL;
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
