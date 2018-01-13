#ifndef __MAINUI_H__
#define __MAINUI_H__

class CMainUI : public CWindowWnd, public INotifyUI, public IDialogBuilderCallback
{
public:
	CMainUI();

	virtual LPCTSTR GetWindowClassName() const override;
	virtual UINT GetClassStyle() const override;
	virtual void OnFinalMessage(HWND hWnd) override;
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

	void Init();
	void OnPrepareAnimation();

	virtual void Notify(TNotifyUI& msg) override;

	virtual CControlUI* CreateControl(LPCTSTR pstrClass) override;

	CStdString GetDialogResource() const;

public:
	CPaintManagerUI m_pm;
};

#endif//__MAINUI_H__
