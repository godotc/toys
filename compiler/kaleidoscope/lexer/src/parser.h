
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <iostream>
#include <map>
#include <memory>
#include <utility>
#include <vector>

#include "ast.h"
#include "token.h"

template <class T, class Dp = std::default_delete<T>>
using Uni = std::unique_ptr<T, Dp>;

// BinopPrecdence
static std::map<char, int> BinopPrecdence;

static int CurTok;
static int get_next_token()
{
    return CurTok = gettok();
}


std::unique_ptr<ExprAST>      LogError(const char *str);
std::unique_ptr<PrototypeAST> LogError_Prototype(const char *str);


Uni<PrototypeAST> ParseExtern();
Uni<FunctionAST>  ParseTopLevelExpr();

Uni<ExprAST> ParsePrimary();
Uni<ExprAST> ParseNumberExpr();

Uni<ExprAST>      ParseNumberExpr();
Uni<ExprAST>      ParseExpression();
Uni<ExprAST>      ParseBinOpRHS(int ExprPrec, Uni<ExprAST> lhs);
Uni<ExprAST>      ParseParenExpr();
Uni<PrototypeAST> ParsePrototype();
Uni<FunctionAST>  ParseDefinition();
Uni<ExprAST>      ParseIdentifierExpr();



static int GetTokenPrecedence()
{
    if (!isascii(CurTok)) {
        return -1;
    }

    int TokPrec = BinopPrecdence[CurTok];
    return TokPrec <= 0 ? -1 : TokPrec;
}
