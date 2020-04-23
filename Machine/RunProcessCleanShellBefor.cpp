/****************************************************************************************
�ļ�����RunProcessCleanShellBefor.cpp
���������������ϴģ��
��ʷ�޸ļ�¼��
*****************************************************************************************/
#include "StdAfx.h"
#include "RunProcessCleanShellBefor.h"
CRunProcessCleanShellBefor::CRunProcessCleanShellBefor(int nID) :CRunProcess(nID)
{
	PowerUpRestart();

	CString strKey = _T("");
	for(int i = 0; i < 2; i++)
	{
		strKey.Format(_T("IShellDirection%d"), i);
		m_InputMap.insert(pair<int*, CString>(&m_IShellDirection[i], strKey));   //�ǿڳ�����

		//���
		strKey.Format(_T("IVacuum%d"), i);									
		m_InputMap.insert(pair<int*, CString>(&m_IVacuum[i], strKey));       //�������ո�Ӧ1
		strKey.Format(_T("OVacuum%d"), i);									
		m_OutputMap.insert(pair<int*, CString>(&m_OVacuum[i], strKey));       //�������ո�Ӧ1
	}

	m_InputMap.insert(pair<int*, CString>(&m_IRotateCylPush, _T("IRotateCylPush")));//��ת�����Ƴ���Ӧ
	m_InputMap.insert(pair<int*, CString>(&m_IRotateCylReturn, _T("IRotateCylReturn")));//��ת���׻��˸�Ӧ
	m_OutputMap.insert(pair<int*, CString>(&m_ORotateCylPush, _T("ORotateCylPush")));//��ת�����Ƴ���ŷ�
	m_OutputMap.insert(pair<int*, CString>(&m_ORotateCylReturn, _T("ORotateCylReturn")));//��ת���׻��˵�ŷ�

	m_InputMap.insert(pair<int*, CString>(&m_ITransferCylReturn, _T("ITransferCylReturn")));//�޸����׻��˸�Ӧ
	m_InputMap.insert(pair<int*, CString>(&m_ITransferCylPush, _T("ITransferCylPush")));//�޸������Ƴ���Ӧ

	m_OutputMap.insert(pair<int*, CString>(&m_OTransferCylPush, _T("OTransferCylPush")));//�޸������Ƴ���ŷ�

	m_InputMap.insert(pair<int*, CString>(&m_IManualBtn, _T("IManualBtn")));//��ť
	m_OutputMap.insert(pair<int*, CString>(&m_OManualBtnLight, _T("OManualBtnLight")));//��ť��

	APP_Add_Output(&m_OTransferCylReturn,_T("OTransferCylReturn"));
	
}

CRunProcessCleanShellBefor::~CRunProcessCleanShellBefor(void)
{
}

void CRunProcessCleanShellBefor::PowerUpRestart()
{
	CRunProcess::PowerUpRestart();
	m_strMsg = m_bLangulageEng?_T("Ready"):_T("׼����");
	m_bRequirePallet = FALSE;
	m_nNextPowerUpStep = PU_WATI_FRONT_MODULE_HOME;
	m_nNextAutoStep = AUTO_TRANSFER_CYL_RETURN;
}

