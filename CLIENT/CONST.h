#pragma once
#include <tchar.h>

#define BUFF_SIZE 2048

static TCHAR szWindowClass[] = _T("win32app");
static TCHAR szTitle[] = _T("Client");

static int bConnect = 10;
static int bForward = 11;
static int bBrowse = 12;
static int bSearch = 13;
static int bHide = 14;

static int editFile = 100;
static int editFileName = 101;
static int editParnerId = 102;

static int staticFile = 200;
static int staticFileName = 201;
static int staticParnerId = 202;
static int staticId = 203;
static int staticIdDetail = 204;
