#include "StdAfx.h"
#include "MachineCtrl.h"
#include "MachineMaintenance.h"
#include <locale>
#include "imagehlp.h"

CMachineCtrl *CMachineCtrl::m_pInstance = NULL;
CString *g_pInputNames = NULL;
CString *g_pOutputNames = NULL;


//清空模组指针
void CMachineCtrl::EmptyRunProcessPtr()
{

	m_pLongCleanOnLoad[0]  = NULL;
	m_pLongCleanOnLoad[1]  = NULL;
	m_pLongEdgeClean[0]    = NULL;
	m_pLongEdgeClean[1]    = NULL;
	m_pLongEdgeFeeder      = NULL;
	m_pLongCleanFrontDistributor = NULL;

	m_pShortCleanOnLoad[0] = NULL;
	m_pShortCleanOnLoad[1] = NULL;
	m_pShortEdgeClean[0]   = NULL;
	m_pShortEdgeClean[1]   = NULL;
	m_pShortEdgeFeeder[0]  = NULL;
	m_pShortEdgeFeeder[1]  = NULL;
	m_pShortCleanFrontDistributor = NULL;

	m_pOnloadShellStack[0] = NULL;
	m_pOnloadShellStack[1] = NULL;
	m_pFeederTray = NULL;
	m_pOnloadShellPnp = NULL;
	m_pCleanBefore = NULL;
	m_pCleanShell = NULL;
	m_pCleanAfter = NULL;
	m_pCarryShellPnp = NULL;
	m_pBatteryInShell[0] = NULL;
	m_pBatteryInShell[1] = NULL;
	m_pOnloadCore = NULL;
	m_pBarCodeNgLine = NULL;
	m_pBatteryCore[0] = NULL;
	m_pBatteryCore[1] = NULL;
	m_pBatteryOffloadPnp[0] = NULL;
	m_pBatteryOffloadPnp[1] = NULL;
	m_pRobotOffload = NULL;
	m_pPressNgLine = NULL;
	m_pUploadMes = NULL;
	m_pReadCode[0]= NULL;
	m_pReadCode[1]= NULL;
	m_pElevatorMaylar = NULL;
	m_pDistributorMylarOffLoad1 = NULL;
	m_pDistributorMylarOffLoad2 = NULL;
	m_pOffLoadMylar1 = NULL;
	m_pOffLoadMylar2 = NULL;
	m_pDamperMylarUp03 = NULL;
	m_pDamperMylarUp04 = NULL;
	m_pDamperMylarUp05 = NULL;
	m_pDistributorCoreDustBefore = NULL;
	m_pCoreDust = NULL;
	m_pDistributorOnLoadBefore = NULL;
	m_pCoreOnLoad1 = NULL;
	m_pCoreOnLoad2 = NULL;
	m_pElevatorCore = NULL;
	m_pDistributorPlateDustBefore = NULL;
	m_pPlateDust = NULL;
	m_pDamperCoreDown08 = NULL;
	m_pDamperCoreDown06 = NULL;
	m_pDamperMylarDown04 = NULL;
	m_pDamperMylarDown03 = NULL;
	m_pDistributorMylarOnLoad = NULL;
}
CMachineCtrl::CMachineCtrl()
{
	MachineExit = FALSE;
	m_user.SetTypeName(USER_ENGINEER, _T("Service"));
	m_user.ChangeType(_T("Service"),USER_ENGINEER);

	m_user.ShowType(USER_ADMIN, FALSE);
	m_user.ShowType(USER_SERVICE, FALSE);
	m_user.ShowType(USER_SUPERUSER, FALSE);
	
	m_bRebuildCTLog=FALSE; 
	m_nLastStation = -1;
	m_strShowRecv = _T("");
	m_strShowSend = _T("");
	m_bManualAuto = TRUE;
	m_bMachineAlarm = FALSE;
	m_bReCoverMachine = FALSE;
	m_bClearMaterial = FALSE;
	m_bStopPickShell = FALSE;
	m_bCreateAndonReport = FALSE;
	m_bUnEnableLineSingle = FALSE;
	m_bUseSafeDoor = TRUE;
	m_bUnEnableDocking = FALSE;
	m_bBuzzer = FALSE;
	for (int n = 0; n < 3; n++)
	{
		m_bSafeStatckDoor[n] = FALSE;
	}
	
	m_bWaitMaterial = FALSE;
	m_nMesInterfaceCode = 0;

	EmptyRunProcessPtr();

	m_hMesEvent = CreateEvent(NULL,FALSE,FALSE,_T(""));
	if (INVALID_HANDLE_VALUE != m_hMesEvent)
	{
		SetEvent(m_hMesEvent);
	}

	m_strCodeMes = _T("NULL");
	m_strMesMsg = _T("NULL");
	m_nRecond = 0;

	m_numModules = IniReadInt(_T("Modules"),_T("NumModules"),1,GetAbsolutePathName(MODULEEX_CFG));

	m_bCheckMes = IniReadBoolean(_T("Machine"), _T("CheckMes"), FALSE, GetAbsolutePathName(MACHINE_CFG));
	m_bCreateCTLog = IniReadBoolean(_T("Machine"), _T("CreateCTLog"), FALSE, GetAbsolutePathName(MACHINE_CFG));

	m_bDefaultEnable = IniReadBoolean(_T("Parameter"), _T("DefaultEnable"), TRUE, ::GetAbsolutePathName(MACHINE_CFG));
	m_nDefaultProduct = IniReadInt(_T("Parameter"), _T("DefaultProduct"), -1, ::GetAbsolutePathName(MACHINE_CFG));

	m_nOnloadCount = IniReadInt(_T("Produ"), _T("m_nOnloadCount"), 0, ::GetAbsolutePathName(MACHINE_CFG));
	m_nOffloadCount = IniReadInt(_T("Produ"), _T("m_nOffloadCount"), 0, ::GetAbsolutePathName(MACHINE_CFG));
	m_nNgCount = IniReadInt(_T("Produ"), _T("m_nNgCount"), 0, ::GetAbsolutePathName(MACHINE_CFG));

	// 产能、报警
	m_dwMachineRunTime = IniReadInt(_T("StatusInfo"), _T("m_dwMachineRunTime"), 0, ::GetAbsolutePathName(MACHINE_CFG));
	m_dwProduceTime = IniReadInt(_T("StatusInfo"), _T("m_dwProduceTime"), 0, ::GetAbsolutePathName(MACHINE_CFG));
	m_dwWaitMaterialTime = IniReadInt(_T("StatusInfo"), _T("m_dwWaitMaterialTime"), 0, ::GetAbsolutePathName(MACHINE_CFG));
	m_dwMachineAlarmTime = IniReadInt(_T("StatusInfo"), _T("m_dwMachineAlarmTime"), 0, ::GetAbsolutePathName(MACHINE_CFG));

	m_nScanNgCount = IniReadInt(_T("Produ"), _T("ScanNgCount"), 0, GetAbsolutePathName(MACHINE_CFG));
	m_nUploadMesNgCount = IniReadInt(_T("Produ"), _T("UploadMesNgCount"), 0, GetAbsolutePathName(MACHINE_CFG));
	m_nShellStationPressNgCount[0] = IniReadInt(_T("Produ"), _T("ShellStation1PressNgCount"), 0, GetAbsolutePathName(MACHINE_CFG));
	m_nShellStationPressNgCount[1] = IniReadInt(_T("Produ"), _T("ShellStation2PressNgCount"), 0, GetAbsolutePathName(MACHINE_CFG));

	m_strMesDataWay = ::IniReadString(_T("MESSET"),_T("strMesDataWay"),_T(""),GetAbsolutePathName(MACHINE_CFG));
	m_strMesOutWay = ::IniReadString(_T("MESSET"),_T("strMesOutWay"),_T(""),GetAbsolutePathName(MACHINE_CFG));
	m_strInWay = ::IniReadString(_T("MESSET"),_T("strInWay"),_T(""),GetAbsolutePathName(MACHINE_CFG));
	m_nDelCycle = ::IniReadInt(_T("MESSET"),_T("DelCycle"),0,GetAbsolutePathName(MACHINE_CFG));

	// 安东
	m_strAndonResource = IniReadString(_T("Andon"), _T("AndonResource"), _T("ANDON"), ::GetAbsolutePathName(MACHINE_CFG));
	m_bEnableAndon = IniReadBoolean(_T("Andon"), _T("EnableAndon"), FALSE, ::GetAbsolutePathName(MACHINE_CFG));


	CString strTemp;
	for (int i = 0; i < 12; i++)
	{
		strTemp.Format(_T("EachPartOffload%d"), i);
		m_nEachPartOffload[i] = IniReadInt(_T("Produ"), strTemp, 0, GetAbsolutePathName(MACHINE_CFG));
	}

	CTime tm = CTime::GetCurrentTime();
	CString FileName = _T("");
	FileName.Format(_T("D:\\机台数据文件\\CTLog\\CLog%d-%d-%d\\"), tm.GetYear(), tm.GetMonth(), tm.GetDay());

	USES_CONVERSION;
	MakeSureDirectoryPathExists(W2A(FileName));


	ReadMesData();
	Release();
}

CMachineCtrl::~CMachineCtrl()
{
	delete [] m_pModules;
	m_pModules = NULL;
	m_strModuleArray.RemoveAll();
	m_ArrayStation.RemoveAll();
	m_vecProduct.clear();
	m_MesErrCodeMap.clear();
	m_vecAlarmList.clear();
	m_MesOnloadMap.clear();

	EmptyRunProcessPtr();
	MachineExit = TRUE;
	m_DlgAndon.DestroyWindow();
	m_DlgAndonFeed.DestroyWindow();
}

CMachineCtrl * CMachineCtrl::GetInstance()
{
	if( !m_pInstance )
		m_pInstance = new CMachineCtrl();
	return m_pInstance;
}

void CMachineCtrl::Destroy()
{
	delete m_pInstance;
	m_pInstance = NULL;
}

