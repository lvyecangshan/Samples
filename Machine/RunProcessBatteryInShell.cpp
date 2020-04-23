/****************************************************************************************
�ļ�����RunProcessBatteryInShell.cpp
���������������ǹ�λ
��ʷ�޸ļ�¼��
*****************************************************************************************/
#include "StdAfx.h"
#include "RunProcessBatteryInShell.h"

CRunProcessBatteryInShell::CRunProcessBatteryInShell(int nID) :
CRunProcess(nID)
{
	PowerUpRestart();
	CString strKey = _T("");
	for(int i = 0; i < 2; i++)
	{
		strKey.Format(_T("IColumnCylOpen%d"), i);
		m_InputMap.insert(pair<int*, CString>(&m_IColumnCylOpen[i], strKey));

		strKey.Format(_T("IColumnCylClose%d"), i);
		m_InputMap.insert(pair<int*, CString>(&m_IColumnCylClose[i], strKey));

		strKey.Format(_T("ILocateShellPush%d"), i);
		m_InputMap.insert(pair<int*, CString>(&m_ILocateShellPush[i], strKey));

		strKey.Format(_T("ILocateShellBack%d"), i);
		m_InputMap.insert(pair<int*, CString>(&m_ILocateShellBack[i], strKey));

		strKey.Format(_T("IVacumCylPush%d"), i);
		m_InputMap.insert(pair<int*, CString>(&m_IVacumCylPush[i], strKey));

		strKey.Format(_T("IVacumCylBack%d"), i);
		m_InputMap.insert(pair<int*, CString>(&m_IVacumCylBack[i], strKey));

		strKey.Format(_T("OVacumCylPush%d"), i);
		m_OutputMap.insert(pair<int*, CString>(&m_OVacumCylPush[i], strKey));

		strKey.Format(_T("ICheckVacum%d"), i);
		m_InputMap.insert(pair<int*, CString>(&m_ICheckVacum[i], strKey));
	}

	m_InputMap.insert(pair<int*, CString>(&m_IHaveCore, _T("IHaveCore")));

	m_InputMap.insert(pair<int*, CString>(&m_ILiftCylUp, _T("ILiftCylUp")));
	m_InputMap.insert(pair<int*, CString>(&m_ILiftCylDown, _T("ILiftCylDown")));
	m_OutputMap.insert(pair<int*, CString>(&m_OLiftCylUp, _T("OLiftCylUp")));
	m_OutputMap.insert(pair<int*, CString>(&m_OLiftCylDown, _T("OLiftCylDown")));

	m_InputMap.insert(pair<int*, CString>(&m_ISendOneCylPush, _T("ISendOneCylPush")));
	m_InputMap.insert(pair<int*, CString>(&m_ISendOneCylBack, _T("ISendOneCylBack")));
	m_OutputMap.insert(pair<int*, CString>(&m_OSendOneCylPush, _T("OSendOneCylPush")));
	m_OutputMap.insert(pair<int*, CString>(&m_OSendOneCylBack, _T("OSendOneCylBack")));

	m_InputMap.insert(pair<int*, CString>(&m_ISendTwoCylPush, _T("ISendTwoCylPush")));
	m_InputMap.insert(pair<int*, CString>(&m_ISendTwoCylBack, _T("ISendTwoCylBack")));
	m_OutputMap.insert(pair<int*, CString>(&m_OSendTwoCylPush, _T("OSendTwoCylPush")));
	m_OutputMap.insert(pair<int*, CString>(&m_OSendTwoCylBack, _T("OSendTwoCylBack")));

	m_OutputMap.insert(pair<int*, CString>(&m_OVacumCyl[0], _T("OVacumCyl0")));
	m_OutputMap.insert(pair<int*, CString>(&m_OVacumCyl[1], _T("OVacumCyl1")));
	m_OutputMap.insert(pair<int*, CString>(&m_OBreakVacum, _T("OBreakVacum")));

	// �ǿڶ�λ
	m_OutputMap.insert(pair<int*, CString>(&m_OLocateShellPush, _T("OLocateShellPush")));
	m_OutputMap.insert(pair<int*, CString>(&m_OLocateShellBack, _T("OLocateShellBack")));

	// ���Ƕ�λ
	m_OutputMap.insert(pair<int*, CString>(&m_OColumnCylClose, _T("OColumnCylClose")));
	m_OutputMap.insert(pair<int*, CString>(&m_OColumnCylOpen, _T("OColumnCylOpen")));

	//�ײ����
	m_InputMap.insert(pair<int*, CString>(&m_ITailVacum, _T("ITailVacum")));
	m_OutputMap.insert(pair<int*, CString>(&m_OTailVacum, _T("OTailVacum")));
}

CRunProcessBatteryInShell::~CRunProcessBatteryInShell(void)
{	
}

void CRunProcessBatteryInShell::PowerUpRestart()
{
	CRunProcess::PowerUpRestart();
	m_strMsg = m_bLangulageEng?_T("Ready"):_T("׼����");
	m_bDataJudgeFinish= FALSE;
	m_bResetJudge = FALSE;

	m_bShellVaild = FALSE;
	m_bCylClosed = FALSE;
	m_bRequirePallet = FALSE;
	m_bShellReady = FALSE;

	m_nRetryTimes = 0;
	m_nStartWithPalletData = -1;

	m_nNextPowerUpStep = PU_WAIT_MODULE_HOME;
	m_nNextAutoStep = AUTO_SEND_SHELL_CORE_REQUIRE;
}

