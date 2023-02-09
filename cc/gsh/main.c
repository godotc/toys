#include <malloc.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "cmds.h"

#ifdef __linux__
#define GSH_TOK_DELIM " \t\r\n"
#endif

#define GSH_TOK_BUFFER_SIZE 64

/***************************************/
void gsh_loop();
char *gsh_readLine();
char **gsh_splitLine();
int gsh_execute(char **args);
/***************************************/

int main()
{
    gsh_loop();
    return 0;
}

void gsh_loop()
{
    char *line;
    char **args; // split a line
    STATUS status;

    do
    {
        char path[100];
        getcwd(path, 100);

        char now[200] = "[gsh ";

        //
        strcat(now, path);
        strcat(now, " $]");
        fprintf(stdout, "%s", now);

        // read, parse, execute
        line = gsh_readLine();
        args = gsh_splitLine(line);
        status = gsh_execute(args);

        // free(line); // line will be NULL after strtok in gsh_splitLine()
        free(args);

    } while (status);
}

char *gsh_readLine()
{
    char *line;
    size_t bufsize;
    getline(&line, &bufsize, stdin);
    return line;
}

char **gsh_splitLine(char *line)
{
    char **tokens = malloc(GSH_TOK_BUFFER_SIZE * sizeof(char *));
    int position = 0;
    char *one_token;

    one_token = strtok(line, GSH_TOK_DELIM);
    while (one_token != NULL)
    {
        tokens[position++] = one_token;
        one_token = strtok(NULL, GSH_TOK_DELIM); // 调用一次后会把 Line 置为 NULL
    }
    tokens[position] = NULL;

    return tokens;
}
