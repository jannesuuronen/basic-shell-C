#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include <unistd.h> 
#include "defines.h"
// #include "shell_builtins.h"
#ifndef SHELL_H
#define SHELL_H

int builtin_cd(char **args);

int builtin_help(char **args);

int builtin_exit();

char *builtin_cmds[] = {
    "cd",
    "help",
    "exit"
};

//functions pointers
int (*builtin_functions[]) (char **) = {
    &builtin_cd,
    &builtin_help,
    &builtin_exit
};

int get_num_builtins();

char* read_shell_line();

char* read_line_shell();

char **split_shell_line(char* line);

int launch_process(char **args);

int execute_command(char **args);

void run_shell_loop();
#endif // !1