#include <bits/types/FILE.h>
#include <cstdio>
#include <stdexcept>
#include <stdio.h>
#include <string>
using std::string;


enum State
{
    INIT = 0,
    NUM  = 1
};

int main()
{
    printf("arthmetic_test\n");
    FILE *fp = fopen("../sample/1_arithmetic/test_expression.txt", "r");
    char  ch;
    if (!fp) {
        throw std::runtime_error("filed to open file");
    }

    State state = INIT;
    int   num   = 0;

    while ((ch = std::getc(fp)) && ch != EOF) {
        if (ch == ' ' || ch == '\n') {
            if (state == NUM) {
                printf("token NUM: %d\n", num);
                state = INIT;
                num   = 0;
            }
        }

        else if (ch >= '0' && ch < '9') {
            state = NUM;
            num   = num * 10 + ch - '0';
        }

        else if (string("+-*/").find(ch) != string::npos) {

            if (state == NUM) {
                printf("token NUM: %d\n", num);
                state = INIT;
                num   = 0;
            }

            printf("token operator: %c\n", ch);
            state = INIT;
        }
    }

    fclose(fp);
    return 0;
}