void CRunProcessCleanShellBefor::PowerUpOperation(void)
{
	if (!IsModuleEnable())
	{
		CRun::PowerUpOperation();
		m_strMsg = m_bLangulageEng?_T("Moudle UnEnable"):_T("ģ�����");
		return;
	}

	switch(m_nNextPowerUpStep)
	{
		//�ȴ��Ͽǻ�е�ֻ���
	case PU_WATI_FRONT_MODULE_HOME:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Shell PNP Modle Home"):_T("�Ͽǻ�е��Z�Ḵλ���");

			if(MACHINE.m_pOnloadShellPnp != NULL && (MACHINE.m_pOnloadShellPnp->m_bHomeMoveSafePos || !MACHINE.m_pOnloadShellPnp->IsModuleEnable()))
			{
				//�Ͽǻ�е��Z�Ḵλ����ź�
				m_nNextPowerUpStep = PU_CHECK_HAVE_SHELL;
			}
			break;
		}

		//������޿�
	case PU_CHECK_HAVE_SHELL:
		{
			m_strMsg = m_bLangulageEng?_T("Check Have Shell"):_T("����޿�");

			OpenVacuum(TRUE);
			if (IsNoHardware() || m_bDryRun || CheckVacuum(FALSE))
			{
				OpenVacuum(FALSE);
				m_nNextPowerUpStep = PU_ROTATE_RETURN;
			}
			break;
		}

		//��ת���׻���
	case PU_ROTATE_RETURN:
		{
			m_strMsg = m_bLangulageEng?_T("Rotate Cyl Return"):_T("��ת���׻���");

			if (IsNoHardware() || RotateClyPush(FALSE))
			{
				m_nNextPowerUpStep = PU_TRANSFER_CYL_RETURN;
			}
			break;
		}

		//�������׻���
	case PU_TRANSFER_CYL_RETURN:
		{
			m_strMsg = m_bLangulageEng?_T("Transfer Cyl Return"):_T("�������׻���");

			if (IsNoHardware() || TransferCylPush(FALSE))
			{
				m_nNextPowerUpStep = PU_BUTTON_RESET;
			}
			break;
		}
	
		//��ť�Ƹ�λ
	case PU_BUTTON_RESET:
		{
			m_strMsg = m_bLangulageEng?_T("Button Reset"):_T("��ť�Ƹ�λ");
			if(IsNoHardware())
			{
				m_nNextPowerUpStep = PU_FINISH;
				break;
			}

			if (m_OManualBtnLight > -1)
			{
				OUTPUTS[m_OManualBtnLight]->Off();
			}
			m_nNextPowerUpStep = PU_FINISH;
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

void CRunProcessCleanShellBefor::AutoOperation()
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
		if (m_nSaveLastStep == AUTO_TRANSFER_CYL_RETURN)
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
		//�޸����׻���
	case AUTO_TRANSFER_CYL_RETURN:
		{
			m_strMsg = m_bLangulageEng?_T("Transfer Cyl Return"):_T("�޸����׻���");

			if(IsNoHardware() || TransferCylPush(FALSE))
			{
				m_nNextAutoStep = AUTO_ROTATE_RETURN;
			}
			break;
		}

		//��ת���׻���
	case AUTO_ROTATE_RETURN:
		{
			m_strMsg = m_bLangulageEng?_T("Rotate Cyl Return"):_T("��ת���׻���");

			if (IsNoHardware() || RotateClyPush(FALSE))
			{
				m_nNextAutoStep = AUTO_SEND_NEED_REQUIRE_EVENT;
			}
			break;
		}

		//����Ҫ�������ź�
	case AUTO_SEND_NEED_REQUIRE_EVENT:
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

			if (!m_bRequirePallet  || (MACHINE.m_pOnloadShellPnp != NULL && !MACHINE.m_pOnloadShellPnp->IsModuleEnable()))
			{
				m_bShellVaild = TRUE;
				m_nNextAutoStep = AUTO_VACUUM_OPEN;
			}
			break;   
		}

		//�򿪼�����
	case AUTO_VACUUM_OPEN:
		{
			m_strMsg = m_bLangulageEng?_T("Check Vacuun Open"):_T("�򿪼�����");

			OpenVacuum(TRUE);
			if (IsNoHardware() || m_bDryRun || CheckVacuum(TRUE))
			{
				m_nNextAutoStep = AUTO_CHECK_SHELL_DIRECTION;	
			}
			break;
		}

		//���ǿڷ���
	case AUTO_CHECK_SHELL_DIRECTION:
		{
			m_strMsg = m_bLangulageEng?_T("Check Shell Direction"):_T("���ǿڷ���");

			if (IsNoHardware() || m_bDryRun
				|| (m_IShellDirection[0] > -1 && INPUTS[m_IShellDirection[0]]->IsOn() 
				&& m_IShellDirection[1] > -1 && INPUTS[m_IShellDirection[1]]->IsOn()))
			{
				m_nNextAutoStep = AUTO_TRANSFER_CYL_PUSH;
			}
			else
			{
				//���ǿڷ�������ȡ�߿�
				ShowMessage(MSG_DUST_BEFORE_CHECK_SHELL_DIRECTION_REVERSE);
				if (m_OManualBtnLight > -1 && MACHINE.m_nTowerBuzzer > -1)
				{
					OUTPUTS[m_OManualBtnLight]->On();
					MACHINE.SetBuzzerStatue(TRUE);
					BLMessageBox(_T("���˹�ȡ�߷ŷ�����"));
					MACHINE.SetBuzzerStatue(FALSE);
					m_nNextAutoStep = AUTO_MANUAL_PICK_SHELL_CLOSE_VACUUM;
				}
			}
			break;
		}

		//  �ȴ��˹�ȡ�����
	case AUTO_MANUAL_PICK_SHELL_CLOSE_VACUUM:
		{
			m_strMsg = m_bLangulageEng?_T("Check Shell Direction"):_T("����գ��ȴ��˹�ȡ��");
			if (ButtonCheck())
			{
				OpenVacuum(FALSE);
				m_nNextAutoStep = AUTO_WAIT_MANUAL_PICK_SHELL_FINISH;
			}
			break;
		}
		//  �ȴ��˹�ȡ�����
	case AUTO_WAIT_MANUAL_PICK_SHELL_FINISH:
		{
			m_strMsg = m_bLangulageEng?_T("Check Shell Direction"):_T("�ȴ��˹�ȡ�����");
			OpenVacuum(TRUE);
			if (CheckVacuum(FALSE) && !CheckDirection())
			{
				OpenVacuum(FALSE);
				m_bShellVaild = FALSE;
				m_nNextAutoStep = AUTO_TRANSFER_CYL_RETURN;
			}
			break;
		}

		//�޸������ƶ�������λ
	case AUTO_TRANSFER_CYL_PUSH:
		{
			m_strMsg = m_bLangulageEng?_T("Transfer Cyl Push"):_T("�޸������ƶ�������λ");

			if (IsNoHardware() || TransferCylPush(TRUE))
			{
				m_nNextAutoStep = AUTO_WAIT_NEXT_MODULE_PICK;
			}			
			break;
		}

		//�ȴ���ϴ��λҪ��
	case AUTO_WAIT_NEXT_MODULE_PICK:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Next Module Pick"):_T("�ȴ���ϴ��λҪ��");

			if (MACHINE.m_pCleanShell != NULL && (MACHINE.m_pCleanShell->m_bRequirePallet || !MACHINE.m_pCleanShell->IsModuleEnable()))
			{
				m_nNextAutoStep = AUTO_ROTATE_PUSH;
			}
			break;
		}

		//��ת�����Ƴ�
	case AUTO_ROTATE_PUSH:
		{
			m_strMsg = m_bLangulageEng?_T("Rotate Cyl Push"):_T("��ת�����Ƴ�");

			if (IsNoHardware() || RotateClyPush(TRUE))
			{
				if (MACHINE.m_pCleanShell != NULL && (MACHINE.m_pCleanShell->m_bRequirePallet || !MACHINE.m_pCleanShell->IsModuleEnable()))
				{
					MACHINE.m_pCleanShell->m_bRequirePallet = FALSE;
					m_nNextAutoStep = AUTO_WAIT_NEXT_MODULE_FINGER_CLOSE;
				}
			}
			break;
		}

		//�ȴ���һģ��н��ź�  ��ϴģ��ȡ����
	case AUTO_WAIT_NEXT_MODULE_FINGER_CLOSE:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Pick Finish"):_T("�ȴ���ϴģ��н��ź�");

			if (MACHINE.m_pCleanShell != NULL && (MACHINE.m_pCleanShell->m_bFingerClose || !MACHINE.m_pCleanShell->IsModuleEnable()))
			{
				m_nNextAutoStep = AUTO_VACUUM_CLOSE;
			}
			break;
		}

		//��չر�
	case AUTO_VACUUM_CLOSE:
		{
			m_strMsg = m_bLangulageEng?_T("Vacuum Close"):_T("��չر�");

			OpenVacuum(FALSE);
			if (IsNoHardware() || CheckVacuum(FALSE))
			{
				m_nNextAutoStep = AUTO_SEND_SAFE_SIGNAL;
			}
			break;
		}
		//����ϴ������������ź�
	case AUTO_SEND_SAFE_SIGNAL:
		{
			m_strMsg = m_bLangulageEng?_T("Send Offload Finish"):_T("����ϴ������������ź�");

			if (MACHINE.m_pCleanShell != NULL && (MACHINE.m_pCleanShell->m_bFingerClose || !MACHINE.m_pCleanShell->IsModuleEnable()))
			{
				m_bShellVaild = FALSE;
				MACHINE.m_pCleanShell->m_bFingerClose = FALSE;
				m_nNextAutoStep = AUTO_TRANSFER_CYL_RETURN_OFFLOAD;
			}
			break;
		}
		//�޸������ƶ�������λ
	case AUTO_TRANSFER_CYL_RETURN_OFFLOAD:
		{
			m_strMsg = m_bLangulageEng?_T("Transfer Cyl Return Offload"):_T("�޸������ƶ�������λ");

			if (IsNoHardware() || TransferCylPush(FALSE))
			{
				m_nNextAutoStep = AUTO_WORK_END;
			}
			break;
		}
		//�������
	case AUTO_WORK_END:
		{
			m_strMsg = m_bLangulageEng?_T("Work end"):_T("�������");
			m_nNextAutoStep = AUTO_TRANSFER_CYL_RETURN;
			break;
		}
	default:
		{
			ASSERT(0);
			break;
		}
	}
}

