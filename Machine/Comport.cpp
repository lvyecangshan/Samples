#include "StdAfx.h"
#include "ComPort.h"

CComPort::CComPort(void)
{
	m_iPort = -1;
	m_bOpen = FALSE;
	m_hCom = (HANDLE)-1;
	m_dwBaudRate = CBR_115200;
	m_btStopBits = ONESTOPBIT;
	m_btByteSize = 8;
	m_btParity = NOPARITY;
}

CComPort::~CComPort(void)
{
	m_bOpen = FALSE;
}

BOOL CComPort::OpenPort(int iPort,DWORD dwBaudRate)
{
	if (m_bOpen)
	{
		return TRUE;
	}

	if(iPort > 0)
	{
		m_iPort = iPort;
	}

	if(dwBaudRate > CBR_110)
	{
		m_dwBaudRate = dwBaudRate;
	}

	CString strCmd;
	strCmd.Format(_T("Com%d"),m_iPort);
	m_hCom = CreateFile(strCmd,GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);
	if (m_hCom == (HANDLE)-1)
	{
		CString strErr;
		strErr.Format(_T("´ò¿ª´®¿Ú%dÊ§°Ü"),m_iPort);
		return FALSE;
	}
	else
	{
		DCB wdcb;
		COMMTIMEOUTS comTime;

		GetCommState(m_hCom,&wdcb);
		wdcb.BaudRate = m_dwBaudRate;
		wdcb.StopBits =m_btStopBits;
		wdcb.ByteSize = (BYTE)m_btByteSize;
		wdcb.fParity = FALSE;
		wdcb.Parity = m_btParity;
		SetCommState(m_hCom,&wdcb);

		LPCOMMTIMEOUTS lpcomTimeouts = &comTime;
		GetCommTimeouts(m_hCom,lpcomTimeouts);
		lpcomTimeouts->ReadIntervalTimeout = MAXDWORD;
		lpcomTimeouts->ReadTotalTimeoutConstant = 0;
		lpcomTimeouts->ReadTotalTimeoutMultiplier = 0;
		SetCommTimeouts(m_hCom,lpcomTimeouts);
		PurgeComm(m_hCom,PURGE_TXCLEAR);
	}
	m_bOpen = TRUE;
	return TRUE;
}

BOOL CComPort::Read(CString &strResult,int iSleepTime)
{
	if(!m_bOpen)
	{
		return FALSE;
	}
	DWORD dwCount = 0;
	char szCmd[2] = {0x02,0x03};
	char szResult[READ_LIMIT+1];
	PurgeComm(m_hCom,PURGE_TXCLEAR|PURGE_RXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
	Sleep(100);
	if(!WriteFile(m_hCom,&szCmd,sizeof(szCmd),&dwCount,NULL))
	{
		return FALSE;
	}
	Sleep(iSleepTime);
	memset(szResult,0,READ_LIMIT+1);
	dwCount = 0;
	BOOL bReadOk = ReadFile(m_hCom,szResult,READ_LIMIT,&dwCount,NULL);
	if(!bReadOk || dwCount < 10)
	{
		return FALSE;
	}
	strResult = CString((CStringA)szResult);
	strResult = strResult.TrimRight(_T("\r\t"));
	return TRUE;
}

BOOL CComPort::ClsoePort()
{
	if (m_bOpen)
	{
		if (m_hCom)
		{
			CloseHandle(m_hCom);
			m_hCom = NULL;
		}
		m_bOpen = FALSE;
	}
	return TRUE;
}