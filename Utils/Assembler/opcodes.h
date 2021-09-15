#ifndef OPCODES_H
#define OPCODES_H

#include "stdio.h"
#include "stdint.h"
#include "string.h"
#include "stdbool.h"
#include "stdlib.h"
#include "convertnum.h"

extern const char* registers[32];
extern const char* OPCODES[64];
extern const char* SPECIAL[64];
extern const char* SPECIAL2[10];
extern const char* SPECIAL3[41];

int getRegister(char* registerName);
uint32_t assembleOperation(char* operation, char* operand1Str, char* operand2Str, char* operand3Str);

#endif