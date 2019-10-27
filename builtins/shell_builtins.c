#include "shell_builtins.h"

int get_num_builtins()
{
    return (sizeof(builtin_cmds) / sizeof(char *));
}

int builtin_cd(char **args)
{
    if(args[1] == NULL)
    {
        fprintf(stderr, "cd: expected argument was missing\n");        
    }
    else
    {
        if(chdir(args[1]) != 0) 
        {
            perror("cd");
        }
    }
    return 1;
}

int builtin_help(char **args)
{
    printf("---- Basic UNIX shell ----\n");
    printf("Type program names and arguments, and hit enter.\n");
    printf("The following commands are builtin: \n");
    for (int i = 0; i < get_num_builtins(); i++)
    {
        printf(" %s\n", builtin_cmds[i]);
    }

    printf("Use the man command for information on other programs\n");
    return 1;
}

int builtin_exit()
{
    return 0;
}