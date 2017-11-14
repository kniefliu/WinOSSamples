#include "Thread.h"
#include "ThreadPrivate.h"

#include <Windows.h>
#include <process.h>

#include <iostream>


CThread::CThread()
{
	m_private = new CThreadPrivate(this);
}
CThread::~CThread()
{
	::CloseHandle(m_private->m_threadHandle);
	WaitForSingleObject(m_private->m_threadHandle, 200);
	if (m_private)
		delete m_private;
}

void CThread::start()
{ 
	m_private->m_threadHandle = (HANDLE)_beginthreadex(NULL, 0, threadMain, this, 0, 0);
}
void CThread::stop()
{
	SetEvent(m_private->m_exitEvent);
}

void CThread::waitExit()
{
	WaitForSingleObject(m_private->m_threadHandle, INFINITE);
}

void CThread::setThreadData(const CThreadData& data)
{
	threadData = data;
}

unsigned __stdcall CThread::threadMain(void *pData)
{
	CThread* pThread = (CThread*)pData;
	while (true)
	{
		if (WAIT_TIMEOUT == ::WaitForSingleObject(pThread->m_private->m_exitEvent, 20))
			pThread->Run();
		else
			break;
	}
	_endthreadex(0);
	return 0;
}
