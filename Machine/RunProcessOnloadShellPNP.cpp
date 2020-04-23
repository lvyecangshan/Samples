/****************************************************************************************
�ļ�����RunProcessOnloadShellPNP.cpp
�����������Ͽǻ�е��
��ʷ�޸ļ�¼��
*****************************************************************************************/
#include "StdAfx.h"
#include "RunProcessOnloadShellPNP.h"

CRunProcessOnloadShellPNP::CRunProcessOnloadShellPNP(int nID) :
CRunProcess(nID)
{
	PowerUpRestart();

	CString strKey;
	for(int i = 0; i < 2; i++)
	{
		
		strKey.Format(_T("IShellDirection%d"), i);
		m_InputMap.insert(pair<int*, CString>(&m_IShellDirection[i], strKey));   //�ǿڳ�����

		strKey.Format(_T("ICheckVau%d"), i);
		m_InputMap.insert(pair<int*, CString>(&m_ICheckVacu[i], strKey));

		strKey.Format(_T("OOpenVau%d"), i);
		m_OutputMap.insert(pair<int*, CString>(&m_OOpenVacu[i], strKey));

		strKey.Format(_T("ITurnOut%d"), i);
		m_InputMap.insert(pair<int*, CString>(&m_ITurnOut[i], strKey));

		strKey.Format(_T("ITurnReturn%d"), i);
		m_InputMap.insert(pair<int*, CString>(&m_ITurnReturn[i], strKey));

		strKey.Format(_T("OTurnOut%d"), i);
		m_OutputMap.insert(pair<int*, CString>(&m_OTurnOut[i], strKey));

		strKey.Format(_T("OTurnReturn%d"), i);
		m_OutputMap.insert(pair<int*, CString>(&m_OTurnReturn[i], strKey));
	}

	m_InputMap.insert(pair<int*, CString>(&m_IPushCylPush, _T("IPushCylPush")));
	m_InputMap.insert(pair<int*, CString>(&m_IPushCylBack, _T("IPushCylBack")));

	m_OutputMap.insert(pair<int*, CString>(&m_OPushCylPush, _T("OPushCylPush")));
	m_OutputMap.insert(pair<int*, CString>(&m_OPushCylReturn, _T("OPushCylReturn")));

	m_MotorMap.insert(pair<int*, CString>(&m_nMotorY, _T("MotorY")));
	m_MotorMap.insert(pair<int*, CString>(&m_nMotorZ, _T("MotorZ")));

	m_MotorMap.insert(pair<int*, CString>(&m_nMotorR[0], _T("MotorR1")));
	m_MotorMap.insert(pair<int*, CString>(&m_nMotorR[1], _T("MotorR2")));
}

CRunProcessOnloadShellPNP::~CRunProcessOnloadShellPNP(void)
{
}

void CRunProcessOnloadShellPNP::PowerUpRestart()
{
	CRunProcess::PowerUpRestart();
	m_strMsg = m_bLangulageEng?_T("Ready"):_T("׼����");

	m_nMotorMoveIndex = 0;
	m_bPickCoverFinish = FALSE;
	m_bHomeMoveSafePos = FALSE;
	m_bNextRequire = FALSE;
	m_bEnableDocking = FALSE;
	m_nNextPowerUpStep = PU_CHECK_FINGER_STATUS;
	m_nNextAutoStep = AUTO_SEND_FRONT_MODULE_MOVE_MOTOR;
}

