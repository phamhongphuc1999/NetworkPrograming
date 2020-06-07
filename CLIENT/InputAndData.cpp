#include "InputAndData.h"
#include <WS2tcpip.h>
#include <time.h>

int CheckIP(char* IP) {
	int index = 0, sum = 0, countPoint = 0;
	while (IP[index] == ' ') index++;
	char element = IP[index];
	while (element != '\0') {
		if (element == ' ') break;
		else if ('0' <= element && element <= '9') {
			sum = sum * 10 + (element - '0');
			element = IP[++index];
		}
		else if (element == '.') {
			countPoint++;
			if (sum > 255 || countPoint > 3) return 0;
			else {
				sum = 0; element = IP[++index];
			}
		}
		else return 0;
	}
	return (countPoint == 3);
}

bool CheckConnect(char* address, int port) {
	return (port > 0) && CheckIP(address);
}

void InitiateSearchInfo(struct SearchInfo* info) {
	info->fileName = new char[BUFF_SIZE];
	info->searchID = new char[BUFF_SIZE];
	info->listID.clear();
	info->payload.clear();
}

char* StringToChars(string input) {
	int length = input.length();
	char* result = new char[length];
	for (int i = 0; i < length; i++) result[i] = input[i];
	result[length] = 0;
	return result;
}

string WcharToString(wchar_t* wchar_str)
{
	string str = "";
	int index = 0;
	while (wchar_str[index] != 0)
	{
		str += (char)wchar_str[index];
		++index;
	}
	return str;
}

wchar_t* StringToWchar(string str)
{
	int index = 0;
	int count = str.size();
	wchar_t *ws_str = new wchar_t[count + 1];
	while (index < str.size())
	{
		ws_str[index] = (wchar_t)str[index];
		index++;
	}
	ws_str[index] = 0;
	return ws_str;
}

void CreateDATA(char* ID, char* fileName, char* data) {
	int index = 0;
	while (true) {
		if (data[index] == ' ') break;
		ID[index] = data[index];
		index++;
	}
	ID[index++] = 0;
	int index1 = 0;
	while (true)
	{
		if (data[index] == 0) break;
		fileName[index1] = data[index];
		index++; index1++;
	}
	fileName[index1] = 0;
}

char* CreateDATA(char* ID, char* fileName) {
	char* result = new char[BUFF_SIZE] {0};
	strcat_s(result, strlen(result) + strlen(ID) + 1, ID);
	int l = strlen(result);
	result[l] = ' '; result[l + 1] = 0;
	strcat_s(result, strlen(result) + strlen(fileName) + 1, fileName);
	return result;
}
