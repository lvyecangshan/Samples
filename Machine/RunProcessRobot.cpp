/****************************************************************************************
�ļ�����RunProcessRobot.cpp
�������������������Ϲ�λ
��ʷ�޸ļ�¼��
*****************************************************************************************/
#include "StdAfx.h"
#include "RunProcessRobot.h"

UINT RobotThread(LPVOID	 lpvoid)
{
	CRunProcessRobot* pRun = (CRunProcessRobot*)lpvoid;

	if (pRun != NULL)
	{
		pRun->m_bThreadStart = TRUE;
		pRun->m_bThreadEnd = FALSE;
		while(pRun->m_bThreadStart)
		{
			if(!IsNoHardware() && pRun->IsModuleEnable())
				pRun->EmgStop();
			Sleep(100);
		}
		pRun->m_bThreadEnd = TRUE;
	}
	return 0;
}


CRunProcessRobot::CRunProcessRobot(int nID) : CRunProcess(nID)
{
	CString strKey;

	PowerUpRestart();

	for(int i = 0; i < 2; i++)
	{
		//��צ
		strKey.Format(_T("IFingerCylOpen%d"), i);
		m_InputMap.insert(pair<int*, CString>(&m_IFingerCylOpen[i], strKey));

		strKey.Format(_T("IFingerCylClose%d"), i);
		m_InputMap.insert(pair<int*, CString>(&m_IFingerCylClose[i], strKey));

		strKey.Format(_T("OFingerCylClose%d"), i);
		m_OutputMap.insert(pair<int*, CString>(&m_OFingerCylClose[i], strKey));

		strKey.Format(_T("OFingerCylOpen%d"), i);
		m_OutputMap.insert(pair<int*, CString>(&m_OFingerCylOpen[i], strKey));

		strKey.Format(_T("IHasCore%d"), i);//��צ���ϼ��
		m_InputMap.insert(pair<int*, CString>(&m_IHasCore[i], strKey));

		strKey.Format(_T("IHasCore%d"), i + 2);//�������ϼ��
		m_InputMap.insert(pair<int*, CString>(&m_IHasCore[i + 2], strKey));

		strKey.Format(_T("ITailFingerSignal%d"), i);
		m_InputMap.insert(pair<int*, CString>(&m_ITailFingerSignal[i], strKey));

		strKey.Format(_T("ITailSafeSignal%d"), i);
		m_InputMap.insert(pair<int*, CString>(&m_ITailSafeSignal[i], strKey));

		strKey.Format(_T("ORobotPutFinishSignal%d"), i);
		m_OutputMap.insert(pair<int*, CString>(&m_ORobotPutFinishSignal[i], strKey));

		strKey.Format(_T("ORobotPutInPosSignal%d"), i);
		m_OutputMap.insert(pair<int*, CString>(& m_ORobotPutInPosSignal[i], strKey));
	}
	m_InputMap.insert(pair<int*, CString>(&m_IZCheckAbnormal, _T("IZCheckAbnormal")));
	//����
	m_InputMap.insert(pair<int*, CString>(&m_IBufferCylPush,_T("IBufferCylPush")));
	m_InputMap.insert(pair<int*, CString>(&m_IBufferCylReturn,_T("IBufferCylReturn")));
	m_OutputMap.insert(pair<int*, CString>(&m_OBufferCylPush,_T("OBufferCylPush")));
	m_OutputMap.insert(pair<int*, CString>(&m_OBufferCylReturn, _T("OBufferCylReturn")));
	//������
	m_InputMap.insert(pair<int*, CString>(&m_IRobotAlarm, _T("IRobotAlarm")));
	m_InputMap.insert(pair<int*, CString>(&m_IRobotRun, _T("IRobotRun")));
	m_InputMap.insert(pair<int*, CString>(&m_IRobotReset, _T("IRobotReset")));
	m_InputMap.insert(pair<int*, CString>(&m_IRobotEnable, _T("IRobotEnable")));

	m_OutputMap.insert(pair<int*, CString>(&m_ORobotEnable, _T("ORobotEnable")));
	m_OutputMap.insert(pair<int*, CString>(&m_ORobotStop, _T("ORobotStop")));
	m_OutputMap.insert(pair<int*, CString>(&m_ORobotCodeRun, _T("ORobotCodeRun")));
	m_OutputMap.insert(pair<int*, CString>(&m_ORobotAlarmReset, _T("ORobotAlarmReset")));
	m_OutputMap.insert(pair<int*, CString>(&m_ORobotCodeReset, _T("ORobotCodeReset")));
	m_OutputMap.insert(pair<int*, CString>(&m_ORobotEmgStop, _T("ORobotEmgStop")));

	m_InputMap.insert(pair<int*, CString>(&m_INormalRunSingle, _T("INormalRunSingle")));
	m_OutputMap.insert(pair<int*, CString>(&m_ONormalRunSingle, _T("ONormalRunSingle")));

	m_VoidMap.insert(pair<int*, CString>(&m_bEnableRobot, _T("bEnableRobot, ����������")));
	m_VoidMap.insert(pair<void*,CString>(&m_nRobotPort,_T("lRobotPort, ���������Ӷ˿�")));
	m_VoidMap.insert(pair<void*,CString>(&m_strRobotIp,_T("sRobotIp, ����������IP")));

	m_VoidMap.insert(pair<int*, CString>(&m_bShieldServer, _T("b���ÿͻ���, TrueΪ���ã�FalseΪ������")));
	m_VoidMap.insert(pair<void*,CString>(&m_nServerPort,_T("l�������˿�, ���Ӷ˿�")));
	m_VoidMap.insert(pair<void*,CString>(&m_strServerIp,_T("s��������IP, ����IP")));
}

CRunProcessRobot::~CRunProcessRobot(void)
{
	MACHINE.MachineExit = TRUE;
	m_bThreadStart = FALSE;
	DWORD dwTime = GetTickCount();

	while(!m_bThreadEnd)
	{
		if (GetTickCount() - dwTime > 10000)
		{
			break;
		}
		Sleep(1);
	}
	m_Client.Disconnect();

}

void CRunProcessRobot::PowerUpRestart()
{
	CRunProcess::PowerUpRestart();
	m_strMsg = m_bLangulageEng?_T("Ready"):_T("׼����");

	m_nCurRobotLoc = ROBOT_GO_HOME;
	m_unRobotSpeed = ROBOT_LOW_SPEED;
	m_nWorkIndex = -1;
	m_nStartWithPalletData = -1;
	m_bRobotWaitPos = FALSE;
	m_bRobotSafe = FALSE;
	m_bBufferMes = FALSE;
	m_bEnableDocking = TRUE;
	m_pBufferPallet.Release();
	m_pTempPallet[0].Release();
	m_pTempPallet[1].Release();

	m_nNextPowerUpStep = PU_ROBOT_START;
	m_nNextAutoStep = AUTO_IF_CLEAR_MATERIAL;
}

