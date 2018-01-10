#ifndef __UICANVAS_H__
#define __UICANVAS_H__

class CMyCanvasUI : public CControlUI, public IContainerUI
{
public:
	CMyCanvasUI();
	virtual ~CMyCanvasUI();

	virtual LPCTSTR GetClass() const override;
	virtual LPVOID GetInterface(LPCTSTR pstrName) override;

	virtual CControlUI* GetItem(int iIndex) const override;
	virtual int GetCount() const override;
	virtual bool Add(CControlUI* pControl) override;
	virtual bool Remove(CControlUI* pControl) override;
	virtual void RemoveAll() override;

	virtual void Event(TEventUI& event) override;
	virtual void SetVisible(bool bVisible) override;

	virtual void SetPadding(int iPadding);
	virtual void SetWidth(int cx);
	virtual void SetHeight(int cy);
	virtual void SetAutoDestroy(bool bAuto);

	void SetPos(RECT rc);
	SIZE EstimateSize(SIZE szAvailable);
	void DoPaint(HDC hDC, const RECT& rcPaint);

	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	void SetManager(CPaintManagerUI* pManager, CControlUI* pParent);
	CControlUI* FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags);

protected:
	CStdPtrArray m_items;
	int m_iPadding;
	SIZE m_cxyFixed;
	bool m_bAutoDestroy;

protected:
	COLORREF m_clrBack;
};

class CMyWhiteCanvasUI : public CMyCanvasUI
{
public:
	CMyWhiteCanvasUI();
	LPCTSTR GetClass() const;
};

#endif//__UICANVAS_H__
