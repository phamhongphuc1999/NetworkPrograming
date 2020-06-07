#pragma once
#include "CONST.h"
#include "InputAndData.h";
#include <fstream>
#include <list>
#include <string>
#include <windows.h>
using namespace std;

char* EncapsulatePayload(char* id, string fileName, string data);
char* DecapsulatePayload(char* id, char* fileName, char* buff);
bool IsFileExistOrValid(string pathToFile);
list<string> CreatePayload(string pathToFile);
list<char*> CreatePayload(char* id, string pathToFile);
list<string> read_directory(const string& pathToFolder);
bool SearchFileInDirectory(const string& pathToFolder, string fileName);
string GetFileName(const string& str);
bool SearchFileByName(string fileName);