void CRunProcessRobot::PowerUpOperation(void)
{
	if (!IsModuleEnable())
	{
		CRun::PowerUpOperation();
		m_strMsg = m_bLangulageEng?_T("Moudle UnEnable"):_T("ģ�����");		
		return;
	}

	switch(m_nNextPowerUpStep)
	{
		//����������
	case PU_ROBOT_START:
		{
			m_strMsg = m_bLangulageEng?_T("Start Robot Run"):_T("���������ˣ�ͬʱ���η��Ͳ���ȫ�ź�");
			if (IsNoHardware() || RobotStop())
			{
				SendSafeSignal(FALSE);
				m_nNextPowerUpStep = PU_ROBOT_ALARM_RESET;
			}
			break;
		}
		//�����˱�����λ
	case PU_ROBOT_ALARM_RESET:
		{
			m_strMsg = m_bLangulageEng?_T("Reset Robot Alarm"):_T("�����˱�����λ");

			if (IsNoHardware() || RobotAlarmReset())
			{
				m_nNextPowerUpStep = PU_ROBOT_ALARM_CHECK;
			}
			break;
		}
		//�������˱����ź�
	case PU_ROBOT_ALARM_CHECK:
		{
			m_strMsg = m_bLangulageEng?_T("Check Robot Alarm Signal"):_T("�������˱����ź�");

			if (IsNoHardware() || (m_IRobotAlarm > -1 && CheckInput(INPUTS[m_IRobotAlarm],OFF)))
			{
				m_nNextPowerUpStep = PU_ROBOT_CODE_RESET_AND_RUN;
			}
			else
			{
				m_nNextPowerUpStep =PU_ROBOT_ALARM_RESET;
			}
			break;
		}
		//���������˳���
	case PU_ROBOT_CODE_RESET_AND_RUN:
		{
			m_strMsg = m_bLangulageEng?_T("Start Robot Run"):_T("����������");

			if (IsNoHardware() || RobotCodeReset())
			{
				if (IsNoHardware() || RobotCodeRun())
				{
					m_nNextPowerUpStep = PU_ROBOT_CONNET;
				}
			}
			break;
		}
		//���ӻ�����
	case PU_ROBOT_CONNET:
		{
			m_strMsg = m_bLangulageEng?_T("Connect Robot"):_T("���ӻ�����");

			if (IsNoHardware() || !m_bEnableRobot || m_pRobotClient.Connect(m_strRobotIp, m_nRobotPort))
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
				m_nNextPowerUpStep = PU_FINGER_CYL_CLOSE_WITH_PALLET;
			}
			else
			{
				m_nNextPowerUpStep = PU_CHECK_WITHOUT_PRODUCT;
			}
			break;
		}
		//����޲�Ʒ
	case PU_CHECK_WITHOUT_PRODUCT:
		{
			m_strMsg = m_bLangulageEng?_T("Check Without Product"):_T("����޲�Ʒ");

			if (IsNoHardware() || CheckHasVaild(-1, FALSE))
			{
				m_nNextPowerUpStep = PU_FINGER_CYL_OPEN;		 							 
			}
			break;
		}
		//��צ��
	case PU_FINGER_CYL_OPEN:
		{
			m_strMsg = m_bLangulageEng?_T("FingerCyl Cyl Open"):_T("��צ��");

			if (IsNoHardware() || FingerCylClose(-1, FALSE))
			{
				m_nNextPowerUpStep = PU_ROBOT_MOVE_TO_SAFE_POS;
			}
			break;
		}
		//��λ������
		m_strMsg = m_bLangulageEng?_T("Reset Robot"):_T("�����˵���ȫλ");
	case PU_ROBOT_MOVE_TO_SAFE_POS:
		{

			if (IsNoHardware() || !m_bEnableRobot || m_pRobotClient.SendToDeviceAndWait(ROBOT_GO_HOME))
			{
				if(m_bEnableRobot)
					m_nCurRobotLoc = ROBOT_GO_HOME;
				m_bRobotSafe = TRUE;
				if(IsNoHardware())
				{
					m_nNextPowerUpStep = PU_BUFFER_CYL_BACK;
					break;
				}
				SendSafeSignal(TRUE); //����η���ȫ�ź�

				m_nNextPowerUpStep = PU_BUFFER_CYL_BACK;
			}
			else
			{
				//�������ƶ���ʱ
				ShowMessage(MSG_ROBOT_MOVE_TIMEOUT);
			}
			break;
		}
		//�������׻���
	case PU_BUFFER_CYL_BACK:
		{
			m_strMsg = m_bLangulageEng?_T("Reserve Cyl Back"):_T("�������׻���");
			if(IsNoHardware() || BufferCylPush(FALSE))
			{
				//�������
				m_nWorkIndex = -1;
				m_pTempPallet[0].Release();
				m_pTempPallet[1].Release();
				m_pBufferPallet.Release();

				m_nNextPowerUpStep = PU_JOINT_SIGNAL_RESET;
				m_nNextAutoStep = AUTO_IF_CLEAR_MATERIAL;
			}
			break;
		}

		//������������צ�ӽ����ɿ�  
		//���μ�צδ�ӽ����ڼ����������һ��ʱ,�ı��������ݣ������������ݶ�ʧ
	case PU_FINGER_CYL_CLOSE_WITH_PALLET:
		{
			m_strMsg = m_bLangulageEng?_T("Finger Cyl Open"):_T("������������צ�ӽ����ɿ�");

			if (m_pTempPallet[0].m_bIsValid || m_pTempPallet[1].m_bIsValid)// �Ѿ�ȡ�� ֻ�ڸ�ȡ��ʱ���Ƽ�צ����������ֱ�Ӹ�λ������
			{
				m_nNextPowerUpStep = PU_ROBOT_MOVE_TO_SAFE_POS_WITH_PALLET;

				int nAutoStep = m_nNextAutoStep;
				if (AUTO_FINGER_CYL_CLOSE < nAutoStep && nAutoStep <= AUTO_SEND_PICK_BATTERY_FINISH)  //��ȡ��
				{
					if(::IsNoHardware() || FingerCylClose(-1, TRUE))
					{
						m_nNextPowerUpStep = PU_WAIT_FRONT_MODULE_SAFE;
					}
				}
			}
			else
			{
				if(::IsNoHardware() || FingerCylClose(-1, FALSE))
				{
					m_nNextPowerUpStep = PU_ROBOT_MOVE_TO_SAFE_POS_WITH_PALLET;
				}
			}
			break;
		}

		//����ȡ��ؼ�צY�ᵽ��ȫλ
	case PU_WAIT_FRONT_MODULE_SAFE:
		{
			m_strMsg = m_bLangulageEng?_T("Reset Robot"):_T("����ȡ��ؼ�צY�ᵽ��ȫλ");

			if (MACHINE.m_pBatteryOffloadPnp[0] != NULL && MACHINE.m_pBatteryOffloadPnp[0]->FingerCylClose(FALSE) 
				&& MACHINE.m_pBatteryOffloadPnp[1] != NULL  && MACHINE.m_pBatteryOffloadPnp[1]->FingerCylClose(FALSE))
			{
				int nMotor[2] = {MACHINE.m_pBatteryOffloadPnp[0]->m_nMotorX, MACHINE.m_pBatteryOffloadPnp[1]->m_nMotorX};
				if(MotorsHome(nMotor, 2))
				{
					m_nNextPowerUpStep = PU_ROBOT_MOVE_TO_SAFE_POS_WITH_PALLET;
				}
			}
			break;
		}

		//��λ������
	case PU_ROBOT_MOVE_TO_SAFE_POS_WITH_PALLET:
		{
			m_strMsg = m_bLangulageEng?_T("Reset Robot"):_T("�����˵���ȫλ");

			if (IsNoHardware() || !m_bEnableRobot || m_pRobotClient.SendToDeviceAndWait(ROBOT_GO_HOME))
			{
				if(m_bEnableRobot)
				{
					m_nCurRobotLoc = ROBOT_GO_HOME;
				}
				if (!m_bRobotSafe)
				{
					m_bRobotSafe = TRUE;
					m_nNextPowerUpStep = PU_CHECK_BUFFER_PALLET_DATA;
				}
			}
			else
			{
				ShowMessage(MSG_ROBOT_MOVE_TIMEOUT);
			}
			break;
		}

		//��⻺��������ʵ���Ƿ�һ��
	case PU_CHECK_BUFFER_PALLET_DATA:
		{
			m_strMsg = m_bLangulageEng?_T("Move XY To Onload Pos"):_T("��⻺��������ʵ���Ƿ�һ��");
			if (CheckStartWithPalletDataSameForBuffer())
			{ 
				if (m_nBufferWithPalletData ==0)
				{
					m_pBufferPallet.Release();
				}
				m_nNextAutoStep = PU_CHECK_PALLET_DATA;
			}
			else
			{
				m_nNextAutoStep = PU_CHECK_BUFFER_PALLET_DATA;
			}
		}

		//�������������ʵ���Ƿ�һ��
	case PU_CHECK_PALLET_DATA:
		{
			m_strMsg = m_bLangulageEng?_T("Move XY To Onload Pos"):_T("�������������ʵ���Ƿ�һ��");
			if(CheckStartWithPalletDataSame())
			{
				if(m_nStartWithPalletData == 0)
				{
					for(int i = 0; i < 2; i++)
					{
						m_pTempPallet[i].Release();
					}
					m_nWorkIndex = -1;
					m_nNextAutoStep = AUTO_IF_CLEAR_MATERIAL;
					m_nNextPowerUpStep = PU_JOINT_SIGNAL_RESET;
				}
				else if(m_nStartWithPalletData == 1)//12��צ����
				{
					m_nNextAutoStep = AUTO_JUDGE_HAVE_NG;
					m_nNextPowerUpStep = PU_JUDGE_PRESS_DATE_EXIST;
				}
				else if(m_nStartWithPalletData == 2)  //1��צ����
				{
					m_nNextAutoStep = AUTO_JUDGE_HAVE_NG;
					m_nNextPowerUpStep = PU_JUDGE_PRESS_DATE_EXIST;
				}
				else if(m_nStartWithPalletData == 3)//2��צ����
				{
					m_nNextAutoStep = AUTO_JUDGE_HAVE_NG;
					m_nNextPowerUpStep = PU_JUDGE_PRESS_DATE_EXIST;
				}
			}
			break;
		}

		//�ж�ѹ�������Ƿ����  ��ֹ���������ȷ���λ��Ʒ
	case PU_JUDGE_PRESS_DATE_EXIST:
		{
			m_strMsg = m_bLangulageEng?_T("Judge Press Date Exist"):_T("�ж�ѹ�������Ƿ����");

			if (m_pTempPallet[0].m_bIsValid 
				&& (m_pTempPallet[0].m_dPressNormValue[0] ==0 || m_pTempPallet[0].m_dPressNormValue[1] == 0 
				|| m_pTempPallet[0].m_strBarCode.GetLength() == 0)) 
			{
				m_pTempPallet[0].m_bIsNg = TRUE;
			}
			if (m_pTempPallet[1].m_bIsValid
				&& (m_pTempPallet[1].m_dPressNormValue[0] == 0 || m_pTempPallet[1].m_dPressNormValue[1] == 0 
				|| m_pTempPallet[1].m_strBarCode.GetLength() == 0))
			{
				m_pTempPallet[1].m_bIsNg = TRUE;
			}

			if (m_pBufferPallet.m_bIsValid && (m_pBufferPallet.m_dPressNormValue[0] == 0 || m_pBufferPallet.m_strBarCode.GetLength() == 0))
			{
				m_pBufferPallet.m_bIsNg = TRUE;
			}

			m_nNextPowerUpStep = PU_JOINT_SIGNAL_RESET;
			break;
		}

		//��λ���ζԽ��ź�
	case PU_JOINT_SIGNAL_RESET:
		{
			m_strMsg = m_bLangulageEng?_T("Reset Signal"):_T("��λ�����ź�");

			if(IsNoHardware() || MACHINE.m_bUnEnableDocking || SendPlaceFinishSignal(FALSE))
			{
				m_nNextPowerUpStep = PU_CONNECT_SERVER;
			}
			break;
		}

	case PU_CONNECT_SERVER:
		{
			m_strMsg = m_bLangulageEng?_T("Connect Server"):_T("���Ӻ�η�����");
			
			if (!m_bShieldServer || m_Client.Connect(m_strServerIp,m_nServerPort))
			{
				m_nNextPowerUpStep = PU_FINISH;
			}
			else
			{
				ShowMessage(_T("���Ӻ�η�����ʧ�ܣ��������Ƿ��ʼ����"),3);
			}
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

void CRunProcessRobot::AutoOperation()
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
		if (m_nSaveLastStep == AUTO_IF_CLEAR_MATERIAL)
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
	case AUTO_IF_CLEAR_MATERIAL:
		{
			m_strMsg = m_bLangulageEng?_T("Send Front Module Move Motor"):_T("�ж��Ƿ�����");

			if (MACHINE.m_bClearMaterial)
			{
				int nCoreNum =MACHINE.CoreCountForClear(TRUE);
				if (nCoreNum <= 1)
				{
					//�������
					ShowMessage(CLEAR_MATERIAL_FINISH);
					MACHINE.m_bClearMaterial = FALSE;
					MACHINE.m_pRunCtrl->Stop();
				    Sleep(100);
					break;
				}
				else
				{
					m_nNextAutoStep = AUTO_ROBOT_TO_PICK_WAIT_POS_INIT;
					SaveStepInfo(WRITE_TYPE_AUTO_STEP);
				}
			}
			else
			{
				m_nNextAutoStep = AUTO_ROBOT_TO_PICK_WAIT_POS_INIT;
				SaveStepInfo(WRITE_TYPE_AUTO_STEP);
			}
			break;
		}

		//�����˵�ȡ�ϵȴ�λ
	case AUTO_ROBOT_TO_PICK_WAIT_POS_INIT:
		{
			m_strMsg = m_bLangulageEng?_T("Robot To Pick Wait Pos"):_T("�����˵�ȡ�ϵȴ�λ");

			if (IsNoHardware() || !m_bEnableRobot || m_pRobotClient.SendToDeviceAndWait(ROBOT_ONLOAD_WAIT))
			{
				if(m_bEnableRobot)
				{
					m_nCurRobotLoc = ROBOT_ONLOAD_WAIT;
				}
				m_nNextAutoStep = AUTO_FINER_CLY_OPEN_INIT;
			}
			else
			{
				//�������ƶ���ʱ
				ShowMessage(MSG_ROBOT_MOVE_TIMEOUT);
			}
			break;
		}

		//��צ�����ſ�
	case AUTO_FINER_CLY_OPEN_INIT:       
		{
			m_strMsg = m_bLangulageEng?_T("FingerCyl Cly Open"):_T("��צ�����ſ�");

			if (IsNoHardware() || FingerCylClose(-1, FALSE))
			{
				m_nNextAutoStep = AUTO_WAIT_START_EVENT_ONLOAD;
				SaveStepInfo(WRITE_TYPE_AUTO_STEP);
			}
			break;
		}

		//onload
		//�ȴ�ȡ��ؼ�צ�����ź�
	case AUTO_WAIT_START_EVENT_ONLOAD:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Start Event"):_T("�ȴ�ȡ��ؼ�צ�����ź�");

			if (MACHINE.m_pBatteryOffloadPnp[0] != NULL && MACHINE.m_pBatteryOffloadPnp[1] != NULL 
				&& (MACHINE.m_pBatteryOffloadPnp[0]->m_bOffloadToRobot || !MACHINE.m_pBatteryOffloadPnp[0]->IsModuleEnable())
				&& (MACHINE.m_pBatteryOffloadPnp[1]->m_bOffloadToRobot || !MACHINE.m_pBatteryOffloadPnp[1]->IsModuleEnable()))
			{
				m_bRobotWaitPos = FALSE;
				m_nNextAutoStep = AUTO_ROBOT_TO_PICK_POS_ONLOAD;
				SaveStepInfo(WRITE_TYPE_AUTO_STEP);
			}
			else
			{
				if (!m_bRobotWaitPos)
				{
					m_nNextAutoStep = AUTO_ROBOT_TO_PICK_WAIT_POS_ONLOAD;
				}
			}
			break;
		}

		//�����˵�ȡ�ϵȴ�λ
	case AUTO_ROBOT_TO_PICK_WAIT_POS_ONLOAD:
		{
			m_strMsg = m_bLangulageEng?_T("Robot To Pick Wait Pos"):_T("�����˵�ȡ�ϵȴ�λ");

			if (IsNoHardware() || !m_bEnableRobot || m_pRobotClient.SendToDeviceAndWait(ROBOT_ONLOAD_WAIT))
			{
				if(m_bEnableRobot)
				{
					m_nCurRobotLoc = ROBOT_ONLOAD_WAIT;
				}

				if (!m_bRobotWaitPos)
				{
					m_bRobotWaitPos = TRUE;
					m_nNextAutoStep = AUTO_WAIT_START_EVENT_ONLOAD;
				}
			}
			else
			{
				//�������ƶ���ʱ
				ShowMessage(MSG_ROBOT_MOVE_TIMEOUT);
			}
			break;
		}

		//�����˵�ȡ��λ
	case AUTO_ROBOT_TO_PICK_POS_ONLOAD:
		{
			m_strMsg = m_bLangulageEng?_T("Robot To Pick Pos"):_T("�����˵�ȡ��λ");
			if (m_bRobotSafe)
			{
				if (IsNoHardware() || !m_bEnableRobot || m_pRobotClient.SendToDeviceAndWait(ROBOT_ONLOAD_POS))
				{
					if(m_bEnableRobot)
					{
						m_nCurRobotLoc = ROBOT_ONLOAD_POS;
					}
					m_bRobotSafe = FALSE;
					m_nNextAutoStep = AUTO_FINGER_CYL_CLOSE;
				}
				else
				{
					//�������ƶ���ʱ MSG_ROBOT_MOVE_TIMEOUT 2241
					ShowMessage(MSG_ROBOT_MOVE_TIMEOUT);
				}
			}
			break;
		}

		//��צ���׼н�
	case AUTO_FINGER_CYL_CLOSE:       
		{
			m_strMsg = m_bLangulageEng?_T("FingerCyl Cly Close"):_T("��צ���׼н�");

			if (IsNoHardware() || FingerCylClose(-1, TRUE))
			{
				//ȡ��ؼ�צ�������ݸ�������
				if(MACHINE.m_pBatteryOffloadPnp[0] != NULL && MACHINE.m_pBatteryOffloadPnp[1] != NULL)
				{
					m_pTempPallet[0] = MACHINE.m_pBatteryOffloadPnp[0]->m_Pallet;
					m_pTempPallet[1] = MACHINE.m_pBatteryOffloadPnp[1]->m_Pallet;

					MACHINE.m_pBatteryOffloadPnp[0]->m_Pallet.Release();
					MACHINE.m_pBatteryOffloadPnp[1]->m_Pallet.Release();

					//����ȡ��ؼ�צ�ɿ�,�������
					m_nNextAutoStep = AUTO_SEND_PICK_FINISH;

					SaveStepInfo(WRITE_TYPE_AUTO_STEP|WRITE_TYPE_BATTERY);

					MACHINE.m_pBatteryOffloadPnp[0]->SaveStepInfo(WRITE_TYPE_AUTO_STEP|WRITE_TYPE_BATTERY);
					MACHINE.m_pBatteryOffloadPnp[1]->SaveStepInfo(WRITE_TYPE_AUTO_STEP|WRITE_TYPE_BATTERY);
				}
			}
			break;
		}

		/*����ȡ�������*/
	case AUTO_SEND_PICK_FINISH:
		{
			m_strMsg = m_bLangulageEng?_T("Send Onload Finish"):_T("����ȡ�������");

			if (MACHINE.m_pBatteryOffloadPnp[0] != NULL && MACHINE.m_pBatteryOffloadPnp[1] != NULL 
				&& (MACHINE.m_pBatteryOffloadPnp[0]->m_bOffloadToRobot || !MACHINE.m_pBatteryOffloadPnp[0]->IsModuleEnable())
				&& (MACHINE.m_pBatteryOffloadPnp[1]->m_bOffloadToRobot || !MACHINE.m_pBatteryOffloadPnp[1]->IsModuleEnable()))
			{
				MACHINE.m_pBatteryOffloadPnp[0]->m_bOffloadToRobot = FALSE;
				MACHINE.m_pBatteryOffloadPnp[1]->m_bOffloadToRobot = FALSE;
				m_nNextAutoStep = AUTO_WAIT_BATTERY_OFFLOAD_PNP_AVOID;

				MACHINE.m_pBatteryOffloadPnp[0]->SaveStepInfo(WRITE_TYPE_AUTO_STEP|WRITE_TYPE_VARIABLES);
				MACHINE.m_pBatteryOffloadPnp[1]->SaveStepInfo(WRITE_TYPE_AUTO_STEP|WRITE_TYPE_VARIABLES);
			}
			break;
		}
	case AUTO_WAIT_BATTERY_OFFLOAD_PNP_AVOID:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Offload Pnp Avoid"):_T("�ȴ�ȡ��ؼ�צ����");
			if (MACHINE.m_pBatteryOffloadPnp[0] != NULL && MACHINE.m_pBatteryOffloadPnp[1] != NULL 
				&& (MACHINE.m_pBatteryOffloadPnp[0]->m_bOpenCylOpen || !MACHINE.m_pBatteryOffloadPnp[0]->IsModuleEnable())
				&& (MACHINE.m_pBatteryOffloadPnp[1]->m_bOpenCylOpen || !MACHINE.m_pBatteryOffloadPnp[1]->IsModuleEnable()))
			{
				m_nNextAutoStep = AUTO_WAIT_MES_SINGLE;
				SaveStepInfo(WRITE_TYPE_AUTO_STEP);
			}
			break;
		}

		//�ȴ�MES�ź�
	case AUTO_WAIT_MES_SINGLE:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Mes Single"):_T("�ȴ�MES�ź�1");
			if((MACHINE.m_pUploadMes != NULL && (MACHINE.m_pUploadMes->m_bNextRequire || !MACHINE.m_pUploadMes->IsModuleEnable())))
			{
				MACHINE.m_pUploadMes->m_Pallet = m_pTempPallet[0];
				MACHINE.m_pUploadMes->m_bNextRequire = FALSE;
				m_nNextAutoStep = AUTO_ROBOT_TO_PICK_WAIT_POS_ONLOAD_CHECK;
			}
			break;
		}

		//ȡ����ɣ������˵�ȡ�ϵȴ�λ
	case AUTO_ROBOT_TO_PICK_WAIT_POS_ONLOAD_CHECK:
		{
			m_strMsg = m_bLangulageEng?_T("Robot Move To Pick Wait Pos"):_T("ȡ����ɣ������˵�ȡ�ϵȴ�λ");

			if (IsNoHardware() ||  !m_bEnableRobot || m_pRobotClient.SendToDeviceAndWait(ROBOT_ONLOAD_WAIT))
			{
				if(m_bEnableRobot)
				{
					m_nCurRobotLoc = ROBOT_ONLOAD_WAIT;
				}

				if (!m_bRobotSafe)
				{
					m_bRobotSafe = TRUE;
					m_nNextAutoStep = AUTO_SEND_PICK_BATTERY_FINISH;
					SaveStepInfo(WRITE_TYPE_AUTO_STEP);
				}
			}
			else
			{
				//�������ƶ���ʱ
				ShowMessage(MSG_ROBOT_MOVE_TIMEOUT);
			}
			break;
		}
	case AUTO_SEND_PICK_BATTERY_FINISH:
		{
			m_strMsg = m_bLangulageEng?_T("Check Have Product"):_T("����ȡ��ؼ�צ�����ź�");

			if (MACHINE.m_pBatteryOffloadPnp[0] != NULL && MACHINE.m_pBatteryOffloadPnp[1] != NULL 
				&& (MACHINE.m_pBatteryOffloadPnp[0]->m_bOpenCylOpen || !MACHINE.m_pBatteryOffloadPnp[0]->IsModuleEnable())
				&& (MACHINE.m_pBatteryOffloadPnp[1]->m_bOpenCylOpen || !MACHINE.m_pBatteryOffloadPnp[1]->IsModuleEnable()))
			{
				MACHINE.m_pBatteryOffloadPnp[0]->m_bOpenCylOpen = FALSE;
				MACHINE.m_pBatteryOffloadPnp[1]->m_bOpenCylOpen = FALSE;
				m_nNextAutoStep = AUTO_CHECK_HAVE_PRODUCT;

				MACHINE.m_pBatteryOffloadPnp[0]->SaveStepInfo(WRITE_TYPE_AUTO_STEP|WRITE_TYPE_VARIABLES);
				MACHINE.m_pBatteryOffloadPnp[1]->SaveStepInfo(WRITE_TYPE_AUTO_STEP|WRITE_TYPE_VARIABLES);
			}
			break;
		}
		//�������
	case AUTO_CHECK_HAVE_PRODUCT:
		{
			m_strMsg = m_bLangulageEng?_T("Check Have Product"):_T("ȡ����ɣ��������");

			if (IsNoHardware() || m_bDryRun || (CheckHasVaild(0, TRUE) && CheckHasVaild(1, TRUE)))                         
			{
				//m_nWorkIndex = -1;    //���Բ�����
				/*	m_pTempPallet[0].m_bIsNg =TRUE;
				m_pTempPallet[1].m_bIsNg=TRUE;
				m_pTempPallet[0].m_bIsValid = TRUE;
				m_pTempPallet[1].m_bIsValid = TRUE;*/
				m_nNextAutoStep = AUTO_WAIT_MES_SINGLE_FOR_OFFLOAD;
			}
			break;
		}

		//�ȴ�MES�ź�2
	case AUTO_WAIT_MES_SINGLE_FOR_OFFLOAD:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Mes Single"):_T("�ȴ�MES�ź�2");
			if ((MACHINE.m_pUploadMes != NULL && (MACHINE.m_pUploadMes->m_bNextRequire || !MACHINE.m_pUploadMes->IsModuleEnable())))
			{	
				MACHINE.m_pUploadMes->m_Pallet = m_pTempPallet[1];
				MACHINE.m_pUploadMes->m_bNextRequire = FALSE;
				m_nNextAutoStep = AUTO_JUDGE_HAVE_NG;
			}
			break;
		}


		//�ж�����NG
	case AUTO_JUDGE_HAVE_NG:
		{
			m_strMsg = m_bLangulageEng?_T("Judge Have Ng"):_T("�ж�����NG");
			if(m_bDryRun)
			{
				m_nWorkIndex = -1;
				m_nNextAutoStep = AUTO_WAIT_START_EVENT_OFFLOAD;
				break;
			}


			if(m_pTempPallet[0].m_bIsValid && m_pTempPallet[1].m_bIsValid)
			{
				if(  !MACHINE.m_pPressNgLine->IsModuleEnable() || !m_pTempPallet[0].m_bIsNg && !m_pTempPallet[1].m_bIsNg)  //ѹ��������λ����ʱֱ����Ʒ����
				{
					m_nWorkIndex = -1;
					m_nNextAutoStep = AUTO_WAIT_START_EVENT_OFFLOAD;
				}
				else
				{
					if(m_pTempPallet[0].m_bIsNg && m_pTempPallet[1].m_bIsNg)
						m_nWorkIndex = -1;
					else if(m_pTempPallet[0].m_bIsNg)
						m_nWorkIndex = 0;
					else if(m_pTempPallet[1].m_bIsNg)
						m_nWorkIndex = 1;
					m_nNextAutoStep = AUTO_WAIT_START_EVENT_NG;

				}
			}
			else if(m_pTempPallet[0].m_bIsValid || m_pTempPallet[1].m_bIsValid)
			{
				if(m_pTempPallet[0].m_bIsNg || m_pTempPallet[1].m_bIsNg)
				{
					if(m_pTempPallet[0].m_bIsNg)
						m_nWorkIndex = 0;
					else
						m_nWorkIndex = 1;
					m_nNextAutoStep = AUTO_WAIT_START_EVENT_NG;
				}
				else
				{
					if(!m_pBufferPallet.m_bIsValid)
					{// ���ݴ�
						if(m_pTempPallet[0].m_bIsValid)
							m_nWorkIndex = 0;
						else
							m_nWorkIndex = 1;
						m_nNextAutoStep = AUTO_ROBOT_TO_BUF_WAIT_POS;
					}
					else
					{// ȡ�ݴ�
						if(!m_pTempPallet[0].m_bIsValid)
							m_nWorkIndex = 0;
						else
							m_nWorkIndex = 1;
						m_nNextAutoStep = AUTO_ROBOT_TO_BUF_WAIT_POS;
					}
				}	
			}
			else
			{
				if(m_bDryRun)
				{
					m_nWorkIndex = -1;
					m_nNextAutoStep = AUTO_WAIT_START_EVENT_OFFLOAD;
				}
				else
				{
					m_nNextAutoStep = AUTO_WORK_END;
				}
				
			}
			break;
		}

		//offload
		//�ȴ����Ҫ���ź�
	case AUTO_WAIT_START_EVENT_OFFLOAD:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Next Module Require"):_T("�ȴ����Ҫ���ź�");
			if(MACHINE.m_bUnEnableDocking || CheckNeedBatteryEvent(TRUE) && SendSafeSignal(FALSE)) //���Ҫ�ϵ�ͬʱ�򿪲���ȫ�ź�
			{
				m_bRobotWaitPos = FALSE;
				m_nNextAutoStep = AUTO_ROBOT_TO_PUT_POS_OFFLOAD;
			}
			else
			{
				if (!m_bRobotWaitPos)
				{
					m_nNextAutoStep = AUTO_ROBOT_TO_PUT_WAIT_POS_OFFLOAD;
				}
			}
			break;
		}
		//�����˵����ϵȴ�λ
	case AUTO_ROBOT_TO_PUT_WAIT_POS_OFFLOAD:
		{
			m_strMsg = m_bLangulageEng?_T("Robot To Put Wait Pos"):_T("�����˵����ϵȴ�λ");
			if (IsNoHardware() ||  !m_bEnableRobot || m_pRobotClient.SendToDeviceAndWait(ROBOT_OFFLOAD_WAIT_POS))
			{
				if(m_bEnableRobot)
				{
					m_nCurRobotLoc = ROBOT_OFFLOAD_WAIT_POS;
				}

				if (!m_bRobotWaitPos)
				{
					m_bRobotWaitPos = TRUE;
					m_nNextAutoStep = AUTO_WAIT_START_EVENT_OFFLOAD;
				}
			}
			else
			{
				//�������ƶ���ʱ
				ShowMessage(MSG_ROBOT_MOVE_TIMEOUT);
			}
			break;
		}

		//�����˵�����λ
	case AUTO_ROBOT_TO_PUT_POS_OFFLOAD:
		{
			m_strMsg = m_bLangulageEng?_T("Robot To Put Pos"):_T("�����˵�����λ");

			if (IsNoHardware() ||  !m_bEnableRobot || m_pRobotClient.SendToDeviceAndWait(ROBOT_OFFLOAD_POS))
			{
				if(m_bEnableRobot)
				{
					m_nCurRobotLoc = ROBOT_OFFLOAD_POS;
				}

				m_nNextAutoStep = AUTO_FINGER_CYL_OPEN_OFFLOAD;
			}
			else
			{
				//�������ƶ���ʱ
				ShowMessage(MSG_ROBOT_MOVE_TIMEOUT);
			}
			break;
		}

	//	//�ȴ�MES�ź�
	//case AUTO_WAIT_MES_SINGLE_FOR_OFFLOAD:
	//	{
	//		m_strMsg = m_bLangulageEng?_T("Wait Mes Single"):_T("�ȴ�MES�ź�");
	//		if(m_bBufferMes  || (MACHINE.m_pUploadMes != NULL && (MACHINE.m_pUploadMes->m_bNextRequire || !MACHINE.m_pUploadMes->IsModuleEnable())))
	//		{
	//			if (!m_bBufferMes)
	//			{
	//				MACHINE.m_pUploadMes->m_Pallet = m_pTempPallet[1];
	//				MACHINE.m_pUploadMes->m_bNextRequire = FALSE;
	//			}
	//			m_bBufferMes = FALSE;
	//			m_nNextAutoStep = AUTO_FINGER_CYL_OPEN_OFFLOAD;
	//		}
	//		break;
	//	}

		//��צ��
	case AUTO_FINGER_CYL_OPEN_OFFLOAD:
		{
			m_strMsg = m_bLangulageEng?_T("FingerCyl Cyl Open"):_T("����λ����צ��");

			if(IsNoHardware() || FingerCylClose(-1, FALSE))
			{
				m_nNextAutoStep = AUTO_SEND_DATE;
			}
			break;
		}

		//����η�������
	case AUTO_SEND_DATE:
		{
			m_strMsg = m_bLangulageEng?_T("Send Date"):_T("����η�������");

			CString Result=_T(""); //����������
			CString BarCode = _T("");
			GetLocalPalletData(m_pTempPallet,BarCode);   //��÷�������
			if (!m_bShieldServer || (m_Client.SendToDeviceAndWait(BarCode,Result) && Result == _T("02")))  //��λظ�����Ϊ02
			{
				MACHINE.m_strShowRecv = Result; // ���Խ�����ʾ
				//����η�������
				m_pTempPallet[0].Release();
				m_pTempPallet[1].Release();
				MACHINE.m_nOffloadCount += 2;
			}
			else
			{
				ShowMessage(_T("�����������η�����ʧ�ܣ�����!"),3);
				break;
			}
			//MACHINE.m_ReportFile.OkCount();
			//MACHINE.m_ReportFile.OkCount();
			m_nNextAutoStep = AUTO_ROBOT_TO_PUT_WAIT_POS_OFFLOAD_CHECK;
			SaveStepInfo(WRITE_TYPE_AUTO_STEP|WRITE_TYPE_VARIABLES|WRITE_TYPE_BATTERY);
			break;
		}

		//�����˻ص���ȫλ
	case AUTO_ROBOT_TO_PUT_WAIT_POS_OFFLOAD_CHECK:
		{
			m_strMsg = m_bLangulageEng?_T("Robot Move To Safe Loc"):_T("������ɣ������˵���Ʒ���ϵȴ�λ");

			if (IsNoHardware() || !m_bEnableRobot || m_pRobotClient.SendToDeviceAndWait(ROBOT_OFFLOAD_WAIT_POS))
			{
				if(m_bEnableRobot)
				{
					m_nCurRobotLoc = ROBOT_OFFLOAD_WAIT_POS;
				}
				m_nNextAutoStep = AUTO_CHECK_WITHOUT_PRODUCT_OFFLOAD;
			}
			else
			{
				//�������ƶ���ʱ
				ShowMessage(MSG_ROBOT_MOVE_TIMEOUT);
			}
			break;
		}

		//����Ƿ�����
	case AUTO_CHECK_WITHOUT_PRODUCT_OFFLOAD:
		{
			m_strMsg = m_bLangulageEng?_T("Check Without Product"):_T("������ɣ��������");

			if (IsNoHardware() || (CheckHasVaild(0,FALSE) && CheckHasVaild(1, FALSE)))                         
			{
				m_nNextAutoStep = AUTO_SEND_PUT_FINISH_SIGNAL_OFFLOAD;
			}
			break;
		}

		/*���ͷ��������*/
	case AUTO_SEND_PUT_FINISH_SIGNAL_OFFLOAD:
		{
			m_strMsg = m_bLangulageEng?_T("Send Onload Finish"):_T("����η��ͷ�����ɼ���ȫ�ź�");

			if (SendPlaceFinishSignal(TRUE) && SendSafeSignal(TRUE))
			{
				m_nNextAutoStep = AUTO_WAIT_OFFLOAD_TO_NEXT_FINISH;
				SaveStepInfo(WRITE_TYPE_AUTO_STEP|WRITE_TYPE_VARIABLES);
			}
			break;
		}
	case AUTO_WAIT_OFFLOAD_TO_NEXT_FINISH:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Offload To Next Finish"):_T("�ȴ����ȡ�����");
			if (MACHINE.m_bUnEnableDocking || CheckNeedBatteryEvent(FALSE) && SendPlaceFinishSignal(FALSE)) //˫���źŶ�����
			{
				m_nNextAutoStep = AUTO_JUDGE_BATTERY_ALL_PUT_FINISH;
			}
			break;
		}
		//ng
		//�ȴ�NG�ɷ����ź�
	case AUTO_WAIT_START_EVENT_NG:
		{
			m_strMsg = m_bLangulageEng?_T("Wait NG Require"):_T("�ȴ�NG�ɷ����ź�");

			if(MACHINE.m_pPressNgLine != NULL && (MACHINE.m_pPressNgLine->m_bRequireNgBattery || !MACHINE.m_pPressNgLine->IsModuleEnable()))
			{
				m_bRobotWaitPos = FALSE;
				m_nNextAutoStep = AUTO_ROBOT_TO_NG_POS;
			}
			else
			{
				if (!m_bRobotWaitPos)
				{
					m_nNextAutoStep = AUTO_ROBOT_TO_NG_WAIT_POS;
				}
			}
			break;
		}
		//�����˵���NG�ȴ�λ
	case AUTO_ROBOT_TO_NG_WAIT_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Robot To Put NG Wait Pos"):_T("�����˵���NG�ȴ�λ");

			if (IsNoHardware() || !m_bEnableRobot || m_pRobotClient.SendToDeviceAndWait(ROBOT_OFFLOAD_NG_WAIT_POS))
			{
				if(m_bEnableRobot)
				{
					m_nCurRobotLoc = ROBOT_OFFLOAD_NG_WAIT_POS;
				}

				if (!m_bRobotWaitPos)
				{
					m_bRobotWaitPos = TRUE;
					m_nNextAutoStep = AUTO_WAIT_START_EVENT_NG;
				}
			}
			else
			{
				//�������ƶ���ʱ
				ShowMessage(MSG_ROBOT_MOVE_TIMEOUT);
			}
			break;
		}

		//�����˵�NGλ
	case AUTO_ROBOT_TO_NG_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Robot To Put Pos"):_T("�����˵���NGλ");

			if (IsNoHardware() || !m_bEnableRobot || m_pRobotClient.SendToDeviceAndWait(ROBOT_OFFLOAD_NG_POS))
			{
				if(m_bEnableRobot)
				{
					m_nCurRobotLoc = ROBOT_OFFLOAD_NG_POS;
				}
				m_nNextAutoStep = AUTO_FINGER_CYL_OPEN_NG;
			}
			else
			{
				//�������ƶ���ʱ
				ShowMessage(MSG_ROBOT_MOVE_TIMEOUT);
			}
			break;
		}

	//	//�ȴ�MES�ź�
	//case AUTO_WAIT_MES_SINGLE_FOR_NG:
	//	{
	//		m_strMsg = m_bLangulageEng?_T("Wait Mes Single"):_T("�ȴ�MES�ź�");
	//		if((MACHINE.m_pUploadMes != NULL && (MACHINE.m_pUploadMes->m_bNextRequire || !MACHINE.m_pUploadMes->IsModuleEnable())))
	//		{
	//			MACHINE.m_pUploadMes->m_Pallet = m_pTempPallet[1];
	//			MACHINE.m_pUploadMes->m_bNextRequire = FALSE;
	//			m_nNextAutoStep = AUTO_FINGER_CYL_OPEN_NG;
	//		}
	//		break;
	//	}

		//NGλ��צ��
	case AUTO_FINGER_CYL_OPEN_NG:
		{
			m_strMsg = m_bLangulageEng?_T("FingerCyl Cyl Open"):_T("NGλ��צ��");
			ASSERT(-1 <= m_nWorkIndex && m_nWorkIndex <= 1);
			if(IsNoHardware() || FingerCylClose(m_nWorkIndex, FALSE))
			{
				if(0 <= m_nWorkIndex && m_nWorkIndex <= 1)
				{
					m_pTempPallet[m_nWorkIndex].Release();
					MACHINE.m_nShellStationPressNgCount[m_nWorkIndex]++;
					/*MACHINE.m_nNgCount++;*/
					//ASSERT(1 <= m_pTempPallet[m_nWorkIndex].m_nShellID && m_pTempPallet[m_nWorkIndex].m_nShellID <= 2);
					//MACHINE.m_nShellStationPressNgCount[m_pTempPallet[m_nWorkIndex].m_nShellID - 1]++;
					//MACHINE.m_ReportFile.NgCount();
				}
				else if(m_nWorkIndex == -1)
				{
					m_pTempPallet[0].Release();
					m_pTempPallet[1].Release();
					MACHINE.m_nNgCount += 2;
					MACHINE.m_nShellStationPressNgCount[0]++;
					MACHINE.m_nShellStationPressNgCount[1]++;
					//MACHINE.m_ReportFile.NgCount();
					//MACHINE.m_ReportFile.NgCount();
				}
				else
				{
					ASSERT(0);
				}
				m_nNextAutoStep = AUTO_ROBOT_TO_NG_WAIT_POS_CHECK;
				SaveStepInfo(WRITE_TYPE_AUTO_STEP|WRITE_TYPE_BATTERY);
			}
			break;
		}

		//�����˻ص���ȫλ
	case AUTO_ROBOT_TO_NG_WAIT_POS_CHECK:
		{
			m_strMsg = m_bLangulageEng?_T("Robot Move To Safe Loc"):_T("NG������ɣ������˵�NG�ȴ�λ");

			if (IsNoHardware() || !m_bEnableRobot || m_pRobotClient.SendToDeviceAndWait(ROBOT_OFFLOAD_NG_WAIT_POS))
			{
				if(m_bEnableRobot)
				{
					m_nCurRobotLoc = ROBOT_OFFLOAD_NG_WAIT_POS;
				}
				m_nNextAutoStep = AUTO_CHECK_WITHOUT_PRODUCT_NG;
			}
			else
			{
				//�������ƶ���ʱ
				ShowMessage(MSG_ROBOT_MOVE_TIMEOUT);
			}
			break;
		}

		//����Ƿ�����
	case AUTO_CHECK_WITHOUT_PRODUCT_NG:
		{
			m_strMsg = m_bLangulageEng?_T("check FingerCyl Status"):_T("NG������ɣ��������");

			if(m_nWorkIndex == -1)
			{
				if (IsNoHardware() || (CheckHasVaild(0, FALSE) && CheckHasVaild(1, FALSE)))
				{
					m_nNextAutoStep = AUTO_SEND_NG_FINISH_SIGNAL_NG;
					SaveStepInfo(WRITE_TYPE_AUTO_STEP|WRITE_TYPE_VARIABLES);
				}
			}
			else
			{
				ASSERT(0 <= m_nWorkIndex && m_nWorkIndex <= 1);
				if (IsNoHardware() || CheckHasVaild(m_nWorkIndex, FALSE))
				{
					m_nNextAutoStep = AUTO_SEND_NG_FINISH_SIGNAL_NG;
					SaveStepInfo(WRITE_TYPE_AUTO_STEP|WRITE_TYPE_VARIABLES);
				}
			}
			break;
		}

		/*���ͷ��������*/
	case AUTO_SEND_NG_FINISH_SIGNAL_NG:
		{
			m_strMsg = m_bLangulageEng?_T("Send Onload Finish"):_T("���ͷ������");
			if (MACHINE.m_pPressNgLine != NULL && (MACHINE.m_pPressNgLine->m_bRequireNgBattery || !MACHINE.m_pPressNgLine->IsModuleEnable()))
			{
				MACHINE.m_pPressNgLine->m_bRequireNgBattery = FALSE;
				m_nNextAutoStep = AUTO_JUDGE_BATTERY_ALL_PUT_FINISH;
				SaveStepInfo(WRITE_TYPE_AUTO_STEP);
			}
			break;
		}

	case AUTO_ROBOT_TO_BUF_WAIT_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Robot To BufferCyl Wait Pos"):_T("�����˵�����ȴ�λ");
			ASSERT(0 <= m_nWorkIndex && m_nWorkIndex <= 1);
			int nLoc = ((m_nWorkIndex == 0)?(ROBOT_OFFLOAD_BUFF_WAIT_POS1):(ROBOT_OFFLOAD_BUFF_WAIT_POS2));

			if (IsNoHardware() ||  !m_bEnableRobot || m_pRobotClient.SendToDeviceAndWait(nLoc))
			{
				if(m_bEnableRobot)
				{
					m_nCurRobotLoc = nLoc;
				}
				m_nNextAutoStep = AUTO_BUFFER_CYL_ACTION_BUF;
			}
			else
			{
				//�������ƶ���ʱ 
				ShowMessage(MSG_ROBOT_MOVE_TIMEOUT);
			}
			break;
		}

		//����λ���׶���
	case AUTO_BUFFER_CYL_ACTION_BUF:
		{
			m_strMsg = m_bLangulageEng?_T("Reserve Cyl Action"):_T("����λ���׶���");
			// �˴�����ʵ�ʵ����޸�
			ASSERT(0 <= m_nWorkIndex && m_nWorkIndex <= 1);

			if(IsNoHardware() || BufferCylPush(m_nWorkIndex == 1?TRUE:FALSE))
			{
				m_nNextAutoStep = AUTO_CHECK_HAVE_PRODUCT_BUF;
			}
			break;
		}

		//��⻺���о״̬
	case AUTO_CHECK_HAVE_PRODUCT_BUF:
		{
			m_strMsg = m_bLangulageEng?_T("Check Have Product BufferCyl"):_T("��⻺���о״̬");
			// �˴�����ʵ�ʵ����޸�
			ASSERT(0 <= m_nWorkIndex && m_nWorkIndex <= 1);
			if (IsNoHardware() || m_bDryRun || CheckHasVaild((m_nWorkIndex+2), m_pBufferPallet.m_bIsValid))
			{
				m_nNextAutoStep = AUTO_ROBOT_TO_BUF_POS;
			}
			break;
		}

		//�����˵�����λ
	case AUTO_ROBOT_TO_BUF_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Robot To BufferCyl Pos"):_T("�����˵�����λ");
			ASSERT(0 <= m_nWorkIndex && m_nWorkIndex <= 1);
			int nLoc = ((m_nWorkIndex == 0)?(ROBOT_OFFLOAD_BUFF_POS1):(ROBOT_OFFLOAD_BUFF_POS2));

			if (IsNoHardware() || !m_bEnableRobot || m_pRobotClient.SendToDeviceAndWait(nLoc))
			{
				if(m_bEnableRobot)
				{
					m_nCurRobotLoc = nLoc;
				}
				m_nNextAutoStep = AUTO_FINGER_CYL_ACTION_BUF;
			}
			else
			{
				//�������ƶ���ʱ
				ShowMessage(MSG_ROBOT_MOVE_TIMEOUT);
			}
			break;
		}

		//�ڻ���λ��ץ����
	case AUTO_FINGER_CYL_ACTION_BUF:
		{
			m_strMsg = m_bLangulageEng?_T("FingerCyl Cyl For BufferCyl Pos"):_T("�ڻ���λ��ץ����");
			ASSERT(0 <= m_nWorkIndex && m_nWorkIndex <= 1);
			if(IsNoHardware() || FingerCylClose(m_nWorkIndex, m_pBufferPallet.m_bIsValid))
			{
				BOOL bBufPalletRelease;   //TRUE ����������� FALSE��צ�������

				bBufPalletRelease = m_pBufferPallet.m_bIsValid?TRUE:FALSE;

				m_pBufferPallet.m_bIsValid?(m_pTempPallet[m_nWorkIndex] = m_pBufferPallet)
					:(m_pBufferPallet = m_pTempPallet[m_nWorkIndex]);

				bBufPalletRelease?m_pBufferPallet.Release():m_pTempPallet[m_nWorkIndex].Release();

				m_nNextAutoStep =AUTO_ROBOT_TO_BUF_WAIT_POS_CHECK;
				SaveStepInfo(WRITE_TYPE_AUTO_STEP|WRITE_TYPE_BATTERY);
			}
			break;
		}

		//�����˵�����ȴ�λ
	case AUTO_ROBOT_TO_BUF_WAIT_POS_CHECK:
		{
			m_strMsg = m_bLangulageEng?_T("Robot Move To Safe Loc"):_T("�����˵�����ȴ�λ");

			ASSERT(0 <= m_nWorkIndex && m_nWorkIndex <= 1);
			int nLoc = ((m_nWorkIndex == 0)?(ROBOT_OFFLOAD_BUFF_WAIT_POS1):(ROBOT_OFFLOAD_BUFF_WAIT_POS2));

			if (IsNoHardware() || !m_bEnableRobot || m_pRobotClient.SendToDeviceAndWait(nLoc))
			{
				if(m_bEnableRobot)
				{
					m_nCurRobotLoc = nLoc;
				}
				/*	if (m_pBufferPallet.m_bIsValid)
				{
				m_bBufferMes = TRUE;
				}*/
				m_nNextAutoStep = AUTO_CHECK_WITHOUT_PRODUCT_BUF;
			}
			else
			{
				//�������ƶ���ʱ
				ShowMessage(MSG_ROBOT_MOVE_TIMEOUT);
			}
			break;
		}

		//����Ƿ�����
	case AUTO_CHECK_WITHOUT_PRODUCT_BUF:
		{
			m_strMsg = m_bLangulageEng?_T("check FingerCyl Status"):_T("����ȡ/������ɣ�����צ�ͻ�����״̬");
			ASSERT(0 <= m_nWorkIndex && m_nWorkIndex <= 1);
			if (IsNoHardware() || m_bDryRun || (CheckHasVaild(m_nWorkIndex, m_pTempPallet[m_nWorkIndex].m_bIsValid) && CheckHasVaild(m_nWorkIndex + 2, m_pBufferPallet.m_bIsValid)))                         
			{
				m_nNextAutoStep = AUTO_JUDGE_BATTERY_ALL_PUT_FINISH;
			}
			break;
		}

		//�ж����е�ط����
	case AUTO_JUDGE_BATTERY_ALL_PUT_FINISH:
		{
			m_strMsg = m_bLangulageEng?_T("Judeg Put Ok All Finish"):_T("�ж����е�ط����");
			if(m_bDryRun)
			{
				m_nNextAutoStep = AUTO_WORK_END;
			}
			else
			{
				m_nNextAutoStep = AUTO_JUDGE_HAVE_NG;

			}
			
			break;
		}
		//�������
	case AUTO_WORK_END:
		{
			m_strMsg = m_bLangulageEng?_T("Work end"):_T("�������"); 
			m_nNextAutoStep = AUTO_IF_CLEAR_MATERIAL;
			break;
		}
	default:
		{
			ASSERT(0);
			break;
		}
	}
}

