/****************************************************************************************
�ļ�����CRunProcessFeederTray.cpp
�������������̰���ģ��
��ʷ�޸ļ�¼��
*****************************************************************************************/
#include "StdAfx.h"
#include "RunProcessFeederTray.h"

//��ײ��Ӧ�͹�դ���
UINT CheckMoveSafe(void* lpVoid)
{
	CRunProcessFeederTray *pFeederTray = (CRunProcessFeederTray*)lpVoid;
	if (pFeederTray == NULL)
	{
		return 0;
	}
	pFeederTray->m_bThreadFinished = FALSE;
	pFeederTray->m_bThreadStart = TRUE;

	Sleep(2000);
	while (pFeederTray->m_bThreadStart)
	{
		/*if(!IsNoHardware())
		pFeederTray->WorkWhile();*/
		Sleep(5);
	}
	pFeederTray->m_bThreadFinished = TRUE;
	return 0;
}

CRunProcessFeederTray::CRunProcessFeederTray(int nID) :
CRunProcess(nID)
{
	CString strKey;

	PowerUpRestart();

	for(int i = 0; i < 2; i++)
	{
		strKey.Format(_T("ISeparateClose%d"), i);
		m_InputMap.insert(pair<int*, CString>(&m_ISeparateClose[i], strKey));

		strKey.Format(_T("ISeparateOpen%d"), i);
		m_InputMap.insert(pair<int*, CString>(&m_ISeparateOpen[i], strKey));
	}
	for(int  i = 0; i < 6; i ++)
	{
		strKey.Format(_T("IVacuum%d"), i);
		m_InputMap.insert(pair<int*, CString>(&m_IVacuum[i], strKey));

		strKey.Format(_T("OVacuum%d"), i);
		m_OutputMap.insert(pair<int*, CString>(&m_OVacuum[i], strKey));
	}

	for(int i = 0; i < 3; i++)
	{
		strKey.Format(_T("ISafeRasterCheck%d"), i);
		m_InputMap.insert(pair<int*, CString>(&m_ISafeRasterCheck[i],strKey));

		strKey.Format(_T("ITrayInPosPresent%d"), i);
		m_InputMap.insert(pair<int*, CString>(&m_ITrayInPosPresent[i], strKey));
	}

	m_InputMap.insert(pair<int*, CString>(&m_ISafeCheckPallet, _T("ISafeCheckPallet")));
	m_InputMap.insert(pair<int*, CString>(&m_IPalletInPosition, _T("IPalletInPosition")));

	m_OutputMap.insert(pair<int*, CString>(&m_OSeparateClose, _T("OSeparateClose")));
	m_OutputMap.insert(pair<int*, CString>(&m_OSeparateOpen, _T("OSeparateOpen")));

	m_MotorMap.insert(pair<int*, CString>(&m_nMotorX, _T("MotorX")));
}

CRunProcessFeederTray::~CRunProcessFeederTray(void)
{
	Destory();
}

void CRunProcessFeederTray::PowerUpRestart()
{
	CRunProcess::PowerUpRestart();
	m_strMsg = m_bLangulageEng? _T("Ready"):_T("׼����");

	m_nPickPalletPos = -1;
	m_nMoveMotorIndex = 0;
	m_bPutPalletFinish = FALSE;
	m_bMoveMotorFinish = FALSE;
	m_bPickPalletFinish = FALSE;
	m_bOnloadPnpRequireShell = FALSE;

	m_nNextPowerUpStep = PU_CHECK_ANOTONER_MODULE_SAFE;
	m_nNextAutoStep = AUTO_JUDGE_HAVE_PALLET;
}

