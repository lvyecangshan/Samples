#pragma once

#include "Evt.h"
#include <vector>
using namespace std;

class  CEventManager
{
public:
	CEventManager();
	~CEventManager();

public:
	__declspec(dllexport) static CEventManager * GetInstance();
	__declspec(dllexport) CEvt * operator[] (int nID);
	__declspec(dllexport) int GetNumEvents() { return m_nEvents; }

protected:
	static CEventManager *m_pInstance;
    CString		m_strError;
    int			m_nEvents;
	vector<CEvt*> m_ppEvts;

public:
	static void Destroy();
	BOOL Initialize(int nEvents);
    CString GetErrorMessage() { return m_strError; }
};

#define EVENTS (*CEventManager::GetInstance())