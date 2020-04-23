/****************************************************************************************
文件名：RunProcessOnloadShellStackEx.cpp
功能描述：上外壳堆栈模组
历史修改记录：
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

	m_VoidMap.insert(pair<void*, CString>(&m_dlZSafeLimit, _T("dZSafeLimit, 安全限位设置")));
}

CRunProcessOnloadShellStack::~CRunProcessOnloadShellStack(void)
{
}

void CRunProcessOnloadShellStack::PowerUpRestart()
{
	CRunProcess::PowerUpRestart();
	m_strMsg = m_bLangulageEng? _T("Ready"):_T("准备好");

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
		m_strMsg = m_bLangulageEng?_T("Moudle UnEnable"):_T("模块禁用");
		return;
	}

	switch(m_nNextPowerUpStep)
	{
		//锁车气缸打开
	case PU_RELASE_CAR_LOCK_CYL_OPEN:
		{
			m_strMsg = m_bLangulageEng?_T("Relase Car Lock Cyl Lock"): _T("小车安全气缸打开");

			if (::IsNoHardware() || LockCar(FALSE))
			{
				m_nNextPowerUpStep = PU_CHECK_PALLET_DATA;
			}
			break;
		}
		//检查小车
	case PU_CHECK_PALLET_DATA:
		{
			m_strMsg = m_bLangulageEng?_T("Check Have Pallet"): _T("检查小车是否被移走");
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

		//请按确认按钮后安全气缸锁紧关闭
	case PU_SAFE_CYL_CLOSE:
		{
			m_strMsg = m_bLangulageEng?_T("Relase Car Lock Cyl Lock"): _T("请按确认按钮后安全气缸锁紧关闭");
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

		//Z轴电机复位
	case PU_Z_MOTOR_MOVE_HOME:
		{
			m_strMsg = m_bLangulageEng?_T("Z Motor Move Home"): _T("Z轴电机复位");

			if (m_nMotorZ > -1 && HomeMotor(MOTORS[m_nMotorZ]))
			{
				m_bHomeMoveSafePos = TRUE;
				m_nNextPowerUpStep = PU_CHECK_HAVE_PALLET;
			}
			break;
		}

	case PU_CHECK_HAVE_PALLET:
		{
			m_strMsg = m_bLangulageEng?_T("Check Have Pallet"): _T("检测是否有料盘");

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
		//Z轴电机移动到等待位
	case PU_Z_MOTOR_MOVE_WAIT_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Z Motor Move Wait Pos"): _T("Z轴电机移动到等待位");

			if (m_nMotorZ > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorZ, STACK_MOTOR_WAIT_LOC)) 
			{
				m_nNextPowerUpStep = PU_FINISH;
			}
			break;
		}
		//初始化完成
	case PU_FINISH:
		{
			CRun::PowerUpOperation();
			m_strMsg = m_bLangulageEng?_T("Power up finished"): _T("初始化完成");
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
		if (m_nSaveLastStep == AUTO_SAFE_CYL_OPEN)
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
		//安全气缸关闭
	case AUTO_SAFE_CYL_OPEN:
		{
			m_strMsg = m_bLangulageEng?_T("Lock Cyl Open"):_T("锁车气缸打开");

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

		//等待人工上料
	case AUTO_WAIT_PEOPLE_ONLOAD:
		{
			m_strMsg = m_bLangulageEng?_T("Wait People Onload"):_T("等待人工上料");

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

		//检查小车有无到位
	case AUTO_CHECK_ONLOAD:
		{
			m_strMsg = m_bLangulageEng?_T("Check Onload"):_T("检查小车有无到位");

			if (::IsNoHardware() || m_bDryRun || CheckCarAndPallet(TRUE))
			{
				m_nNextAutoStep = AUTO_LOCK_CYL_LOCK;
			}
			else
			{
				//铝壳堆栈#1/2检车小车与料盘状态异常, 为确保设备正常运行，请检查小车前/后到位传感器(ON)，料盘传感器(ON)
				ShowMessage(MSG_ONLOAD_STACK1_CHECK_CAR_AND_PALLET_ABERRANT + m_nGroupID);
				if(m_OReadyLight > -1 && OUTPUTS[m_OReadyLight]->IsOff())
				{
					OUTPUTS[m_OReadyLight]->On();
				}
				m_nNextAutoStep= AUTO_WAIT_PEOPLE_ONLOAD;
			}
			break;
		}

		//锁车气缸锁紧
	case AUTO_LOCK_CYL_LOCK:
		{
			m_strMsg = m_bLangulageEng?_T("Lock Cyl Lock"):_T("锁车气缸锁紧");

			if(::IsNoHardware() || LockCar(TRUE))
			{
				m_nNextAutoStep = AUTO_MOVE_Z_MOTOR_TO_RELASE_CAR_LOC;
			}
			break;
		}
		//移动电机到小车分离位
	case AUTO_MOVE_Z_MOTOR_TO_RELASE_CAR_LOC:
		{
			m_strMsg = m_bLangulageEng?_T("Move Z Motor To Relase Pos"):_T("移动电机到小车分离位");

			if (m_nMotorZ > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorZ, STACK_MOTOR_RELASE_CAR_LOC)) 
			{
				if(m_OLockCyl > -1)
				{// 到分离位后锁车气缸打开
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
			m_strMsg = m_bLangulageEng?_T("Move Z Motor To Relase Pos"):_T("检测料盘高度是否超限位");

			if (::IsNoHardware() || (m_ITrayTopPresent > -1 && INPUTS[m_ITrayTopPresent]->IsOff()))
			{
				m_bHavePallet = TRUE;
				m_nNextAutoStep = AUTO_LOCK_CYL_LOCK_ONE;
			}
			else
			{
				//铝壳上料堆栈料盘数量过多，已超过安全限位感应器，请慎重!
				ShowMessage(MSG_ONLOAD_STACK_PALLET_TOO_MANY);
			}
			break;
		}

		//锁车气缸松开
	case AUTO_LOCK_CYL_LOCK_ONE:
		{
			m_strMsg = m_bLangulageEng?_T("Lock Cyl Lock"):_T("锁车气缸松开");

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

		//到分离位后等待人工确认是否安全
	case AUTO_WAIT_PEOPLE_CHECK_SAFE:
		{
			m_strMsg = m_bLangulageEng?_T("Wait People Check Safe"):_T("到分离位后等待人工确认是否安全");

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
					//铝壳堆栈上料后检测到小车未完全移走，请确认!
					ShowMessage(MSG_ONLOAD_STACK_CHECK_CAR_NOT_REMOVE);
				}
			}
			break;
		}

		//等待开始信号
	case AUTO_WAIT_START_EVENT:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Start Event"):_T("等待搬运模组开始上料信号");
		
			if (!m_bMotorSafePos && (m_bRequirePallet || (MACHINE.m_pFeederTray != NULL && !MACHINE.m_pFeederTray->IsModuleEnable())))
			{
				if (!m_bDryRun && !CheckHavePallet(TRUE))     //等待搬运要料再次检测有无料
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

		//移动Z轴电机到送料位
	case AUTO_MOVE_Z_MOTOR_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Start Event"):_T("Z轴移动到限位感应器停止");

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
			m_strMsg = m_bLangulageEng?_T("Motor Z Absnormal Down To Distance"):_T("Z轴异常报警，下降一段距离");
			if(m_nMotorZ > -1)
			{
				if (fabs(MOTORS[m_nMotorZ]->GetCurPos()-20) < GetMotorPos(GetCurPorductID(MACHINE.m_nDefaultProduct),m_nMotorZ,STACK_MOTOR_RELASE_CAR_LOC)) //电机可移动最低位，防止撞负限位
				{
					m_nNextAutoStep = AUTO_MOVE_Z_MOTOR_POS;
					break;
				}

				double dDownPos = MOTORS[m_nMotorZ]->GetCurPos() - 50;
				if(MoveMotorPos(MOTORS[m_nMotorZ], dDownPos))
				{
					ShowMessage(_T("Z轴异常请检查，或者料盘有料感应没有料"),3);
					m_nNextAutoStep = AUTO_MOVE_Z_MOTOR_POS;
				}
			}
			break;
		}
	case AUTO_MOTOR_Z_DOWN_TO_SAFE_DISTANCE:
		{
			m_strMsg = m_bLangulageEng?_T("Motor Z Down To Distance"):_T("Z轴下降一段距离");
			if(m_nMotorZ > -1)
			{
				if (fabs(MOTORS[m_nMotorZ]->GetCurPos()-20) < GetMotorPos(GetCurPorductID(MACHINE.m_nDefaultProduct),m_nMotorZ,STACK_MOTOR_RELASE_CAR_LOC)) //电机可移动最低位，防止撞负限位
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
			m_strMsg = m_bLangulageEng?_T("Motor Z Move To Onload Pos"):_T("Z轴上升到取料位");
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
		//Z轴可能需要移动一段距离，并发送取料信号
	case AUTO_CAN_MOVE_SHORT_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Start Event"):_T("Z轴可能需要移动一段距离，并发送取料信号");
			if(!m_bSendPalletFinish)
			{
				if(m_nMotorZ > -1)
				{
					m_dMotorPos = MOTORS[m_nMotorZ]->GetCurPos() - m_dlZSafeLimit;//下降距离由参数设置
				}
				m_bSendPalletFinish = TRUE;//发送取料信号
				m_nNextAutoStep = AUTO_WAIT_PICK_PALLET_FINISH;
			}
			break;
		}

		//等待取料盘完成
	case AUTO_WAIT_PICK_PALLET_FINISH:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Pick Pallet Finish"):_T("等待取料盘完成");

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

		//移动电机到偏移位
	case AUTO_MOVE_Z_MOTOR_TO_SAFE_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Move Z Motor To Safe Pos"):_T("移动电机到偏移位");

			double ZPos = GetMotorPos(GetCurPorductID(MACHINE.m_nDefaultProduct),m_nMotorZ,STACK_MOTOR_RELASE_CAR_LOC);
			if (fabs(MOTORS[m_nMotorZ]->GetCurPos()-20) < ZPos)  ////电机可移动最低位，防止撞负限位
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

		//判断堆栈还有无料盘
	case AUTO_JUDGE_STACK_EMPTY:
		{
			m_strMsg = m_bLangulageEng?_T("Judge Stack Empty"):_T("判断堆栈还有无料盘");

			if(m_bMotorSafePos)
				break;

			//以感应器来判断料是否取完
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

		//料盘取完后检测电机移动到等待位是否安全
	case AUT0_CHECK_MOVE_SAFE:
		{
			m_strMsg = m_bLangulageEng?_T("Check Move Safe"):_T("料盘取完后检测电机移动到等待位是否安全");

			if(CheckHaveNoCar())
			{
				m_nNextAutoStep = AUTO_MOVE_Z_MOTOR_WAIT_POS;
			}
			else
			{
				//堆栈料盘取完后，堆栈电机没有移动到等待位前不能将小车提前推进
				ShowMessage(MSG_ONLOAD_STACK_CHECK_CAR_STATUS_ILLEGAL);   
			}
			break;
		}

		//移动电机到等待位
	case AUTO_MOVE_Z_MOTOR_WAIT_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Move Z Motor Wait Pos"):_T("移动电机到等待位");

			if (m_nMotorZ > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorZ, STACK_MOTOR_WAIT_LOC)) 
			{
				m_nNextAutoStep = AUTO_WORK_END;
			}
			break;
		}

		//工作完成
	case AUTO_WORK_END:
		{
			m_strMsg = m_bLangulageEng? _T("Work end"):_T("工作完成");
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
		if(BLMessageBox(_T("Z轴电机移动之前请检查托盘安全，若安全点击'是’Z轴电机移动"), MB_YESNO)==IDYES)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
		ASSERT(m_IHavePallet > -1);
		if (INPUTS[m_IHavePallet]->IsOn())  //堆栈检测有料只能向上走，向下报警
		{
			double dPosZ = MOTORS[m_nMotorZ]->GetCurPos();
			if (bAbsMove == 1 && dPosZ > lfValue || bAbsMove == 0 && 0 > lfValue)
			{
				BLMessageBox(_T("外壳上料堆栈检测有料，(M8/M9)外壳上料Z轴伺服电机不能向下移动"), MB_ICONWARNING);
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
函数描述：锁车气缸动作
输入参数：bLock:TRUE:锁车气缸锁紧 FALSE:锁车气缸打开
函数返回：TRUE:执行成功，FALSE:执行失败
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
函数描述：检查小车到位
输入参数：bHave:TRUE:检查小车到位 FALSE:判断没有小车
函数返回：TRUE:执行成功，FALSE:执行失败
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
函数描述：检查是否有车
输入参数：
函数返回：TRUE:有车，FALSE:无车
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
函数描述：检查小车和料盘到位
输入参数：bHas： TRUE:检查有车和料盘  FALSE:检查没有车和料盘
函数返回：TRUE:与bHas一致，FALSE:与bHas相反
*****************************************************************************************/
BOOL CRunProcessOnloadShellStack::CheckCarAndPallet( BOOL bHas /*= TRUE*/ )
{
	BOOL bHasPallet = TRUE;//有盘标志

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
				bHasPallet = FALSE;//如果没有盘，返回FALSE
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
				bHasPallet = FALSE;//如果有盘，返回FALSE
				break;
			}
			Sleep(100);
		}
	}
	return bHasPallet;
}