void CRunProcessFeederTray::PowerUpOperation(void)
{
	if (!IsModuleEnable())
	{
		CRun::PowerUpOperation();
		m_strMsg = m_bLangulageEng?_T("Moudle UnEnable"):_T("ģ�����");
		return;
	}

	if(::IsNoHardware())
	{
		Sleep(500);
	}

	switch(m_nNextPowerUpStep)
	{
		//�ж�����ģ���Ƿ�ȫ
	case PU_CHECK_ANOTONER_MODULE_SAFE:
		{
			m_strMsg = m_bLangulageEng?_T("Check Another Module Safe"):_T("�ж�����ģ���Ƿ�ȫ");

			if (CheckCanHome())
			{
				m_bPutPalletFinish = FALSE;
				m_bMoveMotorFinish = FALSE;
				m_nNextPowerUpStep = PU_MOVE_MOTOR_X_HOME;
			}
			break;
		}
		//X������λ
	case PU_MOVE_MOTOR_X_HOME:
		{
			m_strMsg = m_bLangulageEng?_T("Move Motor Y Home"):_T("Y������λ");

			if(m_nMotorX > -1 && HomeMotor(MOTORS[m_nMotorX]))
			{
				ResertHomeSingle();
				m_nNextPowerUpStep = PU_CHEACK_HAVE_PALLET;
			}
			break;
		}

		//�����������
	case PU_CHEACK_HAVE_PALLET:
		{
			m_strMsg = m_bLangulageEng?_T("Check Have Pallet, Rejust Recover"):_T("�������״̬���жϻָ�����");
			if(MACHINE.m_bReCoverMachine)
			{
				ReadStepInfo();
				if(m_IPalletInPosition > -1 && INPUTS[m_IPalletInPosition]->IsOn())	
				{
					m_nNextAutoStep = AUTO_JUDGE_ALL_COVE_PICK_FINISH;
					m_nNextPowerUpStep = PU_MOTOR_TO_ONLOAD_POS;
				}
				else
				{
					m_nPickPalletPos = -1;
					m_nMoveMotorIndex = 0;
					m_nNextAutoStep = AUTO_JUDGE_HAVE_PALLET;
					m_nNextPowerUpStep = PU_OPEN_CLAMP_CYL;
				}
			}
			else
			{
				if(::IsNoHardware() || CheckHaveNoPallet())
				{
					m_nPickPalletPos = -1;
					m_nMoveMotorIndex = 0;
					m_nNextAutoStep = AUTO_JUDGE_HAVE_PALLET;
					m_nNextPowerUpStep = PU_OPEN_CLAMP_CYL;
				}
				else
				{
					// ����ģ���⵽���̣�Ϊ��֤�豸�����������˹�����
					ShowMessage(MSG_FEEDERTRAY_CHECK_PALLET_VAILD);
				}
			}
			break;
		}

		//������մ�
	case PU_OPEN_CLAMP_CYL:
		{
			m_strMsg = m_bLangulageEng?_T("Close Clamp Cyl"):_T("������չر�");

			if(::IsNoHardware() || OpenVacuum(FALSE))
			{
				m_nNextPowerUpStep = PU_OPEN_FINGLE_CYL;
			}
			break;
		}

		//�������״�
	case  PU_OPEN_FINGLE_CYL:
		{
			m_strMsg = m_bLangulageEng?_T("Open Fingle Cyl"):_T("�������״�");

			if (IsNoHardware() || SeparateClose(FALSE))
			{
				m_nNextPowerUpStep = PU_MOTOR_TO_ONLOAD_POS;
			}
			break;
		}

		//����ƶ���������λ1
	case PU_MOTOR_TO_ONLOAD_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Motor To Onload Pos"):_T("����ƶ���������λ1");

			if(m_nMotorX > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorX, X_MOTOR_ONLOAD_LOC1))
			{
				m_nNextPowerUpStep = PU_CHECK_MOTOR_INPOS;
			}
			break;
		}

		//�����λ��
	case PU_CHECK_MOTOR_INPOS:
		{
			m_strMsg = m_bLangulageEng?_T("Check Motor Inpos"):_T("�����λ��");

			if(m_ITrayInPosPresent[0] > -1 && WAIT_OK == WaitInput(INPUTS[m_ITrayInPosPresent[0]], ON, MSG_SHOW_DEFAULT)) 
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

void CRunProcessFeederTray::AutoOperation()
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
		if (m_nSaveLastStep == AUTO_JUDGE_HAVE_PALLET)
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
		//����ջģ����������
	case AUTO_JUDGE_HAVE_PALLET:
		{
			m_strMsg = m_bLangulageEng?_T("Judge Have Pallet"):_T("����ջģ����������");

			m_bPutPalletFinish = FALSE;

			ASSERT(m_nPickPalletPos == -1);

			for (int i = 0; i < 2; i++)
			{
				if(MACHINE.m_pOnloadShellStack[i] != NULL && MACHINE.m_pOnloadShellStack[i]->m_bHavePallet)
				{
					m_nPickPalletPos = i;
					break;
				}
			}

			if(-1 < m_nPickPalletPos && m_nPickPalletPos <= 1)
			{
				m_nNextAutoStep = AUTO_MOVE_MOTOR_TO_PICK_PALLET_POS;
			}
			break;
		}

		//�ƶ������ȡ����λ
	case AUTO_MOVE_MOTOR_TO_PICK_PALLET_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Move Motor To Pick Pallet Pos"):_T("�ƶ������ȡ����λ");
			ASSERT(0 <= m_nPickPalletPos && m_nPickPalletPos <= 1);

			if(m_nMotorX > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorX, X_MOTOR_ONLOAD_LOC1 + m_nPickPalletPos))
			{
				m_bPickPalletFinish = FALSE;
				m_nNextAutoStep =AUTO_CHECK_MOTOR_MOVE_ONLOAD_INPOS;
			}
			break;
		}

		//������Ƿ��ƶ���ȡ����
	case AUTO_CHECK_MOTOR_MOVE_ONLOAD_INPOS:
		{
			m_strMsg = m_bLangulageEng?_T("Check Motor Move Onload1 Inpos"):_T("������Ƿ��ƶ���ȡ����λ");

			ASSERT(0 <= m_nPickPalletPos && m_nPickPalletPos <= 1 && m_ITrayInPosPresent[m_nPickPalletPos] > -1);

			if(IsNoHardware() || INPUTS[m_ITrayInPosPresent[m_nPickPalletPos]]->IsOn())
			{
				Sleep(100);
				if(IsNoHardware() || INPUTS[m_ITrayInPosPresent[m_nPickPalletPos]]->IsOn())
				{
					m_nNextAutoStep = AUTO_SEND_NEDD_PALLET_SINGLE_TO_ONLOAD;
					break;
				}
			}
			// ����ģ�����ƶ�����ջλ����⵽λ��Ӧ��״̬�쳣(ӦΪON)������ȷ��
			ShowMessage(MSG_FEEDERTRAY_MOTOR_MOVE_NOT_INPOS);
			break;
		}

		//�����Ϲ�λ1���������ź�
	case AUTO_SEND_NEDD_PALLET_SINGLE_TO_ONLOAD:
		{
			m_strMsg = m_bLangulageEng?_T("Send Need Pallet Single To Onload1"):_T("�����Ϲ�λ���������ź�");

			ASSERT(0 <= m_nPickPalletPos && m_nPickPalletPos <= 1);

			if(MACHINE.m_pOnloadShellStack[m_nPickPalletPos] != NULL && !MACHINE.m_pOnloadShellStack[m_nPickPalletPos]-> m_bRequirePallet)
			{
				MACHINE.m_pOnloadShellStack[m_nPickPalletPos]-> m_bRequirePallet = TRUE;
				m_nNextAutoStep = AUTO_WAIT_ONLOAD_MODULE_SEND_PALLET_FINISH;
			}
			break; 
		} 

		//�ȴ����϶�ջ1��λ�������͵�λ
	case AUTO_WAIT_ONLOAD_MODULE_SEND_PALLET_FINISH:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Onload Module1 Send Pallet Finish"):_T("�ȴ����϶�ջ��λ�������͵�λ");

			ASSERT(0 <= m_nPickPalletPos && m_nPickPalletPos <= 1);

			if(MACHINE.m_pOnloadShellStack[m_nPickPalletPos] != NULL
				&& !MACHINE.m_pOnloadShellStack[m_nPickPalletPos]-> m_bRequirePallet
				&& (MACHINE.m_pOnloadShellStack[m_nPickPalletPos]->m_bSendPalletFinish) && !m_bPickPalletFinish)
			{
				MACHINE.m_pOnloadShellStack[m_nPickPalletPos]->m_bSendPalletFinish = FALSE;
				m_nNextAutoStep = AUTO_CLAMP_CHECK_HAVE_PALLET;
			}
			break;
		}

		//����������޵�λ
	case AUTO_CLAMP_CHECK_HAVE_PALLET:
		{
			m_strMsg = m_bLangulageEng?_T("Clamp Check Have Pallet"):_T("����������޵�λ");

			if(::IsNoHardware() || m_bDryRun || !CheckHaveNoPallet())
			{
				m_nNextAutoStep = AUTO_CLMAP_CYL_CLAMP;
			}
			else
			{
				// ����ģ��������Ƕ�ջ����ʱ����ⲻ�����̣����˹���Ԥ����
				ShowMessage(MSG_FEEDERTRAY_CHECK_PALLET_INVAILD);
			}
			break;
		}
		//��մ�
	case AUTO_CLMAP_CYL_CLAMP:
		{
			m_strMsg = m_bLangulageEng?_T("Vacuum Open"):_T("��մ�");

			//if((::IsNoHardware() || OpenVacuum(TRUE)) && !m_bPickPalletFinish)  ������ʱ���Ρ���2019��6��21��0:43:46
			{
				m_nNextAutoStep = AUTO_FINGLE_CYL_CLAMP;
			}
			break;
		}
	
		//�������׼н�
	case AUTO_FINGLE_CYL_CLAMP:
		{
			m_strMsg = m_bLangulageEng?_T("Fingle Cyl Clamp"):_T("�������׼н�����");

			if (IsNoHardware() || SeparateClose(TRUE))
			{
				m_bPickPalletFinish = TRUE;
				m_nNextAutoStep = AUTO_CHECK_HAVE_PALLET_AGANIN;
			}
			break;
		}
		//�ٴ�ȷ����������
	case AUTO_CHECK_HAVE_PALLET_AGANIN:
		{
			m_strMsg = m_bLangulageEng?_T("Check Have Pallet Again"):_T("�ٴ�ȷ����������");

			if(::IsNoHardware() || m_bDryRun || !CheckHaveNoPallet()) 
			{
				m_nNextAutoStep = AUTO_WAIT_ONLOAD_MODULE_MOVE_SAFE_POS;
			}
			else
			{
				// ����ģ��������Ƕ�ջ����ʱ����ⲻ�����̣����˹���Ԥ����
				ShowMessage(MSG_FEEDERTRAY_CHECK_PALLET_INVAILD);
			}
			break;
		}

		//�ȴ��������ƶ�����ȫλ��
	case AUTO_WAIT_ONLOAD_MODULE_MOVE_SAFE_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Onload Moudle Move Safe Pos"):_T("�ȴ��������϶�ջ����ƶ�����ȫλ��");

			ASSERT(0 <= m_nPickPalletPos && m_nPickPalletPos <= 1);

			if(!m_bPickPalletFinish && MACHINE.m_pOnloadShellStack[m_nPickPalletPos] != NULL && MACHINE.m_pOnloadShellStack[m_nPickPalletPos]->m_bMotorSafePos)
			{
				MACHINE.m_pOnloadShellStack[m_nPickPalletPos]->m_bMotorSafePos = FALSE;
				m_nPickPalletPos = -1;
				m_nNextAutoStep = AUTO_MOVE_MOTOR_TO_START_FEED_COVER_POS;
				break;
			}
			break;
		}

		//������ƶ�����ʼ�Ͽ�λ
	case AUTO_MOVE_MOTOR_TO_START_FEED_COVER_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Move Motor To Start Feed Cover Pos"):_T("������ƶ�����ʼ�Ͽ�λ");

			if(m_nMotorX > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorX, X_PICK_COVER_1))
			{
				m_nMoveMotorIndex = 0;
				m_nNextAutoStep = AUTO_WAIT_START_EVENT;
			}
			break;
		}

		//�ȴ���ʼ�����ź�
	case AUTO_WAIT_START_EVENT:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Start Event"):_T("�ȴ���ʼ�����ź�");
			//�Ͽǻ�е�ַ���ȡ���ź�
			if(m_bOnloadPnpRequireShell && !m_bMoveMotorFinish)
			{
				m_bOnloadPnpRequireShell = FALSE;
				m_nNextAutoStep = AUTO_MOVE_MOTOR_TO_FEED_COVER_POS;
			}
			break;
		}

		//�ƶ������ȡ��λ
	case AUTO_MOVE_MOTOR_TO_FEED_COVER_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Move Motor To Feed Cover Pos"):_T("�ƶ������ȡ��λ");

			int nLoc = X_PICK_COVER_1 + m_nMoveMotorIndex;
			ASSERT(X_PICK_COVER_1 <= nLoc && nLoc <= X_PICK_COVER_6);
			if(m_nMotorX > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorX, nLoc))
			{
				if(MACHINE.m_pOnloadShellPnp != NULL && !MACHINE.m_pOnloadShellPnp->m_bPickCoverFinish)
				{
					m_bMoveMotorFinish = TRUE;
					m_nNextAutoStep = AUTO_WAIT_PICK_COVER_FINISH;
				}
			}
			break;
		}

		//�ȴ�ץ��ץ�����
	case AUTO_WAIT_PICK_COVER_FINISH:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Pick Cover Finish"):_T("�ȴ�ץ��ץ�����");

			if(MACHINE.m_pOnloadShellPnp != NULL && MACHINE.m_pOnloadShellPnp->m_bPickCoverFinish && !m_bMoveMotorFinish)
			{
				m_nMoveMotorIndex++;
				MACHINE.m_pOnloadShellPnp->m_bPickCoverFinish = FALSE;
				m_nNextAutoStep = AUTO_JUDGE_ALL_COVE_PICK_FINISH;
				SaveStepInfo(WRITE_TYPE_VARIABLES);
			}			
			break;
		}

		//�ж����õĿ�����ץ��
	case AUTO_JUDGE_ALL_COVE_PICK_FINISH:
		{ 
			m_strMsg = m_bLangulageEng?_T("Judge All Cover Pick Finish"):_T("�ж�һ��������ץ��");

			if(m_nMoveMotorIndex < 6)
			{
				m_nNextAutoStep = AUTO_WAIT_START_EVENT;
			}
			else
			{
				m_nMoveMotorIndex = 0;
				m_bMoveMotorFinish = FALSE;
				m_bOnloadPnpRequireShell = FALSE;
				m_nNextAutoStep = AUTO_MOVE_PALLET_TO_OFFLAOD_POS;
				SaveStepInfo(WRITE_TYPE_VARIABLES);
			}
			break;
		}

		//�ƶ����̵�����λ
	case AUTO_MOVE_PALLET_TO_OFFLAOD_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Move Pallet To Offload Pos"):_T("�ƶ����̵�����λ");

			if(m_nMotorX > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorX, X_MOTOR_OFFLOAD_LOC))
			{
				m_nNextAutoStep = AUTO_CHECK_MOTOR_MOVE_OFFLOAD_INPOS;
			}
			break;
		}

		//������Ƿ��ƶ���ȡ����2
	case AUTO_CHECK_MOTOR_MOVE_OFFLOAD_INPOS:
		{
			m_strMsg = m_bLangulageEng?_T("Check Motor Move Onload1 Inpos"):_T("������Ƿ��ƶ����¿����̶�ջλ");

			ASSERT(m_ITrayInPosPresent[2] > -1);

			if(IsNoHardware() || CheckInput(INPUTS[m_ITrayInPosPresent[2]],ON))
			{
				Sleep(100);
				if(IsNoHardware() || CheckInput(INPUTS[m_ITrayInPosPresent[2]],ON))
				{
					m_nNextAutoStep = AUTO_FINGLE_CYL_CLAMP_CLOSE;
					break;
				}
			}
			// ����ģ�����ƶ�����ջλ����⵽λ��Ӧ��״̬�쳣(ӦΪON)������ȷ��
			ShowMessage(MSG_FEEDERTRAY_MOTOR_MOVE_NOT_INPOS);

			break;
		}
		//�����ϴ����������ɿ�
	case AUTO_FINGLE_CYL_CLAMP_CLOSE:
		{
			m_strMsg = m_bLangulageEng?_T("Fingle Cyl Clamp"):_T("�¿����̶�ջλ�����������ɿ�");

			if (IsNoHardware() || SeparateClose(FALSE))
			{
				m_nNextAutoStep = AUTO_SEND_OFFLAD_MODUEL_PICK_PALLET;
			}
			break;
		}
		//������ģ�鷢�ͽ����ź�
	case AUTO_SEND_OFFLAD_MODUEL_PICK_PALLET:
		{
			m_strMsg = m_bLangulageEng?_T("Send Offload Module Pick Pallet"):_T("������ģ�鷢�ͽ����ź�");

			if(MACHINE.m_pOffloadShellStack != NULL && !MACHINE.m_pOffloadShellStack->m_bOffloadEmptyPallet)
			{
				MACHINE.m_pOffloadShellStack-> m_bOffloadEmptyPallet = TRUE;
				m_nNextAutoStep = AUTO_WAIT_OFFLOAD_MODULE_LIFT_FINISH;
			}
			break;
		}
		//�ȴ�����ģ���ƶ�������λ
	case AUTO_WAIT_OFFLOAD_MODULE_LIFT_FINISH:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Offlaod Moduel Lift Finish"):_T("�ȴ�����ģ���ƶ�������λ");

			if(MACHINE.m_pOffloadShellStack != NULL && !MACHINE.m_pOffloadShellStack->m_bOffloadEmptyPallet)
			{
				m_nNextAutoStep = AUTO_CLMAP_CYL_OPEN_OFFLOAD;
			}
			break;
		}

		//�����ϴ���մ�
	case AUTO_CLMAP_CYL_OPEN_OFFLOAD:
		{
			m_strMsg = m_bLangulageEng?_T("Clamp Cyl Open Offload"):_T("�����ϴ���մ�");

			if(::IsNoHardware() || OpenVacuum(FALSE))
			{
				Sleep(300);
				if (IsNoHardware() ||
					(INPUTS[m_IVacuum[0]]->IsOff() && INPUTS[m_IVacuum[1]]->IsOff() 
					&&INPUTS[m_IVacuum[2]]->IsOff() && INPUTS[m_IVacuum[3]]->IsOff()
					&&INPUTS[m_IVacuum[4]]->IsOff() && INPUTS[m_IVacuum[5]]->IsOff()))
				{
					m_bPutPalletFinish = TRUE;
					m_nNextAutoStep = AUTO_WAIT_OFFLOAD_MOVE_SAFE;	
				}
				else
				{
					ShowMessage(_T("����ģ������û������գ�����"),3);
				}
			}
			break;
		}

		//�ȴ�����ģ�����ƶ�����ȫλ
	case AUTO_WAIT_OFFLOAD_MOVE_SAFE:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Offload Move Safe"):_T("�ȴ�����ģ�����ƶ�����ȫλ");

			if (!m_bPutPalletFinish)
			{
				m_nNextAutoStep = AUTO_CHECK_PALLET_REMOVE;
			}
			break;
		}
	case AUTO_CHECK_PALLET_REMOVE:
		{
			m_strMsg = m_bLangulageEng?_T("Check Pallet Remove"):_T("������̱�ȡ��");

			if(::IsNoHardware() || m_bDryRun || (CheckHaveNoPallet()
				&& m_IVacuum[0] > -1 && CheckInput(INPUTS[m_IVacuum[0]], OFF)
				&& m_IVacuum[1] > -1 && CheckInput(INPUTS[m_IVacuum[1]], OFF)
				&&  m_IVacuum[2] > -1 && CheckInput(INPUTS[m_IVacuum[2]], OFF)
				&&  m_IVacuum[3] > -1 && CheckInput(INPUTS[m_IVacuum[3]], OFF)
				&& m_IVacuum[4] > -1 && CheckInput(INPUTS[m_IVacuum[4]], OFF)
				&&  m_IVacuum[5] > -1 && CheckInput(INPUTS[m_IVacuum[5]], OFF)))
			{
				m_nNextAutoStep = AUTO_WORK_END;
			}
			break;
		}
		//�������
	case AUTO_WORK_END:
		{
			m_strMsg = m_bLangulageEng?_T("Work end"):_T("�������");
			m_nNextAutoStep = AUTO_JUDGE_HAVE_PALLET;
			break;
		}
	default:
		{
			ASSERT(0);
			break;
		}
	}
}

