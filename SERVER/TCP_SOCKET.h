#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <WS2tcpip.h>
#include <process.h>
#include "CONST.h"
#include "Common.h"

#pragma comment(lib, "Ws2_32.lib")

int SEND_TCP(SOCKET s, Message data, int flag);
int RECEIVE_TCP(SOCKET s, Message* data, int flag);