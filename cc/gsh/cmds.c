#include "cmds.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

char *p_builtinCmds[] = {"cd", "help", "exit"};

STATUS(*pfp_builtinFuc[])
(char **) = {
    &gsh_cd,
    &gsh_help,
    &gsh_exit,
};

STATUS gsh_bultinNums()
{
    return sizeof(p_builtinCmds) / sizeof(p_builtinCmds[0]);
}

STATUS gsh_cd(char **args)
{
    if (!args[1])
        perror("Gsh error at cd, lack of args\n");
    else
    {
        if (chdir(args[1]) != 0)
            perror("Gsh error at chdir\n");
    }

    return 1;
}

STATUS gsh_help(char **args)
{
    puts("This is godot42's shell");
    puts("There are some built in cmds:");

    for (int i = 0; i < gsh_bultinNums(); ++i)
    {
        printf("%s\n", p_builtinCmds[i]);
    }
    return 1;
}

STATUS gsh_exit(char **args)
{
    return 0;
}

STATUS gsh_launch(char **args)
{
    pid_t pid, wpid;
    STATUS status;

    pid = fork();
    if (pid == 0)
    {
        if (execvp(args[0], args) == -1)
        {
            perror("Gsh error at execvp\n");
        }
        exit(EXIT_FAILURE);
    }
    else
    {
        do
        {
            wpid = waitpid(pid, &status, WUNTRACED); // WUNTRACED: would not raced, 子线程进入暂停马上返回
        } while (!WIFEXITED(status) && !WIFSIGNALED(status)); //判断status非0 即正常退出  &&  没有收到信号(异常终止)
    }
    return 1;
}

STATUS gsh_execute(char **args)
{
    if (!args[0])
        return 1;

    for (int i = 0; i < gsh_bultinNums(); ++i)
    {
        if (strcmp(args[0], p_builtinCmds[i]) == 0)
        {
            return (*pfp_builtinFuc[i])(args);
        }
    }

    return gsh_launch(args);
}
