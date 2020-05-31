#pragma once
#include <string>
#include <time.h>
#include <fstream>
#include <list>
#include <winsock2.h>
#include <WS2tcpip.h>
using namespace std;

struct SESSION {
	SOCKET connSock;
	list<SESSION*>::iterator position;
};

void InitiateSession(struct SESSION* session);
string CreateRamdomID();