void CRunProcessBatteryInShell::PowerUpOperation(void)
{
	if (!IsModuleEnable())
	{
		CRun::PowerUpOperation();
		m_strMsg = m_bLangulageEng?_T("Moudle UnEnable"):_T("ģ�����");
		return;
	}

	switch(m_nNextPowerUpStep)
	{
	case PU_WAIT_MODULE_HOME:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Modules Home Finish"):_T("�ȴ�ģ�鸴λ���");
			ASSERT(0 <= m_nGroupID && m_nGroupID <= 1);
			if((MACHINE.m_pCarryShellPnp != NULL && (MACHINE.m_pCarryShellPnp->m_bHomeMoveSafePos || !MACHINE.m_pCarryShellPnp->IsModuleEnable()))
				&& (MACHINE.m_pBatteryOffloadPnp[m_nGroupID] != NULL && (MACHINE.m_pBatteryOffloadPnp[m_nGroupID]->m_bHomeMoveSafePos || !MACHINE.m_pBatteryOffloadPnp[m_nGroupID]->IsModuleEnable())))
			{
				m_nNextPowerUpStep = PU_JUDGE_IF_WITH_PALLET;
			}
			break;
		}

		//�ж��Ƿ����
	case PU_JUDGE_IF_WITH_PALLET:
		{
			m_strMsg = m_bLangulageEng?_T("Judge If With Pallet"):_T("�ж��Ƿ����");

			if(MACHINE.m_bReCoverMachine)
			{
				ReadStepInfo();
				m_nNextPowerUpStep = PU_WAIT_BATTER_CORE_SIGNAL_WITH_PALLET;
			}
			else
			{
				m_nNextPowerUpStep = PU_CHECK_WITHOUT_SHELL;
			}
			break;
		}

		//������޿�
	case PU_CHECK_WITHOUT_SHELL:
		{
			m_strMsg = m_bLangulageEng?_T("Check Without Shell"):_T("����޿�");

			if (IsNoHardware() || ((m_IHaveCore > -1 && CheckInput(INPUTS[m_IHaveCore], OFF))))
			{
				m_nNextAutoStep = AUTO_SEND_SHELL_CORE_REQUIRE;
				m_nNextPowerUpStep = PU_CLUMN_CYL_OPEN;
			}
			break;
		}
		//�������״�
	case PU_CLUMN_CYL_OPEN:
		{
			m_strMsg = m_bLangulageEng?_T("Clumn Cyl Open"):_T("�������״�");

			if (::IsNoHardware() || ColumnCylClose( FALSE))
			{
				m_nNextPowerUpStep = PU_LIFT_CYL_DOWN;
			}
			break;
		}

		//���������½�
	case PU_LIFT_CYL_DOWN:
		{
			m_strMsg = m_bLangulageEng?_T("Lift Cyl Down"):_T("���������½�");

			if (::IsNoHardware() || DownLiftCylUp(FALSE))
			{
				m_nNextPowerUpStep = PU_SEND_PUSH_CYL_RETURN;
			}
			break;
		}		

		//��������׻���
	case PU_SEND_PUSH_CYL_RETURN:
		{
			m_strMsg = m_bLangulageEng?_T("Send Push Cyl Return"):_T("�������׻���");

			if(::IsNoHardware() || (SendOneCylPush(FALSE) && SendTwoCylPush(FALSE)))
			{
				m_nNextPowerUpStep = PU_CLOSE_VACUM;
			}
			break;
		}

		//�ر����
	case PU_CLOSE_VACUM:
		{
			m_strMsg = m_bLangulageEng?_T("Close Vacum"):_T("�ر����");

			VacumOpenOrCheck(FALSE,FALSE); //�ر����

			if(m_OBreakVacum > -1)
			{
				OUTPUTS[m_OBreakVacum]->Off();
			}

			if(m_OTailVacum > -1)
			{
				OUTPUTS[m_OTailVacum]->Off();
			}
			m_nNextPowerUpStep = PU_VACUM_CYL_UP;
			break;
		}

		//���ǿ����״�
	case PU_VACUM_CYL_UP:
		{
			m_strMsg = m_bLangulageEng?_T("Vacum Cyl Up"):_T("���ǿ����״�");

			if ((m_ICheckVacum[0] < 0 || WAIT_OK == WaitInput(INPUTS[m_ICheckVacum[0]], OFF, MSG_SHOW_DEFAULT))
				&& (m_ICheckVacum[1] < 0 || WAIT_OK == WaitInput(INPUTS[m_ICheckVacum[1]], OFF, MSG_SHOW_DEFAULT)))
			{
				if (VacumCylPush(FALSE))
				{
					m_nNextPowerUpStep = PU_FINISH;
				}
			}
			break;
		}

	case PU_WAIT_BATTER_CORE_SIGNAL_WITH_PALLET:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Modules Home Finish"):_T("�ȴ��Ƶ�оģ�鸴λ���");
			ASSERT(0 <= m_nGroupID && m_nGroupID <= 1);
			if((MACHINE.m_pBatteryCore[0] != NULL && (MACHINE.m_pBatteryCore[0]->m_bPowerUpFinish|| !MACHINE.m_pBatteryCore[0]->IsModuleEnable()))
				&& (MACHINE.m_pBatteryCore[1] != NULL && (MACHINE.m_pBatteryCore[1]->m_bPowerUpFinish|| !MACHINE.m_pBatteryCore[1]->IsModuleEnable())))
			{
				m_nNextPowerUpStep = PU_CHECK_PALLET_DATA;
			}
			break;
		}
		//�������������ʵ���Ƿ�һ��  //ʹ������ǹ�λͬʱ���ϻ�����
	case PU_CHECK_PALLET_DATA:
		{
			m_strMsg = m_bLangulageEng?_T("Move XY To Onload Pos"):_T("�������������ʵ���Ƿ�һ��");

			if(CheckStartWithPalletDataSame())
			{
				if(m_nStartWithPalletData == 0) //����
				{
					m_bShellVaild = FALSE;
					m_Pallet.Release();
					m_nNextAutoStep = AUTO_SEND_SHELL_CORE_REQUIRE;
					m_nNextPowerUpStep = PU_WAIT_JUDGE_FINISH;
				}
				else if(m_nStartWithPalletData == 1)  //ֻ�п�    //��о�뵽һ�룬��оģ���������о
				{
					m_nNextAutoStep = AUTO_SEND_ONE_PUSH_CYL_PUSH;
					m_nNextPowerUpStep = PU_WAIT_JUDGE_FINISH;
				}
				else if(m_nStartWithPalletData == 2) //�п��е�о
				{
					m_nNextAutoStep = AUTO_LIFT_CYL_DOWN;
					m_nNextPowerUpStep = PU_WAIT_JUDGE_FINISH;
				}
				m_bDataJudgeFinish = TRUE;
			}

			break;
		}

	case PU_WAIT_JUDGE_FINISH:
		{
			m_strMsg = m_bLangulageEng?_T("Check Without Shell"):_T("�ȴ������ж����");
			int nID = m_nGroupID == 0?1:0;
			if (m_bDataJudgeFinish && MACHINE.m_pBatteryInShell[nID]->m_bDataJudgeFinish)
			{
				m_bResetJudge = TRUE;
				
				m_nNextPowerUpStep = PU_SAME_STATION_DATA_PAIR;
			}
			break;
		}
		//���˫��λ�����Ƿ�һ��
	case PU_SAME_STATION_DATA_PAIR:
		{
			m_strMsg = m_bLangulageEng?_T("Move XY To Onload Pos"):_T("���˫��λ�����Ƿ�һ��");

			int nID = (m_nGroupID == 0)?1:0;
			ASSERT(MACHINE.m_pBatteryInShell[nID] != NULL);

			if (m_bResetJudge && MACHINE.m_pBatteryInShell[nID]->m_bResetJudge)
			{
				m_bDataJudgeFinish = FALSE;
			}
			else
			{
				break;
			}

			if(m_nStartWithPalletData == 0 && MACHINE.m_pBatteryInShell[nID]->m_nStartWithPalletData == 0
				|| m_nStartWithPalletData == 1 && MACHINE.m_pBatteryInShell[nID]->m_nStartWithPalletData == 1
				|| m_nStartWithPalletData == 2 && MACHINE.m_pBatteryInShell[nID]->m_nStartWithPalletData == 2)
			{
				m_nNextPowerUpStep = PU_FINISH;
			}
			else 
			{
				m_bShellVaild = FALSE;
				m_Pallet.Release();
				MACHINE.m_pBatteryCore[nID]->m_bShellVaild = FALSE;
				MACHINE.m_pBatteryCore[nID]->m_Pallet.Release();
				BLMessageBox(_T("������ǹ�λ���ݲ�һ�£�������ݣ����˹������о��ȷ��������ǹ�λ��Ʒ����״̬һ��"), MB_ICONWARNING);
				m_nNextPowerUpStep = PU_CHECK_PALLET_DATA;
			}

			break;
		}

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

