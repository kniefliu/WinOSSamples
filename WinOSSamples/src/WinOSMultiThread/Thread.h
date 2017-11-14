#ifndef __THREAD_H__
#define __THREAD_H__

struct CThreadData
{
	CThreadData(int i = 0)
	{
		id = i;
	}

	int id;
};

class CThreadPrivate;
class CThread
{
public:
	CThread();
	~CThread();

public:
	virtual void Run() = 0;

	void start();
	void stop();

	void waitExit();

	void setThreadData(const CThreadData& data);

protected:	
	static unsigned __stdcall threadMain(void *);

protected:
	CThreadData threadData;
	CThreadPrivate * m_private;

private:
	friend class CThreadPrivate;
};

#endif//__THREAD_H__
