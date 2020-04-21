#pragma once
#include "JDSockDef.h"

class __declspec(dllexport) CJDAcceptClient:public CJDThread
{
public:
	CJDAcceptClient(SOCKET socket, char* szAddress);
	~CJDAcceptClient(void);

	SOCKET m_socket;

	virtual void RunInWhile();

	BOOL Send(char* szBuf, int iSize);
	int Recv(char* szBuf, int iSize, int iFlags = 0);
	void HandleMessage(char* szBuf);
	BOOL SendOperateToStation(char* szStationId, int iState, int iTransCode);
	BOOL SendOperateToManager(char* szMsg, int iSize);
	int m_iClientType;
	char m_szAddress[JD_ADDRESS_SIZE];
	int m_iRunState;
	int m_iKeepAliveTime;
	char m_szStationId[JD_STATION_ID_SIZE];
	char m_szStationName[JD_STATION_NAME_SIZE];

	CString m_strMsg;
};
