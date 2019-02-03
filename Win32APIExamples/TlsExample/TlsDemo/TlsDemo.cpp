#include <windows.h> 
#include <stdio.h> 
#include <sstream>

#define THREADCOUNT 4 
#define DLL_NAME TEXT("TlsDll")

VOID ErrorExit(LPSTR);

extern "C" BOOL WINAPI StoreData(DWORD dw);
extern "C" BOOL WINAPI GetData(DWORD *pdw);
extern "C" void WINAPI LogMessage(const char * message);

DWORD WINAPI ThreadFunc(VOID)
{
	int i;

	if (!StoreData(GetCurrentThreadId()))
		ErrorExit("StoreData error");

	for (i = 0; i < THREADCOUNT; i++)
	{
		DWORD dwOut;
		if (!GetData(&dwOut))
			ErrorExit("GetData error");
		if (dwOut != GetCurrentThreadId()) {
			std::stringstream ss;
			ss << "thread " << GetCurrentThreadId() << ": data is incorrect (" << dwOut << "%d)\n";
			LogMessage(ss.str().c_str());
		}
		else {
			std::stringstream ss;
			ss << "thread " << GetCurrentThreadId() << ": data is correct\n";
			LogMessage(ss.str().c_str());
		}
		Sleep(0);
	}
	return 0;
}

int main(VOID)
{
	DWORD IDThread;
	HANDLE hThread[THREADCOUNT];
	int i;
	HMODULE hm;

	// Load the DLL

	hm = LoadLibrary(DLL_NAME);
	if (!hm)
	{
		ErrorExit("DLL failed to load");
	}

	// Create multiple threads. 

	for (i = 0; i < THREADCOUNT; i++)
	{
		hThread[i] = CreateThread(NULL, // default security attributes 
			0,                           // use default stack size 
			(LPTHREAD_START_ROUTINE)ThreadFunc, // thread function 
			NULL,                    // no thread function argument 
			0,                       // use default creation flags 
			&IDThread);              // returns thread identifier 

	  // Check the return value for success. 
		if (hThread[i] == NULL)
			ErrorExit("CreateThread error\n");
	}

	WaitForMultipleObjects(THREADCOUNT, hThread, TRUE, INFINITE);

	FreeLibrary(hm);

	getchar();

	return 0;
}

VOID ErrorExit(LPSTR lpszMessage)
{
	fprintf(stderr, "%s\n", lpszMessage);
	ExitProcess(0);
}