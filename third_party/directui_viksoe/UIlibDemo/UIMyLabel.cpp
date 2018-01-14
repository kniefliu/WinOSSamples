
#include "StdAfx.h"
#include "UIMyLabel.h"

CMyLabelPanelUI::CMyLabelPanelUI()
	: m_cxWidth(0)
	, m_uTextStyle(DT_VCENTER)
{
}

LPCTSTR CMyLabelPanelUI::GetClass() const
{
	return _T("MyLabelPanelUI");
}

void CMyLabelPanelUI::SetText(LPCTSTR pstrText)
{
	CControlUI::SetText(pstrText);
}

void CMyLabelPanelUI::SetWidth(int cx)
{
	m_cxWidth = cx;
	UpdateLayout();
}

void CMyLabelPanelUI::SetTextStyle(UINT uStyle)
{
	m_uTextStyle = uStyle;
	Invalidate();
}

void CMyLabelPanelUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if (_tcscmp(pstrName, _T("align")) == 0)
	{
		if (_tcsstr(pstrValue, _T("center")) != NULL)
			m_uTextStyle |= DT_CENTER;
		if (_tcsstr(pstrValue, _T("right")) != NULL)
			m_uTextStyle |= DT_RIGHT;
	}
	else if (_tcscmp(pstrName, _T("width")) == 0)
	{
		SetWidth(_ttoi(pstrValue));
	}
	else 
		CControlUI::SetAttribute(pstrName, pstrValue);
}

SIZE CMyLabelPanelUI::EstimateSize(SIZE/* szAvailable*/)
{
	return CSize(m_cxWidth, m_pManager->GetThemeFontInfo(UIFONT_NORMAL).tmHeight + 4);
}

void CMyLabelPanelUI::DoPaint(HDC hDC, const RECT& rcPaint)
{
	RECT rcText = m_rcItem;
	int nLinks = 0;
	CBlueRenderEngineUI::DoPaintPrettyText(hDC, m_pManager, rcText, m_sText, UICOLOR_EDIT_TEXT_NORMAL,
		UICOLOR__INVALID, NULL, nLinks, DT_SINGLELINE | m_uTextStyle);
}
