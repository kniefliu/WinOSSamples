// Parent process

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <string>

extern "C" VOID __cdecl SetSharedMem(char * lpszBuf);
extern "C" void __cdecl LogMessage(const char * message);

HANDLE CreateChildProcess(LPTSTR szCmdline)
{
	PROCESS_INFORMATION piProcInfo;
	STARTUPINFO siStartInfo;
	BOOL bFuncRetn = FALSE;

	// Set up members of the PROCESS_INFORMATION structure. 

	ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));

	// Set up members of the STARTUPINFO structure. 

	ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
	siStartInfo.cb = sizeof(STARTUPINFO);

	// Create the child process. 

	bFuncRetn = CreateProcess(NULL,
		szCmdline,     // command line 
		NULL,          // process security attributes 
		NULL,          // primary thread security attributes 
		TRUE,          // handles are inherited 
		0,             // creation flags 
		NULL,          // use parent's environment 
		NULL,          // use parent's current directory 
		&siStartInfo,  // STARTUPINFO pointer 
		&piProcInfo);  // receives PROCESS_INFORMATION 

	if (bFuncRetn == 0)
	{
		printf("CreateProcess failed (%d)\n", GetLastError());
		return INVALID_HANDLE_VALUE;
	}
	else
	{
		CloseHandle(piProcInfo.hThread);
		return piProcInfo.hProcess;
	}
}

int _tmain(int argc, TCHAR *argv[])
{
	HANDLE hProcess;

	TCHAR cmdLine[MAX_PATH] = { 0 };
	if (argc == 1)
	{
		printf("Please specify an input file");
		//ExitProcess(0);
		lstrcpy(cmdLine, L"FileMappingChildProcess.exe");
	}
	else {
		lstrcpy(cmdLine, argv[1]);
	}

	// Call the DLL function
	LogMessage("Process is writing to shared memory...\n\n");
	LogMessage("\tThis is a test string\n\n");
	SetSharedMem("This is a test string");

	// Start the child process that will read the memory
	LogMessage("Create child process\n");
	hProcess = CreateChildProcess(cmdLine);

	// Ensure this process is around until the child process terminates
	if (INVALID_HANDLE_VALUE != hProcess)
	{
		WaitForSingleObject(hProcess, INFINITE);
		CloseHandle(hProcess);
	}
	return 0;
}
