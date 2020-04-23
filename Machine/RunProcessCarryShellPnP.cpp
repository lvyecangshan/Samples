/****************************************************************************************
�ļ�����RunProcessCarryShellPnP.h
���������������ǰ��˻�е��
��ʷ�޸ļ�¼��
*****************************************************************************************/
#include "StdAfx.h"
#include "RunProcessCarryShellPnP.h"

CRunProcessCarryShellPnP::CRunProcessCarryShellPnP(int nID) :
CRunProcess(nID)
{
	PowerUpRestart();

	CString strKey;
	for(int i = 0; i < 2; i++)
	{
		strKey.Format(_T("OVacumCyl%d"), i);
		m_OutputMap.insert(pair<int*, CString>(&m_OVacumCyl[i], strKey));

		strKey.Format(_T("ICheckVacum%d"), i);
		m_InputMap.insert(pair<int*, CString>(&m_ICheckVacum[i], strKey));
	}
	//----��������--
	m_InputMap.insert(pair<int*, CString>(&m_IPushCylPush, _T("IPushCylPush")));//���������Ƴ����
	m_InputMap.insert(pair<int*, CString>(&m_IPushCylBack, _T("IPushCylBack")));//�������׻��˼��
	m_OutputMap.insert(pair<int*, CString>(&m_OPushCylPush, _T("OPushCylPush")));//���������Ƴ���ŷ�
	m_OutputMap.insert(pair<int*, CString>(&m_OPushCylBack, _T("OPushCylBack")));//�������׻��˵�ŷ�

	m_OutputMap.insert(pair<int*, CString>(&m_OBreakVacum, _T("OBreakVacum")));
	m_MotorMap.insert(pair<int*, CString>(&m_nMotorZ, _T("MotorZ")));
	m_MotorMap.insert(pair<int*, CString>(&m_nMotorX, _T("MotorX")));
}

CRunProcessCarryShellPnP::~CRunProcessCarryShellPnP(void)
{
}

void CRunProcessCarryShellPnP::PowerUpRestart()
{
	CRunProcess::PowerUpRestart();
	m_strMsg = m_bLangulageEng?_T("Ready"):_T("׼����");
	m_bHomeMoveSafePos = FALSE;
	m_nNextPowerUpStep = PU_CHECK_HAVE_SHELL;
	m_nNextAutoStep = AUTO_MOVE_X_TO_ONLOAD_POS;
}

