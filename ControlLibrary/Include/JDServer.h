#pragma once
#include "JDSockDef.h"
#include "JDTcpServer.h"

class __declspec(dllexport) CJDServer
{
public:
	CJDServer(void);
	~CJDServer(void);
public:
	void  JD_Create(int iPort);
	void  JD_Destroy();
	static CJDServer * GetInstance();
	CJDTcpServer m_JDTcpServer;
	HWND m_hWndMain;
	CRunCtrl *m_pRunCtrl;
protected:
	static CJDServer *m_pInstance;
	BOOL m_bCreate;

};
#define JDSERVER (*CJDServer::GetInstance())