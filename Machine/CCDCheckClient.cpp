#include "StdAfx.h"
#include "CCDCheckClient.h"

CCCDCheckClient::CCCDCheckClient(void)
{
	memset(m_szReceive,0,SIZE_BUF);
	m_strReceive = _T("");
}

CCCDCheckClient::~CCCDCheckClient(void)
{
	Disconnect();
}

BOOL CCCDCheckClient::IsConnect()
{
	return m_pCCDClientSocket.IsConnect();
}

BOOL CCCDCheckClient::Connect( char* szIp,int iPort )
{
	if (IsConnect())
	{
		Disconnect();
	}

	if (m_pCCDClientSocket.Connect(szIp,iPort))
	{
		InitClientThread();
		return TRUE;
	}
	return FALSE;
}

BOOL CCCDCheckClient::Connect( const CString & strIP,int iPort)
{
	int iLen = strIP.GetLength();
	if (iLen < 8 || iPort < 1)
	{
		return FALSE;
	}
	char szIp[256];
	memset(szIp,0,256);
	for (int i = 0;i < iLen;i++)
	{
		szIp[i] =(char) strIP.GetAt(i);
	}
	BOOL bConnect = Connect(szIp,iPort);
	return bConnect;
}

void CCCDCheckClient::Disconnect()
{
	m_pCCDClientSocket.Disconnect();
	ReleaseClientThread();
}

void CCCDCheckClient::RunWhile()
{
	if (!IsConnect())
	{
		m_bRunning = FALSE;
		return;
	}

	memset(m_szReceive,0,SIZE_BUF);
	if (!m_pCCDClientSocket.Recv(m_szReceive,sizeof(m_szReceive)))
	{
		return;
	}

	m_strReceive = (CString)(CStringA)m_szReceive;

}

BOOL CCCDCheckClient::Send( char* szBuf,int iBufSize )
{
	if (!IsConnect())	
	{
		return FALSE;
	}

	ASSERT(szBuf != NULL);

	m_pClientSocketLock.Lock();

	m_strReceive.Empty();

	if (m_pCCDClientSocket.Send(szBuf,iBufSize))
	{
		m_pClientSocketLock.Unlock();
		return TRUE;
	}
	m_pClientSocketLock.Unlock();
	return FALSE;
}

BOOL CCCDCheckClient::SendToDevice(CString strCmd)
{
	if (strCmd.IsEmpty())
	{
		strCmd = _T("Mark0\r\n");
	}
	
	int nLen = strCmd.GetLength()* sizeof(TCHAR);

	/*if (nLen < 1)
	{
		return FALSE;
	}
	char szSend[256];
	memset(szSend,0,256);
	for (int i = 0;i < nLen;i++)
	{
		szSend[i] = (char)strCmd.GetAt(i);
	}*/

	USES_CONVERSION;
	if (Send(W2A(strCmd),nLen))
	{
		Sleep(1);
		return TRUE;
	}
	return FALSE;
}

BOOL CCCDCheckClient::SendToDeviceAndWait(CString strCmd,CString& _strRecv)
{
	m_strReceive = _T("");
	_strRecv = _T("");

	if (!SendToDevice(strCmd))
	{
		return FALSE;
	}
	DWORD dwTime = GetTickCount();
	while(1)
	{
		if (m_strReceive.GetLength() > 0)
		{
			break;
		}
		if (GetTickCount()-dwTime > 20000)
		{
			return FALSE;
		}
		Sleep(1);
	}

	_strRecv = m_strReceive;  

	m_strReceive= _T("");

	return TRUE;
}


//BOOL CCCDCheckClient::GetPressValue( unsigned int nSite, float * pfPressValue )
//{
//	ASSERT(nSite < 6 && pfPressValue != NULL);
//	m_PressLock.Lock();
//	int nNetTimeout = 1000;//超时时长
//	int nRecvLen = 0;
//	unsigned char czCmd[10];
//	unsigned char czRrecv[15];
//	memset(czCmd, 0, sizeof(unsigned char)*10);
//	memset(czRrecv, 0, sizeof(unsigned char)*15);
//
//	czCmd[0] = nSite;
//	czCmd[1] = 0x04;
//	czCmd[2] = czCmd[3] = czCmd[4] = 0x00;
//	czCmd[5] = 0x02;
//
//	unsigned int nCrc = CRC_Check(czCmd, 6);
//
//	czCmd[6] = nCrc & 0x00FF;
//	czCmd[7] = nCrc >> 8;
//
//
//	int st = send(m_pSocket, (char*)czCmd, 8, 0);
//
//	if ( st != 8 || st == SOCKET_ERROR )
//	{
//		m_PressLock.Unlock();
//		return FALSE;
//	}
//
//	setsockopt(m_pSocket,SOL_SOCKET,SO_RCVTIMEO,(char *)&nNetTimeout,sizeof(int));
//	// 接收
//	nRecvLen = recv(m_pSocket, (char*)czRrecv, sizeof(char)*15, 0);
//	if ( 0 == nRecvLen || SOCKET_ERROR == nRecvLen )
//	{
//		m_PressLock.Unlock();
//		return FALSE;
//	}
//
//	// 命令头不符
//	if(czCmd[0] != czRrecv[0] || czCmd[1] != czRrecv[1])
//	{
//		m_PressLock.Unlock();
//		return FALSE;
//	}
//
//	//  数据长度不符
//	if(nRecvLen != 3 + 2 + czRrecv[2])
//	{
//		m_PressLock.Unlock();
//		return FALSE;
//	}
//
//	// 验证校验
//	nCrc = CRC_Check(czRrecv, nRecvLen - 2);
//	unsigned int unRecvCrc = (czRrecv[7] | (czRrecv[8] << 8));
//	if(unRecvCrc != nCrc)
//	{
//		m_PressLock.Unlock();
//		return FALSE;
//	}
//
//	union
//	{
//		unsigned int unValue;
//		float fValue;
//	} unTemp;
//
//	unTemp.unValue = ((czRrecv[3] << 24) | (czRrecv[4] << 16) | (czRrecv[5] << 8) | (czRrecv[6]));
//
//	*pfPressValue = unTemp.fValue;
//
//	m_PressLock.Unlock();
//	return TRUE;
//}
//
//unsigned int CCCDCheckClient::CRC_Check(unsigned char *data, unsigned char length)
//{
//	int j;
//	unsigned int reg_crc = 0xFFFF;
//	while(length--)
//	{
//		reg_crc ^= *data++;
//		for (j = 0; j < 8; j++)
//		{
//			if(reg_crc & 0x01)
//				reg_crc = (reg_crc >> 1) ^ 0xa001;
//			else
//				reg_crc = reg_crc >> 1;
//		}
//	}
//	return reg_crc;
//}