#include "StdAfx.h"
#include "HmsLaser_Tcp.h"


CHmsLaser_Tcp::CHmsLaser_Tcp(void)
{
	m_pSocket = INVALID_SOCKET;
}


CHmsLaser_Tcp::~CHmsLaser_Tcp(void)
{
	DisConnectServer();
}

BOOL CHmsLaser_Tcp::ConnectServer( char* cIP, int nPort )
{
	WSADATA wsaData;
	WORD sockVersion = MAKEWORD(2, 0);
	
	if (0 != WSAStartup(sockVersion, &wsaData))
	{
		return FALSE;
	}

	m_pSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if ( m_pSocket == INVALID_SOCKET )
	{
		m_pSocket = NULL;
		return FALSE;
	}

	m_sockAddr.sin_family = AF_INET;
	m_sockAddr.sin_port = htons(nPort);
	m_sockAddr.sin_addr.s_addr = inet_addr(cIP);
	if ( m_sockAddr.sin_addr.s_addr == INADDR_NONE )
	{
		return FALSE;
	}

	if ( SOCKET_ERROR == connect(m_pSocket, (struct sockaddr *)&m_sockAddr, sizeof(m_sockAddr)) )
	{
		m_pSocket = NULL;
		return FALSE;
	}

	return TRUE;
}

BOOL CHmsLaser_Tcp::DisConnectServer()
{
	if(INVALID_SOCKET != m_pSocket)
	{
		shutdown(m_pSocket, SD_BOTH);
		closesocket(m_pSocket);
	}
	m_pSocket = INVALID_SOCKET;
	return TRUE;
}

BOOL CHmsLaser_Tcp::IsConnectServer()
{
	return (m_pSocket != INVALID_SOCKET && m_pSocket != NULL);
}

BOOL CHmsLaser_Tcp::LaserMark( char* cEntityName, BOOL bIsBlock/*=FALSE*/ )
{
	BOOL bResult = FALSE;
	char byRecvData[10];
	int nNetTimeout = 3000;

	try
	{
		if(IsConnectServer())
		{
			CString strSend;
			CStringA szSend;
			if(cEntityName == NULL || CString(cEntityName).GetLength() == 0)
			{
				strSend.Format(_T("_LaserMark(%s,%d);"),  _T(""),  (int)bIsBlock);
			}
			else
			{
				strSend.Format(_T("_LaserMark(%s,%d);"), CString(cEntityName), (int)bIsBlock);
			}
			szSend = CStringA(strSend);

			int nRet = send(m_pSocket, szSend.GetBuffer(), szSend.GetLength(), 0);

			szSend.ReleaseBuffer();

			if ( nRet != szSend.GetLength() || nRet == SOCKET_ERROR )
			{
				return FALSE;
			}

			setsockopt(m_pSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));
			
			memset(byRecvData, 0, sizeof(char)*10);

			nRet = recv(m_pSocket, (char *)byRecvData, SOCK_RECVBUF, 0);
			
			if(nRet == 0 || nRet == SOCKET_ERROR)
			{
				return FALSE;
			}

			bResult = (BOOL)byRecvData[0];
		}
	}
	catch (...)
	{
		bResult = FALSE;
	}
	return bResult;
}

BOOL CHmsLaser_Tcp::StopMark()
{
	BOOL bResult = FALSE;
	char byRecvData[10];
	int nNetTimeout = 3000;

	try
	{
		if(IsConnectServer())
		{
			CString strSend;
			CStringA szSend;
			strSend = _T("_StopMark();");

			szSend = CStringA(strSend);

			int nRet = send(m_pSocket, szSend.GetBuffer(), szSend.GetLength(), 0);

			szSend.ReleaseBuffer();

			if ( nRet != szSend.GetLength() || nRet == SOCKET_ERROR )
			{
				return FALSE;
			}

			setsockopt(m_pSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));

			memset(byRecvData, 0, sizeof(char)*10);

			nRet = recv(m_pSocket, (char *)byRecvData, SOCK_RECVBUF, 0);

			if(nRet == 0 || nRet == SOCKET_ERROR)
			{
				return FALSE;
			}

			bResult = (BOOL)byRecvData[0];
		}
	}
	catch (...)
	{
		bResult = FALSE;
	}

	return bResult;
}

