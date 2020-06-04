#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "InputAndData.h"
#include "TCP_SOCKET.h"
#include "InteractFile.h"

HWND hWnd;
HINSTANCE hInst;
static HWND btnBrowse, btnForward, btnSearch, btnHide, btnConnect;
static HWND eFile, eFileName, eParnerId, eIdDetail;
static HWND sFile, sFileName, sParnerId, sID;
bool isConnect = false, isHide = true;

#pragma region CONNECT SERVER
WSADATA wsaData;
WORD version = MAKEWORD(2, 2);
SOCKET client;
sockaddr_in serverAddr;
int ret;
char* cPathToFile = new char[BUFF_SIZE];
char* cParnerID = new char[BUFF_SIZE];
list<char*> payload;
#pragma endregion

struct PAYLOAD
{
	list<char*> data;
};

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
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
		MessageBox(NULL, "Call to RegisterClassEx failed!", "Win32 Guided Tour", NULL);
		return 1;
	}

	hInst = hInstance;
	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 700, 400, NULL, NULL, hInstance, NULL);
	if (!hWnd)
	{
		MessageBox(NULL, "Call to CreateWindow failed!", "Win32 Guided Tour", NULL);
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
	eIdDetail = CreateWindow("Edit", "", WS_VISIBLE | WS_CHILD | ES_READONLY, 480, 0, 180, 20, hWnd, (HMENU)editIdDetail, NULL, NULL);

	sFile = CreateWindow("STATIC", "Address", WS_VISIBLE | WS_CHILD | SS_RIGHT, 30, 90, 55, 20, hWnd, (HMENU)staticFile, NULL, NULL);
	sFileName = CreateWindow("STATIC", "Port", WS_VISIBLE | WS_CHILD | SS_RIGHT, 20, 200, 65, 20, hWnd, (HMENU)staticFileName, NULL, NULL);
	sParnerId = CreateWindow("STATIC", "", WS_VISIBLE | WS_CHILD | SS_RIGHT, 20, 120, 65, 20, hWnd, (HMENU)staticParnerId, NULL, NULL);
	sID = CreateWindow("STATIC", "ID", WS_VISIBLE | WS_CHILD | SS_RIGHT, 450, 0, 20, 20, hWnd, (HMENU)staticId, NULL, NULL);
}

#pragma region LISTEN SERVER
unsigned _stdcall ForwardFile(void* param) {
	TCHAR* pathToFile = new TCHAR[1024];
	GetWindowText(eFile, pathToFile, 1024);
	list<string> payload = CreatePayload(pathToFile);
	for (string data : payload) {
		ret = SEND_TCP(client, o_401, StringToChars(data), 0, 0);
		if (ret == SOCKET_ERROR) MessageBox(hWnd, "Can not send to server", "ERROR", MB_OK);
	}
	ret = SEND_TCP(client, o_401, new char[1]{ 0 }, 0, 0);
	if (ret == SOCKET_ERROR) MessageBox(hWnd, "Can not send to server", "ERROR", MB_OK);
	return 0;
}

unsigned _stdcall SaveForwardFile(void* param) {
	PAYLOAD* payload = (PAYLOAD*)param;
	string fileName(*payload->data.begin());
	fstream f; f.open(fileName, ios::out);
	list<char*>::iterator pointer = payload->data.begin();
	pointer++;
	for (; pointer != payload->data.end(); pointer++) {
		string temp(*pointer);
		f << temp;
	}
	f.close();
	MessageBox(hWnd, "RECEIVE FORWARD FILE FINISH", "ANNOUNT", MB_OK);
	return 0;
}

unsigned _stdcall SearchFile(void* param) {
	char* fileName = (char*)param;
	string sFileName(fileName);
	bool check = SearchFileByName(sFileName);
	if (check) {
		ret = SEND_TCP(client, o_321, fileName, 0, 0);
		if (ret == SOCKET_ERROR) MessageBox(hWnd, "Can not send to server", "ERROR", MB_OK);
	}
	else {
		ret = SEND_TCP(client, o_320, new char[1]{ 0 }, 0, 0);
		if (ret == SOCKET_ERROR) MessageBox(hWnd, "Can not send to server", "ERROR", MB_OK);
	}
	return 0;
}

unsigned _stdcall ListenServer(void* param) {
	int ret, offset;
	char* opcode = new char[10];
	char* data = new char[BUFF_SIZE];
	//char* fileName = new char[BUFF_SIZE];
	PAYLOAD payload;
	while (true)
	{
		ret = RECEIVE_TCP(client, opcode, data, 0, &offset);
		if (ret == SOCKET_ERROR) continue;
		data[ret] = 0;
		if (!strcmp(opcode, o_120)) {
			char* temp = new char[100]{ "Request find file with name: " };
			char* temp1 = new char[100]{ "Do you allow find?" };
			strcat_s(temp, strlen(data) + strlen(temp) + 1, data);
			strcat_s(temp, strlen(temp) + strlen(temp1) + 1, temp1);
			int id = MessageBox(hWnd, _T(temp), "ANNOUNT", MB_OKCANCEL);
			if (id == IDCANCEL) {
				ret = SEND_TCP(client, o_320, new char[1]{ 0 }, 0, 0);
				if (ret == SOCKET_ERROR) MessageBox(hWnd, "Can not send to server", "ERROR", MB_OK);
			}
			else if (id == IDOK) _beginthreadex(0, 0, SearchFile, (void*)data, 0, 0);
		}

		else if (!strcmp(opcode, o_200)) {
			char* temp = new char[100]{ "Request forward from client: " };
			strcat_s(temp, strlen(temp) + strlen(data) + 1, data);
			int id = MessageBox(hWnd, _T(temp), "ANNOUNT", MB_OKCANCEL);
			if (id == IDCANCEL) {
				ret = SEND_TCP(client, o_410, data, 0, 0);
				if (ret == SOCKET_ERROR) MessageBox(hWnd, "Can not send to server", "ERROR", MB_OK);
			}
			else if (id == IDOK) {
				ret = SEND_TCP(client, o_411, data, 0, 0);
				if (ret == SOCKET_ERROR) MessageBox(hWnd, "Can not send to server", "ERROR", MB_OK);
			}
		}

		else if (!strcmp(opcode, o_201)) {
			if (offset == 0) payload.data.push_back(data);
			else {
				if (!strcmp(data, "")) _beginthreadex(0, 0, SaveForwardFile, (void*)&payload, 0, 0);
				else payload.data.push_back(data);
			}
		}

		else if (!strcmp(opcode, o_203)) {
			MessageBox(hWnd, "Can not find ID or not allow forward", "ERROR", MB_OK);
			SetWindowTextA(eParnerId, "");
		}

		else if (!strcmp(opcode, o_202)) {
			MessageBox(hWnd, "Beginning upload file to server", "ANNOUNT", MB_OK);
			_beginthreadex(0, 0, ForwardFile, NULL, 0, 0);
		}
	}
}
#pragma endregion

