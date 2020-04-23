#include "StdAfx.h"
#include "ClientSocket.h"

CClientSocket::CClientSocket(void)
{
	WSADATA wsadata;
	m_bConnect = FALSE;
	m_pSocket = INVALID_SOCKET;
	WSAStartup(MAKEWORD(2,2), &wsadata);
}

CClientSocket::~CClientSocket(void)
{
	Disconnect();
}

BOOL CClientSocket::Connect(char* szIp,int iPort)
{
	m_bConnect = FALSE;

	m_pSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (INVALID_SOCKET == m_pSocket)
	{
		return FALSE;
	}

	SOCKADDR_IN addr;
	addr.sin_addr.S_un.S_addr = inet_addr(szIp);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(iPort);
	if (SOCKET_ERROR == connect(m_pSocket,(SOCKADDR*)&addr,sizeof(SOCKADDR)))
	{
		Disconnect();
		return FALSE;
	}

	m_bConnect = TRUE;
	return TRUE;
}

void CClientSocket::Disconnect()
{	
	if (m_pSocket != INVALID_SOCKET)
	{
		shutdown(m_pSocket,2);
		closesocket(m_pSocket);
		m_pSocket = INVALID_SOCKET;
	}
	m_bConnect = FALSE;
}

BOOL CClientSocket::IsConnect()
{
	return m_bConnect;
}

BOOL CClientSocket::Send(char* szBuf,int iSize)
{
	if (!IsConnect())
	{
		return FALSE;
	}

	int iSend = send(m_pSocket,szBuf,iSize,0);
	if (SOCKET_ERROR == iSend)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CClientSocket::Recv(char* szBuf,int iSize,int iFlags /* = 0 */)
{
	int iRev = recv(m_pSocket,szBuf,iSize,iFlags);
	if (iRev < 1)
	{
		if(iRev == 0)
		{
			Disconnect();
		}
		return FALSE;
	}
	return TRUE;
}