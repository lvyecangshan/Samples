/****************************************************************************************
�ļ�����RunProcessOffloadShellStackEx.cpp
��������������Ƕ�ջģ��
��ʷ�޸ļ�¼��
*****************************************************************************************/
#include "StdAfx.h"
#include "RunProcessOffloadShellStack.h"

CRunProcessOffloadShellStack::CRunProcessOffloadShellStack(int nID) :
CRunProcess(nID)
{
	PowerUpRestart();
	m_InputMap.insert(pair<int*, CString>(&m_IButtonReady, _T("IButtonReady")));
	m_InputMap.insert(pair<int*, CString>(&m_ICarInPosFront, _T("ICarInPosFront")));
	m_InputMap.insert(pair<int*, CString>(&m_ICarInPosBehind, _T("ICarInPosBehind")));
	m_InputMap.insert(pair<int*, CString>(&m_ILockCylPushOut, _T("ILockCylPushOut")));
	m_InputMap.insert(pair<int*, CString>(&m_ILockCylBack, _T("ILockCylBack")));

	m_InputMap.insert(pair<int*, CString>(&m_IEmptyCarCheck, _T("IEmptyCarCheck")));
	m_InputMap.insert(pair<int*, CString>(&m_IHavePallet, _T("IHavePallet")));
	m_InputMap.insert(pair<int*, CString>(&m_ITrayTopPresent, _T("ITrayTopPresent")));

	m_OutputMap.insert(pair<int*, CString>(&m_OLockCyl, _T("OLockCyl")));
	m_OutputMap.insert(pair<int*, CString>(&m_OReadyLight, _T("OReadyLight")));
	m_MotorMap.insert(pair<int*, CString>(&m_nMotorZ, _T("MotorZ")));

	m_VoidMap.insert(pair<void*, CString>(&m_dlZSafeLimit, _T("dZSafeLimit, ��ȫ��λ����")));
}

CRunProcessOffloadShellStack::~CRunProcessOffloadShellStack(void)
{
}

void CRunProcessOffloadShellStack::PowerUpRestart()
{
	CRunProcess::PowerUpRestart();
	m_strMsg = m_bLangulageEng? _T("Ready"):_T("׼����");

	m_bOffloadEmptyPallet = FALSE;
	m_bMotorSafePos = FALSE;
	m_bMoveMotorFinish = FALSE;
	m_bHomeMoveSafePos = FALSE;
	m_dMotorDownPos = 0;
	m_dMotorUpPos = 0;
	m_dPalletHigh = 120;
	m_nNextPowerUpStep = PU_LOCK_CAR_OPEN;
	m_nNextAutoStep = AUTO_CHECK_CAR_AND_PALLET;
}

