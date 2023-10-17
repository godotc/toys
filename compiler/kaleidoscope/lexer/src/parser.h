
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <iostream>
#include <map>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

#include "ast.h"
#include "token.h"

template <class T, class Dp = std::default_delete<T>>
using Uni = std::unique_ptr<T, Dp>;

// BinopPrecdence
extern std::unordered_map<int, int> BinopPrecdence;

static EToken get_next_token()
{
    cur_token.type = (EToken)read_token();
    auto str       = cur_token.to_string();
    fprintf(stderr, "%s", str.c_str());
    return cur_token.type;
}

std::unique_ptr<ExprAST>      LogError(const char *str);
std::unique_ptr<PrototypeAST> LogError_Prototype(const char *str);


Uni<PrototypeAST> ParseExtern();
Uni<FunctionAST>  ParseTopLevelExpr();

Uni<ExprAST>      ParsePrimary();
Uni<PrototypeAST> ParsePrototype();
Uni<FunctionAST>  ParseDefinition();

Uni<ExprAST> ParseExpression();
Uni<ExprAST> ParseIdentifierExpr();

Uni<ExprAST> ParseParenExpr();
Uni<ExprAST> ParseNumberExpr();
Uni<ExprAST> ParseBinOpRHS(int ExprPrec, Uni<ExprAST> lhs);



static int GetTokenPrecedence()
{
    if (!isascii(cur_token.type)) {
        return -1;
    }

    int TokPrec = BinopPrecdence[cur_token.type];
    //    fprintf(stderr, "Size: %d\n", BinopPrecdence.size());
    //    for (auto &[_, v] : BinopPrecdence) {
    //        fprintf(stderr, "%d\n", v);
    //    }
    return TokPrec <= 0 ? -1 : TokPrec;
}
