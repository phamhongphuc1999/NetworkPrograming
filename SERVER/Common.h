#pragma once
#include <string>
#include <time.h>
#include <fstream>
#include <list>
#include <winsock2.h>
#include <WS2tcpip.h>
using namespace std;

struct ForwardSession
{
	char* parnerID;
	char* fileName;
	list<char*> payload;
};

struct SESSION {
	char* ID;
	ForwardSession info;
	SOCKET connSock;
	list<SESSION*>::iterator position;
};

void InitiateSession(struct SESSION* session);
bool CheckRamdomID(list<SESSION*> listSession, char* ID);
char* CreateRamdomID();
