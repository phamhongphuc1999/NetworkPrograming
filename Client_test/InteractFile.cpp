#include "InteractFile.h"

char* EncapsulatePayload(char* id, string fileName, string data) {
	char* result = new char[BUFF_SIZE + 1000]{ 0 };
	strcat_s(result, strlen(result) + strlen(id) + 1, id);
	int length = fileName.length();
	int index = 20;
	while (length > 0) {
		int temp = length % 10;
		length = length / 10;
		result[index] = temp + '0';
		index++;
	}
	for (int i = index; i < 23; i++) result[i] = 'e';
	index = 23;
	for (int i = 0; i < length; i++, index++) result[index] = fileName[i];
	result[index] = 0;
	char* cData = StringToChars(data);
	strcat_s(result, strlen(result) + strlen(cData) + 1, cData);
	return result;
}

char* DecapsulatePayload(char* id, char* fileName, char* buff) {
	char* data = new char[BUFF_SIZE + 1];
	for (int i = 0; i < 20; i++) id[i] = buff[i];
	id[20] = 0;
	int length = 0, index = 22;
	for (int i = index; i >= 20; i--)
		length = length * 10 + (buff[i] - '0');
	index = 23;
	for (int i = 0; i < length; i++, index++) fileName[i] = buff[index];
	int iData = 0;
	while (true)
	{
		if (buff[index] == 0) break;
		data[iData] = buff[index];
		iData++; index++;
	}
	return data;
}

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

list<char*> CreatePayload(char* id, string pathToFile) {
	string fileName = GetFileName(pathToFile);
	ifstream file; file.open(pathToFile, ios::out);
	string temp = "", line;
	list<char*> result;
	while (!file.eof()) {
		getline(file, line);
		temp += line + '\n';
		while (temp.length() > BUFF_SIZE) {
			result.push_back(EncapsulatePayload(id, fileName, temp.substr(0, BUFF_SIZE)));
			temp = temp.substr(BUFF_SIZE);
		}
	}
	file.close();
	result.push_back(EncapsulatePayload(id, fileName, temp));
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
