/****************************************************************************************
文件名：RunProcessOffloadShellStackEx.cpp
功能描述：下外壳堆栈模组
历史修改记录：
*****************************************************************************************/
#include "StdAfx.h"
#include "RunProcessOffloadShellStack.h"

CRunProcessOffloadShellStack::CRunProcessOffloadShellStack(int nID) :
CRunProcess(nID)
{
	PowerUpRestart();
	m_InputMap.insert(pair<int*, CString>(&m_IButtonReady, _T("IButtonReady")));
	m_InputMap.insert(pair<int*, CString>(&m_ICarInPosFront, _T("ICarInPosFront")));
	m_InputMap.insert(pair<int*, CString>(&m_ICarInPosBehind, _T("ICarInPosBehind")));
	m_InputMap.insert(pair<int*, CString>(&m_ILockCylPushOut, _T("ILockCylPushOut")));
	m_InputMap.insert(pair<int*, CString>(&m_ILockCylBack, _T("ILockCylBack")));

	m_InputMap.insert(pair<int*, CString>(&m_IEmptyCarCheck, _T("IEmptyCarCheck")));
	m_InputMap.insert(pair<int*, CString>(&m_IHavePallet, _T("IHavePallet")));
	m_InputMap.insert(pair<int*, CString>(&m_ITrayTopPresent, _T("ITrayTopPresent")));

	m_OutputMap.insert(pair<int*, CString>(&m_OLockCyl, _T("OLockCyl")));
	m_OutputMap.insert(pair<int*, CString>(&m_OReadyLight, _T("OReadyLight")));
	m_MotorMap.insert(pair<int*, CString>(&m_nMotorZ, _T("MotorZ")));

	m_VoidMap.insert(pair<void*, CString>(&m_dlZSafeLimit, _T("dZSafeLimit, 安全限位设置")));
}

CRunProcessOffloadShellStack::~CRunProcessOffloadShellStack(void)
{
}

void CRunProcessOffloadShellStack::PowerUpRestart()
{
	CRunProcess::PowerUpRestart();
	m_strMsg = m_bLangulageEng? _T("Ready"):_T("准备好");

	m_bOffloadEmptyPallet = FALSE;
	m_bMotorSafePos = FALSE;
	m_bMoveMotorFinish = FALSE;
	m_bHomeMoveSafePos = FALSE;
	m_dMotorDownPos = 0;
	m_dMotorUpPos = 0;
	m_dPalletHigh = 120;
	m_nNextPowerUpStep = PU_LOCK_CAR_OPEN;
	m_nNextAutoStep = AUTO_CHECK_CAR_AND_PALLET;
}

