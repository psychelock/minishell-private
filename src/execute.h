#pragma once
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include "parse.h"
#include "builtins.h"
#include "ast.h"

int execute_tree (struct Node *root);
int execute_node(char **tokens);
