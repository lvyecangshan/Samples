#include "StdAfx.h"
#include "RunProcessReadCodeConveyer.h"
//--读条码线体--
CRunProcessReadCodeConveyer::CRunProcessReadCodeConveyer(int nID) :
CRunProcess(nID)
{
	PowerUpRestart();	
	m_InputMap.insert(pair<int*, CString>(&m_IReadSingle,_T("IReadSingle")));
	m_OutputMap.insert(pair<int*, CString>(&m_OReadFinish,_T("OReadFinish")));

	m_InputMap.insert(pair<int*, CString>(&m_ICheckProduct,_T("ICheckProduct")));
	
	m_VoidMap.insert(pair<void*, CString>(&m_bReadEnable, _T("b是否启用扫码枪, TRUE开启，FALSE屏蔽")));
	m_VoidMap.insert(pair<void*, CString>(&m_strIp, _T("s扫码枪IP, 扫码枪IP")));
	m_VoidMap.insert(pair<void*, CString>(&m_nPort, _T("l扫码枪端口, 扫码枪端口")));
	m_VoidMap.insert(pair<void*, CString>(&m_strScanCmd, _T("s扫码指令, 扫码指令")));
}

CRunProcessReadCodeConveyer::~CRunProcessReadCodeConveyer(void)
{
}

void CRunProcessReadCodeConveyer::PowerUpRestart()
{
	CRunProcess::PowerUpRestart();
	m_strMsgEng = _T("Ready");
	m_strMsgChs = _T("准备好");
	m_nReadCodeTimes =0;
	m_nMesFailCount = 0;
	m_nWaitTime = 0;
	m_Pallet.Release();
	m_bRequirePallet = FALSE;
	m_strMsg = m_bLangulageEng?m_strMsgEng:m_strMsgChs;
	m_nNextPowerUpStep = PU_CONNECT_BAR_CODE;
	m_nNextAutoStep = AUTO_SEND_REQUEST_PALLET_EVENT;
	m_bCanStarScaleCode = FALSE;
}

void CRunProcessReadCodeConveyer::PowerUpOperation(void)
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
	case PU_CONNECT_BAR_CODE:
		{
			m_strMsg = m_bLangulageEng?_T("Connect Bar Code"):_T("连接条码枪");
			if(IsNoHardware() || !m_bReadEnable || m_pReadCode.Connect(m_strIp,m_nPort))
			{
				m_nNextPowerUpStep = PU_FINISH;
			}
			else
			{
				m_pReadCode.Disconnect();
			}
			break;
		}

	case PU_FINISH:
		{
			CRun::PowerUpOperation();
			m_strMsg = m_bLangulageEng?_T("Power up finished"):_T("初始化完成");
			if (m_OReadFinish > -1)
			{
				OUTPUTS[m_OReadFinish]->Off();
			}
			m_bRequirePallet = FALSE;
			m_nReadCodeTimes =0;
			m_nNextAutoStep = AUTO_SEND_REQUEST_PALLET_EVENT;
			break;
		}
	default:
		ASSERT(0);
		break;
	}
}

