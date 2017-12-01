// WinOSSamples.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "WinOSSamples.h"

#include "PngDecoder.h"

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: 在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_WINOSSAMPLES, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINOSSAMPLES));

	// 主消息循环:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
//  注释:
//
//    仅当希望
//    此代码与添加到 Windows 95 中的“RegisterClassEx”
//    函数之前的 Win32 系统兼容时，才需要此函数及其用法。调用此函数十分重要，
//    这样应用程序就可以获得关联的
//    “格式正确的”小图标。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINOSSAMPLES));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_WINOSSAMPLES);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 将实例句柄存储在全局变量中

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

static HBITMAP s_bmp_png = NULL;
static int s_bmp_width = 0;
static int s_bmp_height = 0;

static HWND s_alpha_wnd;

static TCHAR alphaWndClsName[MAX_PATH] = _T("alphaclswnd");

LRESULT CALLBACK AlphaWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if(uMsg == WM_CREATE)
	{
		return 1L;
	}
	return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}
bool RegisterAlphaWindowClass(HINSTANCE hInst)
{
	WNDCLASS wc = { 0 };
	wc.style = CS_DBLCLKS | CS_VREDRAW | CS_HREDRAW;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.lpfnWndProc = AlphaWndProc;
	wc.hInstance =  hInst;
	wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = alphaWndClsName;
	ATOM ret = ::RegisterClass(&wc);
	return ret != NULL || ::GetLastError() == ERROR_CLASS_ALREADY_EXISTS;
}
//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
#define ID_TIMER 100
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_CREATE:
		{
			//char * data = 0;
			//readPng("pic2.png", s_bmp_width, s_bmp_height, &data);
			char * data = 0;
			s_bmp_height = 128;
			s_bmp_width = 128;
			long file_size = s_bmp_width * s_bmp_height * 4;
			int block_size = 0;
			block_size = 4 ;//: 3;
			data = (char *)malloc(file_size);
			for (int y = 0; y < s_bmp_height; y++)
			{
				for (int x = 0; x < s_bmp_width; x ++)
				{
					data[y * (s_bmp_width * block_size) + x * block_size + 0] = 0x00; // B
					data[y * (s_bmp_width * block_size) + x * block_size + 1] = 0x00; // G
					data[y * (s_bmp_width * block_size) + x * block_size + 2] = 0xFF; // R
					data[y * (s_bmp_width * block_size) + x * block_size + 3] = 0xFF; // A
				}
			}

			BITMAPINFOHEADER hdr = { 0 };
			hdr.biSize = sizeof(BITMAPINFOHEADER);
			hdr.biWidth = s_bmp_width;
			hdr.biHeight = -s_bmp_height;  // minus means top-down bitmap
			hdr.biPlanes = 1;
			hdr.biBitCount = 32;
			hdr.biCompression = BI_RGB;  // no compression
			hdr.biSizeImage = 0;
			hdr.biXPelsPerMeter = 1;
			hdr.biYPelsPerMeter = 1;
			hdr.biClrUsed = 0;
			hdr.biClrImportant = 0;
			
			void* pData;
			s_bmp_png = CreateDIBSection(NULL, reinterpret_cast<BITMAPINFO*>(&hdr),
			0, &pData, NULL, 0);

			memcpy(pData, data, 4 * s_bmp_width * s_bmp_height);

			free(data);
			if (RegisterAlphaWindowClass(hInst))
			{
				s_alpha_wnd = ::CreateWindowEx(WS_EX_TOOLWINDOW | WS_EX_LAYERED, alphaWndClsName, _T("alphawndH"), WS_POPUPWINDOW & (~WS_VISIBLE)/*UI_WNDSTYLE_FRAME*/, 100, 100, s_bmp_width, s_bmp_height, NULL, NULL, hInst, 0);
				if (s_alpha_wnd)
				{
					OutputDebugStringA("create alpha wnd success\n");
				}
			}

			SetTimer(hWnd, ID_TIMER, 1000, NULL);
		}
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		case IDM_ABOUT:
			//DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			{
				
			}
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_TIMER:
		{
			::ShowWindow(s_alpha_wnd, SW_SHOW);

			RECT rc;

			GetClientRect(hWnd, &rc);
			POINT leftTop, rightBottom;
			leftTop.x = rc.left;
			leftTop.y = rc.top;
			rightBottom.x = rc.right;
			rightBottom.y = rc.bottom;
			::ClientToScreen(hWnd, &leftTop);
			::ClientToScreen(hWnd, &rightBottom);
			rc.left = leftTop.x;
			rc.top = leftTop.y;
			rc.right = rightBottom.x;
			rc.bottom = rightBottom.y;

			static int left = 100;
			static int top = 100;

			static int width = 20;
			static int height = 20;

			//left = 100;
			//top = 100;

			if (left > rc.right - rc.left - width)
				left = 100;
			if (top > rc.bottom - rc.top - height)
				top = 100;

			if (width > 200)
				width = 20;
			if (height > 200)
				height = 20;

			// do paint alpha window
			{
				HDC hDesktopDc = GetDC(NULL);
				POINT ptDest;
				
				ptDest.x = rc.left + left - width / 2;
				ptDest.y = rc.top + top - height / 2;
				SIZE sz;
				sz.cx = width;
				sz.cy = height;
				HDC hMemDC = ::CreateCompatibleDC(GetDC(s_alpha_wnd)); 
				::SelectObject(hMemDC, s_bmp_png);
				POINT ptSrc;
				ptSrc.x = 0;
				ptSrc.y = 0;
				BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

				HDC hStrechMemDC = ::CreateCompatibleDC(GetDC(s_alpha_wnd));
				HBITMAP hStrechBMP = ::CreateCompatibleBitmap((GetDC(s_alpha_wnd)), width, height);
				::SelectObject(hStrechMemDC, hStrechBMP);
				
				::StretchBlt(hStrechMemDC, 0, 0, width, height, hMemDC, 0, 0, s_bmp_width, s_bmp_height, SRCCOPY);
				
				::UpdateLayeredWindow(s_alpha_wnd, hDesktopDc, &ptDest, &sz, hStrechMemDC, &ptSrc, 0, &bf, ULW_ALPHA);

				::DeleteDC(hStrechMemDC);
				::DeleteObject(hStrechBMP);

				::DeleteDC(hMemDC);
			}
			left += 10;
			top += 10;
			width += 10;
			height += 10;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此添加任意绘图代码...
		{
#if 0
			HDC hMemDC = ::CreateCompatibleDC(hdc);
			::SelectObject(hMemDC, s_bmp_png);
			//::BitBlt(hdc, 100, 100, s_bmp_width, s_bmp_height, hMemDC, 0, 0, SRCCOPY);
			BLENDFUNCTION bf = {0};
			bf.BlendOp = AC_SRC_OVER;
			bf.BlendFlags = 0;
			bf.AlphaFormat = AC_SRC_ALPHA;
			bf.SourceConstantAlpha = 255;
			::AlphaBlend(hdc, 100, 100, s_bmp_width, s_bmp_height, hMemDC, 0, 0, s_bmp_width, s_bmp_height, bf);
			::DeleteDC(hMemDC);
			InvalidateRect(s_alpha_wnd, NULL, TRUE);
#endif
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		::KillTimer(hWnd, ID_TIMER);
		if (s_alpha_wnd)
			DestroyWindow(s_alpha_wnd);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
