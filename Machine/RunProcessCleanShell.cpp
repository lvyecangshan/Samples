/****************************************************************************************
�ļ�����RunProcessCleanShell.cpp
���������������ϴģ��
��ʷ�޸ļ�¼��
*****************************************************************************************/
#include "StdAfx.h"
#include "RunProcessCleanShell.h"
CRunProcessCleanShell::CRunProcessCleanShell(int nID) : CRunProcess(nID)
{
	PowerUpRestart();
	CString strKey = _T("");
	for(int i = 0; i < 2; i++)
	{
		strKey.Format(_T("IHaveProduct%d"), i);
		m_InputMap.insert(pair<int*, CString>(&m_IHaveProduct[i], strKey));

		strKey.Format(_T("IFingerCylClose%d"), i);
		m_InputMap.insert(pair<int*, CString>(&m_IFingerCylClose[i], strKey));

		strKey.Format(_T("IFingerCylOpen%d"), i);
		m_InputMap.insert(pair<int*, CString>(&m_IFingerCylOpen[i], strKey));
	}

	m_OutputMap.insert(pair<int*, CString>(&m_OFingerCylOpen, _T("OFingerCylOpen")));
	m_OutputMap.insert(pair<int*, CString>(&m_OFingerCylClose, _T("OFingerCylClose")));


	m_InputMap.insert(pair<int*, CString>(&m_ILiftCylDown, _T("ILiftCylDown")));
	m_InputMap.insert(pair<int*, CString>(&m_ILiftCylUp, _T("ILiftCylUp")));
	m_OutputMap.insert(pair<int*, CString>(&m_OLiftCylDown, _T("OLiftCylDown")));
	m_OutputMap.insert(pair<int*, CString>(&m_OLiftCylUp, _T("OLiftCylUp")));

	m_OutputMap.insert(pair<int*, CString>(&m_OAirBlow, _T("OAirBlow")));
	m_OutputMap.insert(pair<int*, CString>(&m_OAirExhaust, _T("OAirExhaust")));

	m_VoidMap.insert(pair<int*, CString>(&m_nAirTimelimit, _T("lAirTimelimit, ����ʱ��(ms)")));
}

CRunProcessCleanShell::~CRunProcessCleanShell(void)
{
}

void CRunProcessCleanShell::PowerUpRestart()
{
	CRunProcess::PowerUpRestart();
	m_strMsg = m_bLangulageEng?_T("Ready"):_T("׼����");

	m_bRequirePallet = FALSE;
	m_bFingerClose = FALSE;
	m_bLiftDown = FALSE;
	m_bOffloadRequire = FALSE;

	m_nAirTime = 0;
	m_nNextPowerUpStep = PU_CLOSE_AIR;
	m_nNextAutoStep = AUTO_FINGER_CYL_OPEN;
}

