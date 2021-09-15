#include "opcodes.h"

const char* registers[32] = {
	"ZERO",
	"AT",
	"V0",
	"V1",
	"A0",
	"A1",
	"A2",
	"A3",
	"T0",
	"T1",
	"T2",
	"T3",
	"T4",
	"T5",
	"T6",
	"T7",
	"S0",
	"S1",
	"S2",
	"S3",
	"S4",
	"S5",
	"S6",
	"S7",
	"T8",
	"T9",
	"K0",
	"K1",
	"GP",
	"SP",
	"S8",
	"RA"
};

const char* OPCODES[64] = {
	"SPECIAL",			// 0
	"REGIMM",
	"J",
	"JAL",
	"BEQ",
	"BNE",
	"BLEZ",
	"BGTZ",
	"ADDI",
	"ADDIU",
	"SLTI",				// 10
	"SLTIU",
	"ANDI",
	"ORI",
	"XORI",
	"AUI",
	"COP0",
	"COP1",
	"COP2",
	"COP1X",
	"BEQL",				// 20
	"BNEL",
	"BLEZL",
	"BGTZL",
	"DADDI",
	"DADDIU",
	"",
	"",
	"SPECIAL2",
	"JALX",
	"",					// 30
	"SPECIAL3",
	"LB",
	"LH",
	"",
	"LW",
	"LBU",
	"LHU",
	"",
	"LWU",
	"SB",				// 40
	"SH",
	"SWL",
	"SW",
	"SDL",
	"SDR",
	"SWR",
	"CACHE",
	"LL",
	"LWC1",
	"BC",				// 50
	"PREF",
	"LLD",
	"LDC1",
	"LDC2",
	"LD",
	"SC",
	"SWC1",
	"BALC",
	"PCREL",
	"SCD",				// 60
	"SDC1",
	"SDC2",
	"SD"
};

const char* SPECIAL[64] = {
	"SLL",			// 0
	"SRL",
	"",
	"SRA",
	"SLLV",
	"SRLV",
	"SRLV",
	"SRAV",
	"JR",
	"JALR",
	"MOVZ",				// 10
	"MOVN",
	"SYSCALL",
	"BREAK",
	"SDBBP",
	"SYNC",
	"CLZ",
	"CLO",
	"DCLZ",
	"DCLO/MTLO",
	"DSLLV",			// 20
	"DLSA",
	"DSRLV",
	"DSRAV",
	"MULT",
	"MULTU",
	"DIV",
	"DIVU",
	"DMULT",
	"DMULTU",
	"DDIV",				// 30
	"DDIVU",
	"ADD",
	"ADDU",
	"MFLO",
	"",
	"AND",
	"OR",
	"XOR",
	"NOR",
	"",					// 40
	"",
	"SLT",
	"SLTU",
	"DADD",
	"DADDU",
	"DSUB",
	"DSUBU",
	"TGE",
	"TGEU",
	"TLT",				// 50
	"TLTU",
	"TEQ",
	"SELEQZ",
	"TNE",
	"SELNEZ",
	"DSLL",
	"",
	"DSRL",
	"DSRA",
	"DSLL32",			// 60
	"",
	"DSLR32/DSRL32",
	"DSRA32"
};

const char* SPECIAL2[10] = {
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	""
};

const char* SPECIAL3[41] = {
	"",				// 0
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",				// 10
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",				// 20
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",				// 30
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	""				// 40
};

int getRegister(char* registerName)
{
	int num = -1;
	for(int i = 0; i < 32; i++)
		if(!strcmp(registerName, registers[i]))
			num = i;

	return num;
}

uint32_t assembleOperation(char* operation, char* operand1Str, char* operand2Str, char* operand3Str)
{
	int opcode = 0;
	int specialCode = 0;
	bool found = false;
	bool isSpecial = false;
	for(int i = 0; i < 64; i++)
	{
		if(!strcmp(OPCODES[i], operation))
		{
			opcode = i;
			found = true;
		}
	}

	if(!found)
	{
		for(int i = 0; i < 64; i++)
		{
			if(!strcmp(SPECIAL[i], operation))
			{
				specialCode = i;
				isSpecial = true;
			}
		}
	}

	uint32_t out = 0;
	out += (opcode << 26);
	if(isSpecial)
		out += specialCode;

	int dst;
	int src;
	int val;

	if(operand1Str != NULL)
		dst = getRegister(operand1Str);
	if(operand2Str != NULL)
		src = getRegister(operand2Str);
	if(operand3Str != NULL)
		val = convertNum(operand3Str);

	//printf("%i %i %i %i\n", opcode, dst, src, val);

	switch(opcode)
	{
		case 0:
			switch(specialCode)
			{
				case 0:
					out = 0;
					break;
			}
			break;
		case 9:
			out += (src << 21);
			out += (dst << 16);
			out += (val);
			break;
	}

	return out;
}