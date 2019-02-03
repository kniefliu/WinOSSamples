// Child process

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <sstream>

extern "C" VOID __cdecl GetSharedMem(char * lpszBuf, DWORD cchSize);
extern "C" void __cdecl LogMessage(const char * message);

int _tmain(void)
{
	char cBuf[MAX_PATH];

	GetSharedMem(cBuf, MAX_PATH);

	std::stringstream ss;
	ss << "\nChild process read from shared memory: " << cBuf << std::endl;

	LogMessage(ss.str().c_str());

	return 0;
}