#include "StdAfx.h"
#include "Server_Tcp.h"

CServer_Tcp::CServer_Tcp(void)
{
	m_bSocketAvailable = FALSE;
	m_pServer =  INVALID_SOCKET;
	m_bServerRun = FALSE;
	m_pServerHandle = NULL;
	m_dwServerThreadID = 0;

	m_ClientArray.RemoveAll();

	for (int iCur = 0; iCur < FD_SETSIZE; iCur++)
	{
		m_pczRecvBuf[iCur] = new char[MAX_RECV_BUF];
		memset(m_pczRecvBuf[iCur], 0, sizeof(char)*MAX_RECV_BUF);
	}
}

CServer_Tcp::~CServer_Tcp(void)
{
	try
	{
		StopServer();
		m_bSocketAvailable = FALSE;

		if(m_pServer != INVALID_SOCKET )
			closesocket(m_pServer);
		m_pServer = INVALID_SOCKET ;

		m_ClientArray.RemoveAll();
		m_bServerRun = FALSE;
		m_pServerHandle = NULL;
		m_dwServerThreadID = 0;

		for (int iCur = 0; iCur < FD_SETSIZE; iCur++)
		{
			delete [] m_pczRecvBuf[iCur];
			m_pczRecvBuf[iCur] = NULL;
		}
	}
	catch (...){}
}

BOOL CServer_Tcp::InitWinSock(int *pError)
{
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(2,2);

	BOOL bInit = (WSAStartup(wVersionRequested, &wsaData)==0);

	if(!bInit)
	{
		*pError = WSAGetLastError();
		WSACleanup();
	}
	
	return bInit;
}

BOOL CServer_Tcp::CreateServer( char *czIp, unsigned int nPort, int* pErrorCode)
{
	BOOL bCreate = FALSE;
	*pErrorCode = 0;
	m_bSocketAvailable = InitWinSock(pErrorCode);

	try
	{
		if(m_bSocketAvailable && m_pServer == INVALID_SOCKET)
		{
			struct sockaddr_in local;

			m_pServer = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
			if(m_pServer != INVALID_SOCKET )
			{
				if(czIp == NULL)
					local.sin_addr.s_addr = htonl(INADDR_ANY);
				else
					local.sin_addr.s_addr = inet_addr(czIp);

				local.sin_family = AF_INET;
				local.sin_port = htons(nPort);

				if(SOCKET_ERROR != bind(m_pServer, (struct sockaddr*)&local, sizeof(local)))
				{
					if(SOCKET_ERROR != listen(m_pServer, 5))
					{
						CString strMsg;

						FD_ZERO(&m_fdSocket);
						FD_SET(m_pServer, &m_fdSocket);

						strMsg.Format(_T("创建服务端：Port：%d"), nPort);
						SendMessage(MACHINE.m_FramHwnd,WM_COMM_INFO_MESSAGE,0,(LPARAM)strMsg.GetBuffer());
						strMsg.ReleaseBuffer();
						bCreate = TRUE;
					}
					else
					{
						*pErrorCode = WSAGetLastError();
						closesocket(m_pServer);
						m_pServer = INVALID_SOCKET;
					}
				}
				else
				{
					*pErrorCode = WSAGetLastError();
					closesocket(m_pServer);
					m_pServer = INVALID_SOCKET;
				}
			}
			else
			{
				*pErrorCode = WSAGetLastError();
				m_pServer = INVALID_SOCKET;
			}
		}
	}
	catch (...)
	{
		bCreate = FALSE;
		*pErrorCode = -1;
	}
	
	return bCreate;
}

BOOL CServer_Tcp::StartServer()
{
	BOOL bStart = FALSE;

	try
	{
		if(m_bSocketAvailable && m_pServerHandle == NULL)
		{
			m_bServerRun = TRUE;
			m_pServerHandle = CreateThread(NULL, 
				0, 
				(LPTHREAD_START_ROUTINE)ServerThreadProc, 
				(LPVOID)this, 
				0, 
				(LPDWORD)&m_dwServerThreadID);
			bStart = TRUE;
		}
	}
	catch(...)
	{
		DWORD dwThreadExitCode;

		bStart = FALSE;
		m_bServerRun = FALSE;
		if(m_pServerHandle != NULL)
		{
			WaitForSingleObject(m_pServerHandle, 100);
			if(GetExitCodeThread(m_pServerHandle, (LPDWORD)&dwThreadExitCode))
			{
				TerminateThread(m_pServerHandle, dwThreadExitCode);
			}
		}
		m_pServerHandle = NULL;
	}
	
	return bStart;
}

