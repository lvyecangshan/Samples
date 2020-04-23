/****************************************************************************************
文件名：RunProcessCleanShellAfter.cpp
功能描述：外壳清洗后翻转
历史修改记录：
*****************************************************************************************/
#include "StdAfx.h"
#include "RunProcessCleanShellAfter.h"
CRunProcessCleanShellAfter::CRunProcessCleanShellAfter(int nID) :CRunProcess(nID)
{
	PowerUpRestart();

	CString strKey = _T("");
	for(int i = 0; i < 2; i++)
	{
		//真空
		strKey.Format(_T("IVacuum%d"), i);
		m_InputMap.insert(pair<int*, CString>(&m_IVacuum[i], strKey));

		strKey.Format(_T("OVacuum%d"), i);
		m_OutputMap.insert(pair<int*, CString>(&m_OVacuum[i], strKey));
	}
	//翻转气缸
	m_InputMap.insert(pair<int*, CString>(&m_IRotateCylPush, _T("IRotateCylPush")));
	m_InputMap.insert(pair<int*, CString>(&m_IRotateCylReturn, _T("IRotateCylReturn")));
	m_OutputMap.insert(pair<int*, CString>(&m_ORotateCylPush, _T("ORotateCylPush")));
	m_OutputMap.insert(pair<int*, CString>(&m_ORotateCylReturn, _T("ORotateCylReturn")));
	//平移气缸
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
	m_strMsg = m_bLangulageEng?_T("Ready"):_T("准备好");

	m_bOffloadRequire = FALSE;

	m_nNextPowerUpStep = PU_WATI_NEXT_MODULE_HOME;
	m_nNextAutoStep = AUTO_TRANSFER_CYL_RETURN;
}

