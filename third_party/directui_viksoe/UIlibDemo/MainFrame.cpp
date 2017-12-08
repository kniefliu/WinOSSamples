#include "StdAfx.h"
#include "MainFrame.h"

#include "resource.h"

#include "MainUI.h"

CMainFrame::CMainFrame()
{
}

LPCTSTR CMainFrame::GetWindowClassName() const
{
	return _T("MainFrame");
}
UINT CMainFrame::GetClassStyle() const
{
	return UI_CLASSSTYLE_FRAME;
}
void CMainFrame::OnFinalMessage(HWND/* hWnd*/)
{
	delete this;
}

void CMainFrame::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click") || msg.sType == _T("link"))
	{
	}
	if (msg.sType == _T("itemactivate"))
	{
	}
}

LRESULT CMainFrame::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_CREATE)
	{
		CMainUI * pMainUI = new CMainUI();
		pMainUI->m_pm.AddNotifier(this);
		m_hWndClient = pMainUI->Create(m_hWnd, NULL, UI_WNDSTYLE_CHILD, 0);
		SetIcon(IDR_MAINFRAME);
	}
	if (uMsg == WM_DESTROY)
	{
		::PostQuitMessage(0);
	}
	if (uMsg == WM_SETFOCUS)
	{
		::SetFocus(m_hWndClient);
	}
	if (uMsg == WM_SIZE)
	{
		RECT rcClient;
		::GetClientRect(m_hWnd, &rcClient);
		::MoveWindow(m_hWndClient, rcClient.left, rcClient.top, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, TRUE);
		return 0;
	}
	if (uMsg == WM_ERASEBKGND)
	{
		return 1;
	}
	if (uMsg == WM_PAINT)
	{
		PAINTSTRUCT ps = {0};
		::BeginPaint(m_hWnd, &ps);
		::EndPaint(m_hWnd, &ps);
		return 0;
	}
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}
