#include "stdio.h"
#include "stdint.h"
#include "string.h"
#include "stdlib.h"
#include "stdbool.h"

#define USAGE_MESSAGE "Usage: makeheader [bootFile] [outFile]\n"

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

typedef struct headerData_s {
	bool		bigEndian;					// 4 bits
	uint8_t		initailPI_BSB_DOM1_LAT_REG;	// 4 bits
	uint8_t		initialPI_BSD_DOM1_PGS_REG;	// 4 bits
	uint8_t		initialPI_BSD_DOM1_PWD_REG;
	uint8_t		initialPI_BSB_DOM1_PGS_REG;
	uint32_t	clockRateOverride;
	uint32_t	programCounter;
	uint32_t	releaseAdress;
	uint32_t	CRC1;
	uint32_t	CRC2;
	// 8 bytes of unused space
	char*		name;						// 20 bytes
	// 4 bytes of unused space
	uint8_t		mediaFormat;
	char*		ID;							// 2 bytes
	uint8_t		regionLanguage;
	uint16_t	catridgeID;
	uint8_t		countryCode;
	uint8_t		version;
	uint8_t*	bootCode;					// 4032 bytes
} HeaderData_t;

uint8_t* buildHeader(HeaderData_t* dh)
{
	uint8_t* header = (uint8_t*)malloc(0x1000);
	
	if(dh->bigEndian)
		header[0x00] = 0x80;
	else
		header[0x00] = 0x00;

	header[0x01] = (dh->initailPI_BSB_DOM1_LAT_REG << 4) + dh->initialPI_BSD_DOM1_PGS_REG;
	header[0x02] = dh->initialPI_BSD_DOM1_PWD_REG;
	header[0x03] = dh->initialPI_BSB_DOM1_PGS_REG;
	*(uint32_t*)&header[0x04] = swapEndianness32(dh->clockRateOverride);
	*(uint32_t*)&header[0x08] = swapEndianness32(dh->programCounter);
	*(uint32_t*)&header[0x0C] = swapEndianness32(dh->releaseAdress);
	*(uint32_t*)&header[0x10] = swapEndianness32(dh->CRC1);
	*(uint32_t*)&header[0x14] = swapEndianness32(dh->CRC2);
	memset(&header[0x18], 0, 8);
	memset(&header[0x20], 0, 20);
	strncpy(&header[0x20], dh->name, 20);
	memset(&header[0x34], 0, 4);
	header[0x38] = dh->mediaFormat;
	strncpy(&header[0x39], dh->ID, 2);
	header[0x3B] = dh->regionLanguage;
	*(uint16_t*)&header[0x3C] = swapEndianness16(dh->catridgeID);
	header[0x3E] = dh->countryCode;
	header[0x3F] = dh->version;
	memcpy(&header[0x40], dh->bootCode, 4032);

	return header;
}

int main(int argc, char** argv)
{
	if(argc != 3)
	{
		printf(USAGE_MESSAGE);
		return 1;
	}

	FILE* bootFile = fopen(argv[1], "r");
	if(bootFile == NULL)
	{
		printf(USAGE_MESSAGE);
		return 1;
	}

	FILE* outFile = fopen(argv[2], "w");
	if(outFile == NULL)
	{
		printf(USAGE_MESSAGE);
		return 1;
	}

	HeaderData_t headerData;
	headerData.bigEndian = true;
	headerData.initailPI_BSB_DOM1_LAT_REG = 0x03;
	headerData.initialPI_BSD_DOM1_PGS_REG = 0x07;
	headerData.initialPI_BSD_DOM1_PWD_REG = 0x12;
	headerData.initialPI_BSB_DOM1_PGS_REG = 0x40;
	headerData.clockRateOverride = 0x00000000;
	headerData.programCounter = 0;
	headerData.releaseAdress = 0;
	headerData.CRC1 = 1;
	headerData.CRC2 = 1;
	headerData.name = "Test ROM";
	headerData.mediaFormat = 'N';
	headerData.ID = "TH";
	headerData.regionLanguage = 'N';
	headerData.catridgeID = 0;
	headerData.countryCode = 'E';
	headerData.version = 0;
	headerData.bootCode = (uint8_t*)malloc(4032);
	fread(headerData.bootCode, 1, 4032, bootFile);

	uint8_t* out = buildHeader(&headerData);

	fwrite(out, 1, 0x1000, outFile);
	
	fclose(bootFile);
	fclose(outFile);

	free(out);
	free(headerData.bootCode);
	return 0;
}