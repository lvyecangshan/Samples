/****************************************************************************************
�ļ�����LineMotorThread.h
������������������ͣ�ͼ�צ��ײ��Ӧ�߳� chen 2019.7.16
��ʷ�޸ļ�¼��
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