BOOL CRunProcessFeederTray::InitializeConfig(CString strProcessModule)
{
	if (!CRunProcess::InitializeConfig(strProcessModule))
	{
		return FALSE;
	}
	AfxBeginThread(CheckMoveSafe, this);
	return TRUE;
}

void CRunProcessFeederTray::GetHardwareConfig(CArray<int, int> *pInputs, CArray<int, int> *pOutputs, CArray<int, int> *pMotors)
{
	CRunProcess::GetHardwareConfig(pInputs, pOutputs, pMotors);
}

BOOL CRunProcessFeederTray::CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{
#ifdef _TEST_
	return TRUE;
#endif
	CString strKey;
	if (m_nMotorX > -1 && pMotor == MOTORS[m_nMotorX])
	{
		if(MACHINE.m_pOnloadShellPnp != NULL)
		{
			if(!MACHINE.m_pOnloadShellPnp->CheckCanMoveMotor())
			{
				return FALSE;
			}
		}

		for (int i = 0; i < 2; i++)
		{
			if(MACHINE.m_pOnloadShellStack[i] != NULL && !(MACHINE.m_pOnloadShellStack[i])->ModuleIsSafe())
			{
				strKey.Format(_T("(M0%d)�������%dZ���ŷ����,δ�ڰ�ȫλ����"),8+i, 1+i);
				BLMessageBox(strKey, MB_ICONWARNING);
				return FALSE; 
			}
		}

		if(MACHINE.m_pOffloadShellStack != NULL && !MACHINE.m_pOffloadShellStack->ModuleIsSafe())
		{
			BLMessageBox(_T("(M10)����������Z���ŷ����δ�ڰ�ȫλ����"), MB_ICONWARNING);
			return FALSE; 

		}
	}
	return TRUE;
}

