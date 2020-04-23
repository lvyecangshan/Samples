#pragma once
#include "stdafx.h"
#include <Lot.h>
#include <EasyLibdef.h>
#include <CentralControl.h>
#include <GeneralHelper.h>
#include <BLMessageBox.h>
#include <IniHelper.h>
#include <LogWrite.h>
#include <UserManager.h>
#include <DataBaseRecord.h>
#include "UserManageDlg.h"
#include "MachineMonitor.h"
#include "MachineProduct.h"
#include "MachineDef.h"
#include "RunProcess.h"
#include "ReportFile.h"
#include "parts\LineMotor.h"

// 安东
#include "AndongDlg.h"
#include "AndonFeedDlg.h"
// 模组
#include "RunProcessOnloadCore.h"
#include "RunProcessNgLine.h"
#include "RunProcessBatteryCore.h"

#include "RunProcessOnloadShellStack.h"
#include "RunProcessOffloadShellStack.h"
#include "RunProcessFeederTray.h"
#include "RunProcessOnloadShellPNP.h"
#include "RunProcessCleanShellBefor.h"
#include "RunProcessCleanShell.h"
#include "RunProcessCleanShellAfter.h"
#include "RunProcessCarryShellPnP.h"
#include "RunProcessBatteryInShell.h"
#include "RunProcessBatteryInShellOffloadPnP.h"
#include "RunProcessRobot.h"
#include "RunProcessMES.h"
#include "RunProcessReadCodeConveyer.h"
#include "RunProcess\RunProcessCleanOnLoad.h"
#include "RunProcess\RunProcessRunProcessShortEdgeFeeder.h"
#include "RunProcess\RunProcessRunProcessShortEdgeClean.h"
#include "RunProcess\RunProcessRunProcessLongEdgeFeeder.h"
#include "RunProcess\RunProcessRunProcessLongEdgeClean.h"

#include "./TranLine/Mylar/RunProcessElevatorMylar.h"
#include "./TranLine/Mylar/RunProcessDistributorMylarOffLoad1.h"
#include "./TranLine/Mylar/RunProcessDistributorMylarOffLoad2.h"
#include "./TranLine/Mylar/RunProcessOffLoadMylar1.h"
#include "./TranLine/Mylar/RunProcessOffLoadMylar2.h"
#include "./TranLine/Mylar/RunProcessDamperMylarUp03.h"
#include "./TranLine/Mylar/RunProcessDamperMylarUp04.h"
#include "./TranLine/Mylar/RunProcessDamperMylarUp05.h"
#include "./TranLine/Core/RunProcessDistributorCoreDustBefore.h"
#include "./TranLine/Core/RunProcessCoreDust.h"
#include "./TranLine/Core/RunProcessDistributorOnLoadBefore.h"
#include "./TranLine/Core/RunProcessCoreOnLoad1.h"
#include "./TranLine/Core/RunProcessCoreOnLoad2.h"
#include "./TranLine/Core/RunProcessElevatorCore.h"
#include "./TranLine/Core/RunProcessDistributorPlateDustBefore.h"
#include "./TranLine/Core/RunProcessPlateDust.h"
#include "./TranLine/Core/RunProcessDamperCoreDown06.h"
#include "./TranLine/Core/RunProcessDamperCoreDown08.h"
#include "./TranLine/Mylar/RunProcessDamperMylarDwon04.h"
#include "./TranLine/Mylar/RunProcessDamperMylarDwon03.h"
#include "./TranLine/Mylar/RunProcessDistributorMylarOnLoad.h"
#include "./TranLine/Mylar/RunProcessCleanFrontDistributor.h"

//MES
#include "CzMES.h"

enum AndonData
{
	ResourceStatu,
	ResourceAlarm,
	ResourceOutput,
	ResourceLight,
};

class CMachineCtrl : public CCentralControl
{
protected:
	CMachineCtrl();
	//清空模组指针
	void EmptyRunProcessPtr();

public:
	virtual ~CMachineCtrl();

	static CMachineCtrl * GetInstance();

	static void Destroy();

	BOOL Initialize(HWND hMsgProcessWnd);

	// decode the inputs XXXX to coressponding int ID
	int DecodeInputsID(CString strID);

