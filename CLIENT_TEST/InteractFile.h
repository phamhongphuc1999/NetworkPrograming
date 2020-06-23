#pragma once
#include "CONST.h"
#include "Common.h"
#include <fstream>
#include <windows.h>

bool IsFileExistOrValid(string pathToFile);
list<string> read_directory(const string& pathToFolder);
bool SearchFileInDirectory(const string& pathToFolder, string fileName, int* count, string& pathToResult);
string GetFileName(const string& str);
FileData CreatePayload(string pathToFile);
