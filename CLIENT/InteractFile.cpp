#include "InteractFile.h"

bool IsFileExistOrValid(string pathToFile) {
	fstream file; file.open(pathToFile);
	return file.good();
}

list<string> CreatePayload(string pathToFile) {
	ifstream file; file.open(pathToFile, ios::out);
	string temp = "", line;
	list<string> result;
	while (!file.eof()) {
		getline(file, line);
		temp += line + '\n';
		while (temp.length() > BUFF_SIZE) {
			result.push_back(temp.substr(0, BUFF_SIZE));
			temp = temp.substr(BUFF_SIZE);
		}
	}
	file.close();
	result.push_back(temp);
	return result;
}

list<string> read_directory(const string& pathToFolder)
{
	list<string> v;
	string pattern(pathToFolder);
	pattern.append("\\*");
	WIN32_FIND_DATA data;
	HANDLE hFind;
	if ((hFind = FindFirstFile(pattern.c_str(), &data)) != INVALID_HANDLE_VALUE) {
		while (true)
		{
			v.push_back(data.cFileName);
			if (FindNextFile(hFind, &data) == 0) break;
		}
		FindClose(hFind);
	}
	return v;
}

bool SearchFileInDirectory(const string& pathToFolder, string fileName) {
	string pattern(pathToFolder);
	pattern.append("\\*");
	WIN32_FIND_DATA data;
	HANDLE hFind;
	if ((hFind = FindFirstFile(pattern.c_str(), &data)) != INVALID_HANDLE_VALUE) {
		while (true)
		{
			if (fileName == data.cFileName) return true;
			if (FindNextFile(hFind, &data) == 0) break;
		}
		FindClose(hFind);
	}
	return false;
}

string GetFileName(const string& str)
{
	size_t found;
	found = str.find_last_of("/\\");
	return str.substr(found + 1);
}

bool SearchFileByName(string fileName) {
	string pattern("./data");
	pattern.append("\\*");
	WIN32_FIND_DATA data;
	HANDLE hFind;
	if ((hFind = FindFirstFile(pattern.c_str(), &data)) != INVALID_HANDLE_VALUE) {
		while (true)
		{
			if (fileName == data.cFileName) return true;
			if (FindNextFile(hFind, &data) == 0) break;
		}
		FindClose(hFind);
	}
	return false;
}
