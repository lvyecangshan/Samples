#include "StdAfx.h"
#include "NetSenseClient.h"


CNetSenseClient::CNetSenseClient(void)
{
	WSADATA wsadata;
	m_pSocket = INVALID_SOCKET;
	WSAStartup(MAKEWORD(2,2), &wsadata);
}


CNetSenseClient::~CNetSenseClient(void)
{
	Disconnect();
}

BOOL CNetSenseClient::Connect( CString strIP, int nProt )
{
	if(IsConnect())
	{
		return TRUE;
	}
	int iLen = strIP.GetLength();
	if (iLen < 8 || nProt < 1)
	{
		return FALSE;
	}
	char szIp[256];
	memset(szIp, 0, 256);
	for (int i = 0; i < iLen; i++)
	{
		szIp[i] = (char)strIP.GetAt(i);
	}

	m_pSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if ( m_pSocket == INVALID_SOCKET )
	{
		return FALSE;
	}

	m_sockAddr.sin_family = AF_INET;
	m_sockAddr.sin_port = htons(nProt);
	m_sockAddr.sin_addr.s_addr = inet_addr(szIp);
	if ( m_sockAddr.sin_addr.s_addr == INADDR_NONE )
	{
		return FALSE;
	}

	if ( SOCKET_ERROR == connect(m_pSocket, (struct sockaddr *)&m_sockAddr, sizeof(m_sockAddr)) )
	{
		DWORD dwError = GetLastError();
		m_pSocket = INVALID_SOCKET;
		return FALSE;
	}

	return TRUE;
}

