#include "Common.h"
#include "TCP_SOCKET.h"
#include "InteractFile.h"

HWND hMain, hSearch;
HINSTANCE hInst;
//controller in main window
static HWND btnBrowse, btnForward, btnSearch, btnHide, btnConnect, btnClean;
static HWND eFile, eFileName, ePartnerId, eIdDetail;
static HWND sFile, sFileName, sPartnerId, sID;
bool isConnect = false, isHide = true;

//controller in search window
static HWND sFileNameSearch;
static HWND listBoxID;

//connect server
WSADATA wsaData;
WORD version = MAKEWORD(2, 2);
SOCKET client;
sockaddr_in serverAddr;

map<string, SearchInfo> mapSearch;
map<string, map<string, ForwardInfoSend>> mapForwardSend;
map<string, map<string, ForwardInfoReceive>> mapForwardReceive;

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
	hSearch = CreateWindow(w_search.lpszClassName, szSearch, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 400, 300, NULL, NULL, hInstance, NULL);
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
	ePartnerId = CreateWindow("Edit", "", WS_CHILD | SW_HIDE | WS_BORDER, 100, 120, 350, 20, window, (HMENU)editPartnerId, NULL, NULL);
	eIdDetail = CreateWindow("Edit", "", WS_VISIBLE | WS_CHILD | ES_READONLY, 480, 0, 180, 20, window, (HMENU)editIdDetail, NULL, NULL);

	sFile = CreateWindow("STATIC", "Address", WS_VISIBLE | WS_CHILD | SS_RIGHT, 30, 90, 55, 20, window, (HMENU)staticFile, NULL, NULL);
	sFileName = CreateWindow("STATIC", "Port", WS_VISIBLE | WS_CHILD | SS_RIGHT, 20, 200, 65, 20, window, (HMENU)staticFileName, NULL, NULL);
	sPartnerId = CreateWindow("STATIC", "", WS_VISIBLE | WS_CHILD | SS_RIGHT, 20, 120, 65, 20, window, (HMENU)staticPartnerId, NULL, NULL);
	sID = CreateWindow("STATIC", "ID", WS_VISIBLE | WS_CHILD | SS_RIGHT, 450, 0, 20, 20, window, (HMENU)staticId, NULL, NULL);
}

void DrawSearchWindow(HWND window) {
	sFileNameSearch = CreateWindow("STATIC", "", WS_VISIBLE | WS_CHILD | SS_CENTER, 160, 0, 80, 20, window, (HMENU)staticFNSearch, NULL, NULL);
	listBoxID = CreateWindow("LISTBOX", NULL, WS_VISIBLE | WS_CHILD | LBS_STANDARD | LBS_NOTIFY, 100, 40, 200, 100, window, (HMENU)lbID, NULL, NULL);
}

void SendDataToSearchWindow(list<char*> listID, char* fileName) {
	SetWindowTextA(sFileNameSearch, fileName);
	for (char* ID : listID)
		SendMessage(GetDlgItem(hSearch, lbID), LB_ADDSTRING, 0, (LPARAM)ID);
}

#pragma region LISTEN SERVER
unsigned _stdcall SearchFile(void* param) {
	Message* searchInfo = (Message*)param;
	string fileName(searchInfo->fileName);
	bool check = SearchFileInDirectory("Data", fileName);
	if (check) {
		searchInfo->type = 321;
		int ret = SEND_TCP(client, *searchInfo, 0);
		if (ret == SOCKET_ERROR) MessageBox(hMain, "Can not send to server", "ERROR", MB_ICONERROR);
	}
	else {
		searchInfo->type = 320;
		int ret = SEND_TCP(client, *searchInfo, 0);
		if (ret == SOCKET_ERROR) MessageBox(hMain, "Can not send to server", "ERROR", MB_ICONERROR);
	}
	return 0;
}

unsigned _stdcall SendSearchFileToServer(void* param) {
	Message* searchFile = (Message*)param;
	FileData fileData = CreatePayload("Data/" + string(searchFile->fileName));
	Message message; int ret;
	for (int i = 0; i < fileData.length; i++) {
		CreateMessage(&message, 311, searchFile->fileName, searchFile->ID, fileData.data[i]);
		ret = SEND_TCP(client, message, 0);
		if (ret == SOCKET_ERROR) MessageBox(hMain, "Can not send to server", "ERROR", MB_ICONERROR);
	}
	CreateMessage(&message, 311, searchFile->fileName, searchFile->ID, 0);
	ret = SEND_TCP(client, message, 0);
	if (ret == SOCKET_ERROR) MessageBox(hMain, "Can not send to server", "ERROR", MB_ICONERROR);
	return 0;
}

