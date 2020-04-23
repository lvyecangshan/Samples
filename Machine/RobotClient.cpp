#include "StdAfx.h"
#include "RobotClient.h"
#include <string.h>

CRobotClient::CRobotClient(void)
{
	memset(m_szReceive, 0, sizeof(m_szReceive));
	m_strRecvResult.Empty();
	m_bEnableReceive = FALSE;
}

CRobotClient::~CRobotClient(void)
{
	Disconnect();
}

BOOL CRobotClient::Connect( char* szIp,int iPort )
{
	if (m_ClientSocket.IsConnect())
	{
		return TRUE;
	}
	ReleaseClientThread();
	if (m_ClientSocket.Connect(szIp,iPort))
	{
		InitClientThread();
		CString strConnect;
		strConnect.Format(_T("%s<%d>: 连接机器人成功"),m_strRobotIP, m_nRobotPort);
		return TRUE;
	}
	return FALSE;
}

BOOL CRobotClient::Connect( const CString strIP,int iPort )
{
	Disconnect();
	int iLen = strIP.GetLength();
	if (iLen < 8 || iPort < 1)
	{
		return FALSE;
	}
	char szIp[256];
	memset(szIp,0,256);
	for (int i = 0; i < iLen; i++)
	{
		szIp[i] = (char)strIP.GetAt(i);
	}
	m_strRobotIP = strIP;
	m_nRobotPort = iPort;
	return Connect(szIp,iPort);
}

void CRobotClient::Disconnect()
{
	m_ClientSocket.Disconnect();
	ReleaseClientThread();
	CString strConnect;
	strConnect.Format(_T("%s<%d>: 断开机器人连接"),m_strRobotIP, m_nRobotPort);
}

BOOL CRobotClient::SendToDevice( CString strSend)
{
	char szSend[256];
	CStringA czSend(strSend);
	if (strSend.GetLength() < 0 ) return FALSE;
	
	memset(szSend,0,256);
	int nSendLength = czSend.GetLength();
	memcpy(szSend, czSend.GetBuffer(),nSendLength );
	czSend.ReleaseBuffer();

	m_strRecvResult.Empty();
	
	BOOL bSend = m_ClientSocket.Send(szSend,nSendLength);

	return bSend;
}

BOOL CRobotClient::SendToDeviceAndWait(int g_Cmd)
{
	BOOL bSend = FALSE, bRes = FALSE;

	ASSERT(ROBOT_HIGH_SPEED <= g_Cmd && g_Cmd < ALL_ROBOT_POS 
		&& g_robotcmd[g_Cmd].unCmdIndex == g_Cmd);

	m_strRecvResult.Empty();

	bSend = SendToDevice(g_robotcmd[g_Cmd].strSendCmd);

	DWORD dwTime = GetTickCount();

	while(bSend && GetTickCount() - dwTime < 40000)
	{
		if (GetReceiveResult(g_Cmd))
		{
			bRes = TRUE;
			break;
		}
		Sleep(1);
	}
	return bRes;
}

BOOL CRobotClient::GetReceiveResult(int g_Cmd)
{
	ASSERT(ROBOT_HIGH_SPEED <= g_Cmd && g_Cmd < ALL_ROBOT_POS 
		&& g_robotcmd[g_Cmd].unCmdIndex == g_Cmd);

	return (m_strRecvResult.Compare(g_robotcmd[g_Cmd].strRecvCmd) == 0);
}

void CRobotClient::RunWhile()
{
	if (!m_ClientSocket.IsConnect())
	{
		m_bRunning = FALSE;
		return;
	}

	memset(m_szReceive, 0, sizeof(m_szReceive));
	if (!m_ClientSocket.Recv(m_szReceive, sizeof(m_szReceive)))
	{
		return;
	}
	m_strRecvResult = m_szReceive;
}