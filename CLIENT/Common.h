#pragma once
#include <string>
#include "Config.h"

struct FileData
{
	char** data;
	int length;
	int lastLength;
};

struct ForwardInfoSend
{
	char ID[30];
	char fileName[BUFF_SIZE];
	char pathToFile[BUFF_SIZE];
};

struct ForwardInfoReceive
{
	char partnerID[30];
	int lastLength;
	list<char*> data;
	char fileName[BUFF_SIZE];
};

struct SearchInfoSend
{
	char partnerID[30];
	int lastLength;
	list<char*> data;
	char fileName[BUFF_SIZE];
};

struct SearchInfoReceive
{
	char partnerID[30];
	char fileName[BUFF_SIZE];
	char pathToFile[BUFF_SIZE];
};

int CheckIP(char* IP);
bool CheckConnect(char* address, int port);
bool CheckRamdomID(char* ID, char* client_id);
char* StringToChars(string input);
