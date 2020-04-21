#pragma once
#include "JDSockDef.h"
//#include "winsock2.h"
//#pragma comment(lib,"ws2_32.lib")
class __declspec(dllexport) CJDTcpClient :public CJDThread
{
public:
	CJDTcpClient(void);
	~CJDTcpClient(void);
	virtual void RunInWhile();
	virtual void AliveRunInWhile();

	BOOL Connect(char* szIp,int iPort);
	BOOL IsConnect();
	void Disconnect();
	BOOL Send(char* szBuf,int iSize);
	int Recv(char* szBuf,int iSize,int iFlags = 0);
	BOOL GetState();
	void SendKeepAlive();
	SOCKET m_ClientSocket;
private:
	BOOL m_bConnect;
	BOOL m_bState;
	int m_iKeepAliveTime;
	int m_iSendAliveTime;
	HWND m_hwdMain;
	CString m_strMsg;
};