	// decode the outputs YYYY to coressponding int ID
	int DecodeOutputsID(CString strID);

	void UpdateLanguage();

	BOOL CreateMultipleDirectory(const CString& strPath);

	void WriteLog(CString FileName,CString strText);

	CRunProcess* GetRun(CString strModuleName);

	BOOL ReadMachineParamter();

	BOOL WriteModule(int index, CString strName, CArray<int, int> *pInputs, CArray<int, int> *pOutputs, CArray<int, int> *pMotors);

	void ShowMain( CWnd *pParentWnd, CRect rcDesired,CRunProcess * process,BOOL bOperator);

	// MES
	void WriteMesErrCode();
	void ReadMesErrCode();
	void ReadMesData();
	void WriteMesLog(CString strFilePath,CString strText);
	void WriteMesInLog(CString strFilePath,CString strText);
	// 安东
	void AndonDataReport(const int nCurDire, CString strLog);
	void AndonLight(CString StrResourceId, CString StrLight);		// 三色灯
	void AndonMachineStatus(CString StrResourceId,int nMachineStatusCode);  //安东上传机器状态和停机原因
	void AndonMachineAlarms(CString StrResourceId, int nMachineStatusCode,vector<CString> AlarmList); //安东上传报警
	void AndonMachineTotal(CString StrResourceId, int nInputTotal, int nOutputTotal, int nOKTotal, int nNGTotal); //安东上传总量
	
   //mes
	// 其他功能函数
	int CoreCountForClear(BOOL bCountAll); //机台内电芯数量
	void  Release();

	void SetBuzzerStatue(BOOL bOn);
	BOOL DeleteMesInLog();
	BOOL DeleteMesOutLog();
	BOOL DeleteMesData();

protected:
	static CMachineCtrl	*m_pInstance;
	BOOL InitializeRunThreads(HWND hMsgProcessWnd);

private:
	static BOOL BeforeStart();
	static void BeforeStop();

public:
	/////IO////////
	int  m_numInputs;
	int  m_numOutputs;
	int  m_numMotors;
	// 设备灯塔
	int	 m_nTowerRed;
	int	 m_nTowerAmber;
	int	 m_nTowerGreen;
	int	 m_nTowerBuzzer;

	// 设备灯塔
	int m_nRedLight;
	int m_nYellowLight;
	int m_nGreenLight;
	int m_nLighthouseBuzzer;

	int  m_nIStart;
	int  m_nIStop;
	int  m_nIReset;
	int  m_nIAuto;
	int  m_nIManual;
	int  m_nIVacumAlarm;
	int  m_nIAirAlarm;
	int  m_nIDustAirPressAlarm;

	BOOL MachineExit;
	//蜂鸣器
	int m_nLastStation;

	// 急停
	int	 m_IEmgStop;
	// 安全门
	int	 m_ISafeDoor;

public:
	int  m_numModules;

	// 当前方案编号
	int  m_nDefaultProduct;
	// 是否启用方案
	BOOL m_bDefaultEnable;

	// 容器类变量成员
	vector<CProductEX> m_vecProduct;
	CArray<CString> m_strModuleArray;
	CArray<CRunProcess*> m_ArrayStation;
	// 类对象成员

	HWND				m_FramHwnd;
	CWnd*				m_pMachineViewWnd;
	CRunProcess**		m_pModules;
	CUserManager		m_user;
	CMachineProduct		m_product;
	CDataBaseRecord		m_dbRecord;

	CReportFile		m_ReportFile;

	CAndongDlg	m_DlgAndon;
	CAndonFeedDlg	m_DlgAndonFeed;
	
	// 模组指针
	CRunProcessOnloadShellStack*			m_pOnloadShellStack[2];
	CRunProcessOffloadShellStack*			m_pOffloadShellStack;
	CRunProcessFeederTray*					m_pFeederTray;
	CRunProcessOnloadShellPNP*				m_pOnloadShellPnp;
	CRunProcessCleanShellBefor*				m_pCleanBefore;
	CRunProcessCleanShell*					m_pCleanShell;
	CRunProcessCleanShellAfter*				m_pCleanAfter;
	CRunProcessCarryShellPnP*				m_pCarryShellPnp;
	CRunProcessBatteryInShell*				m_pBatteryInShell[2];