void CRunProcessBatteryInShell::AutoOperation()
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
		if (m_nSaveLastStep == AUTO_SEND_SHELL_CORE_REQUIRE)
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
	case AUTO_SEND_SHELL_CORE_REQUIRE:
		{
			m_strMsg = m_bLangulageEng?_T("Send Need Core Require"):_T("����Ҫ���ź�");
			if(!m_bRequirePallet)
			{
				m_bRequirePallet = TRUE;
				m_nNextAutoStep = AUTO_START_EVENT;
				SaveStepInfo(WRITE_TYPE_VARIABLES);
			}
			break;
		}
		//�ȴ���ʼ�ź�
	case AUTO_START_EVENT:
		{
			m_strMsg = m_bLangulageEng?_T("Start Event"):_T("�ȴ���ʼ�ź�");

			if(!m_bRequirePallet || (MACHINE.m_pCarryShellPnp != NULL  && !MACHINE.m_pCarryShellPnp->IsModuleEnable()))
			{
				if (!MACHINE.m_pCarryShellPnp->IsModuleEnable())
				{
					m_bRequirePallet = FALSE;
				}
				m_nNextAutoStep = AUTO_CHECK_HAVE_PRODUCT;
			}
			break;
		}
		//�������
	case AUTO_CHECK_HAVE_PRODUCT:
		{
			m_strMsg = m_bLangulageEng?_T("Check Have Product"):_T("�������");

			if (IsNoHardware() || m_bDryRun || m_IHaveCore > -1 && WAIT_OK == WaitInput(INPUTS[m_IHaveCore], ON, MSG_SHOW_DEFAULT))
			{
				m_nNextAutoStep = AUTO_LIFT_CYL_UP;
			}
			break;
		}
		//������������
	case AUTO_LIFT_CYL_UP:
		{
			m_strMsg = m_bLangulageEng?_T("Lift Cyl Up"):_T("�������׶���");

			if(IsNoHardware() || DownLiftCylUp(TRUE))
			{
				m_nNextAutoStep = AUTO_SEND_TWICE_PUSH_CYL_PUSH;
			}
			break;
		}
		//���������Ƴ�
	case AUTO_SEND_TWICE_PUSH_CYL_PUSH:
		{
			m_strMsg = m_bLangulageEng?_T("Send Push Cyl Push"):_T("���������Ƴ�");

			if(IsNoHardware() || SendTwoCylPush(TRUE))
			{
				m_nNextAutoStep = AUTO_TAIL_VACUUM_OPEN;
			}
			break;
		}
		//�򿪿ǵײ����
	case AUTO_TAIL_VACUUM_OPEN:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("�򿪿ǵײ����");

			ASSERT(m_OTailVacum > -1 && m_ITailVacum > -1);
			OUTPUTS[m_OTailVacum]->On();
			//if (IsNoHardware() || m_bDryRun ||WAIT_OK == WaitInput(INPUTS[m_ITailVacum], ON, MSG_SHOW_DEFAULT))
			{
				m_nNextAutoStep = AUTO_CLUMMN_CYL_CLOSE;
			}
			break;
		}

		//�������׼н�
	case AUTO_CLUMMN_CYL_CLOSE:
		{
			m_strMsg = m_bLangulageEng?_T("Clumn Cyl Close"):_T("�Ƕ�λ���׼н�");
			if (IsNoHardware() || (ColumnCylClose(TRUE) && !m_bCylClosed))
			{
				m_bCylClosed = TRUE;
				m_bShellVaild = TRUE;
				m_nNextAutoStep = AUTO_WAIT_CARRYSHELL_FINISH;
				SaveStepInfo(WRITE_TYPE_BATTERY);
			}
			break;
		}
		//������������
	case AUTO_WAIT_CARRYSHELL_FINISH:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Carry Module Z axis Up"):_T("�ȴ�����ģ��Z������");

			if(!m_bCylClosed || (MACHINE.m_pCarryShellPnp != NULL  && !MACHINE.m_pCarryShellPnp->IsModuleEnable()))
			{
				m_nNextAutoStep = AUTO_SEND_ONE_PUSH_CYL_PUSH;
				SaveStepInfo(WRITE_TYPE_AUTO_STEP|WRITE_TYPE_VARIABLES);
			}
			break;
		}
		//����������Ƴ�
	case AUTO_SEND_ONE_PUSH_CYL_PUSH:
		{
			m_strMsg = m_bLangulageEng?_T("Send Push Cyl Push"):_T("�������о�������״�,һ�������Ƴ�");
			ASSERT(0 <= m_nGroupID && m_nGroupID <= 1);
			if(MACHINE.m_pBatteryCore[m_nGroupID] != NULL && MACHINE.m_pBatteryCore[m_nGroupID]->OpenCylOpen(TRUE))
			{
				if(IsNoHardware() || (SendOneCylPush(TRUE)))
				{
					m_nNextAutoStep = AUTO_OPEN_CYL_CLOSE;
				}
			}
			break;
		}

		//�������о�������׹ر�                        
	case AUTO_OPEN_CYL_CLOSE:
		{
			m_strMsg = m_bLangulageEng?_T("Send Push Cyl Push"):_T("�������о�������׹ر�,�ǿڶ�λ�����Ƴ�");
			ASSERT(0 <= m_nGroupID && m_nGroupID <= 1);
			if(MACHINE.m_pBatteryCore[m_nGroupID] != NULL 
				&& MACHINE.m_pBatteryCore[m_nGroupID]->OpenCylOpen(FALSE))
			{
				m_nNextAutoStep = AUTO_VACUM_CYL_DOWN;
			}
			break;
		}
		//�ǿ���������½�
	case AUTO_VACUM_CYL_DOWN:
		{
			m_strMsg = m_bLangulageEng?_T("Locate Cyl Push, Vacum Cyl Push"):_T("�ǿڶ�λ�����Ƴ���������������Ƴ�");
			if(m_OLocateShellPush > -1 && OUTPUTS[m_OLocateShellPush]->IsOff())
			{
				OUTPUTS[m_OLocateShellPush]->On();
			}
			if(m_OLocateShellBack > -1 && OUTPUTS[m_OLocateShellBack]->IsOn())
			{
				OUTPUTS[m_OLocateShellBack]->Off();
			}
			
			VacumOpenOrCheck(TRUE,FALSE); //����ղ����

			for(int i = 0; i < 2; i++)
			{
				if(m_OVacumCylPush[i] > -1 && OUTPUTS[m_OVacumCylPush[i]]->IsOff())
					OUTPUTS[m_OVacumCylPush[i]]->On();
			}
			if(IsNoHardware() || VacumCylPush(TRUE) && LocateCylPush(TRUE))
			{
				//Sleep(500);
				m_nNextAutoStep = AUTO_MOUTH_VACUUM_OPEN;
			}
			break;
		}

		//�򿪿ǿ����
	case AUTO_MOUTH_VACUUM_OPEN:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("�򿪿ǿ����");

			//ASSERT(m_OVacumCyl[0] > -1 && m_OVacumCyl[1] > -1 && m_ICheckVacum[0] > -1 && m_ICheckVacum[1] > -1);
			////add by ls 201905061606 
			//OUTPUTS[m_OVacumCyl[0]]->On();
			//OUTPUTS[m_OVacumCyl[1]]->On();
			////end ls
			Sleep(700);
			if (IsNoHardware()||  m_bDryRun	|| VacumOpenOrCheck(TRUE,TRUE)) //����ռ��
			{
				m_nNextAutoStep = AUTO_VACUM_CYL_UP;
			}
			break;
		}
		//�ǿ������������
	case AUTO_VACUM_CYL_UP:
		{
			m_strMsg = m_bLangulageEng?_T("Vacum Cyl Up"):_T("�������������");

			if(IsNoHardware() || VacumCylPush(FALSE))
			{
				m_nNextAutoStep = AUTO_CHECK_VACUM;
			}
			break;
		}
		//������
	case AUTO_CHECK_VACUM:
		{

			m_strMsg = m_bLangulageEng?_T("Check Vacum"):_T("������,��������׼�����ź�");
			
			if(IsNoHardware() || 1 || m_bDryRun
				|| ((m_ICheckVacum[0] > -1 && INPUTS[m_ICheckVacum[0]]->IsOn()) && (m_ICheckVacum[1] > -1 && INPUTS[m_ICheckVacum[1]]->IsOn())))
			{
				m_nRetryTimes = 0;
				m_bShellReady = TRUE;
				m_nNextAutoStep = AUTO_CLOSE_VACUM;
				SaveStepInfo(WRITE_TYPE_AUTO_STEP|WRITE_TYPE_VARIABLES);
			}
			else
			{
				m_nRetryTimes++;
				if(m_nRetryTimes >= 3)
				{
					m_nRetryTimes = 0;
					CString strMsg = _T("");
					strMsg.Format(_T("%s����������ʧ�ܣ�����"), m_strProcess);
					ShowMessage(strMsg, 3);
				}
				m_nNextAutoStep = AUTO_VACUM_CYL_DOWN;
			}
			break;
		}

		//�ȴ��������״�
	case AUTO_CLOSE_VACUM:
		{
			m_strMsg = m_bLangulageEng?_T("Close Vacum"):_T("�ȴ��������״�");
			ASSERT(0 <= m_nGroupID && m_nGroupID <= 1);

			if(MACHINE.m_pBatteryCore[m_nGroupID] != NULL && (MACHINE.m_pBatteryCore[m_nGroupID]->m_bOpenLoc || !MACHINE.m_pBatteryCore[m_nGroupID]->IsModuleEnable()))
			{
				MACHINE.m_pBatteryCore[m_nGroupID]->m_bOpenLoc = FALSE;
				m_nNextAutoStep = AUTO_WAIT_CORE_READY;
			}
			break;
		}

		//�ȴ���о׼����
	case AUTO_WAIT_CORE_READY:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Core Ready"):_T("�ȴ���о׼����,�ر����");
			ASSERT(0 <= m_nGroupID && m_nGroupID <= 1);
			if(!m_bShellReady || (MACHINE.m_pBatteryCore[m_nGroupID] != NULL && !MACHINE.m_pBatteryCore[m_nGroupID]->IsModuleEnable()))
			{
				VacumOpenOrCheck(FALSE,FALSE); //�ر���ղ����

				if (m_OBreakVacum > -1)
				{
					OUTPUTS[m_OBreakVacum]->On();
				}
				if (m_OTailVacum > -1)
				{
					OUTPUTS[m_OTailVacum]->Off();
				}
				m_nNextAutoStep = AUTO_LIFT_CYL_DOWN;
				SaveStepInfo(WRITE_TYPE_AUTO_STEP);
			}
			break;
		}

		//���������½�
	case AUTO_LIFT_CYL_DOWN:
		{
			m_strMsg = m_bLangulageEng?_T("Lift Cyl Down"):_T("���������½�");

			if(::IsNoHardware() ||DownLiftCylUp(FALSE))
			{
				m_nNextAutoStep = AUTO_WAIT_NEXT_MODULE_REQUIRE;
			}
			break;
		}

		//�ȴ���һģ��Ҫ���ź�  ��Ǻ��צ�н�������
	case AUTO_WAIT_NEXT_MODULE_REQUIRE:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Next Module Require"):_T("�ȴ���һģ��Ҫ���ź�");
			ASSERT(0 <= m_nGroupID && m_nGroupID <= 1);
			if(MACHINE.m_pBatteryOffloadPnp[m_nGroupID] != NULL 
				&& (MACHINE.m_pBatteryOffloadPnp[m_nGroupID]->m_bRequirePallet || !MACHINE.m_pBatteryOffloadPnp[m_nGroupID]->IsModuleEnable()))
			{
				MACHINE.m_pBatteryOffloadPnp[m_nGroupID]->m_bRequirePallet =FALSE;
				m_nNextAutoStep = AUTO_WAIT_NEXT_MODULE_WAORK_FINISH;

				MACHINE.m_pBatteryOffloadPnp[m_nGroupID]->SaveStepInfo(WRITE_TYPE_AUTO_STEP|WRITE_TYPE_VARIABLES);
			}
			break;
		}

		//�ȴ���һģ�鹤������ź�
	case AUTO_WAIT_NEXT_MODULE_WAORK_FINISH:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Next Module Work Finish"):_T("�ȴ���һģ�鹤������ź�");
			ASSERT(0 <= m_nGroupID && m_nGroupID <= 1);
			if(MACHINE.m_pBatteryOffloadPnp[m_nGroupID] !=NULL && (MACHINE.m_pBatteryOffloadPnp[m_nGroupID]->m_bPnPSendCylReturn||!MACHINE.m_pBatteryOffloadPnp[m_nGroupID]->IsModuleEnable()))
			{
				MACHINE.m_pBatteryOffloadPnp[m_nGroupID]->m_bPnPSendCylReturn =FALSE;
				m_bShellVaild = FALSE;

				m_nNextAutoStep = AUTO_SEND_CYL_RETURN;
				m_Pallet.Release();//���ȡ�ϻ�е��δʹ��

				SaveStepInfo(WRITE_TYPE_AUTO_STEP|WRITE_TYPE_VARIABLES|WRITE_TYPE_BATTERY);
				MACHINE.m_pBatteryOffloadPnp[m_nGroupID]->SaveStepInfo(WRITE_TYPE_AUTO_STEP|WRITE_TYPE_VARIABLES);
			}
			break;
		}

		//��������׻���
	case AUTO_SEND_CYL_RETURN:
		{
			m_strMsg = m_bLangulageEng?_T("Send Cyl Return"):_T("�������׻���");

			if(IsNoHardware() ||( SendOneCylPush(FALSE) && SendTwoCylPush(FALSE)))
			{
				m_nNextAutoStep = AUTO_CLUMMN_CYL_OPEN;
			}
			break;
		}
	case AUTO_CLUMMN_CYL_OPEN:
		{
			m_strMsg = m_bLangulageEng?_T("Clumn Cyl Open"):_T("�������״�");
			if(IsNoHardware() || ColumnCylClose(FALSE))
			{
				m_nNextAutoStep = AUTO_CHECK_NO_HAVE_PRODUCT;
			}
			break;
		}
		//�������
	case AUTO_CHECK_NO_HAVE_PRODUCT:
		{
			m_strMsg = m_bLangulageEng?_T("Check No Have Product"):_T("�������");

			//if (m_bDryRun || m_IHaveCore > -1 && WAIT_OK == WaitInput(INPUTS[m_IHaveCore], OFF, MSG_SHOW_DEFAULT))
			{
				m_nNextAutoStep = AUTO_WORK_END;
			}
			break;
		}

		//�������
	case AUTO_WORK_END:
		{
			m_strMsg = m_bLangulageEng?_T("Work end"):_T("�������");
			m_nNextAutoStep = AUTO_SEND_SHELL_CORE_REQUIRE;
			break;
		}
	default:
		{
			ASSERT(0);
			break;
		}
	}
}

