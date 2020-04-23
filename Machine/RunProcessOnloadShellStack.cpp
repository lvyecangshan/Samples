/****************************************************************************************
�ļ�����RunProcessOnloadShellStackEx.cpp
��������������Ƕ�ջģ��
��ʷ�޸ļ�¼��
*****************************************************************************************/
#include "StdAfx.h"
#include "RunProcessOnloadShellStack.h"

CRunProcessOnloadShellStack::CRunProcessOnloadShellStack(int nID) :
CRunProcess(nID)
{
	PowerUpRestart();

	m_InputMap.insert(pair<int*, CString>(&m_IButtonReady, _T("IButtonReady")));
	m_InputMap.insert(pair<int*, CString>(&m_ICarInPosFront, _T("ICarInPosFront")));
	m_InputMap.insert(pair<int*, CString>(&m_ICarInPosBehind, _T("ICarInPosBehind")));
	m_InputMap.insert(pair<int*, CString>(&m_ILockCylPushOut, _T("ILockCylPushOut")));
	m_InputMap.insert(pair<int*, CString>(&m_ILockCylBack, _T("ILockCylBack")));
	m_InputMap.insert(pair<int*, CString>(&m_IPalletMoveInPos, _T("IPalletMoveInPos")));
	m_InputMap.insert(pair<int*, CString>(&m_IHavePallet, _T("IHavePallet")));
	m_InputMap.insert(pair<int*, CString>(&m_ITrayTopPresent, _T("ITrayTopPresent")));

	m_OutputMap.insert(pair<int*, CString>(&m_OLockCyl, _T("OLockCyl")));
	m_OutputMap.insert(pair<int*, CString>(&m_OReadyLight, _T("OReadyLight")));

	m_MotorMap.insert(pair<int*, CString>(&m_nMotorZ, _T("MotorZ")));

	m_VoidMap.insert(pair<void*, CString>(&m_dlZSafeLimit, _T("dZSafeLimit, ��ȫ��λ����")));
}

CRunProcessOnloadShellStack::~CRunProcessOnloadShellStack(void)
{
}

void CRunProcessOnloadShellStack::PowerUpRestart()
{
	CRunProcess::PowerUpRestart();
	m_strMsg = m_bLangulageEng? _T("Ready"):_T("׼����");

	m_bHomeMoveSafePos = FALSE;
	m_bMotorSafePos = FALSE;
	m_bHavePallet = FALSE;
	m_bSendPalletFinish = FALSE;
	m_bRequirePallet = FALSE;
	m_dMotorPos = 0.00;

	m_nNextPowerUpStep = PU_RELASE_CAR_LOCK_CYL_OPEN;
	m_nNextAutoStep = AUTO_SAFE_CYL_OPEN;
}