BOOL CRunProcessFeederTray::CheckOutputCanActive(COutput *pOutput)
{
#ifdef _TEST_
	return TRUE;
#endif
	if (m_OSeparateOpen < 0 || pOutput == OUTPUTS[m_OSeparateOpen])
	{
		if (m_IPalletInPosition < 0 || INPUTS[m_IPalletInPosition]->IsOn())
		{
			if (BLMessageBox(_T("X0407��λ�����̸�Ӧ���ϣ��Ƿ��ɿ���YES ���ɿ���NO�����ɿ�"),MB_YESNO) == IDNO)
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}

/****************************************************************************************
�����������������׶���
���������bClose:TRUE:�������׼н� FALSE:�������״�
�������أ�TRUE:ִ�гɹ���FALSE:ִ��ʧ��
*****************************************************************************************/
BOOL CRunProcessFeederTray::SeparateClose( BOOL bClose /*= TRUE*/ )
{
	ASSERT(m_ISeparateClose[0] > -1 && m_ISeparateClose[1] > -1 && m_ISeparateOpen[0] > -1
		&& m_ISeparateOpen[1] > -1 && m_OSeparateOpen > -1 && m_OSeparateClose > -1);

	BOOL bRes = FALSE;

	if(bClose)
	{
		OUTPUTS[m_OSeparateOpen]->Off();
		OUTPUTS[m_OSeparateClose]->On();

		if(WAIT_OK == WaitInput(INPUTS[m_ISeparateClose[0]], ON, MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_ISeparateOpen[0]], OFF, MSG_SHOW_DEFAULT)
			&& WAIT_OK == WaitInput(INPUTS[m_ISeparateClose[1]], ON, MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_ISeparateOpen[1]], OFF, MSG_SHOW_DEFAULT))
		{
			bRes = TRUE;
		}
	}
	else
	{
		OUTPUTS[m_OSeparateClose]->Off();
		OUTPUTS[m_OSeparateOpen]->On();

		if(WAIT_OK == WaitInput(INPUTS[m_ISeparateOpen[0]], ON, MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_ISeparateClose[0]], OFF, MSG_SHOW_DEFAULT)
			&& WAIT_OK == WaitInput(INPUTS[m_ISeparateOpen[1]], ON, MSG_SHOW_DEFAULT)
			&& WAIT_OK == WaitInput(INPUTS[m_ISeparateClose[1]], OFF, MSG_SHOW_DEFAULT)) 
		{
			bRes = TRUE;
		}
	}
	return bRes;
}