	CRunProcessOnloadCore*					m_pOnloadCore;
	CRunProcessNgLine*						m_pBarCodeNgLine;
	CRunProcessBatteryCore*					m_pBatteryCore[2];
	CRunProcessBatteryInShellOffloadPnP*	m_pBatteryOffloadPnp[2];
	CRunProcessRobot*						m_pRobotOffload;
	CRunProcessNgLine*						m_pPressNgLine;
	CRunProcessMES*							m_pUploadMes;
	CRunProcessReadCodeConveyer*			m_pReadCode[2];

	CRunProcessCleanFrontDistributor*       m_pLongCleanFrontDistributor; 
	CRunProcessCleanFrontDistributor*       m_pShortCleanFrontDistributor;  

	CRunProcessElevatorMylar*				m_pElevatorMaylar;				//包膜机升降
	CRunProcessDistributorMylarOffLoad1*    m_pDistributorMylarOffLoad1;	//包膜1上料前分料
	CRunProcessDistributorMylarOffLoad2*    m_pDistributorMylarOffLoad2;	//包膜2上料前分料
	CRunProcessOffLoadMylar1*			    m_pOffLoadMylar1;				//包膜机下料1
	CRunProcessOffLoadMylar2*			    m_pOffLoadMylar2;				//包膜机下料2
	CRunProcessDamperMylarUp03*             m_pDamperMylarUp03;             //包膜3段上层转角阻挡
	CRunProcessDamperMylarUp04*             m_pDamperMylarUp04;             //包膜4段上层阻挡
	CRunProcessDamperMylarUp05*             m_pDamperMylarUp05;             //包膜5段上层阻挡
	CRunProcessDistributorCoreDustBefore*   m_pDistributorCoreDustBefore;   //入壳机除尘前分料
	CRunProcessCoreDust*					m_pCoreDust;					//电芯除尘
	CRunProcessDistributorOnLoadBefore*     m_pDistributorOnLoadBefore;     //入壳机上料前分料
	CRunProcessCoreOnLoad1*					m_pCoreOnLoad1;                  //入壳机上料工位线体
	CRunProcessCoreOnLoad2*					m_pCoreOnLoad2;                  //入壳机上料工位线体
	CRunProcessElevatorCore*                m_pElevatorCore;				//入壳机升降
	CRunProcessDistributorPlateDustBefore*  m_pDistributorPlateDustBefore;  //回流空盘除尘前分料
	CRunProcessPlateDust*					m_pPlateDust;					//回流动空盘除尘
	CRunProcessDamperCoreDown08*			m_pDamperCoreDown08;            //入壳阻挡下层08段
	CRunProcessDamperCoreDown06*			m_pDamperCoreDown06;            //入壳阻挡下层06段
	CRunProcessDamperMylarDown04*           m_pDamperMylarDown04;           //包膜4段下层阻挡
	CRunProcessDamperMylarDown03*           m_pDamperMylarDown03;           //包膜3段下层阻挡
	CRunProcessDistributorMylarOnLoad*      m_pDistributorMylarOnLoad;      //包膜上料分料下层

	CRunProcessCleanOnLoad*                 m_pLongCleanOnLoad[2];          //长边清洁上料
	CRunProcessCleanOnLoad*                 m_pShortCleanOnLoad[2];          //短边清洁上料

	CRunProcessRunProcessShortEdgeClean*    m_pShortEdgeClean[2];       //短边清洁
	CRunProcessRunProcessLongEdgeClean*     m_pLongEdgeClean[2];       //长边清洁

	CRunProcessRunProcessShortEdgeFeeder*   m_pShortEdgeFeeder[2];     //短边供料
	CRunProcessRunProcessLongEdgeFeeder*    m_pLongEdgeFeeder;         //长边供料

	// MES 成员
	map<int,CString> m_MesErrCodeMap;	// MES错误代码列表
	map<CString,CString> m_MesOnloadMap;

