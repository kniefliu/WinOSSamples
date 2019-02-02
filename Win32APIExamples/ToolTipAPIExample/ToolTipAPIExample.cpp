#define _WIN32_WINNT 0x0501

#include <Windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <tchar.h>

#define TOOLTIP_MSG_BUF_MAX_LEN 32

HINSTANCE instance_handle_;
HWND tooltip_hwnd_ = NULL;
TOOLINFO tooltip_info_;
TCHAR tooltip_msg_buf_[TOOLTIP_MSG_BUF_MAX_LEN] = { 0 };
RECT tooltip_button_pos_ = { 10, 10, 110, 40 };

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE/* hPreInst*/, PSTR szCmdLine, int iCmdShow)
{
	instance_handle_ = hInst;

	::CoInitialize(0);

	static TCHAR szAppName[] = TEXT("ToolTipAPIExample");
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

	hwnd = CreateWindow(szAppName, TEXT("The ToolTipAPIExample Example"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInst, NULL);

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
	case WM_GETICON:
		break;
	case WM_CREATE: 
	{
		//创建提示窗口
		tooltip_hwnd_ = CreateWindowEx(WS_EX_TOPMOST,
			TOOLTIPS_CLASS,
			NULL,
			WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			hwnd,
			NULL,
			instance_handle_,
			NULL
		);

		//设置提示窗口的信息
		memset(&tooltip_info_, 0, sizeof(TOOLINFO));
		tooltip_info_.cbSize = sizeof(TOOLINFO);
		tooltip_info_.uFlags = TTF_SUBCLASS;
		tooltip_info_.uId = (UINT_PTR)hwnd;
		tooltip_info_.hwnd = hwnd;
		//tti.rect = { 0, 0, 100, 30 };
		tooltip_info_.hinst = instance_handle_;
		tooltip_info_.lpszText = TEXT("hello");

		//新增一个提示
		SendMessage(tooltip_hwnd_, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&tooltip_info_);
	}
		return 0;
	case WM_MOUSEMOVE:
	{
		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);
		if (tooltip_button_pos_.left < xPos && xPos < tooltip_button_pos_.right && tooltip_button_pos_.top < yPos && yPos < tooltip_button_pos_.bottom) {			
			//显示工具提示
			TCHAR tooltip_msg_buf[TOOLTIP_MSG_BUF_MAX_LEN] = { 0 };
			_stprintf(tooltip_msg_buf, TEXT("cursor:(%d, %d)"), xPos, yPos);
			if (_tcscmp(tooltip_msg_buf, tooltip_msg_buf_) != 0) {
				_tcscpy(tooltip_msg_buf_, tooltip_msg_buf);
				tooltip_info_.lpszText = tooltip_msg_buf;
				SendMessage(tooltip_hwnd_, TTM_SETTOOLINFO, (WPARAM)0, (LPARAM)&tooltip_info_);
			}
			SendMessage(tooltip_hwnd_, TTM_TRACKACTIVATE, (WPARAM)TRUE, (LPARAM)&tooltip_info_);
			SendMessage(tooltip_hwnd_, TTM_TRACKPOSITION, 0, (LPARAM)lParam);
		}
		else {
			SendMessage(tooltip_hwnd_, TTM_TRACKACTIVATE, (WPARAM)FALSE, (LPARAM)0);
		}
		return 0;
	}
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		{
			HBRUSH brush = GetStockBrush(GRAY_BRUSH);
			FillRect(hdc, &tooltip_button_pos_, brush);
		}
		EndPaint(hwnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