void CMachineCtrl::ReadMesData()
{
	//MES系统配置显示
	m_strMesName = ::IniReadString(_T("MESSET"), _T("MesName"),  _T("NULL"), GetAbsolutePathName(MACHINE_CFG));
	m_strMesPassword =::IniReadString(_T("MESSET"), _T("MesPassword"),  _T("NULL"), GetAbsolutePathName(MACHINE_CFG));
	m_strSite =::IniReadString(_T("MESSET"), _T("Site"),  _T("NULL"), GetAbsolutePathName(MACHINE_CFG));
	m_strUser =::IniReadString(_T("MESSET"), _T("User"),  _T("NULL"), GetAbsolutePathName(MACHINE_CFG));
	m_strOperation=::IniReadString(_T("MESSET"), _T("Operation"),  _T("INCAHP"), GetAbsolutePathName(MACHINE_CFG));
	m_strOperationRevision =::IniReadString(_T("MESSET"), _T("Revision"),  _T("#"), GetAbsolutePathName(MACHINE_CFG));
	m_strResource =::IniReadString(_T("MESSET"), _T("Resource"),  _T("NULL"), GetAbsolutePathName(MACHINE_CFG));
	m_strDcgrpup = ::IniReadString(_T("MESSET"), _T("Dcgrpup"),  _T("NULL"), GetAbsolutePathName(MACHINE_CFG));
	m_nModeProcessSfc = ::IniReadInt(_T("MESSET"),_T("ModeProcessSfc"),2,GetAbsolutePathName(MACHINE_CFG));

	m_strMesMiChangeResourceStatus =::IniReadString(_T("MESSET"), _T("MesMiChangeResourceStatus"),  _T("NULL"), GetAbsolutePathName(MACHINE_CFG));
	m_strMesMiStartSfcEx =::IniReadString(_T("MESSET"), _T("MesMiStartSfcEx"),  _T("NULL"), GetAbsolutePathName(MACHINE_CFG));
	m_strMesMachineIntegration =::IniReadString(_T("MESSET"), _T("MesMachineIntegration"),  _T("NULL"), GetAbsolutePathName(MACHINE_CFG));
	m_fMesTime =::IniReadDouble(_T("MESSET"), _T("MesTime"), 30, GetAbsolutePathName(MACHINE_CFG));

	//MES系统配置显示
	m_strMesCodelftThree =::IniReadString(_T("MESSET"),_T("MesCodelftThree"), _T("NULL"),GetAbsolutePathName(MACHINE_CFG));
	m_nBarCodeLength =::IniReadInt(_T("MESSET"),_T("BarCodeLength"), 0,GetAbsolutePathName(MACHINE_CFG));
	m_nMESFaultCount =::IniReadInt(_T("MESSET"),_T("MESFaultCount"), 0,GetAbsolutePathName(MACHINE_CFG));
	m_nMESFaultUploadCount =::IniReadInt(_T("MESSET"),_T("MESFaultUploadCount"), 0,GetAbsolutePathName(MACHINE_CFG));

	// Mes Log路径
	m_strMesDatePath = ::IniReadString(_T("MESSET"), _T("MesDatePath"),  _T("NULL"), GetAbsolutePathName(MACHINE_CFG));
	m_strMesLogPath = ::IniReadString(_T("MESSET"), _T("MesLogPath"),  _T("NULL"), GetAbsolutePathName(MACHINE_CFG));
	m_strMesMessagePath = ::IniReadString(_T("MESSET"), _T("MesMessagePath"),  _T("NULL"), GetAbsolutePathName(MACHINE_CFG));

	CString strKey;
	for (int i = 0; i < MES_ONLOADCOUNT; i++)
	{
		strKey.Format(_T("OnloadMesType%d"),i);
		m_nOnloadMesType[i] = IniReadInt(_T("MESSET"), strKey, 0, ::GetAbsolutePathName(MACHINE_CFG));
		strKey.Format(_T("AddVaul%d"),i);
		m_strAddVaul[i] = IniReadString(_T("MESSET"), strKey, _T(""), ::GetAbsolutePathName(MACHINE_CFG));		
		strKey.Format(_T("strTableValue%d"),i);
		m_strTableValue[i] =  ::IniReadString(_T("MESSET"), strKey,  _T("NULL"), GetAbsolutePathName(MACHINE_CFG));
	}
	m_nMesPraCount = IniReadInt(_T("MESSET"), _T("MesPraCount"), 0, ::GetAbsolutePathName(MACHINE_CFG));
}

void CMachineCtrl::WriteMesErrCode()
{
	int nIndex = 0;
	CString strMsg,strKey;

	int nCount = m_MesErrCodeMap.size();

	IniWriteInt(_T("MesErrCode"),_T("Count"),nCount,GetAbsolutePathName(PARAMETER_CFG));

	map<int,CString>::iterator itor;

	for (itor = m_MesErrCodeMap.begin(); itor != m_MesErrCodeMap.end(); itor++,nIndex++)
	{
		strKey.Format(_T("ErrCode%d"),nIndex);
		IniWriteInt(_T("MesErrCode"),strKey,itor->first,GetAbsolutePathName(PARAMETER_CFG));
		strKey.Format(_T("ErrMsg%d"),nIndex);
		IniWriteString(_T("MesErrCode"),strKey,itor->second,GetAbsolutePathName(PARAMETER_CFG));
	}
}

void CMachineCtrl::ReadMesErrCode()
{
	int nCode;
	CString strMsg,strKey;

	int nCount = IniReadInt(_T("MesErrCode"),_T("Count"),0,GetAbsolutePathName(PARAMETER_CFG));

	for (int nCur = 0; nCur < nCount; nCur++)
	{
		strKey.Format(_T("ErrCode%d"),nCur);
		nCode = IniReadInt(_T("MesErrCode"),strKey,0,GetAbsolutePathName(PARAMETER_CFG));
		strKey.Format(_T("ErrMsg%d"),nCur);
		strMsg = IniReadString(_T("MesErrCode"),strKey,0,GetAbsolutePathName(PARAMETER_CFG));

		m_MesErrCodeMap.insert(make_pair(nCode,strMsg));
	}
}

BOOL WriteModuleCfg(int index, CString strName, CArray<int, int> *pInputs, CArray<int, int> *pOutputs, CArray<int, int> *pMotors)
{
	CString strSection, strKey, strValue;
	strSection.Format(_T("Module%d"), index);

	// name
	::IniWriteString(strSection, _T("Name"), strName, GetAbsolutePathName(MODULE_CFG));

	// input
	int count = pInputs ? (int)pInputs->GetCount() : 0;
	::IniWriteInt(strSection, _T("InputCount"), count, GetAbsolutePathName(MODULE_CFG));

	for (int i=0; i<count; i++)
	{
		strKey.Format(_T("Input%d"), i);
		::IniWriteInt(strSection, strKey, pInputs->GetAt(i), GetAbsolutePathName(MODULE_CFG));
	}

	// output
	count = pOutputs ? (int)pOutputs->GetCount() : 0;
	::IniWriteInt(strSection, _T("OutputCount"), count, GetAbsolutePathName(MODULE_CFG));


	for (int i=0; i<count; i++)
	{
		strKey.Format(_T("Output%d"), i);
		::IniWriteInt(strSection, strKey, pOutputs->GetAt(i), GetAbsolutePathName(MODULE_CFG));
	}

	// motor
	count = pMotors ? (int)pMotors->GetCount() : 0;
	::IniWriteInt(strSection, _T("MotorCount"), count, GetAbsolutePathName(MODULE_CFG));

	for (int i=0; i<count; i++)
	{
		strKey.Format(_T("Motor%d"), i);
		::IniWriteInt(strSection, strKey, pMotors->GetAt(i), GetAbsolutePathName(MODULE_CFG));
	}
	return FALSE;
}

BOOL CMachineCtrl::Initialize(HWND hMsgProcessWnd)
{
	// get input count
	HWND hwnd = GetDesktopWindow();
	RECT rect;
	GetClientRect(hwnd, &rect);
	int nWidth = GetSystemMetrics(SM_CXSCREEN);
	int nHeight = GetSystemMetrics(SM_CYSCREEN);

	m_DlgAndon.Create(IDD_DIALOG_ANDONG);
	m_DlgAndon.MoveWindow(nWidth/2 - 410, nHeight/2 - 250, 820, 500, TRUE);

	m_DlgAndonFeed.Create(IDD_DIALOG_ANDONG_FEED);
	m_DlgAndonFeed.MoveWindow(nWidth/2 - 200, nHeight/2 - 100, 400, 200, TRUE);

	CString strSection, strName;
	m_numInputs = 0, m_numOutputs = 0, m_numMotors = 0;

	for (int i = 0; i< 500; i++)
	{
		strSection.Format(_T("INPUT%d"), i);
		strName = ::IniReadString(strSection, _T("Name"), _T(""), GetAbsolutePathName(INPUT_CFG));
		if (strName.IsEmpty())
			break;
		m_numInputs++;
	}

	// get output count

	for (int i = 0; i< 500; i++)
	{
		strSection.Format(_T("OUTPUT%d"), i);
		strName = ::IniReadString(strSection, _T("Name"), _T(""), GetAbsolutePathName(OUTPUT_CFG));
		if (strName.IsEmpty())
			break;
		m_numOutputs++;
	}

	// get the motor count
	CFileFind fileFind;
	for (int i = 0; i< 100; i++)
	{
		strName.Format(_T("%sMotor%d.cfg"), MOTOR_CFG_FOLDER, i);
		if (!fileFind.FindFile(GetAbsolutePathName(strName)))
			break;
		m_numMotors++;
	}

	::DeleteFile(GetAbsolutePathName(MODULE_CFG));
	// call parent to init first
	if (!CCentralControl::Initialize(hMsgProcessWnd, m_numMotors, m_numInputs, m_numOutputs, NUM_OF_EVENTS, NUM_OF_MACHINE_TIME))
	{
		exit(0);
	}

	// initialize package first so that later when creat run theads, they can use package's data
	CString strPkgName = IniReadString(_T("Run"), _T("CurrentPackage"), _T(""), GetAbsolutePathName(MACHINE_CFG));
	if (!strPkgName.IsEmpty())
	{
		if (!m_product.LoadPkgParam(strPkgName))
		{
			AfxMessageBox(_T("Load package file failed!"));
		}
		else
		{
			if (!m_product.LoadMotorParam())
			{
				AfxMessageBox(_T("Load package motor param failed!"));
			}
			else
			{
				m_pRunCtrl->m_pPackage = &m_product;
			}
		}
	}
	m_pRunCtrl->BypassDoor(TRUE);
	return TRUE;
}

