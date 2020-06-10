#pragma once
#include "CONST.h"

struct ForwardInfoClient
{
	char fileName[BUFF_SIZE];
	char partnerID[BUFF_SIZE];
};

struct Message {
	int type;
	char data[BUFF_SIZE];
	ForwardInfoClient* forwardInfo;
};

static int MessageSize = sizeof(Message);

int CheckIP(char* IP);
bool CheckConnect(char* address, int port);
char* StringToChars(string input);
void CreateMessage(Message* message, int type, char* data, ForwardInfoClient* forwardInfo);
