#pragma once
#include <string>
#include "CONST.h"
using namespace std;

int CheckIP(char* IP);
bool CheckConnect(char* address, int port);
char* StringToChars(string input);
string WcharToString(wchar_t* wchar_str);
wchar_t* StringToWchar(string str);
void CreateDATA(char* ID, char* fileName, char* data);
char* CreateDATA(char* ID, char* fileName);