void CRunProcessReadCodeConveyer::AutoOperation()
{
	if (!IsModuleEnable())
	{
		m_strMsg = m_bLangulageEng?_T("Moudle not enable"):_T("模块禁用");
		Sleep(500);
		return;
	}
	if (IsNoHardware())
	{
		Sleep(200);
	}

	if (m_nSaveLastStep == -1)
	{
		//第一次记录步骤和开始时间
		m_nSaveLastStep = m_nNextAutoStep;
		m_nStepStartTime = GetTickCount();
	}
	if (m_nSaveLastStep != m_nNextAutoStep)
	{
		//第一步记录开始分隔行
		if (m_nSaveLastStep == AUTO_SEND_REQUEST_PALLET_EVENT)
		{
			WriteCTLog(_T("----------开始----------"));
		}
		//每切换一次步骤记录一次CT
		DWORD nStepEndTime = GetTickCount() - m_nStepStartTime;
		CString strText = _T("");
		strText.Format(_T("%s-耗时：%d"),m_strMsg.m_strRunInfo,nStepEndTime);
		WriteCTLog (strText);
		m_nTotalTime += nStepEndTime;
		//最后一步记录总时间
		if (m_nSaveLastStep == AUTO_WORK_END)
		{
			CString str = _T("");
			str.Format(_T("总时间：%d"),m_nTotalTime);
			WriteCTLog (str);
			WriteCTLog (_T("----------结束----------"));
			m_nTotalTime = 0;
		}
		//切换步骤后重新记录新的步骤和开始时间
		m_nSaveLastStep = m_nNextAutoStep;
		m_nStepStartTime = GetTickCount();
	}

	switch(m_nNextAutoStep)
	{
	case AUTO_SEND_REQUEST_PALLET_EVENT:
		{
			m_strMsg = m_bLangulageEng?_T("Send Request Pallet Event"):_T("发请求信号");
		
			m_nNextAutoStep = AUTO_WAIT_PALLET_DATA;
			break;
		}

	case AUTO_WAIT_PALLET_DATA:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Pallet Data"):_T("等待开始扫码信号");

			if (MACHINE.m_bClearMaterial)
			{
				break;
			}
			
			//if (MACHINE.m_bUnEnableLineSingle || m_IReadSingle < 0 || INPUTS[m_IReadSingle]->IsOn() || m_bCanStarScaleCode)//add by ls 20190429
			/*
			if (MACHINE.m_pBarCodeNgLine != NULL && INPUTS[MACHINE.m_pBarCodeNgLine->m_IManualBtn]->IsOn())  //手动上料
			{
				Sleep(1000);
				if (MACHINE.m_pBarCodeNgLine != NULL && INPUTS[MACHINE.m_pBarCodeNgLine->m_IManualBtn]->IsOn())
				{
					m_nNextAutoStep = AUTO_CHECK_LINE_HAVE_CORE;

				}
			}
			*/

			//等待上料机械手通知可扫码  Yao  5.9
			if (MACHINE.m_pReadCode[0]->m_bCanStarScaleCode && MACHINE.m_pReadCode[1]->m_bCanStarScaleCode)
			{
				m_nNextAutoStep = AUTO_CHECK_LINE_HAVE_CORE;
			}
			break;
		}

	case AUTO_CHECK_LINE_HAVE_CORE:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Pallet Data"):_T("检查线体有无产品");

			//if (m_bDryRun || (m_ICheckProduct < 0 || INPUTS[m_ICheckProduct]->IsOn()))
			//if (m_ICheckProduct < 0 || CheckInput(INPUTS[m_ICheckProduct],ON))
			{
				/*if (m_OReadFinish > -1)
				{
				OUTPUTS[m_OReadFinish]->Off();
				}*/
				m_nNextAutoStep = AUTO_JUDGE_NEED_CODE;
			}
			break;
		}

	case AUTO_JUDGE_NEED_CODE:
		{
			m_strMsg = m_bLangulageEng?_T("Judge Need Code"):_T("开始读条码");
		
			//modify by ls 20190425  扫码指令由READ改为02
			CString strCmd=m_strScanCmd;
			strCmd.Trim();
			strCmd += _T("\r\n");
			//end ls  
			CStringA strCmdEx;
			strCmdEx = CStringA(strCmd);
			CString strCodeResult =_T("");

			m_strProcess;
			if(!m_bReadEnable || m_bDryRun || m_pReadCode.ReadCode(strCmdEx.GetBuffer(),strCodeResult))
			{
				strCmdEx.ReleaseBuffer();
				if (!m_bReadEnable || m_bDryRun)
				{
					strCodeResult.Format(_T("%d"),GetTickCount());
				}
				m_Pallet.m_strBarCode =strCodeResult.Left(24);
				m_Pallet.m_bIsNg =FALSE;
				m_Pallet.m_bIsValid =TRUE;
				m_nReadCodeTimes =0;
				m_nNextAutoStep =AUTO_CHECK_CODE_LENGTH;
				m_bCanStarScaleCode = FALSE; //add by ls 20190429
			}
			else
			{
				strCmdEx.ReleaseBuffer();
				m_nReadCodeTimes ++;
				Sleep(100);
				if(m_nReadCodeTimes >=3)
				{
					m_nReadCodeTimes =0;
					m_Pallet.m_bIsNg =TRUE;
					m_Pallet.m_bIsValid =TRUE;
					m_bCanStarScaleCode = FALSE; //add by ls 20190429
					//ShowMessage(_T("扫码失败"),3);
					m_nNextAutoStep =AUTO_WAIT_NEXT_STATION_REQUIRE_EVENT;
					
				}
			}
			break;
		}

	case AUTO_CHECK_CODE_LENGTH:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Next Sation require Event"):_T("MES进站效验条码");

			BOOL bRes= FALSE;
			if(MACHINE.m_bCheckMes)
			{
				CString strResult = _T("");
				if (MACHINE.m_objMes.X_UpdateBarCode(m_Pallet.m_strBarCode,strResult))
				{
					if (strResult == _T("1"))
					{
						bRes = TRUE;
					}
				}

				if(!bRes)
				{
					m_Pallet.m_bIsNg =TRUE;
					MACHINE.m_nUploadMesNgCount ++;
				}
			}
			
			m_nNextAutoStep = AUTO_WAIT_NEXT_STATION_REQUIRE_EVENT;   
			break;
		}

	case AUTO_WAIT_NEXT_STATION_REQUIRE_EVENT:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Next Sation require Event"):_T("给下一工位发送取料信号");

			if (!m_bRequirePallet)
			{
				m_bRequirePallet = TRUE;
				m_nNextAutoStep = AUTO_SEND_PALLET_DATA;
			}
			break;
		}

	case AUTO_SEND_PALLET_DATA:
		{
			m_strMsg = m_bLangulageEng?_T("Send Pallet Data"):_T("等待下一工位取料完成");
		
			if (!m_bRequirePallet)
			{
				if (m_OReadFinish > -1)
				{
					OUTPUTS[m_OReadFinish]->On();
				}
				Sleep(1000);
				m_nWaitTime = GetTickCount();
				m_nNextAutoStep = AUTO_WORK_END;
			}

			break;
		}

	case AUTO_WORK_END:
		{
			m_strMsgEng = _T("Work end");
			m_strMsgChs = _T("工作完成");
			m_strMsg = m_bLangulageEng?m_strMsgEng:m_strMsgChs;
			if (MACHINE.m_bUnEnableLineSingle || m_IReadSingle < 0 || INPUTS[m_IReadSingle]->IsOff())
			{
				if (m_OReadFinish > -1)
				{
					OUTPUTS[m_OReadFinish]->Off();
				}
				m_nNextAutoStep = AUTO_SEND_REQUEST_PALLET_EVENT;
			}	
			else if (MACHINE.m_bUnEnableLineSingle || ((m_IReadSingle < 0 || INPUTS[m_IReadSingle]->IsOn())&& (GetTickCount() - m_nWaitTime > 15000)))
			{
				if (m_OReadFinish > -1)
				{
					OUTPUTS[m_OReadFinish]->Off();
				}
				m_nNextAutoStep = AUTO_SEND_REQUEST_PALLET_EVENT;
			}
			break;
		}
		
	default:
		ASSERT(0);
		break;
	}
}

