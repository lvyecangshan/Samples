// ŷķ���¶Ȼ�ȡģ��

#pragma once
#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>

#define SERIAL_SENDBUF	(100)		// ���ڷ���������Buf��С
#define SERIAL_RECVBUF	(100)		// ���ڷ���������Buf��С

class CNetSenseClient
{
public:
	CNetSenseClient(void);
	~CNetSenseClient(void);

public:
	BOOL Connect(CString strIP, int nProt);				//�򿪴��ڷ�����
	BOOL IsConnect();													//�жϴ��ڷ������Ƿ�����
	BOOL Disconnect();												//�رմ��ڷ�����

	// ѹ��������
	BOOL GetPressValue( unsigned int nSite, float * pfPressValue );

	// ��ȡ��ѹ����ֵ
	BOOL GetHipotValue(char * cmd, double * dValue);

	// ��ȡ��ά��
	
	BOOL ReadCode(char * cmd, CString &strValue);

	unsigned int CRC_Check(unsigned char *data, unsigned char length);

private:
	struct sockaddr_in	m_sockAddr;
	SOCKET m_pSocket;
	CCriticalSection m_PressLock;
	CCriticalSection m_HipotLock;
	CCriticalSection m_CodeLock;
};