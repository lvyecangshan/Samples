#pragma once

#include "JDSockDef.h"
#include "JDAcceptClient.h"

#include <map>
using namespace std;

#define MAP_CLIENT std::map<SOCKET, CJDAcceptClient*>
#define ITER_CLIENT std::map<SOCKET, CJDAcceptClient*>::iterator

class __declspec(dllexport) CJDClientMgr:public CJDThread
{
public:
	CJDClientMgr(void);
	~CJDClientMgr(void);

	void DeleteAllClient();

	void Add(int iSocket);
	virtual void RunInWhile();
	BOOL SendOperateToStation(char* szStationId, int iState, int iTransCode);
	BOOL SendOperateToManager(char* szMsg, int iSize);
private:
	CJDCS m_csMapClient;
	MAP_CLIENT m_mapClient;
};