BOOL CHmsLaser_Tcp::SetEntityText( char* cEntityName, char* cText )
{
	int nNetTimeout = 3000;
	BOOL bResult = FALSE;
	char byRecvData[10];

	try
	{
		if(IsConnectServer())
		{
			CString strSend;
			CStringA szSend;

			strSend.Format(_T("_SetEntityText(%s,%s);"), CString(cEntityName), CString(cText));
			szSend = CStringA(strSend);

			int nRet = send(m_pSocket, szSend.GetBuffer(), szSend.GetLength(), 0);

			szSend.ReleaseBuffer();

			if ( nRet != szSend.GetLength() || nRet == SOCKET_ERROR)
			{
				return FALSE;
			}

			setsockopt(m_pSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));

			memset(byRecvData, 0, sizeof(char)*10);

			nRet = recv(m_pSocket, (char *)byRecvData, SOCK_RECVBUF, 0);

			if(nRet == 0 || nRet == SOCKET_ERROR)
			{
				return FALSE;
			}

			bResult = (BOOL)byRecvData[0];
		}
	}
	catch(...)
	{
		bResult = FALSE;
	}
	return bResult;
}

BOOL CHmsLaser_Tcp::SetEntityTextOnly( char* cEntityName, char* cText )
{
	int nNetTimeout = 3000;
	BOOL bResult = FALSE;
	char byRecvData[10];

	try
	{
		if(IsConnectServer())
		{
			CString strSend;
			CStringA szSend;

			strSend.Format(_T("_SetEntityTextOnly(%s,%s);"), CString(cEntityName), CString(cText));
			szSend = CStringA(strSend);

			int nRet = send(m_pSocket, szSend.GetBuffer(), szSend.GetLength(), 0);
			szSend.ReleaseBuffer();

			if ( nRet != szSend.GetLength() || nRet == SOCKET_ERROR)
			{
				return FALSE;
			}

			setsockopt(m_pSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));

			memset(byRecvData, 0, sizeof(char)*10);

			nRet = recv(m_pSocket, (char *)byRecvData, SOCK_RECVBUF, 0);

			if(nRet == 0 || nRet == SOCKET_ERROR)
			{
				return FALSE;
			}

			bResult = (BOOL)byRecvData[0];
		}
	}
	catch(...)
	{
		bResult = FALSE;
	}
	return bResult;
}

BOOL CHmsLaser_Tcp::ReDraw()
{
	int nNetTimeout = 3000;
	BOOL bResult = FALSE;
	char byRecvData[10];

	try
	{
		if(IsConnectServer())
		{
			CString strSend;
			CStringA szSend;

			strSend = _T("_ReDraw();");
			szSend = CStringA(strSend);

			int nRet = send(m_pSocket, szSend.GetBuffer(), szSend.GetLength(), 0);
			szSend.ReleaseBuffer();

			if ( nRet != szSend.GetLength() || nRet == SOCKET_ERROR)
			{
				return FALSE;
			}

			setsockopt(m_pSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));

			memset(byRecvData, 0, sizeof(char)*10);

			nRet = recv(m_pSocket, (char *)byRecvData, SOCK_RECVBUF, 0);

			if(nRet == 0 || nRet == SOCKET_ERROR)
			{
				return FALSE;
			}

			bResult = (BOOL)byRecvData[0];
		}
	}
	catch(...)
	{
		bResult = FALSE;
	}
	return bResult;
}

BOOL CHmsLaser_Tcp::SetEntityPos( char* cEntityName, double dX, double dY, BOOL bIsAbsolute /*= FALSE*/ )
{
	int nNetTimeout = 3000;
	BOOL bResult = FALSE;
	char byRecvData[10];

	try
	{
		if(IsConnectServer())
		{
			CString strSend;
			CStringA szSend;

			strSend.Format(_T("_SetEntityPos(%s,%f,%f,%d);"), CString(cEntityName), dX, dY, (int)bIsAbsolute);
			szSend = CStringA(strSend);

			int nRet = send(m_pSocket, szSend.GetBuffer(), szSend.GetLength(), 0);

			szSend.ReleaseBuffer();

			if ( nRet != szSend.GetLength() || nRet == SOCKET_ERROR)
			{
				return FALSE;
			}

			setsockopt(m_pSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));

			memset(byRecvData, 0, sizeof(char)*10);

			nRet = recv(m_pSocket, (char *)byRecvData, SOCK_RECVBUF, 0);

			if(nRet == 0 || nRet == SOCKET_ERROR)
			{
				return FALSE;
			}

			bResult = (BOOL)byRecvData[0];
		}
	}
	catch(...)
	{
		bResult = FALSE;
	}

	return bResult;
}

