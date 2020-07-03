#include "TCP_SOCKET.h"

int SEND_TCP(SOCKET s, Message data, int flag) {
	char* buff = new char[MessageSize];
	Message* message = (Message*)buff;
	*message = data;
	return send(s, buff, MessageSize, 0);
}

int RECEIVE_TCP(SOCKET s, Message* data, int flag) {
	char* buff = new char[MessageSize];
	int ret = recv(s, buff, MessageSize, 0);
	*data = *((Message*)buff);
	if (ret == SOCKET_ERROR) return SOCKET_ERROR;
	return ret;
}

void CreateMessage(Message* message, int type, char* fileName, char* ID, char* data, int dataLen) {
	message->type = type;
	message->dataLen = dataLen;

	if (fileName == 0) message->fileName[0] = 0;
	else strcpy_s(message->fileName, strlen(fileName) + 1, fileName);

	if (ID == 0) message->ID[0] = 0;
	else strcpy_s(message->ID, strlen(ID) + 1, ID);

	if (data == 0) message->data[0] = 0;
	else memcpy_s(message->data, dataLen, data, dataLen);
}
