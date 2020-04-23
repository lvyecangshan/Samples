#include "StdAfx.h"
#include "LineMotorThread.h"

DWORD WINAPI MotorThread(void* pParam)
{
	LineMotorThread* pThread = (LineMotorThread*)pParam;
	if (pThread == NULL)
	{
		return 0;
	}

	while(pThread->m_bRunning)
	{
		pThread->RunWhile();
		Sleep(10);
	}

	return 0;
}

LineMotorThread::LineMotorThread(void)
{
	m_dwThreadId = 0;
	m_bRunning = FALSE;
	m_hThread = NULL;
}

LineMotorThread::~LineMotorThread(void)
{
	ReleaseMotorThread();
}

BOOL LineMotorThread::InitMotorThread()
{
	if (m_hThread != NULL)
	{
		return TRUE;
	}

	m_bRunning = TRUE;
	m_hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MotorThread, this, 0, &m_dwThreadId);

	if (m_hThread == NULL)
	{
		return FALSE;
	}
	return TRUE;
}

void LineMotorThread::ReleaseMotorThread()
{

	if (m_hThread != NULL)
	{
		DWORD dwExitcode;
		m_bRunning = FALSE;
		if(GetExitCodeThread(m_hThread, &dwExitcode))
		{
			TerminateThread(m_hThread,dwExitcode);
			WaitForSingleObject(m_hThread,INFINITE);
		}
		m_hThread = NULL;
		m_dwThreadId = 0;
	}
}

void LineMotorThread::RunWhile()
{

}