void CRunProcessOnloadShellStack::PowerUpOperation(void)
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
	case PU_RELASE_CAR_LOCK_CYL_OPEN:
		{
			m_strMsg = m_bLangulageEng?_T("Relase Car Lock Cyl Lock"): _T("С����ȫ���״�");

			if (::IsNoHardware() || LockCar(FALSE))
			{
				m_nNextPowerUpStep = PU_CHECK_PALLET_DATA;
			}
			break;
		}
		//���С��
	case PU_CHECK_PALLET_DATA:
		{
			m_strMsg = m_bLangulageEng?_T("Check Have Pallet"): _T("���С���Ƿ�����");
			if (::IsNoHardware() || CheckHaveNoCar())
			{
				if (m_OReadyLight > -1)
				{
					OUTPUTS[m_OReadyLight]->On();
				}
				m_nNextPowerUpStep = PU_SAFE_CYL_CLOSE;
			}
			else
			{
				m_nNextPowerUpStep = PU_RELASE_CAR_LOCK_CYL_OPEN;
				ShowMessage(MSG_ONLOAD_STACK_CHECK_CAR_VAILD);
			}
			break;
		}

		//�밴ȷ�ϰ�ť��ȫ���������ر�
	case PU_SAFE_CYL_CLOSE:
		{
			m_strMsg = m_bLangulageEng?_T("Relase Car Lock Cyl Lock"): _T("�밴ȷ�ϰ�ť��ȫ���������ر�");
			if (::IsNoHardware() || (m_IButtonReady > -1 && INPUTS[m_IButtonReady]->IsOn()))
			{
				if (::IsNoHardware() ||LockCar(FALSE))
				{
					if (m_OReadyLight > -1)
					{
						OUTPUTS[m_OReadyLight]->Off();
					}
					m_nNextPowerUpStep = PU_Z_MOTOR_MOVE_HOME;
				}
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
				m_nNextPowerUpStep = PU_CHECK_HAVE_PALLET;
			}
			break;
		}

	case PU_CHECK_HAVE_PALLET:
		{
			m_strMsg = m_bLangulageEng?_T("Check Have Pallet"): _T("����Ƿ�������");

			ASSERT(m_IHavePallet > -1);

		/*	if (CheckHavePallet(TRUE))
			{
				m_bHavePallet = TRUE;
				m_nNextAutoStep = AUTO_WAIT_START_EVENT;
			}
			else*/
			{
				m_bHavePallet = FALSE;
				m_nNextAutoStep = AUTO_SAFE_CYL_OPEN;
			}
			m_nNextPowerUpStep = PU_Z_MOTOR_MOVE_WAIT_POS;
			break;
		}
		//Z�����ƶ����ȴ�λ
	case PU_Z_MOTOR_MOVE_WAIT_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Z Motor Move Wait Pos"): _T("Z�����ƶ����ȴ�λ");

			if (m_nMotorZ > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorZ, STACK_MOTOR_WAIT_LOC)) 
			{
				m_nNextPowerUpStep = PU_FINISH;
			}
			break;
		}
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

