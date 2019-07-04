#pragma once
#include <stdio.h>

#include "parse.h"

struct Node
{
    struct Node *left;
    struct Node *right;
    char **command;
    struct Node *pipe;
};

struct Redir
{
    int ionum;
    char redir;
    char *string;
    struct Redir *next;
};

struct Node* create_node(char *args);
//struct Node* build_tree(char **tokens);
struct Node* build_tree(char **tokens, int i, int *eaten);
void print_node(struct Node *tree);
void free_tree(struct Node *tree);