/****************************************************************************************
函数描述：电机运动处理函数
输入参数：
函数返回：TRUE:运动完成，FALSE:运动失败
*****************************************************************************************/
BOOL CRunProcessOnloadShellStack::MotorHandMove( CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove )
{
	if (m_nMotorZ > -1 && pMotor == MOTORS[m_nMotorZ])
	{
		if (!MOTORS[m_nMotorZ]->IsOriRefKnown() && bAbsMove)
		{
			BLMessageBox(_T("电机没有复位，请复位完成后再操作!"), MB_OK);
			return FALSE;
		}
		if (M_BUSY == MOTORS[m_nMotorZ]->GetMotionStatus())
		{
			BLMessageBox(_T("电机正在移动，请移动完成后再操作!"), MB_OK);
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
				BLMessageBox(_T("托盘不在等待位，检测到有小车，Z轴不能往下移动，请将小车取走移到安全位后再往下移动"), MB_OK);
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
				BLMessageBox(_T("请检查限位感应 与料盘搬运-有盘感应、到位感应状态"));
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
				BLMessageBox(_T("Z轴20秒内没移动到目标位置，请继续移动！"), MB_OK);				
				break;
			}
			Sleep(1);
		}
		return FALSE;//防止二次移动，必须为FALSE
	}
	return TRUE;//其它电机，为TRUE，由系统处理
}

