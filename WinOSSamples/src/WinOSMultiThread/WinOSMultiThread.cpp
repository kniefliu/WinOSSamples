#include <iostream>

#include "Thread.h"

#include <Windows.h>

static int g_Count = 0;
static HANDLE g_mutex = 0;

class CThread1 : public CThread
{
public:
	void Run()
	{
		WaitForSingleObject(g_mutex, INFINITE);
		std::cout << "thread" << threadData.id << " : " << g_Count++ << std::endl;
		::ReleaseMutex(g_mutex);
	}
};

class CThread2 : public CThread
{
public:
	void Run()
	{
		WaitForSingleObject(g_mutex, INFINITE);
		std::cout << "thread" << threadData.id << " : "  << g_Count++ << std::endl;
		::ReleaseMutex(g_mutex);
	}
};


int main()
{
	std::cout << "WinOSMultiThread start." << std::endl;

	CThread1 thread1;
	CThread2 thread2;

	g_mutex = CreateMutex(NULL, FALSE, 0);

	thread1.setThreadData(CThreadData(1));
	thread2.setThreadData(CThreadData(2));

	thread1.start();
	thread2.start();

	for (int i = 0; i < 10; i++)
	{
		::Sleep(500);
	}

	thread1.stop();
	thread2.stop();

	thread1.waitExit();
	thread2.waitExit();

	CloseHandle(g_mutex);

	std::cout << "WinOSMultiThread stop." << std::endl;
	return 0;
}
