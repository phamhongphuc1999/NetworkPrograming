#pragma once
#include "CONST.h"
#include <fstream>
#include <list>
#include <string>
#include <windows.h>
using namespace std;

bool IsFileExistOrValid(string pathToFile);
list<string> CreatePayload(string pathToFile);
list<string> read_directory(const string& pathToFolder);
string GetFileName(const string& str);
bool SearchFileByName(string fileName);
