#pragma once
#include "CONST.h"

struct FileData
{
	char** data;
	int length;
	int lastLength;
};

struct ForwardInfo
{
	list<char*> data;
	char fileName[BUFF_SIZE];
};

struct SearchInfo
{
	list<char*> data;
	char fileName[BUFF_SIZE];
};

int CheckIP(char* IP);
bool CheckConnect(char* address, int port);
char* StringToChars(string input);