void CRunProcessCleanShell::PowerUpOperation(void)
{
	if (!IsModuleEnable())
	{
		CRun::PowerUpOperation();
		m_strMsg = m_bLangulageEng?_T("Moudle UnEnable"):_T("ģ�����");
		return;
	}

	switch(m_nNextPowerUpStep)
	{
		//�رմ���������
	case PU_CLOSE_AIR:
		{
			m_strMsg = m_bLangulageEng?_T("Close Air"):_T("�رմ���");

			if(m_OAirBlow > -1)
			{
				OUTPUTS[m_OAirBlow]->Off();
			}
			if(m_OAirExhaust > -1)
			{
				OUTPUTS[m_OAirExhaust]->Off();
			}

			m_nNextPowerUpStep = PU_JUDGE_IF_WITH_PALLET;
			break;
		}

		//�ж��Ƿ����
	case PU_JUDGE_IF_WITH_PALLET:
		{
			m_strMsg = m_bLangulageEng?_T("Judge If With Pallet"):_T("�ж��Ƿ����");

			if(MACHINE.m_bReCoverMachine)
			{
				m_nNextPowerUpStep = JUDGE_SHELL_NUM;
			}
			else
			{
				m_nNextPowerUpStep = PU_FINGER_CYL_OPEN;
			}
			break;
		}

		//�н����״�
	case PU_FINGER_CYL_OPEN:
		{
			m_strMsg = m_bLangulageEng?_T("Finger Cyl Open"):_T("�н����״�");

			if (IsNoHardware() || FingerCylClose(FALSE))
			{
				m_nNextPowerUpStep = PU_CHECK_WITHOUT_SHELL;
			}
			break;
		}

		//������޿�
	case PU_CHECK_WITHOUT_SHELL:
		{
			m_strMsg = m_bLangulageEng?_T("Check Without Shell"):_T("����޿�");

			if (IsNoHardware() || m_bDryRun || ((m_IHaveProduct[0] > -1 && CheckInput(INPUTS[m_IHaveProduct[0]], OFF)) 
				&& (m_IHaveProduct[1] > -1 && CheckInput(INPUTS[m_IHaveProduct[1]], OFF))))
			{
				m_nNextPowerUpStep = PU_LIFT_CYL_DOWN;
			}
			break;
		}

		//���������½�
	case PU_LIFT_CYL_DOWN:
		{
			m_strMsg = m_bLangulageEng?_T("Lift Cyl Down"):_T("���������½�");

			if (IsNoHardware() || LiftCylUp(FALSE))
			{
				//�������
				m_bRequirePallet = FALSE;
				m_bForNextReady = FALSE;
				m_bFingerClose = FALSE;//��צ�н��ź�
				m_bLiftDown = FALSE;//�����½��ź�

				m_nNextAutoStep = AUTO_FINGER_CYL_OPEN;
				m_nNextPowerUpStep = PU_FINISH;
			}
			break;
		}

		//��������
	case JUDGE_SHELL_NUM:
		{
			m_strMsg = m_bLangulageEng?_T("Check Pallet Date"):_T("��������");
			int nShellNum = JudgeShellNum();
			if (2 == nShellNum)
			{
				m_nNextPowerUpStep = PU_CHECK_FINGER_STATE;
			}
			else if (0 == nShellNum || 1 == nShellNum)
			{
				m_nNextPowerUpStep = PU_CHECK_WITHOUT_SHELL;
			}
			break;
		}

		//����צ״̬
	case PU_CHECK_FINGER_STATE:
		{
			m_strMsg = m_bLangulageEng?_T("Check Pallet Date"):_T("����צ״̬");

			ASSERT(m_IFingerCylClose[0] > -1 && m_IFingerCylOpen[0] > -1
				&& m_IFingerCylClose[1] > -1 && m_IFingerCylOpen[1] > -1);

			if (INPUTS[m_IFingerCylClose[0]]->IsOff() || INPUTS[m_IFingerCylOpen[0]]->IsOn()
				&& INPUTS[m_IFingerCylClose[1]]->IsOff() || INPUTS[m_IFingerCylOpen[1]]->IsOn())
			{
				//���ģ���צ�򿪣����������
				ShowMessage(CLEAN_SHELL_FINGER_OPEN_WITHOUT_PALLET);
				m_nNextPowerUpStep = PU_CHECK_WITHOUT_SHELL;
			}
			else
			{
			    m_nAirTime = 0;
				m_nNextAutoStep = AUTO_WAIT_AIR_TIME_OUT;
				m_nNextPowerUpStep = PU_FINGER_CYL_CLOSE;
			}
			break;
		}

		
		//��צ�ر�
	case PU_FINGER_CYL_CLOSE:
		{
			m_strMsg = m_bLangulageEng?_T("Finger Cyl Open"):_T("��צ�ر�");

			if (IsNoHardware() || FingerCylClose(TRUE))
			{
				m_nNextPowerUpStep = PU_LIFT_CYL_UP;
			}
			break;
		}

		//������������
	case PU_LIFT_CYL_UP:
		{
			m_strMsg = m_bLangulageEng?_T("Lift Cyl Down"):_T("������������");

			if (IsNoHardware() || LiftCylUp(TRUE))
			{
				m_nNextPowerUpStep = PU_FINISH;
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

void CRunProcessCleanShell::AutoOperation()
{
	if (!IsModuleEnable())
	{
		m_strMsg = m_bLangulageEng?_T("Moudle UnEnable"):_T("ģ�����");
		Sleep(500);
		return;
	}

	if (IsNoHardware())
	{
		Sleep(500);
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
		if (m_nSaveLastStep == AUTO_CHECK_WITHOUT_SHELL)
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
		//����޿�
	case AUTO_CHECK_WITHOUT_SHELL:
		{
			m_strMsg = m_bLangulageEng?_T("Check Without Shell"):_T("����޿�");

			if (IsNoHardware() || (m_IHaveProduct[0] > -1 && CheckInput(INPUTS[m_IHaveProduct[0]],OFF)
				&& m_IHaveProduct[1] > -1 && CheckInput(INPUTS[m_IHaveProduct[1]],OFF)))
			{
				m_nNextAutoStep = AUTO_FINGER_CYL_OPEN;	
			}
			break;
		}
		//��ץ�����ɿ�
	case AUTO_FINGER_CYL_OPEN:
		{
			m_strMsg = m_bLangulageEng?_T("Finger Cyl Open"):_T("��ץ�����ɿ�");

			if (IsNoHardware() || FingerCylClose(FALSE))
			{
				m_nNextAutoStep = AUTO_LIFT_CYL_DOWN;
			}
			break;
		}

		//�������׻���
	case AUTO_LIFT_CYL_DOWN:
		{
			m_strMsg = m_bLangulageEng?_T("Lift Cyl Down"):_T("���������½�");

			if(IsNoHardware() || LiftCylUp(FALSE))
			{
				m_nNextAutoStep = AUTO_SEND_REQUIRE_EVENT;
			}
			break;
		}

		//����Ҫ�������ź�
	case AUTO_SEND_REQUIRE_EVENT:
		{
			m_strMsg = m_bLangulageEng?_T("Send Need Require Event"):_T("����Ҫ�������ź�");

			if(!m_bRequirePallet)
			{
				m_bRequirePallet = TRUE;
				m_nNextAutoStep = AUTO_WAIT_START_EVENT;
			}

			break;
		}

		//�ȴ���ʼ�ź�
	case AUTO_WAIT_START_EVENT:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Start Event"):_T("�ȴ���ʼ�ź�");

			if (!m_bRequirePallet  || (MACHINE.m_pCleanBefore != NULL && !MACHINE.m_pCleanBefore->IsModuleEnable()))
			{
				m_nNextAutoStep = AUTO_LIFT_CYL_UP;
			}
			break;
		}

		//������������
	case AUTO_LIFT_CYL_UP:
		{
			m_strMsg = m_bLangulageEng?_T("Lift Cyl Up"):_T("������������");

			if(IsNoHardware() || LiftCylUp(TRUE))
			{
				Sleep(200);
				m_nNextAutoStep = AUTO_CHECK_HAVE_SHELL;
			}
			break;
		}

		//����п�
	case AUTO_CHECK_HAVE_SHELL:
		{
			m_strMsg = m_bLangulageEng?_T("Check Have Shell"):_T("����п�");

			if (IsNoHardware() || m_bDryRun
				|| (m_IHaveProduct[0] > -1 && CheckInput(INPUTS[m_IHaveProduct[0]],ON)
				&& m_IHaveProduct[1] > -1 && CheckInput(INPUTS[m_IHaveProduct[1]],ON)))
			{
				m_nNextAutoStep = AUTO_SEND_FINGER_CYL_CLOSE_SINGLE;
			}
			break;
		}

		//���ͼн��ź�
	case AUTO_SEND_FINGER_CYL_CLOSE_SINGLE:
		{
			m_strMsg = m_bLangulageEng?_T("Send Finger Cyl Close Single"):_T("���ͼн��ź�");
			if (!m_bFingerClose)
			{
				m_bFingerClose = TRUE;
				m_nNextAutoStep = AUTO_WAIT_FRONT_MODULE_MOVE_SAFE_POS;
			}
			break;
		}

		//�ȴ���ϴǰ��ת��λ�ƶ�����ȫλ��
	case AUTO_WAIT_FRONT_MODULE_MOVE_SAFE_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Front Module Move Safe Pos"):_T("�ȴ���ϴǰ��ת��λ�ƶ�����ȫλ��");

			if (!m_bFingerClose || (MACHINE.m_pCleanBefore != NULL && !MACHINE.m_pCleanBefore->IsModuleEnable()))
			{
				m_bShellVaild = TRUE;
				m_nNextAutoStep = AUTO_FINGER_CYL_CLOSE;
			}
			break;
		}
		//��ץ���׼н�
	case AUTO_FINGER_CYL_CLOSE:
		{
			m_strMsg = m_bLangulageEng?_T("Finger Cyl Close"):_T("��ץ���׼н�");
			if (IsNoHardware() || FingerCylClose(TRUE))
			{
				if (IsNoHardware() || FingerCylClose(FALSE))
				{
					Sleep(200);
					if (IsNoHardware() || FingerCylClose(TRUE))
					{
						if (m_OAirBlow > -1 )
						{
							OUTPUTS[m_OAirBlow]->On();
						}
						if(m_OAirExhaust > -1)
						{
							OUTPUTS[m_OAirExhaust]->On();
						}
						m_nAirTime = GetTickCount();
						m_nNextAutoStep = AUTO_WAIT_AIR_TIME_OUT;
					}
				}

			}
			break;
		}
		//�ȴ��������
	case AUTO_WAIT_AIR_TIME_OUT:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Air Time Out"):_T("�ȴ��������");
			if (0 == m_nAirTime)  //�����������¼�ʱ
			{
				m_nAirTime = GetTickCount();  
			}

			if(GetTickCount() - m_nAirTime > (DWORD)m_nAirTimelimit)
			{
				if(m_OAirBlow > -1)
				{
					OUTPUTS[m_OAirBlow]->Off();
				}
				if(m_OAirExhaust > -1)
				{
					OUTPUTS[m_OAirExhaust]->Off();
				}
				m_nNextAutoStep = AUTO_SEND_OFFLOAD_REQUIRE;	
			}
			break;
		}

		//��������׼�����ź�
	case AUTO_SEND_OFFLOAD_REQUIRE:
		{
			m_strMsg = m_bLangulageEng?_T("Send Ready Event"):_T("��������׼�����ź�");
			if (!m_bOffloadRequire)
			{
				m_bOffloadRequire = TRUE;
				m_nNextAutoStep = AUTO_WAIT_REQUIRE_RESPONSE;
			}
			break;
		}

		//�ȴ���ʼ�ź�
	case AUTO_WAIT_REQUIRE_RESPONSE:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Start Event"):_T("�ȴ���ת��ʼ�ź�");
			if (!m_bOffloadRequire)
			{
				m_nNextAutoStep = AUTO_FINGER_CYL_OPEN_OFFLOAD;
			}
			break;   
		}

		//��ץ�����ɿ�
	case AUTO_FINGER_CYL_OPEN_OFFLOAD:
		{
			m_strMsg = m_bLangulageEng?_T("Finger Cyl Open"):_T("��תҪ�ϣ���ץ�����ɿ�");

			if (IsNoHardware() || FingerCylClose(FALSE))
			{
				m_nNextAutoStep = AUTO_LIFT_CYL_DOWN_OFFLOAD;
			}
			break;
		}

		//�������׻���
	case AUTO_LIFT_CYL_DOWN_OFFLOAD:
		{
			m_strMsg = m_bLangulageEng?_T("Lift Cyl Down"):_T("��תҪ�ϣ����������½�");

			if(IsNoHardware() || LiftCylUp(FALSE))
			{
				m_nNextAutoStep = AUTO_SEND_LIFT_CYL_DOWN_EVENT;
			}
			break;
		}

		//���Ͷ����½��ź�
	case AUTO_SEND_LIFT_CYL_DOWN_EVENT:
		{
			m_strMsg = m_bLangulageEng?_T("Send Lift Cyl Down Event"):_T("���Ͷ����½��ź�");
			if (!m_bLiftDown)
			{
				m_bLiftDown = TRUE;
				m_nNextAutoStep = AUTO_WAIT_NEXT_MODULE_MOVE_SAFE_POS;
				SaveStepInfo(WRITE_TYPE_AUTO_STEP|WRITE_TYPE_VARIABLES);
			}
			break;
		}

		//�ȴ���ת��ȫ�ź�
	case AUTO_WAIT_NEXT_MODULE_MOVE_SAFE_POS:
		{
			m_strMsg = m_bLangulageEng?_T(" Wait Next Module Move Safe Pos"):_T("�ȴ���ת��ȫ�ź�");

			if (!m_bLiftDown  || (MACHINE.m_pCleanAfter != NULL && !MACHINE.m_pCleanAfter->IsModuleEnable()))
			{
				m_bShellVaild = FALSE;
				m_nNextAutoStep = AUTO_WORK_END;
			}
			break;
		}

		//�������
	case AUTO_WORK_END:
		{
			m_strMsg = m_bLangulageEng?_T("Work end"):_T("�������");
			m_nAirTime = 0;
			m_nNextAutoStep = AUTO_CHECK_WITHOUT_SHELL;
			break;
		}
	default:
		{
			ASSERT(0);
			break;
		}
	}
}

