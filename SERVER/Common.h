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

struct ForwardInfo
{
	char* parnerID;
	char* fileName;
	list<char*> payload;
	int status;
};

struct SearchInfo {
	char* fileName;
	list<char*> Yes;
	list<char*> No;
	list<char*> payload;
	int status;
};

struct SESSION {
	char* ID;
	map<string, ForwardInfo> forwardInfo;
	map<string, SearchInfo> searchInfo;
	SOCKET connSock;
};

void InitiateSession(struct SESSION* session);
bool CheckRamdomID(map<string, SESSION*> listSession, char* ID);
char* CreateRamdomID();
char* StringToChars(string input);
