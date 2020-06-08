#pragma once
#include "CONST.h"
#include "InputAndData.h";
#include <fstream>
#include <list>
#include <string>
#include <windows.h>
using namespace std;

bool IsFileExistOrValid(string pathToFile);
list<string> CreatePayload(string pathToFile);
list<string> read_directory(const string& pathToFolder);
bool SearchFileInDirectory(const string& pathToFolder, string fileName);
string GetFileName(const string& str);
bool SearchFileByName(string fileName);