BOOL CRunProcessBatteryInShell::InitializeConfig(CString strProcessModule)
{
	if (!CRunProcess::InitializeConfig(strProcessModule))
	{
		return FALSE;
	}	
	return TRUE;
}

void CRunProcessBatteryInShell::GetHardwareConfig(CArray<int, int> *pInputs, CArray<int, int> *pOutputs, CArray<int, int> *pMotors)
{
	CRunProcess::GetHardwareConfig(pInputs, pOutputs, pMotors);
}

BOOL CRunProcessBatteryInShell::  ReadRunParameter()
{
	CRunProcess:: ReadRunParameter();
	return TRUE;
}

BOOL CRunProcessBatteryInShell::SaveParameter()
{
	CRunProcess::SaveParameter();
	return TRUE;
}

BOOL CRunProcessBatteryInShell::CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{
	return TRUE;
}

BOOL  CRunProcessBatteryInShell::CheckOutputCanActive(COutput *pOutput)
{

#ifdef _TEST_
	return TRUE;
#endif

	CString strKey;

	if (m_OSendOneCylPush > -1 && pOutput == OUTPUTS[m_OSendOneCylPush])
	{
		if ( OUTPUTS[m_OSendOneCylPush]->IsOn())
		{
			if (MACHINE.m_pBatteryOffloadPnp[m_nGroupID] != NULL)
			{
				int nMotorX = MACHINE.m_pBatteryOffloadPnp[m_nGroupID]->m_nMotorX;
				double dPos0 = MOTORS[nMotorX]->GetCurPos();
				if (dPos0 > 5)
				{
					strKey.Format(_T("��о���%dץȡY�᲻�ڰ�ȫλ��һ����������ײ��ܻ���"), m_nGroupID + 1);
					BLMessageBox(strKey, MB_ICONWARNING);
					return FALSE;
				}
			}
		}

		if (m_ISendOneCylPush > -1 && OUTPUTS[m_ISendOneCylPush]->IsOff())
		{
			if (MACHINE.m_pCarryShellPnp != NULL)
			{
				int nMotorZ = MACHINE.m_pCarryShellPnp->m_nMotorZ;
				double dPos0 = MOTORS[nMotorZ]->GetCurPos(); 
				if (dPos0 > 5)
				{
					strKey.Format(_T("�������Z����(M05)���ڰ�ȫλ��һ����������ײ����Ƴ�"));
					BLMessageBox(strKey, MB_ICONWARNING);
					return FALSE;
				}
			}
		}
	}
	return TRUE;
}