BOOL CRunProcessRobot::InitializeConfig(CString strProcessModule)
{
	if (!CRunProcess::InitializeConfig(strProcessModule))
	{
		return FALSE;
	}
	AfxBeginThread(RobotThread,this);
	return TRUE;
}

void CRunProcessRobot::GetHardwareConfig(CArray<int, int> *pInputs, CArray<int, int> *pOutputs, CArray<int, int> *pMotors)
{
	CRunProcess::GetHardwareConfig(pInputs, pOutputs, pMotors);
}
/****************************************************************************************
����������������ܷ��ƶ�
���������
�������أ�TRUE:ִ�гɹ���FALSE:ִ��ʧ��
*****************************************************************************************/
BOOL  CRunProcessRobot::CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{
	return TRUE;
}

BOOL  CRunProcessRobot::CheckOutputCanActive(COutput *pOutput)
{
#ifdef _TEST_
	return TRUE;
#endif
	ASSERT(m_OBufferCylPush > -1 && m_OBufferCylReturn > -1);

	if (pOutput == OUTPUTS[m_OBufferCylPush] || pOutput == OUTPUTS[m_OBufferCylReturn])
	{
		if (!CheckRobotSafe())
		{
			BLMessageBox(_T("�����˲��ڰ�ȫλ���޸����ײ����ƶ�"), MB_ICONWARNING);
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CRunProcessRobot::SaveParameter()
{
	CRunProcess::SaveParameter();

	if (!IniWriteInt(m_strProcess, _T("RobotPort"), m_nRobotPort, GetAbsolutePathName(PARAMETER_CFG)))
		return FALSE;

	if (!IniWriteString(m_strProcess, _T("RobotIp"), m_strRobotIp, GetAbsolutePathName(PARAMETER_CFG)))
		return FALSE;

	if (!IniWriteBoolean(m_strProcess, _T("EnableRobot"), m_bEnableRobot, GetAbsolutePathName(PARAMETER_CFG)))
		return FALSE;

	if (!IniWriteBoolean(m_strProcess, _T("���ÿͻ���"), m_bShieldServer, GetAbsolutePathName(PARAMETER_CFG)))
		return FALSE;

	if (!IniWriteInt(m_strProcess, _T("�������˿�"), m_nServerPort, GetAbsolutePathName(PARAMETER_CFG)))
		return FALSE;

	if (!IniWriteString(m_strProcess, _T("��������IP"), m_strServerIp, GetAbsolutePathName(PARAMETER_CFG)))
		return FALSE;

	return TRUE;
}

BOOL CRunProcessRobot::ReadRunParameter()
{
	CRunProcess:: ReadRunParameter();

	m_bEnableRobot = IniReadBoolean(m_strProcess, _T("EnableRobot"), FALSE, GetAbsolutePathName(PARAMETER_CFG));
	m_nRobotPort = IniReadInt(m_strProcess, _T("RobotPort"), 8880, GetAbsolutePathName(PARAMETER_CFG));
	m_strRobotIp = IniReadString(m_strProcess, _T("RobotIp"), _T("192.168.1.5"), GetAbsolutePathName(PARAMETER_CFG));

	m_bShieldServer = IniReadBoolean(m_strProcess, _T("���ÿͻ���"),TRUE, GetAbsolutePathName(PARAMETER_CFG));
	m_nServerPort = IniReadInt(m_strProcess, _T("�������˿�"), 6668, GetAbsolutePathName(PARAMETER_CFG));
	m_strServerIp = IniReadString(m_strProcess, _T("��������IP"), _T("192.168.1.5"), GetAbsolutePathName(PARAMETER_CFG));

	return TRUE;
}

/****************************************************************************************
������������צ���׶���
���������nIndex:-1�������м�צ���ף�Ϊ0-1ʱ�ֱ�����צ����1��2
bClose:TRUE:��צ���׼н� FALSE:��צ�����ɿ�
�������أ�TRUE:ִ�гɹ���FALSE:ִ��ʧ��
*****************************************************************************************/
BOOL CRunProcessRobot::FingerCylClose(int nIndex, BOOL bClose)
{
	ASSERT(-1 <= nIndex && nIndex < 2);
	for (int i = 0; i < 2; i++)
	{
		ASSERT(m_IFingerCylClose[i] > -1 && m_IFingerCylOpen[i] > -1 && m_OFingerCylClose[i] > -1 && m_OFingerCylOpen[i] > -1);
	}

	BOOL bRes = FALSE;

	if(nIndex == -1)
	{
		bClose?OUTPUTS[m_OFingerCylClose[0]]->On():OUTPUTS[m_OFingerCylClose[0]]->Off();
		bClose?OUTPUTS[m_OFingerCylClose[1]]->On():OUTPUTS[m_OFingerCylClose[1]]->Off();
		bClose?OUTPUTS[m_OFingerCylOpen[0]]->Off():OUTPUTS[m_OFingerCylOpen[0]]->On();
		bClose?OUTPUTS[m_OFingerCylOpen[1]]->Off():OUTPUTS[m_OFingerCylOpen[1]]->On();

		if (bClose && m_bDryRun)
		{
			Sleep(200);
			return TRUE;
		}

		if(WAIT_OK == WaitInput(INPUTS[m_IFingerCylClose[0]], bClose?ON:OFF, MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_IFingerCylClose[1]], bClose?ON:OFF, MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_IFingerCylOpen[0]], bClose?OFF:ON, MSG_SHOW_DEFAULT)
			&& WAIT_OK == WaitInput(INPUTS[m_IFingerCylOpen[1]], bClose?OFF:ON, MSG_SHOW_DEFAULT))
		{
			bRes = TRUE;
		}
	}
	else
	{
		bClose?OUTPUTS[m_OFingerCylClose[nIndex]]->On():OUTPUTS[m_OFingerCylClose[nIndex]]->Off();
		bClose?OUTPUTS[m_OFingerCylOpen[nIndex]]->Off():OUTPUTS[m_OFingerCylOpen[nIndex]]->On();

		if(WAIT_OK == WaitInput(INPUTS[m_IFingerCylClose[nIndex]], bClose?ON:OFF, MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_IFingerCylOpen[nIndex]], bClose?OFF:ON, MSG_SHOW_DEFAULT))
		{
			bRes = TRUE;
		}
	}
	return bRes;

}

