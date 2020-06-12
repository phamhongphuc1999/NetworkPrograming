#pragma once
#include "CONST.h"

struct FileData
{
	char** data;
	int length;
	int lastLength;
};

int CheckIP(char* IP);
bool CheckConnect(char* address, int port);
char* StringToChars(string input);