BOOL CHmsLaser_Tcp::SetEntityRotate( char* cEntityName, double dAngle )
{
	int nNetTimeout = 3000;
	BOOL bResult = FALSE;
	char byRecvData[10];
	try
	{
		if(IsConnectServer())
		{
			CString strSend;
			CStringA szSend;

			strSend.Format(_T("_SetEntityRotate(%s,%f);"), CString(cEntityName), dAngle);
			szSend = CStringA(strSend);

			int nRet = send(m_pSocket, szSend.GetBuffer(), szSend.GetLength(), 0);
			szSend.ReleaseBuffer();

			if ( nRet != szSend.GetLength() || nRet == SOCKET_ERROR)
			{
				return FALSE;
			}

			setsockopt(m_pSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));

			memset(byRecvData, 0, sizeof(char)*10);

			nRet = recv(m_pSocket, (char *)byRecvData, SOCK_RECVBUF, 0);

			if(nRet == 0 || nRet == SOCKET_ERROR)
			{
				return FALSE;
			}

			bResult = (BOOL)byRecvData[0];
		}
	}
	catch(...)
	{
		bResult = FALSE;
	}

	return bResult;
}

BOOL CHmsLaser_Tcp::SetCurDocByName( char* cDocName )
{
	int nNetTimeout = 3000;
	BOOL bResult = FALSE;
	char byRecvData[10];
	try
	{
		if(IsConnectServer())
		{
			CString strSend;
			CStringA szSend;
			strSend.Format(_T("_SetCurDocByName(%s);"), CString(cDocName));
			szSend = CStringA(strSend);

			int nRet = send(m_pSocket, szSend.GetBuffer(), szSend.GetLength(), 0);
			szSend.ReleaseBuffer();

			if ( nRet != szSend.GetLength() || nRet == SOCKET_ERROR)
			{
				return FALSE;
			}

			setsockopt(m_pSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));

			memset(byRecvData, 0, sizeof(char)*10);

			nRet = recv(m_pSocket, (char *)byRecvData, SOCK_RECVBUF, 0);

			if(nRet == 0 || nRet == SOCKET_ERROR)
			{
				return FALSE;
			}

			bResult = (BOOL)byRecvData[0];
		}
	}
	catch(...)
	{
		bResult = FALSE;
	}

	return bResult;
}

char CHmsLaser_Tcp::GetIOInput()
{
	int nNetTimeout = 3000;
	BOOL bResult = FALSE;
	char byRecvData[10];

	try
	{
		if(IsConnectServer())
		{
			CString strSend;
			CStringA szSend;

			strSend = _T("_GetIOInput();");
			szSend = CStringA(strSend);

			int nRet = send(m_pSocket, szSend.GetBuffer(), szSend.GetLength(), 0);
			szSend.ReleaseBuffer();

			if ( nRet != szSend.GetLength() || nRet == SOCKET_ERROR)
			{
				return FALSE;
			}

			setsockopt(m_pSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));

			memset(byRecvData, 0, sizeof(char)*10);

			nRet = recv(m_pSocket, (char *)byRecvData, SOCK_RECVBUF, 0);

			if(nRet == 0 || nRet == SOCKET_ERROR)
			{
				return FALSE;
			}

			bResult = (BOOL)byRecvData[0];
		}
	}
	catch (...)
	{
		bResult = FALSE;
	}
	
	return bResult;
}

BOOL CHmsLaser_Tcp::SetIOOutput( char btOutPutIOVal )
{
	int nNetTimeout = 3000;
	BOOL bResult = FALSE;
	char byRecvData[10];

	try
	{
		if(IsConnectServer())
		{
			CString strSend;
			CStringA szSend;

			strSend.Format(_T("_SetIOOutput(%d);"),  (int)btOutPutIOVal);
			szSend = CStringA(strSend);

			int nRet = send(m_pSocket, szSend.GetBuffer(), szSend.GetLength(), 0);
			szSend.ReleaseBuffer();

			if ( nRet != szSend.GetLength() || nRet == SOCKET_ERROR)
			{
				return FALSE;
			}

			setsockopt(m_pSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));

			memset(byRecvData, 0, sizeof(char)*10);

			nRet = recv(m_pSocket, (char *)byRecvData, SOCK_RECVBUF, 0);

			if(nRet == 0 || nRet == SOCKET_ERROR)
			{
				return FALSE;
			}

			bResult = (BOOL)byRecvData[0];
		}
	}
	catch(...)
	{
		bResult = FALSE;
	}

	return bResult;
}

