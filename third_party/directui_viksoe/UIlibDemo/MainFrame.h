#ifndef __MAINFRAME_H__
#define __MAINFRAME_H__

class CMainFrame : public CWindowWnd, public INotifyUI
{
public:
	CMainFrame();

	virtual LPCTSTR GetWindowClassName() const override;
	virtual UINT GetClassStyle() const override;
	virtual void OnFinalMessage(HWND hWnd) override;

	virtual void Notify(TNotifyUI& msg) override;

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

protected:
	HWND m_hWndClient;
};

#endif//__MAINFRAME_H__