BOOL CMachineCtrl::InitializeRunThreads(HWND hMsgProcessWnd)
{
	ASSERT(m_pRunCtrl == NULL);
	CString strKey;
	CString strSection,strClass;
	// allocate the process pointer array
	m_pModules = new CRunProcess*[m_numModules];

	for (int i = 0; i< m_numModules; i++)
	{
		m_pModules[i] = NULL;
	}

	::IniWriteInt(_T("Modules"), _T("NumModule"), m_numModules+1, GetAbsolutePathName(MODULE_CFG));
	::IniWriteString(_T("Module0"), _T("Name"), _T("System"), GetAbsolutePathName(MODULE_CFG));

	CArray<CString> m_strClassArray;
	for (int i = 0; i < m_numModules; i++)
	{
		strKey.Format(_T("Module%d"),i);
		strSection = ::IniReadString(_T("Modules"), strKey, _T(""), GetAbsolutePathName(MODULEEX_CFG));
		strClass = ::IniReadString(strSection, _T("Class"), _T("Process"), GetAbsolutePathName(MODULEEX_CFG));
		m_strModuleArray.Add(strSection);
		m_strClassArray.Add(strClass);

		if(strClass == _T("OnloadCore"))
		{
			m_pModules[i] = new CRunProcessOnloadCore(i);
			m_pOnloadCore = (CRunProcessOnloadCore*)m_pModules[i];
		}
		else if(strClass == _T("NgLine"))
		{
			m_pModules[i] = new CRunProcessNgLine(i);
			if(strSection == _T("BarCodeNgLine"))
				m_pBarCodeNgLine = (CRunProcessNgLine*)m_pModules[i];
			else
				m_pPressNgLine = (CRunProcessNgLine*)m_pModules[i];
		}
		else if(strClass == _T("BatteryCore"))
		{
			m_pModules[i] = new CRunProcessBatteryCore(i);
			if(strSection == _T("BatteryCore1"))
				m_pBatteryCore[0] = (CRunProcessBatteryCore* )m_pModules[i];
			else
				m_pBatteryCore[1] = (CRunProcessBatteryCore* )m_pModules[i];
		}
		else if(strClass == _T("OnloadShellStack"))
		{
			m_pModules[i] = new CRunProcessOnloadShellStack(i);
			if(strSection == _T("OnloadShellStack1"))
				m_pOnloadShellStack[0] = (CRunProcessOnloadShellStack *)m_pModules[i];
			else
				m_pOnloadShellStack[1] = (CRunProcessOnloadShellStack *)m_pModules[i];
		}
		else if(strClass == _T("OffloadShellStack"))
		{
			m_pModules[i] = new CRunProcessOffloadShellStack(i);
			m_pOffloadShellStack = (CRunProcessOffloadShellStack *)m_pModules[i];
		}
		else if(strClass == _T("FeederTray"))
		{
			m_pModules[i] = new CRunProcessFeederTray(i);
			m_pFeederTray = (CRunProcessFeederTray *)m_pModules[i];
		}
		else if(strClass == _T("OnloadShellPnp"))
		{
			m_pModules[i] = new CRunProcessOnloadShellPNP(i);
			m_pOnloadShellPnp = (CRunProcessOnloadShellPNP *)m_pModules[i];
		}
		else if(strClass == _T("CleanBefore"))
		{
			m_pModules[i] = new CRunProcessCleanShellBefor(i);
			m_pCleanBefore = (CRunProcessCleanShellBefor*)m_pModules[i];
		}
		else if(strClass == _T("CleanShell"))
		{
			m_pModules[i] = new CRunProcessCleanShell(i);
			m_pCleanShell = (CRunProcessCleanShell*)m_pModules[i];
		}
		else if(strClass == _T("CleanAfter"))
		{
			m_pModules[i] = new CRunProcessCleanShellAfter(i);
			m_pCleanAfter = (CRunProcessCleanShellAfter*)m_pModules[i];
		}
		else if(strClass == _T("CarryShellPnp"))
		{
			m_pModules[i] = new CRunProcessCarryShellPnP(i);
			m_pCarryShellPnp = (CRunProcessCarryShellPnP*)m_pModules[i];
		}
		else if(strClass == _T("BatteryInShell"))
		{
			m_pModules[i] = new CRunProcessBatteryInShell(i);
			if(strSection == _T("BatteryInShell1"))
				m_pBatteryInShell[0] = (CRunProcessBatteryInShell* )m_pModules[i];
			else
				m_pBatteryInShell[1] = (CRunProcessBatteryInShell* )m_pModules[i];
		}
		else if(strClass == _T("BatteryInShellOffloadPnp"))
		{
			m_pModules[i] = new CRunProcessBatteryInShellOffloadPnP(i);

			if(strSection == _T("BatteryInShellOffloadPnp1"))
				m_pBatteryOffloadPnp[0] = (CRunProcessBatteryInShellOffloadPnP*)m_pModules[i];
			else
				m_pBatteryOffloadPnp[1] = (CRunProcessBatteryInShellOffloadPnP*)m_pModules[i];
		}
		else if(strClass == _T("RobotOffload"))
		{
			m_pModules[i] = new CRunProcessRobot(i);
			m_pRobotOffload = (CRunProcessRobot*)m_pModules[i];
		}
		else if(strClass == _T("MES"))
		{
			m_pModules[i] = new CRunProcessMES(i);
			m_pUploadMes = (CRunProcessMES*)m_pModules[i];
		}
		else if(strClass == _T("ReadCodeConveyer"))
		{
			m_pModules[i] = new CRunProcessReadCodeConveyer(i);

			if(strSection == _T("ReadCode1"))
				m_pReadCode[0] = (CRunProcessReadCodeConveyer*)m_pModules[i];
			else
				m_pReadCode[1] = (CRunProcessReadCodeConveyer*)m_pModules[i];
		}
		else if(strClass == _T("CRunProcessElevatorMylar"))
		{
			m_pModules[i] = new CRunProcessElevatorMylar(i);
			m_pElevatorMaylar = (CRunProcessElevatorMylar*)m_pModules[i];
		}
		else if(strClass == _T("CRunProcessDistributorMylarOffLoad1"))
		{
			m_pModules[i] = new CRunProcessDistributorMylarOffLoad1(i);
			m_pDistributorMylarOffLoad1 = (CRunProcessDistributorMylarOffLoad1*)m_pModules[i];
		}
		else if(strClass == _T("CRunProcessDistributorMylarOffLoad2"))
		{
			m_pModules[i] = new CRunProcessDistributorMylarOffLoad2(i);
			m_pDistributorMylarOffLoad2 = (CRunProcessDistributorMylarOffLoad2*)m_pModules[i];
		}
		else if(strClass == _T("CRunProcessOffLoadMylar1"))
		{
			m_pModules[i] = new CRunProcessOffLoadMylar1(i);
			m_pOffLoadMylar1 = (CRunProcessOffLoadMylar1*)m_pModules[i];
		}
		else if(strClass == _T("CRunProcessOffLoadMylar2"))
		{
			m_pModules[i] = new CRunProcessOffLoadMylar2(i);
			m_pOffLoadMylar2 = (CRunProcessOffLoadMylar2*)m_pModules[i];
		}
		else if(strClass == _T("CRunProcessDamperMylarUp03"))
		{
			m_pModules[i] = new CRunProcessDamperMylarUp03(i);
			m_pDamperMylarUp03 = (CRunProcessDamperMylarUp03*)m_pModules[i];
		}
		else if(strClass == _T("CRunProcessDamperMylarUp04"))
		{
			m_pModules[i] = new CRunProcessDamperMylarUp04(i);
			m_pDamperMylarUp04 = (CRunProcessDamperMylarUp04*)m_pModules[i];
		}
		else if(strSection == _T("LongCleanFrontDistributor"))        //长边清洁前分料1
		{   
			m_pModules[i] = new CRunProcessCleanFrontDistributor(i);
			m_pLongCleanFrontDistributor = (CRunProcessCleanFrontDistributor*)m_pModules[i];
		}
		else if(strSection == _T("LongCleanOnLoad01"))  //长边清洁上料1
		{
			m_pModules[i] = new CRunProcessCleanOnLoad(i);
			m_pLongCleanOnLoad[0] = (CRunProcessCleanOnLoad*)m_pModules[i];
		}
		else if(strSection == _T("LongEdgeClean01"))  //长边清洁1
		{
			m_pModules[i] = new CRunProcessRunProcessLongEdgeClean(i);
			m_pLongEdgeClean[0] = (CRunProcessRunProcessLongEdgeClean*)m_pModules[i];
		}
		else if(strSection == _T("LongCleanOnLoad02"))   //长边清洁上料2    
		{   
			m_pModules[i] = new CRunProcessCleanOnLoad(i);
			m_pLongCleanOnLoad[1] = (CRunProcessCleanOnLoad*)m_pModules[i];
		}
		else if(strSection == _T("LongEdgeClean02"))  //长边清洁2
		{
			m_pModules[i] = new CRunProcessRunProcessLongEdgeClean(i);
			m_pLongEdgeClean[1] = (CRunProcessRunProcessLongEdgeClean*)m_pModules[i];
		}
		else if(strSection == _T("LongEdgeFeeder"))   //长边供料    
		{   
			m_pModules[i] = new CRunProcessRunProcessLongEdgeFeeder(i);
			m_pLongEdgeFeeder = (CRunProcessRunProcessLongEdgeFeeder*)m_pModules[i];
		}
		else if(strSection == _T("ShortCleanFrontDistributor"))      //短边清洁前分料
		{
			m_pModules[i] = new CRunProcessCleanFrontDistributor(i);
			m_pShortCleanFrontDistributor = (CRunProcessCleanFrontDistributor*)m_pModules[i];
		}
		else if(strSection == _T("ShortCleanOnLoad01"))  //短边清洁上料1
		{
			m_pModules[i] = new CRunProcessCleanOnLoad(i);
			m_pShortCleanOnLoad[0] = (CRunProcessCleanOnLoad*)m_pModules[i];
		}
		else if(strSection == _T("ShortEdgeClean01"))  //短边清洁1
		{
			m_pModules[i] = new CRunProcessRunProcessShortEdgeClean(i);
			m_pShortEdgeClean[0] = (CRunProcessRunProcessShortEdgeClean*)m_pModules[i];
		}
		else if(strSection == _T("ShortEdgeFeeder01"))  //短边供料1
		{
			m_pModules[i] = new CRunProcessRunProcessShortEdgeFeeder(i);
			m_pShortEdgeFeeder[0] = (CRunProcessRunProcessShortEdgeFeeder*)m_pModules[i];
		}
		else if(strSection == _T("ShortCleanOnLoad02"))   //短边清洁上料2    
		{   
			m_pModules[i] = new CRunProcessCleanOnLoad(i);
			m_pShortCleanOnLoad[1] = (CRunProcessCleanOnLoad*)m_pModules[i];
		}
		else if(strSection == _T("ShortEdgeClean02"))   //短边清洁2   
		{   
			m_pModules[i] = new CRunProcessRunProcessShortEdgeClean(i);
			m_pShortEdgeClean[1] = (CRunProcessRunProcessShortEdgeClean*)m_pModules[i];
		}
		else if(strSection == _T("ShortEdgeFeeder02"))  //短边供料2
		{
			m_pModules[i] = new CRunProcessRunProcessShortEdgeFeeder(i);
			m_pShortEdgeFeeder[1] = (CRunProcessRunProcessShortEdgeFeeder*)m_pModules[i];
		}
		else if(strClass == _T("CRunProcessDamperMylarUp05"))
		{
			m_pModules[i] = new CRunProcessDamperMylarUp05(i);
			m_pDamperMylarUp05 = (CRunProcessDamperMylarUp05*)m_pModules[i];
		}
		else if(strClass == _T("CRunProcessDistributorCoreDustBefore"))
		{
			m_pModules[i] = new CRunProcessDistributorCoreDustBefore(i);
			m_pDistributorCoreDustBefore = (CRunProcessDistributorCoreDustBefore*)m_pModules[i];
		}
		else if(strClass == _T("CRunProcessCoreDust"))
		{
			m_pModules[i] = new CRunProcessCoreDust(i);
			m_pCoreDust = (CRunProcessCoreDust*)m_pModules[i];
		}
		else if(strClass == _T("CRunProcessDistributorOnLoadBefore"))
		{
			m_pModules[i] = new CRunProcessDistributorOnLoadBefore(i);
			m_pDistributorOnLoadBefore = (CRunProcessDistributorOnLoadBefore*)m_pModules[i];
		}
		else if(strClass == _T("CRunProcessCoreOnLoad1"))
		{
			m_pModules[i] = new CRunProcessCoreOnLoad1(i);
			m_pCoreOnLoad1 = (CRunProcessCoreOnLoad1*)m_pModules[i];
		}
		else if(strClass == _T("CRunProcessCoreOnLoad2"))
		{
			m_pModules[i] = new CRunProcessCoreOnLoad2(i);
			m_pCoreOnLoad2 = (CRunProcessCoreOnLoad2*)m_pModules[i];
		}
		else if(strClass == _T("CRunProcessElevatorCore"))
		{
			m_pModules[i] = new CRunProcessElevatorCore(i);
			m_pElevatorCore = (CRunProcessElevatorCore*)m_pModules[i];
		}
		else if(strClass == _T("CRunProcessDistributorPlateDustBefore"))
		{
			m_pModules[i] = new CRunProcessDistributorPlateDustBefore(i);
			m_pDistributorPlateDustBefore = (CRunProcessDistributorPlateDustBefore*)m_pModules[i];
		}
		else if(strClass == _T("CRunProcessPlateDust"))
		{
			m_pModules[i] = new CRunProcessPlateDust(i);
			m_pPlateDust = (CRunProcessPlateDust*)m_pModules[i];
		}
		else if(strClass == _T("CRunProcessDamperCoreDown08"))
		{
			m_pModules[i] = new CRunProcessDamperCoreDown08(i);
			m_pDamperCoreDown08 = (CRunProcessDamperCoreDown08*)m_pModules[i];
		}
		else if(strClass == _T("CRunProcessDamperCoreDown06"))
		{
			m_pModules[i] = new CRunProcessDamperCoreDown06(i);
			m_pDamperCoreDown06 = (CRunProcessDamperCoreDown06*)m_pModules[i];
		}
		else if(strClass == _T("CRunProcessDamperMylarDown04"))
		{
			m_pModules[i] = new CRunProcessDamperMylarDown04(i);
			m_pDamperMylarDown04 = (CRunProcessDamperMylarDown04*)m_pModules[i];
		}
		else if(strClass == _T("CRunProcessDamperMylarDown03"))
		{
			m_pModules[i] = new CRunProcessDamperMylarDown03(i);
			m_pDamperMylarDown03 = (CRunProcessDamperMylarDown03*)m_pModules[i];
		}
		else if(strClass == _T("CRunProcessDistributorMylarOnLoad"))
		{
			m_pModules[i] = new CRunProcessDistributorMylarOnLoad(i);
			m_pDistributorMylarOnLoad = (CRunProcessDistributorMylarOnLoad*)m_pModules[i];
		}
		else
		{
			m_pModules[i] = new CRunProcess(i);
		}

		// write to module config
		m_pModules[i]->InitializeConfig(strSection);
		CArray<int,int> inputs, outputs, motors;
		m_pModules[i]->GetHardwareConfig(&inputs, &outputs, &motors);
		WriteModuleCfg(i+1, m_pModules[i]->m_strName, &inputs, &outputs, &motors);
	}

	for (int i = 0; i < m_strModuleArray.GetCount(); i++)
	{
		if (!m_pModules[i]->m_strParent.IsEmpty())
		{
			for (int n = 0; n < m_strModuleArray.GetCount(); n++)
			{
				if (m_strModuleArray[n] == m_pModules[i]->m_strParent)
				{
					m_pModules[i]->m_pFather = m_pModules[n];
					break;
				}
			}
		}

		if (!m_pModules[i]->m_strChild.IsEmpty())
		{
			for (int n = 0; n < m_strModuleArray.GetCount(); n++)
			{
				if (m_strModuleArray[n] == m_pModules[i]->m_strChild)
				{
					m_pModules[i]->m_pChild = m_pModules[n];
					break;
				}
			}
		}

		if (!m_pModules[i]->m_strNormalFront.IsEmpty())
		{
			for (int n = 0; n < m_strModuleArray.GetCount(); n++)
			{
				if (m_strModuleArray[n] == m_pModules[i]->m_strNormalFront)
				{
					m_pModules[i]->m_pNormalFront = m_pModules[n];
					break;
				}
			}
		}

		//分配工作指针
		if (!m_pModules[i]->m_strNormalNext.IsEmpty())
		{
			for (int n = 0; n < m_strModuleArray.GetCount(); n++)
			{
				if (m_strModuleArray[n] == m_pModules[i]->m_strNormalNext && m_pModules[i]->m_strClass==_T("CRunProcessCleanFrontDistributor"))
				{
					((CRunProcessCleanFrontDistributor*)m_pModules[i])->m_pClaenOnLoad1 = (CRunProcessCleanOnLoad*)m_pModules[n];  //给短边分料分配的指针
					break;
				}

				if (m_strModuleArray[n] == m_pModules[i]->m_strNormalNext && m_pModules[i]->m_strClass==_T("CRunProcessCleanOnLoad"))
				{
					((CRunProcessCleanOnLoad*)m_pModules[i])->m_pOnLoad2 = (CRunProcessCleanOnLoad*)m_pModules[n];  //给短边上料位1分配的指针
					break;
				}

				if (m_strModuleArray[n] == m_pModules[i]->m_strNormalNext && m_pModules[i]->m_strClass==_T("CRunProcessRunProcessShortEdgeClean"))
				{
					((CRunProcessRunProcessShortEdgeClean*)m_pModules[i])->SetCleanOnLoad(m_pModules[n]);  //给短边清洁2分配的指针
					break;
				}

				if (m_strModuleArray[n] == m_pModules[i]->m_strNormalNext && m_pModules[i]->m_strClass==_T("CRunProcessRunProcessLongEdgeClean"))
				{
					((CRunProcessRunProcessLongEdgeClean*)m_pModules[i])->SetCleanOnLoad(m_pModules[n]);  //给长边清洁2分配
					break;
				}

				if (m_strModuleArray[n] == m_pModules[i]->m_strNormalNext && m_pModules[i]->m_strClass==_T("CRunProcessRunProcessShortEdgeFeeder"))
				{
					((CRunProcessRunProcessShortEdgeFeeder*)m_pModules[i])->SetCleanStation(m_pModules[n]);  //给供料2分配指针
					break;//
				}

			}
		}

		if (!m_pModules[i]->m_strBranchFront.IsEmpty())
		{
			for (int n = 0; n < m_strModuleArray.GetCount(); n++)
			{
				if (m_strModuleArray[n] == m_pModules[i]->m_strBranchFront)
				{
					m_pModules[i]->m_pBranchFront = m_pModules[n];
					break;
				}
			}
		}

		if (!m_pModules[i]->m_strNormalNext.IsEmpty())
		{
			for (int n = 0; n < m_strModuleArray.GetCount(); n++)
			{
				if (m_strModuleArray[n] == m_pModules[i]->m_strNormalNext)
				{
					m_pModules[i]->m_pNormalNext = m_pModules[n];
					break;
				}
			}
		}

		if (!m_pModules[i]->m_strBranchNext.IsEmpty())
		{
			for (int n = 0; n < m_strModuleArray.GetCount(); n++)
			{
				if (m_strModuleArray[n] == m_pModules[i]->m_strBranchNext)
				{
					m_pModules[i]->m_pBranchNext = m_pModules[n];
					break;
				}
			}
		}
	}

	// initialize the run theads
	CRun **pRuns = new CRun*[m_numModules];
	for (int i = 0; i < m_numModules; i++)
	{
		pRuns[i] = m_pModules[i];
	}
	m_pMaintenance = new CMachineMaintenance;
	m_pMonitor = new CMachineMonitor;

	// the main center control
	m_pRunCtrl = new CRunCtrl();

	if( !m_pRunCtrl )
	{
		PRINTF(_T("Create RunCtrl Thread Failed..."));
		AfxMessageBox(_T("Create RunCtrl Thread Failed"), MB_ICONSTOP | MB_SYSTEMMODAL);
		delete [] pRuns;
		exit(0);
	}
	if(!m_pRunCtrl->Initialize(m_numModules, pRuns, m_pMaintenance, m_pMonitor, hMsgProcessWnd))
	{
		delete [] pRuns;
		exit(0);
	}
	delete [] pRuns;
	// set the run control signals
	int nCount = 0;

	::IniWriteInt(_T("Module0"), _T("InputCount"), 0, GetAbsolutePathName(MODULE_CFG));

	m_nIStart = MACHINE.DecodeInputsID(::IniReadString(_T("System"), _T("IStart"), _T(""), GetAbsolutePathName(MODULEEX_CFG)));
	if (m_nIStart > -1)
	{
		m_pRunCtrl->SetSystemInput(SWITCH_START, INPUTS[m_nIStart]);
		strKey.Format(_T("Input%d"),nCount);
		::IniWriteInt(_T("Module0"), strKey, m_nIStart, GetAbsolutePathName(MODULE_CFG));
		nCount++;
	}

	m_nIStop = MACHINE.DecodeInputsID(::IniReadString(_T("System"), _T("IStop"), _T(""), GetAbsolutePathName(MODULEEX_CFG)));
	if (m_nIStop > -1)
	{
		m_pRunCtrl->SetSystemInput(SWITCH_STOP, INPUTS[m_nIStop]);
		strKey.Format(_T("Input%d"),nCount);
		::IniWriteInt(_T("Module0"), strKey, m_nIStop, GetAbsolutePathName(MODULE_CFG));
		nCount++;
	}

	m_nIReset = MACHINE.DecodeInputsID(::IniReadString(_T("System"), _T("IReset"), _T(""), GetAbsolutePathName(MODULEEX_CFG)));
	if (m_nIReset > -1)
	{
		m_pRunCtrl->SetSystemInput(SWITCH_RESET, INPUTS[m_nIReset]);
		strKey.Format(_T("Input%d"),nCount);
		::IniWriteInt(_T("Module0"), strKey, m_nIReset, GetAbsolutePathName(MODULE_CFG));
		nCount++;
	}

	m_nIAuto = MACHINE.DecodeInputsID(::IniReadString(_T("System"), _T("IAuto"), _T(""), GetAbsolutePathName(MODULEEX_CFG)));
	if (m_nIAuto > -1)
	{
		strKey.Format(_T("Input%d"),nCount);
		::IniWriteInt(_T("Module0"), strKey, m_nIAuto, GetAbsolutePathName(MODULE_CFG));
		nCount++;
	}

	m_nIManual = MACHINE.DecodeInputsID(::IniReadString(_T("System"), _T("IManual"), _T(""), GetAbsolutePathName(MODULEEX_CFG)));
	if (m_nIManual > -1)
	{
		strKey.Format(_T("Input%d"),nCount);
		::IniWriteInt(_T("Module0"), strKey, m_nIManual, GetAbsolutePathName(MODULE_CFG));
		nCount++;
	}

	m_nIVacumAlarm = MACHINE.DecodeInputsID(::IniReadString(_T("System"), _T("IVacumAlarm"), _T(""), GetAbsolutePathName(MODULEEX_CFG)));
	if (m_nIVacumAlarm > -1)
	{
		m_pRunCtrl->SetSystemInput(SWITCH_VACUUM, INPUTS[m_nIVacumAlarm]);
		strKey.Format(_T("Input%d"),nCount);
		::IniWriteInt(_T("Module0"), strKey, m_nIVacumAlarm, GetAbsolutePathName(MODULE_CFG));
		nCount++;
	}

	m_nIAirAlarm = MACHINE.DecodeInputsID(::IniReadString(_T("System"), _T("IAirAlarm"), _T(""), GetAbsolutePathName(MODULEEX_CFG)));
	if (m_nIAirAlarm > -1)
	{
		m_pRunCtrl->SetSystemInput(SWITCH_AIR, INPUTS[m_nIAirAlarm]);
		strKey.Format(_T("Input%d"),nCount);
		::IniWriteInt(_T("Module0"), strKey, m_nIAirAlarm, GetAbsolutePathName(MODULE_CFG));
		nCount++;
	}

	m_nIDustAirPressAlarm = MACHINE.DecodeInputsID(::IniReadString(_T("System"), _T("IDustAirPressAlarm"), _T(""), GetAbsolutePathName(MODULEEX_CFG)));
	if (m_nIDustAirPressAlarm > -1)
	{
		strKey.Format(_T("Input%d"),nCount);
		::IniWriteInt(_T("Module0"), strKey, m_nIDustAirPressAlarm, GetAbsolutePathName(MODULE_CFG));
		nCount++;
	}

	::IniWriteInt(_T("Module0"), _T("InputCount"), nCount, GetAbsolutePathName(MODULE_CFG));
	nCount = 0;
	::IniWriteInt(_T("Module0"), _T("OutputCount"), 0, GetAbsolutePathName(MODULE_CFG));

	int index = MACHINE.DecodeOutputsID(::IniReadString(_T("System"), _T("OStartLED"), _T(""), GetAbsolutePathName(MODULEEX_CFG)));
	if (index > -1)
	{
		m_pRunCtrl->SetSystemOutput(SYS_OUTPUT_START_BUTTON_LED, OUTPUTS[index]);
		strKey.Format(_T("Output%d"),nCount);
		::IniWriteInt(_T("Module0"), strKey, index, GetAbsolutePathName(MODULE_CFG));
		nCount++;
	}

	index = MACHINE.DecodeOutputsID(::IniReadString(_T("System"), _T("OStopLED"), _T(""), GetAbsolutePathName(MODULEEX_CFG)));
	if (index > -1)
	{
		m_pRunCtrl->SetSystemOutput(SYS_OUTPUT_STOP_BUTTON_LED, OUTPUTS[index]);
		strKey.Format(_T("Output%d"),nCount);
		::IniWriteInt(_T("Module0"), strKey, index, GetAbsolutePathName(MODULE_CFG));
		nCount++;
	}

	index = MACHINE.DecodeOutputsID(::IniReadString(_T("System"), _T("OResetLED"), _T(""), GetAbsolutePathName(MODULEEX_CFG)));
	if (index > -1)
	{
		m_pRunCtrl->SetSystemOutput(SYS_OUTPUT_RESET_BUTTON_LED, OUTPUTS[index]);
		strKey.Format(_T("Output%d"),nCount);
		::IniWriteInt(_T("Module0"), strKey, index, GetAbsolutePathName(MODULE_CFG));
		nCount++;
	}

	m_nTowerRed = MACHINE.DecodeOutputsID(::IniReadString(_T("System"), _T("OTowerRed"), _T(""), GetAbsolutePathName(MODULEEX_CFG)));
	if (m_nTowerRed > -1)
	{
		strKey.Format(_T("Output%d"),nCount);
		::IniWriteInt(_T("Module0"), strKey, m_nTowerRed, GetAbsolutePathName(MODULE_CFG));
		nCount++;
	}

	m_nTowerAmber = MACHINE.DecodeOutputsID(::IniReadString(_T("System"), _T("OTowerAmber"), _T(""), GetAbsolutePathName(MODULEEX_CFG)));
	if (m_nTowerAmber > -1)
	{
		strKey.Format(_T("Output%d"),nCount);
		::IniWriteInt(_T("Module0"), strKey, m_nTowerAmber, GetAbsolutePathName(MODULE_CFG));
		nCount++;
	}

	m_nTowerGreen = MACHINE.DecodeOutputsID(::IniReadString(_T("System"), _T("OTowerGreen"), _T(""), GetAbsolutePathName(MODULEEX_CFG)));
	if (m_nTowerGreen > -1)
	{
		strKey.Format(_T("Output%d"),nCount);
		::IniWriteInt(_T("Module0"), strKey, m_nTowerGreen, GetAbsolutePathName(MODULE_CFG));
		nCount++;
	}

	m_nTowerBuzzer = MACHINE.DecodeOutputsID(::IniReadString(_T("System"), _T("OTowerBuzzer"), _T(""), GetAbsolutePathName(MODULEEX_CFG)));
	if (m_nTowerBuzzer > -1)
	{
		strKey.Format(_T("Output%d"),nCount);
		::IniWriteInt(_T("Module0"), strKey, m_nTowerBuzzer, GetAbsolutePathName(MODULE_CFG));
		nCount++;
	}

	//以下新添加2018-11-28

	m_nRedLight = MACHINE.DecodeOutputsID(::IniReadString(_T("System"), _T("ORedLight"), _T(""), GetAbsolutePathName(MODULEEX_CFG)));
	if (m_nRedLight > -1)
	{
		strKey.Format(_T("Output%d"),nCount);
		::IniWriteInt(_T("Module0"), strKey, m_nRedLight, GetAbsolutePathName(MODULE_CFG));
		nCount++;
	}

    m_nYellowLight = MACHINE.DecodeOutputsID(::IniReadString(_T("System"), _T("OYellowLight"), _T(""), GetAbsolutePathName(MODULEEX_CFG)));
	if (m_nYellowLight > -1)
	{
		strKey.Format(_T("Output%d"),nCount);
		::IniWriteInt(_T("Module0"), strKey, m_nYellowLight, GetAbsolutePathName(MODULE_CFG));
		nCount++;
	}

	m_nGreenLight = MACHINE.DecodeOutputsID(::IniReadString(_T("System"), _T("OGreenLight"), _T(""), GetAbsolutePathName(MODULEEX_CFG)));
	if (m_nGreenLight > -1)
	{
		strKey.Format(_T("Output%d"),nCount);
		::IniWriteInt(_T("Module0"), strKey, m_nGreenLight, GetAbsolutePathName(MODULE_CFG));
		nCount++;
	}

	m_nLighthouseBuzzer = MACHINE.DecodeOutputsID(::IniReadString(_T("System"), _T("OLighthouseBuzzer"), _T(""), GetAbsolutePathName(MODULEEX_CFG)));
	if (m_nLighthouseBuzzer > -1)
	{
		strKey.Format(_T("Output%d"),nCount);
		::IniWriteInt(_T("Module0"), strKey, m_nLighthouseBuzzer, GetAbsolutePathName(MODULE_CFG));
		nCount++;
	}

	::IniWriteInt(_T("Module0"), _T("OutputCount"), nCount, GetAbsolutePathName(MODULE_CFG));

	delete [] g_pInputNames;
	delete [] g_pOutputNames;

	m_pRunCtrl->m_pBeforeStart = &CMachineCtrl::BeforeStart;
	m_pRunCtrl->m_pBeforeStop = &CMachineCtrl::BeforeStop;

	return TRUE;
}

