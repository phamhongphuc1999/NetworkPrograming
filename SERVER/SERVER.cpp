// SERVER.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TCP_SOCKET.h"
#include "Common.h"
#include "CONST.h"

using namespace std;

unsigned _stdcall Handler(void* param);

int main()
{
	WSADATA wsaData;
	WORD version = MAKEWORD(2, 2);
	if (WSAStartup(version, &wsaData)) {
		printf("Version is not supported\n");
		return 0;
	}
	SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	u_short serverPort = 0;
node1:
	printf("%s ", SERVER_EXE);
	scanf_s("%d", &serverPort);
	if (serverPort < 1) {
		printf("Wrong port\n");
		goto node1;
	}
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(serverPort);
	serverAddr.sin_addr.s_addr = inet_addr(SERVER_ADDR);
	if (bind(listenSocket, (sockaddr*)&serverAddr, sizeof(serverAddr))) {
		printf("Can not bind this address"); 
		return 0;
	}
	if (listen(listenSocket, MAX_CLIENT)) {
		printf("Can not listen"); 
		return 0;
	}
	printf("SERVER STARTED\n");
	lockSession = 0; isThreadFull = 1;
	while (true)
	{
		if (isThreadFull == 1) {
			_beginthreadex(0, 0, Handler, (void*)listenSocket, 0, 0);
			isThreadFull = 0;
		}
	}
}

unsigned _stdcall Handler(void* param) {
	SOCKET listenSocket = (SOCKET)param;
	sockaddr_in clientAddr;
	char buff[BUFF_SIZE], buffSend[BUFF_SIZE];
	char username[2048], password[2048];
	char* result = new char[10];
	int ret, clientAddrLen = sizeof(clientAddr);
	DWORD nEvents = MAKEWORD(0, 0), i, index;
	SOCKET connSock;

	SESSION client;
	WSAEVENT events[WSA_MAXIMUM_WAIT_EVENTS];
	events[0] = WSACreateEvent();
	WSANETWORKEVENTS sockEvent;
}
