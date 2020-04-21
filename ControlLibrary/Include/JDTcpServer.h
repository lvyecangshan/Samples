#pragma once
#include "JDSockDef.h"
#include "JDClientMgr.h"

class __declspec(dllexport) CJDTcpServer:public CJDThread
{
public:
	CJDTcpServer(void);
	~CJDTcpServer(void);

	void Create(int iPort);
	void Destroy();

	SOCKET m_ServerSocket;

	virtual void RunInWhile();
	
	BOOL SendOperateToStation(char* szStationId, int iState, int iTransCode);
	BOOL SendOperateToManager(char* szMsg, int iSize);
	CJDClientMgr m_ClientMgr;
};
