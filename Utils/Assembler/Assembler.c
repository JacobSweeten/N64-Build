#include "stdio.h"
#include "string.h"
#include "stdint.h"
#include "stdbool.h"

#include "stringops.h"
#include "opcodes.h"

#define USAGE_MESSAGE "Usage: assembler [file.asm] [out.bin]\n"

typedef struct instructionLink_s instructionLink_t;
typedef struct instructionLink_s {
	uint32_t instruction;
	instructionLink_t* next;
} instructionLink_t;

typedef struct label_s {
	char* labelName;
	int offset;
} label_t;

int main(int argc, char** argv)
{
	if(argc != 3)
	{
		printf(USAGE_MESSAGE);
		return 1;
	}

	FILE* inFile = fopen(argv[1], "r");
	if(inFile == NULL)
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

	uint32_t* out;
	instructionLink_t* head;
	instructionLink_t* next;
	int instructionCount = 0;

	// Get file length
	fseek(inFile, 0, SEEK_END);
	int inFileLen = ftell(inFile);
	fseek(inFile, 0, SEEK_SET);

	// Put file into a string
	char* inFileContents = (char*)malloc(sizeof(char) * inFileLen + 1);
	for(int i = 0; i < inFileLen; i++)
	{
		inFileContents[i] = fgetc(inFile);
	}

	// Clean up and break down file
	char** lines;
	int lineCount;
	
	// Unify whitespace
	replace(inFileContents, '\t', ' ');

	// Commas not needed. Replace with space.
	replace(inFileContents, ',', ' ');

	// Convert CRLF->LF
	removeChar(inFileContents, '\r');

	int offset = 0xA4000000;
	label_t labels[1000];
	int labelCount = 0;

	// Split by \n
	
	split(inFileContents, '\n', &lines, &lineCount);
	for(int i = 0; i < lineCount; i++)
	{
		char* instruction = lines[i];

		// Remove comments
		removeComments(instruction);
		char** args;
		int argCount;

		// Split by ' '
		split(instruction, ' ', &args, &argCount);

		bool isInstruction = true;
		
		// TODO: Find a more intuitive way to do this... (Function overloads?)
		int assembledInstruction;
		if(argCount == 1)
		{
			// Check for label or operation
			int argLen = strlen(args[0]);
		
			if(args[0][argLen - 1] == ':')
			{
				int labelOffset = offset + (instructionCount * 4);

				labels[labelCount].labelName = (char*)malloc(sizeof(char) * argLen);
				strcpy(labels[labelCount].labelName, args[0]);
				labels[labelCount].labelName[argLen -  1] = 0;

				labels[labelCount].offset = labelOffset;
				labelCount++;
				isInstruction = false;
			}
			else
			{
				assembledInstruction = assembleOperation(args[0], NULL, NULL, NULL);
			}
		}
		else if(argCount == 2)
		{
			assembledInstruction = assembleOperation(args[0], args[1], NULL, NULL);
		}
		else if(argCount == 3)
		{
			assembledInstruction = assembleOperation(args[0], args[1], args[2], NULL);
		}
		else if(argCount == 4)
		{
			assembledInstruction = assembleOperation(args[0], args[1], args[2], args[3]);
		}

		if(isInstruction)
		{
			printf("%08X\n", assembledInstruction);
			instructionCount++;
		}
	}

	printf("Labels:\n");
	for(int i = 0; i < labelCount; i++)
	{
		printf("\t%s: %08X\n", labels[i].labelName, labels[i].offset);
	}
}