BOOL CMachineCtrl::BeforeStart()
{
	MACHINE.m_ReportFile.UpdateAlmDuration();		
	if(MACHINE.m_bManualAuto || (MACHINE.m_nIManual > -1 && INPUTS[MACHINE.m_nIManual]->IsOn()))
	{
		BLMessageBox(_T("需将设备切换为自动状态"), MB_ICONWARNING);
		return FALSE;
	}
	if(GetTickCount() - MACHINE.m_dwAndonShowTime < 60*1000)
	{
		MACHINE.m_DlgAndon.ShowWindow(SW_HIDE);
		MACHINE.m_DlgAndonFeed.ShowWindow(SW_HIDE);
	}

	if (MACHINE.m_bEnableAndon)
	{
		CFileFind finder;
		CStdioFile file;
		CString strFilePath(_T("D:\\AndonData\\AndonControl\\ResourControl*.txt"));
		
		//查找文件
		if(finder.FindFile(strFilePath))
		{
			finder.FindNextFile();
			strFilePath = finder.GetFilePath();

			if (file.Open(strFilePath, CFile::modeRead | CFile::typeText))
			{
				CString strContent;
				if (file.ReadString(strContent))
				{
					CString strDestLogFile, strPath, strFileName;
					file.Close();
					strFileName = file.GetFileName();
					strPath = strFilePath.Left(strFilePath.GetLength() - strFileName.GetLength()) + + _T("Backup\\");
					strDestLogFile = strPath + strFileName;
					CStringA strBackUpPath(strPath);
					MakeSureDirectoryPathExists(strBackUpPath.GetBuffer());
					strBackUpPath.ReleaseBuffer();
					CopyFile(strFilePath, strDestLogFile, FALSE);
					DeleteFile(strFilePath);
					if (strContent.Find('1') != -1)
					{
						return FALSE;
					}
				}
			}
		}
	}

	//MES设备状态上传(生产时)
	if (MACHINE.m_bCheckMes)
	{
		if (1 != MACHINE.m_objMes.X_isConnected())
		{
			BLMessageBox(_T("MES未登录,设备不能生产"));
			return FALSE;
		}

		if (1 != MACHINE.m_objMes.X_IsMESOnLine())
		{
			BLMessageBox(_T("欣旺达MES后台不在线,设备不能生产"));
			return FALSE;
		}

		MACHINE.m_MachineStatus.strRun = L"1";
		MACHINE.m_MachineStatus.strStop = L"0";
		MACHINE.m_MachineStatus.strFault = L"0";
		MACHINE.m_MachineStatus.strWaiting = L"0";
		MACHINE.m_MachineStatus.strErrCode = L"";
		MACHINE.m_objMes.X_UpdateStatus(MACHINE.m_MachineStatus);
	}

	return TRUE;
}