/****************************************************************************************
�����������ж�û������
���������
�������أ�TRUE:ִ�гɹ���FALSE:ִ��ʧ��
*****************************************************************************************/
BOOL CRunProcessFeederTray::CheckHaveNoPallet()
{	
	ASSERT(m_IPalletInPosition > -1);

	BOOL bCheckRes = TRUE;

	for(int i = 0; i < 3; i++)
	{
		if(INPUTS[m_IPalletInPosition]->IsOn())
		{
			bCheckRes = FALSE;
			break;
		}
		Sleep(100);
	}

	return bCheckRes;
}

/****************************************************************************************
�����������ж�����ģ���Ƿ�ȫ
���������
�������أ�TRUE:��ȫ��FALSE:����ȫ
*****************************************************************************************/
BOOL CRunProcessFeederTray:: CheckCanHome(void)
{
	BOOL bTrue[4] = {FALSE, FALSE, FALSE, FALSE};

	if(MACHINE.m_pOnloadShellStack[0] != NULL && (MACHINE.m_pOnloadShellStack[0]->m_bHomeMoveSafePos || !MACHINE.m_pOnloadShellStack[0]->IsModuleEnable()))
	{// ���Ƕ�ջ1
		bTrue[0] = TRUE;
	}

	if(MACHINE.m_pOnloadShellStack[1] != NULL && (MACHINE.m_pOnloadShellStack[1]->m_bHomeMoveSafePos || !MACHINE.m_pOnloadShellStack[1]->IsModuleEnable()))
	{// ���Ƕ�ջ2
		bTrue[1] = TRUE;
	}

	if(MACHINE.m_pOffloadShellStack != NULL && (MACHINE.m_pOffloadShellStack->m_bHomeMoveSafePos || !MACHINE.m_pOffloadShellStack->IsModuleEnable()))
	{// �������϶�ջ
		bTrue[2] = TRUE;
	}

	if(MACHINE.m_pOnloadShellPnp != NULL && (MACHINE.m_pOnloadShellPnp->m_bHomeMoveSafePos || !MACHINE.m_pOnloadShellPnp->IsModuleEnable()))
	{//ȡ����
		bTrue[3] = TRUE;
	}

	return (bTrue[0] && bTrue[1] && bTrue[2]  && bTrue[3]);
}

