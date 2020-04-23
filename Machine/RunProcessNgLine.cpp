/****************************************************************************************
�ļ�����RunProcessNgLine.h
����������ѹ��������������
��ʷ�޸ļ�¼��
*****************************************************************************************/
#include "StdAfx.h"
#include "RunProcessNgLine.h"


DWORD WINAPI NgLineThread(LPVOID lpParam)
{
	CRunProcessNgLine *pMotorRun = (CRunProcessNgLine*)lpParam;
	if (pMotorRun == NULL)
	{
		return 0;
	}

	pMotorRun->m_bThreadEnd = FALSE;
	pMotorRun->m_bThreadStart = TRUE;

	while (pMotorRun->m_bThreadStart)
	{
		pMotorRun ->WorkWhile();
		Sleep(1);
	}
	pMotorRun->m_bThreadEnd = TRUE;
	return 0;
}

CRunProcessNgLine::CRunProcessNgLine(int nID) : CRunProcess(nID)
{
	PowerUpRestart();

	m_InputMap.insert(pair<int*, CString>(&m_ICheckBattery[0], _T("ICheckBattery0")));
	m_InputMap.insert(pair<int*, CString>(&m_ICheckBattery[1], _T("ICheckBattery1")));

	m_InputMap.insert(pair<int*, CString>(&m_ICheckInPos, _T("ICheckInPos")));
	m_InputMap.insert(pair<int*, CString>(&m_ICheckLineFull, _T("ICheckLineFull")));
	
	m_OutputMap.insert(pair<int*, CString>(&m_OLineMotor, _T("OLineMotor")));

	m_InputMap.insert(pair<int*, CString>(&m_IManualBtn, _T("IManualBtn")));
	m_OutputMap.insert(pair<int*, CString>(&m_OManualBtnLight, _T("OManualBtnLight")));
}

CRunProcessNgLine::~CRunProcessNgLine(void)
{
	if (m_OLineMotor > -1 )
	{
		OUTPUTS[m_OLineMotor]->Off();
	}
	Destory();
}

void CRunProcessNgLine::PowerUpRestart()
{
	CRunProcess::PowerUpRestart();
	m_strMsg = m_bLangulageEng?_T("Ready"):_T("׼����");

	m_bNextRequire = FALSE;
	m_bRequireNgBattery = FALSE;

	m_nNextPowerUpStep = PU_WAIT_SAFE_SIGNAL;
	m_nNextAutoStep = AUTO_SEND_REQUIRE_SIGNAL;
}

void CRunProcessNgLine::PowerUpOperation(void)
{
	if (!IsModuleEnable())
	{
		CRun::PowerUpOperation();
		m_strMsg = m_bLangulageEng?_T("Moudle UnEnable"):_T("ģ�����");
		return;
	}

	switch(m_nNextPowerUpStep)
	{
	case PU_WAIT_SAFE_SIGNAL:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Safe Signal"):_T("�ȴ���ȫ�ź�");
			if (m_nNgLineID == 0 && MACHINE.m_pOnloadCore != NULL)
			{
				if (MACHINE.m_pOnloadCore->m_bHomeMoveSafePos)
				{
					m_nNextPowerUpStep = PU_MANUAL_RELEASE;
				}
			}

 			if (m_nNgLineID == 1 && MACHINE.m_pRobotOffload != NULL)
 			{
 				if (MACHINE.m_pRobotOffload->m_bRobotSafe)
 				{
 					m_nNextPowerUpStep = PU_MANUAL_RELEASE;	
 				}
 			}
			break;
		}

		//����Ƥ��
	case PU_MANUAL_RELEASE:
		{
			m_strMsg = m_bLangulageEng?_T("Release All Status"):_T("����ȫ��״̬");
			if (m_OLineMotor > -1 && OUTPUTS[m_OLineMotor]->IsOn())
			{
				OUTPUTS[m_OLineMotor]->Off();
			}

			if(m_OManualBtnLight > -1)
			{
				OUTPUTS[m_OManualBtnLight]->Off();
			}
			m_nNextPowerUpStep = PU_CHECK_WITHOUT_PRODUCT;			
			break;
		}

		//����Ƿ��в�Ʒ
	case PU_CHECK_WITHOUT_PRODUCT:
		{
			m_strMsg = m_bLangulageEng?_T("Check Without Product"):_T("����޲�Ʒ");		

			if (m_OLineMotor > -1 && OUTPUTS[m_OLineMotor]->IsOff())
			{
				OUTPUTS[m_OLineMotor]->On();
			}

			DWORD dwCurTime = GetTickCount();
			BOOL bCheckBattery = FALSE;

			while(GetTickCount() - dwCurTime < 3000)
			{
				if(CheckHaveProduct())
				{
					bCheckBattery = TRUE;
					break;
				}
			}

			if (m_OLineMotor > -1 && OUTPUTS[m_OLineMotor]->IsOn())
			{
				OUTPUTS[m_OLineMotor]->Off();
			}			

			if(bCheckBattery)
			{
				if(m_OManualBtnLight > -1)
					OUTPUTS[m_OManualBtnLight]->On();
				if(MACHINE.m_nTowerBuzzer > -1)
					OUTPUTS[MACHINE.m_nTowerBuzzer]->On();

				BLMessageBox(_T("Ƥ���߼���в�Ʒ���봦��"));
				m_nNextPowerUpStep = PU_WAIT_MANUAL_CLEAR_BATTERY;
			}
			else
			{
				m_nNextPowerUpStep = PU_FINISH;
			}
			break;
		}

	case PU_WAIT_MANUAL_CLEAR_BATTERY:
		{
			m_strMsg = m_bLangulageEng?_T("Motor Stop"):_T("Ƥ��ֹͣ");		

			if (IsNoHardware() || m_bDryRun || (m_IManualBtn > -1 && INPUTS[m_IManualBtn]->IsOn()))
			{
				if(m_OManualBtnLight > -1)
					OUTPUTS[m_OManualBtnLight]->Off();

				if(MACHINE.m_nTowerBuzzer > -1)
					OUTPUTS[MACHINE.m_nTowerBuzzer]->Off();

				m_nNextPowerUpStep = PU_MANUAL_RELEASE;
			}
			break;
		}

		//��ʼ�����
	case PU_FINISH:
		{
			CRun::PowerUpOperation();
			m_strMsg = m_bLangulageEng?_T("Power up finished"):_T("��ʼ�����");
			break;
		}
	default:
		{
			ASSERT(0);
			break;
		}
	}
}

