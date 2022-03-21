#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

uint64_t swapEndianness64(uint64_t val);

uint32_t swapEndianness32(uint32_t val);

uint16_t swapEndianness16(uint16_t val);

char* swapStringEndianness32(char* str, int len);

#endif