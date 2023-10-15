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

    fprintf(stderr, "ready> ");
    get_next_token();

    MainLoop();

    return 0;
}



static void MainLoop()
{
    while (true) {
        fprintf(stderr, "ready> ");

        switch (cur_token.type) {
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

static void HandleDefinition()

{
    if (ParseDefinition()) {
        fprintf(stderr, "Parsed a function definition.\n");
        fflush(stderr);
    }
    else {
        // Skip cur_token for error recovery.
        // if previous is def, did not update, will go this way to read next token
        // But one line from stdin in done, so it will block here
        get_next_token();
    }
}

static void HandleExtern()
{
    if (ParseExtern()) {
        fprintf(stderr, "Parsed an extern\n");
        fflush(stderr);
    }
    else {
        // Skip cur_token for error recovery.
        get_next_token();
    }
}

static void HandleTopLevelExpression()
{
    // Evaluate a top-level expression into an anonymous function.
    if (ParseTopLevelExpr()) {
        fprintf(stderr, "Parsed a top-level expr\n");
        fflush(stderr);
    }
    else {
        // Skip cur_token for error recovery.
        get_next_token();
    }
}
