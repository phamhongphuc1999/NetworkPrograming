#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <WS2tcpip.h>
#include <process.h>
#include "CONST.h"

#pragma comment(lib, "Ws2_32.lib")

char* EncapsulateData(char* opcode, char* data);
int DecapsulationData(char* buff, char* opcode);
int RECEIVE_TCP(SOCKET s, char* opcode, char* buff, int flag);
int SEND_TCP(SOCKET s, char* buff, int flag);