BOOL CRunProcessCleanShell::InitializeConfig(CString strProcessModule)
{
	if (!CRunProcess::InitializeConfig(strProcessModule))
	{
		return FALSE;
	}
	return TRUE;
}

void CRunProcessCleanShell::GetHardwareConfig(CArray<int, int> *pInputs, CArray<int, int> *pOutputs, CArray<int, int> *pMotors)
{
	CRunProcess::GetHardwareConfig(pInputs, pOutputs, pMotors);
}

BOOL CRunProcessCleanShell:: CheckOutputCanActive(COutput *pOutput)
{
	return TRUE;
}

BOOL CRunProcessCleanShell::SaveParameter()
{
	if(!IniWriteInt(m_strProcess, _T("AirTimelimit"), 2000, GetAbsolutePathName(PARAMETER_CFG)))
	{
		return FALSE;
	}
	return CRunProcess::SaveParameter();
}

BOOL  CRunProcessCleanShell::ReadRunParameter()
{
	CRunProcess::ReadRunParameter();

	m_nAirTimelimit = IniReadInt(m_strProcess, _T("AirTimelimit"), 2000, GetAbsolutePathName(PARAMETER_CFG));
	return TRUE;
}

/****************************************************************************************
������������צ���׶���
���������bClose:TRUE:��צ���׼н� FALSE:��צ�����ɿ�
�������أ�TRUE:ִ�гɹ���FALSE:ִ��ʧ��
*****************************************************************************************/
BOOL CRunProcessCleanShell:: FingerCylClose(BOOL bClose)
{
	ASSERT(m_IFingerCylOpen[0] > -1 
		&& m_IFingerCylOpen[1] > -1 
		&& m_IFingerCylClose[0] > -1 
		&& m_IFingerCylClose[1] > -1
		&& m_OFingerCylOpen > -1 
		&& m_OFingerCylClose > -1);

	BOOL Res = FALSE;

	bClose?OUTPUTS[m_OFingerCylClose]->On():OUTPUTS[m_OFingerCylClose]->Off();
	bClose?OUTPUTS[m_OFingerCylOpen]->Off():OUTPUTS[m_OFingerCylOpen]->On();

	if (WAIT_OK == WaitInput(INPUTS[m_IFingerCylClose[0]], bClose?ON:OFF, MSG_SHOW_DEFAULT) 
		&& WAIT_OK == WaitInput(INPUTS[m_IFingerCylOpen[0]], bClose?OFF:ON, MSG_SHOW_DEFAULT)
		&& WAIT_OK == WaitInput(INPUTS[m_IFingerCylClose[1]], bClose?ON:OFF, MSG_SHOW_DEFAULT) 
		&& WAIT_OK == WaitInput(INPUTS[m_IFingerCylOpen[1]], bClose?OFF:ON, MSG_SHOW_DEFAULT))
	{
		Res = TRUE;
	}
	return Res;
}