#pragma region BUTTON CONNECT
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
	ret = SEND_TCP(client, o_300, new char[1]{ 0 }, 0, 0);
	if (ret == SOCKET_ERROR) return SOCKET_ERROR;

	int offset = 0;
	char* opcode = new char[10];
	char* buff = new char[BUFF_SIZE];
	ret = RECEIVE_TCP(client, opcode, buff, 0, &offset);
	if (ret == SOCKET_ERROR) return SOCKET_ERROR;
	buff[ret] = 0;
	if (!strcmp(opcode, o_100)) SetWindowTextA(eIdDetail, buff);
}

void BnClickedMakeDisconnect() {
	closesocket(client);
	WSACleanup();
}

void OnBnClickedConnect(HWND hWnd) {
	if (isConnect) {
		int id = MessageBox(hWnd, "Are you sure?", "WARNING", MB_OKCANCEL);
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
			int status = BnClickedMakeConnect(cAddress, iPort);
			if (status == -1) MessageBox(hWnd, "Version is not supported", "ERROR", MB_OK);
			else if (status == -2) MessageBox(hWnd, "Can not connect server", "ERROR", MB_OK);
			else if (status == SOCKET_ERROR) MessageBox(hWnd, "Can not get ID", "ERROR", MB_OK);
			else if (status == 1) {
				BnClickedDrawConnect();
				_beginthreadex(0, 0, ListenServer, NULL, 0, 0);
			}
		}
		else {
			MessageBox(hWnd, "Wrong IP address or port", "ERROR", MB_OK);
			SetWindowTextA(eFile, "");
			SetWindowTextA(eFileName, "");
		}
	}
}
#pragma endregion

void OnBnClickedForward(HWND hWnd) {
	if (isHide) {
		MessageBox(hWnd, "Enter my parner ID", "ANNOUNT", MB_OK);
		ShowWindow(btnHide, SW_SHOW);
		ShowWindow(eParnerId, SW_SHOW);
		ShowWindow(sParnerId, SW_SHOW);
		SetWindowTextA(sParnerId, "Parner ID");
		isHide = false;
	}
	else {
		TCHAR* pathToFile = new TCHAR[1024];
		TCHAR* parnerID = new TCHAR[1024];
		GetWindowText(eFile, pathToFile, 1024);
		GetWindowText(eParnerId, parnerID, 1024);
		if (IsFileExistOrValid(pathToFile) && (string)parnerID != "") {
			strcpy_s(cPathToFile, strlen(pathToFile) + 1, pathToFile);
			strcpy_s(cParnerID, strlen(parnerID) + 1, parnerID);
			ret = SEND_TCP(client, o_400, cParnerID, 0, 0);
			if (ret == SOCKET_ERROR) MessageBox(hWnd, "Can not send to server", "ERROR", MB_OK);
			ret = SEND_TCP(client, o_400, StringToChars(GetFileName(pathToFile)), 0, 1);
			if (ret == SOCKET_ERROR) MessageBox(hWnd, "Can not send to server", "ERROR", MB_OK);
		}
		else {
			MessageBox(hWnd, "Wrong your path of file or parner ID", "ERROR", MB_OK);
			SetWindowTextA(eFile, "");
			SetWindowTextA(eParnerId, "");
		}
	}
}

void OnBnClickedSearch(HWND hWnd) {
	TCHAR* fileName = new TCHAR[BUFF_SIZE];
	GetWindowText(eFileName, fileName, BUFF_SIZE);
	char* cFileName = fileName;
	if (!strcmp(cFileName, "")) MessageBox(hWnd, "Must be enter file name", "ANNOUNT", MB_OK);
	else {
		ret = SEND_TCP(client, o_310, cFileName, 0, 0);
		if (ret == SOCKET_ERROR) MessageBox(hWnd, "Can not send to server", "ERROR", MB_OK);
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

void OnBnClickedHide(HWND hWnd) {
	ShowWindow(btnHide, SW_HIDE);
	ShowWindow(eParnerId, SW_HIDE);
	SetWindowTextA(sParnerId, "");
	isHide = true;
}

//handler event
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
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
			else if (id == bConnect) OnBnClickedConnect(hWnd);
			else if (id == bSearch) OnBnClickedSearch(hWnd);
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
