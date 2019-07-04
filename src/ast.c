#include "ast.h"

int number(char **tokens)
{
    int count = 0;
    for(int i = 0; tokens[i]; i++)
        count++;
    return count;
}

int has_redir(char *string)
{
    return ((strchr(string, '>') != NULL) || (strchr(string, '<') != NULL));
}

void free_redir(struct Redir *node)
{ 
    struct Redir *tmp;
    while(node != NULL)
    {
        tmp = node;
        node = node->next;
        free_words(tmp->string);
        free(tmp);
    }
}

void free_tree_aux(struct Node *tree)
{
    if(!tree)
        return;
    if(tree->left)
        free_tree(tree->left);
    if(tree->right)
        free_tree(tree->right);
    if(tree->command)
        free_words(tree->command);
    if(tree->redir)
        free_redir(tree->redir);
    free(tree);
}


int redirection(int i, char *string)
{
    if(string[i] == '<')
        return 2;
    if(string[i+1] && string[i+1] == '>')
        return 1;
    return 0;
}

int get_ionum(int i , char *string)
{
    int res = -1;
    if(i >= 1)
    {
        if(string[i-1] && isdigit(string[i-1]))
            res = string[i-1] - '0';
    }
    return res;
}

int get_index(char *string, char c)
{
    char *p = strchr(string, c);
    if(p)
        return p - string;
    return BUFSIZ;
}

struct Redir* create_redir(int ionum, int redir, char **file, int n)
{
    struct Redir *res = (struct Redir*)calloc(1, sizeof(struct Redir));
    res->ionum = ionum;
    res->redir = redir;
    char **resfile = (char **)calloc(n+1, sizeof(char *));
    for(int i = 0; i < n; i++)
    {
        resfile[i] = strdup(file[i]);
    }
    resfile[n]= NULL;
    res->string = resfile;
    res->next = NULL;
    return res;
}

struct Redir* create_redir_list(char **tokens)
{
    int count = 0;
    if(!tokens)
        return NULL;
    int i = 0;
    for(; tokens[i]; i++)
    {
        if(!has_redir(tokens[i]))
            count++;
        else
            break;
    }
    if(i < number(tokens))
    {
        int a = get_index(tokens[i], '<');
        int b = get_index(tokens[i], '>');
        int index = (a < b) ? a : b;
        int ionum = get_ionum(index, tokens[i]);
        int redir = redirection(index, tokens[i]);
        struct Redir *tmp = create_redir(ionum, redir, tokens, count);
        struct Redir *next = create_redir_list(tokens+count+1);
        tmp->next = next;
        return tmp;
    }
    else
    {
        return create_redir(-2, -1, tokens, count);
    }
}

struct Node* create_node(char *args)
{
    struct Node* res = (struct Node *)calloc(1, sizeof(struct Node));
    res->left = NULL;
    res->right = NULL;
    res->redir = NULL;
    res->pipe = NULL;
    int redirection = has_redir(args);
    char **cmd = (char **)calloc(1, sizeof(char *));
    cmd[0] = NULL;
    int count = 1;
    char *p = strtok(args, " ");
    while(p)
    {
        cmd[count-1] = (char *)calloc(255, sizeof(char));
        memcpy(cmd[count-1], p, strlen(p)+1);
        count++;
        cmd = realloc(cmd, sizeof(char *) * (count));
        cmd[count-1] = NULL;
        p = strtok(NULL, " ");
    }
    if (!redirection)
    {
        res->command = cmd;
        res->redir = NULL;
    }
    else
    {
        res->command = NULL;
        struct Redir *lol = create_redir_list(cmd);
        res->redir = lol;
        free_words(cmd);
    }
    return res;
}

struct Node* build_tree(char **tokens, int i, int *eaten)
{
    if(!tokens)
        return NULL;
    struct Node *tree = NULL;
    for(; tokens[i]; i++)
    {
        struct Node *tmp = create_node(tokens[i]);
        if(is_seperator(tokens[i]))
        {
            tmp->left = tree;
            tree = tmp;
        }
        else if (strcmp(tokens[i], "|") == 0)
        {
            free_tree_aux(tmp);
            tree->pipe  = build_tree(tokens, i+1, eaten);
            i += *eaten;
        }
        else
        {
            if(!tree)
                tree = tmp;
            else
                tree->right = tmp;
        }
        (* eaten)++;
        if(i >= number(tokens))
            break;
    }
    return tree;
}

void print_node(struct Node *tree)
{
    if(tree->left)
        print_node(tree->left);
    print_tokens(tree->command);
    if(tree->right)
        print_node(tree->right);
}

void free_tree(struct Node *tree)
{
    struct Node *tmp;
    while(tree != NULL)
    {
        tmp = tree;
        tree = tree->pipe;
        free_tree_aux(tmp);
    }
}