/****************************************************************************************
�����������������׶���
���������bPush:TRUE:���������Ƴ� FALSE:�������׻���
�������أ�TRUE:ִ�гɹ���FALSE:ִ��ʧ��
*****************************************************************************************/
BOOL CRunProcessCleanShell:: LiftCylUp(BOOL bUp)
{
	ASSERT(m_ILiftCylUp > -1 && m_ILiftCylDown > -1 && m_OLiftCylUp > -1 && m_OLiftCylDown > -1);
	BOOL Res = FALSE;

	bUp?OUTPUTS[m_OLiftCylUp]->On():OUTPUTS[m_OLiftCylUp]->Off();
	bUp?OUTPUTS[m_OLiftCylDown]->Off():OUTPUTS[m_OLiftCylDown]->On();

	if (WAIT_OK == WaitInput(INPUTS[m_ILiftCylUp], bUp?ON:OFF, MSG_SHOW_DEFAULT) 
		&& WAIT_OK == WaitInput(INPUTS[m_ILiftCylDown], bUp?OFF:ON, MSG_SHOW_DEFAULT))
	{
		Res = TRUE;
	}
	return Res;
}

/****************************************************************************************
��������������Ƿ��п�
���������
�������أ�nRes = 0:û���� 1����⵽һ���� 2;��⵽������
*****************************************************************************************/
int CRunProcessCleanShell::JudgeShellNum()
{
	ASSERT(m_IHaveProduct[0] > -1 && m_IHaveProduct[1] > -1);
	int nRes = 0;

	if (INPUTS[m_IHaveProduct[0]]->IsOn())
	{
		nRes++;
	} 
	if (INPUTS[m_IHaveProduct[1]]->IsOn())
	{
		nRes++;
	} 
	return nRes;
}
