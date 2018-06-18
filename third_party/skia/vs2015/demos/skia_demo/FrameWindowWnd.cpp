
#include "FrameWindowWnd.h"

#include "skwin\LyraWindow.h"

#include "SkTypes.h"

#include "SkiaTest.h"



CFrameWindowWnd::CFrameWindowWnd(void)
{
	m_pPaintWnd = nullptr;
}


CFrameWindowWnd::~CFrameWindowWnd(void)
{
}

LPCTSTR CFrameWindowWnd::GetWindowClassName() const 
{ 
	return TEXT("UIMainFrame"); 
};
   
UINT CFrameWindowWnd::GetClassStyle() const 
{ 
	return UI_CLASSSTYLE_FRAME; 
};
   
void CFrameWindowWnd::OnFinalMessage(HWND /*hWnd*/) 
{ 
	delete this; 
};

#define ID_TIMER 100

LRESULT CFrameWindowWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if( uMsg == WM_CREATE ) {
		//SetIcon(IDR_MAINFRAME);
		m_pPaintWnd = new CLyraWindow(this->GetHWND(), NULL);
		if (m_pPaintWnd)
		{
			m_pPaintWnd->Init();
			m_pPaintWnd->setDraw(this);
		}

		//m_pLeftChild = new CLeftChildWindowWndParent();
		//m_pLeftChild->Create(m_hWnd, TEXT("leftchildparent"), UI_WNDSTYLE_CHILD | WS_BORDER, 0);

		//m_pRightChild = new CRightChildWindowWnd();
		//m_pRightChild->Create(m_hWnd, TEXT("rightchild"), UI_WNDSTYLE_CHILD | WS_BORDER, 0);
		SetTimer(m_hWnd, ID_TIMER, 1000, NULL);
	}
	if( uMsg == WM_DESTROY ) {
		::KillTimer(m_hWnd, ID_TIMER);
		::PostQuitMessage(0L);
	}
	if (uMsg == WM_ERASEBKGND) {
		//HDC hdc = (HDC)wParam;
		//RECT rc;
		//GetClientRect(m_hWnd, &rc);
		//SetMapMode(hdc, MM_ANISOTROPIC);
		//SetWindowExtEx(hdc, 100, 100, NULL);
		//SetViewportExtEx(hdc, rc.right, rc.bottom, NULL);
		//FillRect(hdc, &rc, (HBRUSH)GetStockObject(WHITE_BRUSH));

		//MoveToEx(hdc, rc.left, rc.top+10, NULL);
		//LineTo(hdc, rc.right, rc.top+10);

		return 1L;
	}
	if (uMsg == WM_KEYDOWN)
	{
		
	}
	if( uMsg == WM_SETFOCUS ) {
			
	}
	if( uMsg == WM_SIZE ) {
		
		RECT rcClient;
		::GetClientRect(m_hWnd, &rcClient);
		int w = rcClient.right - rcClient.left;
		int h = rcClient.bottom - rcClient.top;
		m_pPaintWnd->resize(w, h);

		//int padding = 40;
		//int space = 40;

		//int wChild = (w - 2 * padding - space) / 2;
		//int hChild = (h - 2 * padding);

		//int xLeftChild = padding;
		//int yLeftChild = padding;
		//int xRightChild = padding + space + wChild;
		//int yRightChild = padding;
		//
		//MoveWindow(m_pLeftChild->GetHWND(), xLeftChild, yLeftChild, wChild, hChild, TRUE);
		//MoveWindow(m_pRightChild->GetHWND(), xRightChild, yRightChild, wChild, hChild, TRUE);

		return 0;
	}
	if (uMsg == WM_TIMER) {
		::InvalidateRect(m_hWnd, NULL, TRUE);
		return 0;
	}
	if (uMsg == WM_PAINT) {
		{
			m_pPaintWnd->forceInvalAll();
			PAINTSTRUCT ps = { 0 };
			HDC hdc = ::BeginPaint(m_hWnd, &ps);
			m_pPaintWnd->doPaint(hdc);
			::EndPaint(m_hWnd, &ps);

		}
		return 1;
	}

	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

void CFrameWindowWnd::drawContent(SkCanvas * canvas) {
	SkColor color = SK_ColorBLUE;
	canvas->clear(color);
}