BOOL CNetSenseClient::IsConnect()
{
	if (INVALID_SOCKET != m_pSocket)
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CNetSenseClient::Disconnect()
{
	shutdown(m_pSocket, SD_BOTH);
	closesocket(m_pSocket);
	m_pSocket = INVALID_SOCKET;

	return TRUE;
}

unsigned int CNetSenseClient::CRC_Check(unsigned char *data, unsigned char length)
{
	int j;
	unsigned int reg_crc = 0xFFFF;
	while(length--)
	{
		reg_crc ^= *data++;
		for (j = 0; j < 8; j++)
		{
			if(reg_crc & 0x01)
				reg_crc = (reg_crc >> 1) ^ 0xa001;
			else
				reg_crc = reg_crc >> 1;
		}
	}
	return reg_crc;
}

BOOL CNetSenseClient::GetPressValue( unsigned int nSite, float * pfPressValue )
{
	ASSERT(nSite < 6 && pfPressValue != NULL);
	m_PressLock.Lock();
	int nNetTimeout = 10000;//超时时长
	int nRecvLen = 0;
	unsigned char czCmd[10];
	unsigned char czRrecv[15];
	memset(czCmd, 0, sizeof(unsigned char)*10);
	memset(czRrecv, 0, sizeof(unsigned char)*15);

	czCmd[0] = nSite;
	czCmd[1] = 0x04;
	czCmd[2] = czCmd[3] = czCmd[4] = 0x00;
	czCmd[5] = 0x02;

	unsigned int nCrc = CRC_Check(czCmd, 6);

	czCmd[6] = nCrc & 0x00FF;
	czCmd[7] = nCrc >> 8;


	int st = send(m_pSocket, (char*)czCmd, 8, 0);

	if ( st != 8 || st == SOCKET_ERROR )
	{
		m_PressLock.Unlock();
		return FALSE;
	}

	setsockopt(m_pSocket,SOL_SOCKET,SO_RCVTIMEO,(char *)&nNetTimeout,sizeof(int));
	// 接收
	nRecvLen = recv(m_pSocket, (char*)czRrecv, sizeof(char)*15, 0);
	if ( 0 == nRecvLen || SOCKET_ERROR == nRecvLen )
	{
		m_PressLock.Unlock();
		return FALSE;
	}

	// 命令头不符
	if(czCmd[0] != czRrecv[0] || czCmd[1] != czRrecv[1])
	{
		m_PressLock.Unlock();
		return FALSE;
	}

	//  数据长度不符
	if(nRecvLen != 3 + 2 + czRrecv[2])
	{
		m_PressLock.Unlock();
		return FALSE;
	}

	// 验证校验
	nCrc = CRC_Check(czRrecv, nRecvLen - 2);
	unsigned int unRecvCrc = (czRrecv[7] | (czRrecv[8] << 8));
	if(unRecvCrc != nCrc)
	{
		m_PressLock.Unlock();
		return FALSE;
	}

	union
	{
		unsigned int unValue;
		float fValue;
	} unTemp;

	unTemp.unValue = ((czRrecv[3] << 24) | (czRrecv[4] << 16) | (czRrecv[5] << 8) | (czRrecv[6]));

	*pfPressValue = unTemp.fValue;

	m_PressLock.Unlock();
	return TRUE;
}

BOOL CNetSenseClient::GetHipotValue(char * cmd, double * dValue)
{
	m_HipotLock.Lock();
	int nSendLen = 0;
	int nRecvLen = 0;
	int nNetTimeout = 1000;//超时时长
	char chRecv[100] = {0};
	CString strSend = _T("");
	CString strRecv = _T("");

	strSend = cmd;
	// 发送
	Sleep(100);//欧姆龙两指令之间间隔2ms以上
	nSendLen = send(m_pSocket, (char *)cmd, strSend.GetLength(), 0);

	if ( nSendLen < 4 || nSendLen == SOCKET_ERROR )
	{
		m_HipotLock.Unlock();
		return FALSE;
	}

	setsockopt(m_pSocket,SOL_SOCKET,SO_RCVTIMEO,(char *)&nNetTimeout,sizeof(int));

	DWORD dwTimeOut = GetTickCount();

	// 接收
	while((GetTickCount() - dwTimeOut) < 22000)
	{
		int nState = MACHINE.m_pRunCtrl->GetMcState();
		if (nState != MC_RUNNING && nState != MC_INITIALIZING)
		{
			nRecvLen = 0;
			break;
		}

		memset(chRecv,0,100);

		nRecvLen = recv(m_pSocket, chRecv, sizeof(chRecv), 0);

		if (nRecvLen > 0)
		{
			break;
		}

		Sleep(10);
	}
	
	if ( 0 == nRecvLen || SOCKET_ERROR == nRecvLen )
	{
		m_HipotLock.Unlock();
		return FALSE;
	}

	strRecv = chRecv;

	int nIndex = -1;

	if (strSend.Find(_T("STARt")) > -1)
	{
		nIndex = strRecv.Find(_T("E"));
		if (nIndex == -1)
		{
			m_HipotLock.Unlock();
			return FALSE;
		}

		strRecv = strRecv.Left(nIndex);
	}

	*dValue = _wtof(strRecv);

	m_HipotLock.Unlock();
	return TRUE;
}

BOOL CNetSenseClient::ReadCode(char * cmd, CString &strValue)
{
	m_CodeLock.Lock();
	int nSendLen = 0;
	int nRecvLen = 0;
	int nNetTimeout = 1000;//超时时长
	char chRecv[100] = {0};
	CString strSend = _T("");
	CString strRecv = _T("");

	strSend = cmd;


	Sleep(100);
	nSendLen = send(m_pSocket, (char *)cmd, strSend.GetLength(), 0);

	if ( nSendLen < strSend.GetLength() || nSendLen == SOCKET_ERROR )
	{
		m_CodeLock.Unlock();
		return FALSE;
	}

	setsockopt(m_pSocket,SOL_SOCKET,SO_RCVTIMEO,(char *)&nNetTimeout,sizeof(int));

	memset(chRecv,0,100);

	nRecvLen = recv(m_pSocket, chRecv, sizeof(chRecv), 0);

	if ( 0 == nRecvLen || SOCKET_ERROR == nRecvLen )
	{
		m_CodeLock.Unlock();
		return FALSE;
	}

	strValue = chRecv;
	
	m_CodeLock.Unlock();

	strValue.Trim();
	if(strValue.CompareNoCase(_T("NoRead")) == 0)
	{
		return FALSE;
	}
	return TRUE;
}