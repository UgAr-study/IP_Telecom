#pragma once

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "header.h"

int is_delim (char c);
int is_command (const char * word);
char * getWord (char *text, int *len);
struct Lexem getNextLexem (char *text);
void print_lex (struct Lexem lex);
