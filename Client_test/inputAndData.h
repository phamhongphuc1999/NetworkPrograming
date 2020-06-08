#pragma once
#include <string>
#include <list>
#include <map>
#include "CONST.h"
using namespace std;

struct PAYLOAD
{
	list<string> data;
	char* fileName;
};

struct SEARCH {
	list<string> listID;
	char* fileName;
};

struct SEARCH_FILE {
	char* fileName;
	char* parnerID;
};

int CheckIP(char* IP);
bool CheckConnect(char* address, int port);
char* StringToChars(string input);
void CreateDATA(char* ID, char* fileName, char* data);
char* CreateDATA(char* ID, char* fileName);
