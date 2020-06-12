#include "InteractFile.h"

bool IsFileExistOrValid(string pathToFile) {
	fstream file; file.open(pathToFile);
	return file.good();
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

//FileData CreatePayload(string pathToFile) {
//	ifstream file(pathToFile, ios::in | ios::binary);
//	file.seekg(0, ios::end);
//	int length = file.tellg();
//	file.seekg(0);
//	FileData result;
//	result.payload = new char[length];
//	result.length = length;
//	file.read(result.payload, length);
//	file.close();
//	return result;
//}

FileData CreatePayload(string pathToFile) {
	ifstream file(pathToFile, ios::in | ios::binary);
	file.seekg(0, ios::end);
	int length = file.tellg();
	file.seekg(0);
	int numberOfPackage = length / 2048;
	int lastLength = length % 2048;
	FileData result; result.lastLength = lastLength;
	result.length = numberOfPackage + 1;
	result.data = new char*[result.length];
	for (int i = 0; i < result.length - 1; i++) {
		result.data[i] = new char[2049];
		file.read(result.data[i], 2048);
		result.data[i][2048] = 0;
	}
	result.data[result.length - 1] = new char[lastLength + 1];
	file.read(result.data[result.length - 1], lastLength);
	result.data[result.length - 1][lastLength] = 0;
	file.close();
	return result;
}