void CMachineCtrl::BeforeStop()
{
	if (MACHINE.m_bCheckMes && 1 == MACHINE.m_objMes.X_isConnected())
	{
		MACHINE.m_MachineStatus.strRun = L"0";
		MACHINE.m_MachineStatus.strStop = L"1";
		MACHINE.m_MachineStatus.strFault = L"0";
		MACHINE.m_MachineStatus.strWaiting = L"0";
		MACHINE.m_MachineStatus.strErrCode = L"";
		MACHINE.m_objMes.X_UpdateStatus(MACHINE.m_MachineStatus);
	}
	MACHINE.m_bManualAuto = TRUE;
}

int CMachineCtrl::DecodeInputsID(CString strID)
{
	// empty
	if (strID.IsEmpty())
		return -1;

	// load the strings
	if (!g_pInputNames)
	{
		CString strSection, strName;
		g_pInputNames = new CString[m_numInputs];
		if (!g_pInputNames)
			return -1;
		for (int i = 0; i < m_numInputs; i++)
		{
			strSection.Format(_T("INPUT%d"), i);
			strName = ::IniReadString(strSection, _T("Name"), _T(""), GetAbsolutePathName(INPUT_CFG));
			g_pInputNames[i] = strName.Left(5);
		}
	}

	// search the input names
	int id = -1;
	for (int i = 0; i < m_numInputs; i++)
	{
		if (g_pInputNames[i] == strID)
		{
			if (id == -1)
				id = i;
			else
			{
				// duplicate
				CString strMsg;
				strMsg.Format(_T("Duplicate input ID %s!"), strID);
				AfxMessageBox(strMsg, MB_ICONSTOP);
				exit(0);
			}
		}
	}

	if (id < 0)
	{
		CString strMsg;
		strMsg.Format(_T("%s configuration not found in input config file!\r\n"), strID);
		OutputDebugString(strID);
		OutputDebugString(_T("\r\n"));
		AfxMessageBox(strMsg);
	}
	return id;
}