BOOL CRunProcessCleanShellBefor::InitializeConfig(CString strProcessModule)
{
	if (!CRunProcess::InitializeConfig(strProcessModule))
	{
		return FALSE;
	}
	MACHINE.SetBuzzerStatue(FALSE);
	return TRUE;
}

void CRunProcessCleanShellBefor::GetHardwareConfig(CArray<int, int> *pInputs, CArray<int, int> *pOutputs, CArray<int, int> *pMotors)
{
	CRunProcess::GetHardwareConfig(pInputs, pOutputs, pMotors);
}

BOOL CRunProcessCleanShellBefor::CheckOutputCanActive(COutput *pOutput)
{
#ifdef _TEST_
	return TRUE;
#endif
	ASSERT(m_ORotateCylPush > -1 && m_ORotateCylReturn > -1 && m_OTransferCylPush > -1);

	if (pOutput == OUTPUTS[m_ORotateCylPush] || pOutput == OUTPUTS[m_ORotateCylReturn] || pOutput == OUTPUTS[m_OTransferCylPush])
	{
		if (MACHINE.m_pOnloadShellPnp != NULL)
		{
			int nMotorZ = MACHINE.m_pOnloadShellPnp->m_nMotorZ;
			if (!MOTORS[nMotorZ]->IsOriRefKnown())
			{
				BLMessageBox(_T("(M13)�������Z��û�и�λ���޸�/��ת���ײ����ƶ�"), MB_ICONWARNING);
				return FALSE;
			}

			if (MOTORS[nMotorZ]->GetCurPos() > 10)
			{ 
				BLMessageBox(_T("(M13)�������Z�ᵱǰλ�ô���10���޸�/��ת���ײ����ƶ�"), MB_ICONWARNING);
				return FALSE;	
			}
		}
	}

	if(m_OTransferCylPush > -1 && OUTPUTS[m_OTransferCylPush]->IsOff())
	{
		if((m_ORotateCylPush > -1 && pOutput == OUTPUTS[m_ORotateCylPush] && pOutput->IsOff())
			|| (m_ORotateCylReturn > -1 && pOutput == OUTPUTS[m_ORotateCylReturn] && pOutput->IsOn()))
		{
			BLMessageBox(_T("�������״��ڻ���״̬����ת���ײ����Ƴ�"), MB_ICONWARNING);
			return FALSE;
		}
	}

	if((m_ORotateCylPush > -1 && OUTPUTS[m_ORotateCylPush]->IsOn() || m_IRotateCylPush > -1 && INPUTS[m_IRotateCylPush]->IsOn()) 
		&&  m_OTransferCylPush > -1 && OUTPUTS[m_OTransferCylPush]->IsOff())
	{
		if(pOutput == OUTPUTS[m_OTransferCylPush])
		{
			BLMessageBox(_T("��ת���״����Ƴ�״̬������ֱ���Ƴ��޸�����"), MB_ICONWARNING);
			return FALSE;
		}
	}

	if(m_OTransferCylPush > -1 && OUTPUTS[m_OTransferCylPush]->IsOn() 
		&& (m_OVacuum[0] > -1 && OUTPUTS[m_OVacuum[0]]->IsOn() || m_OVacuum[1] > -1 && OUTPUTS[m_OVacuum[1]]->IsOn()) 
		&& m_ORotateCylPush > -1 && OUTPUTS[m_ORotateCylPush]->IsOn())
	{
		if((pOutput == OUTPUTS[m_OTransferCylPush])
			|| (pOutput == OUTPUTS[m_ORotateCylPush])
			|| (m_ORotateCylReturn > -1 && pOutput == OUTPUTS[m_ORotateCylReturn] && pOutput->IsOff()))
		{
			BLMessageBox(_T("���ԡ���ת���״����Ƴ�״̬������ѿ��������������ǣ����Ƚ���չر�"), MB_ICONWARNING);
			return FALSE;
		}
	}

	int IFingerCylClose[2];
	int IFingerCylOpen[2];
	int IHaveProduct[2];

	ASSERT(m_IRotateCylPush > -1 && m_IRotateCylReturn > -1 && m_ORotateCylPush > -1 && m_ORotateCylReturn > -1);

	if (MACHINE.m_pCleanShell == NULL)
	{	
		return FALSE;
	}

	for (int i = 0; i < 2; i++)
	{
		IFingerCylClose[i] = MACHINE.m_pCleanShell->m_IFingerCylClose[i];
		IFingerCylOpen[i] = MACHINE.m_pCleanShell->m_IFingerCylOpen[i];
		IHaveProduct[i] = MACHINE.m_pCleanShell->m_IHaveProduct[i];
		ASSERT(IFingerCylClose[i] > -1 && IFingerCylOpen[i] > -1 && IHaveProduct[i] > -1);
	}

	if (pOutput == OUTPUTS[m_ORotateCylPush] || pOutput == OUTPUTS[m_ORotateCylReturn])
	{
		if (INPUTS[m_ITransferCylPush]->IsOn() || INPUTS[m_ITransferCylReturn]->IsOff())
		{
			if (INPUTS[IHaveProduct[0]]->IsOn() || INPUTS[IHaveProduct[1]]->IsOn())
			{
				BLMessageBox(_T("��๤λ���ϸ�Ӧ������ϣ��޸������Ƴ�����ת���ײ��ܶ�"), MB_ICONWARNING);
				return FALSE;
			}

			if ( INPUTS[IFingerCylClose[0]]->IsOn() || INPUTS[IFingerCylClose[1]]->IsOn() 
				|| INPUTS[IFingerCylOpen[0]]->IsOff() || INPUTS[IFingerCylOpen[1]]->IsOff())
			{
				BLMessageBox(_T("��Ǽн�����1��2�н�����ת���ײ��ܶ�"), MB_ICONWARNING);
				return FALSE;
			}

			int ILiftCylUp = MACHINE.m_pCleanShell->m_ILiftCylUp;  
			int ILiftCylDown = MACHINE.m_pCleanShell->m_ILiftCylDown;
			ASSERT(ILiftCylUp > -1 && ILiftCylDown > -1);
			if (INPUTS[ILiftCylUp]->IsOn() || INPUTS[ILiftCylDown]->IsOff())
			{
				BLMessageBox(_T("��ඥ�������Ƴ�����ת���ײ��ܶ�"), MB_ICONWARNING);
				return FALSE;
			}
		}
	}
	return TRUE;
}


