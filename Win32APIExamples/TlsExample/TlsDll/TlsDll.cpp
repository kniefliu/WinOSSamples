// The DLL code

#include <windows.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <stdlib.h>

static DWORD dwTlsIndex; // address of shared memory

CRITICAL_SECTION csMutex;
std::fstream logFile;


class AllocHelper
{
public:
	static void * Alloc(size_t size);
	static void Free(void *pBuffer);
};

// The export mechanism used here is the __declspec(export)
// method supported by Microsoft Visual Studio, but any
// other export method supported by your development
// environment may be substituted.

#ifdef __cplusplus    // If used by C++ code, 
extern "C" {          // we need to export the C interface
#endif

	__declspec(dllexport)
		BOOL WINAPI StoreData(DWORD dw)
	{
		LPVOID lpvData;
		DWORD * pData;  // The stored memory pointer 

		lpvData = TlsGetValue(dwTlsIndex);
		if (lpvData == NULL)
		{
			lpvData = (LPVOID)AllocHelper::Alloc(256);// LocalAlloc(LPTR, 256); 
			if (lpvData == NULL)
				return FALSE;
			if (!TlsSetValue(dwTlsIndex, lpvData))
				return FALSE;
		}

		pData = (DWORD *)lpvData; // Cast to my data type.
								  // In this example, it is only a pointer to a DWORD
								  // but it can be a structure pointer to contain more complicated data.

		(*pData) = dw;
		return TRUE;
	}

	__declspec(dllexport)
		BOOL WINAPI GetData(DWORD *pdw)
	{
		LPVOID lpvData;
		DWORD * pData;  // The stored memory pointer 

		lpvData = TlsGetValue(dwTlsIndex);
		if (lpvData == NULL)
			return FALSE;

		pData = (DWORD *)lpvData;
		(*pdw) = (*pData);
		return TRUE;
	}

	__declspec(dllexport)
		void WINAPI LogMessage(const char * message)
	{
		EnterCriticalSection(&csMutex);
		std::cout << message;
		logFile.write(message, strlen(message));
		LeaveCriticalSection(&csMutex);
	}
#ifdef __cplusplus
}
#endif

// DllMain() is the entry-point function for this DLL. 

BOOL WINAPI DllMain(HINSTANCE hinstDLL, // DLL module handle
	DWORD fdwReason,                    // reason called
	LPVOID lpvReserved)                 // reserved
{
	LPVOID lpvData;
	BOOL fIgnore;

	switch (fdwReason)
	{
		// The DLL is loading due to process 
		// initialization or a call to LoadLibrary. 

	case DLL_PROCESS_ATTACH:

		// Allocate a TLS index.

		if ((dwTlsIndex = TlsAlloc()) == TLS_OUT_OF_INDEXES)
			return FALSE;

		InitializeCriticalSection(&csMutex);
		logFile.open("logMessage.txt", std::fstream::out | std::fstream::trunc);
		

		// No break: Initialize the index for first thread.

	// The attached process creates a new thread. 

	case DLL_THREAD_ATTACH:

		// Initialize the TLS index for this thread.
	{
		std::stringstream ss;
		ss << "Dll Thread Attach : " << GetCurrentThreadId() << std::endl;
		LogMessage(ss.str().c_str());
	}
		lpvData = (LPVOID)AllocHelper::Alloc(256);  // LocalAlloc(LPTR, 256); 
		if (lpvData != NULL)
			fIgnore = TlsSetValue(dwTlsIndex, lpvData);

		break;

		// The thread of the attached process terminates.

	case DLL_THREAD_DETACH:

		// Release the allocated memory for this thread.
	{
		std::stringstream ss;
		ss << "Dll Thread Detach : " << GetCurrentThreadId() << std::endl;
		LogMessage(ss.str().c_str());
	}

		lpvData = TlsGetValue(dwTlsIndex);
		if (lpvData != NULL)
			AllocHelper::Free(lpvData); // LocalFree((HLOCAL)lpvData);



		break;

		// DLL unload due to process termination or FreeLibrary. 

	case DLL_PROCESS_DETACH:

		// Release the allocated memory for this thread.
	{
		std::stringstream ss;
		ss << "Dll Process Detach : " << GetCurrentThreadId() << std::endl;
		LogMessage(ss.str().c_str());
	}

		lpvData = TlsGetValue(dwTlsIndex);
		if (lpvData != NULL)
			AllocHelper::Free(lpvData); // LocalFree((HLOCAL)lpvData);

		// Release the TLS index.

		TlsFree(dwTlsIndex);

		DeleteCriticalSection(&csMutex);
		logFile.close();
		break;

	default:
		break;
	}

	return TRUE;
	UNREFERENCED_PARAMETER(hinstDLL);
	UNREFERENCED_PARAMETER(lpvReserved);
}

void * AllocHelper::Alloc(size_t size) {
	void * pBuffer = malloc(size);
	std::stringstream ss;
	ss << "AllocHelper::Alloc " << size << " to " << pBuffer << std::endl;
	LogMessage(ss.str().c_str());
	return pBuffer;
}
void AllocHelper::Free(void *pBuffer) {
	std::stringstream ss;
	ss << "AllocHelper::Free from " << pBuffer << std::endl;
	LogMessage(ss.str().c_str());
	std::free(pBuffer);
	pBuffer = nullptr;
}