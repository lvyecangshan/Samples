#include "StdAfx.h"
#include "RunProcess.h"
#include "Dbghelp.h"
//#define _TEST_

CRunProcess::CRunProcess(int nID) : CRunEx(nID)
{
	PowerUpRestart();
	InitIO();
	m_bDryRun = FALSE;
	m_bLangulageEng = FALSE;

	m_pFather = NULL;
	m_pChild = NULL;
	m_pBrother = NULL;
	m_pNormalFront = NULL;
	m_pNormalNext = NULL;
	m_pBranchFront = NULL;
	m_pBranchNext = NULL;
}

//IO初始化
BOOL CRunProcess::InitIO()
{
	return TRUE;
}

CRunProcess::~CRunProcess(void)
{
	for (int i = 0;i < (int)m_vecProduct.size();i++)
	{
		if (m_vecProduct[i].m_PkgParameter != NULL)
		{
			delete m_vecProduct[i].m_PkgParameter;
		}
	}
}

void CRunProcess::PowerUpRestart()
{
	CRun::PowerUpRestart();
	m_strMsgEng = _T("Ready");
	m_strMsgChs = _T("准备好");
	m_strMsg = m_bLangulageEng?m_strMsgEng:m_strMsgChs;
	m_bModuleResetFinish = FALSE;
	m_bReCoverMachineAuto = FALSE;
	m_Pallet.Release();
	m_bShellVaild = FALSE;

		//CT
	m_dwModuleStrt = 0;
	m_dwModuleCT = 0;
	m_dwModuleStepCT = 0;
    m_nAutoNext = -1;

	//CT
	m_nSaveLastStep = -1;
	m_nStepStartTime = 0;
	m_nTotalTime = 0;


	m_strModuleCTtemp.Empty();
	for (int i = 0; i < CT_WAITSTEP_TOTAL; i++)
	{
		m_waitStep[i] = -1;
	}

	for (int i = 0; i < STEP_TOTAL; i++)
	{
		m_action[i].strActionName.Empty();
		m_action[i].dwActionTime = 0;
	}
	m_nNextPowerUpStep = PU_FINISH;
	m_nNextAutoStep = AUTO_WAIT_WORK_START;
}

void CRunProcess::PowerUpOperation(void)
{
	if (!IsModuleEnable())
	{
		CRun::PowerUpOperation();
		m_strMsgEng = _T("Moudle not enable");
		m_strMsgChs = _T("模块禁用");
		m_strMsg = m_bLangulageEng?m_strMsgEng:m_strMsgChs;
		return;
	}
	
	switch(m_nNextPowerUpStep)
	{
	case PU_FINISH:
		{
			CRun::PowerUpOperation();
			m_strMsg = m_bLangulageEng?_T("Power up finished"):_T("初始化完成");
			break;
		}
	default:
		ASSERT(0);
		break;
	}
}


void CRunProcess::AutoOperation()
{
	if (!IsModuleEnable())
	{
		m_strMsgEng = _T("Moudle not enable");
		m_strMsgChs = _T("模块禁用");
		m_strMsg = m_bLangulageEng?m_strMsgEng:m_strMsgChs;
		return;
	}


	switch(m_nNextAutoStep)
	{
	case AUTO_WAIT_WORK_START:
		{
			m_strMsg = m_bLangulageEng?_T("Wait start event"):_T("等待开始信号");
			m_nNextAutoStep = AUTO_WORK_END;
			break;
		}
	case AUTO_WORK_END:
		{
			m_strMsg = m_bLangulageEng?_T("Work end"):_T("工作完成");
			break;
		}

	default:
		{
			ASSERT(0);
			break;
		}
	}
}

void CRunProcess::GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors)
{
	CRunEx::GetHardwareConfig(pInputs, pOutputs,pMotors);
}

