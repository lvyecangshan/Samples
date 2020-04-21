#pragma once

#include "Tray.h"

#define TRAY_USER_MAP	0
#define MAX_NUM_TRAYS	32

class __declspec(dllexport) CTrayManager
{
protected:
	CTrayManager(void);

public:
	virtual ~CTrayManager(void);

	void Initialize(int numTrays, int numRow, int numCol,const CString *pNames = NULL);

	CTray & operator[] (int iTrayID);

	static CTrayManager * GetInstance();
	static void Destroy();

	void ShowTrayMapping();
	int GetTrayCount() { return m_numTrays; }
	BOOL AddTray(int numRow,int numCol,CString strName);
	BOOL AddTray(CTray &tray);
	
	BOOL DeleteTray(int num);
	BOOL DeleteTray(CString strName);
protected:
	static CTrayManager *m_pInstance;
    CTray	*m_ppTrays[MAX_NUM_TRAYS];
	int		m_numTrays;
};

#define TRAYS (*CTrayManager::GetInstance())