/****************************************************************************************
�����������������׶���
���������
bPush:TRUE:��ת�����Ƴ� FALSE:��ת���׻���
�������أ�TRUE:ִ�гɹ���FALSE:ִ��ʧ��
*****************************************************************************************/
BOOL CRunProcessRobot::BufferCylPush(BOOL bPush)
{
	ASSERT(m_IBufferCylPush > -1 && m_IBufferCylReturn > -1 && m_OBufferCylPush > -1 && m_OBufferCylReturn > -1);

	BOOL bRes = FALSE;
	bPush?(OUTPUTS[m_OBufferCylPush]->On()):(OUTPUTS[m_OBufferCylPush]->Off());
	bPush?(OUTPUTS[m_OBufferCylReturn]->Off()):(OUTPUTS[m_OBufferCylReturn]->On());

	if(WAIT_OK == WaitInput(INPUTS[m_IBufferCylPush], bPush?ON:OFF, MSG_SHOW_DEFAULT) 
		&& WAIT_OK == WaitInput(INPUTS[m_IBufferCylReturn], bPush?OFF:ON, MSG_SHOW_DEFAULT))
	{
		bRes = TRUE;
	}
	return bRes;
}

/****************************************************************************************
��������������Ƿ��в�Ʒ
���������nIndex:-1�������й�λ��Ϊ0-1ʱ�ֱ�����צ1��2   Ϊ2-3ʱ�ֱ�Ϊ�������ϼ�� 
bHave:��TRUE:�������    FALSE:���û����
�������أ�TRUE:��bHaveһ�£�FALSE:��bHave�෴
*****************************************************************************************/
BOOL CRunProcessRobot::CheckHasVaild(int nIndex, BOOL bHave)
{
	ASSERT(-1 <= nIndex && nIndex <= 3);
	ASSERT(m_IHasCore[0] > -1 && m_IHasCore[1] > -1 && m_IHasCore[2] > -1 && m_IHasCore[3] > -1);

	BOOL bRes = FALSE;

	if(nIndex < 0)
	{
		bRes = TRUE;
		for (int i = 0; i < 4; i++)
		{
			if(WAIT_OK != CheckInput(INPUTS[m_IHasCore[i]], bHave?ON:OFF, MSG_SHOW_DEFAULT))
			{
				bRes = FALSE;
			}
		}
	}
	else
	{
		if(WAIT_OK == CheckInput(INPUTS[m_IHasCore[nIndex]], bHave?ON:OFF, MSG_SHOW_DEFAULT))
		{
			bRes = TRUE;
		}
	}
	return bRes;
}

