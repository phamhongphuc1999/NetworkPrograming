#pragma once
#include <string>
#include <list>
#include <map>
#include <tchar.h>
using namespace std;

#define BUFF_SIZE 10240

static TCHAR szWindowMain[] = _T("win32app");
static TCHAR szWindowSearch[] = _T("win32app_search");
static TCHAR szMain[] = _T("Client");
static TCHAR szSearch[] = _T("Search");

//define ID of controller
static int lbID = 1;

static int buttonConnect = 10;
static int buttonForward = 11;
static int buttonBrowse = 12;
static int buttonSearch = 13;
static int buttonHide = 14;
static int buttonClean = 15;

static int editFile = 100;
static int editFileName = 101;
static int editPartnerId = 102;
static int editIdDetail = 103;
static int editFNSearch = 104;

static int staticFile = 200;
static int staticFileName = 201;
static int staticPartnerId = 202;
static int staticId = 203;