void CRunProcessOnloadShellPNP::PowerUpOperation(void)
{
	if (!IsModuleEnable())
	{
		CRun::PowerUpOperation();
		m_strMsg = m_bLangulageEng?_T("Moudle UnEnable"):_T("ģ�����");
		return;
	}

	switch(m_nNextPowerUpStep)
	{	
		//���ץ��������
	case PU_CHECK_FINGER_STATUS:
		{
			m_strMsg = m_bLangulageEng?_T("Check Figher Status"):_T("���ץ�ֶ�����");

			if (::IsNoHardware() || CheckVacuStatus(FALSE))
			{
				m_nNextPowerUpStep = PU_MOVE_MOTOR_Z_HOME;
			}
			break;
		}

		//Z������λ
	case PU_MOVE_MOTOR_Z_HOME:
		{
			m_strMsg = m_bLangulageEng?_T("Move Motor Z Home"):_T("Z������λ");

			if(m_nMotorZ > -1 && HomeMotor(MOTORS[m_nMotorZ]))
			{
				m_bHomeMoveSafePos = TRUE;
				m_nNextPowerUpStep = PU_ROTATE_CYL_HOME;
			}
			break;
		}

		//��ת���׻�ԭλ
	case PU_ROTATE_CYL_HOME:
		{
			m_strMsg = m_bLangulageEng?_T("Rotate Cyl Home"):_T("��ת���׻�ԭλ");

			if (::IsNoHardware() ||/* TurnOut(-1, FALSE)*/
				(APP_MotorMove(m_nMotorR[0], R_MOTOR_PICK_POS) && APP_MotorMove( m_nMotorR[1], R_MOTOR_PICK_POS)))
			{
				m_nNextPowerUpStep = PU_CLAMP_CYL_OPEN;
			}
			break;
		}

		//�ر����
	case PU_CLAMP_CYL_OPEN:
		{
			m_strMsg = m_bLangulageEng?_T("Clamp Cyl Open"):_T("�ر����");

			if (::IsNoHardware() || CloseVacu())
			{
				m_nNextPowerUpStep = PU_MOVE_MOTOR_Z_TO_SAFE_POS;
			}
			break;
		}

		//Z�����ƶ�����ȫλ
	case PU_MOVE_MOTOR_Z_TO_SAFE_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Move Motor Z To Safe Pos"):_T("Z�����ƶ�����ȫλ");

			if (m_nMotorZ > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorZ, Z_MOTOR_SAFE_POS))
			{
				m_nNextPowerUpStep = PU_MOTOR_Y_HOME;
			}
			break;
		}

		//Y������λ
	case PU_MOTOR_Y_HOME:
		{
			m_strMsg = m_bLangulageEng?_T("Motor Y Home"):_T("Y������λ");

			if (m_nMotorY > -1 && HomeMotor(MOTORS[m_nMotorY]))	
			{
				m_nNextPowerUpStep = PU_JUDGE_IF_WITH_PALLET;	
			}
			break;
		}

		//�ж��Ƿ����
	case PU_JUDGE_IF_WITH_PALLET:
		{
			m_strMsg = m_bLangulageEng?_T("Check Have Product"):_T("�ж��Ƿ����");

			if(MACHINE.m_bReCoverMachine)
			{
				ReadStepInfo();
				if(m_nMotorMoveIndex < 0 || m_nMotorMoveIndex >= 6)
					m_nMotorMoveIndex = 0;
			}
			m_nNextAutoStep = AUTO_SEND_FRONT_MODULE_MOVE_MOTOR;
			m_nNextPowerUpStep = PU_MOTOR_Y_MOVE_PICK_POS;
			break;
		}

		//Y�����ƶ�������λ
	case PU_MOTOR_Y_MOVE_PICK_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Motor Y Move Pick Pos"):_T("Y�����ƶ�������λ");

			if (m_nMotorY > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorY, Y_MOTOR_PICK_LOC1)) 
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

