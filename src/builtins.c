#include <stdio.h>
#include <stdlib.h>
#include "builtins.h"

#define CD "cd"
#define EXIT "exit"
#define ECHO "echo"
#define KILL "kill"


int is_builtin(char **tokens)
{
    if(strcmp(tokens[0], ECHO) == 0)
        return 1;
    else if(strcmp(tokens[0], CD) == 0)
        return 2;
    else if(strcmp(tokens[0], EXIT) == 0)
        return 3;
    else if(strcmp(tokens[0], KILL) == 0)
        return 4;
    else
        return 0;
}
int builtin_echo(char **args, int print)
{
    for(int i = 0; args[i]; i++)
        fprintf(stdout, args[i]);
    if(print)
        fprintf(stdout, "\n");
    fflush(stdout);
    return 0;
}

int builtin_cd(char **args)
{
    int count = 0;
    for(int i = 0; args[i]; i++)
        count++;
    if(count != 1)
    {
        fprintf(stderr, "Invalid number of args\n");
        return 1;
    }
    else
    {
        int a = chdir(args[0]);
        if(a)
            fprintf(stderr, "cd failed\n");
        return a;   
    }
}

int builtin_exit(char **args)
{
    int count = 0;
    for(int i = 0; args[i]; i++)
        count++;
    if(count > 1)
        return 1;
    else if(count == 0)
    {
        return 420;
    }
    else if((int)strlen(args[0]) != snprintf(NULL, 0, "%i", atoi(args[0])))
        fprintf(stderr, "Invalid argument");
    return atoi(args[0]) + 420;
}
