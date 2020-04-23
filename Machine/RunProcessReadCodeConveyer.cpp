#include "StdAfx.h"
#include "RunProcessReadCodeConveyer.h"
//--����������--
CRunProcessReadCodeConveyer::CRunProcessReadCodeConveyer(int nID) :
CRunProcess(nID)
{
	PowerUpRestart();	
	m_InputMap.insert(pair<int*, CString>(&m_IReadSingle,_T("IReadSingle")));
	m_OutputMap.insert(pair<int*, CString>(&m_OReadFinish,_T("OReadFinish")));

	m_InputMap.insert(pair<int*, CString>(&m_ICheckProduct,_T("ICheckProduct")));
	
	m_VoidMap.insert(pair<void*, CString>(&m_bReadEnable, _T("b�Ƿ�����ɨ��ǹ, TRUE������FALSE����")));
	m_VoidMap.insert(pair<void*, CString>(&m_strIp, _T("sɨ��ǹIP, ɨ��ǹIP")));
	m_VoidMap.insert(pair<void*, CString>(&m_nPort, _T("lɨ��ǹ�˿�, ɨ��ǹ�˿�")));
	m_VoidMap.insert(pair<void*, CString>(&m_strScanCmd, _T("sɨ��ָ��, ɨ��ָ��")));
}

CRunProcessReadCodeConveyer::~CRunProcessReadCodeConveyer(void)
{
}

