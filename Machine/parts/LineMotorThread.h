/****************************************************************************************
文件名：LineMotorThread.h
功能描述：线体电机启停和夹爪防撞感应线程 chen 2019.7.16
历史修改记录：
*****************************************************************************************/
#pragma once

class LineMotorThread
{
public:
	LineMotorThread(void);
	~LineMotorThread(void);

public:
	BOOL InitMotorThread();
	void ReleaseMotorThread();
	virtual void RunWhile();

public:
	BOOL m_bRunning;

private:
	HANDLE m_hThread;
	DWORD  m_dwThreadId;
};