/****************************************************************************************
�������������������ˣ�����ֹͣ�źţ������ǵ����ٵ�����Ч������������֮ǰ�����ȵ���ֹͣ�ź�
���������
�������أ�TRUE:ִ�гɹ���FALSE:ִ��ʧ��
*****************************************************************************************/
BOOL CRunProcessRobot::RobotStop()
{
	BOOL bRes = FALSE;
	if(m_ORobotEmgStop > -1 && m_ORobotStop > -1)
	{
		OUTPUTS[m_ORobotEmgStop]->On();
		OUTPUTS[m_ORobotStop]->Off();
		Sleep(300);
		OUTPUTS[m_ORobotStop]->On();
		bRes = TRUE;
	}
	return bRes;
}

/****************************************************************************************
������������λ�����˱���������ͣ������
���������
�������أ�TRUE:ִ�гɹ���FALSE:ִ��ʧ��
*****************************************************************************************/
BOOL CRunProcessRobot::RobotAlarmReset()
{
	BOOL bRes = FALSE;
	if(m_ORobotAlarmReset > -1)
	{
		OUTPUTS[m_ORobotAlarmReset]->Off();
		Sleep(300);
		OUTPUTS[m_ORobotAlarmReset]->On();
		Sleep(300);
		OUTPUTS[m_ORobotAlarmReset]->Off();
		bRes = TRUE;
	}
	return bRes;
}