void CRunProcessCarryShellPnP::PowerUpOperation(void)
{
	if (!IsModuleEnable())
	{
		CRun::PowerUpOperation();
		m_strMsg = m_bLangulageEng?_T("Moudle UnEnable"):_T("ģ�����");
		return;
	}

	switch(m_nNextPowerUpStep)
	{
		//����Ƿ��п�
	case PU_CHECK_HAVE_SHELL:
		{
			m_strMsg = m_bLangulageEng?_T("Check Have Shell"):_T("������޿�");

			if (::IsNoHardware() || m_bDryRun || CheckVacum(-1, FALSE))
			{
				m_nNextPowerUpStep = PU_CLOSE_VACUM;
			}
			break;
		}
		//�ر����
	case PU_CLOSE_VACUM:
		{
			m_strMsg = m_bLangulageEng?_T("Close Vacum"):_T("�ر�����������");

			if(::IsNoHardware() || (VacumOpen(-1, FALSE) && BreakVacum( FALSE)))
			{
				m_nNextPowerUpStep = PU_MOVE_Z_TO_HOME;
			}
			break;
		}
		//Z������λ
	case PU_MOVE_Z_TO_HOME:
		{
			m_strMsg = m_bLangulageEng?_T("Move Z To Home"):_T("Z������λ");

			if(IsNoHardware() || (m_nMotorZ > -1 && HomeMotor(MOTORS[m_nMotorZ])))
			{
				m_nNextPowerUpStep = PU_MOVE_Z_TO_SAFE_POS;
			}
			break;
		}
		//�ƶ�Z�ᵽ��ȫλ
	case PU_MOVE_Z_TO_SAFE_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Move Z To Safe Pos"):_T("�ƶ�Z�ᵽ��ȫλ");

			if (m_nMotorZ > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorZ, Z_MOTOR_SAFE_POS))
			{
				m_bHomeMoveSafePos = TRUE;
				m_nNextPowerUpStep = PU_MOVE_X_TO_HOME;
			}
			break;
		}
		//X������λ
	case PU_MOVE_X_TO_HOME:
		{
			m_strMsg = m_bLangulageEng?_T("Move X To Home"):_T("X������λ");

			if (m_nMotorX > -1 && HomeMotor(MOTORS[m_nMotorX]))
			{
				m_nNextPowerUpStep = PU_MOVE_X_TO_TO_ONLOAD_POS;
			}
			break;
		}
		//X�����ƶ���ȡ��λ
	case PU_MOVE_X_TO_TO_ONLOAD_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Move X To Onload Pos"):_T("X�����ƶ���ȡ��λ");

			if (m_nMotorX > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorX, X_MOTOR_ONLOAD_LOC))
			{
				m_nNextPowerUpStep = PU_PUSH_CYL_RETURN;
			}
			break;
		}
	case PU_PUSH_CYL_RETURN:
		{
			m_strMsg = m_bLangulageEng?_T("Push Cyl Return"):_T("�������׻���");

			if (IsNoHardware() || PushCylPush(FALSE))
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

void CRunProcessCarryShellPnP::AutoOperation()
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
		if (m_nSaveLastStep == AUTO_MOVE_X_TO_ONLOAD_POS)
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
		//�ƶ�X�ᵽȡ��λ
	case AUTO_MOVE_X_TO_ONLOAD_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Move X To Onload Pos"):_T("�ƶ�X�ᵽȡ��λ");

			if(m_OPushCylPush > -1 && m_OPushCylBack > -1)
			{
				OUTPUTS[m_OPushCylPush]->Off();
				OUTPUTS[m_OPushCylBack]->On();
			}
			if (m_nMotorX > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorX, X_MOTOR_ONLOAD_LOC))
			{
				m_nNextAutoStep = AUTO_CYL_PUSH_CYL_RETURN;
			}
			break;
		}

	case AUTO_CYL_PUSH_CYL_RETURN:
		{
			m_strMsg = m_bLangulageEng?_T("Push Cyl Return"):_T("�Ƴ����׻���");

			if(IsNoHardware() || PushCylPush(FALSE))
			{
				m_nNextAutoStep = AUTO_WAIT_ONLOAD_SINGLE;
			}
			break;
		}

		//�ȴ�ǰһ��λ׼�����ź�
	case AUTO_WAIT_ONLOAD_SINGLE:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Onload Single"):_T("�ȴ�ǰһ��λ׼�����ź�");

			if (MACHINE.m_pCleanAfter != NULL && (MACHINE.m_pCleanAfter->m_bOffloadRequire || !MACHINE.m_pCleanAfter->IsModuleEnable()))
			{
				m_nNextAutoStep = AUTO_MOVE_Z_TO_PICK_POS;
			}
			else
			{
				m_nNextAutoStep = AUTO_MOVE_Z_TO_WAIT_PICK_POS;
			}
			break;
		}

		//Z���ƶ���ȡ�ϵȴ�λ
	case AUTO_MOVE_Z_TO_WAIT_PICK_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Move Z To Wait Pick Pos"):_T("Z���ƶ���ȡ�ϵȴ�λ");

			if(m_nMotorZ > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorZ, Z_MOTOR_WAIT_PICK_POS))
			{
				m_nNextAutoStep = AUTO_WAIT_ONLOAD_SINGLE_EX;
			}
			break;
		}

		//Z���ƶ����ȴ�λ��ȴ�ǰһ��λ׼����
	case AUTO_WAIT_ONLOAD_SINGLE_EX:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Onload Single"):_T("Z���ƶ����ȴ�λ��ȴ�ǰһ��λ׼����");

			if (MACHINE.m_pCleanAfter != NULL && (MACHINE.m_pCleanAfter->m_bOffloadRequire || !MACHINE.m_pCleanAfter->IsModuleEnable()))
			{
				m_nNextAutoStep = AUTO_MOVE_Z_TO_PICK_POS;
			}
			break;
		}

		//�ƶ�Z�ᵽȡ��λ
	case AUTO_MOVE_Z_TO_PICK_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Move Z To Pick Pos"):_T("�ƶ�Z�ᵽȡ��λ");

			if(m_nMotorZ > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorZ, Z_MOTOR_PICK_POS))
			{
				m_nNextAutoStep = AUTO_OPEN_VACUM;
			}
			break;
		}

		//�����
	case AUTO_OPEN_VACUM:
		{
			m_strMsg = m_bLangulageEng?_T("Open Vacum"):_T("�����");

			if(::IsNoHardware() || m_bDryRun || VacumOpen(-1, TRUE))
			{
				if (MACHINE.m_pCleanAfter != NULL && MACHINE.m_pCleanAfter->OpenVacuum(FALSE))
				{
					m_nNextAutoStep = AUTO_MOVE_Z_TO_SAFE_POS;
				}
			}
			break;
		}

		//�ƶ�Z�ᵽ��ȫλ
	case AUTO_MOVE_Z_TO_SAFE_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Move Z To Safe Pos"):_T("�ƶ�Z�ᵽ��ȫλ");

			if (m_nMotorZ > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorZ, Z_MOTOR_SAFE_POS))
			{
				m_nNextAutoStep = AUTO_SEND_MOVE_Z_TO_SAFE_SINGLE_TO_FRONT_MODULE;
			}
			break;
		}

		//��ǰһģ�鷢���ƶ�����ȫλ�ź�
	case AUTO_SEND_MOVE_Z_TO_SAFE_SINGLE_TO_FRONT_MODULE:
		{
			m_strMsg = m_bLangulageEng?_T("Send Front Module Safe Single"):_T("��ǰһģ�鷢���ƶ�����ȫλ�ź�");

			if(MACHINE.m_pCleanAfter != NULL && (MACHINE.m_pCleanAfter->m_bOffloadRequire || !MACHINE.m_pCleanAfter->IsModuleEnable()))
			{
				m_bShellVaild = TRUE;
				MACHINE.m_pCleanAfter->m_bOffloadRequire = FALSE;			
				m_nNextAutoStep = AUTO_MOVE_X_TO_OFFLAOD_POS;
			}
			break;
		}

		//�ƶ�X�ᵽ����λ
	case AUTO_MOVE_X_TO_OFFLAOD_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Move X To Wait Pos"):_T("�ƶ�X�ᵽ����λ");

			if (m_OPushCylBack > -1 && m_OPushCylPush > -1)
			{
				OUTPUTS[m_OPushCylBack]->Off();
				OUTPUTS[m_OPushCylPush]->On();
			}
			if (m_nMotorX > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorX, X_MOTOR_PNP_PUT_POS))
			{
				m_nNextAutoStep = AUTO_PUSH_CYL_PUSH;
			}
			break;
		}
	case AUTO_PUSH_CYL_PUSH:
		{
			m_strMsg = m_bLangulageEng?_T("Push Cyl Push"):_T("���������Ƴ�");

			if(IsNoHardware() || PushCylPush(TRUE))
			{
				m_nNextAutoStep = AUTO_JUDGE_WHICH_MODULE_NEED_SHELL;			
			}
			break;
		}
		//�ȴ���ʼ�����ź�
	case AUTO_JUDGE_WHICH_MODULE_NEED_SHELL:
		{
			m_strMsg = m_bLangulageEng?_T("Judge Which Module Need Shell"):_T("�ȴ���ʼ�����ź�");

			if (MACHINE.m_pBatteryInShell[0] != NULL && MACHINE.m_pBatteryInShell[1] != NULL 
				&& (MACHINE.m_pBatteryInShell[0]->m_bRequirePallet || !MACHINE.m_pBatteryInShell[0]->IsModuleEnable())
				&& (MACHINE.m_pBatteryInShell[1]->m_bRequirePallet || !MACHINE.m_pBatteryInShell[1]->IsModuleEnable()))
			{
				m_nNextAutoStep = AUTO_OFFLOAD_CHECK_VACUM;
			}
			else
			{
				m_nNextAutoStep = AUTO_Z_AXIS_MOVE_TO_PLACE_WAIT_LOC;
			}
			break;
		}
	case AUTO_Z_AXIS_MOVE_TO_PLACE_WAIT_LOC:
		{
			m_strMsg = m_bLangulageEng?_T("z axis move to wait place loc"):_T("Z�ᵽ�ȴ�����λ");
			if (m_nMotorZ > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorZ, Z_MOTOR_WAIT_PLACE_POS))
			{
				m_nNextAutoStep = AUTO_CHECK_REQUIRE_AGAIN;
			}
			break;
		}
	case AUTO_CHECK_REQUIRE_AGAIN:
		{
			m_strMsg = m_bLangulageEng?_T("Check Require Again"):_T("�ٴμ�������ź�");
			if (MACHINE.m_pBatteryInShell[0] != NULL && MACHINE.m_pBatteryInShell[1] != NULL 
				&& (MACHINE.m_pBatteryInShell[0]->m_bRequirePallet || !MACHINE.m_pBatteryInShell[0]->IsModuleEnable())
				&& (MACHINE.m_pBatteryInShell[1]->m_bRequirePallet || !MACHINE.m_pBatteryInShell[1]->IsModuleEnable()))
			{
				m_nNextAutoStep = AUTO_OFFLOAD_CHECK_VACUM;
			}
			break;
		}
		// ����ǰ������
	case AUTO_OFFLOAD_CHECK_VACUM:
		{
			m_strMsg = m_bLangulageEng?_T("Offload Check Vacum"):_T("����ǰ������");

			if (IsNoHardware() || m_bDryRun || CheckVacum(-1,TRUE))
			{
				m_nNextAutoStep = AUTO_MOVE_Z_TO_PUT_POS;
			}
			break;
		}
		//�ƶ�Z�ᵽ����λ
	case AUTO_MOVE_Z_TO_PUT_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Move Z To Place Pos"):_T("�ƶ�Z�ᵽ����λ");

			if (m_nMotorZ > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorZ, Z_MOTOR_PLACE_POS))
			{
				m_nNextAutoStep = AUTO_SEND_PLACE_FINISH_SINGLE;
			}
			break;
		}

		//���ͷ�������ź�
	case AUTO_SEND_PLACE_FINISH_SINGLE:
		{
			m_strMsg = m_bLangulageEng?_T("Send Place Finish Single"):_T("���ͷ�������ź�");

			for (int i = 0; i <2; i++)
			{
				if(MACHINE.m_pBatteryInShell[i] != NULL && MACHINE.m_pBatteryInShell[i]->m_bRequirePallet)
				{ 
					MACHINE.m_pBatteryInShell[i]->m_bRequirePallet = FALSE;
				}
			}
			m_nNextAutoStep = AUTO_WAIT_BATTERY_CYL_CLOSE;
			break;
		}

	
	case AUTO_WAIT_BATTERY_CYL_CLOSE:
		{
			m_strMsg = m_bLangulageEng?_T("wait next module cyl close"):_T("�ȴ���ǹ�λ�������׼ӽ�");
			if (MACHINE.m_pBatteryInShell[0] != NULL&& MACHINE.m_pBatteryInShell[1] != NULL 
				&& (MACHINE.m_pBatteryInShell[0]->m_bCylClosed || !MACHINE.m_pBatteryInShell[0]->IsModuleEnable())
				&& (MACHINE.m_pBatteryInShell[1]->m_bCylClosed|| !MACHINE.m_pBatteryInShell[1]->IsModuleEnable()))
			{
				m_nNextAutoStep = AUTO_CLOSE_VACUM;
			}
			break;
		}
		//�ر����
	case AUTO_CLOSE_VACUM:
		{
			m_strMsg = m_bLangulageEng?_T("Close Vacum"):_T("�ر����");

			if(::IsNoHardware() || (VacumOpen(-1, FALSE) && BreakVacum(TRUE)))
			{
				m_nNextAutoStep = AUTO_MOVE_Z_TO_SAFE_POS_AFTER_PLACE;
			}
			break;
		}
		//�������ƶ�Z�ᵽ��ȫλ
	case AUTO_MOVE_Z_TO_SAFE_POS_AFTER_PLACE:
		{
			m_strMsg = m_bLangulageEng?_T("Move Z To Safe Pos After Place"):_T("�������ƶ�Z�ᵽ��ȫλ");

			if (m_nMotorZ > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorZ, Z_MOTOR_SAFE_POS))
			{
				for (int i = 0; i <2; i++)
				{
					if(MACHINE.m_pBatteryInShell[i] != NULL && MACHINE.m_pBatteryInShell[i]->m_bCylClosed)
					{
						MACHINE.m_pBatteryInShell[i]->m_bCylClosed = FALSE;
					}
				}
				m_bShellVaild = FALSE;
				m_nNextAutoStep = AUTO_WORK_END;
			}
			break;
		}

		//�������
	case AUTO_WORK_END:
		{
			m_strMsg = m_bLangulageEng?_T("Work end"):_T("�������");
			m_nNextAutoStep = AUTO_MOVE_X_TO_ONLOAD_POS;
			break;
		}
	default:
		{
			ASSERT(0);
			break;
		}
	}
	if(MACHINE.m_bCreateCTLog && m_nNextAutoStep != m_nAutoNext)
	{
		m_dwModuleStepCT = GetTickCount() - m_dwModuleStrt;
		m_dwModuleCT += m_dwModuleStepCT;

		if(m_nNextAutoStep == AUTO_MOVE_X_TO_ONLOAD_POS)
		{
			m_strModuleCTtemp.Format(_T("��%s������CT��%.2f\r\n\r\n-----------------------------��%s������ѭ��CT��%.2f-----------------------------\r\n"), m_strMsg.GetInfo(), (double)(m_dwModuleStepCT/1000.0), m_strName, (double)(m_dwModuleCT/1000.0));
			m_dwModuleCT = 0;
		}
		else
		{
			m_strModuleCTtemp.Format(_T("��%s������CT��%.2f\r\n"), m_strMsg.GetInfo(), (double)(m_dwModuleStepCT/1000.0));
		}
		WriteLog(m_strModuleCTtemp);
	}
}

