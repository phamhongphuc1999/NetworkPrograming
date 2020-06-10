#include "Common.h"
#include "TCP_SOCKET.h"
#include "InteractFile.h"

HWND hMain, hSearch;
HINSTANCE hInst;
//controller in main window
static HWND btnBrowse, btnForward, btnSearch, btnHide, btnConnect, btnClean;
static HWND eFile, eFileName, eParnerId, eIdDetail;
static HWND sFile, sFileName, sParnerId, sID;
bool isConnect = false, isHide = true;

//controller in search window
static HWND btnSend;
static HWND eID;
static HWND sFileNameSearch, sIDSearch;

#pragma region CONNECT SERVER
WSADATA wsaData;
WORD version = MAKEWORD(2, 2);
SOCKET client;
sockaddr_in serverAddr;
#pragma endregion

LRESULT CALLBACK WndProcMain(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProcSearch(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX w_main; hInst = hInstance;
	ZeroMemory(&w_main, sizeof(WNDCLASSEX));
	w_main.cbSize = sizeof(WNDCLASSEX);
	w_main.style = CS_HREDRAW | CS_VREDRAW;
	w_main.lpfnWndProc = WndProcMain;
	w_main.cbClsExtra = 0;
	w_main.cbWndExtra = 0;
	w_main.hInstance = hInstance;
	w_main.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	w_main.hCursor = LoadCursor(NULL, IDC_ARROW);
	w_main.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	w_main.lpszMenuName = NULL;
	w_main.lpszClassName = szWindowMain;
	w_main.hIconSm = LoadIcon(w_main.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	if (!RegisterClassEx(&w_main))
	{
		MessageBox(NULL, "Call to RegisterClassEx failed!", "Win32 Guided Tour", NULL);
		return 1;
	}
	hMain = CreateWindow(w_main.lpszClassName, szMain, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 700, 400, NULL, NULL, hInstance, NULL);
	if (!hMain)
	{
		MessageBox(NULL, "Call to CreateWindow failed!", "Win32 Guided Tour", NULL);
		return 1;
	}
	ShowWindow(hMain, nCmdShow);
	UpdateWindow(hMain);

	WNDCLASSEX w_search;
	ZeroMemory(&w_search, sizeof(WNDCLASSEX));
	w_search.cbSize = sizeof(WNDCLASSEX);
	w_search.style = CS_HREDRAW | CS_VREDRAW;
	w_search.lpfnWndProc = WndProcSearch;
	w_search.cbClsExtra = 0;
	w_search.cbWndExtra = 0;
	w_search.hInstance = hInstance;
	w_search.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	w_search.hCursor = LoadCursor(NULL, IDC_ARROW);
	w_search.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	w_search.lpszMenuName = NULL;
	w_search.lpszClassName = szWindowSearch;
	w_search.hIconSm = LoadIcon(w_search.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	if (!RegisterClassEx(&w_search))
	{
		MessageBox(NULL, "Call to RegisterClassEx failed!", "Win32 Guided Tour", NULL);
		return 1;
	}
	hSearch = CreateWindow(w_search.lpszClassName, szSearch, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 400, 200, NULL, NULL, hInstance, NULL);
	if (!hSearch)
	{
		MessageBox(NULL, "Call to CreateWindow failed!", "Win32 Guided Tour", NULL);
		return 1;
	}
	UpdateWindow(hSearch);
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

void DrawMainWindow(HWND window) {
	btnConnect = CreateWindow("button", "Connect", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 315, 20, 80, 30, window, (HMENU)buttonConnect, hInst, NULL);
	btnBrowse = CreateWindow("button", "Browse", WS_CHILD | SW_HIDE | BS_DEFPUSHBUTTON, 460, 90, 70, 20, window, (HMENU)buttonBrowse, hInst, NULL);
	btnForward = CreateWindow("button", "Forward", WS_CHILD | SW_HIDE | BS_DEFPUSHBUTTON, 540, 90, 70, 20, window, (HMENU)buttonForward, hInst, NULL);
	btnHide = CreateWindow("button", "Hide", WS_CHILD | SW_HIDE | BS_DEFPUSHBUTTON, 460, 120, 70, 20, window, (HMENU)buttonHide, hInst, NULL);
	btnSearch = CreateWindow("button", "Search", WS_CHILD | SW_HIDE | BS_DEFPUSHBUTTON, 460, 200, 70, 20, window, (HMENU)buttonSearch, hInst, NULL);
	btnClean = CreateWindow("button", "Clean All", WS_CHILD | SW_HIDE | BS_DEFPUSHBUTTON, 315, 300, 80, 30, window, (HMENU)buttonClean, hInst, NULL);

	eFile = CreateWindow("Edit", "", WS_CHILD | WS_VISIBLE | WS_BORDER, 100, 90, 350, 20, window, (HMENU)editFile, NULL, NULL);
	eFileName = CreateWindow("Edit", "", WS_CHILD | WS_VISIBLE | WS_BORDER, 100, 200, 350, 20, window, (HMENU)editFileName, NULL, NULL);
	eParnerId = CreateWindow("Edit", "", WS_CHILD | SW_HIDE | WS_BORDER, 100, 120, 350, 20, window, (HMENU)editParnerId, NULL, NULL);
	eIdDetail = CreateWindow("Edit", "", WS_VISIBLE | WS_CHILD | ES_READONLY, 480, 0, 180, 20, window, (HMENU)editIdDetail, NULL, NULL);

	sFile = CreateWindow("STATIC", "Address", WS_VISIBLE | WS_CHILD | SS_RIGHT, 30, 90, 55, 20, window, (HMENU)staticFile, NULL, NULL);
	sFileName = CreateWindow("STATIC", "Port", WS_VISIBLE | WS_CHILD | SS_RIGHT, 20, 200, 65, 20, window, (HMENU)staticFileName, NULL, NULL);
	sParnerId = CreateWindow("STATIC", "", WS_VISIBLE | WS_CHILD | SS_RIGHT, 20, 120, 65, 20, window, (HMENU)staticParnerId, NULL, NULL);
	sID = CreateWindow("STATIC", "ID", WS_VISIBLE | WS_CHILD | SS_RIGHT, 450, 0, 20, 20, window, (HMENU)staticId, NULL, NULL);
}

void DrawSearchWindow(HWND window) {
	sIDSearch = CreateWindow("STATIC", "ID", WS_VISIBLE | WS_CHILD | SS_RIGHT, 30, 90, 55, 20, window, (HMENU)staticIDSearch, NULL, NULL);
	sFileNameSearch = CreateWindow("STATIC", "", WS_VISIBLE | WS_CHILD | SS_CENTER, 160, 0, 80, 20, window, (HMENU)staticFNSearch, NULL, NULL);
	btnSend = CreateWindow("button", "SEND", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 160, 30, 80, 30, window, (HMENU)buttonSend, hInst, NULL);
	eID = CreateWindow("Edit", "", WS_CHILD | WS_VISIBLE | WS_BORDER, 100, 90, 200, 20, window, (HMENU)editID, NULL, NULL);
}

#pragma region LISTEN SERVER

unsigned _stdcall ListenServer(void* param) {
	return 0;
}
#pragma endregion


#pragma region BUTTON CONNECT
void BnClickedDrawConnect() {
	ShowWindow(btnBrowse, SW_SHOW);
	ShowWindow(btnForward, SW_SHOW);
	ShowWindow(btnSearch, SW_SHOW);
	ShowWindow(btnClean, SW_SHOW);
	SetWindowTextA(btnConnect, "Disconnect");
	SetWindowTextA(sFile, "File");
	SetWindowTextA(sFileName, "FileName");
	SetWindowTextA(eFile, "");
	SetWindowTextA(eFileName, "");
	isConnect = true;
}

void BnClickedDrawDisconnect() {
	ShowWindow(btnBrowse, SW_HIDE);
	ShowWindow(btnForward, SW_HIDE);
	ShowWindow(btnSearch, SW_HIDE);
	ShowWindow(btnClean, SW_HIDE);
	ShowWindow(btnHide, SW_HIDE);
	ShowWindow(eParnerId, SW_HIDE);
	SetWindowTextA(btnConnect, "Connect");
	SetWindowTextA(sFile, "Address");
	SetWindowTextA(sFileName, "Port");
	SetWindowTextA(eFile, "");
	SetWindowTextA(eFileName, "");
	SetWindowTextA(eIdDetail, "");
	isConnect = false;
}

int BnClickedMakeConnect(char* address, u_short port) {
	if (WSAStartup(version, &wsaData)) return -1;
	client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	int tv = 10000;
	setsockopt(client, SOL_SOCKET, SO_RCVTIMEO, (const char*)(&tv), sizeof(int));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = inet_addr(address);
	if (connect(client, (sockaddr*)&serverAddr, sizeof(serverAddr))) return -2;
	int ret = SEND_TCP(client, 300, new char[1]{ 0 }, 0);
	if (ret == SOCKET_ERROR) return -3;
	int type;
	char* buff = new char[BUFF_SIZE];
	ret = RECEIVE_TCP(client, &type, buff, 0);
	if (type == 100) SetWindowTextA(eIdDetail, buff);
	return 1;
}

void BnClickedMakeDisconnect() {
	closesocket(client);
	WSACleanup();
}

void OnBnClickedConnect(HWND window) {
	if (isConnect) {
		int id = MessageBox(window, "Are you sure?", "WARNING", MB_ICONWARNING | MB_OKCANCEL);
		if (id == IDOK) {
			BnClickedMakeDisconnect();
			BnClickedDrawDisconnect();
		}
	}
	else {
		TCHAR* address = new TCHAR[1024], *port = new TCHAR[1024];
		GetWindowText(eFile, address, 1024);
		GetWindowText(eFileName, port, 1024);
		char* cAddress = address;
		u_short iPort = atoi(port);
		if (CheckConnect(cAddress, iPort)) {
		node:
			int status = BnClickedMakeConnect(cAddress, iPort);
			if (status == -1) MessageBox(window, "Version is not supported", "ERROR", MB_ICONERROR);
			else if (status == -2) {
				int id = MessageBox(window, "Can not connect server", "ERROR", MB_RETRYCANCEL | MB_ICONERROR);
				if (id == IDRETRY) goto node;
			}
			else if (status == -3) {
				int id = MessageBox(window, "Can not get ID", "ERROR", MB_RETRYCANCEL | MB_ICONERROR);
				if (id == IDRETRY) goto node;
			}
			else if (status == 1) {
				BnClickedDrawConnect();
				_beginthreadex(0, 0, ListenServer, NULL, 0, 0);
			}
		}
		else {
			MessageBox(window, "Wrong IP address or port", "ERROR", MB_ICONERROR);
			SetWindowTextA(eFile, "");
			SetWindowTextA(eFileName, "");
		}
	}
}
#pragma endregion

void OnBnClickedForward(HWND window) {

}

void OnBnClickedBrowse(HWND window) {

}

void OnBnClickedHide(HWND window) {

}

void OnBnClickedSearch(HWND window) {

}

void OnBnClickedClean(HWND window) {

}

void OnBnClickedSend(HWND window) {

}

//handler hMain event
LRESULT CALLBACK WndProcMain(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message)
	{
	case WM_COMMAND:
		if ((HWND)lParam && (HIWORD(wParam)) == BN_CLICKED) {
			int id = LOWORD(wParam);
			if (id == buttonForward) OnBnClickedForward(hWnd);
			else if (id == buttonBrowse) OnBnClickedBrowse(hWnd);
			else if (id == buttonHide) OnBnClickedHide(hWnd);
			else if (id == buttonConnect) OnBnClickedConnect(hWnd);
			else if (id == buttonSearch) OnBnClickedSearch(hWnd);
			else if (id == buttonClean) OnBnClickedClean(hWnd);
		}
		break;
	case WM_CREATE:
		DrawMainWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

//handler hSearch event
LRESULT CALLBACK WndProcSearch(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message)
	{
	case WM_COMMAND:
		if ((HWND)lParam && (HIWORD(wParam)) == BN_CLICKED) {
			int id = LOWORD(wParam);
			if (id == buttonSend) OnBnClickedSend(hWnd);
		}
		break;
	case WM_CREATE:
		DrawSearchWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}
