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
	char* searchID;
	char* requestID;
	char* parnerID;
	char* fileName;
	list<string> Yes;
	list<string> No;
	list<string> payload;
};

struct SESSION {
	char* ID;
	ForwardInfo forwardInfo;
	SOCKET connSock;
};

void InitiateSession(struct SESSION* session);
void InitiateSearchInfo(struct SearchInfo* info);
bool CheckRamdomIDForSESSION(map<string, SESSION*> mapSession, char* ID);
bool CheckRamdomIDForSearch(map<string, SearchInfo*> mapSearch, char* ID);
bool CheckRamdomIDForForward(map<string, ForwardInfo*> mapForward, char* ID);
char* CreateRamdomID();
char* StringToChars(string input);
void CreateDATA(char* ID, char* fileName, char* data);
char* CreateDATA(char* ID, char* fileName);