/****************************************************************************************
�������������ø�λ�ź�
���������
�������أ�
*****************************************************************************************/
void CRunProcessFeederTray:: ResertHomeSingle(void)
{
	if(MACHINE.m_pOnloadShellStack[0] != NULL)
	{
		MACHINE.m_pOnloadShellStack[0]->m_bHomeMoveSafePos = FALSE;
	}

	if(MACHINE.m_pOnloadShellStack[1] != NULL)
	{
		MACHINE.m_pOnloadShellStack[1]->m_bHomeMoveSafePos = FALSE;
	}

	if(MACHINE.m_pOffloadShellStack != NULL)
	{
		MACHINE.m_pOffloadShellStack->m_bHomeMoveSafePos = FALSE;
	}

	if(MACHINE.m_pOnloadShellPnp != NULL)
	{
		MACHINE.m_pOnloadShellPnp->m_bHomeMoveSafePos = FALSE;
	}
}

/****************************************************************************************
���������������
���������bOpen:TRUE:����� FALSE:�ر����
�������أ�TRUE:ִ�гɹ���FALSE:ִ��ʧ��
*****************************************************************************************/
BOOL CRunProcessFeederTray::OpenVacuum(BOOL bOpen)
{
	int nVacuumCount = 6;
	for(int i = 0; i < nVacuumCount; i ++)
	{
		ASSERT(m_IVacuum[i] > -1  && m_OVacuum[i] > -1);
	}
	

	BOOL bRes = FALSE;
	if(bOpen)
	{
		for(int i =0; i < nVacuumCount; i ++)
		{
			OUTPUTS[m_OVacuum[i]]->On();
		}
		
		if (m_bDryRun)
		{
			Sleep(500);
		}

		if (m_bDryRun
			|| (WAIT_OK == WaitInput(INPUTS[m_IVacuum[0]], ON, MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_IVacuum[1]], ON, MSG_SHOW_DEFAULT)
			&& WAIT_OK == WaitInput(INPUTS[m_IVacuum[2]], ON, MSG_SHOW_DEFAULT)
			&& WAIT_OK == WaitInput(INPUTS[m_IVacuum[3]], ON, MSG_SHOW_DEFAULT)
			&& WAIT_OK == WaitInput(INPUTS[m_IVacuum[4]], ON, MSG_SHOW_DEFAULT)
			&& WAIT_OK == WaitInput(INPUTS[m_IVacuum[5]], ON, MSG_SHOW_DEFAULT)))
		{
			bRes = TRUE;
		}
	}
	else
	{
		for(int i =0; i < nVacuumCount; i ++)
		{
			OUTPUTS[m_OVacuum[i]]->Off();
		}
		

		if (WAIT_OK == WaitInput(INPUTS[m_IVacuum[0]], OFF, MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_IVacuum[1]], OFF, MSG_SHOW_DEFAULT)
			&& WAIT_OK == WaitInput(INPUTS[m_IVacuum[2]], OFF, MSG_SHOW_DEFAULT)
			&& WAIT_OK == WaitInput(INPUTS[m_IVacuum[3]], OFF, MSG_SHOW_DEFAULT)
			&& WAIT_OK == WaitInput(INPUTS[m_IVacuum[4]], OFF, MSG_SHOW_DEFAULT)
			&& WAIT_OK == WaitInput(INPUTS[m_IVacuum[5]], OFF, MSG_SHOW_DEFAULT))
		{
			bRes = TRUE;
		}
	}

	return bRes;
}

