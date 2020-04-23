/********************************************************************
	created：2018/03/30
	file base：Server_Tcp.h   Server_Tcp.cpp
	author：menghui
	purpose:：
	1.解决开启服务端数量少，不能多发多收的问题。
	修改记录：
	2018/03/29：编写完成，目前做了最基本的功能，后续可根据使用自行增加接口。
	2018/03/30：将原本由两个线程负责接收连接以及数据发送修改为一个线程负责
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
	*	czIp：创建服务端的IP；当czIp不为NULL时表示以czIp创建服务端，后续只接受该IP地址下的连接。
	*			 当czIp为NULL时表示以端口创建服务端，接收任何IP地址下特定nPort端口的连接
	*	nPort：创建服务端的端口号
	*	pErrorCode：创建服务端失败后返回错误代码（WSAGetLastError获取），成功为0；
	*	创建Socket服务端，成功返回TRUE；失败返回FALSE，检查pErrorCode。
	*/
	BOOL CreateServer(char *czIp, unsigned int nPort, int* pErrorCode);

	/*	BOOL StartServer()
	*	开启服务端，用于接受客户端的连接以及数据
	*	成功返回TRUE，失败返回FALSE。
	*/
	BOOL StartServer();

	/*	void StopServer()
	*	停止服务端
	*	需要再次开启服务端则要调用StartServer重新开启
	*/
	void StopServer();
	
private:
	/*	BOOL InitWinSock(int *pError)
	*	进行相应的socket库绑定
	*	成功pError为0返回TRUE；
	*	失败返回FALSE，失败信息具体查看pError。
	*/
	BOOL InitWinSock(int *pError);

	/*BOOL SendBuf(const int nCurClient, const char *czbuf, const int nBufVaildLen)
	*	nCurClient：要接收数据的客户端在服务端m_fdSocket集合中的编号
	*	czbuf：需发送的数据
	*	nBufVaildLen：数据长度
	*	失败错误代码
	*	成功返回TRUE，失败返回FALSE
	*/
	BOOL SendBuf(const int nCurClient, const char *czbuf, const int nBufVaildLen, int *pError);

	/*	static DWORD WINAPI ServerThreadProc(LPVOID lplvoid)
	*	接收客户端连接以及数据
	*/
	static DWORD WINAPI ServerThreadProc(LPVOID lplvoid);

private:
	BOOL		m_bSocketAvailable;
	char*		m_pczRecvBuf[FD_SETSIZE];
	SOCKET	m_pServer;
	// 服务端Socket和连接的客户端Socket集合
	fd_set		m_fdSocket;

	BOOL      m_bServerRun;
	HANDLE  m_pServerHandle;
	DWORD m_dwServerThreadID;

	CArray<ClientInfoTag> m_ClientArray;

	CCriticalSection m_Lock;
};