void CRunProcessOffloadShellStack::PowerUpOperation(void)
{
	if (!IsModuleEnable())
	{
		CRun::PowerUpOperation();
		m_strMsg = m_bLangulageEng?_T("Moudle UnEnable"):_T("模组禁用");
		return;
	}

	switch(m_nNextPowerUpStep)
	{
		//锁车气缸打开
	case PU_LOCK_CAR_OPEN:
		{
			m_strMsg = m_bLangulageEng?_T("Release Car Lock Cyl Open"): _T("锁车气缸打开");

			if (::IsNoHardware() || LockCar(FALSE))
			{
				m_nNextPowerUpStep = PU_CHECK_HAVE_CAR;
			}
			break;
		}

		//检查有无小车
	case PU_CHECK_HAVE_CAR:
		{
			m_strMsg = m_bLangulageEng?_T("Check Have Car"): _T("检查有无小车");

			if (::IsNoHardware() || CheckCarInPos(FALSE))
			{
				if(m_OReadyLight > -1)
				{
					OUTPUTS[m_OReadyLight]->On();
				}
				m_nNextPowerUpStep =PU_WAIT_PEOPLE_CHECK_SAFE;
			}
			else
			{
				//铝壳下料堆栈检车到小车，为保证安全请将小车推走
				ShowMessage(MSG_OFFLOAD_STACK_CHECK_CAR_VAILD);
			}
			break;
		}

		//等待人工确认是否安全
	case PU_WAIT_PEOPLE_CHECK_SAFE:
		{
			m_strMsg = m_bLangulageEng?_T("Wait People Check Safe"):_T("等待人工确认是否安全");

			if (::IsNoHardware() || (m_IButtonReady > -1 && INPUTS[m_IButtonReady]->IsOn()))
			{
				if(m_OReadyLight > -1)
				{
					OUTPUTS[m_OReadyLight]->Off();
				}
				m_nNextPowerUpStep = PU_SAFE_CLY_CLOSE;
			}
			break;
		}
		//安全气缸打开
	case PU_SAFE_CLY_CLOSE:
		{
			m_strMsg = m_bLangulageEng?_T("Relase Car Lock Cyl Lock"): _T("锁车气缸关闭");

			if (::IsNoHardware() || LockCar(TRUE))
			{
				m_nNextPowerUpStep = PU_Z_MOTOR_MOVE_HOME;
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
				m_nNextPowerUpStep = PU_CHECK_HAVE_PALLAT;
			}
			break;
		}

	case  PU_CHECK_HAVE_PALLAT:
		{
			m_strMsg = m_bLangulageEng?_T("Z Motor Move Home"): _T("检查有无料盘");

			if (m_IHavePallet > -1 && CheckInput(INPUTS[m_IHavePallet],OFF))
			{
				m_nNextPowerUpStep = PU_MOVE_Z_MTOR_TO_SAFE_POS;
			}
			break;
		}

		//移动电机检查料盘料满位
	case PU_MOVE_Z_MTOR_TO_SAFE_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Move Z Motor To Offload Pos1"): _T("移动电机到安全位");

			if (m_nMotorZ > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorZ, STACK_MOTOR_SAFE_POS))
			{
				m_nNextPowerUpStep = PU_FINISH;
			}
			break;
		}
	//case PU_MOTOR_DOWN_SAFE_DISTANCE:
	//	{
	//		m_strMsg = m_bLangulageEng?_T("Move Z Motor Down"): _T("电机下降一段距离");
	//		// 由于电机报警、异常等原因导致MotorMoveCheckFullPallet执行失败，为保证安全，先将电机下降一段距离后重新执行该函数
	//		double dMotorPos = MOTORS[m_nMotorZ]->GetCurPos() - 100;
	//		if(m_nMotorZ > -1 && MoveMotorPos(MOTORS[m_nMotorZ], dMotorPos))
	//		{
	//			m_nNextPowerUpStep = PU_MOVE_Z_MTOR_TO_SAFE_POS;
	//		}
	//		break;
	//	}
	//case PU_CHECK_PALLET_FULL:
	//	{
	//		m_strMsg = m_bLangulageEng?_T("Check Pallet Full"): _T("检查料盘是否已满");

	//		double dSafeLoc = GetMotorPos(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorZ, STACK_MOTOR_SAFE_POS);
	//		if(MOTORS[m_nMotorZ]->GetCurPos() - dSafeLoc < m_dPalletHigh)	// 当前位置减去安全位，差值小于一个料盘高度，代表料满
	//		{
	//			m_nNextAutoStep = AUTO_CHECK_CAR_AND_PALLET_AGAIN;
	//			m_nNextPowerUpStep = PU_FINISH;
	//		}
	//		else
	//		{
	//			m_nNextAutoStep = AUTO_CHECK_CAR_AND_PALLET;
	//			m_nNextPowerUpStep = PU_FINISH;
	//		}
	//	}
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

