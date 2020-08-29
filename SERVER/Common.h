#pragma once
#include <string>
#include <time.h>
#include <fstream>
#include <list>
#include <map>
#include <winsock2.h>
#include <WS2tcpip.h>
#include "Config.h"
using namespace std;

struct SearchInfo {
	char* fileName;
	char* ID;
	int count = 0;
	int status = 0;
};

struct SESSION {
	char* ID;
	map<string, SearchInfo> searchInfo;
	SOCKET connSock;
};

void InitiateSession(struct SESSION* session);
bool CheckRamdomID(map<string, SESSION*> listSession, char* ID);
char* CreateRamdomID();
char* StringToChars(string input);