/****************************************************************************************
������������ת���׶���
���������bPush:TRUE:��ת�����Ƴ�ˮƽ����  FALSE:��ת���׻�����ֱȡ��
�������أ�TRUE:ִ�гɹ���FALSE:ִ��ʧ��
*****************************************************************************************/
BOOL CRunProcessCleanShellBefor::RotateClyPush(BOOL bPush)
{
	ASSERT(m_IRotateCylPush > -1 &&  m_IRotateCylReturn > -1 && m_ORotateCylPush > -1 && m_ORotateCylReturn > -1);

	BOOL Res = FALSE;
	bPush?OUTPUTS[m_ORotateCylPush]->On():OUTPUTS[m_ORotateCylPush]->Off();
	bPush?OUTPUTS[m_ORotateCylReturn]->Off():OUTPUTS[m_ORotateCylReturn]->On();

	if (WAIT_OK == WaitInput(INPUTS[m_IRotateCylPush], bPush?ON:OFF, MSG_SHOW_DEFAULT) 
		&& WAIT_OK == WaitInput(INPUTS[m_IRotateCylReturn], bPush?OFF:ON, MSG_SHOW_DEFAULT))
	{
		Res = TRUE;
	}
	return Res;
}

/****************************************************************************************
�����������޸����׶���
���������bPush:TRUE:�޸������Ƴ� FALSE:�޸����׻���
�������أ�TRUE:ִ�гɹ���FALSE:ִ��ʧ��
*****************************************************************************************/
BOOL CRunProcessCleanShellBefor::TransferCylPush(BOOL bPush)
{
	if (m_ITransferCylPush < 0 || m_ITransferCylReturn < 0 || m_OTransferCylPush < 0 || m_OTransferCylReturn < 0)
	{
		return FALSE;
	}

	if (bPush)
	{
		OUTPUTS[m_OTransferCylPush]->On();
		OUTPUTS[m_OTransferCylReturn]->Off();

		if (WAIT_OK == WaitInput(INPUTS[m_ITransferCylPush],ON,MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_ITransferCylReturn],OFF,MSG_SHOW_DEFAULT))
		{
			return TRUE;
		}
	}
	else
	{
		OUTPUTS[m_OTransferCylPush]->Off();
		OUTPUTS[m_OTransferCylReturn]->On();

		if (WAIT_OK == WaitInput(INPUTS[m_ITransferCylPush],OFF,MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_ITransferCylReturn],ON,MSG_SHOW_DEFAULT))
		{
			return TRUE;
		}
	}

	return FALSE;
}


