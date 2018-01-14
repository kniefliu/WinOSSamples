#ifndef __MY_DLG_BUILDER_H__
#define __MY_DLG_BUILDER_H__

#include "UIMyMarkup.h"

class CMyDialogBuilder
{
public:
	CControlUI* Create(LPCTSTR pstrXML, IDialogBuilderCallback* pCallback = NULL);
	CControlUI* CreateFromResource(UINT nRes, IDialogBuilderCallback* pCallback = NULL);

private:
	CControlUI* _Parse(CMyMarkupNode* parent, CControlUI* pParent = NULL);

	CMyMarkup m_xml;
	IDialogBuilderCallback* m_pCallback;
};

#endif//__MY_DLG_BUILDER_H__
