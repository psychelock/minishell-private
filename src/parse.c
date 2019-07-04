#include "parse.h"

int is_seperator(char *token)
{
    return ((strcmp(token, "&&") == 0) || (strcmp(token, "||") == 0) ||\
            (strcmp(token, ";")==0));
}

static int parse_error(char **tokens)
{
    int count = 0;
    for(int i = 0; tokens[i]; i++)
        count++;
    char *prev = NULL;
    char *next = tokens[1];
    for(int i = 0; tokens[i];)
    {
        if(!tokens[i+1])
            break;
        if(is_seperator(tokens[i]))
        {
            if(strcmp(tokens[i], ";") != 0)
            {
                if(!prev || !next || is_seperator(prev) || is_seperator(next)) 
                {
                    fprintf(stderr, "Parsing error\n");
                    return 1;
                }
            }
            else if (strcmp(tokens[i], ";") == 0)
            {
                if(!prev || is_seperator(prev))
                {
                    fprintf(stderr, "Parsing error\n");
                    return 1;
                }
            }
        }
        i++;
        prev = tokens[i-1];
        next = (i+1 != count) ? tokens[i+1]: NULL;
    }
    return 0;
}

void print_tokens(char **tokens)
{
    if(tokens)
    {
        for(int i=0; tokens[i]; i++)
            printf("%s \n", tokens[i]);
    }
}

void free_words(char **tokens)
{
    for(int i = 0; tokens[i]; i++)
    {
        free(tokens[i]);
    }
    if(tokens)
        free(tokens);
}

char** parse_to_tokens(char *buffer)
{
    char **res = (char **)calloc(1, sizeof(char *));
    res[0] = NULL;
    int rescount = 1;
    int start = 0;
    int len = 0;
    for(int i =0; buffer[i]; i++)
    {
        if(buffer[i] == ';')
        {
            if(len != 0)
            {
                rescount+=2;
                res = realloc(res, sizeof(char *) * (rescount));
                res[rescount-3] = (char *)calloc(255, sizeof(char));
                res[rescount-2] = (char *)calloc(255, sizeof(char));
                memcpy(res[rescount-3], buffer+start, len);
                memcpy(res[rescount-2], ";", 1);
                start += len;
                len = 0;
                res[rescount-1] = NULL;
            }
            else
            { 
                res[rescount-1] = (char *)calloc(255, sizeof(char));
                memcpy(res[rescount-1], ";", 1);
                start += len;
                len = 0;
                rescount++;
                res = realloc(res, sizeof(char *) * (rescount));
                res[rescount-1] = NULL;
            }
            start++;
            continue;
        }
        else if(buffer[i] == '&')
        {
            if(buffer[i+1] && buffer[i+1] == '&')
            {
               if(len != 0)
               {
                   rescount+=2;
                   res = realloc(res, sizeof(char *) * (rescount));
                   res[rescount-3] = (char *)calloc(255, sizeof(char));
                   res[rescount-2] = (char *)calloc(255, sizeof(char));
                   memcpy(res[rescount-3], buffer+start, len);
                   memcpy(res[rescount-2], "&&", 2);
                   start += len;
                   len = 0;
                   res[rescount-1] = NULL;
               }
               else
               {
                   res[rescount-1] = (char *)calloc(255, sizeof(char));
                   memcpy(res[rescount-1], "&&", 2);
                   start += len;
                   len = 0;
                   rescount++;
                   res = realloc(res, sizeof(char *) * (rescount));
                   res[rescount-1] = NULL;
               }
               i++;
               start+=2;
               continue;
            }
        }
        else if(buffer[i] == '|')
        {
            if(buffer[i+1] && buffer[i+1] == '|')
            {
                if(len != 0)
                {
                    rescount+=2;
                    res = realloc(res, sizeof(char *) * (rescount));
                    res[rescount-3] = (char *)calloc(255, sizeof(char));
                    res[rescount-2] = (char *)calloc(255, sizeof(char));
                    memcpy(res[rescount-3], buffer+start, len);
                    memcpy(res[rescount-2], "||", 2);
                    start += len;
                    len = 0;
                    res[rescount-1] = NULL;
                }
                else
                {
                    res[rescount-1] = (char *)calloc(255, sizeof(char));
                    memcpy(res[rescount-1], "||", 2);
                    start += len;
                    len = 0;
                    rescount++;
                    res = realloc(res, sizeof(char *) * (rescount));
                    res[rescount-1] = NULL;
                }
                i++;
                start+=2;
                continue;
            }
            else
            {
                if(len != 0)
                {
                    rescount+=2;
                    res = realloc(res, sizeof(char *) * (rescount));
                    res[rescount-3] = (char *)calloc(255, sizeof(char));
                    res[rescount-2] = (char *)calloc(255, sizeof(char));
                    memcpy(res[rescount-3], buffer+start, len);
                    memcpy(res[rescount-2], "|", 1);
                    start += len;
                    len = 0;
                    res[rescount-1] = NULL;
                }
                else
                { 
                    res[rescount-1] = (char *)calloc(255, sizeof(char));
                    memcpy(res[rescount-1], "|", 1);
                    start += len;
                    len = 0;
                    rescount++;
                    res = realloc(res, sizeof(char *) * (rescount));
                    res[rescount-1] = NULL;
                }
                start++;
                continue;
            }
        }
        else if(!buffer[i+1])
        { 
            res[rescount-1] = (char *)calloc(255, sizeof(char));
            memcpy(res[rescount-1], buffer+start, len+1);
            rescount++;
            res = realloc(res, sizeof(char *) * (rescount));
            res[rescount-1] = NULL;
            continue;
        }
        len++;
    }
    if(parse_error(res))
    {
        return res;
        free_words(res);
        return NULL;
    }
    return res;
}
