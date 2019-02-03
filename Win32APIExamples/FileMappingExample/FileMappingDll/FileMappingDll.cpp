// The DLL code

#include <windows.h> 
#include <memory.h> 
#include <fstream>
#include <sstream>

#define SHMEMSIZE 4096 

static LPVOID lpvMem = NULL;      // pointer to shared memory
static HANDLE hMapObject = NULL;  // handle to file mapping

static HANDLE mutexOfLog = NULL;
static char logFileName[64] = { "FileMapping.txt" };
static void Log(const char * message);

// The DLL entry-point function sets up shared memory using a 
// named file-mapping object. 

BOOL WINAPI DllMain(HINSTANCE hinstDLL,  // DLL module handle
	DWORD fdwReason,              // reason called 
	LPVOID lpvReserved)           // reserved 
{
	BOOL fInit, fIgnore;
	DWORD err;

	switch (fdwReason)
	{
		// DLL load due to process initialization or LoadLibrary

	case DLL_PROCESS_ATTACH:

		mutexOfLog = CreateMutex(NULL, FALSE, TEXT("FileWriteLock"));
		err = GetLastError();
		if (err == ERROR_ALREADY_EXISTS) {
			//MessageBox(NULL, TEXT("FileWriteLock already exist"), TEXT("Mutex"), 0);
			std::stringstream ss;
			Log("FileWriteLock already exist\n");
		}
		else {
			// clear file
			std::ofstream ofs;
			ofs.open(logFileName, std::ofstream::out | std::ofstream::trunc);
			ofs.close();
		}

		// Create a named file mapping object

		hMapObject = CreateFileMapping(
			INVALID_HANDLE_VALUE,   // use paging file
			NULL,                   // default security attributes
			PAGE_READWRITE,         // read/write access
			0,                      // size: high 32-bits
			SHMEMSIZE,              // size: low 32-bits
			TEXT("dllmemfilemap")); // name of map object
		if (hMapObject == NULL)
			return FALSE;

		// The first process to attach initializes memory

		fInit = (GetLastError() != ERROR_ALREADY_EXISTS);

		// Get a pointer to the file-mapped shared memory

		lpvMem = MapViewOfFile(
			hMapObject,     // object to map view of
			FILE_MAP_WRITE, // read/write access
			0,              // high offset:  map from
			0,              // low offset:   beginning
			0);             // default: map entire file
		if (lpvMem == NULL)
			return FALSE;

		// Initialize memory if this is the first process

		if (fInit)
			memset(lpvMem, '\0', SHMEMSIZE);

		break;

		// The attached process creates a new thread

	case DLL_THREAD_ATTACH:
		break;

		// The thread of the attached process terminates

	case DLL_THREAD_DETACH:
		break;

		// DLL unload due to process termination or FreeLibrary

	case DLL_PROCESS_DETACH:
		CloseHandle(mutexOfLog);

		// Unmap shared memory from the process's address space

		fIgnore = UnmapViewOfFile(lpvMem);

		// Close the process's handle to the file-mapping object

		fIgnore = CloseHandle(hMapObject);

		break;

	default:
		break;
	}

	return TRUE;
	UNREFERENCED_PARAMETER(hinstDLL);
	UNREFERENCED_PARAMETER(lpvReserved);
}

// The export mechanism used here is the __declspec(export)
// method supported by Microsoft Visual Studio, but any
// other export method supported by your development
// environment may be substituted.

#ifdef __cplusplus    // If used by C++ code, 
extern "C" {          // we need to export the C interface
#endif

// SetSharedMem sets the contents of the shared memory 

	__declspec(dllexport) VOID __cdecl SetSharedMem(char * lpszBuf)
	{
		char * lpszTmp;
		DWORD dwCount = 1;

		// Get the address of the shared memory block

		lpszTmp = (char *)lpvMem;

		// Copy the null-terminated string into shared memory

		while (*lpszBuf && dwCount < SHMEMSIZE)
		{
			*lpszTmp++ = *lpszBuf++;
			dwCount++;
		}
		*lpszTmp = '\0';
	}

	// GetSharedMem gets the contents of the shared memory

	__declspec(dllexport) VOID __cdecl GetSharedMem(char * lpszBuf, DWORD cchSize)
	{
		char * lpszTmp;

		// Get the address of the shared memory block

		lpszTmp = (char *)lpvMem;

		// Copy from shared memory into the caller's buffer

		while (*lpszTmp && --cchSize)
			*lpszBuf++ = *lpszTmp++;
		*lpszBuf = '\0';
	}


	__declspec(dllexport) void __cdecl LogMessage(const char * message)
	{
		WaitForSingleObject(mutexOfLog, INFINITE);
		std::fstream fileLog;
		fileLog.open(logFileName, std::fstream::app | std::fstream::out);
		fileLog.write(message, strlen(message));
		fileLog.close();
		::ReleaseMutex(mutexOfLog);
	}
#ifdef __cplusplus
}
#endif

static void Log(const char * message)
{
	LogMessage(message);
}
