#include "Common.h"

//Check the validity of the IP address
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

//Check the validity of the IP address and the port
bool CheckConnect(char* address, int port) {
	return (port > 0) && CheckIP(address);
}

//Check if ID and client_id are the same
bool CheckRamdomID(char* ID, char* client_id) {
	if (!strcmp(ID, client_id)) return false;
	int index = 0;
	while (true)
	{
		if (index == 20 || ID[index] == '\0') break;
		if (!(97 <= ID[index] && ID[index] <= 122)) return false;
		index++;
	}
	return (ID[index] == '\0' && index == 20);
}

//convert string to chars
char* StringToChars(string input) {
	int length = input.length();
	char* result = new char[length];
	for (int i = 0; i < length; i++) result[i] = input[i];
	result[length] = 0;
	return result;
}
