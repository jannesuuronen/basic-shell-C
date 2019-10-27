#include "shell.h"

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

char* read_shell_line()
{
    int line_buffer_size = MAX_LINE_BUFFER_SIZE;
    int cursor_pos = 0;
    char *line_buffer = malloc(sizeof(char) * line_buffer_size);
    //store character as int since EOF is of int type
    int character; 

    if(line_buffer == NULL)
    {
        fprintf(stderr, "Shell: Buffer allocation failed\n");
        exit(EXIT_FAILURE);
    }

    while (1)
    {

        character = getchar();
        

        if(character == EOF || character == '\n' || character == '\r')
        {
            line_buffer[cursor_pos] = '\0';
            return line_buffer;
        }
        else
        {
            line_buffer[cursor_pos] = character;
        }
        cursor_pos++;

        if(cursor_pos > line_buffer_size)
        {
            line_buffer_size += MAX_LINE_BUFFER_SIZE;
            line_buffer = realloc(line_buffer, line_buffer_size);
            if (line_buffer == NULL)
            {
                fprintf(stderr, "Shell: Buffer reallocation failed\n");
                exit(EXIT_FAILURE);
            }
            
        }

    }
    
}
/*
 * UNIX version of read_shell_line. Requires a UNIX system in order to be ran since getline() is
 * being used.  
*/
char* read_line_shell()
{
    char* line = NULL;
    ssize_t line_size = 0;
    getline(&line, &line_size, stdin);
    return line;
}

char **split_shell_line(char* line)
{
    int buffer_size = MAX_TOKEN_BUFFER_SIZE, cursor_pos = 0;
    char **tokens = malloc(sizeof(char*) * buffer_size);
    char *token;

    if(tokens == NULL)
    {
        fprintf(stderr, "Shell: Allocation failed, split_shell_line");
        exit(EXIT_FAILURE);        
    }

    token = strtok(line, TOKEN_DELIMITERS);
    while(token != NULL)
    {
        tokens[cursor_pos++] = token;
        if(cursor_pos >= buffer_size)
        {
            buffer_size += MAX_TOKEN_BUFFER_SIZE;
            tokens = realloc(tokens, buffer_size * sizeof(char*));
            if(tokens == NULL)
            {
                fprintf(stderr, "Shell: Reallocation failed, split_shell_line");
                exit(EXIT_FAILURE);   
            }
        }

        token = strtok(NULL, TOKEN_DELIMITERS);
    }
    tokens[cursor_pos] = NULL;
    return tokens;
}

int launch_process(char **args)
{
    pid_t pid, wpid;
    int status;

    pid = fork();
    if(pid == 0)
    {
        if(execvp(args[0], args) == -1)
        {
            perror("Failed to execute command in child process.\n");
        }
        exit(EXIT_FAILURE);
    }
    //still in parent process
    else if(pid < 0)
    {
        perror("Failed to fork process");
    }
    else
    {
        do
        {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        
    }
    return 1;
}

int execute_command(char **args)
{
    if(args[0] == NULL)
    {
        return 1;
    }

    for (int i = 0; i < get_num_builtins(); i++)
    {
        if(strcmp(args[0], builtin_cmds[i]) == 0)
        {
            return (*builtin_functions[i]) (args);
        }
        
    }
    
}

void run_shell_loop()
{
    char* line;
    char** line_args;
    int status;

    do {
        printf(">");
        // printf("Reading Line");
        line = read_shell_line();
        line_args = split_shell_line(line);
        status = execute_command(line_args);
        // printf("Status: %d", status);
        free(line);
        free(line_args);
    } while(status);
}