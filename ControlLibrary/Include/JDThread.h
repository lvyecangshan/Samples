#pragma once
#include <wtypes.h>
#include <WinBase.h>

class __declspec(dllexport) CJDThread
{
public:
	CJDThread(void);
	~CJDThread(void);
	BOOL InitMachineThread();
	void ReleaseMachineThread();
	virtual void RunInWhile();
	virtual void AliveRunInWhile();

	BOOL InitAliveThread();
	void ReleaseAliveThread();
	BOOL m_bRunning;
	BOOL m_bAliveRunning;
private:
	HANDLE m_hThread;
	DWORD m_dwThreadId;

	HANDLE m_hAliveThread;
	DWORD m_dwAliveThreadId;
};