int CMachineCtrl::DecodeOutputsID(CString strID)
{
	// empty?
	if (strID.IsEmpty())
		return -1;

	// load the strings
	if (!g_pOutputNames)
	{
		CString strSection, strName;
		g_pOutputNames = new CString[m_numOutputs];
		if (!g_pOutputNames)
			return -1;
		for (int i = 0; i < m_numOutputs; i++)
		{
			strSection.Format(_T("OUTPUT%d"), i);
			strName = ::IniReadString(strSection, _T("Name"), _T(""), GetAbsolutePathName(OUTPUT_CFG));
			g_pOutputNames[i] = strName.Left(5);
		}
	}

	// search the input names
	int id = -1;
	for (int i = 0; i < m_numOutputs; i++)
	{
		if (g_pOutputNames[i] == strID)
		{
			if (id == -1)
				id = i;
			else
			{
				// duplicate
				CString strMsg;
				strMsg.Format(_T("Duplicate output ID %s!"), strID);
				AfxMessageBox(strMsg, MB_ICONSTOP);
				exit(0);
			}
		}
	}

	if (id < 0)
	{
		CString strMsg;
		strMsg.Format(_T("%s configuration not found in output config file!"), strID);
		OutputDebugString(strID);
		OutputDebugString(_T("\r\n"));
		AfxMessageBox(strMsg);
	}

	return id;
}

/*********************************************************
	功能：检查并创建多级文件目录
	输入：
	strPath需要检查或创建的文件目录
	输出：无
	返回值：目录存在或不存在但创建成功则返回TRUE，否则返回FALSE	
	*********************************************************/
BOOL CMachineCtrl::CreateMultipleDirectory(const CString& strPath)
{
	if (PathFileExists(strPath))
	{
		return TRUE;
	}

	CString strDir(strPath);
	vector<CString>vPath;
	CString strTemp;
	BOOL bResult = FALSE;
	for (int i=0; i < strDir.GetLength(); i++)
	{
		if (strDir.GetAt(i) != _T('\\'))
		{
			strTemp.AppendChar(strDir.GetAt(i));
		}
		else
		{
			vPath.push_back(strTemp);
			strTemp.AppendChar(_T('\\'));
		}
	}
	vector<CString>::const_iterator vIter;
	for (vIter = vPath.begin(); vIter != vPath.end(); vIter++)
	{
		bResult = FALSE;
		if (PathFileExists(*vIter))
		{
			bResult = TRUE;
		}
		else if (CreateDirectory(*vIter, NULL))
		{
			bResult = TRUE;
		}
	}
	return bResult;
}

void CMachineCtrl::WriteLog(CString FileName,CString strText)
{
	CStdioFile sfile;

	if (CreateMultipleDirectory(FileName))
	{
		if(!sfile.Open(FileName,CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate|CFile::typeText))
			return;

		sfile.SeekToEnd();
		char* old_locale = _strdup( setlocale(LC_CTYPE,NULL));
		setlocale( LC_CTYPE, "chs" );
		sfile.WriteString(strText+_T("\r\n"));
		setlocale(LC_CTYPE, old_locale);
		free(old_locale);
		sfile.Close();
	}
}

CRunProcess* CMachineCtrl::GetRun(CString strModuleName)
{
	for (int n = 0; n < m_strModuleArray.GetCount(); n++)
	{
		if(strModuleName == m_strModuleArray[n])
		{
			if(m_pModules[n] != NULL)
			{
				return m_pModules[n];
			}
		}
	}
	return NULL;
}

void CMachineCtrl::UpdateLanguage()
{
	CCentralControl::UpdateLanguage();
	for (int i = 0; i < m_numModules; i++)
	{
		if(m_pModules[i] != NULL)
		{
			m_pModules[i]->UpdateLanguage();
		}
	}
}

void CMachineCtrl::ShowMain( CWnd *pParentWnd, CRect rcDesired,CRunProcess * process /*= NULL*/,BOOL bOperator )
{
	if (process == NULL)
	{
		return;
	}
	int nNum =0;
	CArray<int,int> inputs, outputs, motors;
	process->GetHardwareConfig(&inputs, &outputs, &motors);
	WriteModule(nNum,process->m_strName, &inputs, &outputs, &motors);
	nNum++;
	if (process->m_pFather != NULL)
	{
		process->m_pFather->GetHardwareConfig(&inputs, &outputs, &motors);
		MACHINE.WriteModule(nNum,process->m_pFather->m_strName, &inputs, &outputs, &motors);
		nNum++;
		if (process->m_pFather->m_pBranchFront != NULL)
		{
			process->m_pFather->m_pBranchFront->GetHardwareConfig(&inputs, &outputs, &motors);
			MACHINE.WriteModule(nNum,process->m_pFather->m_pBranchFront->m_strName, &inputs, &outputs, &motors);
			nNum++;
		}
	}

	if (process->m_pBranchFront != NULL)
	{
	}
	if (process->m_pChild != NULL)
	{
		process->m_pChild->GetHardwareConfig(&inputs, &outputs, &motors);
		MACHINE.WriteModule(nNum,process->m_pChild->m_strName, &inputs, &outputs, &motors);
		nNum++;
	}
	::IniWriteInt(_T("Modules"), _T("NumModule"),nNum, GetAbsolutePathName(_T("System\\ModuleDlg\\ModuleDlg.cfg")));
	Sleep(10);
	MACHINE.m_pRunCtrl->m_bRunningEnableMaintance  = bOperator;
	MODULES.Initialize(::GetAbsolutePathName(_T("System\\ModuleDlg\\ModuleDlg.cfg")));
	ShowMaintenanceDialog(pParentWnd,rcDesired);
}

