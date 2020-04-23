/****************************************************************************************
文件名：RunProcessCarryShellPnP.h
功能描述：入铝壳搬运机械手
历史修改记录：
*****************************************************************************************/
#include "StdAfx.h"
#include "RunProcessCarryShellPnP.h"

CRunProcessCarryShellPnP::CRunProcessCarryShellPnP(int nID) :
CRunProcess(nID)
{
	PowerUpRestart();

	CString strKey;
	for(int i = 0; i < 2; i++)
	{
		strKey.Format(_T("OVacumCyl%d"), i);
		m_OutputMap.insert(pair<int*, CString>(&m_OVacumCyl[i], strKey));

		strKey.Format(_T("ICheckVacum%d"), i);
		m_InputMap.insert(pair<int*, CString>(&m_ICheckVacum[i], strKey));
	}
	//----搬运气缸--
	m_InputMap.insert(pair<int*, CString>(&m_IPushCylPush, _T("IPushCylPush")));//搬运气缸推出检测
	m_InputMap.insert(pair<int*, CString>(&m_IPushCylBack, _T("IPushCylBack")));//搬运气缸回退检测
	m_OutputMap.insert(pair<int*, CString>(&m_OPushCylPush, _T("OPushCylPush")));//搬运气缸推出电磁阀
	m_OutputMap.insert(pair<int*, CString>(&m_OPushCylBack, _T("OPushCylBack")));//搬运气缸回退电磁阀

	m_OutputMap.insert(pair<int*, CString>(&m_OBreakVacum, _T("OBreakVacum")));
	m_MotorMap.insert(pair<int*, CString>(&m_nMotorZ, _T("MotorZ")));
	m_MotorMap.insert(pair<int*, CString>(&m_nMotorX, _T("MotorX")));
}

CRunProcessCarryShellPnP::~CRunProcessCarryShellPnP(void)
{
}

void CRunProcessCarryShellPnP::PowerUpRestart()
{
	CRunProcess::PowerUpRestart();
	m_strMsg = m_bLangulageEng?_T("Ready"):_T("准备好");
	m_bHomeMoveSafePos = FALSE;
	m_nNextPowerUpStep = PU_CHECK_HAVE_SHELL;
	m_nNextAutoStep = AUTO_MOVE_X_TO_ONLOAD_POS;
}

