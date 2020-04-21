#pragma once

class  CMachineTimeManager
{
public:
	CMachineTimeManager(void);
	~CMachineTimeManager(void);

public:
	__declspec(dllexport) DWORD & operator[] (int nID);
	__declspec(dllexport) static CMachineTimeManager * GetInstance();
	__declspec(dllexport) int GetNumTimes() { return m_nTime; }
	__declspec(dllexport) CString GetName(int nIndex);
    __declspec(dllexport) BOOL	RefreshTimer();
	__declspec(dllexport) BOOL	Save(void);
public:
	CStringArray m_astrNames;
protected:
	DWORD	*m_pTime;
	int		m_nTime;
	static CMachineTimeManager *m_pInstance;
public:
	static void Destroy();
	BOOL	Initialize(int nTimes);
};

#define MACHINE_TIME (*CMachineTimeManager::GetInstance())