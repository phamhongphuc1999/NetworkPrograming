#include "stdafx.h"
#include "Common.h"
#include "CONST.h"
#include <list>

void InitiateSession(struct SESSION* session) {
	session->forwardInfo.fileName = new char[BUFF_SIZE];
	session->forwardInfo.parnerID = new char[BUFF_SIZE];
	session->forwardInfo.payload.clear();
	session->connSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	session->connSock = 0;
}

void InitiateSearchInfo(struct SearchInfo* info) {
	info->fileName = new char[BUFF_SIZE];
	info->searchID = new char[30];
	info->requestID = new char[30];
	info->parnerID = new char[30];
	info->Yes.clear();
	info->No.clear();
	info->payload.clear();
}

bool CheckRamdomIDForSESSION(map<string, SESSION*> mapSession, char* ID) {
	string id(ID);
	for (pair<string, SESSION*> data : mapSession)
		if (id == data.first) return false;
	return true;
}

bool CheckRamdomIDForSearch(map<string, SearchInfo*> mapSearch, char* ID) {
	string id(ID);
	for (pair<string, SearchInfo*> data : mapSearch)
		if (id == data.first) return false;
	return true;
}
bool CheckRamdomIDForForward(map<string, ForwardInfo*> mapForward, char* ID) {
	string id(ID);
	for (pair<string, ForwardInfo*> data : mapForward)
		if (id == data.first) return false;
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

char* StringToChars(string input) {
	int length = input.length();
	char* result = new char[length];
	for (int i = 0; i < length; i++) result[i] = input[i];
	result[length] = 0;
	return result;
}

void CreateDATA(char* ID, char* fileName, char* data) {
	int index = 0;
	while (true) {
		if (data[index] == ' ') break;
		ID[index] = data[index];
		index++;
	}
	ID[index++] = 0;
	int index1 = 0;
	while (true)
	{
		if (data[index] == 0) break;
		fileName[index1] = data[index];
		index++; index1++;
	}
	fileName[index1] = 0;
}

char* CreateDATA(char* ID, char* fileName) {
	char* result = new char[BUFF_SIZE] {0};
	strcat_s(result, strlen(result) + strlen(ID) + 1, ID);
	int l = strlen(result);
	result[l] = ' '; result[l + 1] = 0;
	strcat_s(result, strlen(result) + strlen(fileName) + 1, fileName);
	return result;
}
