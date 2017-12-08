#include "StdAfx.h"
#include "MainFrame.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
   CPaintManagerUI::SetResourceInstance(hInstance);

   HRESULT Hr = ::CoInitialize(NULL);
   if( FAILED(Hr) ) return 0;

   if( ::LoadLibrary("d3d9.dll") == NULL ) ::MessageBox(NULL, "Denne applikation vises bedst med DirectX 9 installeret!", "Test", MB_ICONINFORMATION);

   CMainFrame* pFrame = new CMainFrame();
   if( pFrame == NULL ) return 0;
   pFrame->Create(NULL, "MainFrameWnd", UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);

   CPaintManagerUI::MessageLoop();

   ::CoUninitialize();
   return 0;
}