	//对应MesDlg MES系统配置显示
	CString  m_strMesMiChangeResourceStatus;//设备OEE数据采集
	CString  m_strMesMiStartSfcEx;						//电芯进站；
	CString  m_strMesMachineIntegration;				//电芯出站
	double m_fMesTime;											//MES上传超时设定

	CString  m_strMesName;					//MES用户名
	CString  m_strMesPassword;				//MES密码
	CString  m_strSite;								//站点
	CString  m_strUser;							//操作用户
	CString  m_strOperation;					//工位
	CString  m_strOperationRevision;		//工位版本
	CString  m_strResource;					// 设备资源号
	CString  m_strDcgrpup;
	int m_nModeProcessSfc;

	// 对应MesDlg MES系统配置显示
	int m_nBarCodeLength;						// 条码长度
	CString m_strMesCodelftThree;			// 条码前三位
	int m_nMESFaultCount;					// 连接故障次数
	int m_nMESFaultUploadCount;		// 连续故障上传次数

	// Mes Log路径
	CString  m_strMesDatePath;				//MES文件路径
	CString  m_strMesLogPath;				//MES进站路径
	CString  m_strMesMessagePath;		//MES出站路径

	// MES接口调用
	int m_nMesInterfaceCode;
	// 安东
	DWORD m_dwAndonShowTime;
	CString  m_strAndonResource;
	CString m_strAlarmMes;
	vector<CString> m_vecAlarmList;

	// 安全锁成员



	// 其他
	CString m_strShowSend;
	CString m_strShowRecv;
	BOOL m_bStopPickShell;	//停止取铝壳

	BOOL m_bUnEnableLineSingle;	// 屏蔽流水线信号

	BOOL m_bUnEnableDocking;		// 屏蔽后段对接

	// 恢复生产
	BOOL m_bReCoverMachine;
	// 手动/自动
	BOOL m_bManualAuto;
	// 清料
	BOOL m_bClearMaterial;
	// 生成CT Log
	BOOL m_bCreateCTLog;

	//CT
	BOOL m_bRebuildCTLog;

	// MES
	BOOL m_bCheckMes;

	BOOL m_bUseSafeDoor;

	BOOL m_bSafeStatckDoor[3];   //堆栈安全门

	BOOL m_bEnableAndon;
	BOOL m_bCreateAndonReport;

	// 设备运行时间
	DWORD   m_dwMachineRunTime;
	// 设备生产时间
	DWORD	m_dwProduceTime;
	// 设备待料时间
	BOOL		m_bWaitMaterial;
	DWORD	m_dwWaitMaterialTime;
	// 设备报警时间
	BOOL	m_bMachineAlarm;
	DWORD	m_dwMachineAlarmTime;

	// 产品计数
	int m_nOnloadCount;
	int m_nOffloadCount;
	int m_nNgCount;
	int m_nScanNgCount;
	int m_nUploadMesNgCount;
	int m_nShellStationPressNgCount[2];
	int m_nEachPartOffload[12];
	int m_nMesPraCount;
	//MES上次参数配置
	int m_nOnloadMesType[MES_ONLOADCOUNT];     //数据类型
	CString m_strTableValue[MES_ONLOADCOUNT];     //数据名称
	CString m_strAddVaul[MES_ONLOADCOUNT];    //数据实参
	CString m_strInprocess;   //入壳压力
	CString m_strInstation;    //入壳工位

	//MES日志删除
	CString m_strMesDataWay;
	CString m_strMesOutWay;
	CString m_strInWay;
	int m_nDelCycle;
	CString m_strCodeMes;
	CString m_strMesMsg;
	int m_nRecond;
	BOOL m_bBuzzer;
	CCriticalSection m_MesLock;
	CCriticalSection m_MesInLock;
	CCriticalSection m_LBuzzerLock;
	CCriticalSection m_MesLogLock, m_MesLogInLock;
	HANDLE m_hMesEvent;

	CzMES m_objMes;

	X_ValueSetting m_ValueSetting;//参数设定值
	X_ValueRange   m_ValueRange;//参数上下限范围
	X_BatteryData  m_BatteryData;//电池数据
	X_Status       m_MachineStatus;//机器状态参数
	CString m_strAddress;//OPC 服务器网址

};
// some shortcuts
#define MACHINE (*CMachineCtrl::GetInstance())

