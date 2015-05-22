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

    case CallExp: {
        expADT         arg      = GetCallActualArg(exp);
        expADT         call     = GetCallExp(exp);
        string         funcName = ExpIdentifier(call);
        valueADT       func     = GetIdentifierValue(env, funcName);
        expADT         funcExp  = GetFuncValueBody(func);
        string         argName  = GetFuncFormalArg(funcExp);
        expADT         funcBody = GetFuncBody(funcExp);
        environmentADT closure  = GetFuncValueClosure(func);

        DefineIdentifier(env, argName, arg, closure);

        return Eval(funcBody, closure);

    }

    case FuncExp: {
        return NewFuncValue(GetFuncFormalArg(exp), GetFuncBody(exp), NewClosure(env));
    }

    case ConstExp: {
        return NewIntegerValue(ExpInteger(exp));
    }

    case IdentifierExp: {
        string ident = ExpIdentifier(exp);
        valueADT val = GetIdentifierValue(env, ident);

        if (ValueType(val) == FuncValue) {
            expADT         funcExp  = GetFuncValueBody(val);
            string         argName  = GetFuncValueFormalArg(val);
            environmentADT closure  = GetFuncValueClosure(val);

            if (argName)
                return val;

            return Eval(funcExp, closure);
        }

        return val;

    }

    case CompoundExp: {
        return EvalCompound(exp, env);

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

    expADT lhsExp = ExpLHS(exp);
    expADT rhsExp = ExpRHS(exp);
    lhs = Eval(lhsExp, env);
    rhs = Eval(rhsExp, env);

    int lhv, rhv;

    lhv = GetIntValue(lhs);
    rhv = GetIntValue(rhs);

    switch (op) {
      case '+': return NewIntegerValue(lhv + rhv);
      case '-': return NewIntegerValue(lhv - rhv);
      case '*': return NewIntegerValue(lhv * rhv);
      case '/': return NewIntegerValue(lhv / rhv);
      default:  Error("Illegal operator");
    }
}