/****************************************************************************************
������������������ʱһֱ����Ƿ�ȫ
���������
�������أ�
*****************************************************************************************/
void CRunProcessFeederTray::WorkWhile()
{
	if(MACHINE.m_pRunCtrl == NULL)
	{
		return;
	}
	if (IsNoHardware())
	{
		return;
	}
	ASSERT(m_ISafeRasterCheck[0] > -1 && m_ISafeRasterCheck[1] > -1&& m_ISafeRasterCheck[2] > -1 && m_ISafeCheckPallet > -1);
	int nStatues = MACHINE.m_pRunCtrl->GetMcState();
	if( !MACHINE.m_bSafeStatckDoor[0] && !MACHINE.m_bSafeStatckDoor[1]  && !MACHINE.m_bSafeStatckDoor[2] && (INPUTS[m_ISafeRasterCheck[0]]->IsOff() || INPUTS[m_ISafeRasterCheck[1]]->IsOff()|| INPUTS[m_ISafeRasterCheck[2]]->IsOff())
		&& (nStatues == MC_RUNNING || nStatues == MC_INITIALIZING))
		//��դ��Ӧ���źź���,������ʱ�����
	{
		if (m_nMotorX > -1)
		{
			MOTORS[m_nMotorX] -> Stop(1);
		}

		if(MACHINE.m_pOnloadShellStack[0] !=NULL && MACHINE.m_pOnloadShellStack[0]->m_nMotorZ > -1)
		{
			MOTORS[MACHINE.m_pOnloadShellStack[0]->m_nMotorZ]->Stop(1);
		}

		if(MACHINE.m_pOnloadShellStack[1] !=NULL && MACHINE.m_pOnloadShellStack[1]->m_nMotorZ > -1)
		{
			MOTORS[MACHINE.m_pOnloadShellStack[1]->m_nMotorZ]->Stop(1);
		}

		if(MACHINE.m_pOffloadShellStack !=NULL && MACHINE.m_pOffloadShellStack->m_nMotorZ > -1)
		{
			MOTORS[MACHINE.m_pOffloadShellStack->m_nMotorZ]->Stop(1);
		}

		if(MACHINE.m_pOnloadShellPnp != NULL && MACHINE.m_pOnloadShellPnp->m_nMotorZ > -1)
		{
			MOTORS[MACHINE.m_pOnloadShellPnp->m_nMotorZ]->Stop(1);
		}

		if(MACHINE.m_pOnloadShellPnp != NULL && MACHINE.m_pOnloadShellPnp->m_nMotorY > -1)
		{
			MOTORS[MACHINE.m_pOnloadShellPnp->m_nMotorY]->Stop(1);
		}
		//���Ƕ�ջģ���ⰲȫ��դ����������ע�ⰲȫ
		ShowMessage(MSG_FEEDERTRAY_SAFE_RASTER_CHECK_UNNOTMAL);
	}

	if(INPUTS[m_ISafeCheckPallet]->IsOff())
	{
		Sleep(50);
		if(INPUTS[m_ISafeCheckPallet]->IsOff())
		{
			if (m_nMotorX > -1)
			{
				MOTORS[m_nMotorX] -> Stop();
			}

			if(MACHINE.m_pOnloadShellStack[0] !=NULL && MACHINE.m_pOnloadShellStack[0]->m_nMotorZ > -1)
			{
				MOTORS[MACHINE.m_pOnloadShellStack[0]->m_nMotorZ]->Stop(1);
			}

			if(MACHINE.m_pOnloadShellStack[1] !=NULL && MACHINE.m_pOnloadShellStack[1]->m_nMotorZ > -1)
			{
				MOTORS[MACHINE.m_pOnloadShellStack[1]->m_nMotorZ]->Stop(1);
			}

			if(MACHINE.m_pOffloadShellStack !=NULL && MACHINE.m_pOffloadShellStack->m_nMotorZ > -1)
			{
				MOTORS[MACHINE.m_pOffloadShellStack->m_nMotorZ]->Stop(1);
			}

			if(MACHINE.m_pOnloadShellPnp != NULL && MACHINE.m_pOnloadShellPnp->m_nMotorZ > -1)
			{
				MOTORS[MACHINE.m_pOnloadShellPnp->m_nMotorZ]->Stop(1);
			}

			if(MACHINE.m_pOnloadShellPnp != NULL && MACHINE.m_pOnloadShellPnp->m_nMotorY > -1)
			{
				MOTORS[MACHINE.m_pOnloadShellPnp->m_nMotorY]->Stop(1);
			}

			MACHINE.m_pRunCtrl->Stop();

			if(BLMessageBox(_T("��ײ��Ӧ�����ˣ���ȷ���Ƿ�ײ��"),MB_YESNO|MB_ICONWARNING) == IDYES)
			{
				if (MOTORS[m_nMotorX]->GetCurLoc() == X_MOTOR_ONLOAD_LOC1)
				{// ȷ��ײ���Ժ��ջ�����λ
					if(MACHINE.m_pOnloadShellStack[0] != NULL && MACHINE.m_pOnloadShellStack[0]->m_nMotorZ > -1)
					{
						HomeMotor(MOTORS[MACHINE.m_pOnloadShellStack[0]->m_nMotorZ]);
					}
				}

				if (MOTORS[m_nMotorX]->GetCurLoc() == X_MOTOR_ONLOAD_LOC2)
				{// ȷ��ײ���Ժ��ջ�����λ
					if(MACHINE.m_pOnloadShellStack[1] != NULL && MACHINE.m_pOnloadShellStack[1]->m_nMotorZ > -1)
					{
						HomeMotor(MOTORS[MACHINE.m_pOnloadShellStack[1]->m_nMotorZ]);
					}
				}

				if (MOTORS[m_nMotorX]->GetCurLoc() == X_MOTOR_OFFLOAD_LOC)
				{
					if(MACHINE.m_pOffloadShellStack !=NULL && MACHINE.m_pOffloadShellStack->m_nMotorZ > -1)
					{
						HomeMotor(MOTORS[MACHINE.m_pOffloadShellStack->m_nMotorZ]);
					}
				}
			}
		}
	}
}

