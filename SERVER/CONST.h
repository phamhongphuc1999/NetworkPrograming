#pragma once
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>

#define BUFF_SIZE 2048
#define SERVER_ADDR "127.0.0.1"
#define SERVER_EXE "Server.exe"
#define MAX_CLIENT 64

//define opcode
//server
static char* o_100 = new char[4]{ "100" };
static char* o_110 = new char[4]{ "110" };
static char* o_111 = new char[4]{ "111" };
static char* o_112 = new char[4]{ "112" };
static char* o_120 = new char[4]{ "120" };
static char* o_200 = new char[4]{ "200" };
static char* o_201 = new char[4]{ "201" };

//client
static char* o_300 = new char[4]{ "300" };
static char* o_310 = new char[4]{ "310" };
static char* o_311 = new char[4]{ "311" };
static char* o_320 = new char[4]{ "320" };
static char* o_321 = new char[4]{ "321" };
static char* o_400 = new char[4]{ "400" };
static char* o_410 = new char[4]{ "410" };
static char* o_411 = new char[4]{ "411" };
