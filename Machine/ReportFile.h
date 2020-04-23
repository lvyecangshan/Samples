#pragma once

#include <locale>
#import "C:\Program Files\Common Files\System\ado\msado15.dll" \
	no_namespace \
	rename("EOF","adoEOF") rename("DataTypeEnum","adoDataTypeEnum") \
	rename("FieldAttributeEnum", "adoFielAttributeEnum") rename("EditModeEnum", "adoEditModeEnum") \
	rename("LockTypeEnum", "adoLockTypeEnum") rename("RecordStatusEnum", "adoRecordStatusEnum") \
	rename("ParameterDirectionEnum", "adoParameterDirectionEnum")


typedef struct _NG_CLASS
{
	CString strNgName;			// 不良品名称
	int nNgCount;				// 不良品数量
}ngClass;


typedef struct _MODULE_CLASS
{
	CString strModuleName;		// 模组名称描述
	DWORD dwModuleTime;		// 模组运动时间，单位：秒
}moduleClass;


typedef struct _ACTION_CLASS
{
	CString strActionName;		// 单动作名称描述
	DWORD dwActionTime;			// 单动作运动时间，单位：毫秒
	CTime DateTime;		// 时间
}actionClass;


typedef struct _FAULT_DURATION_CLASS
{
	CString strDate[1024];		// 日期
	int nDay;					// 天数

	CString strDuration[4096];			// 报警时长	
}faultDurationClass;


typedef struct _LAST_FAULT
{
	CString strInfo;
	CString strDate;

	_LAST_FAULT()
	{
		strInfo = _T("");
		strDate = _T("");
	}
}lastFault;


typedef struct _FAULT_COUNT_CLASS
{
	CString strDate[1024];		// 日期
	CString strCount[4096];			// 报警次数
	int nDay;					// 天数
}faultCountClass;


typedef struct _CT_FUNCTION
{
	int nActionCount;
	CString strLastStepName;
	int nLastStep;
	DWORD dwCTStartTime;
	DWORD dwModuleCT;

	_CT_FUNCTION()
	{
		nActionCount = 0;
		nLastStep = -1;
		dwCTStartTime = 0;
		dwModuleCT = 0;
	}
}CTFunction;


typedef struct _PROD_DATA
{
	UINT nOkTotal;
	UINT nNgTotal;

	DWORD dwRunTime;
	DWORD dwProdTime;
	DWORD dwFreeTime;
	DWORD dwFaultTime;

	DWORD dwFlagTime_Run;
	DWORD dwFlagTime_Prod;
	DWORD dwFlagTime_Free;
	DWORD dwFlagTime_Fault;

	_PROD_DATA()
	{
		nOkTotal = 0;
		nNgTotal = 0;

		dwRunTime = 0;
		dwProdTime = 0;
		dwFreeTime = 0;
		dwFaultTime = 0;

		dwFlagTime_Run = 0;
		dwFlagTime_Prod = 0;
		dwFlagTime_Free = 0;
		dwFlagTime_Fault = 0;
	}
}prodData;


typedef enum _ENUM_RESULT
{
	rst_success = 0,
	rst_noinit,
	rst_connnull,
	rst_nofault,
}enumResult;


class CReportFile
{
private:
	enumResult UpdateProdRecord(DWORD nRunTime, DWORD nProdTime, DWORD nFreeTime, DWORD nFaultTime, int nOkTotal, int nNgTotal);

	_ConnectionPtr m_connPtr;

	prodData m_ProdData;
	int m_nLastDay;
	lastFault m_LastFault;

public:
	CReportFile(void);
	~CReportFile(void);

	enumResult Initial(_ConnectionPtr connPtr);

	void WriteCSV(CString FileName,CString strText);

	// 保存生产信息的函数，文件类型是CSV
	BOOL WriteProdInfo(CString strPath, CString strFileName, CString strDate, CString strRunTime, CString strProdTime, CString strFreeTime, CString strFaultTime, CString strProdCount, CString strOkCount, CString strNgCount, CString strYield, CString strPPM, CString strFaultRate);

	// 保存模组CT信息的函数，文件类型是CSV
	BOOL WriteModultCT(CString strPath, CString strFileName, moduleClass* modules, int nModuleCount);

	// 保存模组单动作CT的函数，文件类型是CSV
	BOOL WriteActionCT(CString strPath, CString strFileName, actionClass* actions, int nActionCount);

	// 保存单次报警记录，文件类型是CSV
	BOOL WriteSingleFault(CString strPath, CString strFileName, CString strID, CString strFault, CString strType, CString strModule, CString strDateTime, CString strDuration);

	// 保存每个报警时长（一定时间段内）和统计的报警数据，文件类型是CSV
	BOOL WriteFaultDurationAndTotal(CString strPath, CString strFileName, faultDurationClass* faultClass, int nFaultCount);

	// 保存每个报警次数（一定时间段内）和统计的报警数据，文件类型是CSV
	BOOL WriteFaultCountAndTotal(CString strPath, CString strFileName, faultCountClass* faultClass, int nFaultCount);
	
	enumResult UpdateModuleCTRecord(CTime time, CString strModuleName, int nProdDuration);

	enumResult UpdateStepCTRecord(CTime time, CString strModuleName, actionClass* act, int nActCount);

	enumResult UpdateAlmRecord(CTime time, int nMsgID, CString strInfo, int nType, int nModuleID, CString strModuleName);

	enumResult UpdateAlmDuration();

	void MeasureCT(CString strModuleName, CString strStepName, int nStepNum, int nStartStep, int nEndStep, int nStepTotal, int* waitStep, int nWaitStepTotal, int* LastStep, int* ActionCount, actionClass* action, DWORD* CTStartTime, DWORD* ModuleCT);

	UINT OkCount();
	UINT NgCount();

	DWORD RunTimeCount();
	DWORD ProdTimeCount();
	DWORD FreeTimeCount();
	DWORD FaultTimeCount();

	void FlagTime();

	void ResetOkCount();
	void ResetNgCount();
	void ResetRunTime();
	void ResetProdTime();
	void ResetFreeTime();
	void ResetFaultTime();

	void GetDate(CTime startTime, CTime endTime, CTime* timeDate, int* nDateCount);

	void ClearData();

	CString MillisecToTime(DWORD dwTime);

	void QueryAlmDuration(CListCtrl* ListInfo, COleDateTime dtStartData, COleDateTime dtStartTime, COleDateTime dtEndData, COleDateTime dtEndTime);
	void QueryAlmCount(CListCtrl* ListInfo, COleDateTime dtStartData, COleDateTime dtStartTime, COleDateTime dtEndData, COleDateTime dtEndTime);
	void QueryModuleCT(CListCtrl* ListInfo, COleDateTime dtStartData, COleDateTime dtStartTime, COleDateTime dtEndData, COleDateTime dtEndTime, CComboBox* cbModule);
	void QueryStepCT(CListCtrl* ListInfo, COleDateTime dtStartData, COleDateTime dtStartTime, COleDateTime dtEndData, COleDateTime dtEndTime, CString strModuleName);

	void SaveAllReport(CListCtrl* ListInfo, CComboBox* cbModule, CComboBox* cbAlmReport);
};