void CRunProcessCleanShellAfter::PowerUpOperation(void)
{
	if (!IsModuleEnable())
	{
		CRun::PowerUpOperation();
		m_strMsg = m_bLangulageEng?_T("Moudle UnEnable"):_T("模块禁用");
		return;
	}

	switch(m_nNextPowerUpStep)
	{
		//外壳清洗后搬运Z轴复位完成
	case PU_WATI_NEXT_MODULE_HOME:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Shell PNP Modle Home"):_T("外壳清洗后搬运Z轴复位完成");
			if(MACHINE.m_pCarryShellPnp != NULL && (MACHINE.m_pCarryShellPnp->m_bHomeMoveSafePos || !MACHINE.m_pCarryShellPnp->IsModuleEnable()))
			{
				//搬运Z轴复位完成
				m_nNextPowerUpStep = PU_CHECK_WITHOUT_SHELL;
			}
			break;
		}
		//检测有无壳
	case PU_CHECK_WITHOUT_SHELL:
		{
			m_strMsg = m_bLangulageEng?_T("Check Have Shell"):_T("检测无壳");
			OpenVacuum(TRUE);
			if (IsNoHardware() || CheckVacuum(FALSE))
			{
				OpenVacuum(FALSE);
				m_nNextPowerUpStep = PU_TRANSFER_CYL_RETURN;
			}
			break;
		}
		//平移气缸回退
	case PU_TRANSFER_CYL_RETURN:
		{
			m_strMsg = m_bLangulageEng?_T("Transfer Cyl Return"):_T("平移气缸回退");

			if (IsNoHardware() || TransferCylPush(FALSE))
			{
				m_nNextPowerUpStep = PU_ROTATE_RETURN;
			}
			break;
		}
		//翻转气缸回退(水平)
	case PU_ROTATE_RETURN:
		{
			m_strMsg = m_bLangulageEng?_T("Rotate Cyl Return"):_T("翻转气缸回退");

			if (IsNoHardware() || RotateClyPush(FALSE))
			{
				m_nNextPowerUpStep = PU_FINISH;
			}
			break;
		}
		//初始化完成
	case PU_FINISH:
		{
			CRun::PowerUpOperation();
			m_strMsg = m_bLangulageEng?_T("Power up finished"):_T("初始化完成");
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
		m_strMsg = m_bLangulageEng?_T("Moudle UnEnable"):_T("模块禁用");
		Sleep(500);
		return;
	}

	if (IsNoHardware())
	{
		Sleep(500);
	}

	if (m_nSaveLastStep == -1)
	{
		//第一次记录步骤和开始时间
		m_nSaveLastStep = m_nNextAutoStep;
		m_nStepStartTime = GetTickCount();
	}

	if (m_nSaveLastStep != m_nNextAutoStep)
	{
		//第一步记录开始分隔行
		if (m_nSaveLastStep == AUTO_VACUUM_CLOSE)
		{
			WriteCTLog(_T("----------开始----------"));
		}
		//每切换一次步骤记录一次CT
		DWORD nStepEndTime = GetTickCount() - m_nStepStartTime;
		CString strText = _T("");
		strText.Format(_T("%s-耗时：%d"),m_strMsg.m_strRunInfo,nStepEndTime);
		WriteCTLog (strText);
		m_nTotalTime += nStepEndTime;
		//最后一步记录总时间
		if (m_nSaveLastStep == AUTO_WORK_END)
		{
			CString str = _T("");
			str.Format(_T("总时间：%d"),m_nTotalTime);
			WriteCTLog (str);
			WriteCTLog (_T("----------结束----------"));
			m_nTotalTime = 0;
		}
		//切换步骤后重新记录新的步骤和开始时间
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
		//打开检测真空
	case AUTO_VACUUM_CLOSE:
		{
			m_strMsg = m_bLangulageEng?_T("Check Vacuun Open"):_T("关闭真空");

			if (OpenVacuum(FALSE) && (IsNoHardware() || CheckVacuum(FALSE)))
			{
				m_nNextAutoStep = AUTO_TRANSFER_CYL_RETURN;	
			}
			break;
		}

		//平移气缸回退
	case AUTO_TRANSFER_CYL_RETURN:
		{
			m_strMsg = m_bLangulageEng?_T("Transfer Cyl Return"):_T("平移气缸回退");

			if(IsNoHardware() || TransferCylPush(FALSE))
			{
				m_nNextAutoStep = AUTO_ROTATE_RETURN;
			}
			break;
		}

		//翻转气缸回退
	case AUTO_ROTATE_RETURN:
		{
			m_strMsg = m_bLangulageEng?_T("Rotate Cyl Return"):_T("翻转气缸推出");

			if (IsNoHardware() || RotateClyPush(TRUE))
			{
				m_nNextAutoStep = AUTO_WAIT_START_EVENT;
			}
			break;
		}

		//等待开始信号
	case AUTO_WAIT_START_EVENT:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Start Event"):_T("等待开始信号");

			if (MACHINE.m_pCleanShell != NULL &&(MACHINE.m_pCleanShell->m_bOffloadRequire || !MACHINE.m_pCleanShell->IsModuleEnable()))
			{
				m_nNextAutoStep = AUTO_TRANSFER_CYL_PUSH;
			}
			break;
		}

		//平移气缸推出取料位
	case AUTO_TRANSFER_CYL_PUSH:
		{
			m_strMsg = m_bLangulageEng?_T("Transfer Cyl Push"):_T("平移气缸推出取料");

			if (IsNoHardware() || TransferCylPush(TRUE))
			{
				m_nNextAutoStep = AUTO_VACUUM_OPEN;
			}
			break;
		}

		//打开检测真空
	case AUTO_VACUUM_OPEN:
		{
			m_strMsg = m_bLangulageEng?_T("Check Vacuun Open"):_T("打开检测真空");

			OpenVacuum(TRUE);
			if (IsNoHardware() || m_bDryRun || CheckVacuum(TRUE))
			{
				m_nNextAutoStep = AUTO_SEND_PICK_SIGNAL;	
			}
			break;
		}

		//给清洗工位发取料信号
	case AUTO_SEND_PICK_SIGNAL:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Next Module Pick"):_T("给清洗工位发取料信号");

			if (MACHINE.m_pCleanShell!= NULL && (MACHINE.m_pCleanShell->m_bOffloadRequire || !MACHINE.m_pCleanShell->IsModuleEnable()))
			{
				MACHINE.m_pCleanShell->m_bOffloadRequire = FALSE;
				m_nNextAutoStep = AUTO_WAIT_NEXT_MODULE_LIFT;
			}
			break;
		}	


		//等待清洗工位顶升下降
	case AUTO_WAIT_NEXT_MODULE_LIFT:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Next Module Pick"):_T("等待清洗工位顶升下降");

			if (MACHINE.m_pCleanShell != NULL && (MACHINE.m_pCleanShell->m_bLiftDown || !MACHINE.m_pCleanShell->IsModuleEnable()))
			{
				m_nNextAutoStep = AUTO_ROTATE_PUSH;
			}
			break;
		}

		//翻转气缸推出
	case AUTO_ROTATE_PUSH:
		{
			m_strMsg = m_bLangulageEng?_T("Rotate Cyl Push"):_T("翻转气缸回退");

			if (IsNoHardware() || RotateClyPush(FALSE))
			{
				m_nNextAutoStep = AUTO_SEND_SAFE_SIGNAL;
			}
			break;
		}

		//给清洗发安全信号
	case AUTO_SEND_SAFE_SIGNAL:
		{
			m_strMsg = m_bLangulageEng?_T("Send Safe Signal"):_T("给清洗发安全信号");

			if (MACHINE.m_pCleanShell != NULL && (MACHINE.m_pCleanShell->m_bLiftDown || !MACHINE.m_pCleanShell->IsModuleEnable()))
			{
				m_bShellVaild = TRUE;
				MACHINE.m_pCleanShell->m_bLiftDown = FALSE ;
				m_nNextAutoStep = AUTO_TRANSFER_CYL_RETURN_OFFLOAD;
			}
			break;
		}

		//平移气缸回退
	case AUTO_TRANSFER_CYL_RETURN_OFFLOAD:
		{
			m_strMsg = m_bLangulageEng?_T("Transfer Cyl Return"):_T("平移气缸回退");

			if(IsNoHardware() || TransferCylPush(FALSE))
			{
				m_nNextAutoStep = AUTO_SEND_READY_SIGNAL;
			}		
			break;
		}

		//给翻转后搬运发准备好信号
	case AUTO_SEND_READY_SIGNAL:
		{
			m_strMsg = m_bLangulageEng?_T("Send Ready Signal"):_T("给翻转后搬运发准备好信号");
			if (!m_bOffloadRequire)
			{
				m_bOffloadRequire = TRUE;
				m_nNextAutoStep = AUTO_WAIT_NEXT_MODULE_SAFE_SIGNAL;
			}
			break;
		}

		//等待翻转后搬运取料完成信号
	case AUTO_WAIT_NEXT_MODULE_SAFE_SIGNAL:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Pick Finish"):_T("等待翻转后搬运取料完成信号");
			if (!m_bOffloadRequire  || (MACHINE.m_pCarryShellPnp != NULL && !MACHINE.m_pCarryShellPnp->IsModuleEnable()))
			{
				Sleep(3000);
				m_bOffloadRequire = FALSE;


				m_bShellVaild = FALSE;
				m_nNextAutoStep = AUTO_WORK_END;
			}
			break;
		}

		//工作完成
	case AUTO_WORK_END:
		{
			m_strMsg = m_bLangulageEng?_T("Work end"):_T("工作完成");
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
			m_strModuleCTtemp.Format(_T("【%s】单步CT：%.2f\r\n\r\n-----------------------------【%s】单次循环CT：%.2f-----------------------------\r\n"), m_strMsg.GetInfo(), (double)(m_dwModuleStepCT/1000.0), m_strName, (double)(m_dwModuleCT/1000.0));
			m_dwModuleCT = 0;
		}
		else
		{
			m_strModuleCTtemp.Format(_T("【%s】单步CT：%.2f\r\n"), m_strMsg.GetInfo(), (double)(m_dwModuleStepCT/1000.0));
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
				BLMessageBox(_T("(M05)外壳移载Z轴电机，平移/清洁后旋转气缸不能移动"), MB_ICONWARNING);
				return FALSE;
			}

			if (MOTORS[nMotorZ]->GetCurPos() > 10)
			{ 
				BLMessageBox(_T("(M05)外壳移载Z轴电机当前位置大于10，平移/清洁后旋转气缸不能移动"), MB_ICONWARNING);
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
				BLMessageBox(_T("清洁后旋转气缸推出水平，真空打开，无杆气缸不能回退"), MB_ICONWARNING);
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
				BLMessageBox(_T("清洁工位有料感应检测有料，平移气缸推出，清洁后旋转气缸不能动"), MB_ICONWARNING);
				return FALSE;
			}

			if ( INPUTS[IFingerCylClose[0]]->IsOn() || INPUTS[IFingerCylClose[1]]->IsOn() 
				|| INPUTS[IFingerCylOpen[0]]->IsOff() || INPUTS[IFingerCylOpen[1]]->IsOff())
			{
				CString strKey;
				strKey.Format(_T("外壳夹紧气缸1、2夹紧，清洁后旋转气缸不能动"), MB_ICONWARNING);
				BLMessageBox(strKey);
				return FALSE;
			}

			int ILiftCylUp = MACHINE.m_pCleanShell->m_ILiftCylUp;  
			int ILiftCylDown = MACHINE.m_pCleanShell->m_ILiftCylDown;
			ASSERT(ILiftCylUp > -1 && ILiftCylDown > -1);
			if (INPUTS[ILiftCylUp]->IsOn() || INPUTS[ILiftCylDown]->IsOff())
			{
				BLMessageBox(_T("清洁顶升气缸推出，清洁后旋转气缸不能动"), MB_ICONWARNING);
				return FALSE;
			}
		}
	}
	return TRUE;
}

/****************************************************************************************
函数描述：翻转气缸动作
输入参数：bPush:TRUE:翻转气缸推出竖直放料  FALSE:翻转气缸回退水平取料
函数返回：TRUE:执行成功，FALSE:执行失败
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
函数描述：平移气缸动作
输入参数：bPush:TRUE:平移气缸推出 FALSE:平移气缸回退
函数返回：TRUE:执行成功，FALSE:执行失败
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
函数描述：打开真空
输入参数：bPush:TRUE:打开真空 FALSE:关闭真空
函数返回：TRUE:执行成功，FALSE:执行失败
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
函数描述：真空检测有料
输入参数：bHave:TRUE:检测有料 FALSE:检测无料
函数返回：TRUE:执行成功，FALSE:执行失败
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