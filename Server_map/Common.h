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

void InitiateSession(struct SESSION* session);
bool CheckRamdomID(map<string, SESSION*> listSession, char* ID);
char* CreateRamdomID();
char* StringToChars(string input);
void CreateDATA(char* ID, char* fileName, char* data);
char* CreateDATA(char* ID, char* fileName);
