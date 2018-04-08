#include <Windows.h>
#include <Shlwapi.h>
#include <tchar.h>

#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "Shlwapi.lib")

#include "TaskbarIconHelper.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

#define ID_TIMER 100
#define ID_TIMER_START 101

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE/* hPreInst*/, PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("MSAAAPIExample");
	HWND hwnd;
	MSG msg;
	WNDCLASS wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInst;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;

	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("This program requires Windows NT!"), szAppName, MB_ICONERROR);
		return 0;
	}

	hwnd = CreateWindow(szAppName, TEXT("The MSAAAPIExample Example"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInst, NULL);

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	switch (message)
	{
	case WM_TIMER:
		if (wParam == ID_TIMER)
		{
			//OutputDebugStringA("GetIconRect\n");
			TaskbarIconHelper::GetIconRect(NULL);
		}
		else if (wParam == ID_TIMER_START)
		{
			//OutputDebugStringA("Start---:\n");
			::KillTimer(hwnd, ID_TIMER_START);
			SetTimer(hwnd, ID_TIMER, 100, 0);
		}
		return 0;
	case WM_CREATE:
		SetTimer(hwnd, ID_TIMER_START, 5000, 0);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		return 0;
	case WM_DESTROY:
		KillTimer(hwnd, ID_TIMER);
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}
