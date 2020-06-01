#include "stdafx.h"
#include "TCP_SOCKET.h"
#include "CONST.h"

char* EncapsulateData(char* opcode, char* data) {
	int length = strlen(data);
	char* result = new char[BUFF_SIZE];
	result[0] = opcode[0];
	result[1] = opcode[1];
	result[2] = opcode[2];
	int index = 3;
	while (length > 0) {
		int temp = length % 10;
		length = length / 10;
		result[index] = temp + '0';
		index++;
	}
	for (int i = index; i < 13; i++) result[i] = 'e';
	result[13] = 0;
	strcat_s(result, strlen(data) + strlen(result) + 1, data);
	return result;
}

int DecapsulateData(char* opcode, char* buff) {
	int result = 0, index = 12;
	while (!('0' <= buff[index] && buff[index] <= '9')) { index--; }
	for (int i = index; i >= 3; i--) {
		result = result * 10 + (buff[i] - '0');
	}
	for (int i = 0; i <= 2; i++) {
		opcode[i] = buff[i];
	}
	opcode[3] = 0;
	return result;
}

int RECEIVE_TCP(SOCKET s, char* opcode, char* data, int flag) {
	int index = 0, ret, result = 0;
	char* temp = new char[20];
	ret = recv(s, temp, 13, flag);
	if (ret == SOCKET_ERROR) return SOCKET_ERROR;
	temp[ret] = 0;
	if (ret == 0) return 0;
	else if (ret == SOCKET_ERROR) return SOCKET_ERROR;
	else {
		int length = DecapsulateData(opcode, temp);
		while (length > 0) {
			ret = recv(s, &data[index], length, 0);
			if (ret == SOCKET_ERROR) return SOCKET_ERROR;
			else result += ret;
			index += ret;
			length -= ret;
		}
		return result;
	}
}

int SEND_TCP(SOCKET s, char* opcode, char* data, int flag) {
	char* buff = EncapsulateData(opcode, data);
	int nLeft = strlen(buff), index = 0, ret, result = 0;
	while (nLeft > 0) {
		ret = send(s, &buff[index], nLeft, flag);
		if (ret == SOCKET_ERROR) return SOCKET_ERROR;
		else result += ret;
		nLeft -= ret; index += ret;
	}
	return result;
}