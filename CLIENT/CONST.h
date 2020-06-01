#pragma once
#include <tchar.h>

#define BUFF_SIZE 2048

static TCHAR szWindowClass[] = _T("win32app");
static TCHAR szTitle[] = _T("Client");

//define ID of controller
static int bConnect = 10;
static int bForward = 11;
static int bBrowse = 12;
static int bSearch = 13;
static int bHide = 14;

static int editFile = 100;
static int editFileName = 101;
static int editParnerId = 102;
static int editIdDetail = 103;

static int staticFile = 200;
static int staticFileName = 201;
static int staticParnerId = 202;
static int staticId = 203;

//define opcode
//server
static char* o_100 = new char[4]{ "100" };
static char* o_110 = new char[4]{ "110" };
static char* o_111 = new char[4]{ "111" };
static char* o_112 = new char[4]{ "112" };
static char* o_120 = new char[4]{ "120" };
static char* o_200 = new char[4]{ "200" };
static char* o_201 = new char[4]{ "201" };
static char* o_202 = new char[4]{ "202" };
static char* o_203 = new char[4]{ "203" };

//client
static char* o_300 = new char[4]{ "300" };
static char* o_310 = new char[4]{ "310" };
static char* o_311 = new char[4]{ "311" };
static char* o_320 = new char[4]{ "320" };
static char* o_321 = new char[4]{ "321" };
static char* o_400 = new char[4]{ "400" };
static char* o_401 = new char[4]{ "401" };
static char* o_410 = new char[4]{ "410" };
static char* o_411 = new char[4]{ "411" };
