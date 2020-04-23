#include "StdAfx.h"
#include "VisionClient.h"


CVisionClient::CVisionClient(void)
{
	m_strReceive.Empty();
	m_bRecvEnable = FALSE;
	memset(m_szReceive,0,sizeof(char)*256);
}


CVisionClient::~CVisionClient(void)
{
	Disconnect();
}

BOOL CVisionClient::Connect( char* szIp,int iPort )
{
	CString strConnect;

	if (m_pSocket.IsConnect())
	{
		return TRUE;
	}
	ReleaseClientThread();
	if (m_pSocket.Connect(szIp,iPort))
	{
		InitClientThread();
		strConnect.Format(_T("%s<%d>: 连接成功"),m_strIP,m_nPort);
		SendMessage(MACHINE.m_FramHwnd,WM_COMM_INFO_MESSAGE,0,(LPARAM)strConnect.GetBuffer());
		return TRUE;
	}
	else
	{
		strConnect.Format(_T("%s<%d>: 连接失败"),m_strIP,m_nPort);
		SendMessage(MACHINE.m_FramHwnd,WM_COMM_INFO_MESSAGE,0,(LPARAM)strConnect.GetBuffer());
	}
	return FALSE;
}

BOOL CVisionClient::Connect(const CString & strIP,int iPort )
{
	char szIp[256];

	int iLen = strIP.GetLength();
	if (iLen < 8 || iPort < 1)
	{
		return FALSE;
	}
	
	memset(szIp, 0, sizeof(char)*256);

	for (int i = 0; i < iLen; i++)
	{
		szIp[i] = (char)strIP.GetAt(i);
	}

	m_nPort = iPort;
	m_strIP = strIP;

	return Connect(szIp,iPort);
}

void CVisionClient::Disconnect()
{
	CString strConnect;
	m_pSocket.Disconnect();
	ReleaseClientThread();
	strConnect.Format(_T("%s<%d>:断开连接"),m_strIP, m_nPort);
	SendMessage(MACHINE.m_FramHwnd, WM_COMM_INFO_MESSAGE, 0, (LPARAM)strConnect.GetBuffer());
}

BOOL CVisionClient::Send( char* szBuf,int iBufSize)
{
	if (!m_pSocket.IsConnect())
	{
		return FALSE;
	}
	if (m_pSocket.Send(szBuf,iBufSize))
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CVisionClient::SendToDevice(CString strSend)
{
	CString strX, strMessage;
	char szSend[256];

	int iLen = strSend.GetLength();
	if (iLen < 0 )
	{
		return FALSE;
	}
	memset(szSend,0,256);
	for (int i = 0;i < iLen;i++)
	{
		szSend[i] = (char)strSend.GetAt(i);
	}
	
	strMessage.Format(_T("SendTo  %s<%d>: "),m_strIP,m_nPort);
	for (int i = 0;i < 7;i++)
	{
		strX.Format(_T(" %02X"),szSend[i]);
		strMessage += strX;
	}

	SendMessage(MACHINE.m_FramHwnd, WM_COMM_INFO_MESSAGE, 0, (LPARAM)strMessage.GetBuffer());

	ClearReceive();

	if (Send(szSend,iLen))
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CVisionClient::SendAndWait(CString srcSend)
{
	BOOL bEnable = m_bRecvEnable;
	m_bRecvEnable = TRUE;
	if (!SendToDevice(srcSend +_T("\r\n")))
	{
		Disconnect();
		Connect(m_strIP, m_nPort);
		m_bRecvEnable = bEnable;
		return FALSE;
	}
	DWORD dwTime = GetTickCount();
	while(1)
	{
		if (GetReceiveResult(srcSend))
		{
			break;
		}
		if (GetTickCount() - dwTime > 20000)
		{
			m_bRecvEnable = bEnable;
			return FALSE;
		}
		Sleep(1);
	}
	m_bRecvEnable = bEnable;
	return TRUE;
}

BOOL CVisionClient::GetReceiveResult(CString strCmd)
{
	CString strResult =_T("");
	strResult =	strCmd +_T("_OK");
	if(strResult == m_strReceive)
	{
		m_VisionLock.Lock();
		m_strReceive =_T("");
		m_VisionLock.Unlock();
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void CVisionClient::ClearReceive()
{
	m_VisionLock.Lock();
	m_strReceive =_T("");
	m_VisionLock.Unlock();
}

void CVisionClient::WriteLog(CString strLog)
{
	SYSTEMTIME CurTime;

	CString strTime,strPath,strRecord;
	
	GetLocalTime(&CurTime);
	
	strTime.Format(_T("Vision-%d-%d-%d.log"),CurTime.wYear,CurTime.wMonth,CurTime.wDay);

	strPath = GetAbsolutePathName(CUSTOMER_TRACE_LOG) +strTime;

	strTime.Format(_T("%d-%d-%d %d:%d:%d : "),CurTime.wYear, CurTime.wMonth, CurTime.wDay, CurTime.wHour, CurTime.wMinute, CurTime.wSecond);

	strRecord = strTime + strLog;

	MACHINE.WriteLog(strPath,strRecord);
}

void CVisionClient::RunWhile()
{
	if (!m_pSocket.IsConnect())
	{
		m_bRunning = FALSE;
		return;
	}
	char szBuf[256];
	memset(szBuf, 0, sizeof(char)*256);
	memset(m_szReceive, 0 ,sizeof(char)*256);

	if (!m_pSocket.Recv(szBuf,sizeof(szBuf)))
	{
		return;
	}
	m_VisionLock.Lock();
	memcpy(m_szReceive, szBuf, sizeof(szBuf));
	m_strReceive = m_szReceive;
	m_VisionLock.Unlock();

	CString strReve, strTempReve;
	strReve.Empty();

	for(int i = 0; i < sizeof(m_szReceive); i++)
	{
		strTempReve.Format(_T("%c"),m_szReceive[i]);
		strReve +=strTempReve;
	}
	CString strLog;
	strLog.Format(_T("Receive %s<%d>: %s"),m_strIP, m_nPort, strReve);
	WriteLog(strLog);

	SendMessage(MACHINE.m_FramHwnd,WM_COMM_INFO_MESSAGE,0,(LPARAM)strLog.GetBuffer());
}