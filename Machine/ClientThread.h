#pragma once


class CClientThread
{
public:
	CClientThread(void);
	~CClientThread(void);

public:
	BOOL InitClientThread();
	void ReleaseClientThread();
	virtual void RunWhile();

public:
	BOOL m_bRunning;

private:
	HANDLE m_hThread;
	DWORD  m_dwThreadId;
};
