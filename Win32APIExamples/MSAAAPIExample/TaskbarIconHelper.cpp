#include "TaskbarIconHelper.h"

#include <Oleacc.h>
#pragma comment(lib, "Oleacc.lib")

#include <string>
#include <sstream>

HRESULT PrintState(IAccessible* pAcc, long childId)
{
	if (pAcc == NULL)
	{
		return E_INVALIDARG;
	}
	VARIANT      varChild;
	varChild.vt = VT_I4;
	varChild.lVal = childId;
	VARIANT varResult;
	HRESULT hr = pAcc->get_accState(varChild, &varResult);
	long stateBits = 0;
	if ((hr == S_OK) && (varResult.vt == VT_I4))
	{
		printf("State: ");
		stateBits = (DWORD)varResult.lVal;
		for (DWORD mask = 1; mask <= 0x8000; mask <<= 1)
		{
			if (mask & stateBits)
			{

				// Get the length of the string. 
				UINT stateLength = GetStateText(mask, NULL, 0);

				// Allocate memory for the string. Add one character to 
				// the length you got in the previous call to make room 
				// for the null character. 
				LPTSTR lpszStateString = (LPTSTR)malloc(
					(stateLength + 1) * sizeof(TCHAR));
				if (lpszStateString != NULL)
				{
					// Get the string. 
					GetStateText(mask,
						lpszStateString, stateLength + 1);
#ifdef UNICODE
					printf("%S\n", lpszStateString);
#else
					printf(("%s\n", lpszStateString);
#endif
					// Free the allocated memory
					free(lpszStateString);
				}
				else
				{
					return E_OUTOFMEMORY;
				}
			}
		}
	}
	return hr;
}


static BOOL CALLBACK EnumShellWindowsInVistaOrWin7(HWND hwnd, LPARAM userParam)
{
	wchar_t buffer[255];
	HWND *hwndTaskList = (HWND *)userParam;

	GetClassNameW(hwnd, buffer, sizeof buffer);
	if (std::wstring(buffer) == L"MSTaskListWClass") {
		*hwndTaskList = hwnd;
		return FALSE;
	}
	return TRUE;
}

static BOOL CALLBACK GetReBarWindow32InXP(HWND hwnd, LPARAM userParam)
{
	wchar_t buffer[255];
	HWND *hwndTaskList = (HWND *)userParam;

	GetClassNameW(hwnd, buffer, sizeof buffer);
	if (std::wstring(buffer) == L"ReBarWindow32") {
		*hwndTaskList = hwnd;
		return FALSE;
	}
	return TRUE;
}
static BOOL CALLBACK GetToolbarWindow32InXP(HWND hwnd, LPARAM userParam)
{
	wchar_t buffer[255];
	HWND *hwndTaskList = (HWND *)userParam;

	GetClassNameW(hwnd, buffer, sizeof buffer);
	if (std::wstring(buffer) == L"ToolbarWindow32") {
		*hwndTaskList = hwnd;
		return FALSE;
	}
	return TRUE;
}

bool FindByAccessibleState(IAccessible* pAccessible, RECT& needRect, DWORD needState = STATE_SYSTEM_PRESSED)
{
	IEnumVARIANT* pEnum = NULL;

	bool bFind = false;


	HRESULT hr = S_OK;
	ULONG ul_reference_count = 0; // for test
	hr = pAccessible->QueryInterface(IID_IEnumVARIANT, (PVOID*)&pEnum);

	long numChild = 0;
	pAccessible->get_accChildCount(&numChild);

#if 1
	for (long index = 1; index <= numChild && !bFind; index++)
	{
		VARIANT varChild;
		VariantInit(&varChild);
		
		ULONG numFetched = 0;
		if (pEnum)
			hr = pEnum->Next(1, &varChild, &numFetched);
		else
		{
			varChild.vt = VT_I4;
			varChild.lVal = index;
		}

		IDispatch *pChildDispatch = NULL;
		if (varChild.vt == VT_I4)
		{
			hr = pAccessible->get_accChild(varChild, &pChildDispatch);
		}
		else
			pChildDispatch = varChild.pdispVal;

		IAccessible * pTmpAccessible = NULL;	
		if (pChildDispatch)
		{
			hr = pChildDispatch->QueryInterface(IID_IAccessible, (void**)&pTmpAccessible);
			ul_reference_count = pChildDispatch->Release();
			VariantClear(&varChild);
		}
		else
		{
			// 如果没有找到pChildDispatch,那么说明这个pAccessible到终点了，即他的孩子再没有孩子了
			DWORD state = 0;
			VARIANT varState;
			VariantInit(&varState);
			pAccessible->get_accState(varChild, &varState);
			if (varState.vt == VT_I4)
				state = varState.lVal;
			VariantClear(&varState);

			//PrintState(pAccessible, index);

			long l;
			long t;
			long w;
			long h;
			hr = pAccessible->accLocation(&l, &t, &w, &h, varChild);

			if (hr == S_OK)
			{
				if ((STATE_SYSTEM_PRESSED & state) && w > 0 && h > 0)
				{
					needRect.left = l;
					needRect.top = t;
					needRect.right = l + w;
					needRect.bottom = t + h;
					bFind = true;
				}
			}
			VariantClear(&varChild);
			continue;
		}
		
		IAccessible * pChildAccessible = pTmpAccessible;
		if (pChildAccessible)
		{
			// 到这里说明pChildAccessible 所以先找自己
			VariantInit(&varChild);
			varChild.vt = VT_I4;
			varChild.lVal = CHILDID_SELF;
			
			DWORD state = 0;
			VARIANT varState;
			VariantInit(&varState);
			pChildAccessible->get_accState(varChild, &varState);
			if (varState.vt == VT_I4)
				state = varState.lVal;
			VariantClear(&varState);

			long l;
			long t;
			long w;
			long h;
			hr = pChildAccessible->accLocation(&l, &t, &w, &h, varChild);

			if (hr == S_OK)
			{
				if ((needState & state) && w > 0 && h > 0)
				{
					needRect.left = l;
					needRect.top = t;
					needRect.right = l + w;
					needRect.bottom = t + h;
					bFind = true;
				}
				else if (w > 0 && h > 0)
				{
					bFind = FindByAccessibleState(pChildAccessible, needRect, needState);
				}
			}

			VariantClear(&varChild);

			ul_reference_count = pChildAccessible->Release();
		}
	}

#endif

	if (pEnum)
		pEnum->Release();

	return bFind;
}

/*
 * 
 */
bool GetIconWithMSAA(HWND hTaskBar, RECT& rcIcon)
{
	bool bFind = false;

	RECT rcAppIcon;
	ZeroMemory(&rcAppIcon, sizeof(rcAppIcon));

	IAccessible *pAccessibleTaskList = NULL;
	AccessibleObjectFromWindow(hTaskBar, OBJID_WINDOW, IID_IAccessible, (void**)&pAccessibleTaskList);

	bFind = FindByAccessibleState(pAccessibleTaskList, rcAppIcon, STATE_SYSTEM_PRESSED);
	
	if (pAccessibleTaskList)
		pAccessibleTaskList->Release();

	return bFind;
}
RECT GetIconRectInXPWithMSAA(HWND hwndApp)
{
	// winxp : get taskbar hwnd
	HWND hwndTrayWnd = ::FindWindowW(L"Shell_TrayWnd", NULL);
	HWND hwndRebar = 0;
	::EnumChildWindows(hwndTrayWnd, GetReBarWindow32InXP, (LPARAM)&hwndRebar);
	HWND hwndToolbar = 0;
	::EnumChildWindows(hwndRebar, GetToolbarWindow32InXP, (LPARAM)&hwndToolbar);
	RECT rcToolbar;
	GetWindowRect(hwndToolbar, &rcToolbar);

	RECT rcAppIcon;
	BOOL bFind = GetIconWithMSAA(hwndToolbar, rcAppIcon);
	if (bFind)
		return rcAppIcon;

	return rcToolbar;
}
RECT GetIconRectInVistaOrWin7WithMSAA(HWND hwndApp)
{
	// vista or win7 : get taskbar hwnd
	HWND hwndTrayWnd = ::FindWindowW(L"Shell_TrayWnd", NULL);

	HWND hwndTaskList = 0;
	::EnumChildWindows(hwndTrayWnd, EnumShellWindowsInVistaOrWin7, (LPARAM)&hwndTaskList);

	RECT rcTaskList;
	GetWindowRect(hwndTaskList, &rcTaskList);

	RECT rcAppIcon;
	bool bFind = GetIconWithMSAA(hwndTaskList, rcAppIcon);
	if (bFind)
		return rcAppIcon;

	return rcTaskList;
}

RECT TaskbarIconHelper::GetIconRect(HWND hwndApp)
{
	return GetIconRectInVistaOrWin7WithMSAA(hwndApp);
}
