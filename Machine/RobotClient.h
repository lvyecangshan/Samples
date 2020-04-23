#pragma once
#include "clientthread.h"
#include "ClientSocket.h"


class CRobotClient :public CClientThread
{
public:
	CRobotClient(void);
	~CRobotClient(void);

public:
	BOOL Connect(const CString strIP,int iPort);
	BOOL SendToDeviceAndWait(int g_RobotCMd);
	BOOL GetReceiveResult(int g_Cmd);
	void Disconnect();
	BOOL IsConnect()
	{
		return m_ClientSocket.IsConnect();
	}

private:
	BOOL Connect(char* szIp,int iPort);
	BOOL Send(char* szBuf,int iBufSize);
	BOOL SendToDevice(CString strSend);
	virtual void RunWhile();
	void ClearReceiveResult();

private:
	BOOL m_bEnableReceive;
	int m_nRobotPort;
	CString m_strRobotIP;
	CString m_strRecvResult;
	char m_szReceive[256];
	CClientSocket m_ClientSocket;
};