unsigned _stdcall ReceiveSearchFileToServer(void* param) {
	SearchInfo* info = (SearchInfo*)param;
	ofstream f("Data/" + string(info->fileName), ios::out | ios::binary);
	for (char* data : info->data) {
		f.write(data, strlen(data));
	}
	f.close();
	MessageBox(hMain, "RECEIVE SEARCH FILE FINISH IN CLIENT TEST", "ANNOUNT", MB_ICONINFORMATION);
	mapSearch.erase(string(info->fileName));
	return 0;
}

unsigned _stdcall ReceiveListSearchID(void* param) {
	SearchInfo* searchList = (SearchInfo*)param;
	if (searchList->data.size() == 0) {
		char* fileName = new char[BUFF_SIZE] {"do not find the file: "};
		strcat_s(fileName, strlen(fileName) + strlen(searchList->fileName) + 1, searchList->fileName);
		MessageBox(hMain, fileName, "ANNOUNT", MB_ICONINFORMATION);
	}
	else {
		char* result = new char[22 * searchList->data.size()]{ 0 };
		char* space = new char[2]{ "\n" };
		for (char* ID : searchList->data) {
			strcat_s(result, strlen(ID) + strlen(result) + 1, ID);
			strcat_s(result, strlen(result) + strlen(space) + 1, space);
		}
		char* fileName = new char[BUFF_SIZE] {"IDs have the file: "};
		strcat_s(fileName, strlen(fileName) + strlen(searchList->fileName) + 1, searchList->fileName);
		SendDataToSearchWindow(searchList->data, searchList->fileName);
		ShowWindow(hSearch, SW_SHOW);
	}
	return 0;
}

unsigned _stdcall SendForwardFile(void* param) {
	ForwardInfoSend* info = (ForwardInfoSend*)param;
	int ret; Message message;
	FileData fileData = CreatePayload(string(info->pathToFile));
	for (int i = 0; i < fileData.length; i++) {
		CreateMessage(&message, 401, info->fileName, info->ID, fileData.data[i]);
		ret = SEND_TCP(client, message, 0);
		if (ret == SOCKET_ERROR) MessageBox(hMain, "Can not send to server", "ERROR", MB_ICONERROR);
	}
	CreateMessage(&message, 401, info->fileName, info->ID, 0);
	ret = SEND_TCP(client, message, 0);
	if (ret == SOCKET_ERROR) MessageBox(hMain, "Can not send to server", "ERROR", MB_ICONERROR);
	mapForwardSend[string(info->ID)].erase(string(info->fileName));
	return 0;
}

unsigned _stdcall ReceiveForwardFile(void* param) {
	ForwardInfoReceive* info = (ForwardInfoReceive*)param;
	ofstream f("Data/" + string(info->fileName), ios::out | ios::binary);
	for (char* data : info->data) {
		f.write(data, strlen(data));
	}
	f.close();
	MessageBox(hMain, "RECEIVE FORWARD FILE FINISH IN CLIENT TEST", "ANNOUNT", MB_ICONINFORMATION);

	return 0;
}

