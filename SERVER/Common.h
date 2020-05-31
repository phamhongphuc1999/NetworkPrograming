#pragma once
#include <string>
#include <time.h>
#include <fstream>
#include <list>
using namespace std;

typedef struct SESSION {
	SOCKET connSock;
	list<SESSION*>::iterator position;
};

//list of session
list<SESSION*> listSession;

int lockSession, isThreadFull;

string CreateRamdomID();
void InitiateSession(SESSION* session);
