#pragma once
#include <string>
#include <time.h>
#include <fstream>
#include <list>
#include <map>
#include <winsock2.h>
#include <WS2tcpip.h>
using namespace std;

struct ForwardInfo
{
	char* parnerID;
	char* fileName;
	list<char*> payload;
};

struct SearchInfo {
	char* fileName;
	list<char*> Yes;
	list<char*> No;
	int status;
};

struct SESSION {
	char* ID;
	ForwardInfo* forwardInfo;
	map<string, SearchInfo> searchInfo;
	SOCKET connSock;
};

void InitiateSession(struct SESSION* session);
bool CheckRamdomID(map<string, SESSION*> listSession, char* ID);
char* CreateRamdomID();
