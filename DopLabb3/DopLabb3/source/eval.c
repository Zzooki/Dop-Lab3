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

int recursion = 0;

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
      case '/':
          if (rhv == 0)
              Error("Division by zero.");
          return NewIntegerValue(lhv / rhv);
      default:  Error("Illegal operator");
    }
}


valueADT EvalConst(expADT exp, environmentADT env) {
    return NewIntegerValue(ExpInteger(exp));
}


valueADT EvalIdentifier(expADT exp, environmentADT env) {
    string id = ExpIdentifier(exp);

    valueADT ident = GetIdentifierValue(env, id);
    environmentADT closure = GetFuncValueClosure(ident);
    expADT identFunc = GetFuncValueBody(ident);
    return Eval(identFunc, closure);
}

valueADT EvalIf(expADT exp, environmentADT env) {
    expADT lhs = GetIfLHSExpression(exp);
    expADT rhs = GetIfRHSExpression(exp);

    environmentADT closure = NewClosure(env);

    int lhv = GetIntValue(Eval(lhs, closure));
    int rhv = GetIntValue(Eval(rhs, closure));

    char relOp = GetIfRelOp(exp);

    expADT thenPart = GetIfThenPart(exp);
    expADT elsePart = GetIfElsePart(exp);

    switch (relOp) {
    case '=':
        if (lhv == rhv)
            return Eval(thenPart, env);
        return Eval(elsePart, env);
    case '<':
        if (lhv < rhv)
            return Eval(thenPart, env);
        return Eval(elsePart, env);
    case '>':
        if (lhv > rhv)
            return Eval(thenPart, env);
        return Eval(elsePart, env);
    }
}

valueADT EvalFunc(expADT exp, environmentADT env) {
    return NewFuncValue(GetFuncFormalArg(exp), GetFuncBody(exp), NewClosure(env));
}

valueADT EvalCall(expADT exp, environmentADT env) {
    expADT callExp = GetCallExp(exp);
    expADT actualArg = GetCallActualArg(exp);

    valueADT val = Eval(callExp, env);

    if (ValueType(val) == FuncValue) {
        environmentADT closure = GetFuncValueClosure(val);
        string argName = GetFuncValueFormalArg(val);
        DefineIdentifier(closure, argName, actualArg, env);
        return Eval(GetFuncValueBody(val), closure);
    }

    return val;

    //environmentADT closure = GetFuncValueClosure()
    //DefineIdentifier(closure, argName, actualArg, env);
}

valueADT Eval(expADT exp, environmentADT env) {
    recursion++;
    if (recursion > 500) {
        recursion = 0;
        Error("Recursion too deep.\n");
    }

    valueADT val = NULL;
    switch (ExpType(exp)) {
    case IfExp:         val = EvalIf(exp, env); break;
    case CallExp:       val =  EvalCall(exp, env);break;
    case FuncExp:       val =  EvalFunc(exp, env);break;
    case ConstExp:      val = EvalConst(exp, env); break;
    case IdentifierExp: val = EvalIdentifier(exp, env); break;
    case CompoundExp:   val = EvalCompound(exp, env); break;

    }
    recursion--;
    return val;
}