/****************************************************************************************
������������λ�����˳���ʱ�����˳����ͷ��ʼִ�У�����λ�����ӻ�������m_ORobotStop��DI06�����»�����ֹͣʱ�������˳�����ͣ�ڵĴ����д���ʼ
���������
�������أ�TRUE:ִ�гɹ���FALSE:ִ��ʧ��
*****************************************************************************************/
BOOL CRunProcessRobot::RobotCodeReset()
{
	BOOL bRes = FALSE;
	if(m_ORobotCodeReset > -1)
	{
		OUTPUTS[m_ORobotCodeReset]->Off();
		Sleep(300);
		OUTPUTS[m_ORobotCodeReset]->On();
		Sleep(300);
		OUTPUTS[m_ORobotCodeReset]->Off();
		bRes = TRUE;
	}
	return bRes;
}

/****************************************************************************************
���������������˳������У���ʼ������λ������
���������
�������أ�TRUE:ִ�гɹ���FALSE:ִ��ʧ��
*****************************************************************************************/
BOOL CRunProcessRobot::RobotCodeRun()
{
	BOOL bRes = FALSE;
	if(m_ORobotCodeRun > -1)
	{
		OUTPUTS[m_ORobotCodeRun]->Off();
		Sleep(300);
		OUTPUTS[m_ORobotCodeRun]->On();
		bRes = TRUE;
	}
	return bRes;
}

