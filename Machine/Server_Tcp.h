/********************************************************************
	created��2018/03/30
	file base��Server_Tcp.h   Server_Tcp.cpp
	author��menghui
	purpose:��
	1.�����������������٣����ܶ෢���յ����⡣
	�޸ļ�¼��
	2018/03/29����д��ɣ�Ŀǰ����������Ĺ��ܣ������ɸ���ʹ���������ӽӿڡ�
	2018/03/30����ԭ���������̸߳�����������Լ����ݷ����޸�Ϊһ���̸߳���
			...
*********************************************************************/

#pragma once
#include "WinSock2.h"
#include "WS2tcpip.h"

#define  MAX_RECV_BUF	60
#define  MAX_SEND_BUF	60

typedef struct ClientInfo
{
	CString strClientIp;
	int nClinetPort;
}ClientInfoTag;

class CServer_Tcp
{
public:
	CServer_Tcp(void);
	~CServer_Tcp(void);

public:
	/*	BOOL CreateServer(char *czIp, unsigned int nPort);
	*	czIp����������˵�IP����czIp��ΪNULLʱ��ʾ��czIp��������ˣ�����ֻ���ܸ�IP��ַ�µ����ӡ�
	*			 ��czIpΪNULLʱ��ʾ�Զ˿ڴ�������ˣ������κ�IP��ַ���ض�nPort�˿ڵ�����
	*	nPort����������˵Ķ˿ں�
	*	pErrorCode�����������ʧ�ܺ󷵻ش�����루WSAGetLastError��ȡ�����ɹ�Ϊ0��
	*	����Socket����ˣ��ɹ�����TRUE��ʧ�ܷ���FALSE�����pErrorCode��
	*/
	BOOL CreateServer(char *czIp, unsigned int nPort, int* pErrorCode);

	/*	BOOL StartServer()
	*	��������ˣ����ڽ��ܿͻ��˵������Լ�����
	*	�ɹ�����TRUE��ʧ�ܷ���FALSE��
	*/
	BOOL StartServer();

	/*	void StopServer()
	*	ֹͣ�����
	*	��Ҫ�ٴο����������Ҫ����StartServer���¿���
	*/
	void StopServer();
	
private:
	/*	BOOL InitWinSock(int *pError)
	*	������Ӧ��socket���
	*	�ɹ�pErrorΪ0����TRUE��
	*	ʧ�ܷ���FALSE��ʧ����Ϣ����鿴pError��
	*/
	BOOL InitWinSock(int *pError);

	/*BOOL SendBuf(const int nCurClient, const char *czbuf, const int nBufVaildLen)
	*	nCurClient��Ҫ�������ݵĿͻ����ڷ����m_fdSocket�����еı��
	*	czbuf���跢�͵�����
	*	nBufVaildLen�����ݳ���
	*	ʧ�ܴ������
	*	�ɹ�����TRUE��ʧ�ܷ���FALSE
	*/
	BOOL SendBuf(const int nCurClient, const char *czbuf, const int nBufVaildLen, int *pError);

	/*	static DWORD WINAPI ServerThreadProc(LPVOID lplvoid)
	*	���տͻ��������Լ�����
	*/
	static DWORD WINAPI ServerThreadProc(LPVOID lplvoid);

private:
	BOOL		m_bSocketAvailable;
	char*		m_pczRecvBuf[FD_SETSIZE];
	SOCKET	m_pServer;
	// �����Socket�����ӵĿͻ���Socket����
	fd_set		m_fdSocket;

	BOOL      m_bServerRun;
	HANDLE  m_pServerHandle;
	DWORD m_dwServerThreadID;

	CArray<ClientInfoTag> m_ClientArray;

	CCriticalSection m_Lock;
};