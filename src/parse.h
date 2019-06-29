#pragma once
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char** parse_to_tokens(char *buffer);
void print_tokens(char **tokens);
void free_words(char **tokens);
int is_seperator(char *token);