void CRunProcessOnloadShellPNP::AutoOperation()
{
	if (!IsModuleEnable())
	{
		m_strMsg = m_bLangulageEng?_T("Moudle not enable"):_T("ģ�����");
		Sleep(500);
		return;
	}

	if (IsNoHardware())
	{
		;//Sleep(500);
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
		if (m_nSaveLastStep == AUTO_SEND_FRONT_MODULE_MOVE_MOTOR)
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
		//������һ����λ�ƶ�����ź�
	case AUTO_SEND_FRONT_MODULE_MOVE_MOTOR:
		{
			m_strMsg = m_bLangulageEng?_T("Send Front Module Move Motor"):_T("������һ����λ�ƶ�����ź�");
			
			if (MACHINE.m_pFeederTray != NULL && (!MACHINE.m_pFeederTray->m_bOnloadPnpRequireShell || !MACHINE.m_pFeederTray->IsModuleEnable()))
			{
				//�����̰����ź��ƶ���ȡ��λ
				MACHINE.m_pFeederTray->m_bOnloadPnpRequireShell = TRUE;
				m_nNextAutoStep = AUTO_MOVE_Y_MOTOR_TO_ONLOAD_POS;
			}
			break;
		}
		//Y�����ƶ�������λ
	case AUTO_MOVE_Y_MOTOR_TO_ONLOAD_POS:
		{
			CString strKey;
			strKey.Format(_T("Y�����ƶ���ȡ��λ%d"),m_nMotorMoveIndex+1);
			m_strMsg = m_bLangulageEng?_T("Move Y Motor To Onload Pos"):strKey;


			int pos[2]={R_MOTOR_PICK_POS,R_MOTOR_PICK_POS};
			MotorsMove( GetCurPorductID(MACHINE.m_nDefaultProduct),m_nMotorR,pos,2);

			ASSERT(0 <= m_nMotorMoveIndex && m_nMotorMoveIndex < 6);
			int nLoc = Y_MOTOR_PICK_LOC1 + m_nMotorMoveIndex;
			if (m_nMotorY > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorY, nLoc))
			{
				m_nNextAutoStep = AUTO_CHECK_ROATE_BACK;
			}
			break;
		}

		//�����ת�������޻�ԭλ
	case AUTO_CHECK_ROATE_BACK:
		{
			m_strMsg = m_bLangulageEng?_T("Check Roate Back"):_T("�����ת�������޻�ԭλ");
			//if(IsNoHardware() || TurnOut(-1, FALSE))
			{
				m_nNextAutoStep = AUTO_CHECK_PUSH_PUSH;
			}
			break;
		}
	case AUTO_CHECK_PUSH_PUSH:
		{
			m_strMsg = m_bLangulageEng?_T("Check Push Cyl Push"):_T("�����������Ƴ�");
			if(IsNoHardware() || PushCylPush(FALSE))
			{
				m_nNextAutoStep = AUTO_WAIT_FRONT_MODULE_MOVE_MOTOR_FINISH;
			}
			break;
		}

		//�ȴ�ǰһ����λ����ƶ���λ
	case AUTO_WAIT_FRONT_MODULE_MOVE_MOTOR_FINISH:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Frong Module Move Motor Finish"):_T("�ȴ�ǰһ����λ����ƶ���λ");

			if( MACHINE.m_pFeederTray != NULL && (MACHINE.m_pFeederTray->m_bMoveMotorFinish || !MACHINE.m_pFeederTray->IsModuleEnable()))
			{
				MACHINE.m_pFeederTray->m_bMoveMotorFinish = FALSE;
				m_nNextAutoStep = AUTO_MOVE_MOTOR_Z_MOTOR_TO_PICK_POS;
			}
			else
			{
				m_nNextAutoStep = AUTO_MOTOR_Z_MOVE_TO_WAIT_POS;
			}
			break;
		}
	case AUTO_MOTOR_Z_MOVE_TO_WAIT_POS:
		{
			CString strKey;
			strKey.Format(_T("��Y��ȡ��λ%d����Z���Ƶ�ȡ�ϵȴ�λ"),m_nMotorMoveIndex+1);
			m_strMsg = m_bLangulageEng?_T("Move Motor Z Motor To Pick Pos"):strKey;

			if (m_nMotorZ > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorZ, Z_MOTOR_PICK_WAIT_POS))
			{
				m_nNextAutoStep = AUTO_WAIT_FRONT_MODULE_MOVE_MOTOR_FINISH_AGAIN;
			}
			break;
		}
	case AUTO_WAIT_FRONT_MODULE_MOVE_MOTOR_FINISH_AGAIN:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Frong Module Move Motor Finish"):_T("�ȴ�ǰһ����λ����ƶ���λ");

			if( MACHINE.m_pFeederTray != NULL && (MACHINE.m_pFeederTray->m_bMoveMotorFinish || !MACHINE.m_pFeederTray->IsModuleEnable()))
			{
				MACHINE.m_pFeederTray->m_bMoveMotorFinish = FALSE;
				m_nNextAutoStep = AUTO_MOVE_MOTOR_Z_MOTOR_TO_PICK_POS;
			}
			break;
		}

		//Z���½���ȡ��λ
	case AUTO_MOVE_MOTOR_Z_MOTOR_TO_PICK_POS:
		{
			CString strKey;
			strKey.Format(_T("��Y��ȡ��λ%d����Z���Ƶ�ȡ��λ"),m_nMotorMoveIndex+1);
			m_strMsg = m_bLangulageEng?_T("Move Motor Z Motor To Pick Pos"):strKey;

			if (m_nMotorZ > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorZ, Z_MOTOR_PICK_POS))
			{
				ASSERT(m_OOpenVacu[0] > -1 && m_OOpenVacu[1] > -1 && m_ICheckVacu[0] >-1 && m_ICheckVacu[1] >-1);

				OUTPUTS[m_OOpenVacu[0]]->On();
				OUTPUTS[m_OOpenVacu[1]]->On();
				Sleep(300);
				if(::IsNoHardware() || m_bDryRun || CheckVacuStatus(TRUE))
				{
					m_nNextAutoStep = AUTO_MOVE_MOTOR_Z_TO_SAFE_POS_ONLOAD;
				}
			}
			break;
		}

		//���ϴ�����ƶ�����ȫλ
	case AUTO_MOVE_MOTOR_Z_TO_SAFE_POS_ONLOAD:
		{
			m_strMsg = m_bLangulageEng?_T("Move Motor Z to Safe Pos Onload"):_T("���ϴ�����ƶ�����ȫλ");

			if (m_nMotorZ > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorZ, Z_MOTOR_SAFE_POS))
			{
				m_nMotorMoveIndex++;
				m_nMotorMoveIndex = m_nMotorMoveIndex % 6;
				m_nNextAutoStep = AUTO_CHECK_ALL_VACU_ONLOAD;
				SaveStepInfo(WRITE_TYPE_VARIABLES);
			}
			break;
		}

		//������λ���������Ʒ���
	case AUTO_CHECK_ALL_VACU_ONLOAD:
		{
			m_strMsg = m_bLangulageEng?_T("Check All Vacu Onload"):_T("������λ���������Ʒ���");

			if((::IsNoHardware() || m_bDryRun || CheckVacuStatus(TRUE)) && !m_bPickCoverFinish)
			{
				if(MACHINE.m_pFeederTray != NULL && MACHINE.m_pFeederTray->IsModuleEnable())
				{
					m_bPickCoverFinish = TRUE;
				}
				if (m_OPushCylPush > -1)
				{
					OUTPUTS[m_OPushCylPush]->On();
				}
				if(m_OPushCylReturn > -1)
				{
					OUTPUTS[m_OPushCylReturn]->Off();
				}

				for(int i = 0; i < 2; i++)
				{
					if(m_OTurnOut[i] > -1)
					{
						OUTPUTS[m_OTurnOut[i]]->On();
					}
				}
				m_nNextAutoStep = AUTO_MOVE_Y_MOTOR_TO_OFFLOAD_POS;
			}
			break;
		}

		//�ƶ�Y����������λ
	case AUTO_MOVE_Y_MOTOR_TO_OFFLOAD_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Move Y Motor To Offload Pos"):_T("�ƶ�Y����������λ");

			if (m_nMotorY > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorY, Y_MOTOR_PLACE_LOC))
			{
				m_nNextAutoStep = AUTO_CHECK_ALL_VACU_OFFLOAD;
			}
			break;
		}

		//������λ���������Ʒ���
	case AUTO_CHECK_ALL_VACU_OFFLOAD:
		{
			m_strMsg = m_bLangulageEng?_T("Check All Vacu Onload"):_T("������λ���������Ʒ���");

			if (::IsNoHardware() || m_bDryRun || CheckVacuStatus(TRUE))
			{
				m_nNextAutoStep = AUTO_PUSH_CYL_BACK_TO_OFFLOAD;
			}
			break;
		}
	case AUTO_PUSH_CYL_BACK_TO_OFFLOAD:
		{
			m_strMsg = m_bLangulageEng?_T("Push Cyl Back"):_T("�������׻���");
			if(IsNoHardware() || PushCylPush(TRUE))
			{
				m_nNextAutoStep = AUTO_ROATE_CYL_RAOTE;
			}
			break;
		}
		//��ת������ת
	case AUTO_ROATE_CYL_RAOTE:
		{
			m_strMsg = m_bLangulageEng?_T("Roate Cyl Roate"):_T("��ת������ת");
			int pos[2]={R_MOTOR_ROTATE_NORMAL,R_MOTOR_ROTATE_NORMAL};
			if(::IsNoHardware() || MotorsMove( GetCurPorductID(MACHINE.m_nDefaultProduct),m_nMotorR,pos,2))
			{
				m_nNextAutoStep = AUTO_WAIT_START_EVENT;
			}
			break;
		}
		//�ȴ���ʼ�ź�
	case AUTO_WAIT_START_EVENT:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Start Event"):_T("�ȴ���ʼ�ź�");

			if (MACHINE.m_pCleanBefore != NULL && (MACHINE.m_pCleanBefore->m_bRequirePallet || !MACHINE.m_pCleanBefore->IsModuleEnable()))
			{
				//MACHINE.m_pCleanBefore->m_bRequirePallet = FALSE;
				m_nNextAutoStep = AUTO_MOVE_Z_MTOR_TO_PUT_POS;
			}
			else
			{
				m_nNextAutoStep = AUTO_MOVE_Z_TO_WAIT_PUT_POS;
			}
			break;
		}

		//�ƶ���������ϵȴ�λ
	case AUTO_MOVE_Z_TO_WAIT_PUT_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Move Z To Wait Put Pos"):_T("�ƶ���������ϵȴ�λ");

			if (m_nMotorZ > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorZ, Z_MOTOR_WAIT_PLACE_POS))
			{
				m_nNextAutoStep = AUTO_WAIT_START_EVENTEX;
			}
			break;
		}

		//����ƶ������ϵȴ�λ��ȴ���ʼ�ź�
	case AUTO_WAIT_START_EVENTEX:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Start EventEx"):_T("����ƶ������ϵȴ�λ��ȴ���ʼ�ź�");

			if (MACHINE.m_pCleanBefore != NULL && (MACHINE.m_pCleanBefore->m_bRequirePallet || !MACHINE.m_pCleanBefore->IsModuleEnable()))
			{
				m_nNextAutoStep = AUTO_MOVE_Z_MTOR_TO_PUT_POS;
			}
			break;
		}

		//�ƶ�Z�ᵽ����λ
	case AUTO_MOVE_Z_MTOR_TO_PUT_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Move Z Motor To Put Pos"):_T("�ƶ�Z�ᵽ����λ");

			if (m_nMotorZ > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorZ, Z_MOTOR_PLACE_POS))
			{
				m_nNextAutoStep = AUTO_CHECK_DIRECTION;//AUTO_CLAMP_CYL_OPEN;
			}
			break;
		}
		case  AUTO_CHECK_DIRECTION:
			{
				m_strMsg = m_bLangulageEng?_T("check ShellPnP direction"):_T("������ǳ��ڷ���");
				if (IsNoHardware() || m_bDryRun
					|| (m_IShellDirection[0] > -1 && INPUTS[m_IShellDirection[0]]->IsOn() 
					&& m_IShellDirection[1] > -1 && INPUTS[m_IShellDirection[1]]->IsOn()))
				{
					m_nNextAutoStep = AUTO_CLAMP_CYL_OPEN;
				}
				else
				{
					m_bNeedChangeDirect[0] = FALSE;
					m_bNeedChangeDirect[1] = FALSE;
					if(m_IShellDirection[0] > -1 && INPUTS[m_IShellDirection[0]]->IsOff())
					{
						m_bNeedChangeDirect[0] = TRUE;
					}
					if(m_IShellDirection[1] > -1 && INPUTS[m_IShellDirection[1]]->IsOff())
					{
						m_bNeedChangeDirect[1] = TRUE;
					}
					m_nNextAutoStep = AUTO_CHANGE_DIRECTION;
				}
				break;
			}
		case  AUTO_CHANGE_DIRECTION:
			{
				m_strMsg = m_bLangulageEng?_T("change ShellPnP direction"):_T("�ı����ǳ��ڷ���");
				if(APP_MotorMove(m_nMotorZ,Z_MOTOR_CHANGE_DIRECTION))
				{
					if(m_bNeedChangeDirect[0])
					{
						APP_MotorMove(m_nMotorR[0],R_MOTOR_ROTATE_REVERSE);
					}
					if(m_bNeedChangeDirect[1])
					{
						APP_MotorMove(m_nMotorR[1],R_MOTOR_ROTATE_REVERSE);
					}
					m_nNextAutoStep = AUTO_MOVE_Z_MTOR_TO_PUT_POS;
				}
				break;
			}
		//���ϴ��ر����
	case AUTO_CLAMP_CYL_OPEN: 
		{
			m_strMsg = m_bLangulageEng?_T("Close Vacuum"):_T("���ϴ��򿪷�תǰ��ղ��ҹر����");

			ASSERT(m_OOpenVacu[0] > -1 && m_OOpenVacu[1] > -1 && m_ICheckVacu[0] >-1 && m_ICheckVacu[1] >-1);
			ASSERT(MACHINE.m_pCleanBefore != NULL);

			if (::IsNoHardware() || m_bDryRun || MACHINE.m_pCleanBefore->OpenVacuum(TRUE))
			{
				OUTPUTS[m_OOpenVacu[0]]->Off();
				OUTPUTS[m_OOpenVacu[1]]->Off();

				if(::IsNoHardware() || m_bDryRun || CheckVacuStatus(FALSE))
				{
					m_nNextAutoStep = AUTO_MOVE_Z_MOTOR_SAFE_POS_OFFLOAD;	
				}	
			}
			break;
		}

		//�����ϴ��ƶ�Z�ᵽ��ȫλ
	case AUTO_MOVE_Z_MOTOR_SAFE_POS_OFFLOAD: 
		{
			m_strMsg = m_bLangulageEng?_T("Move Z Motor Safe Pos Offload"):_T("�����ϴ��ƶ�Z�ᵽ��ȫλ");

			if (m_nMotorZ > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorZ, Z_MOTOR_SAFE_POS))
			{
				m_nNextAutoStep = AUTO_SEND_OFFLOAD_FINISH;
			}
			break;
		}

	
	case AUTO_SEND_OFFLOAD_FINISH:
		{
			m_strMsg = m_bLangulageEng?_T("Send Offload Finish Single"):_T("������������ź�");
			if (MACHINE.m_pCleanBefore != NULL && (MACHINE.m_pCleanBefore->m_bRequirePallet || !MACHINE.m_pCleanBefore->IsModuleEnable()))
			{
				MACHINE.m_pCleanBefore->m_bRequirePallet = FALSE;
				m_nNextAutoStep = AUTO_ROATE_CYL_RETURN;
			}
		}
		//��ת���׻�ԭλ
	case AUTO_ROATE_CYL_RETURN:
		{
			m_strMsg = m_bLangulageEng?_T("Roate Cyl Return"):_T("��ת���׻�ԭλ");

			int pos[2]={R_MOTOR_PICK_POS,R_MOTOR_PICK_POS};
			MotorsMove( GetCurPorductID(MACHINE.m_nDefaultProduct),m_nMotorR,pos,2);
			
			m_nNextAutoStep = AUTO_WORK_END;
			break;
		}

		//�������
	case AUTO_WORK_END:
		{
			m_strMsg = m_bLangulageEng?_T("Work end"):_T("�������");

			if(!MACHINE.m_bStopPickShell)
			{
				m_nNextAutoStep = AUTO_SEND_FRONT_MODULE_MOVE_MOTOR;
			}
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

		if(m_nNextAutoStep == AUTO_SEND_FRONT_MODULE_MOVE_MOTOR)
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

BOOL CRunProcessOnloadShellPNP::InitializeConfig(CString strProcessModule)
{
	if (!CRunProcess::InitializeConfig(strProcessModule))
	{
		return FALSE;
	}
	return TRUE;
}

void CRunProcessOnloadShellPNP::GetHardwareConfig(CArray<int, int> *pInputs, CArray<int, int> *pOutputs, CArray<int, int> *pMotors)
{
	CRunProcess::GetHardwareConfig(pInputs, pOutputs, pMotors);
}

/****************************************************************************************
������������ת���׶���
���������nIndex:-1����������ת���ף�Ϊ0-1ʱ�ֱ������ת����1��2
bOut:TRUE:��ת�����Ƴ� FALSE:��ת���׻���
�������أ�TRUE:ִ�гɹ���FALSE:ִ��ʧ��
*****************************************************************************************/
BOOL CRunProcessOnloadShellPNP::TurnOut(int nIndex, BOOL bOut)
{
	ASSERT(m_ITurnOut[0] > -1 && m_ITurnReturn[0] > -1 && m_OTurnOut[0] > -1 && m_OTurnReturn[0] > -1
		&& m_ITurnOut[1] > -1 && m_ITurnReturn[1] > -1 && m_OTurnOut[1] > -1 && m_OTurnReturn[1] > -1);

	ASSERT(-1 <= nIndex && nIndex < 2);

	BOOL bRes = FALSE;

	if (nIndex > -1 && nIndex < 2)
	{
		if(bOut)
		{
			OUTPUTS[m_OTurnOut[nIndex]]->On();
			OUTPUTS[m_OTurnReturn[nIndex]]->Off();

			if(WAIT_OK == WaitInput(INPUTS[m_ITurnReturn[nIndex]], OFF, MSG_SHOW_DEFAULT) 
				&& WAIT_OK == WaitInput(INPUTS[m_ITurnOut[nIndex]], ON, MSG_SHOW_DEFAULT))
			{
				bRes = TRUE;
			}
		}
		else
		{
			OUTPUTS[m_OTurnReturn[nIndex]]->On();
			OUTPUTS[m_OTurnOut[nIndex]]->Off();

			if(WAIT_OK == WaitInput(INPUTS[m_ITurnOut[nIndex]], OFF, MSG_SHOW_DEFAULT) 
				&& WAIT_OK == WaitInput(INPUTS[m_ITurnReturn[nIndex]], ON, MSG_SHOW_DEFAULT))
			{
				bRes = TRUE;
			}
		}
	}
	else
	{

		if(bOut)
		{
			for(int i = 0; i < 2; i++)
			{
				OUTPUTS[m_OTurnOut[i]]->On();
				OUTPUTS[m_OTurnReturn[i]]->Off();
			}

			if(WAIT_OK == WaitInput(INPUTS[m_ITurnReturn[0]], OFF, MSG_SHOW_DEFAULT) 
				&& WAIT_OK == WaitInput(INPUTS[m_ITurnOut[0]], ON, MSG_SHOW_DEFAULT)
				&& WAIT_OK == WaitInput(INPUTS[m_ITurnReturn[1]], OFF, MSG_SHOW_DEFAULT)
				&& WAIT_OK == WaitInput(INPUTS[m_ITurnOut[1]], ON, MSG_SHOW_DEFAULT))
			{
				bRes = TRUE;
			}			
		}
		else
		{
			for(int i = 0; i < 2; i++)
			{
				OUTPUTS[m_OTurnReturn[i]]->On();
				OUTPUTS[m_OTurnOut[i]]->Off();
			}

			if(WAIT_OK == WaitInput(INPUTS[m_ITurnOut[0]], OFF, MSG_SHOW_DEFAULT) 
				&& WAIT_OK == WaitInput(INPUTS[m_ITurnReturn[0]], ON, MSG_SHOW_DEFAULT)
				&& WAIT_OK == WaitInput(INPUTS[m_ITurnOut[1]], OFF, MSG_SHOW_DEFAULT) 
				&& WAIT_OK == WaitInput(INPUTS[m_ITurnReturn[1]], ON, MSG_SHOW_DEFAULT))
			{
				bRes = TRUE;
			}
		}
	}

	return bRes;	
}

/****************************************************************************************
�����������ر����
���������
�������أ�TRUE:ִ�гɹ���FALSE:ִ��ʧ��
*****************************************************************************************/
BOOL CRunProcessOnloadShellPNP::CloseVacu()
{
	ASSERT(m_ICheckVacu[0] > -1 && m_ICheckVacu[1] > -1 && m_OOpenVacu[0] > -1 && m_OOpenVacu[1] > -1);

	BOOL bRes = FALSE;

	for(int i = 0; i < 2; i++)
	{
		OUTPUTS[m_OOpenVacu[i]]->Off();
	}

	if(WAIT_OK == WaitInput(INPUTS[m_ICheckVacu[0]], OFF, MSG_SHOW_DEFAULT) 
		&& WAIT_OK == WaitInput(INPUTS[m_ICheckVacu[1]], OFF, MSG_SHOW_DEFAULT))
	{
		Sleep(100);
		bRes = TRUE;
	}

	return bRes;
}

//���ģ�鰲ȫ
BOOL CRunProcessOnloadShellPNP::ModuleIsSafe()
{
	ASSERT(m_nMotorZ> -1);
	CString str = _T("");

	if (MOTORS[m_nMotorZ]->IsOriRefKnown())
	{
		if (MOTORS[m_nMotorZ]->GetCurPos() > 5 )
		{
			BLMessageBox(_T("(M13)�������Z���ŷ����λ�ô���5mm�����ƶ�����ȫλ"));  
			return FALSE;
		}
	}
	else
	{
		BLMessageBox(_T("(M13)�������Z���ŷ����û�и�λ���븴λ"));  
		return FALSE;
	}

	return TRUE;
}


BOOL CRunProcessOnloadShellPNP::CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{

#ifdef _TEST_
	return TRUE;
#endif
	ASSERT(m_nMotorY > -1 && m_nMotorZ> -1);

	if (pMotor == MOTORS[m_nMotorY])
	{
		//�ж�Z���Ƿ��ڰ�ȫλ
		if (MOTORS[m_nMotorZ]->IsOriRefKnown())
		{
			double  dZMotorCurPos = 0.0;
			dZMotorCurPos = MOTORS[m_nMotorZ]->GetCurPos();
			if (MOTORS[m_nMotorZ]->GetCurPos() > 5) 
			{
				ShowMessage(_T("(M13)�������Z���ŷ����λ�ô���5mm��Y�᲻���ƶ�"), 2);   
				return FALSE;
			}
		}
		else
		{
			ShowMessage(_T("(M13)�������Z���ŷ����û�и�λ�����ȸ�λ��"), 2);
			return FALSE;
		}
	}

	if (pMotor == MOTORS[m_nMotorZ])
	{
		//ȡ��λ
		if (iLoc == Z_MOTOR_PICK_POS)
		{
			if ( Y_MOTOR_PICK_LOC1 > MOTORS[m_nMotorY]->GetCurLoc() 
				|| MOTORS[m_nMotorY]->GetCurLoc() > Y_MOTOR_PICK_LOC12)
			{
				BLMessageBox(_T("(M12)�������X���ŷ��������ȡ��λ��Z�᲻���ƶ���ȡ��λ"), MB_ICONWARNING);
				return FALSE;
			}

			for (int i = 0; i < 2; i++)
			{
				ASSERT(m_ITurnOut[i] > -1 && m_ITurnReturn[i] > -1);
				if (INPUTS[m_ITurnOut[i]]->IsOn() || INPUTS[m_ITurnReturn[i]]->IsOff())
				{
					BLMessageBox(_T("��ת���״����Ƴ�״̬��Z�᲻���ƶ���ȡ��λ"), MB_ICONWARNING);
					return FALSE;
				}
			}

			ASSERT(m_IPushCylPush > -1 && m_IPushCylBack > -1);
			if (INPUTS[m_IPushCylPush]->IsOn() || INPUTS[m_IPushCylBack]->IsOff())
			{
				BLMessageBox(_T("��Ǳ�������Ƴ���Z�᲻���ƶ���ȡ��λ"), MB_ICONWARNING);
				return FALSE;
			}
		}

		//����λ   ���ϵȴ�λ
		if (iLoc == Z_MOTOR_PLACE_POS || iLoc == Z_MOTOR_WAIT_PLACE_POS)
		{
			double motorcurpos = MOTORS[m_nMotorY]->GetCurPos() ;
			double motopos1  = GetMotorPos(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorY, Y_MOTOR_PLACE_LOC);
			if((fabs(motorcurpos-motopos1)) > 0.5)
			{
				BLMessageBox(_T("(M12)�������X���ŷ�������ڷ���λ��Z�᲻���ƶ�������λ����ϵȴ�λ"), MB_ICONWARNING);
				return FALSE;
			}

			for (int i = 0; i < 2; i++)
			{
				ASSERT(m_ITurnOut[i] > -1 && m_ITurnReturn[i] > -1);
				if (INPUTS[m_ITurnOut[i]]->IsOff() && INPUTS[m_ITurnReturn[i]]->IsOn())
				{
					BLMessageBox(_T("��ת���״��ڻ���״̬��Z�᲻���ƶ�������λ����ϵȴ�λ"), MB_ICONWARNING);
					return FALSE;
				}
			}

			ASSERT(m_IPushCylPush > -1 && m_IPushCylBack > -1);
			if (INPUTS[m_IPushCylPush]->IsOff() && INPUTS[m_IPushCylBack]->IsOn())
			{
				BLMessageBox(_T("��Ǳ�����׻��ˣ�Z�᲻���ƶ�������λ"), MB_ICONWARNING);
				return FALSE;
			}

			int ITransferCylPush = MACHINE.m_pCleanBefore->m_ITransferCylPush;
            int ITransferCylReturn = MACHINE.m_pCleanBefore->m_ITransferCylReturn;
			ASSERT(ITransferCylPush > -1 && ITransferCylReturn > -1);
			if (INPUTS[ITransferCylPush]->IsOn() && INPUTS[ITransferCylReturn]->IsOff())
			{
				BLMessageBox(_T("����ǰģ������������״����Ƴ�״̬��Z�᲻���ƶ�������λ"), MB_ICONWARNING);
				return FALSE;
			}

			int IRotateCylPush = MACHINE.m_pCleanBefore->m_IRotateCylPush;
			int IRotateCylReturn =MACHINE.m_pCleanBefore->m_IRotateCylReturn;
			ASSERT(IRotateCylPush > -1 && IRotateCylReturn > -1);
			if (INPUTS[IRotateCylPush]->IsOn() && INPUTS[IRotateCylReturn]->IsOff())
			{
				BLMessageBox(_T("����ǰģ����ת���״����Ƴ�״̬��Z�᲻���ƶ�������λ"), MB_ICONWARNING);
				return FALSE;
			}
		}
	}
	return TRUE;
}

BOOL CRunProcessOnloadShellPNP::CheckOutputCanActive(COutput *pOutput)
{
#ifdef _TEST_
	return TRUE;
#endif
	ASSERT(m_nMotorZ> -1);

	double  dPosZ0 = MOTORS[m_nMotorZ]->GetCurPos();
	if (MOTORS[m_nMotorZ]->GetCurPos() > 5)
	{
		ASSERT(m_OPushCylPush > -1 && m_OPushCylReturn> -1);
		if (pOutput == OUTPUTS[m_OPushCylPush] || pOutput == OUTPUTS[m_OPushCylReturn])
		{
			BLMessageBox(_T("(M13)�������Z��λ�ô���5mm����Ǳ�����ײ��ܶ�"), MB_ICONWARNING);
			return FALSE;
		}
		/*ASSERT(m_OTurnOut[0] > -1 && m_OTurnOut[1] > -1);
		if (pOutput == OUTPUTS[m_OTurnOut[0]] || pOutput == OUTPUTS[m_OTurnOut[1]])
		{
			BLMessageBox(_T("(M13)�������Z��λ�ô���5mm�������ת���ײ��ܶ�"), MB_ICONWARNING);
			return FALSE;
		}*/
	}

	if((m_IPushCylBack > -1 && INPUTS[m_IPushCylBack]->IsOn()) || (m_OPushCylPush > -1 && OUTPUTS[m_OPushCylPush]->IsOff()))
	{
		if((m_OTurnOut[0] > -1 && pOutput == OUTPUTS[m_OTurnOut[0]]) || (m_OTurnOut[1] > -1 && pOutput == OUTPUTS[m_OTurnOut[1]]))
		{
			BLMessageBox(_T("��Ǳ�����״��ڻ���״̬�������ת���ײ��ܶ�"), MB_ICONWARNING);
			return FALSE;
		}
	}
	return TRUE ;
}

BOOL CRunProcessOnloadShellPNP::CheckCanMoveMotor(void)
{
	ASSERT(m_nMotorZ> -1);

	if (MOTORS[m_nMotorZ]->IsOriRefKnown())
	{
		double  dZMotorCurPos = 0.0;
		dZMotorCurPos = MOTORS[m_nMotorZ]->GetCurPos();
		if (MOTORS[m_nMotorZ]->GetCurPos() > 5)
		{
			ShowMessage(_T("����ץ��Z�᲻�ڰ�ȫλ�ã������ƶ�"), 2);   
			return FALSE;
		}
	}
	else
	{
		ShowMessage(_T("ץ��Z����û�л��㣬���̵�������ƶ���"), 2);
		return FALSE;
	}

	return TRUE;
}

/****************************************************************************************
�����������Ƴ����׶���
���������bPush:TRUE:�Ƴ������Ƴ� FALSE:�Ƴ����׻���
�������أ�TRUE:ִ�гɹ���FALSE:ִ��ʧ��
*****************************************************************************************/
BOOL CRunProcessOnloadShellPNP::PushCylPush(BOOL bPush)
{
	ASSERT(m_IPushCylPush > -1 && m_IPushCylBack > -1 && m_OPushCylPush > -1 && m_OPushCylReturn > -1);

	BOOL bRes = FALSE;

	if (bPush)
	{
		OUTPUTS[m_OPushCylReturn]->Off();
		OUTPUTS[m_OPushCylPush]->On();

		if(WAIT_OK == WaitInput(INPUTS[m_IPushCylPush], ON, MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_IPushCylBack], OFF, MSG_SHOW_DEFAULT))
		{
			bRes = TRUE;
		}
	}
	else
	{
		OUTPUTS[m_OPushCylPush]->Off();
		OUTPUTS[m_OPushCylReturn]->On();

		if(WAIT_OK == WaitInput(INPUTS[m_IPushCylPush], OFF, MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_IPushCylBack], ON, MSG_SHOW_DEFAULT))
		{
			bRes = TRUE;
		}
	}
	return bRes;
}