void CRunProcessOffloadShellStack::PowerUpOperation(void)
{
	if (!IsModuleEnable())
	{
		CRun::PowerUpOperation();
		m_strMsg = m_bLangulageEng?_T("Moudle UnEnable"):_T("ģ�����");
		return;
	}

	switch(m_nNextPowerUpStep)
	{
		//�������״�
	case PU_LOCK_CAR_OPEN:
		{
			m_strMsg = m_bLangulageEng?_T("Release Car Lock Cyl Open"): _T("�������״�");

			if (::IsNoHardware() || LockCar(FALSE))
			{
				m_nNextPowerUpStep = PU_CHECK_HAVE_CAR;
			}
			break;
		}

		//�������С��
	case PU_CHECK_HAVE_CAR:
		{
			m_strMsg = m_bLangulageEng?_T("Check Have Car"): _T("�������С��");

			if (::IsNoHardware() || CheckCarInPos(FALSE))
			{
				if(m_OReadyLight > -1)
				{
					OUTPUTS[m_OReadyLight]->On();
				}
				m_nNextPowerUpStep =PU_WAIT_PEOPLE_CHECK_SAFE;
			}
			else
			{
				//�������϶�ջ�쳵��С����Ϊ��֤��ȫ�뽫С������
				ShowMessage(MSG_OFFLOAD_STACK_CHECK_CAR_VAILD);
			}
			break;
		}

		//�ȴ��˹�ȷ���Ƿ�ȫ
	case PU_WAIT_PEOPLE_CHECK_SAFE:
		{
			m_strMsg = m_bLangulageEng?_T("Wait People Check Safe"):_T("�ȴ��˹�ȷ���Ƿ�ȫ");

			if (::IsNoHardware() || (m_IButtonReady > -1 && INPUTS[m_IButtonReady]->IsOn()))
			{
				if(m_OReadyLight > -1)
				{
					OUTPUTS[m_OReadyLight]->Off();
				}
				m_nNextPowerUpStep = PU_SAFE_CLY_CLOSE;
			}
			break;
		}
		//��ȫ���״�
	case PU_SAFE_CLY_CLOSE:
		{
			m_strMsg = m_bLangulageEng?_T("Relase Car Lock Cyl Lock"): _T("�������׹ر�");

			if (::IsNoHardware() || LockCar(TRUE))
			{
				m_nNextPowerUpStep = PU_Z_MOTOR_MOVE_HOME;
			}
			break;
		}
		//Z������λ
	case PU_Z_MOTOR_MOVE_HOME:
		{
			m_strMsg = m_bLangulageEng?_T("Z Motor Move Home"): _T("Z������λ");

			if (m_nMotorZ > -1 && HomeMotor(MOTORS[m_nMotorZ]))
			{
				m_bHomeMoveSafePos = TRUE;
				m_nNextPowerUpStep = PU_CHECK_HAVE_PALLAT;
			}
			break;
		}

	case  PU_CHECK_HAVE_PALLAT:
		{
			m_strMsg = m_bLangulageEng?_T("Z Motor Move Home"): _T("�����������");

			if (m_IHavePallet > -1 && CheckInput(INPUTS[m_IHavePallet],OFF))
			{
				m_nNextPowerUpStep = PU_MOVE_Z_MTOR_TO_SAFE_POS;
			}
			break;
		}

		//�ƶ���������������λ
	case PU_MOVE_Z_MTOR_TO_SAFE_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Move Z Motor To Offload Pos1"): _T("�ƶ��������ȫλ");

			if (m_nMotorZ > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorZ, STACK_MOTOR_SAFE_POS))
			{
				m_nNextPowerUpStep = PU_FINISH;
			}
			break;
		}
	//case PU_MOTOR_DOWN_SAFE_DISTANCE:
	//	{
	//		m_strMsg = m_bLangulageEng?_T("Move Z Motor Down"): _T("����½�һ�ξ���");
	//		// ���ڵ���������쳣��ԭ����MotorMoveCheckFullPalletִ��ʧ�ܣ�Ϊ��֤��ȫ���Ƚ�����½�һ�ξ��������ִ�иú���
	//		double dMotorPos = MOTORS[m_nMotorZ]->GetCurPos() - 100;
	//		if(m_nMotorZ > -1 && MoveMotorPos(MOTORS[m_nMotorZ], dMotorPos))
	//		{
	//			m_nNextPowerUpStep = PU_MOVE_Z_MTOR_TO_SAFE_POS;
	//		}
	//		break;
	//	}
	//case PU_CHECK_PALLET_FULL:
	//	{
	//		m_strMsg = m_bLangulageEng?_T("Check Pallet Full"): _T("��������Ƿ�����");

	//		double dSafeLoc = GetMotorPos(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorZ, STACK_MOTOR_SAFE_POS);
	//		if(MOTORS[m_nMotorZ]->GetCurPos() - dSafeLoc < m_dPalletHigh)	// ��ǰλ�ü�ȥ��ȫλ����ֵС��һ�����̸߶ȣ���������
	//		{
	//			m_nNextAutoStep = AUTO_CHECK_CAR_AND_PALLET_AGAIN;
	//			m_nNextPowerUpStep = PU_FINISH;
	//		}
	//		else
	//		{
	//			m_nNextAutoStep = AUTO_CHECK_CAR_AND_PALLET;
	//			m_nNextPowerUpStep = PU_FINISH;
	//		}
	//	}
		//��ʼ�����
	case PU_FINISH:
		{
			CRun::PowerUpOperation();
			m_strMsg = m_bLangulageEng?_T("Power up finished"): _T("��ʼ�����");
			break;
		}
	default:
		{
			ASSERT(0);
			break;
		}
	}
}