BOOL CRunProcessCarryShellPnP::InitializeConfig(CString strProcessModule)
{
	if (!CRunProcess::InitializeConfig(strProcessModule))
	{
		return FALSE;
	}
	return TRUE;
}

void CRunProcessCarryShellPnP::GetHardwareConfig(CArray<int, int> *pInputs, CArray<int, int> *pOutputs, CArray<int, int> *pMotors)
{
	CRunProcess::GetHardwareConfig(pInputs, pOutputs, pMotors);
}


//����ƶ�����
BOOL CRunProcessCarryShellPnP::CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{
#ifdef _TEST_
	return TRUE;
#endif
	if(m_nMotorX > -1 && pMotor == MOTORS[m_nMotorX])
	{
		if(m_nMotorZ > -1)
		{
			if(!MOTORS[m_nMotorZ]->IsOriRefKnown())
			{
				BLMessageBox(_T("(M05)�������Z������λ, X���������ƶ�"), MB_ICONWARNING);
				return FALSE;
			}
			else
			{
				if( MOTORS[m_nMotorZ]->GetCurPos() > 5)
				{
					BLMessageBox(_T("(M05)�������Z�������ڰ�ȫλ, X���������ƶ�"), MB_ICONWARNING);
					return FALSE;
				}
			}
		}	
	}

	if ((m_nMotorZ > -1 && pMotor == MOTORS[m_nMotorZ]))
	{
		//ȡ�ϵȴ�λ ȡ��λ
		if (iLoc == Z_MOTOR_WAIT_PICK_POS || iLoc == Z_MOTOR_PICK_POS)
		{
			if (m_nMotorX > -1)
			{
				double pos = GetMotorPos(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorX, X_MOTOR_ONLOAD_LOC);
				double npos = MOTORS[m_nMotorX]->GetCurPos();
				if (fabs(pos - npos) > 0.5)
				{
					BLMessageBox(_T("(M04)�������X��������ȡ��λ��Z�᲻���ƶ���ȡ��λ��ȡ�ϵȴ�λ"), MB_ICONWARNING);
					return FALSE;
				}
			}		

			ASSERT(m_IPushCylPush > -1 && m_IPushCylBack > -1);
			if (INPUTS[m_IPushCylPush]->IsOn() && INPUTS[m_IPushCylBack]->IsOff())
			{
				BLMessageBox(_T("��Ǳ�������Ƴ���Z�᲻���ƶ���ȡ��λ��ȡ�ϵȴ�λ"), MB_ICONWARNING);
				return FALSE;
			}

			int RotateCylPush = MACHINE.m_pCleanAfter->m_IRotateCylPush;
			int RotateCylReturn = MACHINE.m_pCleanAfter->m_IRotateCylReturn;
            ASSERT(RotateCylPush > -1 && RotateCylReturn > -1);
            if (INPUTS[RotateCylReturn]->IsOn() && INPUTS[RotateCylPush]->IsOff())
            {
				BLMessageBox(_T("������λģ����ת���״��ڻ���״̬��Z�᲻���ƶ���ȡ��λ��ȡ�ϵȴ�λ"), MB_ICONWARNING);
				return FALSE;
            }

		}

		//����λ   ���ϵȴ�λ
		if (iLoc == Z_MOTOR_WAIT_PLACE_POS || iLoc == Z_MOTOR_PLACE_POS)
		{
			if (m_nMotorX > -1)
			{
				if (MOTORS[m_nMotorX]->GetCurLoc() != X_MOTOR_PNP_PUT_POS )					
				{
					BLMessageBox(_T("(M04)�������X�������ڷ���λ��Z�᲻���ƶ�������λ����ϵȴ�λ"), MB_ICONWARNING);
					return FALSE;
				}
			}

			ASSERT(m_IPushCylPush > -1 && m_IPushCylBack > -1);
			if (INPUTS[m_IPushCylPush]->IsOff() && INPUTS[m_IPushCylBack]->IsOn())
			{
				BLMessageBox(_T("��Ǳ�����׻��ˣ�Z�᲻���ƶ�������λ����ϵȴ�λ"), MB_ICONWARNING);
				return FALSE;
			}
		}
	}
	return TRUE;
}



