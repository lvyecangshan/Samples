#pragma once
#include "winsock2.h"
#pragma comment(lib,"ws2_32.lib")

class CClientSocket
{
public:
	CClientSocket(void);
	~CClientSocket(void);

public:
	BOOL Connect(char* szIp,int iPort);
	BOOL IsConnect();
	void Disconnect();

	BOOL Send(char* szBuf,int iSize);
	BOOL Recv(char* szBuf,int iSize,int iFlags = 0);

public:
	SOCKET m_pSocket;
private:
	BOOL m_bConnect;
};