#ifndef __LEFTCHILDWINDOWWNDPARENT_H__
#define __LEFTCHILDWINDOWWNDPARENT_H__

#include "WindowWnd.h"
#include "LeftChildWindowWnd.h"

#include "skwin\LyraWindow.h"

class CLeftChildWindowWndParent : public CWindowWnd, public IDraw
{
public:
	CLeftChildWindowWndParent(void);
	~CLeftChildWindowWndParent(void);

	LPCTSTR GetWindowClassName() const;

	UINT GetClassStyle() const;

	void OnFinalMessage(HWND /*hWnd*/);

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void drawContent(SkCanvas * canvas) override;


protected:
	CLyraWindow * m_pPaintWnd;
	CLeftChildWindowWnd * child_wnd_;
};
#endif//__LEFTCHILDWINDOWWNDPARENT_H__
