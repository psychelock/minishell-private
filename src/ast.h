#pragma once
#include <stdio.h>

#include "parse.h"

struct Node
{
    struct Node *left;
    struct Node *right;
    char **command;
};

struct Node* create_node(char *args);
struct Node* build_tree(char **tokens);
void print_node(struct Node *tree);
void free_tree(struct Node *tree);
