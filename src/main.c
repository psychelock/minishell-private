#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "parse.h"
#include "ast.h"
#include "execute.h"

int main(int argc, char **argv)
{
    int returnval = 0;
    FILE *input = stdin;
    if (argc == 2) // file input
    { 
        char *buf = NULL;
        size_t len = 0;
        input = fopen(argv[1], "r");
        if (!input)
        {
            printf("File failed to open");
            exit(127);
        }
        while(getline(&buf, &len, input) != -1)
        {
            if(buf[strlen(buf)-1] == '\n')
                buf[strlen(buf)-1] = 0;
            char **lol = parse_to_tokens(buf);
            if(*lol)
            {
                int eaten = 0;
                int *peaten = &eaten;
                struct Node *root = build_tree(lol, 0, peaten);
                free_words(lol);
                returnval = execute_pipes(root);
                if(returnval >= 420)
                {
                    free_tree(root);
                    free(buf);
                    fclose(input);
                    exit(returnval - 420);
                }
                free_tree(root);
            }
        }
        free(buf);
        fclose(input);
    }
    else
    {
        char *buf = NULL;
        size_t len = 0;
        if(isatty(fileno(stdin))) //interactive
        {
            while(1)
            {
                printf("minishell$ ");
                getline(&buf, &len, input);
                if(buf[strlen(buf)-1] == '\n')
                    buf[strlen(buf)-1] = 0;
                char ** lol = parse_to_tokens(buf);
                if(*lol)
                {
                    int eaten = 0;
                    int *peaten = &eaten;
                    struct Node *root = build_tree(lol, 0, peaten);
                    free_words(lol);
                    returnval = execute_pipes(root);
                    if(returnval >= 420)
                    {
                        free_tree(root);
                        free(buf);
                        exit(returnval - 420);
                    }
                    free_tree(root);
                }
            }
            free(buf);
        }
        else    //pipeline
        {
            getline(&buf, &len, stdin);
            if(buf[strlen(buf)-1] == '\n')
                buf[strlen(buf)-1] = 0;
            char ** lol = parse_to_tokens(buf);
            if(*lol)
            {
                int eaten = 0;
                int *peaten = &eaten;
                struct Node *root = build_tree(lol, 0, peaten);
                free_words(lol);
                returnval = execute_pipes(root);
                if(returnval >= 420)
                {
                    free_tree(root);
                    free(buf);
                    exit(returnval - 420);
                }
                free_tree(root);
            }
            free(buf);
        }
        return 0;
    }
}
