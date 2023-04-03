
#include "any_out.h"

int main()
{
    int  a = 1;
    int  b = 2;
    int *p = &a;
    p      = &b;
    *p     = 1;


    int c = 1;
    int d = 2;

    int const *cp = &c;
    out(cp);
    cp = &d;
    out(cp);
    // *cp = 3; // can not change  value

    int *const pc = &c;
    out(pc);
    // pc = &d; // can not change addrsss
    *pc = 3;


    // so
    //  int const *  -> cont int*
    //  int * const
}
