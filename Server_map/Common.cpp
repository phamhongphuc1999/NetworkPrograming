#include "stdafx.h"
#include "Common.h"
#include "CONST.h"
#include <list>

void InitiateSession(struct SESSION* session) {
	session->forwardInfo = new ForwardInfo[1];
	session->forwardInfo->fileName = new char[BUFF_SIZE];
	session->forwardInfo->parnerID = new char[BUFF_SIZE];
	session->forwardInfo->payload.clear();
	session->connSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	session->connSock = 0;
}

bool CheckRamdomID(map<string, SESSION*> listSession, char* ID) {
	for (pair<string, SESSION*> session : listSession)
		if (!strcmp(ID, session.second->ID)) return false;
	return true;
}

char* CreateRamdomID() {
	char* result = new char[20];
	srand((int)time(0));
	for (int i = 0; i < 20; i++) {
		int element = rand() % 26 + 97;
		result[i] = (char)element;
	}
	result[20] = 0;
	return result;
}
