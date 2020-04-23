#pragma once
#include "clientthread.h"
#include "ClientSocket.h"

#define SIZE_BUF 50
class CCCDCheckClient :
	public CClientThread
{
public:
	CCCDCheckClient(void);
	~CCCDCheckClient(void);


public:
	BOOL Connect(const CString & strIP,int iPort);
	void Disconnect();
	BOOL IsConnect();

	//send recv
	BOOL SendToDeviceAndWait(CString strCmd,CString& _strRecv);

protected:
	virtual void RunWhile();
private:
	BOOL Connect(char* szIp,int iPort);
	BOOL Send(char* szBuf,int iBufSize);
	BOOL SendToDevice(CString strCmd = NULL);
	BOOL GetPressValue( unsigned int nSite, float * pfPressValue);
	unsigned int CRC_Check(unsigned char *data, unsigned char length);
public:
	CClientSocket m_pCCDClientSocket;

private:
	CString m_strReceive;
	char m_szReceive[SIZE_BUF];
	CCriticalSection m_pClientSocketLock;
	CCriticalSection m_PressLock;
};