BOOL CRunProcessBatteryInShell::LocateCylPush( BOOL bPush)
{
	ASSERT(m_ILocateShellBack[0] > -1
		&& m_ILocateShellBack[1] > -1
		&& m_ILocateShellPush[0] > -1
		&& m_ILocateShellPush[1] > -1
		&& m_OLocateShellPush > -1
		&& m_OLocateShellBack > -1);

	BOOL bRes = FALSE;

	if(bPush)
	{
		OUTPUTS[m_OLocateShellPush]->On();
		OUTPUTS[m_OLocateShellBack]->Off();
		if(m_bDryRun)
		{
			Sleep(300);
			return TRUE;
		}
		Sleep(10);
		if(WAIT_OK == WaitInput(INPUTS[m_ILocateShellPush[0]], ON, MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_ILocateShellPush[1]], ON, MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_ILocateShellBack[0]], OFF, MSG_SHOW_DEFAULT)
			&& WAIT_OK == WaitInput(INPUTS[m_ILocateShellBack[1]], OFF, MSG_SHOW_DEFAULT))
		{
			bRes = TRUE;
		}
	}
	else
	{

		OUTPUTS[m_OLocateShellPush]->Off();
		OUTPUTS[m_OLocateShellBack]->On();
		Sleep(10);
		if(WAIT_OK == WaitInput(INPUTS[m_ILocateShellPush[0]], OFF, MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_ILocateShellPush[1]], OFF, MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_ILocateShellBack[0]], ON, MSG_SHOW_DEFAULT)
			&& WAIT_OK == WaitInput(INPUTS[m_ILocateShellBack[1]], ON, MSG_SHOW_DEFAULT))
		{
			bRes = TRUE;
		}
	}
	return bRes;
}

