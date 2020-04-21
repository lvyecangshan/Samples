#pragma once

#include <vector>
using namespace std;

struct sMODULE
{
	CString m_strName;
	int m_nInputs;
    int m_nOutputs;
    int m_nMotors;
    vector<int> m_pInputs;
    vector<int> m_pOutputs;
    vector<int> m_pMotors;
};

class  CModuleManager
{
// singulator class, constructor not public
public:
    CModuleManager();
	~CModuleManager();
public:
	__declspec(dllexport) static CModuleManager * GetInstance();
	__declspec(dllexport) sMODULE &  operator[] (int nID);
    __declspec(dllexport) int GetNumModule() { return m_nModules; }
	__declspec(dllexport) BOOL Initialize(CString strFile);
protected:
	static CModuleManager *m_pInstance;
	vector<sMODULE> m_pModules;
	CString m_strError;
	int m_nModules;
    int m_nCurModule;
public:
	static void Destroy();
	
	int GetCurModule() { return m_nCurModule; }
    void SetCurModule(int nModule) { m_nCurModule = nModule; }
	CString GetErrorMessage() { return m_strError; }
};

// global instance of modules
#define MODULES (*CModuleManager::GetInstance())