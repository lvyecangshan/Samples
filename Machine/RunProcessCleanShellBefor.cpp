/****************************************************************************************
文件名：RunProcessCleanShellBefor.cpp
功能描述：外壳清洗模组
历史修改记录：
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
		m_InputMap.insert(pair<int*, CString>(&m_IShellDirection[i], strKey));   //壳口朝向检测

		//真空
		strKey.Format(_T("IVacuum%d"), i);									
		m_InputMap.insert(pair<int*, CString>(&m_IVacuum[i], strKey));       //吸外壳真空感应1
		strKey.Format(_T("OVacuum%d"), i);									
		m_OutputMap.insert(pair<int*, CString>(&m_OVacuum[i], strKey));       //吸外壳真空感应1
	}

	m_InputMap.insert(pair<int*, CString>(&m_IRotateCylPush, _T("IRotateCylPush")));//翻转气缸推出感应
	m_InputMap.insert(pair<int*, CString>(&m_IRotateCylReturn, _T("IRotateCylReturn")));//翻转气缸回退感应
	m_OutputMap.insert(pair<int*, CString>(&m_ORotateCylPush, _T("ORotateCylPush")));//翻转气缸推出电磁阀
	m_OutputMap.insert(pair<int*, CString>(&m_ORotateCylReturn, _T("ORotateCylReturn")));//翻转气缸回退电磁阀

	m_InputMap.insert(pair<int*, CString>(&m_ITransferCylReturn, _T("ITransferCylReturn")));//无杆气缸回退感应
	m_InputMap.insert(pair<int*, CString>(&m_ITransferCylPush, _T("ITransferCylPush")));//无杆气缸推出感应

	m_OutputMap.insert(pair<int*, CString>(&m_OTransferCylPush, _T("OTransferCylPush")));//无杆气缸推出电磁阀

	m_InputMap.insert(pair<int*, CString>(&m_IManualBtn, _T("IManualBtn")));//按钮
	m_OutputMap.insert(pair<int*, CString>(&m_OManualBtnLight, _T("OManualBtnLight")));//按钮灯

	APP_Add_Output(&m_OTransferCylReturn,_T("OTransferCylReturn"));
	
}

CRunProcessCleanShellBefor::~CRunProcessCleanShellBefor(void)
{
}

void CRunProcessCleanShellBefor::PowerUpRestart()
{
	CRunProcess::PowerUpRestart();
	m_strMsg = m_bLangulageEng?_T("Ready"):_T("准备好");
	m_bRequirePallet = FALSE;
	m_nNextPowerUpStep = PU_WATI_FRONT_MODULE_HOME;
	m_nNextAutoStep = AUTO_TRANSFER_CYL_RETURN;
}

void CRunProcessCleanShellBefor::PowerUpOperation(void)
{
	if (!IsModuleEnable())
	{
		CRun::PowerUpOperation();
		m_strMsg = m_bLangulageEng?_T("Moudle UnEnable"):_T("模块禁用");
		return;
	}

	switch(m_nNextPowerUpStep)
	{
		//等待上壳机械手回零
	case PU_WATI_FRONT_MODULE_HOME:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Shell PNP Modle Home"):_T("上壳机械手Z轴复位完成");

			if(MACHINE.m_pOnloadShellPnp != NULL && (MACHINE.m_pOnloadShellPnp->m_bHomeMoveSafePos || !MACHINE.m_pOnloadShellPnp->IsModuleEnable()))
			{
				//上壳机械手Z轴复位完成信号
				m_nNextPowerUpStep = PU_CHECK_HAVE_SHELL;
			}
			break;
		}

		//检测有无壳
	case PU_CHECK_HAVE_SHELL:
		{
			m_strMsg = m_bLangulageEng?_T("Check Have Shell"):_T("检测无壳");

			OpenVacuum(TRUE);
			if (IsNoHardware() || m_bDryRun || CheckVacuum(FALSE))
			{
				OpenVacuum(FALSE);
				m_nNextPowerUpStep = PU_ROTATE_RETURN;
			}
			break;
		}

		//翻转气缸回退
	case PU_ROTATE_RETURN:
		{
			m_strMsg = m_bLangulageEng?_T("Rotate Cyl Return"):_T("翻转气缸回退");

			if (IsNoHardware() || RotateClyPush(FALSE))
			{
				m_nNextPowerUpStep = PU_TRANSFER_CYL_RETURN;
			}
			break;
		}

		//传送气缸回退
	case PU_TRANSFER_CYL_RETURN:
		{
			m_strMsg = m_bLangulageEng?_T("Transfer Cyl Return"):_T("传送气缸回退");

			if (IsNoHardware() || TransferCylPush(FALSE))
			{
				m_nNextPowerUpStep = PU_BUTTON_RESET;
			}
			break;
		}
	
		//按钮灯复位
	case PU_BUTTON_RESET:
		{
			m_strMsg = m_bLangulageEng?_T("Button Reset"):_T("按钮灯复位");
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

void CRunProcessCleanShellBefor::AutoOperation()
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
		if (m_nSaveLastStep == AUTO_TRANSFER_CYL_RETURN)
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

	switch(m_nNextAutoStep)
	{
		//无杆气缸回退
	case AUTO_TRANSFER_CYL_RETURN:
		{
			m_strMsg = m_bLangulageEng?_T("Transfer Cyl Return"):_T("无杆气缸回退");

			if(IsNoHardware() || TransferCylPush(FALSE))
			{
				m_nNextAutoStep = AUTO_ROTATE_RETURN;
			}
			break;
		}

		//翻转气缸回退
	case AUTO_ROTATE_RETURN:
		{
			m_strMsg = m_bLangulageEng?_T("Rotate Cyl Return"):_T("翻转气缸回退");

			if (IsNoHardware() || RotateClyPush(FALSE))
			{
				m_nNextAutoStep = AUTO_SEND_NEED_REQUIRE_EVENT;
			}
			break;
		}

		//发送要料请求信号
	case AUTO_SEND_NEED_REQUIRE_EVENT:
		{
			m_strMsg = m_bLangulageEng?_T("Send Need Require Event"):_T("发送要料请求信号");
			if(!m_bRequirePallet)
			{
				m_bRequirePallet = TRUE;
				m_nNextAutoStep = AUTO_WAIT_START_EVENT;
			}
			break;
		}

		//等待开始信号
	case AUTO_WAIT_START_EVENT:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Start Event"):_T("等待开始信号");

			if (!m_bRequirePallet  || (MACHINE.m_pOnloadShellPnp != NULL && !MACHINE.m_pOnloadShellPnp->IsModuleEnable()))
			{
				m_bShellVaild = TRUE;
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
				m_nNextAutoStep = AUTO_CHECK_SHELL_DIRECTION;	
			}
			break;
		}

		//检测壳口方向
	case AUTO_CHECK_SHELL_DIRECTION:
		{
			m_strMsg = m_bLangulageEng?_T("Check Shell Direction"):_T("检测壳口方向");

			if (IsNoHardware() || m_bDryRun
				|| (m_IShellDirection[0] > -1 && INPUTS[m_IShellDirection[0]]->IsOn() 
				&& m_IShellDirection[1] > -1 && INPUTS[m_IShellDirection[1]]->IsOn()))
			{
				m_nNextAutoStep = AUTO_TRANSFER_CYL_PUSH;
			}
			else
			{
				//检测壳口方向反向，请取走壳
				ShowMessage(MSG_DUST_BEFORE_CHECK_SHELL_DIRECTION_REVERSE);
				if (m_OManualBtnLight > -1 && MACHINE.m_nTowerBuzzer > -1)
				{
					OUTPUTS[m_OManualBtnLight]->On();
					MACHINE.SetBuzzerStatue(TRUE);
					BLMessageBox(_T("请人工取走放反铝壳"));
					MACHINE.SetBuzzerStatue(FALSE);
					m_nNextAutoStep = AUTO_MANUAL_PICK_SHELL_CLOSE_VACUUM;
				}
			}
			break;
		}

		//  等待人工取料完成
	case AUTO_MANUAL_PICK_SHELL_CLOSE_VACUUM:
		{
			m_strMsg = m_bLangulageEng?_T("Check Shell Direction"):_T("关真空，等待人工取壳");
			if (ButtonCheck())
			{
				OpenVacuum(FALSE);
				m_nNextAutoStep = AUTO_WAIT_MANUAL_PICK_SHELL_FINISH;
			}
			break;
		}
		//  等待人工取料完成
	case AUTO_WAIT_MANUAL_PICK_SHELL_FINISH:
		{
			m_strMsg = m_bLangulageEng?_T("Check Shell Direction"):_T("等待人工取料完成");
			OpenVacuum(TRUE);
			if (CheckVacuum(FALSE) && !CheckDirection())
			{
				OpenVacuum(FALSE);
				m_bShellVaild = FALSE;
				m_nNextAutoStep = AUTO_TRANSFER_CYL_RETURN;
			}
			break;
		}

		//无杆气缸移动到下料位
	case AUTO_TRANSFER_CYL_PUSH:
		{
			m_strMsg = m_bLangulageEng?_T("Transfer Cyl Push"):_T("无杆气缸移动到下料位");

			if (IsNoHardware() || TransferCylPush(TRUE))
			{
				m_nNextAutoStep = AUTO_WAIT_NEXT_MODULE_PICK;
			}			
			break;
		}

		//等待清洗工位要料
	case AUTO_WAIT_NEXT_MODULE_PICK:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Next Module Pick"):_T("等待清洗工位要料");

			if (MACHINE.m_pCleanShell != NULL && (MACHINE.m_pCleanShell->m_bRequirePallet || !MACHINE.m_pCleanShell->IsModuleEnable()))
			{
				m_nNextAutoStep = AUTO_ROTATE_PUSH;
			}
			break;
		}

		//翻转气缸推出
	case AUTO_ROTATE_PUSH:
		{
			m_strMsg = m_bLangulageEng?_T("Rotate Cyl Push"):_T("翻转气缸推出");

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

		//等待下一模组夹紧信号  清洗模组取数据
	case AUTO_WAIT_NEXT_MODULE_FINGER_CLOSE:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Pick Finish"):_T("等待清洗模组夹紧信号");

			if (MACHINE.m_pCleanShell != NULL && (MACHINE.m_pCleanShell->m_bFingerClose || !MACHINE.m_pCleanShell->IsModuleEnable()))
			{
				m_nNextAutoStep = AUTO_VACUUM_CLOSE;
			}
			break;
		}

		//真空关闭
	case AUTO_VACUUM_CLOSE:
		{
			m_strMsg = m_bLangulageEng?_T("Vacuum Close"):_T("真空关闭");

			OpenVacuum(FALSE);
			if (IsNoHardware() || CheckVacuum(FALSE))
			{
				m_nNextAutoStep = AUTO_SEND_SAFE_SIGNAL;
			}
			break;
		}
		//给清洗发送下料完成信号
	case AUTO_SEND_SAFE_SIGNAL:
		{
			m_strMsg = m_bLangulageEng?_T("Send Offload Finish"):_T("给清洗发送下料完成信号");

			if (MACHINE.m_pCleanShell != NULL && (MACHINE.m_pCleanShell->m_bFingerClose || !MACHINE.m_pCleanShell->IsModuleEnable()))
			{
				m_bShellVaild = FALSE;
				MACHINE.m_pCleanShell->m_bFingerClose = FALSE;
				m_nNextAutoStep = AUTO_TRANSFER_CYL_RETURN_OFFLOAD;
			}
			break;
		}
		//无杆气缸移动到上料位
	case AUTO_TRANSFER_CYL_RETURN_OFFLOAD:
		{
			m_strMsg = m_bLangulageEng?_T("Transfer Cyl Return Offload"):_T("无杆气缸移动到上料位");

			if (IsNoHardware() || TransferCylPush(FALSE))
			{
				m_nNextAutoStep = AUTO_WORK_END;
			}
			break;
		}
		//工作完成
	case AUTO_WORK_END:
		{
			m_strMsg = m_bLangulageEng?_T("Work end"):_T("工作完成");
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
				BLMessageBox(_T("(M13)外壳上料Z轴没有复位，无杆/旋转气缸不能移动"), MB_ICONWARNING);
				return FALSE;
			}

			if (MOTORS[nMotorZ]->GetCurPos() > 10)
			{ 
				BLMessageBox(_T("(M13)外壳上料Z轴当前位置大于10，无杆/旋转气缸不能移动"), MB_ICONWARNING);
				return FALSE;	
			}
		}
	}

	if(m_OTransferCylPush > -1 && OUTPUTS[m_OTransferCylPush]->IsOff())
	{
		if((m_ORotateCylPush > -1 && pOutput == OUTPUTS[m_ORotateCylPush] && pOutput->IsOff())
			|| (m_ORotateCylReturn > -1 && pOutput == OUTPUTS[m_ORotateCylReturn] && pOutput->IsOn()))
		{
			BLMessageBox(_T("移栽气缸处于回退状态，旋转气缸不能推出"), MB_ICONWARNING);
			return FALSE;
		}
	}

	if((m_ORotateCylPush > -1 && OUTPUTS[m_ORotateCylPush]->IsOn() || m_IRotateCylPush > -1 && INPUTS[m_IRotateCylPush]->IsOn()) 
		&&  m_OTransferCylPush > -1 && OUTPUTS[m_OTransferCylPush]->IsOff())
	{
		if(pOutput == OUTPUTS[m_OTransferCylPush])
		{
			BLMessageBox(_T("旋转气缸处于推出状态，不能直接推出无杆气缸"), MB_ICONWARNING);
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
			BLMessageBox(_T("移栽、旋转气缸处于推出状态且真空已开启，可能有铝壳，请先将真空关闭"), MB_ICONWARNING);
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
				BLMessageBox(_T("清洁工位有料感应检测有料，无杆气缸推出，旋转气缸不能动"), MB_ICONWARNING);
				return FALSE;
			}

			if ( INPUTS[IFingerCylClose[0]]->IsOn() || INPUTS[IFingerCylClose[1]]->IsOn() 
				|| INPUTS[IFingerCylOpen[0]]->IsOff() || INPUTS[IFingerCylOpen[1]]->IsOff())
			{
				BLMessageBox(_T("外壳夹紧气缸1、2夹紧，旋转气缸不能动"), MB_ICONWARNING);
				return FALSE;
			}

			int ILiftCylUp = MACHINE.m_pCleanShell->m_ILiftCylUp;  
			int ILiftCylDown = MACHINE.m_pCleanShell->m_ILiftCylDown;
			ASSERT(ILiftCylUp > -1 && ILiftCylDown > -1);
			if (INPUTS[ILiftCylUp]->IsOn() || INPUTS[ILiftCylDown]->IsOff())
			{
				BLMessageBox(_T("清洁顶升气缸推出，旋转气缸不能动"), MB_ICONWARNING);
				return FALSE;
			}
		}
	}
	return TRUE;
}


/****************************************************************************************
函数描述：翻转气缸动作
输入参数：bPush:TRUE:翻转气缸推出水平放料  FALSE:翻转气缸回退竖直取料
函数返回：TRUE:执行成功，FALSE:执行失败
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
函数描述：无杆气缸动作
输入参数：bPush:TRUE:无杆气缸推出 FALSE:无杆气缸回退
函数返回：TRUE:执行成功，FALSE:执行失败
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
函数描述：打开真空
输入参数：bPush:TRUE:打开真空 FALSE:关闭真空
函数返回：TRUE:执行成功，FALSE:执行失败
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
函数描述：真空检测有料
输入参数：bHave:TRUE:检测有料 FALSE:检测无料
函数返回：TRUE:执行成功，FALSE:执行失败
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
函数描述：壳口检测
输入参数：
函数返回：TRUE:壳口向正，FALSE:壳口朝反
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
函数描述：按钮按下检测
输入参数：
函数返回：TRUE:按钮按下，FALSE:按钮未按下
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