BOOL CRunProcessCarryShellPnP::CheckOutputCanActive(COutput *pOutput)
{
#ifdef _TEST_
	return TRUE;
#endif
	ASSERT(m_OPushCylPush > -1 && m_OPushCylBack > -1 && m_nMotorZ > -1);
	if (pOutput == OUTPUTS[m_OPushCylPush] || pOutput == OUTPUTS[m_OPushCylBack])
	{
		double dPosZ0 = MOTORS[m_nMotorZ]->GetCurPos();
		double dPosZ1 = GetMotorPos(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorZ, Z_MOTOR_SAFE_POS);
		if (dPosZ0 > 5)
		{
			BLMessageBox(_T("(M05)�������Z�������ڰ�ȫλ, ��Ǳ�����ײ��ܶ�"));
			return FALSE;
		}
	}

	return TRUE;
}

/****************************************************************************************
������������մ�
���������nIndex:		-1����������յ�ŷ���Ϊ0-1ʱ�ֱ�������1�����2
bClose:		TRUE:��մ�		FALSE:��չر�
�������أ�TRUE:ִ�гɹ���FALSE:ִ��ʧ��
*****************************************************************************************/
BOOL CRunProcessCarryShellPnP::VacumOpen(int nIndex, BOOL bOpen)
{
	ASSERT(-1 <= nIndex && nIndex <= 1);
	ASSERT(m_ICheckVacum[0] > -1 && m_ICheckVacum[1] > -1 && m_OVacumCyl[0] > -1 && m_OVacumCyl[1] > -1);

	BOOL bRes = FALSE;

	if (nIndex < 0)
	{
		if(bOpen)
		{
			for(int i = 0; i < 2; i++)
			{
				OUTPUTS[m_OVacumCyl[i]]->On();
			}

			if(WAIT_OK == WaitInput(INPUTS[m_ICheckVacum[0]], ON, MSG_SHOW_DEFAULT) 
				&& WAIT_OK == WaitInput(INPUTS[m_ICheckVacum[1]], ON, MSG_SHOW_DEFAULT))
			{
				Sleep(300);
				bRes = TRUE;
			}
		}
		else
		{
			for(int i = 0; i < 2; i++)
			{
				OUTPUTS[m_OVacumCyl[i]]->Off();
			}

			if(WAIT_OK == WaitInput(INPUTS[m_ICheckVacum[0]], OFF, MSG_SHOW_DEFAULT) 
				&& WAIT_OK == WaitInput(INPUTS[m_ICheckVacum[1]], OFF, MSG_SHOW_DEFAULT))
			{
				Sleep(100);
				bRes = TRUE;
			}
		}
	}
	else	
	{
		if(bOpen)
		{			
			OUTPUTS[m_OVacumCyl[nIndex]]->On();			

			if(WAIT_OK == WaitInput(INPUTS[m_ICheckVacum[nIndex]], ON, MSG_SHOW_DEFAULT))
			{
				Sleep(300);
				bRes = TRUE;
			}
		}
		else
		{
			OUTPUTS[m_OVacumCyl[nIndex]]->Off();

			if(WAIT_OK == WaitInput(INPUTS[m_ICheckVacum[nIndex]], OFF, MSG_SHOW_DEFAULT))
			{
				Sleep(100);
				bRes = TRUE;
			}
		}
	}	
	return bRes;
}