/****************************************************************************************
�����������������׶���
���������bClose:		TRUE:�������׹ر�		FALSE:�������״�
�������أ�TRUE:ִ�гɹ���FALSE:ִ��ʧ��
*****************************************************************************************/
BOOL CRunProcessBatteryInShell::ColumnCylClose( BOOL bClose)
{
	ASSERT(m_IColumnCylOpen[0] > -1
		&& m_IColumnCylOpen[1] > -1
		&& m_IColumnCylClose[0] > -1
		&& m_IColumnCylClose[1] > -1
		&& m_OColumnCylClose > -1
		&& m_OColumnCylOpen > -1);

	BOOL bRes = FALSE;

	if(bClose)
	{
		OUTPUTS[m_OColumnCylClose]->On();
		OUTPUTS[m_OColumnCylOpen]->Off();
		
		Sleep(300);  //add by ls 201904292018 ������ʱ����ֹû��ס������
		//return TRUE;

		if(WAIT_OK == WaitInput(INPUTS[m_IColumnCylClose[0]], ON, MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_IColumnCylOpen[0]], OFF, MSG_SHOW_DEFAULT)
			&& WAIT_OK == WaitInput(INPUTS[m_IColumnCylClose[1]], ON, MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_IColumnCylOpen[1]], OFF, MSG_SHOW_DEFAULT))
		{
			bRes = TRUE;
		}
	}
	else
	{
		OUTPUTS[m_OColumnCylClose]->Off();
		OUTPUTS[m_OColumnCylOpen]->On();

		if(WAIT_OK == WaitInput(INPUTS[m_IColumnCylClose[0]], OFF, MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_IColumnCylOpen[0]], ON, MSG_SHOW_DEFAULT)
			&& WAIT_OK == WaitInput(INPUTS[m_IColumnCylClose[1]], OFF, MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_IColumnCylOpen[1]], ON, MSG_SHOW_DEFAULT))
		{
			bRes = TRUE;
		}
	}
	return bRes;
}

