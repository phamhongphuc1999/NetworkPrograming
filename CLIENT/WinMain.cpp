#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "InputAndData.h"
#include "TCP_SOCKET.h"
#include "InteractFile.h"

HINSTANCE hInst;
static HWND btnBrowse, btnForward, btnSearch, btnHide, btnConnect;
static HWND eFile, eFileName, eParnerId;
static HWND sFile, sFileName, sParnerId, sID, sIdDetail;
bool isConnect = false, isHide = true;

#pragma region CONNECT SERVER
WSADATA wsaData;
WORD version = MAKEWORD(2, 2);
SOCKET client;
sockaddr_in serverAddr;
int ret;
#pragma endregion

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL, _T("Call to RegisterClassEx failed!"), _T("Win32 Guided Tour"), NULL);
		return 1;
	}

	hInst = hInstance;
	HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 700, 400, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		MessageBox(NULL, _T("Call to CreateWindow failed!"), _T("Win32 Guided Tour"), NULL);
		return 1;
	}
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

void InitializeController(HWND hWnd) {
	btnConnect = CreateWindow("button", "Connect", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 315, 20, 80, 30, hWnd, (HMENU)bConnect, hInst, NULL);
	btnBrowse = CreateWindow("button", "Browse", WS_CHILD | SW_HIDE | BS_DEFPUSHBUTTON, 460, 90, 70, 20, hWnd, (HMENU)bBrowse, hInst, NULL);
	btnForward = CreateWindow("button", "Forward", WS_CHILD | SW_HIDE | BS_DEFPUSHBUTTON, 540, 90, 70, 20, hWnd, (HMENU)bForward, hInst, NULL);
	btnHide = CreateWindow("button", "Hide", WS_CHILD | SW_HIDE | BS_DEFPUSHBUTTON, 460, 120, 70, 20, hWnd, (HMENU)bHide, hInst, NULL);
	btnSearch = CreateWindow("button", "Search", WS_CHILD | SW_HIDE | BS_DEFPUSHBUTTON, 460, 200, 70, 20, hWnd, (HMENU)bSearch, hInst, NULL);

	eFile = CreateWindow("Edit", "", WS_CHILD | WS_VISIBLE | WS_BORDER, 100, 90, 350, 20, hWnd, (HMENU)editFile, NULL, NULL);
	eFileName = CreateWindow("Edit", "", WS_CHILD | WS_VISIBLE | WS_BORDER, 100, 200, 350, 20, hWnd, (HMENU)editFileName, NULL, NULL);
	eParnerId = CreateWindow("Edit", "", WS_CHILD | SW_HIDE | WS_BORDER, 100, 120, 350, 20, hWnd, (HMENU)editParnerId, NULL, NULL);

	sFile = CreateWindow("STATIC", "Address", WS_VISIBLE | WS_CHILD | SS_RIGHT, 30, 90, 55, 20, hWnd, (HMENU)staticFile, NULL, NULL);
	sFileName = CreateWindow("STATIC", "Port", WS_VISIBLE | WS_CHILD | SS_RIGHT, 20, 200, 65, 20, hWnd, (HMENU)staticFileName, NULL, NULL);
	sParnerId = CreateWindow("STATIC", "", WS_VISIBLE | WS_CHILD | SS_RIGHT, 20, 120, 65, 20, hWnd, (HMENU)staticParnerId, NULL, NULL);
	sID = CreateWindow("STATIC", "ID", WS_VISIBLE | WS_CHILD | SS_RIGHT, 450, 0, 20, 20, hWnd, (HMENU)staticId, NULL, NULL);
	sIdDetail = CreateWindow("STATIC", "", WS_VISIBLE | WS_CHILD | SS_RIGHT, 480, 0, 200, 20, hWnd, (HMENU)staticIdDetail, NULL, NULL);
}

void BnClickedDrawConnect() {
	ShowWindow(btnBrowse, SW_SHOW);
	ShowWindow(btnForward, SW_SHOW);
	ShowWindow(btnSearch, SW_SHOW);
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
	ShowWindow(btnHide, SW_HIDE);
	ShowWindow(eParnerId, SW_HIDE);
	SetWindowTextA(btnConnect, "Connect");
	SetWindowTextA(sFile, "Address");
	SetWindowTextA(sFileName, "Port");
	SetWindowTextA(eFile, "");
	SetWindowTextA(eFileName, "");
	isConnect = false;
}

