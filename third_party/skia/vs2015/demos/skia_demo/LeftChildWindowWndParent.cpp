#include "LeftChildWindowWndParent.h"

#include "skwin\LyraWindow.h"
#include "SkCanvas.h"

CLeftChildWindowWndParent::CLeftChildWindowWndParent(void)
{
}


CLeftChildWindowWndParent::~CLeftChildWindowWndParent(void)
{
}

LPCTSTR CLeftChildWindowWndParent::GetWindowClassName() const
{
	return TEXT("UILeftChildWnd");
};

UINT CLeftChildWindowWndParent::GetClassStyle() const
{
	return UI_CLASSSTYLE_CHILD;
};

void CLeftChildWindowWndParent::OnFinalMessage(HWND /*hWnd*/)
{
	m_pPaintWnd->UnInit();
	delete this;
};

LRESULT CLeftChildWindowWndParent::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_CREATE) {
		if (uMsg == WM_CREATE) {
			//SetIcon(IDR_MAINFRAME);
			child_wnd_ = new CLeftChildWindowWnd();
			child_wnd_->Create(m_hWnd, TEXT("leftchild"), UI_WNDSTYLE_CHILD | WS_BORDER, 0);
		}

		m_pPaintWnd = new CLyraWindow(this->GetHWND(), NULL);
		if (m_pPaintWnd)
		{
			m_pPaintWnd->Init();
			m_pPaintWnd->setDraw(this);
		}
	}
	if (uMsg == WM_DESTROY) {
		::PostQuitMessage(0L);
	}
	if (uMsg == WM_TIMER)
	{
		m_pPaintWnd->forceInvalAll();
		InvalidateRect(m_hWnd, NULL, TRUE);
		return 0;
	}
	// Or when the window is clicked.
	if (uMsg == WM_LBUTTONDOWN) {
		SetFocus(m_hWnd);
		// Msg was handled, return zero.
		return 0;
	}
	if (uMsg == WM_SETFOCUS)
	{
		OutputDebugStringA("child: setfocus\n");
	}
	if (uMsg == WM_SIZE) {
		RECT rcClient;
		::GetClientRect(m_hWnd, &rcClient);
		int w = rcClient.right - rcClient.left;
		int h = rcClient.bottom - rcClient.top;
		m_pPaintWnd->resize(w, h);

		{
			RECT rcClient;
			::GetClientRect(m_hWnd, &rcClient);
			int w = rcClient.right - rcClient.left;
			int h = rcClient.bottom - rcClient.top;

			int padding = 40;
			int space = 40;

			int wChild = (w - 2 * padding - space) / 2;
			int hChild = (h - 2 * padding);

			int xLeftChild = padding;
			int yLeftChild = padding;
			int xRightChild = padding + space + wChild;
			int yRightChild = padding;

			MoveWindow(child_wnd_->GetHWND(), xLeftChild, yLeftChild, wChild, hChild, TRUE);
		}

		return 0;
	}
	if (uMsg == WM_ERASEBKGND) {
		return 1;
	}
	if (uMsg == WM_PAINT) {
		{
			PAINTSTRUCT ps = { 0 };
			HDC hdc = ::BeginPaint(m_hWnd, &ps);
			m_pPaintWnd->doPaint(hdc);
			::EndPaint(m_hWnd, &ps);

		}
		return 1;
	}
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

void CLeftChildWindowWndParent::drawContent(SkCanvas * canvas) {
	SkColor color = SK_ColorBLUE;
	canvas->clear(color);
}
