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

// ����
#include "AndongDlg.h"
#include "AndonFeedDlg.h"
// ģ��
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
	//���ģ��ָ��
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
	// ����
	void AndonDataReport(const int nCurDire, CString strLog);
	void AndonLight(CString StrResourceId, CString StrLight);		// ��ɫ��
	void AndonMachineStatus(CString StrResourceId,int nMachineStatusCode);  //�����ϴ�����״̬��ͣ��ԭ��
	void AndonMachineAlarms(CString StrResourceId, int nMachineStatusCode,vector<CString> AlarmList); //�����ϴ�����
	void AndonMachineTotal(CString StrResourceId, int nInputTotal, int nOutputTotal, int nOKTotal, int nNGTotal); //�����ϴ�����
	
   //mes
	// �������ܺ���
	int CoreCountForClear(BOOL bCountAll); //��̨�ڵ�о����
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
	// �豸����
	int	 m_nTowerRed;
	int	 m_nTowerAmber;
	int	 m_nTowerGreen;
	int	 m_nTowerBuzzer;

	// �豸����
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
	//������
	int m_nLastStation;

	// ��ͣ
	int	 m_IEmgStop;
	// ��ȫ��
	int	 m_ISafeDoor;

public:
	int  m_numModules;

	// ��ǰ�������
	int  m_nDefaultProduct;
	// �Ƿ����÷���
	BOOL m_bDefaultEnable;

	// �����������Ա
	vector<CProductEX> m_vecProduct;
	CArray<CString> m_strModuleArray;
	CArray<CRunProcess*> m_ArrayStation;
	// ������Ա

	HWND				m_FramHwnd;
	CWnd*				m_pMachineViewWnd;
	CRunProcess**		m_pModules;
	CUserManager		m_user;
	CMachineProduct		m_product;
	CDataBaseRecord		m_dbRecord;

	CReportFile		m_ReportFile;

	CAndongDlg	m_DlgAndon;
	CAndonFeedDlg	m_DlgAndonFeed;
	
	// ģ��ָ��
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

	CRunProcessElevatorMylar*				m_pElevatorMaylar;				//��Ĥ������
	CRunProcessDistributorMylarOffLoad1*    m_pDistributorMylarOffLoad1;	//��Ĥ1����ǰ����
	CRunProcessDistributorMylarOffLoad2*    m_pDistributorMylarOffLoad2;	//��Ĥ2����ǰ����
	CRunProcessOffLoadMylar1*			    m_pOffLoadMylar1;				//��Ĥ������1
	CRunProcessOffLoadMylar2*			    m_pOffLoadMylar2;				//��Ĥ������2
	CRunProcessDamperMylarUp03*             m_pDamperMylarUp03;             //��Ĥ3���ϲ�ת���赲
	CRunProcessDamperMylarUp04*             m_pDamperMylarUp04;             //��Ĥ4���ϲ��赲
	CRunProcessDamperMylarUp05*             m_pDamperMylarUp05;             //��Ĥ5���ϲ��赲
	CRunProcessDistributorCoreDustBefore*   m_pDistributorCoreDustBefore;   //��ǻ�����ǰ����
	CRunProcessCoreDust*					m_pCoreDust;					//��о����
	CRunProcessDistributorOnLoadBefore*     m_pDistributorOnLoadBefore;     //��ǻ�����ǰ����
	CRunProcessCoreOnLoad1*					m_pCoreOnLoad1;                  //��ǻ����Ϲ�λ����
	CRunProcessCoreOnLoad2*					m_pCoreOnLoad2;                  //��ǻ����Ϲ�λ����
	CRunProcessElevatorCore*                m_pElevatorCore;				//��ǻ�����
	CRunProcessDistributorPlateDustBefore*  m_pDistributorPlateDustBefore;  //�������̳���ǰ����
	CRunProcessPlateDust*					m_pPlateDust;					//���������̳���
	CRunProcessDamperCoreDown08*			m_pDamperCoreDown08;            //����赲�²�08��
	CRunProcessDamperCoreDown06*			m_pDamperCoreDown06;            //����赲�²�06��
	CRunProcessDamperMylarDown04*           m_pDamperMylarDown04;           //��Ĥ4���²��赲
	CRunProcessDamperMylarDown03*           m_pDamperMylarDown03;           //��Ĥ3���²��赲
	CRunProcessDistributorMylarOnLoad*      m_pDistributorMylarOnLoad;      //��Ĥ���Ϸ����²�

	CRunProcessCleanOnLoad*                 m_pLongCleanOnLoad[2];          //�����������
	CRunProcessCleanOnLoad*                 m_pShortCleanOnLoad[2];          //�̱��������

	CRunProcessRunProcessShortEdgeClean*    m_pShortEdgeClean[2];       //�̱����
	CRunProcessRunProcessLongEdgeClean*     m_pLongEdgeClean[2];       //�������

	CRunProcessRunProcessShortEdgeFeeder*   m_pShortEdgeFeeder[2];     //�̱߹���
	CRunProcessRunProcessLongEdgeFeeder*    m_pLongEdgeFeeder;         //���߹���

	// MES ��Ա
	map<int,CString> m_MesErrCodeMap;	// MES��������б�
	map<CString,CString> m_MesOnloadMap;

	//��ӦMesDlg MESϵͳ������ʾ
	CString  m_strMesMiChangeResourceStatus;//�豸OEE���ݲɼ�
	CString  m_strMesMiStartSfcEx;						//��о��վ��
	CString  m_strMesMachineIntegration;				//��о��վ
	double m_fMesTime;											//MES�ϴ���ʱ�趨

	CString  m_strMesName;					//MES�û���
	CString  m_strMesPassword;				//MES����
	CString  m_strSite;								//վ��
	CString  m_strUser;							//�����û�
	CString  m_strOperation;					//��λ
	CString  m_strOperationRevision;		//��λ�汾
	CString  m_strResource;					// �豸��Դ��
	CString  m_strDcgrpup;
	int m_nModeProcessSfc;

	// ��ӦMesDlg MESϵͳ������ʾ
	int m_nBarCodeLength;						// ���볤��
	CString m_strMesCodelftThree;			// ����ǰ��λ
	int m_nMESFaultCount;					// ���ӹ��ϴ���
	int m_nMESFaultUploadCount;		// ���������ϴ�����

	// Mes Log·��
	CString  m_strMesDatePath;				//MES�ļ�·��
	CString  m_strMesLogPath;				//MES��վ·��
	CString  m_strMesMessagePath;		//MES��վ·��

	// MES�ӿڵ���
	int m_nMesInterfaceCode;
	// ����
	DWORD m_dwAndonShowTime;
	CString  m_strAndonResource;
	CString m_strAlarmMes;
	vector<CString> m_vecAlarmList;

	// ��ȫ����Ա



	// ����
	CString m_strShowSend;
	CString m_strShowRecv;
	BOOL m_bStopPickShell;	//ֹͣȡ����

	BOOL m_bUnEnableLineSingle;	// ������ˮ���ź�

	BOOL m_bUnEnableDocking;		// ���κ�ζԽ�

	// �ָ�����
	BOOL m_bReCoverMachine;
	// �ֶ�/�Զ�
	BOOL m_bManualAuto;
	// ����
	BOOL m_bClearMaterial;
	// ����CT Log
	BOOL m_bCreateCTLog;

	//CT
	BOOL m_bRebuildCTLog;

	// MES
	BOOL m_bCheckMes;

	BOOL m_bUseSafeDoor;

	BOOL m_bSafeStatckDoor[3];   //��ջ��ȫ��

	BOOL m_bEnableAndon;
	BOOL m_bCreateAndonReport;

	// �豸����ʱ��
	DWORD   m_dwMachineRunTime;
	// �豸����ʱ��
	DWORD	m_dwProduceTime;
	// �豸����ʱ��
	BOOL		m_bWaitMaterial;
	DWORD	m_dwWaitMaterialTime;
	// �豸����ʱ��
	BOOL	m_bMachineAlarm;
	DWORD	m_dwMachineAlarmTime;

	// ��Ʒ����
	int m_nOnloadCount;
	int m_nOffloadCount;
	int m_nNgCount;
	int m_nScanNgCount;
	int m_nUploadMesNgCount;
	int m_nShellStationPressNgCount[2];
	int m_nEachPartOffload[12];
	int m_nMesPraCount;
	//MES�ϴβ�������
	int m_nOnloadMesType[MES_ONLOADCOUNT];     //��������
	CString m_strTableValue[MES_ONLOADCOUNT];     //��������
	CString m_strAddVaul[MES_ONLOADCOUNT];    //����ʵ��
	CString m_strInprocess;   //���ѹ��
	CString m_strInstation;    //��ǹ�λ

	//MES��־ɾ��
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

	X_ValueSetting m_ValueSetting;//�����趨ֵ
	X_ValueRange   m_ValueRange;//���������޷�Χ
	X_BatteryData  m_BatteryData;//�������
	X_Status       m_MachineStatus;//����״̬����
	CString m_strAddress;//OPC ��������ַ

};
// some shortcuts
#define MACHINE (*CMachineCtrl::GetInstance())

