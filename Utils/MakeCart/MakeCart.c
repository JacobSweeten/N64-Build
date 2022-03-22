#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "common.h"

#define USAGE_MESSAGE "Usage: makecart [bootFile] [gameCodeFile] [assetFile] [outFile]\n"

typedef struct headerData_s {
	int			romSize;
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
	int			assetStart;
	uint8_t*	game;
	int			gameLen;
	uint8_t*	assets;
	int			assetLen;
} HeaderData_t;

uint8_t* buildHeader(HeaderData_t* dh)
{
	uint8_t* header = (uint8_t*)malloc(dh->romSize);
	
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
	*(uint32_t*)&header[0x1000] = swapEndianness32(dh->romSize);
	*(uint32_t*)&header[0x1004] = swapEndianness32(dh->assetStart);
	memcpy(&header[0x1008], dh->game, dh->gameLen);
	memcpy(&header[0x1008 + dh->gameLen], dh->assets, dh->assetLen);

	return header;
}

int main(int argc, char** argv)
{
	if(argc != 5)
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

	FILE* gameFile = fopen(argv[2], "r");
	if(bootFile == NULL)
	{
		printf(USAGE_MESSAGE);
		return 1;
	}

	FILE* assetFile = fopen(argv[3], "r");
	if(assetFile == NULL)
	{
		printf(USAGE_MESSAGE);
		return 1;
	}

	FILE* outFile = fopen(argv[4], "w");
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

	// Load boot file
	fseek(bootFile, 0L, SEEK_END);
	int bootLen = ftell(bootFile);
	fseek(bootFile, 0L, SEEK_SET);
	fread(headerData.bootCode, 1, bootLen, bootFile);

	for(int i = 0; i < (4032 - bootLen); i++)
	{
		headerData.bootCode[i + bootLen] = 0;
	}

	// Load game file
	fseek(gameFile, 0L, SEEK_END);
	int gameLen = ftell(gameFile);
	fseek(gameFile, 0L, SEEK_SET);

	headerData.game = (uint8_t*)malloc(gameLen);
	headerData.gameLen = gameLen;

	fread(headerData.game, 1, gameLen, gameFile);

	headerData.assetStart = gameLen + 0x80000400;

	// Load asset file
	fseek(assetFile, 0L, SEEK_END);
	int assetFileLen = ftell(assetFile);
	fseek(assetFile, 0L, SEEK_SET);

	headerData.assets = (uint8_t*)malloc(assetFileLen);
	headerData.assetLen = assetFileLen;

	fread(headerData.assets, 1, assetFileLen, assetFile);

	headerData.romSize = 0x1000 + 8 + gameLen + assetFileLen;

	uint8_t* out = buildHeader(&headerData);

	fwrite(out, 1, headerData.romSize, outFile);
	
	fclose(bootFile);
	fclose(gameFile);
	fclose(assetFile);
	fclose(outFile);

	free(out);
	free(headerData.bootCode);
	free(headerData.game);
	free(headerData.assets);
	return 0;
}