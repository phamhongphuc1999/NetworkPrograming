#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <process.h>
#include <Windows.h>
#include "CONST.h"
#include "Common.h"

#pragma comment(lib,"WS2_32")

int SEND_TCP(SOCKET s, int type, char* data, int flag);
int RECEIVE_TCP(SOCKET s, int* type, char* data, int flag);