int BnClickedMakeConnect(char* address, int port) {
	if (WSAStartup(version, &wsaData)) return -1;
	client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	int tv = 10000;
	setsockopt(client, SOL_SOCKET, SO_RCVTIMEO, (const char*)(&tv), sizeof(int));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = inet_addr(address);
	if (connect(client, (sockaddr*)&serverAddr, sizeof(serverAddr))) return -2;
	ret = SEND_TCP(client, new char[4]{ "300" }, new char[1]{ 0 }, 0);
	if (ret == SOCKET_ERROR) return SOCKET_ERROR;
	else return 1;
}

void BnClickedMakeDisconnect() {

}

unsigned _stdcall ListenServer(void* param) {
	SOCKET client = (SOCKET)param;
	char* opcode = new char[10];
	char* buff = new char[BUFF_SIZE];
	int ret;
	while (true) {
		int ret = RECEIVE_TCP(client, opcode, buff, 0);
		if (!strcmp(opcode, new char[4]{ "100" })) {
			SetWindowTextA(sIdDetail, opcode);
		}
	}
	return 0;
}

void OnBnClickedConnect(HWND hWnd) {
	if (isConnect) {
		int id = MessageBox(hWnd, "Are you sure?", "WARNING", MB_OKCANCEL);
		if (id == IDOK) {
			BnClickedDrawDisconnect();
			BnClickedMakeDisconnect();
		}
	}
	else {
		TCHAR* address = new TCHAR[1024], *port = new TCHAR[1024];
		GetWindowText(eFile, address, 1024);
		GetWindowText(eFileName, port, 1024);
		char* cAddress = address;
		u_short iPort = atoi(port);
		if (CheckConnect(cAddress, iPort)) {
			BnClickedDrawConnect();
			int status = BnClickedMakeConnect(cAddress, iPort);
			if (status == -1) MessageBox(hWnd, "Version is not supported", "Annount", MB_OK);
			else if (status == -2) MessageBox(hWnd, "Can not connect server", "Annount", MB_OK);
			else if (status == SOCKET_ERROR) MessageBox(hWnd, "Can not get ID", "Annount", MB_OK);
			else if (status == 1) {
				MessageBox(hWnd, "Connected to server", "Annount", MB_OK);
				_beginthreadex(0, 0, ListenServer, (void*)client, 0, 0);
			}
		}
		else {
			MessageBox(hWnd, "Wrong IP address or port", "ERROR", MB_OK);
			SetWindowTextA(eFile, "");
			SetWindowTextA(eFileName, "");
		}
	}
}

void OnBnClickedBrowse(HWND hWnd) {
	OPENFILENAME ofn;
	char szFile[100];
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	GetOpenFileName(&ofn);
	if (strcmp(ofn.lpstrFile, "")) SetWindowTextA(eFile, ofn.lpstrFile);
}

void OnBnClickedForward(HWND hWnd) {
	if (isHide) {
		MessageBox(hWnd, "Enter my parner ID", "Annount", MB_OK);
		ShowWindow(btnHide, SW_SHOW);
		ShowWindow(eParnerId, SW_SHOW);
		ShowWindow(sParnerId, SW_SHOW);
		SetWindowTextA(sParnerId, "Parner ID");
		isHide = false;
	}
	else {
		
	}
}

void OnBnClickedHide(HWND hWnd) {
	ShowWindow(btnHide, SW_HIDE);
	ShowWindow(eParnerId, SW_HIDE);
	SetWindowTextA(sParnerId, "");
	isHide = true;
}

//handler event
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
	HWND hwndButton;
	PAINTSTRUCT ps;
	HDC hdc;
	switch (message)
	{
	case WM_COMMAND:
		if ((HWND)lParam && (HIWORD(wParam)) == BN_CLICKED) {
			int id = LOWORD(wParam);
			if (id == bBrowse) OnBnClickedBrowse(hWnd);
			else if (id == bHide) OnBnClickedHide(hWnd);
			else if (id == bForward) OnBnClickedForward(hWnd);
			else if (id = bConnect) OnBnClickedConnect(hWnd);
		}
		break;
	case WM_CREATE:
		InitializeController(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_DRAWITEM:
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	return 0;
}
