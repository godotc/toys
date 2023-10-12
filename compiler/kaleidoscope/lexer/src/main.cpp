#include "parser.h"
#include "token.h"
#include <cstdio>



static void HandleDefinition()
{
    if (ParseDefinition()) {
        fprintf(stderr, "Parsed a function definition.\n");
    }
    else {
        // Skip token for error recovery.
        get_next_token();
    }
}

static void HandleExtern()
{
    if (ParseExtern()) {
        fprintf(stderr, "Parsed an extern\n");
    }
    else {
        // Skip token for error recovery.
        get_next_token();
    }
}

static void HandleTopLevelExpression()
{
    // Evaluate a top-level expression into an anonymous function.
    if (ParseTopLevelExpr()) {
        fprintf(stderr, "Parsed a top-level expr\n");
    }
    else {
        // Skip token for error recovery.
        get_next_token();
    }
}


// top ::== definition | external | expression | ';'
static void MainLoop()
{
    while (1) {
        fprintf(stderr, "ready> ");

        switch ((EToken)CurTok) {
        case tok_eof:
            return;
        case ';':
            get_next_token();
            break;
        case tok_def:
            HandleDefinition();
            break;
        case tok_extern:
            HandleExtern();
            break;
        default:
            HandleTopLevelExpression();
            return;
        }
    }
}


int main(int argc, char **argv)
{
    // Install standard binary operators.
    // 1 is lowest precedence.
    BinopPrecdence['<'] = 10;
    BinopPrecdence['+'] = 20;
    BinopPrecdence['-'] = 20;
    BinopPrecdence['*'] = 40; // highest.

    fprintf(stderr, "ready> ");
    get_next_token();

    MainLoop();

    return 0;
}