void CRunProcessFeederTray::Destory()
{
	m_bThreadStart = FALSE;

	DWORD dwTime = GetTickCount();

	while(!m_bThreadFinished)
	{
		if (GetTickCount() - dwTime > 20000)
		{
			break;
		}
		Sleep(1);
	}
}

/****************************************************************************************
�����������ж����̰���ģ��״̬
���������
�������أ�
*****************************************************************************************/
BOOL CRunProcessFeederTray:: CheckPalletInPos()
{
	if(m_IPalletInPosition  < 0 )
	{
		return FALSE;
	}
	BOOL m_bCheckCore = FALSE;
	DWORD m_WordTime = GetTickCount();
	for (;;)
	{
		if (GetTickCount() - m_WordTime > 15000)
		{
			BLMessageBox(_T("���϶�ջ���̴��ͳ�ʱ��"));
			break;
		}

		if(INPUTS[m_IPalletInPosition]->IsOn())//��ʾ������
		{
			m_bCheckCore = TRUE;
			break;
		}

	}
	return m_bCheckCore;
}

//��ȡ������������
void CRunProcessFeederTray::ReadStepInfo()
{
	CString strPath = GetAbsolutePathName(MODULE_RUN_FOLDER) + m_strName + _T(".cfg");

	m_nMoveMotorIndex = IniReadInt(_T("Data"), _T("m_nMoveMotorIndex"), 0, strPath);

	return ;
}

//���������������
BOOL CRunProcessFeederTray::SaveStepInfo(int nWriteType)
{
	CString strPath = GetAbsolutePathName(MODULE_RUN_FOLDER) + m_strName + _T(".cfg");

	IniWriteInt(_T("Data"), _T("m_nMoveMotorIndex"), m_nMoveMotorIndex, strPath);
	return TRUE;
}