unsigned _stdcall ListenServer(void* param) {
	int ret; Message message;
	while (true)
	{
		ret = RECEIVE_TCP(client, &message, 0);
		if (ret == SOCKET_ERROR) continue;

		if (message.type == 111) {
			char* temp = new char[BUFF_SIZE];
			string fileName(message.fileName);
			strcpy_s(temp, strlen(message.ID) + 1, message.ID);
			if (strcmp(message.ID, "")) mapSearch[fileName].data.push_back(temp);
			else {

				_beginthreadex(0, 0, ReceiveListSearchID, (void*)&mapSearch[fileName], 0, 0);
			}
		}

		else if (message.type == 112) {
			char* temp = new char[BUFF_SIZE];
			string fileName(message.fileName);
			strcpy_s(temp, strlen(message.data) + 1, message.data);
			if (strcmp(message.data, "")) mapSearch[fileName].data.push_back(temp);
			else _beginthreadex(0, 0, ReceiveSearchFileToServer, (void*)&mapSearch[fileName], 0, 0);
		}

		else if (message.type == 120) {
			char* temp1 = new char[100]{ "Require find the file with name: " };
			char* temp2 = new char[20]{ " to client: " };
			char* temp3 = new char[100]{ "\nDo you allow find?" };
			strcat_s(temp1, strlen(message.fileName) + strlen(temp1) + 1, message.fileName);
			strcat_s(temp1, strlen(temp2) + strlen(temp1) + 1, temp2);
			strcat_s(temp1, strlen(message.ID) + strlen(temp1) + 1, message.ID);
			strcat_s(temp1, strlen(temp3) + strlen(temp1) + 1, temp3);
			int id = MessageBox(hMain, temp1, "ANNOUNT", MB_OKCANCEL | MB_ICONINFORMATION);
			if (id == IDCANCEL) {
				message.type = 320;
				ret = SEND_TCP(client, message, 0);
				if (ret == SOCKET_ERROR) MessageBox(hMain, "Can not send to server", "ERROR", MB_ICONERROR);
			}
			else if (id == IDOK) {
				Message searchInfo; CreateMessage(&searchInfo, 0, message.fileName, message.ID, 0);
				_beginthreadex(0, 0, SearchFile, (void*)&searchInfo, 0, 0);
			}
		}

		else if (message.type == 121) {
			_beginthreadex(0, 0, SendSearchFileToServer, (void*)&message, 0, 0);
		}

		else if (message.type == 200) {
			char* temp1 = new char[100]{ "Request receive forward file " };
			char* temp2 = new char[100]{ " from client " };
			char* temp3 = new char[100]{ "\nDo you allow find?" };
			strcat_s(temp1, strlen(temp1) + strlen(message.fileName) + 1, message.fileName);
			strcat_s(temp1, strlen(temp1) + strlen(temp2) + 1, temp2);
			strcat_s(temp1, strlen(temp1) + strlen(message.ID) + 1, message.ID);
			strcat_s(temp1, strlen(temp3) + strlen(temp1) + 1, temp3);
			int id = MessageBox(hMain, temp1, "ANNOUNT", MB_OKCANCEL | MB_ICONINFORMATION);
			if (id == IDOK) {
				message.type = 411;
				string ID(message.ID);
				string fileName(message.fileName);
				ForwardInfoReceive info;
				strcpy_s(info.fileName, strlen(message.fileName) + 1, message.fileName);
				mapForwardReceive[ID].insert({ fileName, info });
			}
			else if (id == IDCANCEL) message.type = 410;
			int ret = SEND_TCP(client, message, 0);
			if (ret == SOCKET_ERROR) MessageBox(hMain, "Can not send to server", "ERROR", MB_ICONERROR);
		}

		else if (message.type == 201) {
			char* temp = new char[BUFF_SIZE];
			strcpy_s(temp, strlen(message.data) + 1, message.data);
			string ID(message.ID);
			string fileName(message.fileName);
			if (strcmp(message.data, "")) mapForwardReceive[ID][fileName].data.push_back(temp);
			else _beginthreadex(0, 0, ReceiveForwardFile, (void*)&mapForwardReceive[ID][fileName], 0, 0);
		}

		else if (message.type == 202) {
			MessageBox(hMain, "Beginning upload file to server", "ANNOUNT", MB_ICONINFORMATION);
			string ID(message.ID);
			string fileName(message.fileName);
			_beginthreadex(0, 0, SendForwardFile, &mapForwardSend[ID][fileName], 0, 0);
		}

		else if (message.type == 203) {
			char* temp1 = new char[100]{ "Can not forward file " };
			char* temp2 = new char[100]{ " to client " };
			strcat_s(temp1, strlen(temp1) + strlen(message.fileName) + 1, message.fileName);
			strcat_s(temp1, strlen(temp1) + strlen(temp2) + 1, temp2);
			strcat_s(temp1, strlen(temp1) + strlen(message.ID) + 1, message.ID);
			MessageBox(hMain, temp1, "ERROR", MB_ICONERROR);
		}
	}
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
	ShowWindow(ePartnerId, SW_HIDE);
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
	Message message; CreateMessage(&message, 300, 0, 0, 0);
	int ret = SEND_TCP(client, message, 0);
	if (ret == SOCKET_ERROR) return -3;

	ret = RECEIVE_TCP(client, &message, 0);
	if (message.type == 100) SetWindowTextA(eIdDetail, message.ID);
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
	if (isHide) {
		MessageBox(window, "Enter your parner ID", "ANNOUNT", MB_ICONINFORMATION);
		ShowWindow(btnHide, SW_SHOW);
		ShowWindow(ePartnerId, SW_SHOW);
		ShowWindow(sPartnerId, SW_SHOW);
		SetWindowTextA(sPartnerId, "Parner ID");
		isHide = false;
	}
	else {
		TCHAR* pathToFile = new TCHAR[1024];
		TCHAR* partnerID = new TCHAR[1024];
		GetWindowText(eFile, pathToFile, 1024);
		GetWindowText(ePartnerId, partnerID, 1024);
		if (IsFileExistOrValid(pathToFile) && (string)partnerID != "") {
			char* fileName = StringToChars(GetFileName(pathToFile));
			string file_name(fileName);
			string sID(partnerID);
			ForwardInfoSend info;
			strcpy_s(info.ID, strlen(partnerID) + 1, partnerID);
			strcpy_s(info.fileName, strlen(fileName) + 1, fileName);
			strcpy_s(info.pathToFile, strlen(pathToFile) + 1, pathToFile);
			mapForwardSend[sID].insert({ file_name, info });
			Message message; CreateMessage(&message, 400, fileName, partnerID, 0);
			int ret = SEND_TCP(client, message, 0);
			if (ret == SOCKET_ERROR) MessageBox(window, "Can not send to server", "ERROR", MB_ICONERROR);
		}
		else {
			MessageBox(window, "Wrong your path of file or parner ID", "ERROR", MB_ICONERROR);
			SetWindowTextA(eFile, "");
			SetWindowTextA(ePartnerId, "");
		}
	}
}

