#pragma once
#include "CONST.h"

struct Message {
	int type;
	char data[BUFF_SIZE];
};

static int MessageSize = sizeof(Message);

int CheckIP(char* IP);
bool CheckConnect(char* address, int port);
char* StringToChars(string input);
