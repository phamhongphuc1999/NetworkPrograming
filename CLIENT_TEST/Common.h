#pragma once
#include "CONST.h"

struct FileData
{
	char* payload;
	int length;
};

int CheckIP(char* IP);
bool CheckConnect(char* address, int port);
char* StringToChars(string input);