void CRunProcessRobot::GetLocalPalletData(CPallet* pallet,CString & strResult)
{
	if (pallet[0].m_strBarCode.IsEmpty() && pallet[1].m_strBarCode.IsEmpty())  //��������ǿյ�
	{
		CString m_pallet = _T("");
		CTime TIME = CTime::GetCurrentTime();
		m_pallet = TIME.Format(_T("%X"));
		pallet[0].m_strBarCode = m_pallet;
		pallet[1].m_strBarCode = m_pallet;
		strResult.Format(_T("%s,%s"),pallet[0].m_strBarCode,pallet[1].m_strBarCode);
	}
	else
	{
		strResult.Format(_T("%s,%s"),m_pTempPallet[0].m_strBarCode,m_pTempPallet[1].m_strBarCode);
	}
	MACHINE.m_strShowSend = strResult;  //���Խ�����ʾ����

}

/****************************************************************************************
������������������Z���Ƿ��ڰ�ȫλ ֻ��ⲻ�ƶ�
���������
�������أ�TRUE:ִ�гɹ���FALSE:ִ��ʧ��
*****************************************************************************************/
BOOL CRunProcessRobot::CheckRobotSafe()
{
	if (!m_bEnableRobot || g_robotcmd[m_nCurRobotLoc].nCmdLevel != 1)
	{
		return TRUE;
	}
	return FALSE;
}

/****************************************************************************************
������������ȡ����
���������
�������أ�TRUE:ִ�гɹ���FALSE:ִ��ʧ��
*****************************************************************************************/
void CRunProcessRobot::ReadStepInfo()				//��ȡ������������
{
	CString strPath = GetAbsolutePathName(MODULE_RUN_FOLDER) + m_strName + _T(".cfg");

	m_nNextAutoStep = IniReadInt(_T("Data"), _T("m_nNextAutoStep"), 0, strPath);
	m_nWorkIndex = IniReadInt(_T("Data"), _T("m_nWorkIndex"), -1, strPath);

	m_pBufferPallet.ReadCfg(_T("BufferCylPallet"), strPath);
	m_pTempPallet[0].ReadCfg(_T("TempPallet0"), strPath);
	m_pTempPallet[1].ReadCfg(_T("TempPallet1"), strPath);

	return;
}

/****************************************************************************************
������������������
���������
�������أ�TRUE:ִ�гɹ���FALSE:ִ��ʧ��
*****************************************************************************************/
BOOL CRunProcessRobot::SaveStepInfo(int nWriteType)   	//���������������
{
	CString strPath = GetAbsolutePathName(MODULE_RUN_FOLDER) + m_strName + _T(".cfg");

	if(nWriteType & WRITE_TYPE_AUTO_STEP)
	{
		IniWriteInt(_T("Data"), _T("m_nNextAutoStep"), m_nNextAutoStep, strPath);
	}

	if(nWriteType & WRITE_TYPE_VARIABLES)
	{
		IniWriteInt(_T("Data"), _T("m_nWorkIndex"), m_nWorkIndex, strPath);
	}

	if(nWriteType & WRITE_TYPE_BATTERY)
	{
		m_pBufferPallet.WriteCfg(_T("BufferCylPallet"), strPath);
		m_pTempPallet[0].WriteCfg(_T("TempPallet0"), strPath);
		m_pTempPallet[1].WriteCfg(_T("TempPallet1"), strPath);
	}
	return TRUE;
}

