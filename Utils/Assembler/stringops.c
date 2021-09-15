#include "stringops.h"

void split(char* string, char match, char*** out, int* count)
{
	(*count) = 0;
	int len = strlen(string);
	int currentLen = 0;
	int currentString = 0;
	int currentIndex = 0;
	bool lastWasMatch = false;

	// Cut off the front end
	for(int i = 0; i < len; i++)
	{
		if(string[i] != match)
		{
			string = &string[i];
			break;
		}
	}

	// Length has changed
	len = strlen(string);

	// Cut off the back end
	if(string[len - 1] == match)
	{
		for(int i = len - 1; i >= 0; i--)
		{
			if(string[i] != match && i != len - 1)
			{
				string[i + 1] = 0;
				break;
			}
		}
	}
	

	// If after we trim the ends and find no actual string, return NULL
	len = strlen(string);
	if(len == 0)
	{
		out = NULL;
		(*count) = -1;
		return;
	}
	
	// Find out how many we are going to have
	for(int i = 0; i < len; i++)
	{
		if(string[i] == match)
		{
			if(!lastWasMatch)
			{
				lastWasMatch = true;
				(*count)++;
			}
		}
		else
		{
			lastWasMatch = false;
		}
	}

	// There is always at least one
	(*count)++;

	// Create array
	(*out) = (char**)malloc(sizeof(char*) * (*count));

	// Create strings in array
	for(int i = 0; i < len; i++)
	{
		if(string[i] == match)
		{
			if(!lastWasMatch)
			{
				lastWasMatch = true;
				(*out)[currentString] = (char*)malloc(sizeof(char) * currentLen + 1);
				(*out)[currentString][currentLen] = 0;
				currentLen = 0;
				currentString++;
			}
		}
		else
		{
			lastWasMatch = false;
			currentLen++;

			if(i == len - 1)
			{
				(*out)[currentString] = (char*)malloc(sizeof(char) * currentLen + 1);
				(*out)[currentString][currentLen] = 0;
			}
		}
	}

	currentString = 0;

	// Transfer strings
	for(int i = 0; i < len; i++)
	{
		if(string[i] == match)
		{
			if(!lastWasMatch)
			{
				lastWasMatch = true;
				currentString++;
				currentIndex = 0;
			}
		}
		else
		{
			lastWasMatch = false;
			(*out)[currentString][currentIndex] = string[i];
			currentIndex++;
		}
	}
}

void replace(char* string, char target, char replaceWith)
{
	int len = strlen(string);
	for(int i = 0; i < len; i++)
	{
		if(string[i] == target)
		{
			string[i] = replaceWith;
		}
	}
}

void removeChar(char* string, char target)
{
	int len = strlen(string);
	int index = 0;
	for(int i = 0; i < len; i++)
	{
		if(string[i] != target)
		{
			string[index] = string[i];
			index++;
		}
	}
}

void removeComments(char* string)
{
	int len = strlen(string);
	for(int i = 0; i < len - 1; i++)
	{
		if(string[i] == '/' && string[i + 1] == '/')
		{
			string[i] = 0;
		}
	}
}