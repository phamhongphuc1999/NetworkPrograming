#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <WS2tcpip.h>
#include <process.h>
#include "CONST.h"

#pragma comment(lib, "Ws2_32.lib")

char* EncapsulateData(char* opcode, int offset, char* data);
int DecapsulateData(char* opcode, int* offset, char* buff);
int RECEIVE_TCP(SOCKET s, char* opcode, char* data, int flag, int* offset);
int SEND_TCP(SOCKET s, char* opcode, char* data, int flag, int offset);