BOOL CRunProcessReadCodeConveyer::InitializeConfig(CString strProcessModule)
{
	if (!CRunProcess::InitializeConfig(strProcessModule))
		return FALSE;
	return TRUE;
}


void CRunProcessReadCodeConveyer::GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors)
{
	CRunProcess::GetHardwareConfig(pInputs,pOutputs,pMotors);
}
void CRunProcessReadCodeConveyer::UpdateLanguage()
{
	if (GetLanguage() == _T("CHS"))
	{
		m_bLangulageEng = FALSE;
		m_strMsg = m_strMsgChs;
	}
	else
	{
		m_bLangulageEng = TRUE;
		m_strMsg = m_strMsgEng;
	}
}

BOOL CRunProcessReadCodeConveyer::ShowToolsDlg()
{
	return TRUE;
}

BOOL CRunProcessReadCodeConveyer::  ReadRunParameter()
{
	CRunProcess:: ReadRunParameter();
	m_bReadEnable =::IniReadBoolean(m_strProcess, _T("是否启用扫码枪"), TRUE, GetAbsolutePathName(PARAMETER_CFG));
	m_strIp = IniReadString(m_strProcess, _T("扫码枪IP"), _T("192.168.1.125"), GetAbsolutePathName(PARAMETER_CFG));
	m_nPort = IniReadInt(m_strProcess, _T("扫码枪端口"),  10001, GetAbsolutePathName(PARAMETER_CFG));
	m_strScanCmd = IniReadString(m_strProcess, _T("扫码指令"), _T(""), GetAbsolutePathName(PARAMETER_CFG));
	return TRUE;
}

BOOL CRunProcessReadCodeConveyer::SaveParameter()
{
	if (!IniWriteBoolean(m_strProcess, _T("是否启用扫码枪"), m_bReadEnable, GetAbsolutePathName(PARAMETER_CFG)))
	{
		return FALSE;
	}
	if (!IniWriteString(m_strProcess, _T("扫码枪IP"), m_strIp, GetAbsolutePathName(PARAMETER_CFG)))
	{
		return FALSE;
	}
	if (!IniWriteInt(m_strProcess, _T("扫码枪端口"), m_nPort, GetAbsolutePathName(PARAMETER_CFG)))
	{
		return FALSE;
	}
	if (!IniWriteString(m_strProcess, _T("扫码指令"), m_strScanCmd, GetAbsolutePathName(PARAMETER_CFG)))
	{
		return FALSE;
	}
	CRunProcess::SaveParameter();
	return TRUE;
}
