#include "TCP_SOCKET.h"

int SEND_TCP(SOCKET s, int type, char* data, int flag) {
	char* buff = new char[MessageSize];
	Message* message = (Message*)buff;
	message->type = type;
	strcpy_s(message->data, strlen(data) + 1, data);
	return send(s, buff, MessageSize, 0);
}

int RECEIVE_TCP(SOCKET s, int* type, char* data, int flag) {
	char* buff = new char[MessageSize];
	Message* message = (Message*)buff;
	int ret = recv(s, buff, MessageSize, 0);
	if (ret == SOCKET_ERROR) return SOCKET_ERROR;
	*type = message->type;
	strcpy_s(data, strlen(message->data) + 1, message->data);
	return ret;
}
