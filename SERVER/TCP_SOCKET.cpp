#include "TCP_SOCKET.h"

//send data by TCP Socket
//s[IN]   : a descriptor identifying a connected socket
//data[IN]: a pointer to a buffer containing the data to be transmitted
//flag[IN]: a set of flags that specify the way in which the call is made. This parameter is 
//          constructed by using the bitwise OR operator with any of the following values
int SEND_TCP(SOCKET s, Message data, int flag) {
	char* buff = new char[MessageSize];
	Message* message = (Message*)buff;
	*message = data;
	return send(s, buff, MessageSize, 0);
}

//receive data by TCP Socket
//s[IN]    : the descriptor that identifies a connected socket
//data[OUT]: a pointer to the buffer to receive the incoming data
//falg[IN] : a set of flags that influences the behavior of this function. See remarks below. 
//           See the Remarks section for details on the possible value for this parameter.
int RECEIVE_TCP(SOCKET s, Message* data, int flag) {
	char* buff = new char[MessageSize];
	int ret = recv(s, buff, MessageSize, 0);
	*data = *((Message*)buff);
	if (ret == SOCKET_ERROR) return SOCKET_ERROR;
	return ret;
}

//Packed data before transmission
//message[OUT]: the data after packed
//type[IN]    :
//fileName[IN]:
//ID[IN]      :
//data[IN]    :
//dataLen[IN] : the data lenght
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