void CRunProcessOffloadShellStack::AutoOperation()
{
	if (!IsModuleEnable())
	{
		m_strMsg = m_bLangulageEng?_T("Moudle not enable"):_T("ģ�����");
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
		if (m_nSaveLastStep == AUTO_CHECK_CAR_AND_PALLET)
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
		//��⳵������
	case AUTO_CHECK_CAR_AND_PALLET:
		{
			m_strMsg = m_bLangulageEng?_T("Check Car And Pallet"): _T("��⳵������");

			if(::IsNoHardware() || CheckCarAndPallet(FALSE))
			{
				m_nNextAutoStep = AUTO_WAIT_START_EVENT;
			}
			else
			{
				//�������϶�ջ�쳵��С����Ϊ��֤��ȫ�뽫С���ƿ�
				ShowMessage(MSG_OFFLOAD_STACK_CHECK_CAR_VAILD);
			}
			break;
		}

			//�ȴ���ʼ�ź�
	case AUTO_WAIT_START_EVENT:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Start Event"): _T("�ȴ���ʼ�ź�");

			if (m_bOffloadEmptyPallet || (MACHINE.m_pFeederTray != NULL && !MACHINE.m_pFeederTray->IsModuleEnable()))
			{
				m_nNextAutoStep = AUTO_MOVE_Z_MOTOR_TO_SAFE_POS;
			}
			break;
		}

		//�ƶ���������������λ
	case AUTO_MOVE_Z_MOTOR_TO_SAFE_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Move Z Motor To Safe Pos"): _T("�ƶ���������������λ");
			// ���ǵ���������ᵼ���ж�ʧ��
			if (MotorMoveCheckFullPallet())
			{
				m_nNextAutoStep = AUTO_CHECK_PALLET_IS_FULL;
			}
			else
			{
				m_nNextAutoStep=AUTO_MOTOR_Z_DOWN_TO_SAFE_DISTANCE;
			}
			break;
		}
	case AUTO_MOTOR_Z_DOWN_TO_SAFE_DISTANCE:
		{
			m_strMsg = m_bLangulageEng?_T("Move Z Motor Down"): _T("����½�һ�ξ���");
			// ���ڵ���������쳣��ԭ����MotorMoveCheckFullPalletִ��ʧ�ܣ�Ϊ��֤��ȫ���Ƚ�����½�һ�ξ��������ִ�иú���
			double dMotorPos = MOTORS[m_nMotorZ]->GetCurPos() - 100;
			if(dMotorPos >= 0 && m_nMotorZ > -1 && MoveMotorPos(MOTORS[m_nMotorZ], dMotorPos))
			{
				m_nNextAutoStep = AUTO_MOVE_Z_MOTOR_TO_SAFE_POS;
			}
			break;
		}
	case AUTO_CHECK_PALLET_IS_FULL:
		{
			m_strMsg = m_bLangulageEng?_T("Check Pallet Full"): _T("��������Ƿ�����");
			double dSafeLoc = GetMotorPos(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorZ, STACK_MOTOR_SAFE_POS);
			if(MOTORS[m_nMotorZ]->GetCurPos() - dSafeLoc < m_dPalletHigh)	// ��ǰλ�ü�ȥ��ȫλ����ֵС��һ�����̸߶ȣ���������
			{
				m_nNextAutoStep = AUTO_CHECK_CAR_AND_PALLET_AGAIN;
			}
			else
			{
				m_nNextAutoStep = AUTO_JUDGE_CAR_MOVE;
			}
			break;
		}
	

		//�ƶ�ǰ�ж�С�������ƿ�
	case AUTO_JUDGE_CAR_MOVE:
		{
			m_strMsg = m_bLangulageEng?_T("Judge Car Move"): _T("�ж�С���Ƿ�������");

			if(::IsNoHardware() || CheckCarInPos(FALSE))
			{
				m_dMotorUpPos = MOTORS[m_nMotorZ]->GetCurPos() + m_dlZSafeLimit;
				m_nNextAutoStep = AUTO_MOVE_Z_MOTOR_TO_WORK_POS;
			}
			else
			{
				// �������϶�ջ�쳵��С����Ϊ��֤��ȫ�뽫С���ƿ�
				ShowMessage(MSG_OFFLOAD_STACK_CHECK_CAR_VAILD);
			}
			break;
		}

		//�ƶ����������λ
	case AUTO_MOVE_Z_MOTOR_TO_WORK_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Move Z Motor To Work Pos"):_T("�ƶ����������λ");

			if (!m_bMoveMotorFinish && m_nMotorZ > -1 && MoveMotorPos(MOTORS[m_nMotorZ], m_dMotorUpPos))
			{
				m_bOffloadEmptyPallet = FALSE;
				m_dMotorDownPos = MOTORS[m_nMotorZ]->GetCurPos() - m_dlZSafeLimit - m_dPalletHigh;
				m_nNextAutoStep = AUTO_WAIT_PUT_PALLET_FINISH;
			}
			break;
		}

		//�ȴ��������
	case AUTO_WAIT_PUT_PALLET_FINISH:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Put Pallet Finish"): _T("�ȴ��������");
	
			if (MACHINE.m_pFeederTray != NULL && ( MACHINE.m_pFeederTray->m_bPutPalletFinish || !MACHINE.m_pFeederTray->IsModuleEnable()))
			{
				m_nNextAutoStep = AUTO_CHECK_CAR_REMOVE;
			}
			break;
		}

		// ���С���Ƿ��뿪
	case AUTO_CHECK_CAR_REMOVE:
		{
			m_strMsg = m_bLangulageEng?_T("Check Car Remove"): _T("���С���ƿ�");

			if(::IsNoHardware() || CheckCarInPos(FALSE))
			{
				m_nNextAutoStep = AUTO_MOVE_Z_MOTOR_TO_SAFE_POS_AFTER_PICK_PALLET;
			}
			else
			{
				// �������϶�ջ�쳵��С����Ϊ��֤��ȫ�뽫С���ƿ�
				ShowMessage(MSG_OFFLOAD_STACK_CHECK_CAR_VAILD);
			}
			break;
		}

		//�ƶ�Z�ᵽƫ��λ
	case AUTO_MOVE_Z_MOTOR_TO_SAFE_POS_AFTER_PICK_PALLET:
		{
			m_strMsg = m_bLangulageEng?_T("Move Z Motor To Safe Pos"): _T("�ƶ�Z�ᵽƫ��λ");

			if (m_dMotorDownPos >= 0 && m_nMotorZ > -1 && MoveMotorPos(MOTORS[m_nMotorZ], m_dMotorDownPos))
			{
				if (MACHINE.m_pFeederTray != NULL)
				{
					MACHINE.m_pFeederTray->m_bPutPalletFinish = FALSE;
				}
				m_nNextAutoStep = AUTO_JUDGE_PALLET_FULL;
			}
			break;
		}

		//�ж��������޽���
	case AUTO_JUDGE_PALLET_FULL:
		{
			m_strMsg = m_bLangulageEng?_T("Judge Pallet Full"): _T("�ж��������޽���");

			double dSafeLoc = GetMotorPos(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorZ, STACK_MOTOR_SAFE_POS);
			if(MOTORS[m_nMotorZ]->GetCurPos() - dSafeLoc < m_dPalletHigh)	// ��ǰλ�ü�ȥ��ȫλ����ֵС��һ�����̸߶ȣ���������
			{
				m_nNextAutoStep = AUTO_CHECK_CAR_AND_PALLET_AGAIN;
			}
			else
			{
				m_nNextAutoStep = AUTO_WAIT_START_EVENT;
			}
			break;
		}

		//�ٴμ����������
	case AUTO_CHECK_CAR_AND_PALLET_AGAIN:
		{
			m_strMsg = m_bLangulageEng?_T("Check Car And Pallet Again"): _T("�ٴμ��С��");

			if(::IsNoHardware() || CheckCarInPos(FALSE))
			{
				m_nNextAutoStep = AUTO_MOTOR_OFFLOAD_MOVE_TO_SAFE_POS;
			}
			break;
		}

		//����ƶ�����ȫλ
	case AUTO_MOTOR_OFFLOAD_MOVE_TO_SAFE_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Motor Move"): _T("����ƶ�����ȫλ");

			if(m_nMotorZ > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorZ, STACK_MOTOR_SAFE_POS))
			{
				MACHINE.m_bSafeStatckDoor[2] = TRUE;
				m_nNextAutoStep = AUTO_SET_OFFLOAD_PALLET_SINGLE;
			}
			break;
		}
	case AUTO_SET_OFFLOAD_PALLET_SINGLE:
		{
			m_strMsg = m_bLangulageEng?_T("Set Offload Pallet Single"): _T("�����¿������ź�");
			// ������϶�ջ������
			// MACHINE.SetBuzzerStatue(TRUE);  mh
			if(m_OReadyLight > -1)
			{
				OUTPUTS[m_OReadyLight]->On();
			}
			m_nNextAutoStep = AUTO_WAIT_LOCK_CYL_LOCK;
			break;
		}
		//С����ȥ��������������
	case AUTO_WAIT_LOCK_CYL_LOCK:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Lock Cyl Lock"): _T("С����ȥ���������״�");

			if (::IsNoHardware() || LockCar(FALSE))
			{
				m_nNextAutoStep = AUTO_WAIT_PEOPLE_MOVE_CAR_IN;
			}

			if(!IsNoHardware() && !m_bDryRun)
			{
				// �������϶�ջ���������������뽫С���ƽ���ջ�ӿ�����
				ShowMessage(MSG_OFFLOAD_STACK_PROMPT_OFFLOAD_EMPTY_PALLET);
			}
			break;
		}

		//�ȴ��˰�С���ƽ�ȥ
	case AUTO_WAIT_PEOPLE_MOVE_CAR_IN:
		{
			m_strMsg = m_bLangulageEng?_T("Wait People Move Car In"): _T("�ȴ�С���ƽ����϶�ջ");

			if (::IsNoHardware() || m_bDryRun || m_IButtonReady > -1 && INPUTS[m_IButtonReady]->IsOn())  //���ܲ�����ť
			{
				 // ������϶�ջ������
				//MACHINE.SetBuzzerStatue(FALSE);  mh
				if(m_OReadyLight > -1)
				{
					OUTPUTS[m_OReadyLight]->Off();
				}
				m_nNextAutoStep = AUTO_CHECK_HAVE_CAR;
			}
			break;
		}

		//���С�����޵�λ
	case AUTO_CHECK_HAVE_CAR:
		{
			m_strMsg = m_bLangulageEng?_T("Check Have Car"): _T("���С���Ƿ��Ѿ���λ");

			if (::IsNoHardware() || m_bDryRun || CheckCarInPos(TRUE))
			{
				m_nNextAutoStep = AUTO_LOC_CYL_LOCATION_LOCK;
			}
			else
			{
				ShowMessage(MSG_OFFLOAD_STACK_CHECK_CAR_NOT_INPOS);
				m_nNextAutoStep = AUTO_SET_OFFLOAD_PALLET_SINGLE;
			}
			break;
		}

		//С��������������
	case AUTO_LOC_CYL_LOCATION_LOCK:
		{
			m_strMsg = m_bLangulageEng?_T("Loc Cyl Location Lock"): _T("С��������������");

			if(IsNoHardware() || LockCar(TRUE))
			{
				m_nNextAutoStep = AUTO_MOVE_Z_MOTOR_TO_RELASE_POS;
			}
			break;
		}

		//�ƶ����������λ
	case AUTO_MOVE_Z_MOTOR_TO_RELASE_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Move Z Motor To Relase Pos"): _T("�ƶ����������λ");

			if (m_nMotorZ > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorZ, STACK_MOTOR_RELASE_CAR_LOC))
			{
				m_nNextAutoStep = AUTO_LOCK_CYL_OPEN_ON_REMOVE_CAR;
			}
			break;
		}

		//�������״�
	case AUTO_LOCK_CYL_OPEN_ON_REMOVE_CAR:
		{
			m_strMsg = m_bLangulageEng?_T("Lock Cyl Open On "): _T("�������״�");

			if(IsNoHardware() || LockCar(FALSE))
			{
				if(m_OReadyLight > -1)
				{
					OUTPUTS[m_OReadyLight]->On();
				}
				m_nNextAutoStep = AUTO_REMOVE_CAR_AFTER_OFFLOAD;
			}
			break;
		}

		//�ȴ��˹��ѳ�����
	case AUTO_REMOVE_CAR_AFTER_OFFLOAD:
		{
			m_strMsg = m_bLangulageEng?_T("Lock Cyl Open On Remove Car"): _T("�ȴ��˹��ѳ�����");

			if(::IsNoHardware() || m_bDryRun ||(m_IButtonReady > -1 && INPUTS[m_IButtonReady]->IsOn())) //���ܲ�����ť
			{
				if(m_OReadyLight > -1)
				{
					OUTPUTS[m_OReadyLight]->Off();
				}
				m_nNextAutoStep = AUTO_CHECK_HAVE_NO_PALLET;
			}
			break;
		}

		//�����������
	case AUTO_CHECK_HAVE_NO_PALLET:
		{
			m_strMsg = m_bLangulageEng?_T("Check Have No Pallet"): _T("�����������");

			if(::IsNoHardware() || CheckHaveNoPallet())
			{
				MACHINE.m_bSafeStatckDoor[2] = FALSE;
				m_nNextAutoStep = AUTO_SAFE_LOCK_CYL_CLOSE;
			}
			else
			{
				ShowMessage(_T("���϶�ջ�������̣�����С���Ƿ�����"),3);
			}
			break;
		}

		//��ȫ���׹ر�
	case AUTO_SAFE_LOCK_CYL_CLOSE:
		{
			m_strMsg = m_bLangulageEng?_T("Safe Lock Cyl Close"): _T(" �������׹ر�");

			if(::IsNoHardware() || LockCar(TRUE))
			{
				m_nNextAutoStep = AUTO_WORK_END;
			}
			break;
		}

		//�������
	case AUTO_WORK_END:
		{
			m_strMsg = m_bLangulageEng? _T("Work end"):_T("�������");
			m_nNextAutoStep = AUTO_CHECK_CAR_AND_PALLET;
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

		if(m_nNextAutoStep == AUTO_CHECK_CAR_AND_PALLET)
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

BOOL CRunProcessOffloadShellStack::InitializeConfig(CString strProcessModule)
{
	if (!CRunProcess::InitializeConfig(strProcessModule))
	{
		return FALSE;
	}
	return TRUE;
}

void CRunProcessOffloadShellStack::GetHardwareConfig(CArray<int, int> *pInputs, CArray<int, int> *pOutputs, CArray<int, int> *pMotors)
{
	CRunProcess::GetHardwareConfig(pInputs, pOutputs, pMotors);
}

/****************************************************************************************
�����������������׶���
���������bLock:TRUE:������������ FALSE:�������״�
�������أ�TRUE:ִ�гɹ���FALSE:ִ��ʧ��
*****************************************************************************************/
BOOL CRunProcessOffloadShellStack::LockCar(BOOL bLock)
{
	if (m_ILockCylPushOut < 0 || m_ILockCylBack  < 0 || m_OLockCyl < 0)
	{
		return FALSE;
	}

	if (bLock)
	{
		OUTPUTS[m_OLockCyl]->On();

		if (WAIT_OK == WaitInput(INPUTS[m_ILockCylPushOut], ON, MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_ILockCylBack], OFF, MSG_SHOW_DEFAULT))
		{
			return TRUE;
		}
	}
	else 
	{
		OUTPUTS[m_OLockCyl]->Off();
		if (WAIT_OK == WaitInput(INPUTS[m_ILockCylBack], ON, MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_ILockCylPushOut], OFF, MSG_SHOW_DEFAULT))
		{
			return TRUE;
		}
	}
	return FALSE;
}

