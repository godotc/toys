#include "parser.h"
#include "token.h"
#include <cstdio>

static void HandleTopLevelExpression();
static void HandleExtern();
static void HandleDefinition();

// top ::== definition | external | expression | ';'
static void MainLoop();

int main(int argc, char **argv)
{
    // Install standard binary operators.
    // 1 is lowest precedence.
    BinopPrecdence.insert({'<', 10});
    BinopPrecdence['+'] = 20;
    BinopPrecdence['-'] = 20;
    BinopPrecdence['*'] = 40; // highest.

    get_next_token();
    fprintf(stderr, "ready> ");
    MainLoop();

    return 0;
}



static void MainLoop()
{
    while (true) {
        auto type = token_str_mapping[cur_token.type];
        printf("%s\n", type.c_str());

        // def foo(x y) x+y y;
        switch (cur_token.type) {
        case tok_eof:
            return;
        case ';':
            break;
        case tok_def:
            // def foo(x y) x+y
            HandleDefinition();
            break;
        case tok_extern:
            HandleExtern();
            break;
        default: // tok_identifier or other
            // y;
            HandleTopLevelExpression();
            return;
        }
    }
}

static void HandleDefinition()
{
    if (auto FuncExpr = ParseDefinition()) {
        fprintf(stderr, "Parsed a function definition.\n");
        fprintf(stderr, "%s\n", FuncExpr->to_string().c_str());
        fflush(stderr);
    }
    else
        get_next_token();
}

static void HandleExtern()
{
    if (ParseExtern()) {
        fprintf(stderr, "Parsed an extern\n");
        fflush(stderr);
    }
    else
        get_next_token();
}

static void HandleTopLevelExpression()
{
    // Evaluate a top-level expression into an anonymous function.
    if (auto FuncExpr = ParseTopLevelExpr()) {
        fprintf(stderr, "Parsed a top-level expr\n");

        fflush(stderr);
    }
    else
        get_next_token();
}
