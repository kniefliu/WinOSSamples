#ifndef __MY_LABEL_H__
#define __MY_LABEL_H__

class CMyLabelPanelUI : public CControlUI
{
public:
	CMyLabelPanelUI();

	virtual LPCTSTR GetClass() const override;

	virtual void SetText(LPCTSTR pstrText) override;

	void SetWidth(int cxWidth);
	void SetTextStyle(UINT uStyle);

	virtual SIZE EstimateSize(SIZE szAvailable) override;
	virtual void DoPaint(HDC hDC, const RECT& rcPaint) override;
	virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;

protected:
	int m_cxWidth;
	UINT m_uTextStyle;
};

#endif//__MY_LABEL_H__