//�������������ʵ�ʼ������һ����,nCheckResultΪ�������0��ץû���ϣ�1��ץ�����ϣ�2����ס1���ϼ�ץ2û�ϣ�3����ס1���ϼ�ץ2����
BOOL CRunProcessRobot::CheckStartWithPalletDataSame()
{
	CString strMsg = _T("");
	for(int i=0;i<2;i++)
	{
		if(m_pTempPallet[i].m_bIsValid )
		{
			if((m_IHasCore[i] > -1 && INPUTS[m_IHasCore[i]]->IsOff()))
			{
				strMsg.Format(_T("��������,���������ϸ�Ӧ��ʵ�����ݲ�һ��,��������,��Ӧ����,��ȷ��"));
				ShowMessageEX(strMsg);
				strMsg.Format(_T("ȷ�ϻ��������������ϵ�YES�����ϵ�NO"));
				if(BLMessageBox(strMsg,MB_YESNO) == IDYES)
				{
					m_pTempPallet[i].m_bIsValid = TRUE;
				}
				else
				{
					m_pTempPallet[i].m_bIsValid = FALSE;
				}
				return FALSE;
			}
		}
		else
		{
			if((m_IHasCore[i] > -1 && INPUTS[m_IHasCore[i]]->IsOn()))
			{
				strMsg.Format(_T("��������,���������ϸ�Ӧ��ʵ�����ݲ�һ��,��������,��Ӧ����,��ȷ��"));
				ShowMessageEX(strMsg);
				strMsg.Format(_T("ȷ�ϻ��������������ϵ�YES�����ϵ�NO"));
				if(BLMessageBox(strMsg,MB_YESNO) == IDYES)
				{
					m_pTempPallet[i].m_bIsValid = TRUE;
				}
				else
				{
					m_pTempPallet[i].m_bIsValid = FALSE;
				}
				return FALSE;
			}
		}
	}

	if(!m_pTempPallet[0].m_bIsValid && !m_pTempPallet[1].m_bIsValid)
	{
		m_nStartWithPalletData = 0;
	}
	else if(m_pTempPallet[0].m_bIsValid && m_pTempPallet[1].m_bIsValid)
	{
		m_nStartWithPalletData = 1;
	}
	else if(m_pTempPallet[0].m_bIsValid && !m_pTempPallet[1].m_bIsValid)
	{
		m_nStartWithPalletData = 2;
	}
	else if(!m_pTempPallet[0].m_bIsValid && m_pTempPallet[1].m_bIsValid)
	{
		m_nStartWithPalletData = 3;
	}
	return TRUE;
}


BOOL CRunProcessRobot::CheckStartWithPalletDataSameForBuffer()
{
	CString strMsg = _T("");
	if(m_pBufferPallet.m_bIsValid)
	{
		if(m_IHasCore[2] > -1 && INPUTS[m_IHasCore[2]]->IsOff()
			&& m_IHasCore[3] > -1 && INPUTS[m_IHasCore[3]]->IsOff())
		{
			strMsg.Format(_T("��������,�����˻������ϸ�Ӧ��ʵ�����ݲ�һ��,��������,��Ӧ����,��ȷ��"));
			ShowMessageEX(strMsg);
			strMsg.Format(_T("ȷ�ϻ����˻������������ϵ�YES�����ϵ�NO"));
			if(BLMessageBox(strMsg,MB_YESNO) == IDYES)
			{
				m_pBufferPallet.m_bIsValid = TRUE;
			}
			else
			{
				m_pBufferPallet.m_bIsValid = FALSE;
			}
			return FALSE;
		}
	}
	else
	{
		if(m_IHasCore[2] > -1 && INPUTS[m_IHasCore[2]]->IsOn()
			|| m_IHasCore[3] > -1 && INPUTS[m_IHasCore[3]]->IsOn())
		{
			strMsg.Format(_T("��������,�����˻������ϸ�Ӧ��ʵ�����ݲ�һ��,��������,��Ӧ����,��ȷ��"));
			ShowMessageEX(strMsg);
			strMsg.Format(_T("ȷ�ϻ����˻������������ϵ�YES�����ϵ�NO"));
			if(BLMessageBox(strMsg,MB_YESNO) == IDYES)
			{
				m_pBufferPallet.m_bIsValid = TRUE;
			}
			else
			{
				m_pBufferPallet.m_bIsValid = FALSE;
			}
			return FALSE;
		}
	}

	if (!m_pBufferPallet.m_bIsValid)
	{
		m_nBufferWithPalletData = 0;
	}
	else if (m_pBufferPallet.m_bIsValid)
	{
		m_nBufferWithPalletData = 1;
	}
	return TRUE;
}

/****************************************************************************************
�������������ӻ�����
���������
�������أ�TRUE:ִ�гɹ���FALSE:ִ��ʧ��
*****************************************************************************************/
BOOL CRunProcessRobot::ConnectRobot()
{
	BOOL bRes = FALSE;
	if(m_ORobotEmgStop > -1 && m_ORobotStop > -1 && m_ORobotAlarmReset > -1 
		&& m_ORobotCodeReset > -1 && m_ORobotCodeRun > -1)
	{
		OUTPUTS[m_ORobotEmgStop]->On();
		Sleep(100);
		OUTPUTS[m_ORobotStop]->On();
		Sleep(100);
		OUTPUTS[m_ORobotAlarmReset]->On();
		Sleep(300);
		OUTPUTS[m_ORobotAlarmReset]->Off();
		Sleep(100);
		OUTPUTS[m_ORobotCodeReset]->On();
		Sleep(300);
		OUTPUTS[m_ORobotCodeReset]->Off();
		Sleep(100);
		OUTPUTS[m_ORobotCodeRun]->On();
		bRes = TRUE;
	}
	return bRes;
}

/****************************************************************************************
�����������Ͽ����ӻ�����
���������
�������أ�TRUE:ִ�гɹ���FALSE:ִ��ʧ��
*****************************************************************************************/
BOOL CRunProcessRobot::DisconnectRobot()
{
	BOOL bRes = FALSE;
	if(m_ORobotStop > -1 && m_ORobotEmgStop > -1 && m_ORobotCodeRun > -1)
	{
		OUTPUTS[m_ORobotStop]->Off();
		OUTPUTS[m_ORobotEmgStop]->Off();
		OUTPUTS[m_ORobotCodeRun]->Off();
		bRes = TRUE;
	}
	return bRes;
}

/****************************************************************************************
������������ѹ�ϼ�ͣ
���������
�������أ�TRUE:��צ��ȫ��FALSE:��צѹ��
*****************************************************************************************/
void CRunProcessRobot::EmgStop()
{
	if(MACHINE.m_pRunCtrl == NULL)
	{		
		return;
	}
	int nStatues = MACHINE.m_pRunCtrl->GetMcState();
	if (nStatues == MC_RUNNING)
	{
		if (m_IZCheckAbnormal > -1 && INPUTS[m_IZCheckAbnormal]->IsOff())
		{
			if (m_ORobotEmgStop > -1)
			{
				OUTPUTS[m_ORobotEmgStop]->Off();
			}

			MACHINE.m_pRunCtrl->Stop();
			ShowMessage(ROBOT_PRESS_ALARM);
			return ;
		}
		else
		{
			if (m_ORobotEmgStop > -1 && OUTPUTS[m_ORobotEmgStop]->IsOff())
			{
				OUTPUTS[m_ORobotEmgStop]->On();
			}
		}
	}
	return;
}

/****************************************************************************************
���������� ���ͻ������Ƿ��ڰ�ȫλ�ź�
��������� bFinish:TRUE:��ȫ�ź�  FALSE:����ȫ�ź�
�������أ�TRUE:ִ�гɹ���FALSE:ִ��ʧ��
*****************************************************************************************/


BOOL  CRunProcessRobot::SendSafeSignal(BOOL bSafe)
{
	if (m_ORobotPutInPosSignal[0] < 0 || m_ORobotPutInPosSignal[1] < 0)
	{
		return FALSE;
	}
	
	if (bSafe)
	{
		if (OUTPUTS[m_ORobotPutInPosSignal[0]]->On() && OUTPUTS[m_ORobotPutInPosSignal[1]]->On())
		{
			return TRUE;
		}
	}
	else
	{
		if (OUTPUTS[m_ORobotPutInPosSignal[0]]->Off() && OUTPUTS[m_ORobotPutInPosSignal[1]]->Off())  //����ȫ�ź�
		{
			return TRUE;
		}
	}

	return FALSE;
}

/****************************************************************************************
���������� ���ͷ�������ź�
��������� bFinish:TRUE:�������   FALSE:��λ�����ź�
�������أ�TRUE:ִ�гɹ���FALSE:ִ��ʧ��
*****************************************************************************************/

BOOL CRunProcessRobot::SendPlaceFinishSignal(BOOL bFinish)
{
	if (m_ORobotPutFinishSignal[0] < 0 && m_ORobotPutFinishSignal[1] < 0)
	{
		return FALSE;
	}

	if (bFinish)
	{
		if (OUTPUTS[m_ORobotPutFinishSignal[0]]->On() && OUTPUTS[m_ORobotPutFinishSignal[1]]->On())
		{
			return TRUE;
		}
	}
	else
	{
		if (OUTPUTS[m_ORobotPutFinishSignal[0]]->Off() && OUTPUTS[m_ORobotPutFinishSignal[1]]->Off())
		{
			return TRUE;
		}
	}

	return FALSE;

}

/****************************************************************************************
���������� ������Ƿ�Ҫ��
��������� bNeed:TRUE:���Ҫ��   FALSE:��鸴λҪ���ź�
�������أ�TRUE:ִ�гɹ���FALSE:ִ��ʧ��
*****************************************************************************************/

BOOL CRunProcessRobot::CheckNeedBatteryEvent(BOOL bNeed)
{
	if (m_ITailFingerSignal[0] < 0 || m_ITailFingerSignal[1] < 0)
	{
		return FALSE;
	}

	if (bNeed)
	{
		if (INPUTS[m_ITailFingerSignal[0]]->IsOn() && INPUTS[m_ITailFingerSignal[1]]->IsOn() &&INPUTS[m_INormalRunSingle]->IsOn())
		{
			return TRUE;
		}
	}
	else
	{
		if (INPUTS[m_ITailFingerSignal[0]]->IsOff() && INPUTS[m_ITailFingerSignal[1]]->IsOff() && INPUTS[m_INormalRunSingle]->IsOn())
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CRunProcessRobot::CheckBackSafe()
{
	return FALSE;
}

void CRunProcessRobot::NomalRunEvent()
{
	if (!MACHINE.MachineExit)
	{
		int nStatus =  MACHINE.m_pRunCtrl->GetMcState();
		if (MC_INITIALIZING == nStatus || MC_RUNNING == nStatus )
		{
			if (m_ONormalRunSingle > -1 && OUTPUTS[m_ONormalRunSingle]->IsOff()) //��������������ź�
			{
				OUTPUTS[m_ONormalRunSingle]->On();
			}
		}
		else if(m_ONormalRunSingle > -1 && OUTPUTS[m_ONormalRunSingle]->IsOn())
		{
			OUTPUTS[m_ONormalRunSingle]->Off();
		}
	}
}