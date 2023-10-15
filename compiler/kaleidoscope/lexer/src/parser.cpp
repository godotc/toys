#include "parser.h"
#include "ast.h"
#include <algorithm>
#include <memory>
#include <utility>
#include <vector>


std::unordered_map<int, int> BinopPrecdence;

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
    auto res = std::make_unique<NumberExprAST>(cur_token.NumVal);

    get_next_token();
    return res;
}



// parenexpr ::= expression
Uni<ExprAST> ParseExpression()
{
    // if def a (b c) b+c+1

    // will get a  variable_epr of 'b'
    auto lhs = ParsePrimary();
    if (!lhs)
        return nullptr;

    // then continue parse bin op
    return ParseBinOpRHS(0, std::move(lhs));
}

// parenexpr ::= ( expression )
Uni<ExprAST> ParseParenExpr()
{
    get_next_token(); // eat (
    auto v = ParseExpression();
    if (!v)
        return nullptr;
    if (cur_token.type != ')') {
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
    std::string IdentName = cur_token.IdentifierStr;

    get_next_token(); // eat identifer

    if (cur_token.type != ('(')) {
        return std::make_unique<VariableExprAST>(IdentName);
    }

    get_next_token(); // eat (

    std::vector<Uni<ExprAST>> Args;

    if (cur_token.type != ')') {
        while (1) {
            if (auto arg = ParseExpression()) {
                Args.push_back(std::move(arg));
            }
            else {
                return nullptr;
            }

            if (cur_token.type == ')') {
                break;
            }
            if (cur_token.type != ',') {
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
    switch ((EToken)cur_token.type) {
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
        // if def a(b c) b+c+1; will return the precedence of '+'
        int TokPrec = GetTokenPrecedence();

        // asif here is  precedenc['+'] < 0 is wrong, when meet no operation that we set will return
        if (TokPrec < ExprPrec) {
            return lhs;
        }

        int BinOp = cur_token.type;
        get_next_token(); // eat binop -> just to set cur_token as next token

        auto rhs = ParsePrimary();
        if (!rhs) {
            return nullptr;
        }

        int NextPrec = GetTokenPrecedence();
        // will pack the b+1 as rhs in a+b+1 (as example, here 2 '+' are same level, will run the #161? line
        if (TokPrec < NextPrec) {
            rhs = ParseBinOpRHS(TokPrec + 1, std::move(rhs));

            if (!rhs) {
                return nullptr;
            }
        }

        // a recursive operation ((lhs, rhs), rhs) rhs
        // merge lhs rght
        lhs = std::make_unique<BinaryExprAST>(BinOp, std::move(lhs), std::move(rhs));
    }
}


// prototype
// 	::= id '(' id* ')'
Uni<PrototypeAST> ParsePrototype()
{

    if (cur_token.type != tok_identifier) {
        return LogError_Prototype("Expected function name in prototype");
    }
std:;
    std::string func_name = cur_token.IdentifierStr;
    get_next_token(); // eat (

    if (cur_token.type != '(') {
        return LogError_Prototype("Expected '(' in prototype");
    }

    std::vector<std::string> ArgNames;
    while (get_next_token() == tok_identifier) {
        ArgNames.push_back(cur_token.IdentifierStr);
    }
    if (cur_token.type != ')') {
        return LogError_Prototype("Expected ')' in prototype");
    }

    get_next_token(); // eat ')'

    return std::make_unique<PrototypeAST>(func_name, std::move(ArgNames));
}


// definition ::= 'def' prototype expression
Uni<FunctionAST> ParseDefinition()
{
    // def a (b c) b+c+1
    get_next_token(); // eat def

    auto proto = ParsePrototype(); // def a (b c)
    if (!proto) {
        return nullptr;
    }

    //  a+b+1
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