BOOL CMachineCtrl::WriteModule(int index, CString strName, CArray<int, int> *pInputs, CArray<int, int> *pOutputs, CArray<int, int> *pMotors)
{
	CString strSection, strKey, strValue;
	strSection.Format(_T("Module%d"), index);
	// nam
	::IniWriteString(strSection, _T("Name"), strName, GetAbsolutePathName(_T("System\\ModuleDlg\\ModuleDlg.cfg")));
	// input
	int count = pInputs ? (int)pInputs->GetCount() : 0;
	::IniWriteInt(strSection, _T("InputCount"), count, GetAbsolutePathName(_T("System\\ModuleDlg\\ModuleDlg.cfg")));
	for (int i=0; i<count; i++)
	{
		strKey.Format(_T("Input%d"), i);
		::IniWriteInt(strSection, strKey, pInputs->GetAt(i), GetAbsolutePathName(_T("System\\ModuleDlg\\ModuleDlg.cfg")));
	}
	// output
	count = pOutputs ? (int)pOutputs->GetCount() : 0;
	::IniWriteInt(strSection, _T("OutputCount"), count, GetAbsolutePathName(_T("System\\ModuleDlg\\ModuleDlg.cfg")));
	for (int i=0; i<count; i++)
	{
		strKey.Format(_T("Output%d"), i);
		::IniWriteInt(strSection, strKey, pOutputs->GetAt(i), GetAbsolutePathName(_T("System\\ModuleDlg\\ModuleDlg.cfg")));
	}

	// motor
	count = pMotors ? (int)pMotors->GetCount() : 0;
	::IniWriteInt(strSection, _T("MotorCount"), count, GetAbsolutePathName(_T("System\\ModuleDlg\\ModuleDlg.cfg")));
	for (int i=0; i<count; i++)
	{
		strKey.Format(_T("Motor%d"), i);
		::IniWriteInt(strSection, strKey, pMotors->GetAt(i), GetAbsolutePathName(_T("System\\ModuleDlg\\ModuleDlg.cfg")));
	}
	return TRUE;
}

BOOL CMachineCtrl::ReadMachineParamter()
{
	CString strKey;

	for (int i = 0; i < m_numModules; i++)
	{
		if(m_pModules[i] != NULL)
		{
			m_pModules[i]->m_ParameterLock.Lock();
		}
	}

	m_dbRecord.LoadProductEX(m_vecProduct);
	
	// 产品参数读取 区分不同产品文件
	for (int i=0;i < (int)m_vecProduct.size();i++)
	{
		m_vecProduct[i].m_MachineParameter = new CMachineParamter();
		m_vecProduct[i].m_MachineParameter->Init();
		strKey.Format(_T("Machine%d"),m_vecProduct[i].m_nIndex);
		((CMachineParamter*)m_vecProduct[i].m_MachineParameter)->m_bEnable = ::IniReadBoolean(strKey,_T("Enable"),TRUE,GetAbsolutePathName(PARAMETER_CFG));
	}

	for (int i = 0; i < m_numModules; i++)
	{
		if(m_pModules[i] != NULL)
		{
			m_pModules[i]->m_ParameterLock.Unlock();
		}
	}
	return TRUE;
}
/****************************************************************************************
函数描述：计算机台内电芯数量
输入参数;
bCountAll:计算类型  TRUE;所有电芯数量   FALSE：未入壳电芯数量
函数返回：返回机台内未入电芯铝壳数量
*****************************************************************************************/
int CMachineCtrl::CoreCountForClear(BOOL bCountAll)
{
	int nNum = 0;
	
	//工位是否已经计数
	BOOL bBatteryCoreCount = FALSE;
	BOOL bBatteryInShellCount = FALSE;
	BOOL bBatteryOffloadPnpCount = FALSE;
	
	for (int i = 0; i < m_numModules; i++)
	{
		if (m_pModules[i] != NULL)
		{
			if (m_pModules[i]->m_strModuleName == _T("OnloadCore"))
			{
				if (m_pOnloadCore->m_pTempPallet[0].m_bIsValid 
					&& m_pOnloadCore->m_pTempPallet[1].m_bIsValid)
				{
					nNum+=2;
				}

				else if (m_pOnloadCore->m_pTempPallet[1].m_bIsValid 
					&& !m_pOnloadCore->m_pTempPallet[1].m_bIsValid 
					&& m_pOnloadCore->m_pBufferPallet.m_bIsValid)
				{
					nNum+=2;
				}

				else if (!m_pOnloadCore->m_pTempPallet[1].m_bIsValid 
					&& m_pOnloadCore->m_pTempPallet[1].m_bIsValid 
					&& m_pOnloadCore->m_pBufferPallet.m_bIsValid)
				{
					nNum+=2;
				}
			}

			if (!bBatteryCoreCount 
				&& (m_pModules[i]->m_strModuleName == _T("BatteryCore0") || m_pModules[i]->m_strModuleName == _T("BatteryCore1")))
			{
				if (m_pBatteryCore[0]->m_Pallet.m_bIsValid || m_pBatteryCore[1]->m_Pallet.m_bIsValid)
				{
					nNum+=2;
					bBatteryCoreCount = TRUE;
				}
			}

			if (bCountAll)
			{
				if (!bBatteryInShellCount 
					&& (m_pModules[i]->m_strModuleName == _T("BatteryInShell0") || m_pModules[i]->m_strModuleName == _T("BatteryInShell1")))
				{
					if (m_pBatteryInShell[0]->m_Pallet.m_bIsValid && m_pBatteryInShell[0]->m_bShellVaild 
						|| m_pBatteryInShell[1]->m_Pallet.m_bIsValid && m_pBatteryInShell[1]->m_bShellVaild)
					{
						nNum+=2;
						bBatteryInShellCount = TRUE;
					}
				}

				if (!bBatteryOffloadPnpCount 
					&& (m_pModules[i]->m_strModuleName == _T("BatteryInShellOffloadPnp0") || m_pModules[i]->m_strModuleName == _T("BatteryInShellOffloadPnp1")))
				{
					if (m_pBatteryOffloadPnp[0]->m_Pallet.m_bIsValid || m_pBatteryOffloadPnp[1]->m_Pallet.m_bIsValid)
					{
						nNum+=2;
						bBatteryOffloadPnpCount = TRUE;
					}
				}
			}

			if (m_pModules[i]->m_strModuleName == _T("RobotOffload"))
			{
				if (m_pRobotOffload->m_pTempPallet[0].m_bIsValid && m_pRobotOffload->m_pTempPallet[1].m_bIsValid)
				{
					nNum+=2;
				}
				else if (m_pRobotOffload->m_pTempPallet[1].m_bIsValid 
					&& !m_pRobotOffload->m_pTempPallet[1].m_bIsValid 
					&& m_pRobotOffload->m_pBufferPallet.m_bIsValid)
				{
					nNum+=2;
				}

				else if (!m_pRobotOffload->m_pTempPallet[1].m_bIsValid 
					&& m_pRobotOffload->m_pTempPallet[1].m_bIsValid 
					&& m_pRobotOffload->m_pBufferPallet.m_bIsValid)
				{
					nNum+=2;
				}
			}
		}
	}
	for (int i = 0; i < 2; i++)
	{
		if (MACHINE.m_pReadCode[i] != NULL)
		{
			if (MACHINE.m_pReadCode[i]->m_Pallet.m_bIsValid)
			{
				nNum++;
			}
		}
	}
	return nNum;
}

void CMachineCtrl::AndonDataReport(const int nCurDire, CString strLog)
{
	if(m_bCreateAndonReport)
		return;
	m_bCreateAndonReport = TRUE;

	SYSTEMTIME systime;
	GetLocalTime(&systime);
	CString strTemp, strCurMillisec, strCurTime;
	strTemp.Format(_T("%d"), systime.wMilliseconds);

	if (strTemp.GetLength() == 1)
	{
		strCurMillisec.Format(_T("00%s"),strTemp);
	}
	else if(strTemp.GetLength() == 2)
	{
		strCurMillisec.Format(_T("0%s"),strTemp);
	}
	else
	{
		strCurMillisec.Format(_T("%s"),strTemp);
	}

	strCurTime.Format(_T("%4d%02d%02d%02d%02d%02d%s"), systime.wYear, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute, systime.wSecond, strCurMillisec);

	CString strCurLogFile, strDestLogFile,strFileTitle;

	if (nCurDire == ResourceStatu)
	{
		strCurLogFile.Format(_T("D:\\temp\\AndonStatus\\ResourceStatus_%s.csv"), strCurTime);
		strDestLogFile.Format(_T("D:\\AndonData\\AndonStatus\\ResourceStatus_%s.csv"), strCurTime);
		strFileTitle = _T("ResourceID,Time,DeviceStatus_Reason\n");
	}
	else if (nCurDire == ResourceAlarm)
	{
		strCurLogFile.Format(_T("D:\\temp\\AndonAlarm\\ResourceAlarm_%s.csv"), strCurTime);
		strDestLogFile.Format(_T("D:\\AndonData\\AndonAlarm\\ResourceAlarm_%s.csv"), strCurTime);
		strFileTitle = _T("ResourceID,Time,DeviceStatus_Reason,AlarmAddress1,AlarmAddress2,AlarmAddress3,AlarmAddress4,AlarmAddress5,AlarmAddress6,AlarmAddress7,AlarmAddress8,AlarmAddress9,AlarmAddress10,AlarmAddress11,AlarmAddress12,AlarmAddress13,AlarmAddress14,AlarmAddress15,AlarmAddress16,AlarmAddress17,AlarmAddress18,AlarmAddress19,AlarmAddress20\n");
	}
	else if (nCurDire == ResourceOutput)
	{
		strCurLogFile.Format(_T("D:\\temp\\AndonOutput\\ResourceOutput_%s.csv"), strCurTime);
		strDestLogFile.Format(_T("D:\\AndonData\\AndonOutput\\ResourceOutput_%s.csv"), strCurTime);
		strFileTitle = _T("ResourceID,Time,QTY_INPUT,QTY_TOTAL,QTY_YIELD,QTY_SCRAP\n");
	}
	else if (nCurDire == ResourceLight)
	{
		strCurLogFile.Format(_T("D:\\temp\\AndonLight\\ResourceLight_%s.csv"), strCurTime);
		strDestLogFile.Format(_T("D:\\AndonData\\AndonLight\\ResourceLight_%s.csv"), strCurTime);
		strFileTitle = _T("ResourceID,Time,Light\n");
	}
	
	strTemp.Empty();
	if (!PathFileExists(strDestLogFile))
	{
		CString strDir(strDestLogFile);
		vector<CString> vecPath;

		for (int i = 0; i < strDir.GetLength(); i++)
		{
			if (strDir.GetAt(i) != _T('\\'))
			{
				strTemp.AppendChar(strDir.GetAt(i));
			}
			else
			{
				vecPath.push_back(strTemp);
				strTemp.AppendChar(_T('\\'));
			}
		}
		vector<CString>::const_iterator vIter;
		for (vIter = vecPath.begin(); vIter != vecPath.end(); vIter++)
		{
			if (!PathFileExists(*vIter))
			{
				CreateDirectory(*vIter, NULL);
			}
		}
	}

	strTemp.Empty();
	if (!PathFileExists(strCurLogFile))
	{
		CString strDir(strCurLogFile);
		vector<CString>vPath;

		for (int i = 0; i < strDir.GetLength(); i++)
		{
			if (strDir.GetAt(i) != _T('\\'))
			{
				strTemp.AppendChar(strDir.GetAt(i));
			}
			else
			{
				vPath.push_back(strTemp);
				strTemp.AppendChar(_T('\\'));
			}
		}
		vector<CString>::const_iterator vIter;
		for (vIter = vPath.begin(); vIter != vPath.end(); vIter++)
		{
			if (!PathFileExists(*vIter))
			{
				CreateDirectory(*vIter, NULL);
			}
		}
	}

	CStdioFile sfile;
	FILE *file = NULL;
	CFileFind fileFind;

	if (!fileFind.FindFile(strCurLogFile))
	{
		if (_tfopen_s(&file, strCurLogFile, _T("w+, ccs=UTF-8")) != 0)
		{
			m_bCreateAndonReport = FALSE;
			return;
		}
		_ftprintf_s(file, strFileTitle);
	}
	else
	{
		if (_tfopen_s(&file, strCurLogFile, _T("a+, ccs=UTF-8")) != 0)
		{
			m_bCreateAndonReport = FALSE;
			return;
		}
	}
	_ftprintf_s(file, _T("%s\r"), strLog);
	//拷贝文件到Andon目录
	fclose(file);
	CopyFile(strCurLogFile, strDestLogFile, FALSE);
	
	m_bCreateAndonReport = FALSE;
}

