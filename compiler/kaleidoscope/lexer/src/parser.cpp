#include "parser.h"
#include "ast.h"
#include <algorithm>
#include <memory>
#include <utility>
#include <vector>

std::unique_ptr<ExprAST> LogError(const char *str)
{
    fprintf(stderr, "Error: %s\n", str);
    return nullptr;
}

std::unique_ptr<PrototypeAST> LogError_Prototype(const char *str)
{
    LogError(str);
    return nullptr;
}

// parenexpr ::= number
Uni<ExprAST> ParseNumberExpr()
{
    auto res = std::make_unique<NumberExprAST>(NumVal);

    get_next_token();
    return res;
}



// parenexpr ::= expression
Uni<ExprAST> ParseExpression()
{
    auto lhs = ParsePrimary();
    if (!lhs)
        return nullptr;

    return ParseBinOpRHS(0, std::move(lhs));
}

// parenexpr ::= ( expression )
Uni<ExprAST> ParseParenExpr()
{
    get_next_token(); // eat (
    auto v = ParseExpression();
    if (!v)
        return nullptr;
    if (CurTok != ')') {
        return LogError("expect ')' ");
    }
    get_next_token(); // eat )
    return v;
}

// identifierexpr
// 	::= identifer
// 	::= identifer ( expression )
Uni<ExprAST> ParseIdentifierExpr()
{
    std::string IdentName = IdentifierStr;

    get_next_token(); // eat identifer

    if (CurTok != ('(')) {
        return std::make_unique<VariableExprAST>(IdentName);
    }

    get_next_token(); // eat (


    std::vector<Uni<ExprAST>> Args;

    if (CurTok != ')') {
        while (1) {
            if (auto arg = ParseExpression()) {
                Args.push_back(std::move(arg));
            }
            else {
                return nullptr;
            }

            if (CurTok == ')') {
                break;
            }
            if (CurTok != ',') {
                return LogError("Expect ')' or ',' in argument list");
            }

            get_next_token();
        }
    }

    get_next_token(); // eat )

    return std::make_unique<CallExprAST>(IdentName, std::move(Args));
}


// primary
// 	::= identifierexpr
// 	::= nubmerexpr
// 	::= parenexpr
Uni<ExprAST> ParsePrimary()
{
    switch ((EToken)CurTok) {
    // case tok_eof:
    // case tok_def:
    // case tok_extern:
    case tok_identifier:
        return ParseIdentifierExpr();
    case tok_number:
        return ParseNumberExpr();
    case '(':
        return ParseParenExpr();

    default:
        return LogError("unknown token when expecting an expression");
    }
}



// binoprhs
// 	::= ( '+' primary )*
Uni<ExprAST> ParseBinOpRHS(int ExprPrec, Uni<ExprAST> lhs)
{
    while (1) {
        int TokPrec = GetTokenPrecedence();

        //
        if (TokPrec < ExprPrec)
            return lhs;

        int BinOp = CurTok;
        get_next_token();

        auto rhs = ParsePrimary();
        if (!rhs)
            return nullptr;

        int NextPrec = gettok();
        if (TokPrec < NextPrec) {
            rhs = ParseBinOpRHS(TokPrec + 1, std::move(rhs));

            if (!rhs) {
                return nullptr;
            }
        }

        // merge lhs rght
        lhs = std::make_unique<BinaryExprAST>(BinOp, std::move(lhs), std::move(rhs));
    }
}


// prototype
// 	::= id '(' id* ')'
Uni<PrototypeAST> ParsePrototype()
{

    if (CurTok != tok_identifier) {
        return LogError_Prototype("Expected function name in prototype");
    }
std:;
    std::string func_name = IdentifierStr;
    get_next_token();

    if (CurTok != '(') {
        return LogError_Prototype("Expected '(' in prototype");
    }

    std::vector<std::string> ArgNames;
    while (get_next_token() == tok_identifier) {
        ArgNames.push_back(IdentifierStr);
    }
    if (CurTok != ')') {
        return LogError_Prototype("Expected ')' in prototype");
    }

    get_next_token(); // eat ')'

    return std::make_unique<PrototypeAST>(func_name, std::move(ArgNames));
}


// definition ::= 'def' prototype expression
Uni<FunctionAST> ParseDefinition()
{
    get_next_token(); // eat def

    auto proto = ParsePrototype();
    if (!proto) {
        return nullptr;
    }

    if (auto expr = ParseExpression()) {
        return std::make_unique<FunctionAST>(std::move(proto), std::move(expr));
    }

    return nullptr;
}

// external ::= extern' prototype
Uni<PrototypeAST> ParseExtern()
{
    get_next_token(); // eat extern
    return ParsePrototype();
}

// toplevelexpr ::= expression
Uni<FunctionAST> ParseTopLevelExpr()
{
    if (auto expr = ParseExpression()) {
        // make an anoymous proto
        auto proto = std::make_unique<PrototypeAST>("", std::vector<std::string>{});
        return std::make_unique<FunctionAST>(std::move(proto), std::move(expr));
    }
    return nullptr;
}
