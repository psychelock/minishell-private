#pragma once
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#include "parse.h"
#include "builtins.h"
#include "ast.h"

int execute_tree (struct Node *root, int returnval);
int execute_node(char **tokens);
int execute_pipes(struct Node *root);
