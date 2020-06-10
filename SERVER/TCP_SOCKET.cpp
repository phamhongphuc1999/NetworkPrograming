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
