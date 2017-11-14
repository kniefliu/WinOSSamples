#ifndef __THREADPRIVATE_H__
#define __THREADPRIVATE_H__

#include <Windows.h>

class CThread;
class CThreadPrivate
{
public:
	CThreadPrivate(CThread * owner);
	~CThreadPrivate();


protected:
	CThread * m_owner;
	HANDLE m_threadHandle;
	DWORD m_threadId;
	HANDLE m_exitEvent;
	
private:
	friend class CThread;
};

#endif//__THREADPRIVATE_H__
