#include "StdAfx.h"
#include "MainUI.h"

CMainUI::CMainUI()
{
}

LPCTSTR CMainUI::GetWindowClassName() const
{
	return _T("MainUI");
}

UINT CMainUI::GetClassStyle() const
{
	return UI_CLASSSTYLE_CHILD;
}

void CMainUI::OnFinalMessage(HWND/* hWnd*/)
{
	delete this;
}

LRESULT CMainUI::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_CREATE)
	{
		m_pm.Init(m_hWnd);
		CDialogBuilder builder;
		CControlUI * pRoot = builder.Create(GetDialogResource());
		ASSERT(pRoot && "Failed to parse XML");
		m_pm.AttachDialog(pRoot);
		m_pm.AddNotifier(this);
		Init();
		return 0;
	}
	LRESULT lRes = 0;
	if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes))
		return lRes;

	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

void CMainUI::Init()
{
}

void CMainUI::OnPrepareAnimation()
{
}

void CMainUI::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("windowinit"))
		OnPrepareAnimation();
}

LPCTSTR CMainUI::GetDialogResource() const
{
	return "<Dialog>"
      "<HorizontalLayout>"
        "<VerticalLayout width=\"150\" >"
          "<NavigatorPanel>"
            "<PaddingPanel height=\"18\" />"
            "<NavigatorButton name=\"page_start\" text=\"<i 0> Start\" selected=\"true\" tooltip=\"Vis start siden\" />"
            "<NavigatorButton name=\"page_registers\" text=\"<i 4> Registre\" tooltip=\"Vis forskellige registre\" />"
            "<NavigatorButton name=\"page_systems\" text=\"<i 4> Systemer\" />"
            "<NavigatorButton name=\"page_configure\" text=\"<i 4> Options\" />"
            "<NavigatorButton name=\"page_reports\" text=\"<i 4> Rapporter\" />"
          "</NavigatorPanel>"
        "</VerticalLayout>"
      "</HorizontalLayout>"
      "</Dialog>";     
}
