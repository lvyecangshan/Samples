/****************************************************************************************
�ļ�����RunProcessCleanShellAfter.cpp
���������������ϴ��ת
��ʷ�޸ļ�¼��
*****************************************************************************************/
#include "StdAfx.h"
#include "RunProcessCleanShellAfter.h"
CRunProcessCleanShellAfter::CRunProcessCleanShellAfter(int nID) :CRunProcess(nID)
{
	PowerUpRestart();

	CString strKey = _T("");
	for(int i = 0; i < 2; i++)
	{
		//���
		strKey.Format(_T("IVacuum%d"), i);
		m_InputMap.insert(pair<int*, CString>(&m_IVacuum[i], strKey));

		strKey.Format(_T("OVacuum%d"), i);
		m_OutputMap.insert(pair<int*, CString>(&m_OVacuum[i], strKey));
	}
	//��ת����
	m_InputMap.insert(pair<int*, CString>(&m_IRotateCylPush, _T("IRotateCylPush")));
	m_InputMap.insert(pair<int*, CString>(&m_IRotateCylReturn, _T("IRotateCylReturn")));
	m_OutputMap.insert(pair<int*, CString>(&m_ORotateCylPush, _T("ORotateCylPush")));
	m_OutputMap.insert(pair<int*, CString>(&m_ORotateCylReturn, _T("ORotateCylReturn")));
	//ƽ������
	m_InputMap.insert(pair<int*, CString>(&m_ITransferCylPush, _T("ITransferCylPush")));
	m_InputMap.insert(pair<int*, CString>(&m_ITransferCylReturn, _T("ITransferCylReturn")));
	m_OutputMap.insert(pair<int*, CString>(&m_OTransferCylPush, _T("OTransferCylPush")));

	APP_Add_Output(&m_OTransferCylReturn,_T("OTransferCylReturn"));
}

CRunProcessCleanShellAfter::~CRunProcessCleanShellAfter(void)
{
}

void CRunProcessCleanShellAfter::PowerUpRestart()
{
	CRunProcess::PowerUpRestart();
	m_strMsg = m_bLangulageEng?_T("Ready"):_T("׼����");

	m_bOffloadRequire = FALSE;

	m_nNextPowerUpStep = PU_WATI_NEXT_MODULE_HOME;
	m_nNextAutoStep = AUTO_TRANSFER_CYL_RETURN;
}