BOOL CRunProcess::InitializeConfig(CString strProcessModule)
{
	map<int*,CString>::iterator _iter;

	m_strProcess = strProcessModule;
	m_strName = ::IniReadString(strProcessModule, _T("Name"), _T(""), GetAbsolutePathName(MODULEEX_CFG));
	m_strClass = ::IniReadString(strProcessModule, _T("Class"), _T(""), GetAbsolutePathName(MODULEEX_CFG));
	m_strModuleName = strProcessModule;

	m_strNormalFront = ::IniReadString(strProcessModule, _T("NormalFront"), _T(""), GetAbsolutePathName(MODULEEX_CFG));
	m_strBranchFront = ::IniReadString(strProcessModule, _T("BranchFront"), _T(""), GetAbsolutePathName(MODULEEX_CFG));
	m_strNormalNext = ::IniReadString(strProcessModule, _T("NormalNext"), _T(""), GetAbsolutePathName(MODULEEX_CFG));
	m_strBranchNext = ::IniReadString(strProcessModule, _T("BranchNext"), _T(""), GetAbsolutePathName(MODULEEX_CFG));

	m_strParent = ::IniReadString(strProcessModule, _T("ParentModule"), _T(""), GetAbsolutePathName(MODULEEX_CFG));
	m_strChild = ::IniReadString(strProcessModule, _T("ChildModule"), _T(""), GetAbsolutePathName(MODULEEX_CFG));
	m_bDryRun = ::IniReadBoolean(strProcessModule, _T("DryRun"), FALSE, GetAbsolutePathName(MODULEEX_CFG));

	m_nGroupID = ::IniReadInt(strProcessModule, _T("GroupID"), -1, GetAbsolutePathName(MODULEEX_CFG));
	Enable(::IniReadBoolean(strProcessModule, _T("Enable"), TRUE, GetAbsolutePathName(MODULEEX_CFG)));

	for(_iter = m_InputMap.begin(); _iter != m_InputMap.end(); _iter++)
	{
		*(_iter->first) = MACHINE.DecodeInputsID(::IniReadString(strProcessModule, _iter->second, _T(""), GetAbsolutePathName(MODULEEX_CFG)));
	}

	for(_iter = m_OutputMap.begin(); _iter != m_OutputMap.end(); _iter++)
	{
		*(_iter->first) = MACHINE.DecodeOutputsID(::IniReadString(strProcessModule, _iter->second, _T(""), GetAbsolutePathName(MODULEEX_CFG)));
	}

	for(_iter = m_MotorMap.begin(); _iter != m_MotorMap.end(); _iter++)
	{
		*(_iter->first) = ::IniReadInt(strProcessModule, _iter->second, -1, GetAbsolutePathName(MODULEEX_CFG));
	}
	
	CRunEx::InitializeConfig(strProcessModule);
	return TRUE;
}

BOOL CRunProcess::MotorHandMove( CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove )
{
	return TRUE;
}

BOOL CRunProcess::CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{
	return TRUE;
}

BOOL CRunProcess::CheckOutputCanActive(COutput *pOutput)
{
	return TRUE;
}

BOOL CRunProcess::ShowToolsDlg(void)
{
	return FALSE;
}

BOOL CRunProcess::ReadParameter()
{
	if (!MACHINE.m_dbRecord.LoadProduct(m_vecProduct, m_MotorMap))
	{
		return FALSE;
	}
	for (int i = 0;i < (int)m_vecProduct.size(); i++)
	{
		m_vecProduct[i].m_PkgParameter = new CPkgParamter();
	}
	ReadRunParameter();
	return TRUE;
}

BOOL CRunProcess::ReadRunParameter()
{
	CString strKey;
	// 设置参数权限
	// 权限一共分三个级别 与 登录用户等级相关联（管理员0级  调试员1级  生产员2级）
	// 当登录用户权限 >= 参数设置权限 ，该参数才可更改


	//  产品模组参数读取
	for (int i = 0;i < (int)m_vecProduct.size();i++)
	{
		if (m_vecProduct[i].m_PkgParameter != NULL)
		{
			m_vecProduct[i].m_PkgParameter->Init();
			strKey = m_vecProduct[i].m_strName + _T("-Enable");
			((CPkgParamter*)m_vecProduct[i].m_PkgParameter)->m_bEnable = ::IniReadBoolean(m_strProcess,strKey,TRUE,GetAbsolutePathName(PARAMETER_CFG));
		}
	}

	return TRUE;
}


BOOL CRunProcess::SaveParameter()
{
	return TRUE;
}

BOOL CRunProcess::SetParameterEnable(CString strParameter, int nParamLevel)
{
	BOOL bResult = FALSE;
	for (int i = 0; i < (int)m_vecParameterEN.size(); i++)
	{
		if (strParameter == m_vecParameterEN[i].m_strParameter)
		{
			bResult = TRUE;
			m_vecParameterEN[i].m_nParamLevel = nParamLevel;
		}
	}
	if (!bResult)
	{
		PARAMETER_EN ParameterEN;
		ParameterEN.m_strParameter = strParameter;
		ParameterEN.m_nParamLevel = nParamLevel;
		m_vecParameterEN.push_back(ParameterEN);
	}
	return TRUE;
}

