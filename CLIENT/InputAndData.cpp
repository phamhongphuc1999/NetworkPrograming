#include "InputAndData.h"
#include <WS2tcpip.h>

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

char* StringToChars(string input) {
	int length = input.length();
	char* result = new char[length];
	for (int i = 0; i < length; i++) result[i] = input[i];
	result[length] = 0;
	return result;
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
