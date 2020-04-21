#pragma once

#define STATUS(str) {m_strMsg = str; PRINTF(_T("%s %s"), m_strName, str);}

struct SMsgRunInfo
{
	CCriticalSection m_lock;
	CString m_strRunInfo;
	_declspec(dllexport) void operator =(LPCTSTR strMsg);
	_declspec(dllexport) CString GetInfo();
};
class  CRunTask
{
public:
   CRunTask();
   virtual ~CRunTask();
public:
	__declspec(dllexport) BOOL SetThreadPriority(int nPriority);
	__declspec(dllexport) BOOL Resume();
	__declspec(dllexport) BOOL Suspend();
public:
	BOOL IsTerminate(void);
	// Operations
	virtual UINT RunThread();

	// Run Thread Control
	BOOL Begin();
	BOOL DeleteThread();

	CString GetMsg()	{ return m_strMsg.GetInfo(); }

public:
	CString		m_strName;
	COLORREF	m_crMsg;

protected:
	CWinThread	*m_pThread;
	BOOL m_bSuspended;
	BOOL m_bTerminate;

	SMsgRunInfo		m_strMsg;			// message to show at main page
};