#pragma once

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include "parse.h"

int is_builtin(char **token);
int builtin_echo(char **tokens, int print);
int builtin_cd(char **tokens);
int builtin_exit(char **tokens);
int builtin_kill(char **tokens);
