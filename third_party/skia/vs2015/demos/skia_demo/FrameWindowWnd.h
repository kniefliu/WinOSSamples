#ifndef __FRAMEWINDOWWND_H__
#define __FRAMEWINDOWWND_H__

#include <Windows.h>
#include "WindowWnd.h"

#include "LeftChildWindowWndParent.h"
#include "RightChildWindowWnd.h"

#include "SkCanvas.h"

class CLyraWindow;
class CFrameWindowWnd : public CWindowWnd, public IDraw
{
public:
	CFrameWindowWnd(void);
	~CFrameWindowWnd(void);

	LPCTSTR GetWindowClassName() const;

	UINT GetClassStyle() const;

	void OnFinalMessage(HWND /*hWnd*/);

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void drawContent(SkCanvas * canvas);

protected:
	CLeftChildWindowWndParent * m_pLeftChild;
	CRightChildWindowWnd * m_pRightChild;
	CLyraWindow * m_pPaintWnd;
};

#endif//__FRAMEWINDOWWND_H__