void CRunProcessNgLine::AutoOperation()
{
	if (!IsModuleEnable())
	{
		m_strMsg = m_bLangulageEng?_T("Moudle UnEnable"):_T("ģ�����");
		Sleep(500);
		return;
	}

	if (IsNoHardware())
	{
		Sleep(100);
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
		if (m_nSaveLastStep == AUTO_SEND_REQUIRE_SIGNAL)
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
		//����Ҫ���ź�
	case AUTO_SEND_REQUIRE_SIGNAL:
		{
			m_strMsg = m_bLangulageEng?_T("Send Require Signal"):_T("����Ҫ���ź�");		

			if (!CheckHaveProduct())
			{
				if (m_OLineMotor > -1 && OUTPUTS[m_OLineMotor]->IsOn())
				{
					OUTPUTS[m_OLineMotor]->Off();
				}

				if (m_ICheckLineFull > -1 && INPUTS[m_ICheckLineFull]->IsOn())
				{
					m_nNextAutoStep = AUTO_CHECK_FULL;
				}
				else
				{
					if (INPUTS[m_ICheckBattery[0]]->IsOff() && INPUTS[m_ICheckBattery[1]]->IsOff())
					{
						if(!m_bRequireNgBattery)
						{
							m_bRequireNgBattery = TRUE;
							m_nNextAutoStep = AUTO_WAIT_OFFLOAD_FINISH_SIGNAL;	
						}
					}
				}
			}
			else
			{
				ShowMessage(_T("NG�����в�Ʒ�����ܷ���Ҫ���źţ��봦��"),3);
			}
			break;
		}

		//�ȴ���������ź�
	case AUTO_WAIT_OFFLOAD_FINISH_SIGNAL:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Offload Signal"):_T("�ȴ�NG�������");

			if (!m_bRequireNgBattery)
			{
				m_nNextAutoStep = AUTO_CHECK_HAVE_CORE;
			}
			break;
		}

		//�������
	case AUTO_CHECK_HAVE_CORE:
		{		
			m_strMsg = m_bLangulageEng?_T("Move XY Solder Pos"):_T("������ɣ��������");

			ASSERT(m_ICheckBattery[0] > -1 && m_ICheckBattery[1] > -1);

			if (IsNoHardware() || m_bDryRun || (INPUTS[m_ICheckBattery[0]]->IsOn() || INPUTS[m_ICheckBattery[1]]->IsOn()))
			{
				m_nNextAutoStep = AUTO_MOTOR_MOVE_TO_RECV_NG;
			}
			break;
		}

		//Ƥ������
	case AUTO_MOTOR_MOVE_TO_RECV_NG:
		{
			m_strMsg = m_bLangulageEng?_T("Motor Move"):_T("Ƥ��ת��������NGƷ");

			if (m_OLineMotor > -1 && OUTPUTS[m_OLineMotor]->IsOff())
			{
				OUTPUTS[m_OLineMotor]->On();
			}
			m_nDelayTime = GetTickCount();
			m_nNextAutoStep = AUTO_CHECK_FULL;
			break;
		}

		//����Ƿ�����
	case AUTO_CHECK_FULL:
		{
			m_strMsg = m_bLangulageEng?_T("Check Full"):_T("����Ƿ�����");

			if(IsNoHardware() || m_bDryRun)
			{
				m_nNextAutoStep = AUTO_WORK_END;
				break;
			}

			if (m_ICheckLineFull > -1 && INPUTS[m_ICheckLineFull]->IsOn())
			{
				if(m_OManualBtnLight > -1)
				{
					OUTPUTS[m_OManualBtnLight]->On();
				}
				//Sleep(1000);
				if (m_OLineMotor > -1 && OUTPUTS[m_OLineMotor]->IsOn())
				{
					OUTPUTS[m_OLineMotor]->Off();
				}
				MACHINE.SetBuzzerStatue(TRUE);
				CString strMsg;
				strMsg = (m_strProcess == _T("PressNgLine"))?_T("ѹ��������������������"):_T("ɨ�벻����������������");
				BLMessageBox(strMsg);
				m_nNextAutoStep = AUTO_WAIT_MANUAL_OPERATOR;
			}
			else if (GetTickCount() - m_nDelayTime > 10000)
			{
				CString strMsg;
				strMsg = (m_strProcess == _T("PressNgLine"))?_T("ѹ�������������ϸ�Ӧ10Sû�н��յ���Ʒ�������Ƿ��ϻ�Ƥ���쳣"):_T("ɨ�벻���������ϸ�Ӧ10Sû�н��յ���Ʒ�������Ƿ��ϻ�Ƥ���쳣");
				ShowMessage(strMsg,3);
			}
			break;
		}

	case AUTO_WAIT_MANUAL_OPERATOR:
		{
			m_strMsg = m_bLangulageEng?_T("manual operator"):_T("�ȴ��˹�����");

			if(IsNoHardware() || m_bDryRun || (m_IManualBtn > -1 && INPUTS[m_IManualBtn]->IsOn()))
			{
				MACHINE.SetBuzzerStatue(FALSE);
				if(m_OManualBtnLight > -1)
				{
					OUTPUTS[m_OManualBtnLight]->Off();
				}
				m_nNextAutoStep = AUTO_WORK_END;
			}
			break;
		}
		//�������
	case AUTO_WORK_END:
		{
			m_strMsg = m_bLangulageEng?_T("Work end"):_T("�������");

			m_nNextAutoStep = AUTO_SEND_REQUIRE_SIGNAL;
			break;
		}

	default:
		{
			ASSERT(0);
			break;
		}
	}
}