BOOL CRunProcessOnloadShellPNP::CheckVacuStatus( BOOL bOpen )
{
	ASSERT(m_ICheckVacu[0] > -1 && m_ICheckVacu[1] > -1);

	BOOL bRes = FALSE;

	if(bOpen)
	{
		//if(INPUTS[m_ICheckVacu[0]]->IsOn() && INPUTS[m_ICheckVacu[1]]->IsOn())
		{
			Sleep(100);
			if(WAIT_OK == WaitInput(INPUTS[m_ICheckVacu[0]], ON, MSG_SHOW_DEFAULT)
				&& WAIT_OK == WaitInput(INPUTS[m_ICheckVacu[1]], ON, MSG_SHOW_DEFAULT))
			{
				bRes = TRUE;
			}
		}
	}
	else
	{
		//if(INPUTS[m_ICheckVacu[0]]->IsOff() && INPUTS[m_ICheckVacu[1]]->IsOff())
		{
			Sleep(100);
			if(WAIT_OK == WaitInput(INPUTS[m_ICheckVacu[0]], OFF, MSG_SHOW_DEFAULT)
				&& WAIT_OK == WaitInput(INPUTS[m_ICheckVacu[1]], OFF, MSG_SHOW_DEFAULT))
			{
				bRes = TRUE;
			}
		}
	}

	return bRes;
}

/****************************************************************************************
������������ȡ����
���������
�������أ�TRUE:ִ�гɹ���FALSE:ִ��ʧ��
*****************************************************************************************/
void CRunProcessOnloadShellPNP::ReadStepInfo()				//��ȡ������������
{
	CString strPath = GetAbsolutePathName(MODULE_RUN_FOLDER) + m_strName + _T(".cfg");

	m_nMotorMoveIndex = IniReadInt(_T("Data"), _T("m_nMotorMoveIndex"), 0, strPath);

	return;
}

/****************************************************************************************
������������������
���������
�������أ�TRUE:ִ�гɹ���FALSE:ִ��ʧ��
*****************************************************************************************/
BOOL CRunProcessOnloadShellPNP::SaveStepInfo(int nWriteType)   	//���������������
{
	CString strPath = GetAbsolutePathName(MODULE_RUN_FOLDER) + m_strName + _T(".cfg");

	if(nWriteType & WRITE_TYPE_VARIABLES)
	{
		IniWriteInt(_T("Data"), _T("m_nMotorMoveIndex"), m_nMotorMoveIndex, strPath);
	}

	return TRUE;
}