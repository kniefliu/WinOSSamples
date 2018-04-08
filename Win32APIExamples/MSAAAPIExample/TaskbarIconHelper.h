#ifndef __TASKBARICONHELPER_H__
#define __TASKBARICONHELPER_H__

#include <Windows.h>

class TaskbarIconHelper
{
public:
	/*RECT : screen */
	static RECT GetIconRect(HWND hwndApp);
};

#endif//__TASKBARICONHELPER_H__
