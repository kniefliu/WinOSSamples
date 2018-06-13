#ifndef __LYRAWINDOW_H__
#define __LYRAWINDOW_H__

#include "SkWindow.h"
#include "DeviceManager.h"

class IDraw
{
public:
	virtual void drawContent(SkCanvas * canvas) = 0;
};

class CLyraWindow : public SkOSWindow
{
public:
	CLyraWindow(void* hwnd, DeviceManager*);
	virtual ~CLyraWindow(void);

	virtual void Init();
	virtual void UnInit();

	DeviceType getDeviceType() const { return fDeviceType; }
	void setDeviceType(DeviceType type);

	sk_sp<SkSurface> makeSurface();

	virtual void draw(SkCanvas* canvas);

	GrContext* getGrContext() const { return fDevManager->getGrContext(); }

	void setDraw(IDraw *draw_ptr) {
		draw_ = draw_ptr;
	}

protected:
	virtual void onDraw(SkCanvas* canvas);
    virtual void onSizeChange();

private:
	DeviceType fDeviceType;
    DeviceManager* fDevManager;

	DeviceManager::BackendOptions fBackendOptions;

	int fMSAASampleCount;

	IDraw * draw_;

private:
	typedef SkOSWindow INHERITED;
};

#endif//__LYRAWINDOW_H__
