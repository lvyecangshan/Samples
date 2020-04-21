#pragma once
#include <vector>
#include <map>
using namespace std;

#define DATABASE_MDB _T("Log\\machine.mdb")
#define MODULEEX_CFG	 _T("System\\ModuleEx.cfg")
#define PARAMETER_CFG     _T("System\\Parameter.cfg")
#define WM_SHOW_MESSAGE_EX	WM_USER + 102

class __declspec(dllexport) CListHead
{
public:
	CListHead()
	{
		m_strName  = _T("");
		m_strCName = _T("");
		m_iNameLen = 60;
	}
	CString m_strName;
	CString m_strCName;
	int     m_iNameLen;
};
class __declspec(dllexport) CModuleCT
{
public:
	CModuleCT();
	CModuleCT(int nModuleId,CString strModuleName,CString strProductSN,CString strStartTime,CString strEndTime,double dlWorkTime)
	{
		m_nModuleId = nModuleId;
		m_strModuleName = strModuleName;
		m_strProductSN = strProductSN;
		m_strStartTime = strStartTime;
		m_strEndTime = strEndTime;
		m_dlWorkTime = dlWorkTime;
	}
public:
	int		m_nModuleId;
	CString m_strModuleName;
	CString m_strProductSN;
	CString m_strStartTime;
	CString m_strEndTime;
	double  m_dlWorkTime;
};
class __declspec(dllexport) CStepCT
{
public:
	CStepCT();
	CStepCT(int nModuleId,CString strModuleName,CString strProductSN,int nCurStep,CString strStartTime,CString strEndTime,double dlWorkTime)
	{
		m_nModuleId = nModuleId;
		m_strModuleName = strModuleName;
		m_strProductSN = strProductSN;
		m_nCurStep = nCurStep;
		m_strStartTime = strStartTime;
		m_strEndTime = strEndTime;
		m_dlWorkTime = dlWorkTime;
	}
public:
	int		m_nModuleId;
	CString m_strModuleName;
	CString m_strProductSN;
	int		m_nCurStep;
	CString m_strStartTime;
	CString m_strEndTime;
	double  m_dlWorkTime;
};
class __declspec(dllexport) CAlarmInfo
{
public:
	CAlarmInfo();
	CAlarmInfo(int nAlarmId,CString strAlarmInfo,int nAlarmType,int nModuleId,CString strModuleName,CString strProductSN)
	{
		m_nAlarmId = nAlarmId;
		m_strAlarmInfo = strAlarmInfo;
		m_nAlarmType = nAlarmType;
		m_nModuleId = nModuleId;
		m_strModuleName = strModuleName;
		m_strProductSN = strProductSN;
	}
public:
	int		m_nAlarmId;
	CString m_strAlarmInfo;
	int		m_nAlarmType;
	int		m_nModuleId;
	CString m_strModuleName;
	CString m_strProductSN;
};
class __declspec(dllexport) CYieldInfo
{
public:
	CYieldInfo();
	CYieldInfo(int nYieldType,CString strYieldInfo,int nModuleId,CString strModuleName,CString strProductSN,int nAmount)
	{
		m_nYieldType = nYieldType;
		m_strYieldInfo = strYieldInfo;
		m_nModuleId = nModuleId;
		m_strModuleName = strModuleName;
		m_strProductSN = strProductSN;
		m_nAmount = nAmount;
	}
public:
	int		m_nYieldType;
	CString m_strYieldInfo;
	int		m_nModuleId;
	CString m_strModuleName;
	CString m_strProductSN;
	int		m_nAmount;
};

class __declspec(dllexport) CMotorPos
{
public:
	/*CMotorPos()
	{
	m_nIndex = -1;
	m_strName = _T("");
	m_dlPos = 0.0;
	}*/
	CMotorPos(int nIndex= -1,CString strName = _T(""),double dlPos= 0.0)
	{
		m_nIndex = nIndex;
		m_strName = strName;
		m_dlPos = dlPos;
	}
public:
	int m_nIndex;
	CString m_strName;
	double m_dlPos;
};

class __declspec(dllexport) CMotorRun
{
public:
	CMotorRun(int nIndex = -1,CString strName = _T(""))
	{
		m_nIndex = nIndex;
		m_strName = strName;
	}
public:
	int m_nIndex;
	CString m_strName;
	vector<CMotorPos> m_vecMotorPos;
};
class __declspec(dllexport) CPkgPara
{
public:
	CPkgPara()
	{
	}
	virtual ~CPkgPara()
	{
	}
public:
	virtual void Init(){return;}
	map<void*,CString> m_VoidMap;
};
class __declspec(dllexport) CProduct
{
public:
	/*CProduct()
	{
	m_nIndex = -1;
	m_strName = _T("");
	m_PkgParameter = NULL;
	}*/
	virtual ~CProduct()
	{
		
    }
	CProduct(int nIndex = -1,CString strName = _T(""),CString strScheme = _T(""),BOOL bNewPkg = FALSE)
	{
		m_nIndex = nIndex;
		m_strName = strName;
		m_strScheme = strScheme;
		if (bNewPkg == TRUE)
		{
			m_PkgParameter = NULL;
		}
	m_PkgParameter = NULL;
	}
public:
	int m_nIndex;
	CString m_strName;
	CString m_strScheme; 
	CPkgPara *m_PkgParameter;
	vector<CMotorRun> m_vecMotorRun;
};

class __declspec(dllexport) CProductEX
{
public:
	virtual ~CProductEX()
	{
		if (m_MachineParameter != NULL)
		{
			delete m_MachineParameter;
		}
    }
	CProductEX(int nIndex = -1,CString strName = _T(""),CString strScheme = _T(""))
	{
		m_nIndex = nIndex;
		m_strName = strName;
		m_strScheme = strScheme;
		m_MachineParameter = NULL;
	}
public:
	int m_nIndex;
	CString m_strName;
	CString m_strScheme; 
	CPkgPara *m_MachineParameter;
	//map<void*,CString> m_VoidMap;
};
struct Point3D {   
	double x;   
	double y;   
	double z;   
}; 

enum USER_PERMISSION_PROTECT_TYPE
{
	USER_PERMISSION_MOTOR_POS_SAVE = 0,
	USER_PERMISSION_MOTOR_PARAMTER,
	NUM_USER_PERMISSION_PROTECT_TYPE,
};