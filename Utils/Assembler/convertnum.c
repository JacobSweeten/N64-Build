#include "convertnum.h"

int convertNum(char* num)
{
	int out = 0;
	char** dump;
	int len = strlen(num);

	if(!strncmp(num, "0x", 2))
	{
		for(int i = 2; i < len; i++)
		{
			if(!((num[i] >= 48 && num[i] <= 57) ||
				(num[i] >= 65 && num[i] <= 70)))
				return -1;
		}

		return(strtol(&num[2], dump, 16));
	}
	else if(num[len - 1] == 'b')
	{
		num[len - 1] = '\0';

		for(int i = 0; i < len - 1; i++)
		{
			if(num[i] != '0' && num[i] != '1')
				return -1;
		}

		return(strtol(num, dump, 2));
	}
	else if(num[len - 1] == 'h')
	{
		num[len - 1] = '\0';

		for(int i = 0; i < len - 1; i++)
		{
			if(!((num[i] >= 48 && num[i] <= 57) ||
				(num[i] >= 65 && num[i] <= 70)))
				return -1;
		}

		return(strtol(num, dump, 16));
	}
	else
	{
		for(int i = 0; i < len; i++)
		{
			if(!(num[i] >= 48 && num[i] <= 57))
				return -1;
		}

		return(atoi(num));
	}
}