/****************************************************************************************
��������������մ�
���������bBreak:		TRUE:����մ�		FALSE:����չر�
�������أ�TRUE:ִ�гɹ���FALSE:ִ��ʧ��
*****************************************************************************************/
BOOL CRunProcessCarryShellPnP::BreakVacum( BOOL bBreak)
{
	if(m_OBreakVacum < 0 )
	{
		return TRUE;
	}
	if(bBreak)
	{			
		OUTPUTS[m_OBreakVacum]->On();
	}
	else
	{		
		OUTPUTS[m_OBreakVacum]->Off();
	}

	return TRUE;
}

/****************************************************************************************
����������������
���������nIndex:		-1����������յ�ŷ���Ϊ0-1ʱ�ֱ�������1�����2
bOpen:		TRUE:��մ�		FALSE:��չر�
�������أ�TRUE:ִ�гɹ���FALSE:ִ��ʧ��
*****************************************************************************************/
BOOL CRunProcessCarryShellPnP::CheckVacum(int nIndex,BOOL bOpen) 
{
	ASSERT(-1 <= nIndex && nIndex <= 1);
	ASSERT(m_ICheckVacum[0] > -1 && m_ICheckVacum[1] > -1);


	if (nIndex < 0)
	{
		if(bOpen)
		{
			if(CheckInput(INPUTS[m_ICheckVacum[0]], ON) 
				&& CheckInput(INPUTS[m_ICheckVacum[1]], ON))
			{
				return TRUE;
			}
		}
		else
		{
			if(CheckInput(INPUTS[m_ICheckVacum[0]], OFF) 
				&& CheckInput(INPUTS[m_ICheckVacum[1]], OFF))
			{
				return TRUE;
			}
		}
	}
	else	
	{
		if(bOpen)
		{			
			if(CheckInput(INPUTS[m_ICheckVacum[nIndex]], ON))
			{
				return TRUE;
			}
		}
		else
		{
			if(CheckInput(INPUTS[m_ICheckVacum[nIndex]], OFF))
			{
				return TRUE;
			}
		}
	}	
	return FALSE;
}


