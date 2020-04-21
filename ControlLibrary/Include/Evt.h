#pragma once

#include <math.h>
#include <Afxmt.h>
#include <afxtempl.h>
#include <memory>

class __declspec(dllexport) CEvt
{
public:
	CEvt();	// create an object without name, this object can only be used in the local thread 

	HANDLE GetEvt(BOOL bState);
	~CEvt();

	BOOL CEvt::operator =(const BOOL bState);
	BOOL CEvt::operator ==(const BOOL bState);
	BOOL CEvt::operator !=(const BOOL bState);
	
	// Operations
	BOOL WaitEvt(DWORD dwTimeOut=INFINITE);
	BOOL WaitResetEvt(DWORD dwTimeOut=INFINITE);
	BOOL Reset(void);
	BOOL Set(void);

	DWORD GetTimeout() { return m_dwTimeout; }
	void  SetTimeout(DWORD timeout) { m_dwTimeout = timeout; }

public:
	CString		m_strName;
	int			m_nIndex;

private:
	CEvent		m_evtActivate;
	CEvent		m_evtDeactivate;
	DWORD		m_dwTimeout;
};

DWORD WaitHandle(HANDLE handle, DWORD dwTimeout);
DWORD WaitHandle(LPHANDLE handle, BOOL bWaitForAll, DWORD dwTimeout, int nNoOfHandle);
