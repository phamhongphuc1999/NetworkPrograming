#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include "CONST.h"

HINSTANCE hInst;
static HWND btnBrowse, btnForward, btnSearch, btnConnect, btnHide;
static HWND eFile, eFileName, eParnerId;
static HWND sFile, sFileName, sParnerId, sId, SIdDetail;

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
	HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		700, 400, NULL, NULL, hInstance, NULL);

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
	btnConnect = CreateWindow("button", "Connect", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 315, 0, 70, 30, hWnd, (HMENU)bConnect, hInst, NULL);
	btnBrowse = CreateWindow("button", "Browse", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 315, 100, 70, 30, hWnd, (HMENU)bBrowse, hInst, NULL);
	btnForward = CreateWindow("button", "Forward", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 315, 100, 70, 30, hWnd, (HMENU)bForward, hInst, NULL);
	btnHide = CreateWindow("button", "Hide", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 315, 100, 70, 30, hWnd, (HMENU)bHide, hInst, NULL);
	btnSearch = CreateWindow("button", "Search", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 315, 100, 70, 30, hWnd, (HMENU)bSearch, hInst, NULL);

	eFile = CreateWindow("Edit", "", WS_CHILD | WS_VISIBLE | WS_BORDER, 100, 20, 140, 20, hWnd, NULL, NULL, NULL);
	eFileName = CreateWindow("Edit", "", WS_CHILD | WS_VISIBLE | WS_BORDER, 100, 50, 140, 20, hWnd, NULL, NULL, NULL);

	sFile = CreateWindow("STATIC", "File", WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 20, 20, 120, 40, hWnd, (HMENU)(staticFile), GetModuleHandle(NULL), NULL);
	sFileName = CreateWindow("STATIC", "File Name", WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 20, 20, 120, 60, hWnd, (HMENU)(staticFileName), GetModuleHandle(NULL), NULL);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
	HWND hwndButton;
	PAINTSTRUCT ps;
	HDC hdc;
	switch (message)
	{
	case WM_COMMAND:
		break;
	case WM_CREATE:
		InitializeController(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_DRAWITEM:
		LPDRAWITEMSTRUCT pDIS;
		pDIS = (LPDRAWITEMSTRUCT)lParam;
		RECT rc;
		if (pDIS->hwndItem == sFile) {
			SetTextColor(pDIS->hDC, RGB(200, 10, 60));
			SelectObject(pDIS->hDC, (HPEN)GetStockObject(NULL_PEN));
			SelectObject(pDIS->hDC, (HBRUSH)GetStockObject(NULL_BRUSH));
			SetBkMode(pDIS->hDC, TRANSPARENT);
			Rectangle(pDIS->hDC, 0, 0, pDIS->rcItem.right + 1, pDIS->rcItem.bottom + 1);
			DrawText(pDIS->hDC, "File", 10, &pDIS->rcItem, 0);
			return 0;
		}
		else if (pDIS->hwndItem == sFileName) {
			SetTextColor(pDIS->hDC, RGB(200, 10, 60));
			SelectObject(pDIS->hDC, (HPEN)GetStockObject(NULL_PEN));
			SelectObject(pDIS->hDC, (HBRUSH)GetStockObject(NULL_BRUSH));
			SetBkMode(pDIS->hDC, TRANSPARENT);
			Rectangle(pDIS->hDC, 20, 50, pDIS->rcItem.right + 20, pDIS->rcItem.bottom + 30);
			DrawText(pDIS->hDC, "File Name", 20, &pDIS->rcItem, 40);
			return 0;
		}

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
}
