%{
#include <string.h>
	int chars = 0;
	int words = 0;
%}

%%
[a-zA-Z]+ { chars+=strlen(yytext); words++; }
. { }
%%


/* word_char_counter.l */
int main(int argc, char **argv) {
    yylex();
    printf("I found %d words of %d chars.\n", words, chars);
    return 0;
}
