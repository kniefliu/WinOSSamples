#include "StdAfx.h"
#include "UIMyDlgBuilder.h"


CControlUI* CMyDialogBuilder::Create(LPCTSTR pstrXML, IDialogBuilderCallback* pCallback/* = NULL*/)
{
	m_pCallback = pCallback;
	if (!m_xml.Load(pstrXML))
		return NULL;
	CMyMarkupNode root = m_xml.GetRoot();
	return _Parse(&root);
}

CControlUI* CMyDialogBuilder::CreateFromResource(UINT nRes, IDialogBuilderCallback* pCallback/* = NULL*/)
{
	HRSRC hResource = ::FindResource(CPaintManagerUI::GetResourceInstance(), MAKEINTRESOURCE(nRes), _T("XML"));
	if (hResource == NULL)
		return NULL;
	HGLOBAL hGlobal = ::LoadResource(CPaintManagerUI::GetResourceInstance(), hResource);
	if (hGlobal == NULL)
	{
		::FreeResource(hGlobal);
		return NULL;
	}
	CStdString sXML;
	sXML.Assign(static_cast<LPCSTR>(::LockResource(hGlobal)), ::SizeofResource(CPaintManagerUI::GetResourceInstance(), hResource));
	sXML.Replace(_T("\\n"), _T("\n"));
	::FreeResource(hGlobal);
	return Create(sXML, pCallback);
}

CControlUI* CMyDialogBuilder::_Parse(CMyMarkupNode* pRoot, CControlUI* pParent)
{
	CDialogLayoutUI* pStretched = NULL;
	IContainerUI* pContainer = NULL;
	CControlUI* pReturn = NULL;
	for (CMyMarkupNode node = pRoot->GetChild(); node.IsValid(); node = node.GetSibling())
	{
		LPCTSTR pstrClass = node.GetName();
		SIZE_T cchLen = _tcslen(pstrClass);
		CControlUI* pControl = NULL;
		switch (cchLen)
		{
		case 4:
			//if (_tcscmp(pstrClass, _T("List")) == 0)
			//	pControl = new CListUI();
			break;
		case 10:
			if (_tcscmp(pstrClass, _T("LabelPanel")) == 0)
				pControl = new CLabelPanelUI;
			break;
		case 14:
			if (_tcscmp(pstrClass, _T("VerticalLayout")) == 0)
				pControl = new CVerticalLayoutUI;
			break;
		case 16:
			if (_tcscmp(pstrClass, _T("HorizontalLayout")) == 0)
				pControl = new CVerticalLayoutUI;
			break;
		}

		if (pControl == NULL && m_pCallback != NULL)
		{
			pControl = m_pCallback->CreateControl(pstrClass);
		}
		ASSERT(pControl);
		if (pControl == NULL)
			return NULL;

		if (node.HasChildren())
		{
			_Parse(&node, pControl);
		}

		if (pParent != NULL)
		{
			if (pContainer == NULL)
				pContainer = static_cast<IContainerUI*>(pParent->GetInterface(_T("Container")));
			ASSERT(pContainer);
			if (pContainer == NULL)
				return NULL;
			pContainer->Add(pControl);
		}

		if (node.HasAttibutes())
		{
			TCHAR szValue[500] = {0};
			SIZE_T cchLen = lengthof(szValue) - 1;

			int nAttributes = node.GetAttibuteCount();
			for (int i = 0; i < nAttributes; i++)
			{
				pControl->SetAttribute(node.GetAttributeName(i), node.GetAttributeValue(i));
			}
		}

		if (pReturn == NULL)
			pReturn = pControl;
	}
	return pReturn;
}
