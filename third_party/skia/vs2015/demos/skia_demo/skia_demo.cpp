// skia_demo.cpp : 定义应用程序的入口点。
//

#include <Windows.h>
#include "skia_demo.h"

#include "FrameWindowWnd.h"

int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR    lpCmdLine,
	int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	CWindowWnd::SetResourceInstance(hInstance);

	//::Sleep(60000 * 2);

	CFrameWindowWnd* pFrame = new CFrameWindowWnd();
	if( pFrame == NULL ) return 0;
	pFrame->Create(NULL, TEXT("skia_demo : "), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);


	CWindowWnd::MessageLoop();


	return 0;
}