BOOL CRunProcessNgLine::InitializeConfig(CString strProcessModule)
{
	if (!CRunProcess::InitializeConfig(strProcessModule))
		return FALSE;

	m_nNgLineID = -1;
	if(m_strModuleName == _T("BarCodeNgLine"))
		m_nNgLineID = 0;
	else if(m_strModuleName == _T("PressNgLine"))
		m_nNgLineID = 1;
	else
		ASSERT(0);

	AfxBeginThread((AFX_THREADPROC)NgLineThread,(LPVOID)this);
	return TRUE;
}


void CRunProcessNgLine::GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors)
{
	CRunProcess::GetHardwareConfig(pInputs,pOutputs,pMotors);
}

BOOL CRunProcessNgLine::CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{
	return TRUE;
}

BOOL CRunProcessNgLine::CheckOutputCanActive(COutput *pOutput)
{
	return TRUE;
}


/****************************************************************************************
��������������Ƿ�����
�������أ�TRUE:���ϣ�FALSE:û��
*****************************************************************************************/
BOOL CRunProcessNgLine::CheckHaveProduct()
{

	/*ASSERT(m_ICheckBattery[0] > -1 && m_ICheckBattery[1] > -1 && m_ICheckLineFull > -1 && m_ICheckInPos > -1);
	
	if (INPUTS[m_ICheckBattery[0]]->IsOn() || INPUTS[m_ICheckBattery[1]]->IsOn()
		|| INPUTS[m_ICheckInPos]->IsOn() || INPUTS[m_ICheckLineFull]->IsOn())
	{
		return TRUE;
	}*/
	return FALSE;
}

/****************************************************************************************
//����������������ƶ�
����������������ť���Ƶ��
�������أ�
*****************************************************************************************/
void CRunProcessNgLine::WorkWhile()   
{
	return;
}

/****************************************************************************************
���������������Դ
�������أ�
*****************************************************************************************/
void CRunProcessNgLine::Destory()
{
	DWORD dwTime = GetTickCount();

	m_bThreadStart = FALSE;
	
	while(!m_bThreadEnd)
	{
		if (GetTickCount() - dwTime > 10000)
		{
			break;
		}
		Sleep(1);
	}
}
