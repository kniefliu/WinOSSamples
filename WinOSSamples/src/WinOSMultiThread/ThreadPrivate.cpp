#include "ThreadPrivate.h"
#include "Thread.h"


CThreadPrivate::CThreadPrivate(CThread * owner)
	: m_owner(owner)
{
	m_exitEvent = ::CreateEvent(NULL, 0, FALSE, NULL);
}
CThreadPrivate::~CThreadPrivate()
{
	::CloseHandle(m_exitEvent);
}