/****************************************************************************************
�������������С����λ
���������bHave:TRUE:���С����λ FALSE:�ж�û��С��
�������أ�TRUE:ִ�гɹ���FALSE:ִ��ʧ��
*****************************************************************************************/
BOOL CRunProcessOffloadShellStack::CheckCarInPos(BOOL bHave)
{
	BOOL bCheckRes = FALSE;
	ASSERT(m_ICarInPosBehind > -1 && m_ICarInPosFront > -1);

	if(bHave)
	{
		if (INPUTS[m_ICarInPosBehind]->IsOn() && INPUTS[m_ICarInPosFront]->IsOn())
		{
			Sleep(300);
			if (INPUTS[m_ICarInPosBehind]->IsOn() && INPUTS[m_ICarInPosFront]->IsOn())
			{
				bCheckRes = TRUE;
			}
		}
	}
	else
	{
		if (INPUTS[m_ICarInPosBehind]->IsOff() && INPUTS[m_ICarInPosFront]->IsOff())
		{
			Sleep(300);
			if (INPUTS[m_ICarInPosBehind]->IsOff() && INPUTS[m_ICarInPosFront]->IsOff())
			{
				bCheckRes = TRUE;
			}
		}
	}
	return bCheckRes;
}


BOOL CRunProcessOffloadShellStack::CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{
#ifdef _TEST_
	return TRUE;
#endif
	if (m_nMotorZ > -1 && pMotor == MOTORS[m_nMotorZ])
	{
		if(BLMessageBox(_T("Z�����ƶ�֮ǰ�������̰�ȫ������ȫ���'�ǡ�Z�����ƶ�"), MB_YESNO)==IDYES)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	
	ASSERT(m_nMotorZ > -1);
	
	if (pMotor == MOTORS[m_nMotorZ])
	{
		double dPosZ0 = MOTORS[m_nMotorZ]->GetCurPos();
		if (bAbsMove == 1 && 500 < lfValue || bAbsMove == 0 && 500 < dPosZ0 + lfValue )
		{
			BLMessageBox(_T("(M10)�������Z���ŷ�����ƶ�������ȫ��λ"), MB_ICONWARNING);
			return FALSE;
		}

		if(MACHINE.m_pFeederTray != NULL)
		{
			int ITrayInPos[3];
			for(int i = 0; i < 2; i++)
			{
				ITrayInPos[i] = MACHINE.m_pFeederTray->m_ITrayInPosPresent[i];
				ASSERT(ITrayInPos[i] > -1);
			}
			if (INPUTS[ITrayInPos[0]]->IsOff() && INPUTS[ITrayInPos[1]]->IsOff() && INPUTS[ITrayInPos[2]]->IsOff())
			{
				BLMessageBox(_T("X0408/X0409/X0410�����ƶ���λ��Ӧû�������뽫��M11���������Y���ŷ�����ƶ���λ"), MB_ICONWARNING);
				return FALSE;
			}
		}

		ASSERT(m_IHavePallet > -1);
		if (INPUTS[m_IHavePallet]->IsOn())  //��ջ�������ֻ�������ߣ����±���
		{
			double dPosZ = MOTORS[m_nMotorZ]->GetCurPos();
			if (bAbsMove == 1 && dPosZ > lfValue || bAbsMove == 0 && 0 > lfValue)
			{
				BLMessageBox(_T("������϶�ջ������ϣ�(M10)�������Z���ŷ�������������ƶ�"), MB_ICONWARNING);
				return FALSE;
			}
		}

		ASSERT(m_ICarInPosFront > -1 && m_ICarInPosBehind > -1);
		ASSERT(m_ILockCylPushOut > -1 && m_ILockCylBack > -1);
		if (INPUTS[m_ICarInPosFront]->IsOn())
		{
			if (INPUTS[m_ICarInPosBehind]->IsOn() || INPUTS[m_ILockCylPushOut]->IsOff() || INPUTS[m_ILockCylBack]->IsOn())
			{
				BLMessageBox(_T("С��û���Ƶ�λ���������Z���ŷ���������ƶ�"), MB_ICONWARNING);
				return FALSE;
			}
		}
	}
	return TRUE;
}

BOOL CRunProcessOffloadShellStack::CheckOutputCanActive(COutput *pOutput)
{
	return TRUE;
}

BOOL CRunProcessOffloadShellStack::ModuleIsSafe()
{	
	if (m_nMotorZ > -1 && MOTORS[m_nMotorZ]->GetCurPos() > 500)
	{
		return FALSE;
	}
	return TRUE;
}

/****************************************************************************************
�����������ж�û������
���������
�������أ�TRUE:û�����̣�FALSE:������
*****************************************************************************************/
BOOL CRunProcessOffloadShellStack::CheckHaveNoPallet()
{
	ASSERT(m_IHavePallet > -1);

	BOOL bCheckRes = TRUE;

	for(int i = 0; i < 3; i++)
	{
		if(INPUTS[m_IHavePallet]->IsOn())
		{
			bCheckRes = FALSE;
			break;
		}
		Sleep(100);
	}

	return bCheckRes;
}

BOOL CRunProcessOffloadShellStack::MonitorOut()
{	
	return TRUE;
}

BOOL CRunProcessOffloadShellStack::CheckCarAndPallet( BOOL bHas /*= TRUE*/ )
{
	ASSERT(m_IHavePallet > -1 && m_ICarInPosBehind > -1 && m_ICarInPosFront > -1);

	BOOL bHasPallet = TRUE;

	if (bHas)
	{
		for(int i = 0; i< 3;i++)
		{
			if (INPUTS[m_IHavePallet]->IsOff() && INPUTS[m_ICarInPosBehind]->IsOff() && INPUTS[m_ICarInPosFront]->IsOff())
			{
				bHasPallet = FALSE;
				break;
			}
			Sleep(100);
		}
	}
	else
	{
		for(int i = 0; i< 3;i++)
		{
			if (INPUTS[m_IHavePallet]->IsOn() || INPUTS[m_ICarInPosBehind]->IsOn() || INPUTS[m_ICarInPosFront]->IsOn())
			{
				bHasPallet = FALSE;
				break;
			}
			Sleep(100);
		}
	}
	return bHasPallet;
}

BOOL CRunProcessOffloadShellStack::MotorHandMove( CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove )
{
	if (m_nMotorZ > -1 && pMotor == MOTORS[m_nMotorZ])
	{
		if (!MOTORS[m_nMotorZ]->IsOriRefKnown() && bAbsMove)
		{
			BLMessageBox(_T("���û�и�λ���븴λ��ɺ��ٲ���!"), MB_OK);
			return FALSE;
		}
		if (M_BUSY == MOTORS[m_nMotorZ]->GetMotionStatus())
		{
			BLMessageBox(_T("��������ƶ������ƶ���ɺ��ٲ���!"), MB_OK);
			return FALSE;
		}

		BOOL bMove = bAbsMove ? lfValue > MOTORS[m_nMotorZ]->GetCurPos() : lfValue > 0.1;
		BOOL bDownMove = bAbsMove ? lfValue < MOTORS[m_nMotorZ]->GetCurPos() : lfValue < 0.1;

		if ((m_ICarInPosBehind < 0 || INPUTS[m_ICarInPosBehind]->IsOn() || m_ICarInPosFront < 0 || INPUTS[m_ICarInPosFront]->IsOn())
			&& MOTORS[m_nMotorZ]->GetCurPos() > GetMotorPos(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorZ, STACK_MOTOR_SAFE_POS)+1.0 &&bDownMove)
		{
			BLMessageBox(_T("���̲��ڰ�ȫλ����⵽��С����Z�᲻�������ƶ����뽫С��ȡ���Ƶ���ȫλ���������ƶ�"), MB_OK);
			return FALSE;
		}

		if (bAbsMove)
		{
			MOTORS[m_nMotorZ]->TMoveAbs(lfValue, iLoc);
		}
		else
		{
			MOTORS[m_nMotorZ]->TMoveRel(lfValue, iLoc);
		}

		DWORD dwStartTime = GetTickCount();
		Sleep(200);
		while(1)
		{
			if ((m_ITrayTopPresent < 0 ||  INPUTS[m_ITrayTopPresent]->IsOn()) && bMove)
			{
				MOTORS[m_nMotorZ]->Stop(1);
				break;
			}

			if (M_BUSY != MOTORS[m_nMotorZ]->GetMotionStatus())
			{
				MOTORS[m_nMotorZ]->Stop(1);
				break;
			}

			if (GetTickCount() - dwStartTime > 20000)
			{
				MOTORS[m_nMotorZ]->Stop();
				BLMessageBox(_T("Z��10����û�ƶ���Ŀ��λ�ã�������ƶ���"), MB_OK);
				break;
			}
			Sleep(1);
		}
		return FALSE;//��ֹ�����ƶ�������ΪFALSE
	}
	return TRUE;
}

BOOL CRunProcessOffloadShellStack::  ReadRunParameter()
{
	CRunProcess:: ReadRunParameter();
	m_dlZSafeLimit = IniReadDouble(m_strProcess, _T("ZSafeLimit"), 150.00, GetAbsolutePathName(PARAMETER_CFG));
	
	return TRUE;
}

BOOL CRunProcessOffloadShellStack::SaveParameter()
{
	if (!IniWriteDouble(m_strProcess, _T("ZSafeLimit"), m_dlZSafeLimit, GetAbsolutePathName(PARAMETER_CFG)))
	{
		return FALSE;
	}

	CRunProcess::SaveParameter();
	return TRUE;
}
BOOL CRunProcessOffloadShellStack::CheckHavePaller(BOOL bHas /*= TRUE*/)
{
	BOOL bHasPallet = TRUE;//���̱�־
	if (bHas)
	{
		for(int i = 0; i< 3;i++)
		{
			if (m_IHavePallet > -1 && INPUTS[m_IHavePallet]->IsOff() )
			{
				bHasPallet = FALSE;//���û���̣�����FALSE
				break;
			}
			Sleep(100);
		}
	}
	else
	{
		for(int i = 0; i< 3;i++)
		{
			if (m_IHavePallet > -1 && INPUTS[m_IHavePallet]->IsOn())
			{
				bHasPallet = FALSE;//������̣�����FALSE
				break;
			}
			Sleep(100);
		}
	}
	return bHasPallet;
}

BOOL CRunProcessOffloadShellStack::MotorMoveCheckFullPallet(void)
{
	ASSERT(m_ITrayTopPresent > -1);

	BOOL bMotorZAccess = FALSE;

	if(m_nMotorZ > -1)
	{
		if(INPUTS[m_ITrayTopPresent]->IsOff())
		{
			MOTORS[m_nMotorZ]->m_dAloneSpeedRatio = 1;
			double dLocPos = GetMotorPos(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorZ,STACK_MOTOR_RECV_PALLET);
			MOTORS[m_nMotorZ]->TMoveAbs(dLocPos);
			Sleep(300);

			DWORD dwMotorWorkTime = GetTickCount();
			int nMotorStatus = M_OK;
			int nMachineStatus = MC_BEGIN;

			while(GetTickCount() - dwMotorWorkTime < 15000)
			{
				nMotorStatus = MOTORS[m_nMotorZ]->GetMotionStatus();
				nMachineStatus = MACHINE.m_pRunCtrl->GetMcState();

				if(m_evtMcStop == ON || (nMotorStatus != M_OK && nMotorStatus != M_BUSY) || nMachineStatus == MC_STOP_RUN || nMachineStatus == MC_RUN_JAM)
				{
					MOTORS[m_nMotorZ]->Stop(1);
					break;
				}

				if (m_bDryRun && GetTickCount() - dwMotorWorkTime > 3000)  //�����ж�
				{
					bMotorZAccess = TRUE;
					MOTORS[m_nMotorZ]->Stop(1);
					Sleep(5);
					break;
				}

				if (IsNoHardware() || INPUTS[m_ITrayTopPresent]->IsOn())
				{
					bMotorZAccess = TRUE;
					MOTORS[m_nMotorZ]->Stop(1);
					Sleep(5);
					break;
				}
				Sleep(2);
			}
			MOTORS[m_nMotorZ]->Stop(1);
		}
		else
		{
			bMotorZAccess = TRUE;
		}


		if(!m_bDryRun&&bMotorZAccess)
		{
			if(WAIT_OK != CheckInput(INPUTS[m_ITrayTopPresent], ON))
			{
				bMotorZAccess = FALSE;
			}
		}
	}
	
	return bMotorZAccess;
}