//安东上传三色灯
void CMachineCtrl::AndonLight(CString StrResourceId, CString StrLight)
{
	CString strCurTime = COleDateTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S"));
	CString StrData;
	StrData.Format(_T("%s,%s,%s"),
		StrResourceId.GetBuffer(), strCurTime.GetBuffer(), StrLight.GetBuffer());
	AndonDataReport(ResourceLight, StrData);
	Sleep(5);
}


//安东上传机器状态和停机原因
void CMachineCtrl::AndonMachineStatus(CString StrResourceId, int nMachineStatusCode)
{
	CString strCurTime = COleDateTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S"));
	CString StrData;
	StrData.Format(_T("%s,%s,%d"), StrResourceId.GetBuffer(), strCurTime.GetBuffer(), nMachineStatusCode);
	AndonDataReport(ResourceStatu, StrData);
	Sleep(5);
}

//安东上传报警
void CMachineCtrl::AndonMachineAlarms(CString StrResourceId, int nMachineStatusCode, vector<CString> AlarmList)
{
	CString strCurTime = COleDateTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S"));
	CString StrData;
	StrData.Format(_T("%s,%s,%d"), StrResourceId.GetBuffer(), strCurTime.GetBuffer(), nMachineStatusCode);

	for (int i = 0; i <20; i++)
	{
		CString StrTemp;
		if (i < (int)AlarmList.size())
		{
			StrTemp.Format(_T(",%s"), AlarmList[i]);
		}
		else
		{
			StrTemp=_T(",");
		}
		StrData = StrData + StrTemp;
	}
	AndonDataReport(ResourceAlarm, StrData);
	Sleep(5);
}

//安东上传总量
void CMachineCtrl::AndonMachineTotal(CString StrResourceId, int nInputTotal, int nOutputTotal, int nOKTotal, int nNGTotal)
{
	CString strCurTime = COleDateTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S"));
	CString StrData;
	StrData.Format(_T("%s,%s,%d,%d,%d,%d"),
		StrResourceId.GetBuffer(), strCurTime.GetBuffer(), nInputTotal, nOutputTotal, nOKTotal, nNGTotal);
	AndonDataReport(ResourceOutput, StrData);
	Sleep(5);
}

void  CMachineCtrl::Release()
{
	m_strInprocess = _T("");
	m_strInstation = _T("");
}


/************************
函数说明：删除MES本地数据
**************************/
BOOL CMachineCtrl::DeleteMesData()
{
	COleDateTime time=COleDateTime::GetCurrentTime();
	if (time.GetHour() == 8 && time.GetMinute() == 0 && time.GetSecond() > 0 &&  time.GetSecond() < 10)
	{
		int nListCount = 1;
		CString strMsg,strFirstName;
		CFileFind fileFinder;
		WIN32_FIND_DATA pNextInfo;
		HANDLE handle = FindFirstFile(MACHINE.m_strMesDataWay + _T("*.csv"),&pNextInfo);
		if (handle == INVALID_HANDLE_VALUE)
		{
			strMsg.Format(_T("查找MES本地数据目录失败"));
			BLMessageBox(strMsg);
			return FALSE;
		}
		strFirstName = pNextInfo.cFileName;
		while (FindNextFile(handle,&pNextInfo))
		{
			++nListCount;
		}
		if (nListCount > MACHINE.m_nDelCycle)
		{
			if (DeleteFile(MACHINE.m_strMesDataWay + strFirstName) == 0)
			{
				strMsg.Format(_T("删除MES本地数据失败"));
				BLMessageBox(strMsg);
				return FALSE;
			}
		}
	}
	return TRUE;
}

/************************
函数说明：删除MESLOG进站日志
**************************/
BOOL CMachineCtrl::DeleteMesInLog()
{
	COleDateTime time=COleDateTime::GetCurrentTime();
	if (time.GetHour() == 8 && time.GetMinute() == 0 && time.GetSecond() > 10 &&  time.GetSecond() < 20)
	{
		int nListCount = 1;
		CString strMsg,strFirstName;
		CFileFind fileFinder;
		WIN32_FIND_DATA pNextInfo;
		HANDLE handle = FindFirstFile(MACHINE.m_strInWay + _T("*.csv"),&pNextInfo);
		if (handle == INVALID_HANDLE_VALUE)
		{
			strMsg.Format(_T("查找MES进站数据目录失败"));
			BLMessageBox(strMsg);
			return FALSE;
		}
		strFirstName = pNextInfo.cFileName;
		while (FindNextFile(handle,&pNextInfo))
		{
			nListCount++;
		}
		if (nListCount > MACHINE.m_nDelCycle)
		{
			if (DeleteFile(MACHINE.m_strInWay + strFirstName) == 0)
			{
				strMsg.Format(_T("删除MES进站数据失败"));
				BLMessageBox(strMsg);
				return FALSE;
			}
		}
	}
	return TRUE;
}

/************************
函数说明：删除MESLOG出站日志
**************************/
BOOL CMachineCtrl::DeleteMesOutLog()
{
	COleDateTime time=COleDateTime::GetCurrentTime();
	if (time.GetHour() == 8 && time.GetMinute() == 0 && time.GetSecond() > 20 &&  time.GetSecond() < 30)
	{
		int nListCount = 1;
		CString strMsg,strFirstName;
		CFileFind fileFinder;
		WIN32_FIND_DATA pNextInfo;
		HANDLE handle = FindFirstFile(MACHINE.m_strMesOutWay + _T("*.csv"),&pNextInfo);
		if (handle == INVALID_HANDLE_VALUE)
		{
			strMsg.Format(_T("查找MES出站数据目录失败"));
			BLMessageBox(strMsg);
			return FALSE;
		}
		strFirstName = pNextInfo.cFileName;
		while (FindNextFile(handle,&pNextInfo))
		{
			nListCount++;
		}
		if (nListCount > MACHINE.m_nDelCycle)
		{
			if (DeleteFile(MACHINE.m_strMesOutWay + strFirstName) == 0)
			{
				strMsg.Format(_T("删除MES出站数据失败"));
				BLMessageBox(strMsg);
				return FALSE;
			}
		}
	}
	return TRUE;
}

void CMachineCtrl::SetBuzzerStatue(BOOL bOn)
{
	m_LBuzzerLock.Lock();
	if (bOn)
	{
		m_bBuzzer =TRUE;
	}
	else
	{
		m_bBuzzer =FALSE;
	}
	m_LBuzzerLock.Unlock();
}

void CMachineCtrl::WriteMesLog(CString strFilePath,CString strText)
{
	m_MesLogLock.Lock();

	CString strTemp;
	CStringA strDirectory;

	strDirectory = strFilePath;
	MakeSureDirectoryPathExists(strDirectory.GetBuffer());
	strDirectory.ReleaseBuffer();

	CStdioFile file;
	if (!file.Open(strFilePath,CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate))
	{
		m_MesLogLock.Unlock();
		BLMessageBox(_T("MES出站数据文件打开，请关闭"));
		return;
	}
	file.ReadString(strTemp);
	file.SeekToEnd();

	char* old_locale = _strdup(setlocale(LC_CTYPE,NULL));
	setlocale(LC_CTYPE,"chs");

	if(strTemp.Find(_T("条码")) < 0 && strTemp.IsEmpty())
	{
		strTemp = _T("电芯条码,开始调用时间,工位号,资源号,dcgroup,上传完成时间,耗时,返回值,返回信息,内容\n");
		file.WriteString(strTemp);
	}
	
	file.WriteString(strText);
	setlocale(LC_CTYPE,old_locale);
	free(old_locale);

	if (file)
	{
		file.Close();
	}
	m_MesLogLock.Unlock();
	return;
}

void CMachineCtrl::WriteMesInLog(CString strFilePath,CString strText)
{
	m_MesLogInLock.Lock();

	CString strTemp;
	CStringA strDirectory;

	strDirectory = strFilePath;
	MakeSureDirectoryPathExists(strDirectory.GetBuffer());
	strDirectory.ReleaseBuffer();

	CStdioFile file;
	if (!file.Open(strFilePath,CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate))
	{
		m_MesLogInLock.Unlock();
		BLMessageBox(_T("MES进站数据文件打开，请关闭"));
		return;
	}
	file.ReadString(strTemp);
	file.SeekToEnd();

	char* old_locale = _strdup(setlocale(LC_CTYPE,NULL));
	setlocale(LC_CTYPE,"chs");

	if (strTemp.Find(_T("条码")) < 0 && strTemp.IsEmpty())
	{
		strTemp = _T("电芯条码,开始调用时间,工位号,资源号,dcgroup,上传完成时间,耗时,开始进行效验后函数返回值,返回值,返回信息\n");
		file.WriteString(strTemp);
	}

	file.WriteString(strText);
	setlocale(LC_CTYPE,old_locale);
	free(old_locale);

	if (file)
	{
		file.Close();
	}
	m_MesLogInLock.Unlock();
	return;
}