void CRunProcessReadCodeConveyer::PowerUpRestart()
{
	CRunProcess::PowerUpRestart();
	m_strMsgEng = _T("Ready");
	m_strMsgChs = _T("׼����");
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
		m_strMsgChs = _T("ģ�����");
		m_strMsg = m_bLangulageEng?m_strMsgEng:m_strMsgChs;
		return;
	}
	switch(m_nNextPowerUpStep)
	{
	case PU_CONNECT_BAR_CODE:
		{
			m_strMsg = m_bLangulageEng?_T("Connect Bar Code"):_T("��������ǹ");
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
			m_strMsg = m_bLangulageEng?_T("Power up finished"):_T("��ʼ�����");
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
		m_strMsg = m_bLangulageEng?_T("Moudle not enable"):_T("ģ�����");
		Sleep(500);
		return;
	}
	if (IsNoHardware())
	{
		Sleep(200);
	}

	if (m_nSaveLastStep == -1)
	{
		//��һ�μ�¼����Ϳ�ʼʱ��
		m_nSaveLastStep = m_nNextAutoStep;
		m_nStepStartTime = GetTickCount();
	}
	if (m_nSaveLastStep != m_nNextAutoStep)
	{
		//��һ����¼��ʼ�ָ���
		if (m_nSaveLastStep == AUTO_SEND_REQUEST_PALLET_EVENT)
		{
			WriteCTLog(_T("----------��ʼ----------"));
		}
		//ÿ�л�һ�β����¼һ��CT
		DWORD nStepEndTime = GetTickCount() - m_nStepStartTime;
		CString strText = _T("");
		strText.Format(_T("%s-��ʱ��%d"),m_strMsg.m_strRunInfo,nStepEndTime);
		WriteCTLog (strText);
		m_nTotalTime += nStepEndTime;
		//���һ����¼��ʱ��
		if (m_nSaveLastStep == AUTO_WORK_END)
		{
			CString str = _T("");
			str.Format(_T("��ʱ�䣺%d"),m_nTotalTime);
			WriteCTLog (str);
			WriteCTLog (_T("----------����----------"));
			m_nTotalTime = 0;
		}
		//�л���������¼�¼�µĲ���Ϳ�ʼʱ��
		m_nSaveLastStep = m_nNextAutoStep;
		m_nStepStartTime = GetTickCount();
	}

	switch(m_nNextAutoStep)
	{
	case AUTO_SEND_REQUEST_PALLET_EVENT:
		{
			m_strMsg = m_bLangulageEng?_T("Send Request Pallet Event"):_T("�������ź�");
		
			m_nNextAutoStep = AUTO_WAIT_PALLET_DATA;
			break;
		}

	case AUTO_WAIT_PALLET_DATA:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Pallet Data"):_T("�ȴ���ʼɨ���ź�");

			if (MACHINE.m_bClearMaterial)
			{
				break;
			}
			
			//if (MACHINE.m_bUnEnableLineSingle || m_IReadSingle < 0 || INPUTS[m_IReadSingle]->IsOn() || m_bCanStarScaleCode)//add by ls 20190429
			/*
			if (MACHINE.m_pBarCodeNgLine != NULL && INPUTS[MACHINE.m_pBarCodeNgLine->m_IManualBtn]->IsOn())  //�ֶ�����
			{
				Sleep(1000);
				if (MACHINE.m_pBarCodeNgLine != NULL && INPUTS[MACHINE.m_pBarCodeNgLine->m_IManualBtn]->IsOn())
				{
					m_nNextAutoStep = AUTO_CHECK_LINE_HAVE_CORE;

				}
			}
			*/

			//�ȴ����ϻ�е��֪ͨ��ɨ��  Yao  5.9
			if (MACHINE.m_pReadCode[0]->m_bCanStarScaleCode && MACHINE.m_pReadCode[1]->m_bCanStarScaleCode)
			{
				m_nNextAutoStep = AUTO_CHECK_LINE_HAVE_CORE;
			}
			break;
		}

	case AUTO_CHECK_LINE_HAVE_CORE:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Pallet Data"):_T("����������޲�Ʒ");

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
			m_strMsg = m_bLangulageEng?_T("Judge Need Code"):_T("��ʼ������");
		
			//modify by ls 20190425  ɨ��ָ����READ��Ϊ02
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
					//ShowMessage(_T("ɨ��ʧ��"),3);
					m_nNextAutoStep =AUTO_WAIT_NEXT_STATION_REQUIRE_EVENT;
					
				}
			}
			break;
		}

	case AUTO_CHECK_CODE_LENGTH:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Next Sation require Event"):_T("MES��վЧ������");

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
			m_strMsg = m_bLangulageEng?_T("Wait Next Sation require Event"):_T("����һ��λ����ȡ���ź�");

			if (!m_bRequirePallet)
			{
				m_bRequirePallet = TRUE;
				m_nNextAutoStep = AUTO_SEND_PALLET_DATA;
			}
			break;
		}

	case AUTO_SEND_PALLET_DATA:
		{
			m_strMsg = m_bLangulageEng?_T("Send Pallet Data"):_T("�ȴ���һ��λȡ�����");
		
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
			m_strMsgChs = _T("�������");
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
	m_bReadEnable =::IniReadBoolean(m_strProcess, _T("�Ƿ�����ɨ��ǹ"), TRUE, GetAbsolutePathName(PARAMETER_CFG));
	m_strIp = IniReadString(m_strProcess, _T("ɨ��ǹIP"), _T("192.168.1.125"), GetAbsolutePathName(PARAMETER_CFG));
	m_nPort = IniReadInt(m_strProcess, _T("ɨ��ǹ�˿�"),  10001, GetAbsolutePathName(PARAMETER_CFG));
	m_strScanCmd = IniReadString(m_strProcess, _T("ɨ��ָ��"), _T(""), GetAbsolutePathName(PARAMETER_CFG));
	return TRUE;
}

BOOL CRunProcessReadCodeConveyer::SaveParameter()
{
	if (!IniWriteBoolean(m_strProcess, _T("�Ƿ�����ɨ��ǹ"), m_bReadEnable, GetAbsolutePathName(PARAMETER_CFG)))
	{
		return FALSE;
	}
	if (!IniWriteString(m_strProcess, _T("ɨ��ǹIP"), m_strIp, GetAbsolutePathName(PARAMETER_CFG)))
	{
		return FALSE;
	}
	if (!IniWriteInt(m_strProcess, _T("ɨ��ǹ�˿�"), m_nPort, GetAbsolutePathName(PARAMETER_CFG)))
	{
		return FALSE;
	}
	if (!IniWriteString(m_strProcess, _T("ɨ��ָ��"), m_strScanCmd, GetAbsolutePathName(PARAMETER_CFG)))
	{
		return FALSE;
	}
	CRunProcess::SaveParameter();
	return TRUE;
}
