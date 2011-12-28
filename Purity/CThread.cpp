#include "Purity.h"

CThread::CThread():m_hThread(NULL)
{
	m_hThreadStart = ::CreateEvent(NULL,TRUE,FALSE,NULL);
	m_hThreadEnd = ::CreateEvent(NULL,TRUE,FALSE,NULL);
	m_hThreadAbort = ::CreateEvent(NULL,TRUE,FALSE,NULL);
}

CThread::~CThread()
{
	stop();
	::CloseHandle(m_hThreadStart);
	::CloseHandle(m_hThreadEnd);
}

unsigned int __stdcall CThread::__threadFunc(LPVOID pParam)
{
	CThread* pThread = reinterpret_cast<CThread *> (pParam);
	HANDLE hThreadEnd = pThread->m_hThreadEnd;
	
	pThread->threadInit();
	unsigned int nResult = pThread->run();
	pThread->threadCleanup();

	::SetEvent(hThreadEnd);
	return nResult;
}

bool CThread::start()
{
	if(m_hThread != NULL)
		return false;

	::ResetEvent(m_hThreadAbort);
	::ResetEvent(m_hThreadEnd);

	unsigned int nThreadID = 0;
	
	int nResult = (int) _beginthreadex(NULL, 0, __threadFunc, (void *)this, 0, &nThreadID);
	if(nResult <= 0)
		return false;
	
	m_hThread = reinterpret_cast<HANDLE>(nResult);
	DWORD nWait = ::WaitForSingleObject(m_hThreadStart, INFINITE);

	::ResetEvent(m_hThreadStart);

	if(nWait != WAIT_OBJECT_0)
		return false;

	return true;
};

void CThread::stop()
{
	if(m_hThread)
	{
		::SetEvent(m_hThreadAbort);
		if(::WaitForSingleObject(m_hThreadEnd, 5000) == WAIT_OBJECT_0)
			::ResetEvent(m_hThreadEnd);
		else
		{
			::TerminateThread(m_hThread, 1);
			m_hThread = NULL;
		}
	}
	else
		::ResetEvent(m_hThreadEnd);
}

void CThread::threadInit()
{
	::SetEvent(m_hThreadStart);
}

void CThread::threadCleanup()
{
	::CloseHandle(m_hThread);
	m_hThread = NULL;
}


bool CThread::waitThread(unsigned long ulTimeOut)
{
	if(m_hThread == NULL)
		return false;

	bool result = (WaitForSingleObject(m_hThreadEnd, ulTimeOut) == WAIT_OBJECT_0) ? true : false;

	if(!result)
	{
		::TerminateThread(m_hThread, 1);
		m_hThread = NULL;
	}

	return result;
}