BOOL CRunProcess::SaveStepInfo(int nWriteType)
{
	if(nWriteType & WRITE_TYPE_AUTO_STEP)
	{

	}
	
	if(nWriteType & WRITE_TYPE_VARIABLES)
	{

	}
	
	if(nWriteType & WRITE_TYPE_BATTERY)
	{

	}
	return TRUE;
}

void CRunProcess::ReadStepInfo()
{

}

void CRunProcess::CloseAction()
{
}

char* CRunProcess::CStringTochar(CString &strResource)
{
	if(strResource.IsEmpty())
		return NULL;
	int len = strResource.GetLength();
	char * chRtn= (char*)malloc((len*2+1)*sizeof(char));
	memset(chRtn,0,2*len+1);
	USES_CONVERSION;
	strcpy_s((LPSTR)chRtn,strResource.GetLength(), OLE2A(strResource.LockBuffer()));
	return chRtn;
}

BOOL CRunProcess::CreateFilePathDirectory(CString strPath)
{
	CString strTemp;
	CString strFilePath = strPath;
	if(!PathFileExists(strPath))
	{
		strPath.TrimLeft(strPath.Left(3));
		int i = strPath.Find(_T("\\"));
		if(i > 0)
		{
			strTemp = strFilePath.Left(3) + strPath.Left(i);
		}
		else
		{
			strTemp = strFilePath;
		}
		strPath.TrimLeft(strPath.Left(i));
		if(!PathFileExists(strTemp))
		{
			CreateDirectory(strTemp,NULL);
		}
		while(strPath.Find(_T("\\")) == 0)
		{
			strPath.TrimLeft(strPath.Left(1));
			int j = strPath.Find(_T("\\"));
			if(j >0)
			{
				strTemp = strTemp + _T("\\") +strPath.Left(j);
				strPath.TrimLeft(strPath.Left(j));
			}
			else
			{
				strTemp = strTemp + _T("\\") +strPath;			
			}
			if(!PathFileExists(strTemp))
			{
				CreateDirectory(strTemp,NULL);
			}
		}
	}
	return TRUE;
}

void CRunProcess::StationEStop()
{
	return;
}

BOOL CRunProcess::CheckInOutputStatus(BOOL bInput, int nInOutput,BOOL bStatus, BOOL bAlarm/* = FALSE*/)
{
	if(IsNoHardware() || nInOutput < 0)
	{
		return TRUE;
	}

	if(bInput)
	{
		BOOL bCheck = CheckInput(INPUTS[nInOutput], bStatus?TRUE:FALSE, bAlarm?MSG_SHOW_DEFAULT:MSG_NOT_NEED);
		// 	if(!bAlarm && !bCheck && m_bDryRun)
		// 	{
		// 		bCheck = TRUE;
		// 	}
		return bCheck;
	}
	else
	{
		if(bStatus?OUTPUTS[nInOutput]->IsOff():OUTPUTS[nInOutput]->IsOn())
		{
			bStatus?OUTPUTS[nInOutput]->On():OUTPUTS[nInOutput]->Off();
		}
		return TRUE;
	}
}

BOOL CRunProcess::UserPremissionProtect(USER_PERMISSION_PROTECT_TYPE uprType/* = USER_PERMISSION_MOTOR_POS_SAVE*/)
{
	return TRUE;
}

BOOL CRunProcess::ModifyPos(int nPkg,int nMotor,CMotorPos &mPos)
{
	BOOL bRes = MACHINE.m_dbRecord.MotorPosModify(nPkg,nMotor,mPos);
	if(bRes)
	{
		CString strDatabase = GetAbsolutePathName(DATABASE_FILE);
		CString strBkDataBase = GetAbsolutePathName(DATABASE_BACKUP_FILE);
		if(!CopyFile(strDatabase, strBkDataBase, FALSE))
		{
			BLMessageBox(_T("备份数据库失败"), MB_ICONWARNING);
		}
	}
	return TRUE;
}

void CRunProcess::MeasureCT(CString strModuleName, CString strStepName, int nStepNum, int nStartStep, int nEndStep, int* waitStep)
{
    return;
	MACHINE.m_ReportFile.MeasureCT(strModuleName, strStepName, nStepNum, nStartStep, nEndStep, STEP_TOTAL, waitStep, CT_WAITSTEP_TOTAL, &m_CTFunc.nLastStep, &m_CTFunc.nActionCount, m_action, &m_CTFunc.dwCTStartTime, &m_CTFunc.dwModuleCT);
}

