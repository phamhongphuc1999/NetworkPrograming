#pragma once
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#define BUFF_SIZE 2048

static TCHAR szWindowClass[] = _T("win32app");
static TCHAR szTitle[] = _T("Win32 Guided Tour Application");

static UINT bConnect = 10;
static UINT bForward = 11;
static UINT bBrowse = 12;
static UINT bSearch = 13;
static UINT bHide = 14;

static UINT staticFile = 100;
static UINT staticFileName = 101;
