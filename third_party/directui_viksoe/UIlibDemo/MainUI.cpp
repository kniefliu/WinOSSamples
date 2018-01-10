#include "StdAfx.h"
#include "MainUI.h"

#include "UICanvas.h"

#include <fstream>

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
		CControlUI * pRoot = builder.Create(GetDialogResource(), this);
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

CControlUI* CMainUI::CreateControl(LPCTSTR pstrClass)
{
	if (_tcscmp(pstrClass, _T("MyWhiteCanvasUI")) == 0) return new CMyWhiteCanvasUI();
	return NULL;
}

CStdString CMainUI::GetDialogResource() const
{
#ifndef DUI_RES_PATH
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
#else
	CStdString strXML;
	std::string xmlFileName = DUI_RES_PATH;
	xmlFileName.append("mainui.xml");
	std::ifstream fin(xmlFileName, std::ifstream::binary);
	if (fin)
	{
		fin.seekg(0, fin.end);
		std::fstream::pos_type length = fin.tellg();
		fin.seekg(0, fin.beg);

		int len = (int)length;

		char *buffer = new char[len+1];
		memset(buffer, 0, len+1);
		fin.read(buffer, len);

		if (fin)
		{
			strXML.Assign(buffer, (int)len);
		}

		fin.close();
		delete[] buffer;
	}
	return strXML;
#endif
}