BOOL CHmsLaser_Tcp::SetIsMarkByName( char* cEntityName, BOOL bIsMark )
{
	int nNetTimeout = 3000;
	BOOL bResult = FALSE;
	char byRecvData[10];

	try
	{
		if(IsConnectServer())
		{
			CString strSend;
			CStringA szSend;

			strSend.Format(_T("_SetIsMarkByName(%s, %d);"), CString(cEntityName),(int)bIsMark);
			szSend = CStringA(strSend);

			int nRet = send(m_pSocket, szSend.GetBuffer(), szSend.GetLength(), 0);
			szSend.ReleaseBuffer();

			if ( nRet != szSend.GetLength() || nRet == SOCKET_ERROR)
			{
				return FALSE;
			}

			setsockopt(m_pSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));

			memset(byRecvData, 0, sizeof(char)*10);

			nRet = recv(m_pSocket, (char *)byRecvData, SOCK_RECVBUF, 0);

			if(nRet == 0 || nRet == SOCKET_ERROR)
			{
				return FALSE;
			}

			bResult = (BOOL)byRecvData[0];
		}
	}
	catch(...)
	{
		bResult = FALSE;
	}

	return bResult;
}

BOOL CHmsLaser_Tcp::IsMarking()
{
	int nNetTimeout = 3000;
	BOOL bResult = FALSE;
	char byRecvData[10];

	try
	{
		if(IsConnectServer())
		{
			CString strSend;
			CStringA szSend;

			strSend = _T("_IsMarking();");
			szSend = CStringA(strSend);

			int nRet = send(m_pSocket, szSend.GetBuffer(), szSend.GetLength(), 0);
			szSend.ReleaseBuffer();

			if ( nRet != szSend.GetLength() || nRet == SOCKET_ERROR)
			{
				return FALSE;
			}

			setsockopt(m_pSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));

			memset(byRecvData, 0, sizeof(char)*10);

			nRet = recv(m_pSocket, (char *)byRecvData, SOCK_RECVBUF, 0);

			if(nRet == 0 || nRet == SOCKET_ERROR)
			{
				return FALSE;
			}

			bResult = (BOOL)byRecvData[0];
		}
	}
	catch(...)
	{
		bResult = FALSE;
	}

	return bResult;
}

BOOL CHmsLaser_Tcp::MoveRedToDot( double dX, double dY )
{
	int nNetTimeout = 3000;
	BOOL bResult = FALSE;
	char byRecvData[10];
	try
	{
		if(IsConnectServer())
		{
			CString strSend;
			CStringA szSend;

			strSend.Format(_T("_MoveRedToDot(%f,%f);"), dX, dY);
			szSend = CStringA(strSend);

			int nRet = send(m_pSocket, szSend.GetBuffer(), szSend.GetLength(), 0);
			szSend.ReleaseBuffer();

			if ( nRet != szSend.GetLength() || nRet == SOCKET_ERROR)
			{
				return FALSE;
			}

			setsockopt(m_pSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));

			memset(byRecvData, 0, sizeof(char)*10);

			nRet = recv(m_pSocket, (char *)byRecvData, SOCK_RECVBUF, 0);

			if(nRet == 0 || nRet == SOCKET_ERROR)
			{
				return FALSE;
			}

			bResult = (BOOL)byRecvData[0];
		}
	}
	catch(...)
	{
		bResult = FALSE;
	}

	return bResult;
}

BOOL CHmsLaser_Tcp::SetEntitySize( char* cEntityName, double dWidth, double dHeight )
{
	int nNetTimeout = 3000;
	BOOL bResult = FALSE;
	char byRecvData[10];
	try
	{
		if(IsConnectServer())
		{
			CString strSend;
			CStringA szSend;

			strSend.Format(_T("_SetEntitySize(%s,%f,%f);"), CString(cEntityName), dWidth, dHeight);
			szSend = CStringA(strSend);

			int nRet = send(m_pSocket, szSend.GetBuffer(), szSend.GetLength(), 0);
			szSend.ReleaseBuffer();

			if ( nRet != szSend.GetLength() || nRet == SOCKET_ERROR)
			{
				return FALSE;
			}

			setsockopt(m_pSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));

			memset(byRecvData, 0, sizeof(char)*10);

			nRet = recv(m_pSocket, (char *)byRecvData, SOCK_RECVBUF, 0);

			if(nRet == 0 || nRet == SOCKET_ERROR)
			{
				return FALSE;
			}

			bResult = (BOOL)byRecvData[0];
		}
	}
	catch(...)
	{
		bResult = FALSE;
	}

	return bResult;
}

