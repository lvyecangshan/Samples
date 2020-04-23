// 视觉Socket客户端类
// 函数可根据运用实际情况进行更改，而不要只增加不精简
// 发送命令格式以及接受字符串处理需根据运用场景不同进行调整

#pragma once
#include "clientthread.h"
#include "ClientSocket.h"

class CVisionClient : public CClientThread
{
public:
	CVisionClient(void);
	~CVisionClient(void);

	BOOL Connect(char* szIp,int iPort);
	BOOL Connect(const CString & strIP,int iPort);
	void Disconnect();
	BOOL IsConnect(){return m_pSocket.IsConnect();}

public:
	BOOL Send(char* szBuf,int iBufSize);
	BOOL SendToDevice(CString strSend);
	BOOL SendAndWait(CString srcSend);

public:
	BOOL GetReceiveResult(CString strCmd);
	void WriteLog(CString strLog);
	void ClearReceive();
	virtual void RunWhile();

public:
	BOOL m_bRecvEnable;
	CString m_strReceive;

private:
	int m_nPort;
	CString m_strIP;
	char m_szReceive[256];
	CClientSocket m_pSocket;
	CCriticalSection m_VisionLock;
};

