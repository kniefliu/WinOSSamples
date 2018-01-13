
#include "StdAfx.h"
#include "UICanvas.h"


/////////////////////////////////////////////////////////////////////////////////////
//
//

CMyCanvasUI::CMyCanvasUI()
	: m_iPadding(0)
	, m_bAutoDestroy(true)
{
	m_cxyFixed.cx = m_cxyFixed.cy = 0;
}
CMyCanvasUI::~CMyCanvasUI()
{
	RemoveAll();
}

LPCTSTR CMyCanvasUI::GetClass() const
{
	return _T("MyCanvasUI");
}
LPVOID CMyCanvasUI::GetInterface(LPCTSTR pstrName)
{
	if (_tcscmp(pstrName, _T("Container")) == 0) return static_cast<IContainerUI*>(this);
	return CControlUI::GetInterface(pstrName);
}

CControlUI* CMyCanvasUI::GetItem(int iIndex) const
{
	if (iIndex == 0)
	{
		return static_cast<CControlUI*>(m_items.GetAt(iIndex));
	}
	return NULL;
}
int CMyCanvasUI::GetCount() const
{
	return m_items.GetSize();
}
bool CMyCanvasUI::Add(CControlUI* pControl)
{
	if( m_pManager != NULL ) m_pManager->InitControls(pControl, this);
	if( m_pManager != NULL ) m_pManager->UpdateLayout();
	return m_items.Add(pControl);
}
bool CMyCanvasUI::Remove(CControlUI* pControl)
{
	for (int it = 0; m_bAutoDestroy && it < m_items.GetSize(); it++)
	{
		if (static_cast<CControlUI*>(m_items[it]) == pControl)
		{
			if (m_pManager != NULL) m_pManager->UpdateLayout();
			delete pControl;
			return m_items.Remove(it);
		}
	}
	return false;
}
void CMyCanvasUI::RemoveAll()
{
	for (int it = 0; m_bAutoDestroy && it < m_items.GetSize(); it++)
	{
		delete static_cast<CControlUI*>(m_items[it]);
	}
	m_items.Empty();
	if (m_pManager != NULL) m_pManager->UpdateLayout();
}

void CMyCanvasUI::SetPadding(int iPadding)
{
	m_iPadding = iPadding;
}
void CMyCanvasUI::SetWidth(int cx)
{
	m_cxyFixed.cx = cx;
}
void CMyCanvasUI::SetHeight(int cy)
{
	m_cxyFixed.cy = cy;
}
void CMyCanvasUI::SetAutoDestroy(bool bAuto)
{
	m_bAutoDestroy = bAuto;
}

void CMyCanvasUI::Event(TEventUI& event)
{
	CControlUI::Event(event);
}
void CMyCanvasUI::SetVisible(bool bVisible) 
{
	for (int it = 0; m_bAutoDestroy && it < m_items.GetSize(); it++)
	{
		static_cast<CControlUI*>(m_items[it])->SetVisible(bVisible);
	}

	CControlUI::SetVisible();
}

void CMyCanvasUI::SetPos(RECT rc)
{
	CControlUI::SetPos(rc);
	if (m_items.IsEmpty()) return;
	
	ASSERT(m_items.GetSize() == 1);
	static_cast<CControlUI*>(m_items[0])->SetPos(rc);
}
SIZE CMyCanvasUI::EstimateSize(SIZE szAvailable)
{
	return m_cxyFixed;
}
void CMyCanvasUI::DoPaint(HDC hDC, const RECT& rcPaint)
{
	// Fill background
	RECT rcFill = { 0 };
	if( ::IntersectRect(&rcFill, &rcPaint, &m_rcItem) ) {
		CBlueRenderEngineUI::DoFillRect(hDC, m_pManager, rcFill, m_clrBack);
	}

	RECT rcTemp = { 0 };
	if( !::IntersectRect(&rcTemp, &rcPaint, &m_rcItem) ) return;

	CRenderClip clip;
	CBlueRenderEngineUI::GenerateClip(hDC, m_rcItem, clip);

	for( int it = 0; it < m_items.GetSize(); it++ ) {
		CControlUI* pControl = static_cast<CControlUI*>(m_items[it]);
		if( !pControl->IsVisible() ) continue;
		if( !::IntersectRect(&rcTemp, &rcPaint, &pControl->GetPos()) ) continue;
		if( !::IntersectRect(&rcTemp, &m_rcItem, &pControl->GetPos()) ) continue;
		pControl->DoPaint(hDC, rcPaint);
	}
}

void CMyCanvasUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if( _tcscmp(pstrName, _T("padding")) == 0 ) SetPadding(_ttoi(pstrValue));
	else if( _tcscmp(pstrName, _T("width")) == 0 ) SetWidth(_ttoi(pstrValue));
	else if( _tcscmp(pstrName, _T("height")) == 0 ) SetHeight(_ttoi(pstrValue));
	else CControlUI::SetAttribute(pstrName, pstrValue);
}

void CMyCanvasUI::SetManager(CPaintManagerUI* pManager, CControlUI* pParent)
{
	for (int it = 0; m_bAutoDestroy && it < m_items.GetSize(); it++)
	{
		static_cast<CControlUI*>(m_items[it])->SetManager(pManager, this);
	}
	CControlUI::SetManager(pManager, pParent);
}
CControlUI* CMyCanvasUI::FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags)
{
	// Check if this guy is valid
	if( (uFlags & UIFIND_VISIBLE) != 0 && !IsVisible() ) return NULL;
	if( (uFlags & UIFIND_ENABLED) != 0 && !IsEnabled() ) return NULL;
	if( (uFlags & UIFIND_HITTEST) != 0 && !::PtInRect(&m_rcItem, *(static_cast<LPPOINT>(pData))) ) return NULL;
	if( (uFlags & UIFIND_ME_FIRST) != 0 ) {
		CControlUI* pControl = CControlUI::FindControl(Proc, pData, uFlags);
		if( pControl != NULL ) return pControl;
	}
	for( int it = 0; it != m_items.GetSize(); it++ ) {
		CControlUI* pControl = static_cast<CControlUI*>(m_items[it])->FindControl(Proc, pData, uFlags);
		if( pControl != NULL ) return pControl;
	}
	return CControlUI::FindControl(Proc, pData, uFlags);
}



CMyWhiteCanvasUI::CMyWhiteCanvasUI()
{
	m_clrBack = m_pManager->GetThemeColor(UICOLOR_STANDARD_WHITE);
}
LPCTSTR CMyWhiteCanvasUI::GetClass() const
{
	return _T("MyWhiteCanvasUI");
}

