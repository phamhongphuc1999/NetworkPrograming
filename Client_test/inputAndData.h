#pragma once
#include <string>
#include <list>
#include <map>
#include "CONST.h"
using namespace std;

struct SearchInfo {
	char* searchID;
	char* fileName;
	list<string> listID;
	list<string> payload;
};

int CheckIP(char* IP);
bool CheckConnect(char* address, int port);
void InitiateSearchInfo(struct SearchInfo* info);
char* StringToChars(string input);
string WcharToString(wchar_t* wchar_str);
wchar_t* StringToWchar(string str);
void CreateDATA(char* ID, char* fileName, char* data);
char* CreateDATA(char* ID, char* fileName);