DWORD WINAPI CServer_Tcp::ServerThreadProc( LPVOID lplvoid )
{
	CServer_Tcp *pServer = (CServer_Tcp*)lplvoid;

	if(NULL != pServer)
	{
		fd_set fdRead;
		int nError = 0;
		int nRecvLen = 0;
		int unCurClientPort;
		CString strCurClientIp;
		CString strMsg;
		SOCKET pClientSock = SOCKET_ERROR;
		struct sockaddr_in sockAddrtag;
		int unSockaddLen = sizeof(sockAddrtag);
		ClientInfoTag tagClient;
		char czRecv[MAX_RECV_BUF];
		while(pServer->m_bServerRun)
		{
			try
			{
				FD_ZERO(&fdRead);
				fdRead = pServer->m_fdSocket;

				if(0 < select(0, &fdRead, NULL, NULL, NULL))
				{
					for (int i = 0; i < (int)pServer->m_fdSocket.fd_count; i++)
					{
						if(FD_ISSET(pServer->m_fdSocket.fd_array[i], &fdRead))
						{
							if(pServer->m_pServer == pServer->m_fdSocket.fd_array[i])
							{
								pClientSock = SOCKET_ERROR;

								memset(&sockAddrtag, 0, sizeof(sockaddr_in));
								unSockaddLen = sizeof(sockaddr_in);

								pClientSock = accept(pServer->m_pServer, (struct sockaddr*)&sockAddrtag, &unSockaddLen);

								if(INVALID_SOCKET == pClientSock)
									continue;

								memset(&sockAddrtag, 0, sizeof(sockaddr_in));
								unSockaddLen = sizeof(sockaddr_in);

								getpeername(pClientSock, (sockaddr*)&sockAddrtag, &unSockaddLen);
								strCurClientIp = CString(inet_ntoa(sockAddrtag.sin_addr));
								unCurClientPort = ntohs(sockAddrtag.sin_port);
								
								FD_SET(pClientSock, &pServer->m_fdSocket);
								tagClient.strClientIp = strCurClientIp;
								tagClient.nClinetPort = unCurClientPort;
								pServer->m_Lock.Lock();
								pServer->m_ClientArray.Add(tagClient);
								pServer->m_Lock.Unlock();
								strMsg.Format(_T("服务端接收连接:%s<%d>"), strCurClientIp, unCurClientPort);
								SendMessage(MACHINE.m_FramHwnd,WM_COMM_INFO_MESSAGE,0,(LPARAM)strMsg.GetBuffer());
								strMsg.ReleaseBuffer();
							}
							else
							{
								memset(czRecv, 0, sizeof(char)*MAX_RECV_BUF);

								nRecvLen = recv(pServer->m_fdSocket.fd_array[i], czRecv, MAX_RECV_BUF-1, 0);

								if(nRecvLen == SOCKET_ERROR)
								{
									nError = WSAGetLastError();
									closesocket(pServer->m_fdSocket.fd_array[i]);
									FD_CLR(pServer->m_fdSocket.fd_array[i], &pServer->m_fdSocket);
									pServer->m_Lock.Lock();
									strMsg.Format(_T("断开客户端：%s<%d>"), pServer->m_ClientArray[i-1].strClientIp, pServer->m_ClientArray[i-1].nClinetPort);
									pServer->m_ClientArray.RemoveAt(i - 1);
									pServer->m_Lock.Unlock();
									SendMessage(MACHINE.m_FramHwnd,WM_COMM_INFO_MESSAGE,0,(LPARAM)strMsg.GetBuffer());
									strMsg.ReleaseBuffer();
								}

								if(nRecvLen > 0)
								{
									// 处理数据
									memset(pServer->m_pczRecvBuf[i - 1], 0, sizeof(char)*MAX_RECV_BUF);
									memcpy(pServer->m_pczRecvBuf[i - 1], czRecv, nRecvLen);
								}

								if(nRecvLen == 0)
								{
									nError = WSAGetLastError();

									closesocket(pServer->m_fdSocket.fd_array[i]);
									FD_CLR(pServer->m_fdSocket.fd_array[i], &pServer->m_fdSocket);
									pServer->m_Lock.Lock();
									strMsg.Format(_T("断开客户端：%s<%d>"), pServer->m_ClientArray[i-1].strClientIp, pServer->m_ClientArray[i-1].nClinetPort);
									pServer->m_ClientArray.RemoveAt(i - 1);
									pServer->m_Lock.Unlock();
									SendMessage(MACHINE.m_FramHwnd,WM_COMM_INFO_MESSAGE,0,(LPARAM)strMsg.GetBuffer());
									strMsg.ReleaseBuffer();
								}
							}
						}
					}
				}
			}
			catch (...)
			{}
			Sleep(1);
		}
		pServer->m_pServerHandle = NULL;
	}
	return 0;
}

void CServer_Tcp::StopServer()
{
	DWORD dwThreadExitCode = 0;
	m_bServerRun = FALSE;
	if(m_pServerHandle != NULL)
	{
		WaitForSingleObject(m_pServerHandle, 100);
		if(GetExitCodeThread(m_pServerHandle, (LPDWORD)&dwThreadExitCode))
		{
			TerminateThread(m_pServerHandle, dwThreadExitCode);
		}
	}
	m_pServerHandle = NULL;
}

BOOL CServer_Tcp::SendBuf(const int nCurClient, const char *czbuf, const int nBufVaildLen, int *pError)
{
	*pError = 0;
	if(czbuf == NULL || nBufVaildLen <= 0 || nBufVaildLen >= MAX_SEND_BUF)
	{
		*pError = -1;
		return FALSE;
	}

	int unSendLen = send(m_fdSocket.fd_array[nCurClient], czbuf, nBufVaildLen, 0);

	if(unSendLen != nBufVaildLen)
	{
		*pError = WSAGetLastError();

		if(*pError == WSAENETRESET 
			|| *pError == WSAENOTCONN 
			|| *pError == WSAENOTSOCK 
			|| *pError == WSAESHUTDOWN 
			|| *pError == WSAECONNABORTED 
			|| *pError == WSAETIMEDOUT 
			|| *pError == WSAECONNRESET)
		{
			CString strMsg;

			closesocket(m_fdSocket.fd_array[nCurClient]);
			FD_CLR(m_fdSocket.fd_array[nCurClient], &m_fdSocket);
			m_Lock.Lock();
			strMsg.Format(_T("断开客户端：%s<%d>"), m_ClientArray[nCurClient].strClientIp, m_ClientArray[nCurClient].nClinetPort);
			m_ClientArray.RemoveAt(nCurClient);
			m_Lock.Unlock();
			SendMessage(MACHINE.m_FramHwnd,WM_COMM_INFO_MESSAGE,0,(LPARAM)strMsg.GetBuffer());
			strMsg.ReleaseBuffer();
		}
	}
	return (unSendLen == nBufVaildLen);
}