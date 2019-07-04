#include "ast.h"

struct Node* create_node(char *args)
{
    struct Node* res = (struct Node *)calloc(1, sizeof(struct Node));
    res->left = NULL;
    res->right = NULL;
    res->pipe = NULL;
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
    res->command = cmd;
    return res;
}


struct Node* build_tree(char **tokens, int i, int *eaten)
{
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
    if(!tree)
        return;
    if(tree->left)
        free_tree(tree->left);
    if(tree->right)
        free_tree(tree->right);
    free_words(tree->command);
    free(tree);
}