void CRunProcessOffloadShellStack::AutoOperation()
{
	if (!IsModuleEnable())
	{
		m_strMsg = m_bLangulageEng?_T("Moudle not enable"):_T("模块禁用");
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
		if (m_nSaveLastStep == AUTO_CHECK_CAR_AND_PALLET)
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
		//检测车和料盘
	case AUTO_CHECK_CAR_AND_PALLET:
		{
			m_strMsg = m_bLangulageEng?_T("Check Car And Pallet"): _T("检测车和料盘");

			if(::IsNoHardware() || CheckCarAndPallet(FALSE))
			{
				m_nNextAutoStep = AUTO_WAIT_START_EVENT;
			}
			else
			{
				//铝壳下料堆栈检车到小车，为保证安全请将小车移开
				ShowMessage(MSG_OFFLOAD_STACK_CHECK_CAR_VAILD);
			}
			break;
		}

			//等待开始信号
	case AUTO_WAIT_START_EVENT:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Start Event"): _T("等待开始信号");

			if (m_bOffloadEmptyPallet || (MACHINE.m_pFeederTray != NULL && !MACHINE.m_pFeederTray->IsModuleEnable()))
			{
				m_nNextAutoStep = AUTO_MOVE_Z_MOTOR_TO_SAFE_POS;
			}
			break;
		}

		//移动电机检查料盘料满位
	case AUTO_MOVE_Z_MOTOR_TO_SAFE_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Move Z Motor To Safe Pos"): _T("移动电机检查料盘料满位");
			// 若是电机报警，会导致判断失误
			if (MotorMoveCheckFullPallet())
			{
				m_nNextAutoStep = AUTO_CHECK_PALLET_IS_FULL;
			}
			else
			{
				m_nNextAutoStep=AUTO_MOTOR_Z_DOWN_TO_SAFE_DISTANCE;
			}
			break;
		}
	case AUTO_MOTOR_Z_DOWN_TO_SAFE_DISTANCE:
		{
			m_strMsg = m_bLangulageEng?_T("Move Z Motor Down"): _T("电机下降一段距离");
			// 由于电机报警、异常等原因导致MotorMoveCheckFullPallet执行失败，为保证安全，先将电机下降一段距离后重新执行该函数
			double dMotorPos = MOTORS[m_nMotorZ]->GetCurPos() - 100;
			if(dMotorPos >= 0 && m_nMotorZ > -1 && MoveMotorPos(MOTORS[m_nMotorZ], dMotorPos))
			{
				m_nNextAutoStep = AUTO_MOVE_Z_MOTOR_TO_SAFE_POS;
			}
			break;
		}
	case AUTO_CHECK_PALLET_IS_FULL:
		{
			m_strMsg = m_bLangulageEng?_T("Check Pallet Full"): _T("检查料盘是否已满");
			double dSafeLoc = GetMotorPos(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorZ, STACK_MOTOR_SAFE_POS);
			if(MOTORS[m_nMotorZ]->GetCurPos() - dSafeLoc < m_dPalletHigh)	// 当前位置减去安全位，差值小于一个料盘高度，代表料满
			{
				m_nNextAutoStep = AUTO_CHECK_CAR_AND_PALLET_AGAIN;
			}
			else
			{
				m_nNextAutoStep = AUTO_JUDGE_CAR_MOVE;
			}
			break;
		}
	

		//移动前判断小车有无推开
	case AUTO_JUDGE_CAR_MOVE:
		{
			m_strMsg = m_bLangulageEng?_T("Judge Car Move"): _T("判断小车是否已移走");

			if(::IsNoHardware() || CheckCarInPos(FALSE))
			{
				m_dMotorUpPos = MOTORS[m_nMotorZ]->GetCurPos() + m_dlZSafeLimit;
				m_nNextAutoStep = AUTO_MOVE_Z_MOTOR_TO_WORK_POS;
			}
			else
			{
				// 铝壳下料堆栈检车到小车，为保证安全请将小车移开
				ShowMessage(MSG_OFFLOAD_STACK_CHECK_CAR_VAILD);
			}
			break;
		}

		//移动电机到接料位
	case AUTO_MOVE_Z_MOTOR_TO_WORK_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Move Z Motor To Work Pos"):_T("移动电机到接料位");

			if (!m_bMoveMotorFinish && m_nMotorZ > -1 && MoveMotorPos(MOTORS[m_nMotorZ], m_dMotorUpPos))
			{
				m_bOffloadEmptyPallet = FALSE;
				m_dMotorDownPos = MOTORS[m_nMotorZ]->GetCurPos() - m_dlZSafeLimit - m_dPalletHigh;
				m_nNextAutoStep = AUTO_WAIT_PUT_PALLET_FINISH;
			}
			break;
		}

		//等待放料完成
	case AUTO_WAIT_PUT_PALLET_FINISH:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Put Pallet Finish"): _T("等待放料完成");
	
			if (MACHINE.m_pFeederTray != NULL && ( MACHINE.m_pFeederTray->m_bPutPalletFinish || !MACHINE.m_pFeederTray->IsModuleEnable()))
			{
				m_nNextAutoStep = AUTO_CHECK_CAR_REMOVE;
			}
			break;
		}

		// 检查小车是否离开
	case AUTO_CHECK_CAR_REMOVE:
		{
			m_strMsg = m_bLangulageEng?_T("Check Car Remove"): _T("检查小车移开");

			if(::IsNoHardware() || CheckCarInPos(FALSE))
			{
				m_nNextAutoStep = AUTO_MOVE_Z_MOTOR_TO_SAFE_POS_AFTER_PICK_PALLET;
			}
			else
			{
				// 铝壳下料堆栈检车到小车，为保证安全请将小车移开
				ShowMessage(MSG_OFFLOAD_STACK_CHECK_CAR_VAILD);
			}
			break;
		}

		//移动Z轴到偏移位
	case AUTO_MOVE_Z_MOTOR_TO_SAFE_POS_AFTER_PICK_PALLET:
		{
			m_strMsg = m_bLangulageEng?_T("Move Z Motor To Safe Pos"): _T("移动Z轴到偏移位");

			if (m_dMotorDownPos >= 0 && m_nMotorZ > -1 && MoveMotorPos(MOTORS[m_nMotorZ], m_dMotorDownPos))
			{
				if (MACHINE.m_pFeederTray != NULL)
				{
					MACHINE.m_pFeederTray->m_bPutPalletFinish = FALSE;
				}
				m_nNextAutoStep = AUTO_JUDGE_PALLET_FULL;
			}
			break;
		}

		//判断料盘有无接满
	case AUTO_JUDGE_PALLET_FULL:
		{
			m_strMsg = m_bLangulageEng?_T("Judge Pallet Full"): _T("判断料盘有无接满");

			double dSafeLoc = GetMotorPos(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorZ, STACK_MOTOR_SAFE_POS);
			if(MOTORS[m_nMotorZ]->GetCurPos() - dSafeLoc < m_dPalletHigh)	// 当前位置减去安全位，差值小于一个料盘高度，代表料满
			{
				m_nNextAutoStep = AUTO_CHECK_CAR_AND_PALLET_AGAIN;
			}
			else
			{
				m_nNextAutoStep = AUTO_WAIT_START_EVENT;
			}
			break;
		}

		//再次检测有无料盘
	case AUTO_CHECK_CAR_AND_PALLET_AGAIN:
		{
			m_strMsg = m_bLangulageEng?_T("Check Car And Pallet Again"): _T("再次检测小车");

			if(::IsNoHardware() || CheckCarInPos(FALSE))
			{
				m_nNextAutoStep = AUTO_MOTOR_OFFLOAD_MOVE_TO_SAFE_POS;
			}
			break;
		}

		//电机移动到安全位
	case AUTO_MOTOR_OFFLOAD_MOVE_TO_SAFE_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Motor Move"): _T("电机移动到安全位");

			if(m_nMotorZ > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorZ, STACK_MOTOR_SAFE_POS))
			{
				MACHINE.m_bSafeStatckDoor[2] = TRUE;
				m_nNextAutoStep = AUTO_SET_OFFLOAD_PALLET_SINGLE;
			}
			break;
		}
	case AUTO_SET_OFFLOAD_PALLET_SINGLE:
		{
			m_strMsg = m_bLangulageEng?_T("Set Offload Pallet Single"): _T("设置下空料盘信号");
			// 添加下料堆栈蜂鸣器
			// MACHINE.SetBuzzerStatue(TRUE);  mh
			if(m_OReadyLight > -1)
			{
				OUTPUTS[m_OReadyLight]->On();
			}
			m_nNextAutoStep = AUTO_WAIT_LOCK_CYL_LOCK;
			break;
		}
		//小车进去后锁车气缸锁紧
	case AUTO_WAIT_LOCK_CYL_LOCK:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Lock Cyl Lock"): _T("小车进去后锁车气缸打开");

			if (::IsNoHardware() || LockCar(FALSE))
			{
				m_nNextAutoStep = AUTO_WAIT_PEOPLE_MOVE_CAR_IN;
			}

			if(!IsNoHardware() && !m_bDryRun)
			{
				// 铝壳下料堆栈检测空料盘已满，请将小车推进堆栈接空料盘
				ShowMessage(MSG_OFFLOAD_STACK_PROMPT_OFFLOAD_EMPTY_PALLET);
			}
			break;
		}

		//等待人把小车推进去
	case AUTO_WAIT_PEOPLE_MOVE_CAR_IN:
		{
			m_strMsg = m_bLangulageEng?_T("Wait People Move Car In"): _T("等待小车推进下料堆栈");

			if (::IsNoHardware() || m_bDryRun || m_IButtonReady > -1 && INPUTS[m_IButtonReady]->IsOn())  //空跑不按按钮
			{
				 // 添加下料堆栈蜂鸣器
				//MACHINE.SetBuzzerStatue(FALSE);  mh
				if(m_OReadyLight > -1)
				{
					OUTPUTS[m_OReadyLight]->Off();
				}
				m_nNextAutoStep = AUTO_CHECK_HAVE_CAR;
			}
			break;
		}

		//检查小车有无到位
	case AUTO_CHECK_HAVE_CAR:
		{
			m_strMsg = m_bLangulageEng?_T("Check Have Car"): _T("检查小车是否已经到位");

			if (::IsNoHardware() || m_bDryRun || CheckCarInPos(TRUE))
			{
				m_nNextAutoStep = AUTO_LOC_CYL_LOCATION_LOCK;
			}
			else
			{
				ShowMessage(MSG_OFFLOAD_STACK_CHECK_CAR_NOT_INPOS);
				m_nNextAutoStep = AUTO_SET_OFFLOAD_PALLET_SINGLE;
			}
			break;
		}

		//小车锁车气缸锁紧
	case AUTO_LOC_CYL_LOCATION_LOCK:
		{
			m_strMsg = m_bLangulageEng?_T("Loc Cyl Location Lock"): _T("小车锁车气缸锁紧");

			if(IsNoHardware() || LockCar(TRUE))
			{
				m_nNextAutoStep = AUTO_MOVE_Z_MOTOR_TO_RELASE_POS;
			}
			break;
		}

		//移动电机到分盘位
	case AUTO_MOVE_Z_MOTOR_TO_RELASE_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Move Z Motor To Relase Pos"): _T("移动电机到分盘位");

			if (m_nMotorZ > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorZ, STACK_MOTOR_RELASE_CAR_LOC))
			{
				m_nNextAutoStep = AUTO_LOCK_CYL_OPEN_ON_REMOVE_CAR;
			}
			break;
		}

		//锁车气缸打开
	case AUTO_LOCK_CYL_OPEN_ON_REMOVE_CAR:
		{
			m_strMsg = m_bLangulageEng?_T("Lock Cyl Open On "): _T("锁车气缸打开");

			if(IsNoHardware() || LockCar(FALSE))
			{
				if(m_OReadyLight > -1)
				{
					OUTPUTS[m_OReadyLight]->On();
				}
				m_nNextAutoStep = AUTO_REMOVE_CAR_AFTER_OFFLOAD;
			}
			break;
		}

		//等待人工把车移走
	case AUTO_REMOVE_CAR_AFTER_OFFLOAD:
		{
			m_strMsg = m_bLangulageEng?_T("Lock Cyl Open On Remove Car"): _T("等待人工把车移走");

			if(::IsNoHardware() || m_bDryRun ||(m_IButtonReady > -1 && INPUTS[m_IButtonReady]->IsOn())) //空跑不按按钮
			{
				if(m_OReadyLight > -1)
				{
					OUTPUTS[m_OReadyLight]->Off();
				}
				m_nNextAutoStep = AUTO_CHECK_HAVE_NO_PALLET;
			}
			break;
		}

		//检查有无料盘
	case AUTO_CHECK_HAVE_NO_PALLET:
		{
			m_strMsg = m_bLangulageEng?_T("Check Have No Pallet"): _T("检查有无料盘");

			if(::IsNoHardware() || CheckHaveNoPallet())
			{
				MACHINE.m_bSafeStatckDoor[2] = FALSE;
				m_nNextAutoStep = AUTO_SAFE_LOCK_CYL_CLOSE;
			}
			else
			{
				ShowMessage(_T("下料堆栈还有料盘，请检查小车是否拉走"),3);
			}
			break;
		}

		//安全气缸关闭
	case AUTO_SAFE_LOCK_CYL_CLOSE:
		{
			m_strMsg = m_bLangulageEng?_T("Safe Lock Cyl Close"): _T(" 锁车气缸关闭");

			if(::IsNoHardware() || LockCar(TRUE))
			{
				m_nNextAutoStep = AUTO_WORK_END;
			}
			break;
		}

		//工作完成
	case AUTO_WORK_END:
		{
			m_strMsg = m_bLangulageEng? _T("Work end"):_T("工作完成");
			m_nNextAutoStep = AUTO_CHECK_CAR_AND_PALLET;
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

		if(m_nNextAutoStep == AUTO_CHECK_CAR_AND_PALLET)
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

BOOL CRunProcessOffloadShellStack::InitializeConfig(CString strProcessModule)
{
	if (!CRunProcess::InitializeConfig(strProcessModule))
	{
		return FALSE;
	}
	return TRUE;
}

void CRunProcessOffloadShellStack::GetHardwareConfig(CArray<int, int> *pInputs, CArray<int, int> *pOutputs, CArray<int, int> *pMotors)
{
	CRunProcess::GetHardwareConfig(pInputs, pOutputs, pMotors);
}

/****************************************************************************************
函数描述：锁车气缸动作
输入参数：bLock:TRUE:锁车气缸锁紧 FALSE:锁车气缸打开
函数返回：TRUE:执行成功，FALSE:执行失败
*****************************************************************************************/
BOOL CRunProcessOffloadShellStack::LockCar(BOOL bLock)
{
	if (m_ILockCylPushOut < 0 || m_ILockCylBack  < 0 || m_OLockCyl < 0)
	{
		return FALSE;
	}

	if (bLock)
	{
		OUTPUTS[m_OLockCyl]->On();

		if (WAIT_OK == WaitInput(INPUTS[m_ILockCylPushOut], ON, MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_ILockCylBack], OFF, MSG_SHOW_DEFAULT))
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
BOOL CRunProcessOffloadShellStack::CheckCarInPos(BOOL bHave)
{
	BOOL bCheckRes = FALSE;
	ASSERT(m_ICarInPosBehind > -1 && m_ICarInPosFront > -1);

	if(bHave)
	{
		if (INPUTS[m_ICarInPosBehind]->IsOn() && INPUTS[m_ICarInPosFront]->IsOn())
		{
			Sleep(300);
			if (INPUTS[m_ICarInPosBehind]->IsOn() && INPUTS[m_ICarInPosFront]->IsOn())
			{
				bCheckRes = TRUE;
			}
		}
	}
	else
	{
		if (INPUTS[m_ICarInPosBehind]->IsOff() && INPUTS[m_ICarInPosFront]->IsOff())
		{
			Sleep(300);
			if (INPUTS[m_ICarInPosBehind]->IsOff() && INPUTS[m_ICarInPosFront]->IsOff())
			{
				bCheckRes = TRUE;
			}
		}
	}
	return bCheckRes;
}


BOOL CRunProcessOffloadShellStack::CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{
#ifdef _TEST_
	return TRUE;
#endif
	if (m_nMotorZ > -1 && pMotor == MOTORS[m_nMotorZ])
	{
		if(BLMessageBox(_T("Z轴电机移动之前请检查托盘安全，若安全点击'是’Z轴电机移动"), MB_YESNO)==IDYES)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	
	ASSERT(m_nMotorZ > -1);
	
	if (pMotor == MOTORS[m_nMotorZ])
	{
		double dPosZ0 = MOTORS[m_nMotorZ]->GetCurPos();
		if (bAbsMove == 1 && 500 < lfValue || bAbsMove == 0 && 500 < dPosZ0 + lfValue )
		{
			BLMessageBox(_T("(M10)外壳下料Z轴伺服电机移动超出安全限位"), MB_ICONWARNING);
			return FALSE;
		}

		if(MACHINE.m_pFeederTray != NULL)
		{
			int ITrayInPos[3];
			for(int i = 0; i < 2; i++)
			{
				ITrayInPos[i] = MACHINE.m_pFeederTray->m_ITrayInPosPresent[i];
				ASSERT(ITrayInPos[i] > -1);
			}
			if (INPUTS[ITrayInPos[0]]->IsOff() && INPUTS[ITrayInPos[1]]->IsOff() && INPUTS[ITrayInPos[2]]->IsOff())
			{
				BLMessageBox(_T("X0408/X0409/X0410料盘移动到位感应没有亮，请将（M11）外壳料盘Y轴伺服电机移动到位"), MB_ICONWARNING);
				return FALSE;
			}
		}

		ASSERT(m_IHavePallet > -1);
		if (INPUTS[m_IHavePallet]->IsOn())  //堆栈检测有料只能向上走，向下报警
		{
			double dPosZ = MOTORS[m_nMotorZ]->GetCurPos();
			if (bAbsMove == 1 && dPosZ > lfValue || bAbsMove == 0 && 0 > lfValue)
			{
				BLMessageBox(_T("外壳下料堆栈检测有料，(M10)外壳下料Z轴伺服电机不能向下移动"), MB_ICONWARNING);
				return FALSE;
			}
		}

		ASSERT(m_ICarInPosFront > -1 && m_ICarInPosBehind > -1);
		ASSERT(m_ILockCylPushOut > -1 && m_ILockCylBack > -1);
		if (INPUTS[m_ICarInPosFront]->IsOn())
		{
			if (INPUTS[m_ICarInPosBehind]->IsOn() || INPUTS[m_ILockCylPushOut]->IsOff() || INPUTS[m_ILockCylBack]->IsOn())
			{
				BLMessageBox(_T("小车没有推到位，外壳下料Z轴伺服电机不能移动"), MB_ICONWARNING);
				return FALSE;
			}
		}
	}
	return TRUE;
}

BOOL CRunProcessOffloadShellStack::CheckOutputCanActive(COutput *pOutput)
{
	return TRUE;
}

BOOL CRunProcessOffloadShellStack::ModuleIsSafe()
{	
	if (m_nMotorZ > -1 && MOTORS[m_nMotorZ]->GetCurPos() > 500)
	{
		return FALSE;
	}
	return TRUE;
}

/****************************************************************************************
函数描述：判断没有料盘
输入参数：
函数返回：TRUE:没有料盘，FALSE:有料盘
*****************************************************************************************/
BOOL CRunProcessOffloadShellStack::CheckHaveNoPallet()
{
	ASSERT(m_IHavePallet > -1);

	BOOL bCheckRes = TRUE;

	for(int i = 0; i < 3; i++)
	{
		if(INPUTS[m_IHavePallet]->IsOn())
		{
			bCheckRes = FALSE;
			break;
		}
		Sleep(100);
	}

	return bCheckRes;
}

BOOL CRunProcessOffloadShellStack::MonitorOut()
{	
	return TRUE;
}

BOOL CRunProcessOffloadShellStack::CheckCarAndPallet( BOOL bHas /*= TRUE*/ )
{
	ASSERT(m_IHavePallet > -1 && m_ICarInPosBehind > -1 && m_ICarInPosFront > -1);

	BOOL bHasPallet = TRUE;

	if (bHas)
	{
		for(int i = 0; i< 3;i++)
		{
			if (INPUTS[m_IHavePallet]->IsOff() && INPUTS[m_ICarInPosBehind]->IsOff() && INPUTS[m_ICarInPosFront]->IsOff())
			{
				bHasPallet = FALSE;
				break;
			}
			Sleep(100);
		}
	}
	else
	{
		for(int i = 0; i< 3;i++)
		{
			if (INPUTS[m_IHavePallet]->IsOn() || INPUTS[m_ICarInPosBehind]->IsOn() || INPUTS[m_ICarInPosFront]->IsOn())
			{
				bHasPallet = FALSE;
				break;
			}
			Sleep(100);
		}
	}
	return bHasPallet;
}

BOOL CRunProcessOffloadShellStack::MotorHandMove( CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove )
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

		BOOL bMove = bAbsMove ? lfValue > MOTORS[m_nMotorZ]->GetCurPos() : lfValue > 0.1;
		BOOL bDownMove = bAbsMove ? lfValue < MOTORS[m_nMotorZ]->GetCurPos() : lfValue < 0.1;

		if ((m_ICarInPosBehind < 0 || INPUTS[m_ICarInPosBehind]->IsOn() || m_ICarInPosFront < 0 || INPUTS[m_ICarInPosFront]->IsOn())
			&& MOTORS[m_nMotorZ]->GetCurPos() > GetMotorPos(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorZ, STACK_MOTOR_SAFE_POS)+1.0 &&bDownMove)
		{
			BLMessageBox(_T("托盘不在安全位，检测到有小车，Z轴不能往下移动，请将小车取走移到安全位后再往下移动"), MB_OK);
			return FALSE;
		}

		if (bAbsMove)
		{
			MOTORS[m_nMotorZ]->TMoveAbs(lfValue, iLoc);
		}
		else
		{
			MOTORS[m_nMotorZ]->TMoveRel(lfValue, iLoc);
		}

		DWORD dwStartTime = GetTickCount();
		Sleep(200);
		while(1)
		{
			if ((m_ITrayTopPresent < 0 ||  INPUTS[m_ITrayTopPresent]->IsOn()) && bMove)
			{
				MOTORS[m_nMotorZ]->Stop(1);
				break;
			}

			if (M_BUSY != MOTORS[m_nMotorZ]->GetMotionStatus())
			{
				MOTORS[m_nMotorZ]->Stop(1);
				break;
			}

			if (GetTickCount() - dwStartTime > 20000)
			{
				MOTORS[m_nMotorZ]->Stop();
				BLMessageBox(_T("Z轴10秒内没移动到目标位置，请继续移动！"), MB_OK);
				break;
			}
			Sleep(1);
		}
		return FALSE;//防止二次移动，必须为FALSE
	}
	return TRUE;
}

BOOL CRunProcessOffloadShellStack::  ReadRunParameter()
{
	CRunProcess:: ReadRunParameter();
	m_dlZSafeLimit = IniReadDouble(m_strProcess, _T("ZSafeLimit"), 150.00, GetAbsolutePathName(PARAMETER_CFG));
	
	return TRUE;
}

BOOL CRunProcessOffloadShellStack::SaveParameter()
{
	if (!IniWriteDouble(m_strProcess, _T("ZSafeLimit"), m_dlZSafeLimit, GetAbsolutePathName(PARAMETER_CFG)))
	{
		return FALSE;
	}

	CRunProcess::SaveParameter();
	return TRUE;
}
BOOL CRunProcessOffloadShellStack::CheckHavePaller(BOOL bHas /*= TRUE*/)
{
	BOOL bHasPallet = TRUE;//有盘标志
	if (bHas)
	{
		for(int i = 0; i< 3;i++)
		{
			if (m_IHavePallet > -1 && INPUTS[m_IHavePallet]->IsOff() )
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

BOOL CRunProcessOffloadShellStack::MotorMoveCheckFullPallet(void)
{
	ASSERT(m_ITrayTopPresent > -1);

	BOOL bMotorZAccess = FALSE;

	if(m_nMotorZ > -1)
	{
		if(INPUTS[m_ITrayTopPresent]->IsOff())
		{
			MOTORS[m_nMotorZ]->m_dAloneSpeedRatio = 1;
			double dLocPos = GetMotorPos(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorZ,STACK_MOTOR_RECV_PALLET);
			MOTORS[m_nMotorZ]->TMoveAbs(dLocPos);
			Sleep(300);

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
					break;
				}

				if (m_bDryRun && GetTickCount() - dwMotorWorkTime > 3000)  //空跑判断
				{
					bMotorZAccess = TRUE;
					MOTORS[m_nMotorZ]->Stop(1);
					Sleep(5);
					break;
				}

				if (IsNoHardware() || INPUTS[m_ITrayTopPresent]->IsOn())
				{
					bMotorZAccess = TRUE;
					MOTORS[m_nMotorZ]->Stop(1);
					Sleep(5);
					break;
				}
				Sleep(2);
			}
			MOTORS[m_nMotorZ]->Stop(1);
		}
		else
		{
			bMotorZAccess = TRUE;
		}


		if(!m_bDryRun&&bMotorZAccess)
		{
			if(WAIT_OK != CheckInput(INPUTS[m_ITrayTopPresent], ON))
			{
				bMotorZAccess = FALSE;
			}
		}
	}
	
	return bMotorZAccess;
}