void CRunProcess::WriteLog(CString strText)
{
	CString FileName =_T("");
	CString strPath = _T("");
	SYSTEMTIME currentTime;
	GetLocalTime(&currentTime);
	strPath.Format(_T("D:\\CT\\%04d-%02d-%02d\\"),currentTime.wYear,currentTime.wMonth,currentTime.wDay);
	PCSTR AimPath;
	CStringA strTemp(strPath);
	AimPath = strTemp.GetBuffer();
	BOOL bCreate = MakeSureDirectoryPathExists(AimPath);

	FileName = strPath + m_strName + _T(".log");

	CStdioFile sfile;
	if(!sfile.Open(FileName, CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate|CFile::typeText))
	{
		return;
	}
	sfile.SeekToEnd();
	char* old_locale = _strdup( setlocale(LC_CTYPE, NULL) );
	setlocale( LC_CTYPE, "chs" );
	sfile.WriteString(strText+_T("\r\n"));
	setlocale( LC_CTYPE, old_locale );
	free( old_locale );
	sfile.Close();
}

void CRunProcess::APP_MessageBox(CString strMsg)
{
#ifdef _TEST_
	AfxMessageBox(strMsg);
#endif

}

BOOL  CRunProcess::APP_OUTPUTS_On(int index)
{
	if(IsNoHardware())
	{
		return TRUE;
	}
	if(index < 0)
	{
		CString strModule;
		strModule.Format(_T("参数错误!APP_OUTPUTS_On:%s"),this->m_strName);
		APP_MessageBox(strModule);
		return FALSE;
	}
	BOOL bRet = OUTPUTS[index]->On();
	return bRet;
}

BOOL  CRunProcess::APP_OUTPUTS_Off(int index)
{
	if(IsNoHardware())
	{
		return TRUE;
	}
	if(index < 0)
	{
		CString strModule;
		strModule.Format(_T("APP_OUTPUTS_Off的参数错误!：%s"),this->m_strName);
		APP_MessageBox(strModule);
		return FALSE;
	}
	BOOL bRet = OUTPUTS[index]->Off();
	return bRet;
}

BOOL  CRunProcess::APP_OUTPUTS_IsOn(int index)
{
	if(IsNoHardware())
	{
		return TRUE;
	}
	if(index < 0)
	{
		CString strModule;
		strModule.Format(_T("APP_OUTPUTS_IsOn的参数错误!：%s"),this->m_strName);
		APP_MessageBox(strModule);
		return FALSE;
	}
	BOOL bRet = OUTPUTS[index]->IsOn();
	return bRet;
}

BOOL  CRunProcess::APP_OUTPUTS_IsOff(int index)
{
	if(IsNoHardware())
	{
		return TRUE;
	}

	if(index < 0)
	{
		CString strModule;
		strModule.Format(_T("APP_OUTPUTS_IsOff的参数错误!：%s"),this->m_strName);
		APP_MessageBox(strModule);
		return FALSE;
	}

	BOOL bRet = OUTPUTS[index]->IsOff();
	return bRet;
}

BOOL CRunProcess::APP_INPUTS_IsOn(int index)
{
	if(IsNoHardware())
	{
		return TRUE;
	}
	if(index < 0)
	{
		CString strModule;
		strModule.Format(_T("APP_INPUTS_IsOn的参数错误!：%s"),this->m_strName);
		APP_MessageBox(strModule);
		return FALSE;
	}
	BOOL bRet = INPUTS[index]->IsOn();
	return bRet;
}

