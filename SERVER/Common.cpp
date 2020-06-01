#include "stdafx.h"
#include "Common.h"
#include "CONST.h"
#include <list>

void InitiateSession(struct SESSION* session) {
	session->connSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	session->connSock = 0;
}

char* CreateRamdomID() {
	int length = rand() % 2 + 9;
	char* result = new char[20];
	srand((int)time(0));
	int i;
	for (i = 0; i < length; i++) {
		int element = rand() % 26 + 97;
		result[i] = (char)element;
	}
	result[i] = 0;
	return result;
}