void CRunProcessOnloadShellStack::AutoOperation()
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
		if (m_nSaveLastStep == AUTO_SAFE_CYL_OPEN)
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
		//��ȫ���׹ر�
	case AUTO_SAFE_CYL_OPEN:
		{
			m_strMsg = m_bLangulageEng?_T("Lock Cyl Open"):_T("�������״�");

			if (::IsNoHardware() || LockCar(FALSE))
			{
				(m_strProcess == _T("OnloadShellStack1"))?(MACHINE.m_bSafeStatckDoor[0] = TRUE):(MACHINE.m_bSafeStatckDoor[1] = TRUE);
				if(m_OReadyLight > -1)
				{
					OUTPUTS[m_OReadyLight]->On();
				}
				m_nNextAutoStep = AUTO_WAIT_PEOPLE_ONLOAD;
			}
			break;
		}

		//�ȴ��˹�����
	case AUTO_WAIT_PEOPLE_ONLOAD:
		{
			m_strMsg = m_bLangulageEng?_T("Wait People Onload"):_T("�ȴ��˹�����");

			if (::IsNoHardware() || m_bDryRun|| (m_IButtonReady > -1 && INPUTS[m_IButtonReady]->IsOn()))
			{
				Sleep(300);
				if (::IsNoHardware() || m_bDryRun || (m_IButtonReady > -1 && INPUTS[m_IButtonReady]->IsOn()))
				{
					if(m_OReadyLight > -1 && OUTPUTS[m_OReadyLight]->IsOn())
					{
						OUTPUTS[m_OReadyLight]->Off();
					}
					m_nNextAutoStep = AUTO_CHECK_ONLOAD;
				}
			}
			break;
		}

		//���С�����޵�λ
	case AUTO_CHECK_ONLOAD:
		{
			m_strMsg = m_bLangulageEng?_T("Check Onload"):_T("���С�����޵�λ");

			if (::IsNoHardware() || m_bDryRun || CheckCarAndPallet(TRUE))
			{
				m_nNextAutoStep = AUTO_LOCK_CYL_LOCK;
			}
			else
			{
				//���Ƕ�ջ#1/2�쳵С��������״̬�쳣, Ϊȷ���豸�������У�����С��ǰ/��λ������(ON)�����̴�����(ON)
				ShowMessage(MSG_ONLOAD_STACK1_CHECK_CAR_AND_PALLET_ABERRANT + m_nGroupID);
				if(m_OReadyLight > -1 && OUTPUTS[m_OReadyLight]->IsOff())
				{
					OUTPUTS[m_OReadyLight]->On();
				}
				m_nNextAutoStep= AUTO_WAIT_PEOPLE_ONLOAD;
			}
			break;
		}

		//������������
	case AUTO_LOCK_CYL_LOCK:
		{
			m_strMsg = m_bLangulageEng?_T("Lock Cyl Lock"):_T("������������");

			if(::IsNoHardware() || LockCar(TRUE))
			{
				m_nNextAutoStep = AUTO_MOVE_Z_MOTOR_TO_RELASE_CAR_LOC;
			}
			break;
		}
		//�ƶ������С������λ
	case AUTO_MOVE_Z_MOTOR_TO_RELASE_CAR_LOC:
		{
			m_strMsg = m_bLangulageEng?_T("Move Z Motor To Relase Pos"):_T("�ƶ������С������λ");

			if (m_nMotorZ > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorZ, STACK_MOTOR_RELASE_CAR_LOC)) 
			{
				if(m_OLockCyl > -1)
				{// ������λ���������״�
					OUTPUTS[m_OLockCyl]->Off();
				}
				if (m_bDryRun || (m_IHavePallet > -1 && CheckInput(INPUTS[m_IHavePallet],ON)))
				{
					m_nNextAutoStep = AUTO_CHECK_LIMIT_CORE;
				}	
			}
			break;
		}

	case AUTO_CHECK_LIMIT_CORE:
		{
			m_strMsg = m_bLangulageEng?_T("Move Z Motor To Relase Pos"):_T("������̸߶��Ƿ���λ");

			if (::IsNoHardware() || (m_ITrayTopPresent > -1 && INPUTS[m_ITrayTopPresent]->IsOff()))
			{
				m_bHavePallet = TRUE;
				m_nNextAutoStep = AUTO_LOCK_CYL_LOCK_ONE;
			}
			else
			{
				//�������϶�ջ�����������࣬�ѳ�����ȫ��λ��Ӧ����������!
				ShowMessage(MSG_ONLOAD_STACK_PALLET_TOO_MANY);
			}
			break;
		}

		//���������ɿ�
	case AUTO_LOCK_CYL_LOCK_ONE:
		{
			m_strMsg = m_bLangulageEng?_T("Lock Cyl Lock"):_T("���������ɿ�");

			if(IsNoHardware() || LockCar(FALSE))
			{
				if(m_OReadyLight > -1 && OUTPUTS[m_OReadyLight]->IsOff())
				{
					OUTPUTS[m_OReadyLight]->On();
				}
				m_nNextAutoStep = AUTO_WAIT_PEOPLE_CHECK_SAFE;
			}
			break;
		}

		//������λ��ȴ��˹�ȷ���Ƿ�ȫ
	case AUTO_WAIT_PEOPLE_CHECK_SAFE:
		{
			m_strMsg = m_bLangulageEng?_T("Wait People Check Safe"):_T("������λ��ȴ��˹�ȷ���Ƿ�ȫ");

			if (::IsNoHardware() ||m_bDryRun|| (m_IButtonReady > -1 && INPUTS[m_IButtonReady]->IsOn()))
			{
				(m_strProcess == _T("OnloadShellStack1"))?(MACHINE.m_bSafeStatckDoor[0] = FALSE):(MACHINE.m_bSafeStatckDoor[1] = FALSE);
				if (::IsNoHardware() ||  CheckHaveNoCar())
				{
					if(m_OReadyLight > -1)
					{
						OUTPUTS[m_OReadyLight]->Off();
					}
					m_nNextAutoStep = AUTO_WAIT_START_EVENT;
				}
				else
				{
					//���Ƕ�ջ���Ϻ��⵽С��δ��ȫ���ߣ���ȷ��!
					ShowMessage(MSG_ONLOAD_STACK_CHECK_CAR_NOT_REMOVE);
				}
			}
			break;
		}

		//�ȴ���ʼ�ź�
	case AUTO_WAIT_START_EVENT:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Start Event"):_T("�ȴ�����ģ�鿪ʼ�����ź�");
		
			if (!m_bMotorSafePos && (m_bRequirePallet || (MACHINE.m_pFeederTray != NULL && !MACHINE.m_pFeederTray->IsModuleEnable())))
			{
				if (!m_bDryRun && !CheckHavePallet(TRUE))     //�ȴ�����Ҫ���ٴμ��������
				{
					m_bHavePallet = FALSE;
					m_nNextAutoStep = AUT0_CHECK_MOVE_SAFE;
					break;
				}

				m_bRequirePallet = FALSE;
				m_nNextAutoStep = AUTO_MOVE_Z_MOTOR_POS;
			}
			break;
		}

		//�ƶ�Z����������λ
	case AUTO_MOVE_Z_MOTOR_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Start Event"):_T("Z���ƶ�����λ��Ӧ��ֹͣ");

			if (::IsNoHardware()|| MotorZMoveToOnload(1))
			{
				m_nNextAutoStep = AUTO_MOTOR_Z_DOWN_TO_SAFE_DISTANCE;
			}
			else
			{
				m_nNextAutoStep = AUTO_MOTOR_Z_ABSNORMAL_DOWN_TO_DISTANCE;
			}
			break;
		}

	case AUTO_MOTOR_Z_ABSNORMAL_DOWN_TO_DISTANCE:
		{
			m_strMsg = m_bLangulageEng?_T("Motor Z Absnormal Down To Distance"):_T("Z���쳣�������½�һ�ξ���");
			if(m_nMotorZ > -1)
			{
				if (fabs(MOTORS[m_nMotorZ]->GetCurPos()-20) < GetMotorPos(GetCurPorductID(MACHINE.m_nDefaultProduct),m_nMotorZ,STACK_MOTOR_RELASE_CAR_LOC)) //������ƶ����λ����ֹײ����λ
				{
					m_nNextAutoStep = AUTO_MOVE_Z_MOTOR_POS;
					break;
				}

				double dDownPos = MOTORS[m_nMotorZ]->GetCurPos() - 50;
				if(MoveMotorPos(MOTORS[m_nMotorZ], dDownPos))
				{
					ShowMessage(_T("Z���쳣���飬�����������ϸ�Ӧû����"),3);
					m_nNextAutoStep = AUTO_MOVE_Z_MOTOR_POS;
				}
			}
			break;
		}
	case AUTO_MOTOR_Z_DOWN_TO_SAFE_DISTANCE:
		{
			m_strMsg = m_bLangulageEng?_T("Motor Z Down To Distance"):_T("Z���½�һ�ξ���");
			if(m_nMotorZ > -1)
			{
				if (fabs(MOTORS[m_nMotorZ]->GetCurPos()-20) < GetMotorPos(GetCurPorductID(MACHINE.m_nDefaultProduct),m_nMotorZ,STACK_MOTOR_RELASE_CAR_LOC)) //������ƶ����λ����ֹײ����λ
				{
					m_nNextAutoStep = AUTO_MOVE_Z_MOVE_TO_ONLOAD_POS;
					break;
				}

				double dDownPos = MOTORS[m_nMotorZ]->GetCurPos() - 20;
				if(MoveMotorPos(MOTORS[m_nMotorZ], dDownPos))
				{
					m_nNextAutoStep = AUTO_MOVE_Z_MOVE_TO_ONLOAD_POS;
				}
			}
			break;
		}
	case AUTO_MOVE_Z_MOVE_TO_ONLOAD_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Motor Z Move To Onload Pos"):_T("Z��������ȡ��λ");
			if (::IsNoHardware()|| MotorZMoveToOnload(0.5))
			{
				m_nNextAutoStep = AUTO_CAN_MOVE_SHORT_POS;
			}
			else
			{
				m_nNextAutoStep = AUTO_MOTOR_Z_ABSNORMAL_DOWN_TO_DISTANCE;
			}
			break;
		}
		//Z�������Ҫ�ƶ�һ�ξ��룬������ȡ���ź�
	case AUTO_CAN_MOVE_SHORT_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Start Event"):_T("Z�������Ҫ�ƶ�һ�ξ��룬������ȡ���ź�");
			if(!m_bSendPalletFinish)
			{
				if(m_nMotorZ > -1)
				{
					m_dMotorPos = MOTORS[m_nMotorZ]->GetCurPos() - m_dlZSafeLimit;//�½������ɲ�������
				}
				m_bSendPalletFinish = TRUE;//����ȡ���ź�
				m_nNextAutoStep = AUTO_WAIT_PICK_PALLET_FINISH;
			}
			break;
		}

		//�ȴ�ȡ�������
	case AUTO_WAIT_PICK_PALLET_FINISH:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Pick Pallet Finish"):_T("�ȴ�ȡ�������");

			if (MACHINE.m_pFeederTray != NULL)
			{
				if(MACHINE.m_pFeederTray->m_bPickPalletFinish && !m_bSendPalletFinish)
				{
					MACHINE.m_pFeederTray->m_bPickPalletFinish = FALSE;
					m_nNextAutoStep = AUTO_MOVE_Z_MOTOR_TO_SAFE_POS;
				}
			}
			break;
		}

		//�ƶ������ƫ��λ
	case AUTO_MOVE_Z_MOTOR_TO_SAFE_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Move Z Motor To Safe Pos"):_T("�ƶ������ƫ��λ");

			double ZPos = GetMotorPos(GetCurPorductID(MACHINE.m_nDefaultProduct),m_nMotorZ,STACK_MOTOR_RELASE_CAR_LOC);
			if (fabs(MOTORS[m_nMotorZ]->GetCurPos()-20) < ZPos)  ////������ƶ����λ����ֹײ����λ
			{
				m_bMotorSafePos = TRUE;
				m_nNextAutoStep = AUTO_JUDGE_STACK_EMPTY;
				break;
			}

			if (m_nMotorZ > -1 && MoveMotorPos(MOTORS[m_nMotorZ], m_dMotorPos))
			{
				m_bMotorSafePos = TRUE;
				m_nNextAutoStep = AUTO_JUDGE_STACK_EMPTY;
			}
			break;
		}

		//�ж϶�ջ����������
	case AUTO_JUDGE_STACK_EMPTY:
		{
			m_strMsg = m_bLangulageEng?_T("Judge Stack Empty"):_T("�ж϶�ջ����������");

			if(m_bMotorSafePos)
				break;

			//�Ը�Ӧ�����ж����Ƿ�ȡ��
			if (m_bDryRun || CheckHavePallet(TRUE))
			{
				m_nNextAutoStep = AUTO_WAIT_START_EVENT;
			}
			else
			{
				m_bHavePallet = FALSE;
				m_nNextAutoStep = AUT0_CHECK_MOVE_SAFE;
			}
			break;
		}

		//����ȡ��������ƶ����ȴ�λ�Ƿ�ȫ
	case AUT0_CHECK_MOVE_SAFE:
		{
			m_strMsg = m_bLangulageEng?_T("Check Move Safe"):_T("����ȡ��������ƶ����ȴ�λ�Ƿ�ȫ");

			if(CheckHaveNoCar())
			{
				m_nNextAutoStep = AUTO_MOVE_Z_MOTOR_WAIT_POS;
			}
			else
			{
				//��ջ����ȡ��󣬶�ջ���û���ƶ����ȴ�λǰ���ܽ�С����ǰ�ƽ�
				ShowMessage(MSG_ONLOAD_STACK_CHECK_CAR_STATUS_ILLEGAL);   
			}
			break;
		}

		//�ƶ�������ȴ�λ
	case AUTO_MOVE_Z_MOTOR_WAIT_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Move Z Motor Wait Pos"):_T("�ƶ�������ȴ�λ");

			if (m_nMotorZ > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorZ, STACK_MOTOR_WAIT_LOC)) 
			{
				m_nNextAutoStep = AUTO_WORK_END;
			}
			break;
		}

		//�������
	case AUTO_WORK_END:
		{
			m_strMsg = m_bLangulageEng? _T("Work end"):_T("�������");
			m_nNextAutoStep = AUTO_SAFE_CYL_OPEN;
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

		if(m_nNextAutoStep == AUTO_SAFE_CYL_OPEN)
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

BOOL CRunProcessOnloadShellStack::InitializeConfig(CString strProcessModule)
{
	if (!CRunProcess::InitializeConfig(strProcessModule))
	{
		return FALSE;
	}
	return TRUE;
}

void CRunProcessOnloadShellStack::GetHardwareConfig(CArray<int, int> *pInputs, CArray<int, int> *pOutputs, CArray<int, int> *pMotors)
{
	CRunProcess::GetHardwareConfig(pInputs, pOutputs, pMotors);
}

BOOL CRunProcessOnloadShellStack::ShowToolsDlg()
{
	return TRUE;
}

BOOL CRunProcessOnloadShellStack::CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{
#ifdef _TEST_
	return TRUE;
#endif
	ASSERT(m_nMotorZ > -1);
	if (pMotor == MOTORS[m_nMotorZ])
	{
		if(BLMessageBox(_T("Z�����ƶ�֮ǰ�������̰�ȫ������ȫ���'�ǡ�Z�����ƶ�"), MB_YESNO)==IDYES)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
		ASSERT(m_IHavePallet > -1);
		if (INPUTS[m_IHavePallet]->IsOn())  //��ջ�������ֻ�������ߣ����±���
		{
			double dPosZ = MOTORS[m_nMotorZ]->GetCurPos();
			if (bAbsMove == 1 && dPosZ > lfValue || bAbsMove == 0 && 0 > lfValue)
			{
				BLMessageBox(_T("������϶�ջ������ϣ�(M8/M9)�������Z���ŷ�������������ƶ�"), MB_ICONWARNING);
				return FALSE;
			}
		}
	}
	return TRUE;
}

BOOL CRunProcessOnloadShellStack::CheckOutputCanActive(COutput *pOutput)
{
	return TRUE;
}


/****************************************************************************************
�����������������׶���
���������bLock:TRUE:������������ FALSE:�������״�
�������أ�TRUE:ִ�гɹ���FALSE:ִ��ʧ��
*****************************************************************************************/
BOOL CRunProcessOnloadShellStack::LockCar(BOOL bLock)
{
	if (m_ILockCylPushOut < 0 || m_ILockCylBack  < 0 || m_OLockCyl < 0)
	{
		return FALSE;
	}

	if (bLock)
	{
		OUTPUTS[m_OLockCyl]->On();

		if (m_bDryRun ||(WAIT_OK == WaitInput(INPUTS[m_ILockCylPushOut], ON, MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_ILockCylBack], OFF, MSG_SHOW_DEFAULT)))
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
BOOL CRunProcessOnloadShellStack::CheckCarInPos(void)
{
	ASSERT(m_ICarInPosBehind > -1 && m_ICarInPosFront > -1);

	if (CheckInput(INPUTS[m_ICarInPosBehind], ON) 
		&& CheckInput(INPUTS[m_ICarInPosFront], ON))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CRunProcessOnloadShellStack::ModuleIsSafe()
{	
	if (m_nMotorZ > -1 && MOTORS[m_nMotorZ]->GetCurPos() > 500) 
	{
		return FALSE;
	}
	return TRUE;
}

/****************************************************************************************
��������������Ƿ��г�
���������
�������أ�TRUE:�г���FALSE:�޳�
*****************************************************************************************/
BOOL CRunProcessOnloadShellStack::CheckHaveNoCar()
{
	BOOL bRes = FALSE;
	if (m_ICarInPosBehind < 0  || m_ICarInPosFront < 0)
	{
		return bRes;
	}

	if(INPUTS[m_ICarInPosBehind]->IsOff() && INPUTS[m_ICarInPosFront]->IsOff())
	{
		Sleep(500);
		if(INPUTS[m_ICarInPosBehind]->IsOff() && INPUTS[m_ICarInPosFront]->IsOff())
		{
			bRes = TRUE;
		}
	}
	return bRes;
}

BOOL CRunProcessOnloadShellStack::MonitorOut()
{
	return TRUE;
}

/****************************************************************************************
�������������С�������̵�λ
���������bHas�� TRUE:����г�������  FALSE:���û�г�������
�������أ�TRUE:��bHasһ�£�FALSE:��bHas�෴
*****************************************************************************************/
BOOL CRunProcessOnloadShellStack::CheckCarAndPallet( BOOL bHas /*= TRUE*/ )
{
	BOOL bHasPallet = TRUE;//���̱�־

	ASSERT(/*m_IHavePallet > -1 && */m_ICarInPosBehind > -1 && m_ICarInPosFront > -1);

	if(/*m_IHavePallet < 0 || */m_ICarInPosBehind < 0 || m_ICarInPosFront < 0)
	{
		return FALSE;
	}

	if (bHas)
	{
		for(int i = 0; i < 3; i++)
		{
			if (/*INPUTS[m_IHavePallet]->IsOff() 
				|| */INPUTS[m_ICarInPosBehind]->IsOff()
				|| INPUTS[m_ICarInPosFront]->IsOff())
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
			if (/*INPUTS[m_IHavePallet]->IsOn() 
				|| */INPUTS[m_ICarInPosBehind]->IsOn()
				|| INPUTS[m_ICarInPosFront]->IsOn())
			{
				bHasPallet = FALSE;//������̣�����FALSE
				break;
			}
			Sleep(100);
		}
	}
	return bHasPallet;
}

/****************************************************************************************
��������������˶�������
���������
�������أ�TRUE:�˶���ɣ�FALSE:�˶�ʧ��
*****************************************************************************************/
BOOL CRunProcessOnloadShellStack::MotorHandMove( CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove )
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

		int iTrayInpos = -1;
		int iHasTray = -1;
		BOOL bMove = bAbsMove ? lfValue > MOTORS[m_nMotorZ]->GetCurPos() : lfValue > 0.1;
		BOOL bDownMove = bAbsMove ? lfValue < MOTORS[m_nMotorZ]->GetCurPos() : lfValue < 0.1;

		if (MACHINE.m_pFeederTray != NULL)
		{
			iHasTray = MACHINE.m_pFeederTray->m_IPalletInPosition;
			
			iTrayInpos = MACHINE.m_pFeederTray->m_ITrayInPosPresent[m_nGroupID];

			if ((m_ICarInPosBehind < 0 || INPUTS[m_ICarInPosBehind]->IsOn() || m_ICarInPosFront < 0 || INPUTS[m_ICarInPosFront]->IsOn())
				&& MOTORS[m_nMotorZ]->GetCurPos() > GetMotorPos(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorZ, STACK_MOTOR_WAIT_LOC)+1.0 && bDownMove)
			{
				BLMessageBox(_T("���̲��ڵȴ�λ����⵽��С����Z�᲻�������ƶ����뽫С��ȡ���Ƶ���ȫλ���������ƶ�"), MB_OK);
				return FALSE;
			}
		}

		if (bAbsMove)
		{
			MOTORS[m_nMotorZ]->TMoveAbs(lfValue, iLoc);
		}
		else
		{
			MOTORS[m_nMotorZ]->TMoveRel(lfValue, iLoc);
		}

		Sleep(200);
		DWORD dwStartTime = GetTickCount();

		while(1)
		{
			if ((iHasTray < 0 || INPUTS[iHasTray]->IsOn() || iTrayInpos < 0 ||INPUTS[iTrayInpos]->IsOff()) 
				&& (m_ITrayTopPresent < 0 || INPUTS[m_ITrayTopPresent]->IsOn()) && bMove)
			{
				MOTORS[m_nMotorZ]->Stop(1);	
				BLMessageBox(_T("������λ��Ӧ �����̰���-���̸�Ӧ����λ��Ӧ״̬"));
				break;
			}

			if (M_BUSY != MOTORS[m_nMotorZ]->GetMotionStatus())
			{
				MOTORS[m_nMotorZ]->Stop(1);
				break;
			}

			if (GetTickCount() - dwStartTime > 20000)
			{
				MOTORS[m_nMotorZ]->Stop(1);
				BLMessageBox(_T("Z��20����û�ƶ���Ŀ��λ�ã�������ƶ���"), MB_OK);				
				break;
			}
			Sleep(1);
		}
		return FALSE;//��ֹ�����ƶ�������ΪFALSE
	}
	return TRUE;//���������ΪTRUE����ϵͳ����
}

BOOL CRunProcessOnloadShellStack::  ReadRunParameter()
{
	CRunProcess:: ReadRunParameter();
	m_dlZSafeLimit =::IniReadDouble(m_strProcess, _T("ZSafeLimit"), 120.00, GetAbsolutePathName(PARAMETER_CFG));
	return TRUE;
}

BOOL CRunProcessOnloadShellStack::SaveParameter()
{
	//����·�������·��

	if (!IniWriteDouble(m_strProcess, _T("ZSafeLimit"), m_dlZSafeLimit, GetAbsolutePathName(PARAMETER_CFG)))
	{
		return FALSE;
	}

	CRunProcess::SaveParameter();
	return TRUE;
}
/****************************************************************************************
�����������������
���������bHas�� TRUE:���������  FALSE:���û����
�������أ�TRUE:��bHasһ�£�FALSE:��bHas�෴
*****************************************************************************************/
BOOL CRunProcessOnloadShellStack::CheckHavePallet(BOOL bHas /*= TRUE*/)
{
	BOOL bHasPallet = TRUE;//���̱�־
	if (bHas)
	{
		for(int i = 0; i < 3; i++)
		{
			if (m_IHavePallet > -1 && INPUTS[m_IHavePallet]->IsOff())
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


BOOL CRunProcessOnloadShellStack::MotorZMoveToOnload(double dSpeedRatio)
{
	BOOL bMotorZAccess = FALSE;
	
	ASSERT(0 <= dSpeedRatio && dSpeedRatio <= 1);

	if(m_IPalletMoveInPos > -1 && INPUTS[m_IPalletMoveInPos]->IsOff())
	{
		if (m_nMotorZ > -1)
		{
			MOTORS[m_nMotorZ]->m_dAloneSpeedRatio = dSpeedRatio;
			double dLocPos = GetMotorPos(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorZ,STACK_MOTOR_SEND_PALLET_LOC);
			MOTORS[m_nMotorZ]->TMoveAbs(dLocPos);
			Sleep(100);

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
					bMotorZAccess = TRUE;
					break;
				}

				if(MACHINE.m_pFeederTray != NULL && !MACHINE.m_pFeederTray->IsModuleEnable())
				{
					if(GetTickCount() - dwMotorWorkTime > 2000)
					{
						bMotorZAccess = TRUE;
						MOTORS[m_nMotorZ]->Stop(1);
						Sleep(5);
						
						break;
					}
				}

				if (IsNoHardware() || (m_IPalletMoveInPos > -1 && INPUTS[m_IPalletMoveInPos]->IsOn()))
				{
					if(dSpeedRatio != 1)
						Sleep(140);
					bMotorZAccess = TRUE;
					MOTORS[m_nMotorZ]->Stop(1);
					Sleep(5);
					break;
				}
				Sleep(2);
			}
			if(!bMotorZAccess)
			{
				MOTORS[m_nMotorZ]->Stop(1);
			}
			
		}
	}
	else
	{
		bMotorZAccess = TRUE;
	}
	
	if (bMotorZAccess)
	{
		if(WAIT_OK != CheckInput(INPUTS[m_IPalletMoveInPos], ON))
		{
			bMotorZAccess = FALSE;
		}
	}

	return bMotorZAccess;
}