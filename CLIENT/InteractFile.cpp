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
	return result;
}

list<string> read_directory(const string& pathToFile)
{
	list<string> v;
	string pattern(pathToFile);
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
