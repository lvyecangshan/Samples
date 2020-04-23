#include "StdAfx.h"
#include "ClientThread.h"

DWORD WINAPI ClientThreadProc(void* pParam)
{
	CClientThread* pThread = (CClientThread*)pParam;
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

CClientThread::CClientThread(void)
{
	m_dwThreadId = 0;
	m_bRunning = FALSE;
	m_hThread = NULL;
}

CClientThread::~CClientThread(void)
{
	ReleaseClientThread();
}

BOOL CClientThread::InitClientThread()
{
	if (m_hThread != NULL)
	{
		return TRUE;
	}

	m_bRunning = TRUE;
	m_hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ClientThreadProc, this, 0, &m_dwThreadId);

	if (m_hThread == NULL)
	{
		return FALSE;
	}
	return TRUE;
}

void CClientThread::ReleaseClientThread()
{
	if (m_hThread != NULL)
	{
		DWORD dwExitcode;
		m_bRunning = FALSE;
		if(GetExitCodeThread(m_hThread, &dwExitcode))
		{
			TerminateThread(m_hThread, dwExitcode);
		}
		m_hThread = NULL;
		m_dwThreadId = 0;
	}
}

void CClientThread::RunWhile()
{

}