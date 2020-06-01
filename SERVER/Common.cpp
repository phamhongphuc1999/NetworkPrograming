#include "stdafx.h"
#include "Common.h"
#include "CONST.h"
#include <list>

void InitiateSession(struct SESSION* session) {
	session->connSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	session->connSock = 0;
}

bool CheckRamdomID(list<SESSION*> listSession, char* ID) {
	list<SESSION*>::iterator pointer;
	for (pointer = listSession.begin(); pointer != listSession.end(); pointer++) 
		if (!strcmp(ID, (*pointer)->ID)) return false;
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
