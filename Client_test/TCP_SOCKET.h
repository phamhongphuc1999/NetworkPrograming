#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <process.h>
#include <Windows.h>
#include "CONST.h"

#pragma comment(lib,"WS2_32")

struct RECEIVE_INFO
{
	SOCKET s;
	char* opcode;
	char* buff;
	int flag;
	int returnValue;
};

struct SEND_INFO
{
	SOCKET s;
	char* opcode;
	char* buff;
	int flag;
	int returnValue;
};

char* EncapsulateData(char* opcode, char* data);
int DecapsulateData(char* opcode, char* buff);
int RECEIVE_TCP(SOCKET s, char* opcode, char* data, int flag);
int SEND_TCP(SOCKET s, char* opcode, char* data, int flag);
unsigned _stdcall HANDLER_RECEIVE_TCP(void* param);
unsigned _stdcall HANDLER_SEND_TCP(void* param);
void RECEIVE_THREAD_TCP(RECEIVE_INFO* param);
void SEND_THREAD_TCP(SEND_INFO* param);
