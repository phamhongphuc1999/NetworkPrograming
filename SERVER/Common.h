#pragma once
#include <string>
#include <time.h>
#include <fstream>
#include <list>
#include <map>
#include <winsock2.h>
#include <WS2tcpip.h>
#include "CONST.h"
using namespace std;

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

struct ForwardInfo
{
	char* parnerID;
	char* fileName;
	list<string> payload;
};

struct SearchInfo {
	char* fileName;
	list<string> Yes;
	list<string> No;
	int status;
};

struct SESSION {
	char* ID;
	ForwardInfo forwardInfo;
	map<string, SearchInfo> searchInfo;
	SOCKET connSock;
};

static int MessageSize = sizeof(Message);

void InitiateSession(struct SESSION* session);
bool CheckRamdomID(map<string, SESSION*> listSession, char* ID);
char* CreateRamdomID();
char* StringToChars(string input);
void CreateMessage(Message* message, int type, char* data, ForwardInfoClient* forwardInfo);
