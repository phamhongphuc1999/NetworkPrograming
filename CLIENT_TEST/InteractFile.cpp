#include "InteractFile.h"

//check for the existence of the file
bool IsFileExistOrValid(string pathToFile) {
	fstream file; file.open(pathToFile);
	return file.good();
}

//return list of file name or directory name in folder
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

//search file by file name in folder
//pathToFolder[IN] : the path to folder will be searched
//fileName[IN]     : the fileName will be searched
//count[OUT]       : the number of item in folder will be searched
//pathToResult[OUT]: the path to result file
bool SearchFileInDirectory(const string& pathToFolder, string fileName, int* count, string& pathToResult) {
	string pattern(pathToFolder);
	pattern.append("\\*");
	WIN32_FIND_DATA data;
	HANDLE hFind;
	*count = 0;
	if ((hFind = FindFirstFile(pattern.c_str(), &data)) != INVALID_HANDLE_VALUE) {
		while (true)
		{
			(*count)++;
			if (!strcmp(data.cFileName, new char[2]{ "." }) || !strcmp(data.cFileName, new char[3]{ ".." })) {
				if (FindNextFile(hFind, &data) == 0) break;
				else continue;
			}
			else {
				int temp = 0;
				if (SearchFileInDirectory(pathToFolder + "/" + data.cFileName, fileName, &temp, pathToResult)) return true;
				else if (temp == 0)
					if (data.cFileName == fileName) {
						pathToResult = pathToFolder + "/" + fileName;
						return true;
					}
				if (FindNextFile(hFind, &data) == 0) break;
			}
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

//Split file data into packages of size BUFF_SIZE
FileData CreatePayload(string pathToFile) {
	ifstream file(pathToFile, ios::in | ios::binary);
	file.seekg(0, ios::end);
	int length = file.tellg();
	file.seekg(0);
	int numberOfPackage = length / BUFF_SIZE;
	int lastLength = length % BUFF_SIZE;
	FileData result; result.lastLength = lastLength;
	result.length = numberOfPackage + 1;
	result.data = new char*[result.length];
	for (int i = 0; i < result.length - 1; i++) {
		result.data[i] = new char[BUFF_SIZE + 1];
		file.read(result.data[i], BUFF_SIZE);
		result.data[i][BUFF_SIZE] = 0;
	}
	result.data[result.length - 1] = new char[lastLength + 1];
	file.read(result.data[result.length - 1], lastLength);
	result.data[result.length - 1][lastLength] = 0;
	file.close();
	return result;
}
