#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <process.h>
#include <Windows.h>
#include "CONST.h"

#pragma comment(lib,"WS2_32")

char* EncapsulateData(char* opcode, int offset, char* data);
int DecapsulateData(char* opcode, int* offset, char* buff);
int RECEIVE_TCP(SOCKET s, char* opcode, char* data, int flag, int* offset);
int SEND_TCP(SOCKET s, char* opcode, char* data, int flag, int offset);
