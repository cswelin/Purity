/*
	File: CThread.h
	Desc: Thread wrapper class. 
	Author: Colin Swelin
*/

#ifndef __CTHREAD_H__
#define __CTHREAD_H__

class CThread
{
public:
	bool start();
	void stop();
	bool waitThread(unsigned long ulTimeOut = INFINITE);

protected:
	//constructors
	CThread();
	virtual ~CThread();
	
	//overridable functions
	virtual void	threadInit();
	virtual void	threadCleanup();
	virtual unsigned int	run() = 0;

	bool isThreadRunning() { return m_hThread != 0; }

protected:
	HANDLE m_hThread;
	LPVOID pParam;
private:
	HANDLE m_hThreadStart;
	HANDLE m_hThreadEnd;
	HANDLE m_hThreadAbort;
	static unsigned int __stdcall __threadFunc(LPVOID pParam);
};

#endif // ifdef__CTHREAD_H__