BOOL CRunProcess::APP_MotorMove( int nMotorID, int nPos, double dlOffse)
{
	if(IsNoHardware())
	{
		return TRUE;
	}

	if(nMotorID < 0)
	{
		CString strModule;
		strModule.Format(_T("APP_MotorMove的参数错误!：%s"),this->m_strName);
		APP_MessageBox(strModule);
		return FALSE;
	}
	return MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), nMotorID, nPos,dlOffse);
}
BOOL CRunProcess::APP_INPUTS_IsOff(int index)
{
	if(index < 0)
	{
		CString strModule;
		strModule.Format(_T("APP_INPUTS_IsOff的参数错误!：%s"),this->m_strName);
		APP_MessageBox(strModule);
		return FALSE;
	}
	BOOL bRet = INPUTS[index]->IsOff();
	return bRet;
}
int  CRunProcess::APP_WaitInput(int index, BOOL bState, int nMsgID /*= MSG_NOT_NEED*/)
{
	if(index < 0)
	{
		CString strModule;
		strModule.Format(_T("APP_WaitInput的参数错误!：%s"),this->m_strName);
		APP_MessageBox(strModule);
		return WAIT_ERROR;
	}
	int nRet = WaitInput(INPUTS[index],bState,nMsgID);
	return nRet;

}
BOOL CRunProcess::APP_CheckInput(int index, BOOL bState, int nMsgID/* = MSG_SHOW_DEFAULT*/)
{
	if(index < 0)
	{
		CString strModule;
		strModule.Format(_T("APP_CheckInput的参数错误!：%s"),this->m_strName);
		APP_MessageBox(strModule);
		return FALSE;
	}
	BOOL bRet = CheckInput(INPUTS[index],bState,nMsgID);
	return bRet;

}
void CRunProcess::APP_Add_Map(map<int*,CString> &Map,int* pVaule,CString strKey)
{
	if(strKey == _T(""))
	{
		return;
	}
	Map.insert(pair<int*, CString>(pVaule, strKey));
}
void CRunProcess::APP_Add_Map(map<void*,CString> &Map,CString* pVaule,CString strKey)
{
	if(strKey == _T(""))
	{
		return;
	}
	Map.insert(pair<void*, CString>(pVaule, strKey));
}
void CRunProcess::APP_Add_Map(map<void*,CString> &Map,int* pVaule,CString strKey)
{
	if(strKey == _T(""))
	{
		return;
	}
	Map.insert(pair<void*, CString>(pVaule, strKey));
}
void CRunProcess::APP_Add_Input(int* pVaule,CString strKey)
{	
	APP_Add_Map(m_InputMap,pVaule,strKey);
}
void CRunProcess::APP_Add_Motor(int* pVaule,CString strKey)
{	
	APP_Add_Map(m_MotorMap,pVaule,strKey);
}

void CRunProcess::APP_Add_Output(int* pVaule,CString strKey)
{
	APP_Add_Map(m_OutputMap,pVaule,strKey);
}

void CRunProcess::APP_Finger_Sleep()
{
	Sleep(1000);
}

void CRunProcess::APP_NoHardware_Sleep()
{
	Sleep(200);
}

//等待信号
void CRunProcess::APP_Wait_Signal_Sleep()
{
	if(IsNoHardware() || m_bDryRun)
	{
		Sleep(1000);
	}

}
int CRunProcess::APP_BLMessageBox(LPCTSTR lpszMessage, UINT nType)
{
	int nRet = -1;
	CString strMsg;
	strMsg.Format(_T("【%s】:"),m_strName);
	strMsg += CString(lpszMessage);
	return BLMessageBox(strMsg,nType);
}

void CRunProcess::APP_ShowMessage(LPCTSTR lpszMessage, UINT nType)
{
	CString strMsg;
	strMsg.Format(_T("【%s】:"),m_strName);
	strMsg += CString(lpszMessage);
	 ShowMessage(strMsg,nType);
	 return;
}

BOOL CRunProcess::SetNeedSingal(BOOL bNeedPlate)
{
	m_bNeedPlate = bNeedPlate;
	return TRUE;
}

BOOL CRunProcess::GetNeedSingal()
{
	return m_bNeedPlate;
}
BOOL CRunProcess::SetFeedSingal(BOOL bFeedPlate)
{
	m_bFeedPlate = bFeedPlate;
	return TRUE;
}

BOOL CRunProcess::GetFeedSingal()
{	
	return m_bFeedPlate;
}

BOOL CRunProcess::GetNextNeedSingal()
{

	return FALSE;
}

void CRunProcess::WriteCTLog(CString strText)
{
	if (!MACHINE.m_bRebuildCTLog)
	{
		return;
	}

	CTime tm = CTime::GetCurrentTime();
	CString str = _T("");
	str.Format(_T("D:\\机台数据文件\\CTLog\\CLog%d-%d-%d\\"), tm.GetYear(), tm.GetMonth(), tm.GetDay());

	CString FileName =_T("");
	FileName = str +  m_strName + _T(".log");

	CStdioFile sfile;
	if(!sfile.Open(FileName,CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate|CFile::typeText))
	{
		return;
	}
	sfile.SeekToEnd();
	char* old_locale = _strdup( setlocale(LC_CTYPE,NULL) );
	setlocale( LC_CTYPE, "chs" );
	sfile.WriteString(strText+_T("\r\n"));
	setlocale( LC_CTYPE, old_locale );
	free( old_locale );
	sfile.Close();
}