void OnBnClickedBrowse(HWND window) {
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

void OnBnClickedHide(HWND window) {
	ShowWindow(btnHide, SW_HIDE);
	ShowWindow(ePartnerId, SW_HIDE);
	SetWindowTextA(sPartnerId, "");
	isHide = true;
}

void OnBnClickedSearch(HWND window) {
	TCHAR* fileName = new TCHAR[BUFF_SIZE];
	GetWindowText(eFileName, fileName, BUFF_SIZE);
	string file_name(fileName);
	if (file_name == "") MessageBox(window, "Must be enter file name", "ANNOUNT", MB_ICONINFORMATION);
	else {
		SearchInfo info; strcpy_s(info.fileName, strlen(fileName) + 1, fileName);
		mapSearch.insert({ file_name, info });
		Message message; CreateMessage(&message, 310, fileName, 0, 0);
		int ret = SEND_TCP(client, message, 0);
		if (ret == SOCKET_ERROR) MessageBox(window, "Can not send to server", "ERROR", MB_ICONERROR);
	}
}

void OnBnClickedClean(HWND window) {
	SetWindowTextA(eFile, "");
	SetWindowTextA(eFileName, "");
	SetWindowTextA(ePartnerId, "");
}

void OnLbClickItem(HWND window) {
	int count = SendMessage(listBoxID, LB_GETCOUNT, 0, 0);
	int iSelected = -1;
	for (int i = 0; i < count; i++)
	{
		if (SendMessage(listBoxID, LB_GETSEL, i, 0) > 0)
		{
			iSelected = i; break;
		}
	}
	char ID[30];
	SendMessage(listBoxID, LB_GETTEXT, iSelected, (LPARAM)ID);
	char* temp1 = new char[100]{ "You choose the ID: " };
	char* temp2 = new char[100]{ "\nAre you sure?" };
	strcat_s(temp1, strlen(temp1) + strlen(ID) + 1, ID);
	strcat_s(temp1, strlen(temp1) + strlen(temp2) + 1, temp2);
	int id = MessageBox(window, temp1, "ANNOUNT", MB_ICONINFORMATION | MB_OKCANCEL);
	if (id == IDOK) {
		TCHAR* fileName = new char[BUFF_SIZE];
		GetWindowText(sFileNameSearch, fileName, BUFF_SIZE);
		Message message; CreateMessage(&message, 312, fileName, ID, 0);
		int ret = SEND_TCP(client, message, 0);
		if (ret == SOCKET_ERROR) MessageBox(window, "Can not send to server", "ERROR", MB_OK);
		mapSearch[string(fileName)].data.clear();
		ShowWindow(hSearch, SW_HIDE);
	}
}

//handler hMain event
LRESULT CALLBACK WndProcMain(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	int idEvent = LOWORD(wParam);
	int wmEvent = HIWORD(wParam);
	switch (message)
	{
	case WM_COMMAND:
		if ((HWND)lParam && wmEvent == BN_CLICKED) {
			if (idEvent == buttonForward) OnBnClickedForward(hWnd);
			else if (idEvent == buttonBrowse) OnBnClickedBrowse(hWnd);
			else if (idEvent == buttonHide) OnBnClickedHide(hWnd);
			else if (idEvent == buttonConnect) OnBnClickedConnect(hWnd);
			else if (idEvent == buttonSearch) OnBnClickedSearch(hWnd);
			else if (idEvent == buttonClean) OnBnClickedClean(hWnd);
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
	int idEvent = LOWORD(wParam);
	int wmEvent = HIWORD(wParam);
	switch (message)
	{
	case WM_COMMAND:
		if (wmEvent == LBN_SELCHANGE) {
			if (idEvent == lbID) OnLbClickItem(hWnd);
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
