#ifndef STRINGOPS_H
#define STRINGOPS_H

#include "string.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stdio.h"

void split(char* string, char match, char*** out, int* count);
void replace(char* string, char target, char replaceWith);
void removeChar(char* string, char target);
void removeComments(char* string);

#endif