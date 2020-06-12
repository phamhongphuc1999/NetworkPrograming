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

unsigned _stdcall SearchSessionByID(void* param) {
	SESSION* session = (SESSION*)param;
	int ret; Message message;
	for (pair<string, ForwardInfo> info : session->forwardInfo) {
		if (info.second.status == 0) {
			string partnerID(info.second.parnerID);
			SESSION* partner = mapSession[partnerID];
			if (partner != NULL) {
				CreateMessage(&message, 200, info.second.fileName, session->ID, 0);
				ret = SEND_TCP(partner->connSock, message, 0);
				if (ret == SOCKET_ERROR) continue;
			}
			else {
				CreateMessage(&message, 203, info.second.fileName, info.second.parnerID, 0);
				ret = SEND_TCP(session->connSock, message, 0);
				if (ret == SOCKET_ERROR) continue;
			}
			info.second.status = 1;
		}
	}
	return 0;
}

unsigned _stdcall SendForwardFileToClient(void* param) {
	SESSION* session = (SESSION*)param;
	int ret; Message message;
	for (pair<string, ForwardInfo> item : session->forwardInfo) {
		if (item.second.status == 2) {
			string ID(item.second.parnerID);
			for (string data : item.second.payload) {
				CreateMessage(&message, 201, item.second.fileName, session->ID, StringToChars(data));
				ret = SEND_TCP(mapSession[ID]->connSock, message, 0);
				if (ret == SOCKET_ERROR) continue;
			}
			CreateMessage(&message, 201, item.second.fileName, session->ID, 0);
			ret = SEND_TCP(mapSession[ID]->connSock, message, 0);
			if (ret == SOCKET_ERROR) continue;
		}
		item.second.status = 3;
	}
	return 0;
}

unsigned _stdcall SendSearchFileToClient(void* param) {

	return 0;
}

unsigned _stdcall SendListSearchClient(void* param) {
	SESSION* session = (SESSION*)param;
	Message message; int ret;
	for (pair<string, SearchInfo> item : session->searchInfo) {
		if (item.second.status == 2) {
			for (char* ID : item.second.Yes) {
				CreateMessage(&message, 111, item.second.fileName, ID, 0);
				ret = SEND_TCP(session->connSock, message, 0);
				if (ret == SOCKET_ERROR) continue;
			}
			CreateMessage(&message, 111, item.second.fileName, 0, 0);
			ret = SEND_TCP(session->connSock, message, 0);
			if (ret == SOCKET_ERROR) continue;
		}
		item.second.status = 3;
	}
	return 0;
}

unsigned _stdcall SearchFile(void* param) {
	SESSION* session = (SESSION*)param;
	int ret; Message message;
	for (pair<string, SearchInfo> item : session->searchInfo) {
		if (item.second.status == 0) {
			for (pair<string, SESSION*> partner : mapSession) {
				if (strcmp(session->ID, partner.second->ID)) {
					CreateMessage(&message, 120, item.second.fileName, session->ID, 0);
					ret = SEND_TCP(partner.second->connSock, message, 0);
					if (ret == SOCKET_ERROR) continue;
				}
			}
			item.second.status = 1;
		}
	}
	return 0;
}

unsigned _stdcall Handler(void* param) {
	SOCKET listenSocket = (SOCKET)param;
	sockaddr_in clientAddr;
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
					CreateMessage(&message, 100, 0, client[index].ID, 0);
					ret = SEND_TCP(client[index].connSock, message, 0);
					if (ret == SOCKET_ERROR) continue;
				}

				else if (message.type == 310) {
					string fileName(message.fileName);
					SearchInfo info; info.status = 0;
					info.fileName = new char[BUFF_SIZE];
					strcpy_s(info.fileName, strlen(message.fileName) + 1, message.fileName);
					client[index].searchInfo.insert({ fileName, info });
					_beginthreadex(0, 0, SearchFile, (void*)&client[index], 0, 0);
				}

				else if (message.type == 311) {
					string fileName(message.fileName);
					string ID(message.ID);
					if (strcmp(message.data, "")) {
						char* temp = new char[BUFF_SIZE];
						strcpy_s(temp, strlen(message.data) + 1, message.data);
						client[index].searchInfo[fileName].payload.push_back(temp);
					}
					else {
						client[index].forwardInfo[fileName].status = 3;
						_beginthreadex(0, 0, SendSearchFileToClient, (void*)&client[index], 0, 0);
					}
				}

				else if (message.type == 312) {
					Message searchFile; CreateMessage(&searchFile, 121, message.fileName, client[index].ID, 0);
					int ret = SEND_TCP(mapSession[string(message.ID)]->connSock, searchFile, 0);
					if (ret == SOCKET_ERROR) continue;
				}

				else if (message.type == 320 || message.type == 321) {
					string partnerID(message.ID);
					string fileName(message.fileName);
					char* temp = new char[BUFF_SIZE];
					if (message.type == 320)
						mapSession[partnerID]->searchInfo[fileName].No.push_back(client[index].ID);
					else mapSession[partnerID]->searchInfo[fileName].Yes.push_back(client[index].ID);
					int yesSize = mapSession[partnerID]->searchInfo[fileName].Yes.size();
					int noSize = mapSession[partnerID]->searchInfo[fileName].No.size();
					if (yesSize + noSize == mapSession.size() - 1) {
						mapSession[partnerID]->searchInfo[fileName].status = 2;
						_beginthreadex(0, 0, SendListSearchClient, (void*)mapSession[partnerID], 0, 0);
					}
				}

				else if (message.type == 400) {
					ForwardInfo info; info.status = 0;
					info.fileName = new char[BUFF_SIZE];
					info.parnerID = new char[30];
					string sFileName(message.fileName);
					strcpy_s(info.fileName, strlen(message.fileName) + 1, message.fileName);
					strcpy_s(info.parnerID, strlen(message.ID) + 1, message.ID);
					client[index].forwardInfo.insert({ sFileName, info });
					_beginthreadex(0, 0, SearchSessionByID, (void*)&client[index], 0, 0);
				}

				else if (message.type == 401) {
					string fileName(message.fileName);
					string ID(message.ID);
					if (strcmp(message.data, "")) {
						char* temp = new char[BUFF_SIZE];
						strcpy_s(temp, strlen(message.data) + 1, message.data);
						client[index].forwardInfo[fileName].payload.push_back(temp);
					}
					else {
						client[index].forwardInfo[fileName].status = 2;
						_beginthreadex(0, 0, SendForwardFileToClient, (void*)&client[index], 0, 0);
					}
				}

				else if (message.type == 410) {
					string requestID(message.ID);
					strcpy_s(message.ID, strlen(client[index].ID) + 1, client[index].ID);
					message.type = 203;
					ret = SEND_TCP(mapSession[requestID]->connSock, message, 0);
					if (ret == SOCKET_ERROR) continue;
				}

				else if (message.type == 411) {
					string requestID(message.ID);
					strcpy_s(message.ID, strlen(client[index].ID) + 1, client[index].ID);
					message.type = 202;
					ret = SEND_TCP(mapSession[requestID]->connSock, message, 0);
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