void CRunProcessCarryShellPnP::PowerUpOperation(void)
{
	if (!IsModuleEnable())
	{
		CRun::PowerUpOperation();
		m_strMsg = m_bLangulageEng?_T("Moudle UnEnable"):_T("模块禁用");
		return;
	}

	switch(m_nNextPowerUpStep)
	{
		//检测是否有壳
	case PU_CHECK_HAVE_SHELL:
		{
			m_strMsg = m_bLangulageEng?_T("Check Have Shell"):_T("检测有无壳");

			if (::IsNoHardware() || m_bDryRun || CheckVacum(-1, FALSE))
			{
				m_nNextPowerUpStep = PU_CLOSE_VACUM;
			}
			break;
		}
		//关闭真空
	case PU_CLOSE_VACUM:
		{
			m_strMsg = m_bLangulageEng?_T("Close Vacum"):_T("关闭真空与破真空");

			if(::IsNoHardware() || (VacumOpen(-1, FALSE) && BreakVacum( FALSE)))
			{
				m_nNextPowerUpStep = PU_MOVE_Z_TO_HOME;
			}
			break;
		}
		//Z轴电机复位
	case PU_MOVE_Z_TO_HOME:
		{
			m_strMsg = m_bLangulageEng?_T("Move Z To Home"):_T("Z轴电机复位");

			if(IsNoHardware() || (m_nMotorZ > -1 && HomeMotor(MOTORS[m_nMotorZ])))
			{
				m_nNextPowerUpStep = PU_MOVE_Z_TO_SAFE_POS;
			}
			break;
		}
		//移动Z轴到安全位
	case PU_MOVE_Z_TO_SAFE_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Move Z To Safe Pos"):_T("移动Z轴到安全位");

			if (m_nMotorZ > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorZ, Z_MOTOR_SAFE_POS))
			{
				m_bHomeMoveSafePos = TRUE;
				m_nNextPowerUpStep = PU_MOVE_X_TO_HOME;
			}
			break;
		}
		//X轴电机复位
	case PU_MOVE_X_TO_HOME:
		{
			m_strMsg = m_bLangulageEng?_T("Move X To Home"):_T("X轴电机复位");

			if (m_nMotorX > -1 && HomeMotor(MOTORS[m_nMotorX]))
			{
				m_nNextPowerUpStep = PU_MOVE_X_TO_TO_ONLOAD_POS;
			}
			break;
		}
		//X轴电机移动到取料位
	case PU_MOVE_X_TO_TO_ONLOAD_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Move X To Onload Pos"):_T("X轴电机移动到取料位");

			if (m_nMotorX > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorX, X_MOTOR_ONLOAD_LOC))
			{
				m_nNextPowerUpStep = PU_PUSH_CYL_RETURN;
			}
			break;
		}
	case PU_PUSH_CYL_RETURN:
		{
			m_strMsg = m_bLangulageEng?_T("Push Cyl Return"):_T("搬运气缸回退");

			if (IsNoHardware() || PushCylPush(FALSE))
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

void CRunProcessCarryShellPnP::AutoOperation()
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
		if (m_nSaveLastStep == AUTO_MOVE_X_TO_ONLOAD_POS)
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
		//移动X轴到取料位
	case AUTO_MOVE_X_TO_ONLOAD_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Move X To Onload Pos"):_T("移动X轴到取料位");

			if(m_OPushCylPush > -1 && m_OPushCylBack > -1)
			{
				OUTPUTS[m_OPushCylPush]->Off();
				OUTPUTS[m_OPushCylBack]->On();
			}
			if (m_nMotorX > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorX, X_MOTOR_ONLOAD_LOC))
			{
				m_nNextAutoStep = AUTO_CYL_PUSH_CYL_RETURN;
			}
			break;
		}

	case AUTO_CYL_PUSH_CYL_RETURN:
		{
			m_strMsg = m_bLangulageEng?_T("Push Cyl Return"):_T("推出气缸回退");

			if(IsNoHardware() || PushCylPush(FALSE))
			{
				m_nNextAutoStep = AUTO_WAIT_ONLOAD_SINGLE;
			}
			break;
		}

		//等待前一工位准备好信号
	case AUTO_WAIT_ONLOAD_SINGLE:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Onload Single"):_T("等待前一工位准备好信号");

			if (MACHINE.m_pCleanAfter != NULL && (MACHINE.m_pCleanAfter->m_bOffloadRequire || !MACHINE.m_pCleanAfter->IsModuleEnable()))
			{
				m_nNextAutoStep = AUTO_MOVE_Z_TO_PICK_POS;
			}
			else
			{
				m_nNextAutoStep = AUTO_MOVE_Z_TO_WAIT_PICK_POS;
			}
			break;
		}

		//Z轴移动到取料等待位
	case AUTO_MOVE_Z_TO_WAIT_PICK_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Move Z To Wait Pick Pos"):_T("Z轴移动到取料等待位");

			if(m_nMotorZ > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorZ, Z_MOTOR_WAIT_PICK_POS))
			{
				m_nNextAutoStep = AUTO_WAIT_ONLOAD_SINGLE_EX;
			}
			break;
		}

		//Z轴移动到等待位后等待前一工位准备好
	case AUTO_WAIT_ONLOAD_SINGLE_EX:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Onload Single"):_T("Z轴移动到等待位后等待前一工位准备好");

			if (MACHINE.m_pCleanAfter != NULL && (MACHINE.m_pCleanAfter->m_bOffloadRequire || !MACHINE.m_pCleanAfter->IsModuleEnable()))
			{
				m_nNextAutoStep = AUTO_MOVE_Z_TO_PICK_POS;
			}
			break;
		}

		//移动Z轴到取料位
	case AUTO_MOVE_Z_TO_PICK_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Move Z To Pick Pos"):_T("移动Z轴到取料位");

			if(m_nMotorZ > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorZ, Z_MOTOR_PICK_POS))
			{
				m_nNextAutoStep = AUTO_OPEN_VACUM;
			}
			break;
		}

		//打开真空
	case AUTO_OPEN_VACUM:
		{
			m_strMsg = m_bLangulageEng?_T("Open Vacum"):_T("打开真空");

			if(::IsNoHardware() || m_bDryRun || VacumOpen(-1, TRUE))
			{
				if (MACHINE.m_pCleanAfter != NULL && MACHINE.m_pCleanAfter->OpenVacuum(FALSE))
				{
					m_nNextAutoStep = AUTO_MOVE_Z_TO_SAFE_POS;
				}
			}
			break;
		}

		//移动Z轴到安全位
	case AUTO_MOVE_Z_TO_SAFE_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Move Z To Safe Pos"):_T("移动Z轴到安全位");

			if (m_nMotorZ > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorZ, Z_MOTOR_SAFE_POS))
			{
				m_nNextAutoStep = AUTO_SEND_MOVE_Z_TO_SAFE_SINGLE_TO_FRONT_MODULE;
			}
			break;
		}

		//给前一模组发送移动到安全位信号
	case AUTO_SEND_MOVE_Z_TO_SAFE_SINGLE_TO_FRONT_MODULE:
		{
			m_strMsg = m_bLangulageEng?_T("Send Front Module Safe Single"):_T("给前一模组发送移动到安全位信号");

			if(MACHINE.m_pCleanAfter != NULL && (MACHINE.m_pCleanAfter->m_bOffloadRequire || !MACHINE.m_pCleanAfter->IsModuleEnable()))
			{
				m_bShellVaild = TRUE;
				MACHINE.m_pCleanAfter->m_bOffloadRequire = FALSE;			
				m_nNextAutoStep = AUTO_MOVE_X_TO_OFFLAOD_POS;
			}
			break;
		}

		//移动X轴到放料位
	case AUTO_MOVE_X_TO_OFFLAOD_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Move X To Wait Pos"):_T("移动X轴到放料位");

			if (m_OPushCylBack > -1 && m_OPushCylPush > -1)
			{
				OUTPUTS[m_OPushCylBack]->Off();
				OUTPUTS[m_OPushCylPush]->On();
			}
			if (m_nMotorX > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorX, X_MOTOR_PNP_PUT_POS))
			{
				m_nNextAutoStep = AUTO_PUSH_CYL_PUSH;
			}
			break;
		}
	case AUTO_PUSH_CYL_PUSH:
		{
			m_strMsg = m_bLangulageEng?_T("Push Cyl Push"):_T("搬运气缸推出");

			if(IsNoHardware() || PushCylPush(TRUE))
			{
				m_nNextAutoStep = AUTO_JUDGE_WHICH_MODULE_NEED_SHELL;			
			}
			break;
		}
		//等待开始工作信号
	case AUTO_JUDGE_WHICH_MODULE_NEED_SHELL:
		{
			m_strMsg = m_bLangulageEng?_T("Judge Which Module Need Shell"):_T("等待开始工作信号");

			if (MACHINE.m_pBatteryInShell[0] != NULL && MACHINE.m_pBatteryInShell[1] != NULL 
				&& (MACHINE.m_pBatteryInShell[0]->m_bRequirePallet || !MACHINE.m_pBatteryInShell[0]->IsModuleEnable())
				&& (MACHINE.m_pBatteryInShell[1]->m_bRequirePallet || !MACHINE.m_pBatteryInShell[1]->IsModuleEnable()))
			{
				m_nNextAutoStep = AUTO_OFFLOAD_CHECK_VACUM;
			}
			else
			{
				m_nNextAutoStep = AUTO_Z_AXIS_MOVE_TO_PLACE_WAIT_LOC;
			}
			break;
		}
	case AUTO_Z_AXIS_MOVE_TO_PLACE_WAIT_LOC:
		{
			m_strMsg = m_bLangulageEng?_T("z axis move to wait place loc"):_T("Z轴到等待放料位");
			if (m_nMotorZ > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorZ, Z_MOTOR_WAIT_PLACE_POS))
			{
				m_nNextAutoStep = AUTO_CHECK_REQUIRE_AGAIN;
			}
			break;
		}
	case AUTO_CHECK_REQUIRE_AGAIN:
		{
			m_strMsg = m_bLangulageEng?_T("Check Require Again"):_T("再次检测请求信号");
			if (MACHINE.m_pBatteryInShell[0] != NULL && MACHINE.m_pBatteryInShell[1] != NULL 
				&& (MACHINE.m_pBatteryInShell[0]->m_bRequirePallet || !MACHINE.m_pBatteryInShell[0]->IsModuleEnable())
				&& (MACHINE.m_pBatteryInShell[1]->m_bRequirePallet || !MACHINE.m_pBatteryInShell[1]->IsModuleEnable()))
			{
				m_nNextAutoStep = AUTO_OFFLOAD_CHECK_VACUM;
			}
			break;
		}
		// 下料前检查真空
	case AUTO_OFFLOAD_CHECK_VACUM:
		{
			m_strMsg = m_bLangulageEng?_T("Offload Check Vacum"):_T("下料前检查真空");

			if (IsNoHardware() || m_bDryRun || CheckVacum(-1,TRUE))
			{
				m_nNextAutoStep = AUTO_MOVE_Z_TO_PUT_POS;
			}
			break;
		}
		//移动Z轴到放料位
	case AUTO_MOVE_Z_TO_PUT_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Move Z To Place Pos"):_T("移动Z轴到放料位");

			if (m_nMotorZ > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorZ, Z_MOTOR_PLACE_POS))
			{
				m_nNextAutoStep = AUTO_SEND_PLACE_FINISH_SINGLE;
			}
			break;
		}

		//发送放料完成信号
	case AUTO_SEND_PLACE_FINISH_SINGLE:
		{
			m_strMsg = m_bLangulageEng?_T("Send Place Finish Single"):_T("发送放料完成信号");

			for (int i = 0; i <2; i++)
			{
				if(MACHINE.m_pBatteryInShell[i] != NULL && MACHINE.m_pBatteryInShell[i]->m_bRequirePallet)
				{ 
					MACHINE.m_pBatteryInShell[i]->m_bRequirePallet = FALSE;
				}
			}
			m_nNextAutoStep = AUTO_WAIT_BATTERY_CYL_CLOSE;
			break;
		}

	
	case AUTO_WAIT_BATTERY_CYL_CLOSE:
		{
			m_strMsg = m_bLangulageEng?_T("wait next module cyl close"):_T("等待入壳工位整列气缸加紧");
			if (MACHINE.m_pBatteryInShell[0] != NULL&& MACHINE.m_pBatteryInShell[1] != NULL 
				&& (MACHINE.m_pBatteryInShell[0]->m_bCylClosed || !MACHINE.m_pBatteryInShell[0]->IsModuleEnable())
				&& (MACHINE.m_pBatteryInShell[1]->m_bCylClosed|| !MACHINE.m_pBatteryInShell[1]->IsModuleEnable()))
			{
				m_nNextAutoStep = AUTO_CLOSE_VACUM;
			}
			break;
		}
		//关闭真空
	case AUTO_CLOSE_VACUM:
		{
			m_strMsg = m_bLangulageEng?_T("Close Vacum"):_T("关闭真空");

			if(::IsNoHardware() || (VacumOpen(-1, FALSE) && BreakVacum(TRUE)))
			{
				m_nNextAutoStep = AUTO_MOVE_Z_TO_SAFE_POS_AFTER_PLACE;
			}
			break;
		}
		//放完料移动Z轴到安全位
	case AUTO_MOVE_Z_TO_SAFE_POS_AFTER_PLACE:
		{
			m_strMsg = m_bLangulageEng?_T("Move Z To Safe Pos After Place"):_T("放完料移动Z轴到安全位");

			if (m_nMotorZ > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorZ, Z_MOTOR_SAFE_POS))
			{
				for (int i = 0; i <2; i++)
				{
					if(MACHINE.m_pBatteryInShell[i] != NULL && MACHINE.m_pBatteryInShell[i]->m_bCylClosed)
					{
						MACHINE.m_pBatteryInShell[i]->m_bCylClosed = FALSE;
					}
				}
				m_bShellVaild = FALSE;
				m_nNextAutoStep = AUTO_WORK_END;
			}
			break;
		}

		//工作完成
	case AUTO_WORK_END:
		{
			m_strMsg = m_bLangulageEng?_T("Work end"):_T("工作完成");
			m_nNextAutoStep = AUTO_MOVE_X_TO_ONLOAD_POS;
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

		if(m_nNextAutoStep == AUTO_MOVE_X_TO_ONLOAD_POS)
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

BOOL CRunProcessCarryShellPnP::InitializeConfig(CString strProcessModule)
{
	if (!CRunProcess::InitializeConfig(strProcessModule))
	{
		return FALSE;
	}
	return TRUE;
}

void CRunProcessCarryShellPnP::GetHardwareConfig(CArray<int, int> *pInputs, CArray<int, int> *pOutputs, CArray<int, int> *pMotors)
{
	CRunProcess::GetHardwareConfig(pInputs, pOutputs, pMotors);
}


//电机移动保护
BOOL CRunProcessCarryShellPnP::CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{
#ifdef _TEST_
	return TRUE;
#endif
	if(m_nMotorX > -1 && pMotor == MOTORS[m_nMotorX])
	{
		if(m_nMotorZ > -1)
		{
			if(!MOTORS[m_nMotorZ]->IsOriRefKnown())
			{
				BLMessageBox(_T("(M05)外壳移载Z轴电机复位, X轴电机不能移动"), MB_ICONWARNING);
				return FALSE;
			}
			else
			{
				if( MOTORS[m_nMotorZ]->GetCurPos() > 5)
				{
					BLMessageBox(_T("(M05)外壳移载Z轴电机不在安全位, X轴电机不能移动"), MB_ICONWARNING);
					return FALSE;
				}
			}
		}	
	}

	if ((m_nMotorZ > -1 && pMotor == MOTORS[m_nMotorZ]))
	{
		//取料等待位 取料位
		if (iLoc == Z_MOTOR_WAIT_PICK_POS || iLoc == Z_MOTOR_PICK_POS)
		{
			if (m_nMotorX > -1)
			{
				double pos = GetMotorPos(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorX, X_MOTOR_ONLOAD_LOC);
				double npos = MOTORS[m_nMotorX]->GetCurPos();
				if (fabs(pos - npos) > 0.5)
				{
					BLMessageBox(_T("(M04)外壳移载X轴电机不在取料位，Z轴不能移动到取料位或取料等待位"), MB_ICONWARNING);
					return FALSE;
				}
			}		

			ASSERT(m_IPushCylPush > -1 && m_IPushCylBack > -1);
			if (INPUTS[m_IPushCylPush]->IsOn() && INPUTS[m_IPushCylBack]->IsOff())
			{
				BLMessageBox(_T("外壳变距气缸推出，Z轴不能移动到取料位或取料等待位"), MB_ICONWARNING);
				return FALSE;
			}

			int RotateCylPush = MACHINE.m_pCleanAfter->m_IRotateCylPush;
			int RotateCylReturn = MACHINE.m_pCleanAfter->m_IRotateCylReturn;
            ASSERT(RotateCylPush > -1 && RotateCylReturn > -1);
            if (INPUTS[RotateCylReturn]->IsOn() && INPUTS[RotateCylPush]->IsOff())
            {
				BLMessageBox(_T("除尘后工位模组旋转气缸处于回退状态，Z轴不能移动到取料位或取料等待位"), MB_ICONWARNING);
				return FALSE;
            }

		}

		//放料位   放料等待位
		if (iLoc == Z_MOTOR_WAIT_PLACE_POS || iLoc == Z_MOTOR_PLACE_POS)
		{
			if (m_nMotorX > -1)
			{
				if (MOTORS[m_nMotorX]->GetCurLoc() != X_MOTOR_PNP_PUT_POS )					
				{
					BLMessageBox(_T("(M04)外壳移载X轴电机不在放料位，Z轴不能移动到放料位或放料等待位"), MB_ICONWARNING);
					return FALSE;
				}
			}

			ASSERT(m_IPushCylPush > -1 && m_IPushCylBack > -1);
			if (INPUTS[m_IPushCylPush]->IsOff() && INPUTS[m_IPushCylBack]->IsOn())
			{
				BLMessageBox(_T("外壳变距气缸回退，Z轴不能移动到放料位或放料等待位"), MB_ICONWARNING);
				return FALSE;
			}
		}
	}
	return TRUE;
}



BOOL CRunProcessCarryShellPnP::CheckOutputCanActive(COutput *pOutput)
{
#ifdef _TEST_
	return TRUE;
#endif
	ASSERT(m_OPushCylPush > -1 && m_OPushCylBack > -1 && m_nMotorZ > -1);
	if (pOutput == OUTPUTS[m_OPushCylPush] || pOutput == OUTPUTS[m_OPushCylBack])
	{
		double dPosZ0 = MOTORS[m_nMotorZ]->GetCurPos();
		double dPosZ1 = GetMotorPos(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorZ, Z_MOTOR_SAFE_POS);
		if (dPosZ0 > 5)
		{
			BLMessageBox(_T("(M05)外壳移载Z轴电机不在安全位, 外壳变距气缸不能动"));
			return FALSE;
		}
	}

	return TRUE;
}

/****************************************************************************************
函数描述：真空打开
输入参数：nIndex:		-1代表所有真空电磁阀，为0-1时分别代表真空1和真空2
bClose:		TRUE:真空打开		FALSE:真空关闭
函数返回：TRUE:执行成功，FALSE:执行失败
*****************************************************************************************/
BOOL CRunProcessCarryShellPnP::VacumOpen(int nIndex, BOOL bOpen)
{
	ASSERT(-1 <= nIndex && nIndex <= 1);
	ASSERT(m_ICheckVacum[0] > -1 && m_ICheckVacum[1] > -1 && m_OVacumCyl[0] > -1 && m_OVacumCyl[1] > -1);

	BOOL bRes = FALSE;

	if (nIndex < 0)
	{
		if(bOpen)
		{
			for(int i = 0; i < 2; i++)
			{
				OUTPUTS[m_OVacumCyl[i]]->On();
			}

			if(WAIT_OK == WaitInput(INPUTS[m_ICheckVacum[0]], ON, MSG_SHOW_DEFAULT) 
				&& WAIT_OK == WaitInput(INPUTS[m_ICheckVacum[1]], ON, MSG_SHOW_DEFAULT))
			{
				Sleep(300);
				bRes = TRUE;
			}
		}
		else
		{
			for(int i = 0; i < 2; i++)
			{
				OUTPUTS[m_OVacumCyl[i]]->Off();
			}

			if(WAIT_OK == WaitInput(INPUTS[m_ICheckVacum[0]], OFF, MSG_SHOW_DEFAULT) 
				&& WAIT_OK == WaitInput(INPUTS[m_ICheckVacum[1]], OFF, MSG_SHOW_DEFAULT))
			{
				Sleep(100);
				bRes = TRUE;
			}
		}
	}
	else	
	{
		if(bOpen)
		{			
			OUTPUTS[m_OVacumCyl[nIndex]]->On();			

			if(WAIT_OK == WaitInput(INPUTS[m_ICheckVacum[nIndex]], ON, MSG_SHOW_DEFAULT))
			{
				Sleep(300);
				bRes = TRUE;
			}
		}
		else
		{
			OUTPUTS[m_OVacumCyl[nIndex]]->Off();

			if(WAIT_OK == WaitInput(INPUTS[m_ICheckVacum[nIndex]], OFF, MSG_SHOW_DEFAULT))
			{
				Sleep(100);
				bRes = TRUE;
			}
		}
	}	
	return bRes;
}

/****************************************************************************************
函数描述：破真空打开
输入参数：bBreak:		TRUE:破真空打开		FALSE:破真空关闭
函数返回：TRUE:执行成功，FALSE:执行失败
*****************************************************************************************/
BOOL CRunProcessCarryShellPnP::BreakVacum( BOOL bBreak)
{
	if(m_OBreakVacum < 0 )
	{
		return TRUE;
	}
	if(bBreak)
	{			
		OUTPUTS[m_OBreakVacum]->On();
	}
	else
	{		
		OUTPUTS[m_OBreakVacum]->Off();
	}

	return TRUE;
}

/****************************************************************************************
函数描述：检测真空
输入参数：nIndex:		-1代表所有真空电磁阀，为0-1时分别代表真空1和真空2
bOpen:		TRUE:真空打开		FALSE:真空关闭
函数返回：TRUE:执行成功，FALSE:执行失败
*****************************************************************************************/
BOOL CRunProcessCarryShellPnP::CheckVacum(int nIndex,BOOL bOpen) 
{
	ASSERT(-1 <= nIndex && nIndex <= 1);
	ASSERT(m_ICheckVacum[0] > -1 && m_ICheckVacum[1] > -1);


	if (nIndex < 0)
	{
		if(bOpen)
		{
			if(CheckInput(INPUTS[m_ICheckVacum[0]], ON) 
				&& CheckInput(INPUTS[m_ICheckVacum[1]], ON))
			{
				return TRUE;
			}
		}
		else
		{
			if(CheckInput(INPUTS[m_ICheckVacum[0]], OFF) 
				&& CheckInput(INPUTS[m_ICheckVacum[1]], OFF))
			{
				return TRUE;
			}
		}
	}
	else	
	{
		if(bOpen)
		{			
			if(CheckInput(INPUTS[m_ICheckVacum[nIndex]], ON))
			{
				return TRUE;
			}
		}
		else
		{
			if(CheckInput(INPUTS[m_ICheckVacum[nIndex]], OFF))
			{
				return TRUE;
			}
		}
	}	
	return FALSE;
}


/****************************************************************************************
函数描述：推出气缸动作
输入参数：bPush:TRUE:推出气缸推出 FALSE:推出气缸回退
函数返回：TRUE:执行成功，FALSE:执行失败
*****************************************************************************************/
BOOL CRunProcessCarryShellPnP:: PushCylPush(BOOL bPush)
{
	ASSERT(m_IPushCylPush > -1 && m_IPushCylBack > -1 && m_OPushCylPush > -1 && m_OPushCylBack > -1);

	if (bPush)
	{
		OUTPUTS[m_OPushCylBack]->Off();
		OUTPUTS[m_OPushCylPush]->On();

		if(WAIT_OK == WaitInput(INPUTS[m_IPushCylPush], ON, MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_IPushCylBack], OFF, MSG_SHOW_DEFAULT))
		{
			return TRUE;
		}
	}
	else
	{
		OUTPUTS[m_OPushCylPush]->Off();
		OUTPUTS[m_OPushCylBack]->On();

		if(WAIT_OK == WaitInput(INPUTS[m_IPushCylPush], OFF, MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_IPushCylBack], ON, MSG_SHOW_DEFAULT))
		{
			return TRUE;
		}
	}
	return FALSE;
}


/****************************************************************************************
函数描述：获取Z轴安全位位置
输入参数：
函数返回： 返回Z轴安全位位置
*****************************************************************************************/
double CRunProcessCarryShellPnP::GetMotorZPOS()
{
	ASSERT(m_nMotorZ > -1);

	double dPos1 = GetMotorPos(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorZ, Z_MOTOR_SAFE_POS);

	return dPos1;
}
