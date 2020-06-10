#include "TCP_SOCKET.h"
#include "Common.h"
#include <stdio.h>
#include <tchar.h>

unsigned _stdcall Handler(void* param);

map<string, SESSION*> mapSession;
int lockSession, isThreadFull;

int main()
{
	WSADATA wsaData;
	WORD version = MAKEWORD(2, 2);
	if (WSAStartup(version, &wsaData)) {
		return 0;
	}
	SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	u_short serverPort = 0;
node1:
	printf("%s ", SERVER_EXE);
	scanf_s("%d", &serverPort);
	if (serverPort < 1) goto node1;
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(serverPort);
	serverAddr.sin_addr.s_addr = inet_addr(SERVER_ADDR);
	if (bind(listenSocket, (sockaddr*)&serverAddr, sizeof(serverAddr))) return 0;
	if (listen(listenSocket, MAX_CLIENT)) return 0;
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

//Add session in listSession
unsigned _stdcall CreateSession(void* param) {
	while (true)
	{
		if (lockSession == 0) {
			lockSession = 1;
			SESSION* session = (SESSION*)param;
			string ID(session->ID);
			mapSession.insert({ ID, session });
			lockSession = 0;
			break;
		}
	}
	return 0;
}

//Remove session from listSession
unsigned _stdcall ReleaseSession(void* param) {
	while (true)
	{
		if (lockSession == 0) {
			lockSession = 1;
			SESSION* session = (SESSION*)param;
			string ID(session->ID);
			mapSession.erase(ID);
			lockSession = 0;
			break;
		}
	}
	return 0;
}

unsigned _stdcall Handler(void* param) {
	SOCKET listenSocket = (SOCKET)param;
	sockaddr_in clientAddr;
	//char buffReceive[BUFF_SIZE + 1], buffSend[BUFF_SIZE + 1];
	Message message;
	int clientAddrLen = sizeof(clientAddr);
	DWORD nEvents = MAKEWORD(0, 0), i, index;
	SOCKET connSock;

	SESSION client[WSA_MAXIMUM_WAIT_EVENTS];
	WSAEVENT events[WSA_MAXIMUM_WAIT_EVENTS];
	events[0] = WSACreateEvent();
	WSANETWORKEVENTS sockEvent;

	for (int i = 0; i < WSA_MAXIMUM_WAIT_EVENTS; i++) InitiateSession(&client[i]);
	client[0].connSock = listenSocket;
	WSAEventSelect(client[0].connSock, events[0], FD_ACCEPT | FD_CLOSE);
	nEvents++;
	char* fileName = new char[BUFF_SIZE];
	char* parnerID = new char[BUFF_SIZE];
	while (true)
	{
		index = WSAWaitForMultipleEvents(nEvents, events, FALSE, WSA_INFINITE, FALSE);
		if (index == WSA_WAIT_FAILED) break;
		index = index - WSA_WAIT_EVENT_0;
		if (index != WSA_WAIT_FAILED && index != WSA_WAIT_TIMEOUT) {
			WSAEnumNetworkEvents(client[index].connSock, events[index], &sockEvent);

			if (sockEvent.lNetworkEvents & FD_ACCEPT) {
				if (sockEvent.iErrorCode[FD_ACCEPT_BIT] != 0) break;
				if ((connSock = accept(client[index].connSock, (sockaddr *)&clientAddr, &clientAddrLen)) == SOCKET_ERROR)
					continue;
				if (nEvents == WSA_MAXIMUM_WAIT_EVENTS) isThreadFull = 1;
				else {
					for (int j = 1; j < WSA_MAXIMUM_WAIT_EVENTS; j++)
						if (client[j].connSock == 0) {
							char* tempId = CreateRamdomID();
							while (!CheckRamdomID(mapSession, tempId))
								tempId = CreateRamdomID();
							client[j].connSock = connSock;
							client[j].ID = tempId;
							events[j] = WSACreateEvent();
							WSAEventSelect(client[j].connSock, events[j], FD_READ | FD_CLOSE);
							HANDLE hCrSession = (HANDLE)_beginthreadex(0, 0, CreateSession, (void*)&client[j], 0, 0);
							WaitForSingleObject(hCrSession, INFINITE);
							nEvents++;
							break;
						}
				}
				WSAResetEvent(events[index]);
				continue;
			}

			if (sockEvent.lNetworkEvents & FD_READ) {
				if (sockEvent.iErrorCode[FD_READ_BIT] != 0) break;
				int ret = RECEIVE_TCP(client[index].connSock, &message, 0);
				if (ret == SOCKET_ERROR) continue;
				if (message.type == 300) {
					CreateMessage(&message, 100, client[index].ID, NULL);
					ret = SEND_TCP(client[index].connSock, message, 0);
					if (ret == SOCKET_ERROR) continue;
				}

				
				continue;
			}

			if (sockEvent.lNetworkEvents & FD_WRITE) {
				if (sockEvent.iErrorCode[FD_WRITE_BIT] != 0) break;
				continue;
			}

			if (sockEvent.lNetworkEvents & FD_CLOSE) {
				if (sockEvent.iErrorCode[FD_CLOSE_BIT] != 0) {
					HANDLE hRelease = (HANDLE)_beginthreadex(0, 0, ReleaseSession, (void*)&client[index], 0, 0);
					WaitForSingleObject(hRelease, INFINITE);
					closesocket(client[index].connSock);
					InitiateSession(&client[index]);
					WSACloseEvent(events[index]);
					nEvents--; continue;
				}
				else {
					HANDLE hRelease = (HANDLE)_beginthreadex(0, 0, ReleaseSession, (void*)&client[index], 0, 0);
					WaitForSingleObject(hRelease, INFINITE);
					closesocket(client[index].connSock);
					InitiateSession(&client[index]);
					WSACloseEvent(events[index]);
					nEvents--; continue;
				}
			}
		}
	}
	return 0;
}