/****************************************************************************************
�������������ǿ�������׶���
���������bDown:		TRUE:�ǿ���������Ƴ�		FALSE:�ǿ�������׻���
�������أ�TRUE:ִ�гɹ���FALSE:ִ��ʧ��
*****************************************************************************************/
BOOL CRunProcessBatteryInShell::VacumCylPush(BOOL bPush)
{
	ASSERT(m_IVacumCylPush[0] > -1
		&& m_IVacumCylPush[1] > -1
		&& m_IVacumCylBack[0] > -1
		&& m_IVacumCylBack[1] > -1
		&& m_OVacumCylPush[0] > -1
		&& m_OVacumCylPush[1] > -1);

	if(bPush)
	{
		for(int i = 0; i < 2; i++)
		{
			OUTPUTS[m_OVacumCylPush[i]]->On();	
		}
		if(m_bDryRun)
		{
			Sleep(300);
			return TRUE;
		}
		if(WAIT_OK == WaitInput(INPUTS[m_IVacumCylPush[0]], ON, MSG_SHOW_DEFAULT) 
			&&WAIT_OK == WaitInput(INPUTS[m_IVacumCylPush[1]], ON, MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_IVacumCylBack[0]], OFF, MSG_SHOW_DEFAULT)
			&& WAIT_OK == WaitInput(INPUTS[m_IVacumCylBack[1]], OFF, MSG_SHOW_DEFAULT))
		{
			return TRUE;
		}
	}
	else
	{
		for(int i = 0; i < 2; i++)
		{
			OUTPUTS[m_OVacumCylPush[i]]->Off();
		}

		if(WAIT_OK == WaitInput(INPUTS[m_IVacumCylPush[0]], OFF, MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_IVacumCylPush[1]], OFF, MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_IVacumCylBack[0]], ON, MSG_SHOW_DEFAULT)
			&& WAIT_OK == WaitInput(INPUTS[m_IVacumCylBack[1]], ON, MSG_SHOW_DEFAULT))
		{
			return TRUE;
		}
	}
	return FALSE;
}

/****************************************************************************************
������������������׶���
���������bPush:T	RUE:����������Ƴ�			FALSE:��������׻���
�������أ�TRUE:ִ�гɹ���FALSE:ִ��ʧ��
*****************************************************************************************/
BOOL CRunProcessBatteryInShell::SendOneCylPush(BOOL bPush)
{
	ASSERT(m_ISendOneCylPush > -1 && m_ISendOneCylBack > -1 && m_OSendOneCylPush > -1);

	BOOL bRes = FALSE;
	if(bPush)
	{
		OUTPUTS[m_OSendOneCylPush]->On();
		if (m_OSendOneCylBack > -1)
		{
			OUTPUTS[m_OSendOneCylBack]->Off();
		}
		Sleep(10);
		if(WAIT_OK == WaitInput(INPUTS[m_ISendOneCylPush], ON, MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_ISendOneCylBack], OFF, MSG_SHOW_DEFAULT))
		{
			return TRUE;
		}
	}
	else
	{
		if (m_OSendOneCylBack > -1)
		{
			OUTPUTS[m_OSendOneCylBack]->On();
		}
		OUTPUTS[m_OSendOneCylPush]->Off();
		Sleep(10);
		if(WAIT_OK == WaitInput(INPUTS[m_ISendOneCylPush], OFF, MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_ISendOneCylBack], ON, MSG_SHOW_DEFAULT))
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CRunProcessBatteryInShell::VacumOpenOrCheck(BOOL bOpen , BOOL bCheck)
{
	if (bOpen)
	{
		for (int i=0;i < 2;i++)
		{
			if (m_OVacumCyl[i] > -1)
			{
				OUTPUTS[m_OVacumCyl[i]]->On();
			}
		}
	}
	else
	{
		for (int i=0;i < 2;i++)
		{
			if (m_OVacumCyl[i] > -1)
			{
				OUTPUTS[m_OVacumCyl[i]]->Off();
			}
		}
	}

	if (bCheck)
	{
		for (int i=0;i < 2;i++)
		{
			if (m_ICheckVacum[i] < 0 || WaitInput(INPUTS[m_ICheckVacum[i]],ON,MSG_SHOW_DEFAULT))
			{
				continue;
			}
			return FALSE;
		}
	}
	return TRUE;
}

