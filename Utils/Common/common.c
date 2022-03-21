#include "common.h"

uint64_t swapEndianness64(uint64_t val)
{
	uint64_t out = 0x0000000000000000;
	out += (val & 0xFF00000000000000) >> 56;
	out += (val & 0x00FF000000000000) >> 40;
	out += (val & 0x0000FF0000000000) >> 24;
	out += (val & 0x000000FF00000000) >> 8;
	out += (val & 0x00000000FF000000) << 8;
	out += (val & 0x0000000000FF0000) << 24;
	out += (val & 0x000000000000FF00) << 40;
	out += (val & 0x00000000000000FF) << 56;

	return out;
}

uint32_t swapEndianness32(uint32_t val)
{
	uint32_t out = 0x00000000;
	out += (val & 0xFF000000) >> 24;
	out += (val & 0x00FF0000) >> 8;
	out += (val & 0x0000FF00) << 8;
	out += (val & 0x000000FF) << 24;

	return out;
}

uint16_t swapEndianness16(uint16_t val)
{
	uint16_t out = 0x0000;
	out += (val & 0xFF00) >> 8;
	out += (val & 0x00FF) << 8;

	return out;
}

char* swapStringEndianness32(char* str, int len)
{
	uint32_t* tempArr;

	// Determine length. Add one if it doesn't fit prefectly.
	int divisions = len / 4;
	if(len%4 != 0)
		divisions += 1;

	// Allocate array
	tempArr = (uint32_t*)malloc(4 * divisions);

	// Make all elements spaces (except the last one)
	for(int i = 0; i < divisions - 1; i++)
		tempArr[i] = ' ';
	
	tempArr[divisions - 1] = 0x20202000;
	
	// Copy into tempArr, ignoring the type difference.
	strncpy((char*)tempArr, str, len);

	// Swap
	for(int i = 0; i < divisions; i++)
		tempArr[i] = swapEndianness32(tempArr[i]);

	// Cast back to char*
	char* out = (char*)tempArr;
	return out;
}