BOOL CRunProcessOnloadShellStack::  ReadRunParameter()
{
	CRunProcess:: ReadRunParameter();
	m_dlZSafeLimit =::IniReadDouble(m_strProcess, _T("ZSafeLimit"), 120.00, GetAbsolutePathName(PARAMETER_CFG));
	return TRUE;
}

BOOL CRunProcessOnloadShellStack::SaveParameter()
{
	//绝对路径和相对路径

	if (!IniWriteDouble(m_strProcess, _T("ZSafeLimit"), m_dlZSafeLimit, GetAbsolutePathName(PARAMETER_CFG)))
	{
		return FALSE;
	}

	CRunProcess::SaveParameter();
	return TRUE;
}
/****************************************************************************************
函数描述：检查料盘
输入参数：bHas： TRUE:检查有料盘  FALSE:检查没料盘
函数返回：TRUE:与bHas一致，FALSE:与bHas相反
*****************************************************************************************/
BOOL CRunProcessOnloadShellStack::CheckHavePallet(BOOL bHas /*= TRUE*/)
{
	BOOL bHasPallet = TRUE;//有盘标志
	if (bHas)
	{
		for(int i = 0; i < 3; i++)
		{
			if (m_IHavePallet > -1 && INPUTS[m_IHavePallet]->IsOff())
			{
				bHasPallet = FALSE;//如果没有盘，返回FALSE
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
				bHasPallet = FALSE;//如果有盘，返回FALSE
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