/****************************************************************************************
����������������������׶���
���������bPush:T	RUE:����������Ƴ�			FALSE:��������׻���
�������أ�TRUE:ִ�гɹ���FALSE:ִ��ʧ��
*****************************************************************************************/
BOOL CRunProcessBatteryInShell::SendTwoCylPush(BOOL bPush)
{
	ASSERT(m_ISendTwoCylPush > -1 && m_ISendTwoCylBack > -1 && m_OSendTwoCylPush > -1);

	if(bPush)
	{
		OUTPUTS[m_OSendTwoCylPush]->On();

		if (m_OSendTwoCylBack > -1)
		{
			OUTPUTS[m_OSendTwoCylBack]->Off();
		}

		if(WAIT_OK == WaitInput(INPUTS[m_ISendTwoCylPush], ON, MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_ISendTwoCylBack], OFF, MSG_SHOW_DEFAULT))
		{
			return TRUE;
		}
	}
	else
	{
		OUTPUTS[m_OSendTwoCylPush]->Off();

		if (m_OSendTwoCylBack > -1)
		{
			OUTPUTS[m_OSendTwoCylBack]->On();
		}
		if(WAIT_OK == WaitInput(INPUTS[m_ISendTwoCylPush], OFF, MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_ISendTwoCylBack], ON, MSG_SHOW_DEFAULT))
		{
			return TRUE;
		}
	}
	return FALSE;
}

/****************************************************************************************
�����������������׶���
���������bLift:		TRUE:�������׶��� FALSE:�������׻���
�������أ�TRUE:ִ�гɹ���FALSE:ִ��ʧ��
*****************************************************************************************/
BOOL CRunProcessBatteryInShell:: DownLiftCylUp(BOOL bLift)
{
	ASSERT(m_ILiftCylUp > -1 && m_ILiftCylDown > -1 && m_OLiftCylUp > -1 && m_OLiftCylDown > -1);

	if(bLift)
	{
		OUTPUTS[m_OLiftCylDown]->Off();
		OUTPUTS[m_OLiftCylUp]->On();

// 		if(m_bDryRun)
// 		{
// 			Sleep(1000);
// 			return TRUE;
// 		}
		if(WAIT_OK == WaitInput(INPUTS[m_ILiftCylUp], ON, MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_ILiftCylDown], OFF, MSG_SHOW_DEFAULT))
		{
			return TRUE;
		}
	}
	else
	{
		OUTPUTS[m_OLiftCylUp]->Off();
		OUTPUTS[m_OLiftCylDown]->On();

		if(WAIT_OK == WaitInput(INPUTS[m_ILiftCylUp], OFF, MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_ILiftCylDown], ON, MSG_SHOW_DEFAULT))
		{
			return TRUE;
		}
	}
	return FALSE;
}

/****************************************************************************************
������������ȡ����
���������
�������أ�TRUE:ִ�гɹ���FALSE:ִ��ʧ��
*****************************************************************************************/
void CRunProcessBatteryInShell::ReadStepInfo()
{
	CString strPath = GetAbsolutePathName(MODULE_RUN_FOLDER) + m_strName + _T(".cfg");

	m_nNextAutoStep = IniReadInt(_T("Data"), _T("m_nNextAutoStep"), 0, strPath);
	m_bShellVaild = IniReadBoolean(_T("Data"), _T("m_bShellVaild"), m_bShellVaild, strPath);
	m_Pallet.ReadCfg(_T("Pallet"), strPath);
	return ;
}

BOOL CRunProcessBatteryInShell::SaveStepInfo(int nWriteType)
{
	CString strPath = GetAbsolutePathName(MODULE_RUN_FOLDER) + m_strName + _T(".cfg");

	if(nWriteType & WRITE_TYPE_AUTO_STEP)
	{
		IniWriteInt(_T("Data"), _T("m_nNextAutoStep"), m_nNextAutoStep, strPath);
	}

	if(nWriteType & WRITE_TYPE_VARIABLES)
	{
		IniWriteBoolean(_T("Data"), _T("m_bShellVaild"), m_bShellVaild, strPath);
	}

	if(nWriteType & WRITE_TYPE_BATTERY)
	{
		m_Pallet.WriteCfg(_T("Pallet"), strPath);
	}
	return TRUE;
}

/****************************************************************************************
��������������צ����������ʵ�ʼ������һ����,m_nStartWithPalletDataΪ�������0 û���ϣ�1 ֻ�пǣ�2��������
���������
�������أ�TRUE��������  FALSE�������쳣��Ҫ���¼��
*****************************************************************************************/
BOOL CRunProcessBatteryInShell::CheckStartWithPalletDataSame()
{
	CString strMsg;

	if(m_Pallet.m_bIsValid)
	{
		if((m_IHaveCore > -1 && INPUTS[m_IHaveCore]->IsOff()))
		{
			strMsg.Format(_T("�����������ģ�����ϸ�Ӧ��ʵ�����ݲ�һ��,��������,��Ӧ����,��ȷ��"));
			ShowMessageEX(strMsg);
			strMsg.Format(_T("ȷ������ģ�����������ϵ�YES�����ϵ�NO"));
			if(BLMessageBox(strMsg,MB_YESNO) == IDYES)
			{
				m_Pallet.m_bIsValid = TRUE;
			}
			else
			{
				m_Pallet.m_bIsValid = FALSE;
			}
			return FALSE;
		}
	}
	else
	{
		if((m_IHaveCore > -1 && INPUTS[m_IHaveCore]->IsOn()))
		{
			strMsg.Format(_T("�����������ģ�����ϸ�Ӧ��ʵ�����ݲ�һ��,��������,��Ӧ����,��ȷ��"));
			ShowMessageEX(strMsg);
			strMsg.Format(_T("ȷ������ģ�����������ϵ�YES�����ϵ�NO"));
			if(BLMessageBox(strMsg,MB_YESNO) == IDYES)
			{
				m_Pallet.m_bIsValid = TRUE;
			}
			else
			{
				m_Pallet.m_bIsValid = FALSE;
			}
			return FALSE;
		}
	}


	if(!m_bShellVaild)
	{
		m_nStartWithPalletData = 0;  //�޿�
	}
	else
	{
		if(!m_Pallet.m_bIsValid)
		{
			m_nStartWithPalletData = 1;//�п� �޵�о
		}
		else
		{
			m_nStartWithPalletData = 2;//�п��е�о
		}
	}
	return TRUE;
}