/****************************************************************************************
�����������Ƴ����׶���
���������bPush:TRUE:�Ƴ������Ƴ� FALSE:�Ƴ����׻���
�������أ�TRUE:ִ�гɹ���FALSE:ִ��ʧ��
*****************************************************************************************/
BOOL CRunProcessCarryShellPnP:: PushCylPush(BOOL bPush)
{
	ASSERT(m_IPushCylPush > -1 && m_IPushCylBack > -1 && m_OPushCylPush > -1 && m_OPushCylBack > -1);

	if (bPush)
	{
		OUTPUTS[m_OPushCylBack]->Off();
		OUTPUTS[m_OPushCylPush]->On();

		if(WAIT_OK == WaitInput(INPUTS[m_IPushCylPush], ON, MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_IPushCylBack], OFF, MSG_SHOW_DEFAULT))
		{
			return TRUE;
		}
	}
	else
	{
		OUTPUTS[m_OPushCylPush]->Off();
		OUTPUTS[m_OPushCylBack]->On();

		if(WAIT_OK == WaitInput(INPUTS[m_IPushCylPush], OFF, MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_IPushCylBack], ON, MSG_SHOW_DEFAULT))
		{
			return TRUE;
		}
	}
	return FALSE;
}


/****************************************************************************************
������������ȡZ�ᰲȫλλ��
���������
�������أ� ����Z�ᰲȫλλ��
*****************************************************************************************/
double CRunProcessCarryShellPnP::GetMotorZPOS()
{
	ASSERT(m_nMotorZ > -1);

	double dPos1 = GetMotorPos(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorZ, Z_MOTOR_SAFE_POS);

	return dPos1;
}
