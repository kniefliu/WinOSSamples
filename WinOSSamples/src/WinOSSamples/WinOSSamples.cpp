// WinOSSamples.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "WinOSSamples.h"

#include "PngDecoder.h"

#define MAX_LOADSTRING 100

// ȫ�ֱ���:
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������

// �˴���ģ���а����ĺ�����ǰ������:
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

 	// TODO: �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_WINOSSAMPLES, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINOSSAMPLES));

	// ����Ϣѭ��:
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
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
//  ע��:
//
//    ����ϣ��
//    �˴�������ӵ� Windows 95 �еġ�RegisterClassEx��
//    ����֮ǰ�� Win32 ϵͳ����ʱ������Ҫ�˺��������÷������ô˺���ʮ����Ҫ��
//    ����Ӧ�ó���Ϳ��Ի�ù�����
//    ����ʽ��ȷ�ġ�Сͼ�ꡣ
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
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��:
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

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
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��: ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
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
		// �����˵�ѡ��:
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
		// TODO: �ڴ���������ͼ����...
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

// �����ڡ������Ϣ�������
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