BOOL CHmsLaser_Tcp::OpenFile( char * cFileName )
{
	int nNetTimeout = 3000;
	BOOL bResult = FALSE;
	char byRecvData[10];

	try
	{
		if(IsConnectServer())
		{
			CString strSend;
			CStringA szSend;

			strSend.Format(_T("_OpenFile(%s);"), CString(cFileName));
			szSend = CStringA(strSend);

			int nRet = send(m_pSocket, szSend.GetBuffer(), szSend.GetLength(), 0);
			szSend.ReleaseBuffer();

			if ( nRet != szSend.GetLength() || nRet == SOCKET_ERROR)
			{
				return FALSE;
			}

			setsockopt(m_pSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));

			memset(byRecvData, 0, sizeof(char)*10);

			nRet = recv(m_pSocket, (char *)byRecvData, SOCK_RECVBUF, 0);

			if(nRet == 0 || nRet == SOCKET_ERROR)
			{
				return FALSE;
			}

			bResult = (BOOL)byRecvData[0];
		}
	}
	catch(...)
	{
		bResult = FALSE;
	}

	return bResult;
}

BOOL CHmsLaser_Tcp::LoadFile( char * cFilePath )
{
	int nNetTimeout = 3000;
	BOOL bResult = FALSE;
	char byRecvData[10];

	try
	{
		if(IsConnectServer())
		{
			CString strSend;
			CStringA szSend;

			strSend.Format(_T("_LoadFile(%s);"), CString(cFilePath));
			szSend = CStringA(strSend);

			int nRet = send(m_pSocket, szSend.GetBuffer(), szSend.GetLength(), 0);
			szSend.ReleaseBuffer();

			if ( nRet != szSend.GetLength() || nRet == SOCKET_ERROR)
			{
				return FALSE;
			}

			setsockopt(m_pSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));

			memset(byRecvData, 0, sizeof(char)*10);

			nRet = recv(m_pSocket, (char *)byRecvData, SOCK_RECVBUF, 0);

			if(nRet == 0 || nRet == SOCKET_ERROR)
			{
				return FALSE;
			}

			bResult = (BOOL)byRecvData[0];
		}
	}
	catch(...)
	{
		bResult = FALSE;
	}

	return bResult;
}

BOOL CHmsLaser_Tcp::ShowEditWindow( BOOL bIsVisible )
{
	int nNetTimeout = 3000;
	BOOL bResult = FALSE;
	char byRecvData[10];

	try
	{
		if(IsConnectServer())
		{
			CString strSend;
			CStringA szSend;

			strSend.Format(_T("_ShowEditWindow(%d);"), (int)bIsVisible);
			szSend = CStringA(strSend);

			int nRet = send(m_pSocket, szSend.GetBuffer(), szSend.GetLength(), 0);
			szSend.ReleaseBuffer();

			if ( nRet != szSend.GetLength() || nRet == SOCKET_ERROR)
			{
				return FALSE;
			}

			setsockopt(m_pSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));

			memset(byRecvData, 0, sizeof(char)*10);

			nRet = recv(m_pSocket, (char *)byRecvData, SOCK_RECVBUF, 0);

			if(nRet == 0 || nRet == SOCKET_ERROR)
			{
				return FALSE;
			}

			bResult = (BOOL)byRecvData[0];
		}
	}
	catch(...)
	{
		bResult = FALSE;
	}

	return bResult;
}

BOOL CHmsLaser_Tcp::SaveFile( char * cFilePath )
{
	int nNetTimeout = 3000;
	BOOL bResult = FALSE;
	char byRecvData[10];
	try
	{
		if(IsConnectServer())
		{
			CString strSend;
			CStringA szSend;

			strSend.Format(_T("_SaveFile(%s);"), CString(cFilePath));
			szSend = CStringA(strSend);

			int nRet = send(m_pSocket, szSend.GetBuffer(), szSend.GetLength(), 0);
			szSend.ReleaseBuffer();

			if ( nRet != szSend.GetLength() || nRet == SOCKET_ERROR)
			{
				return FALSE;
			}

			setsockopt(m_pSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));

			memset(byRecvData, 0, sizeof(char)*10);

			nRet = recv(m_pSocket, (char *)byRecvData, SOCK_RECVBUF, 0);

			if(nRet == 0 || nRet == SOCKET_ERROR)
			{
				return FALSE;
			}

			bResult = (BOOL)byRecvData[0];
		}
	}
	catch(...)
	{
		bResult = FALSE;
	}

	return bResult;
}