void CRunProcessCleanShellAfter::PowerUpOperation(void)
{
	if (!IsModuleEnable())
	{
		CRun::PowerUpOperation();
		m_strMsg = m_bLangulageEng?_T("Moudle UnEnable"):_T("ģ�����");
		return;
	}

	switch(m_nNextPowerUpStep)
	{
		//�����ϴ�����Z�Ḵλ���
	case PU_WATI_NEXT_MODULE_HOME:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Shell PNP Modle Home"):_T("�����ϴ�����Z�Ḵλ���");
			if(MACHINE.m_pCarryShellPnp != NULL && (MACHINE.m_pCarryShellPnp->m_bHomeMoveSafePos || !MACHINE.m_pCarryShellPnp->IsModuleEnable()))
			{
				//����Z�Ḵλ���
				m_nNextPowerUpStep = PU_CHECK_WITHOUT_SHELL;
			}
			break;
		}
		//������޿�
	case PU_CHECK_WITHOUT_SHELL:
		{
			m_strMsg = m_bLangulageEng?_T("Check Have Shell"):_T("����޿�");
			OpenVacuum(TRUE);
			if (IsNoHardware() || CheckVacuum(FALSE))
			{
				OpenVacuum(FALSE);
				m_nNextPowerUpStep = PU_TRANSFER_CYL_RETURN;
			}
			break;
		}
		//ƽ�����׻���
	case PU_TRANSFER_CYL_RETURN:
		{
			m_strMsg = m_bLangulageEng?_T("Transfer Cyl Return"):_T("ƽ�����׻���");

			if (IsNoHardware() || TransferCylPush(FALSE))
			{
				m_nNextPowerUpStep = PU_ROTATE_RETURN;
			}
			break;
		}
		//��ת���׻���(ˮƽ)
	case PU_ROTATE_RETURN:
		{
			m_strMsg = m_bLangulageEng?_T("Rotate Cyl Return"):_T("��ת���׻���");

			if (IsNoHardware() || RotateClyPush(FALSE))
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

void CRunProcessCleanShellAfter::AutoOperation()
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
		if (m_nSaveLastStep == AUTO_VACUUM_CLOSE)
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
    
	if(m_nAutoNext != m_nNextAutoStep)
	{
		m_nAutoNext = m_nNextAutoStep;
		m_dwModuleStrt = GetTickCount();
	}

	switch(m_nNextAutoStep)
	{
		//�򿪼�����
	case AUTO_VACUUM_CLOSE:
		{
			m_strMsg = m_bLangulageEng?_T("Check Vacuun Open"):_T("�ر����");

			if (OpenVacuum(FALSE) && (IsNoHardware() || CheckVacuum(FALSE)))
			{
				m_nNextAutoStep = AUTO_TRANSFER_CYL_RETURN;	
			}
			break;
		}

		//ƽ�����׻���
	case AUTO_TRANSFER_CYL_RETURN:
		{
			m_strMsg = m_bLangulageEng?_T("Transfer Cyl Return"):_T("ƽ�����׻���");

			if(IsNoHardware() || TransferCylPush(FALSE))
			{
				m_nNextAutoStep = AUTO_ROTATE_RETURN;
			}
			break;
		}

		//��ת���׻���
	case AUTO_ROTATE_RETURN:
		{
			m_strMsg = m_bLangulageEng?_T("Rotate Cyl Return"):_T("��ת�����Ƴ�");

			if (IsNoHardware() || RotateClyPush(TRUE))
			{
				m_nNextAutoStep = AUTO_WAIT_START_EVENT;
			}
			break;
		}

		//�ȴ���ʼ�ź�
	case AUTO_WAIT_START_EVENT:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Start Event"):_T("�ȴ���ʼ�ź�");

			if (MACHINE.m_pCleanShell != NULL &&(MACHINE.m_pCleanShell->m_bOffloadRequire || !MACHINE.m_pCleanShell->IsModuleEnable()))
			{
				m_nNextAutoStep = AUTO_TRANSFER_CYL_PUSH;
			}
			break;
		}

		//ƽ�������Ƴ�ȡ��λ
	case AUTO_TRANSFER_CYL_PUSH:
		{
			m_strMsg = m_bLangulageEng?_T("Transfer Cyl Push"):_T("ƽ�������Ƴ�ȡ��");

			if (IsNoHardware() || TransferCylPush(TRUE))
			{
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
				m_nNextAutoStep = AUTO_SEND_PICK_SIGNAL;	
			}
			break;
		}

		//����ϴ��λ��ȡ���ź�
	case AUTO_SEND_PICK_SIGNAL:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Next Module Pick"):_T("����ϴ��λ��ȡ���ź�");

			if (MACHINE.m_pCleanShell!= NULL && (MACHINE.m_pCleanShell->m_bOffloadRequire || !MACHINE.m_pCleanShell->IsModuleEnable()))
			{
				MACHINE.m_pCleanShell->m_bOffloadRequire = FALSE;
				m_nNextAutoStep = AUTO_WAIT_NEXT_MODULE_LIFT;
			}
			break;
		}	


		//�ȴ���ϴ��λ�����½�
	case AUTO_WAIT_NEXT_MODULE_LIFT:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Next Module Pick"):_T("�ȴ���ϴ��λ�����½�");

			if (MACHINE.m_pCleanShell != NULL && (MACHINE.m_pCleanShell->m_bLiftDown || !MACHINE.m_pCleanShell->IsModuleEnable()))
			{
				m_nNextAutoStep = AUTO_ROTATE_PUSH;
			}
			break;
		}

		//��ת�����Ƴ�
	case AUTO_ROTATE_PUSH:
		{
			m_strMsg = m_bLangulageEng?_T("Rotate Cyl Push"):_T("��ת���׻���");

			if (IsNoHardware() || RotateClyPush(FALSE))
			{
				m_nNextAutoStep = AUTO_SEND_SAFE_SIGNAL;
			}
			break;
		}

		//����ϴ����ȫ�ź�
	case AUTO_SEND_SAFE_SIGNAL:
		{
			m_strMsg = m_bLangulageEng?_T("Send Safe Signal"):_T("����ϴ����ȫ�ź�");

			if (MACHINE.m_pCleanShell != NULL && (MACHINE.m_pCleanShell->m_bLiftDown || !MACHINE.m_pCleanShell->IsModuleEnable()))
			{
				m_bShellVaild = TRUE;
				MACHINE.m_pCleanShell->m_bLiftDown = FALSE ;
				m_nNextAutoStep = AUTO_TRANSFER_CYL_RETURN_OFFLOAD;
			}
			break;
		}

		//ƽ�����׻���
	case AUTO_TRANSFER_CYL_RETURN_OFFLOAD:
		{
			m_strMsg = m_bLangulageEng?_T("Transfer Cyl Return"):_T("ƽ�����׻���");

			if(IsNoHardware() || TransferCylPush(FALSE))
			{
				m_nNextAutoStep = AUTO_SEND_READY_SIGNAL;
			}		
			break;
		}

		//����ת����˷�׼�����ź�
	case AUTO_SEND_READY_SIGNAL:
		{
			m_strMsg = m_bLangulageEng?_T("Send Ready Signal"):_T("����ת����˷�׼�����ź�");
			if (!m_bOffloadRequire)
			{
				m_bOffloadRequire = TRUE;
				m_nNextAutoStep = AUTO_WAIT_NEXT_MODULE_SAFE_SIGNAL;
			}
			break;
		}

		//�ȴ���ת�����ȡ������ź�
	case AUTO_WAIT_NEXT_MODULE_SAFE_SIGNAL:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Pick Finish"):_T("�ȴ���ת�����ȡ������ź�");
			if (!m_bOffloadRequire  || (MACHINE.m_pCarryShellPnp != NULL && !MACHINE.m_pCarryShellPnp->IsModuleEnable()))
			{
				Sleep(3000);
				m_bOffloadRequire = FALSE;


				m_bShellVaild = FALSE;
				m_nNextAutoStep = AUTO_WORK_END;
			}
			break;
		}

		//�������
	case AUTO_WORK_END:
		{
			m_strMsg = m_bLangulageEng?_T("Work end"):_T("�������");
			m_nNextAutoStep = AUTO_VACUUM_CLOSE;
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

		if(m_nNextAutoStep == AUTO_VACUUM_CLOSE)
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

BOOL CRunProcessCleanShellAfter::InitializeConfig(CString strProcessModule)
{
	if (!CRunProcess::InitializeConfig(strProcessModule))
	{
		return FALSE;
	}
	return TRUE;
}

void CRunProcessCleanShellAfter::GetHardwareConfig(CArray<int, int> *pInputs, CArray<int, int> *pOutputs, CArray<int, int> *pMotors)
{
	CRunProcess::GetHardwareConfig(pInputs, pOutputs, pMotors);
}


BOOL CRunProcessCleanShellAfter::CheckOutputCanActive(COutput *pOutput)
{
#ifdef _TEST_
	return TRUE;
#endif
	ASSERT(m_ORotateCylPush > -1 && m_ORotateCylReturn > -1 && m_OTransferCylPush > -1);
	if (pOutput == OUTPUTS[m_ORotateCylPush] || pOutput == OUTPUTS[m_ORotateCylReturn] || pOutput == OUTPUTS[m_OTransferCylPush])
	{
		if (MACHINE.m_pCarryShellPnp != NULL)
		{
			int nMotorZ = MACHINE.m_pCarryShellPnp->m_nMotorZ;
			if (!MOTORS[nMotorZ]->IsOriRefKnown())
			{
				BLMessageBox(_T("(M05)�������Z������ƽ��/������ת���ײ����ƶ�"), MB_ICONWARNING);
				return FALSE;
			}

			if (MOTORS[nMotorZ]->GetCurPos() > 10)
			{ 
				BLMessageBox(_T("(M05)�������Z������ǰλ�ô���10��ƽ��/������ת���ײ����ƶ�"), MB_ICONWARNING);
				return FALSE;
			}
		}	
	}

	ASSERT(m_OTransferCylPush > -1 && m_IRotateCylPush > -1 && m_IRotateCylReturn > -1);
	/*if (INPUTS[m_IRotateCylPush]->IsOn() &&  INPUTS[m_IRotateCylReturn]->IsOff())
	{
		if (pOutput == OUTPUTS[m_OTransferCylPush] && OUTPUTS[m_OTransferCylPush]->IsOn())
		{
			if ( INPUTS[m_IVacuum[0]]->IsOn() || INPUTS[m_IVacuum[1]]->IsOn())
			{
				BLMessageBox(_T("������ת�����Ƴ�ˮƽ����մ򿪣��޸����ײ��ܻ���"), MB_ICONWARNING);
				return FALSE;
			}
		}
	}*/


	ASSERT(m_IRotateCylPush > -1 && m_IRotateCylReturn > -1 
		&& m_ORotateCylPush > -1 && m_ORotateCylReturn > -1);


	int IFingerCylClose[2];
	int IFingerCylOpen[2];
	int IHaveProduct[2];

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
				BLMessageBox(_T("��๤λ���ϸ�Ӧ������ϣ�ƽ�������Ƴ���������ת���ײ��ܶ�"), MB_ICONWARNING);
				return FALSE;
			}

			if ( INPUTS[IFingerCylClose[0]]->IsOn() || INPUTS[IFingerCylClose[1]]->IsOn() 
				|| INPUTS[IFingerCylOpen[0]]->IsOff() || INPUTS[IFingerCylOpen[1]]->IsOff())
			{
				CString strKey;
				strKey.Format(_T("��Ǽн�����1��2�н���������ת���ײ��ܶ�"), MB_ICONWARNING);
				BLMessageBox(strKey);
				return FALSE;
			}

			int ILiftCylUp = MACHINE.m_pCleanShell->m_ILiftCylUp;  
			int ILiftCylDown = MACHINE.m_pCleanShell->m_ILiftCylDown;
			ASSERT(ILiftCylUp > -1 && ILiftCylDown > -1);
			if (INPUTS[ILiftCylUp]->IsOn() || INPUTS[ILiftCylDown]->IsOff())
			{
				BLMessageBox(_T("��ඥ�������Ƴ���������ת���ײ��ܶ�"), MB_ICONWARNING);
				return FALSE;
			}
		}
	}
	return TRUE;
}

/****************************************************************************************
������������ת���׶���
���������bPush:TRUE:��ת�����Ƴ���ֱ����  FALSE:��ת���׻���ˮƽȡ��
�������أ�TRUE:ִ�гɹ���FALSE:ִ��ʧ��
*****************************************************************************************/
BOOL CRunProcessCleanShellAfter::RotateClyPush(BOOL bPush)
{
	ASSERT(m_IRotateCylPush > -1 && m_IRotateCylReturn > -1 &&  m_ORotateCylPush > -1 && m_ORotateCylReturn > -1);
	
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
����������ƽ�����׶���
���������bPush:TRUE:ƽ�������Ƴ� FALSE:ƽ�����׻���
�������أ�TRUE:ִ�гɹ���FALSE:ִ��ʧ��
*****************************************************************************************/
BOOL CRunProcessCleanShellAfter:: TransferCylPush(BOOL bPush)
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
BOOL CRunProcessCleanShellAfter::OpenVacuum(BOOL bOpen)
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
BOOL CRunProcessCleanShellAfter::CheckVacuum(BOOL bHave)
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