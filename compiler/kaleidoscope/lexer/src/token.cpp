
#include "token.h"

char read_char()
{
    static std::string input = "def f(x y) x+y y;";
    static int         index = 0;
    if (index == input.size())
        exit(-1);
    fprintf(stdout, "read a char: %c\n", input[index]);
    fflush(stdout);
    return input[index++];
}
