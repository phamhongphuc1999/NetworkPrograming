#include "InputAndData.h"

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