/****************************************************************************************
���������������
���������bPush:TRUE:����� FALSE:�ر����
�������أ�TRUE:ִ�гɹ���FALSE:ִ��ʧ��
*****************************************************************************************/
BOOL CRunProcessCleanShellBefor::OpenVacuum(BOOL bOpen)
{
	ASSERT(m_OVacuum[0] > -1 && m_OVacuum[1] > -1);

	if(bOpen)
	{
		OUTPUTS[m_OVacuum[0]]->On();
		OUTPUTS[m_OVacuum[1]]->On();
	}
	else
	{
		OUTPUTS[m_OVacuum[0]]->Off();
		OUTPUTS[m_OVacuum[1]]->Off();
	}
	return TRUE;
}

/****************************************************************************************
������������ռ������
���������bHave:TRUE:������� FALSE:�������
�������أ�TRUE:ִ�гɹ���FALSE:ִ��ʧ��
*****************************************************************************************/
BOOL CRunProcessCleanShellBefor::CheckVacuum(BOOL bHave)
{
	ASSERT(m_IVacuum[0] > -1 && m_IVacuum[1] > -1);

	BOOL Res = FALSE;
	if(WAIT_OK == WaitInput(INPUTS[m_IVacuum[0]], bHave?ON:OFF, MSG_SHOW_DEFAULT) 
		&& WAIT_OK == WaitInput(INPUTS[m_IVacuum[1]], bHave?ON:OFF, MSG_SHOW_DEFAULT))
	{
		Res = TRUE;
	}
	return Res;
}

/****************************************************************************************
�����������ǿڼ��
���������
�������أ�TRUE:�ǿ�������FALSE:�ǿڳ���
*****************************************************************************************/
BOOL CRunProcessCleanShellBefor::CheckDirection()
{
	ASSERT(m_IShellDirection[0] > -1 && m_IShellDirection[1] > -1);

	if (INPUTS[m_IShellDirection[0]]->IsOn() &&INPUTS[m_IShellDirection[0]]->IsOn())
	{
		return TRUE;
	}

	return FALSE;
}

/****************************************************************************************
������������ť���¼��
���������
�������أ�TRUE:��ť���£�FALSE:��ťδ����
*****************************************************************************************/
BOOL CRunProcessCleanShellBefor::ButtonCheck()
{
	ASSERT(m_IManualBtn > -1 && m_OManualBtnLight > -1);
	BOOL Res = FALSE;
	if (INPUTS[m_IManualBtn]->IsOn())
	{
		Sleep(300);
		if (INPUTS[m_IManualBtn]->IsOn())
		{
			OUTPUTS[m_OManualBtnLight]->Off();
			Res =TRUE;
		}
	}
	return Res;
}