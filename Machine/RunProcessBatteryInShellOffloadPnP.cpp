/****************************************************************************************
文件名：RunProcessBatteryInShellOffloadPnP.cpp
功能描述：入壳后下料抓手
历史修改记录：
*****************************************************************************************/
#include "StdAfx.h"
#include "RunProcessBatteryInShellOffloadPnP.h"

CRunProcessBatteryInShellOffloadPnP::CRunProcessBatteryInShellOffloadPnP(int nID) :
CRunProcess(nID)
{
	PowerUpRestart();

	m_InputMap.insert(pair<int*, CString>(&m_IPnPSendCylPush, _T("IPnPSendCylPush")));
	m_InputMap.insert(pair<int*, CString>(&m_IPnPSendCylBack, _T("IPnPSendCylBack")));
	m_OutputMap.insert(pair<int*, CString>(&m_OPnPSendCylPush, _T("OPnPSendCylPush")));
	m_OutputMap.insert(pair<int*, CString>(&m_OPnPSendCylBack, _T("OPnPSendCylBack")));

	m_InputMap.insert(pair<int*, CString>(&m_IFingerCylClose, _T("IFingerCylClose")));
	m_InputMap.insert(pair<int*, CString>(&m_IFingerCylOpen, _T("IFingerCylOpen")));
	m_OutputMap.insert(pair<int*, CString>(&m_OFingerCylClose, _T("OFingerCylClose")));
	m_OutputMap.insert(pair<int*, CString>(&m_OFingerCylOpen, _T("OFingerCylOpen")));

	m_InputMap.insert(pair<int*, CString>(&m_IHaveCore, _T("IHaveCore")));

	m_MotorMap.insert(pair<int*,CString>(&m_nMotorX,_T("MotorX")));
}

CRunProcessBatteryInShellOffloadPnP::~CRunProcessBatteryInShellOffloadPnP(void)
{
}

void CRunProcessBatteryInShellOffloadPnP::PowerUpRestart()
{
	CRunProcess::PowerUpRestart();
	m_strMsg = m_bLangulageEng?_T("Ready"):_T("准备好");

	m_nStartWithPalletData = -1;
	m_bOpenCylOpen = FALSE;
	m_bPnPSendCylReturn = FALSE;
	m_bOffloadToRobot = FALSE;
	m_bHomeMoveSafePos = FALSE;
	m_bRequirePallet = FALSE;
	m_bOffload = FALSE;

	m_nNextPowerUpStep = PU_WAIT_ROBOT_HOME_FINISH;
	m_nNextAutoStep = AUTO_SEND_SHELL_CORE_REQUIRE;
}

void CRunProcessBatteryInShellOffloadPnP::PowerUpOperation(void)
{
	if (!IsModuleEnable())
	{
		CRun::PowerUpOperation();
		m_strMsg = m_bLangulageEng?_T("Moudle UnEnable"):_T("模块禁用");
		return;
	}

	switch(m_nNextPowerUpStep)
	{

	case PU_WAIT_ROBOT_HOME_FINISH:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Modules Home Finish"):_T("机器人复位完成信号");
			if(IsNoHardware() || (MACHINE.m_pRobotOffload != NULL && (MACHINE.m_pRobotOffload->m_bRobotSafe || !MACHINE.m_pRobotOffload->IsModuleEnable())))
			{
				m_nNextPowerUpStep = PU_JUDGE_IF_WITH_PALLET;
			}
			break;
		}

	case PU_JUDGE_IF_WITH_PALLET:
		{
			m_strMsg = m_bLangulageEng?_T("Move XY To Onload Pos"):_T("判断带料启动");
			if(MACHINE.m_bReCoverMachine)
			{
				ReadStepInfo();
				m_nNextPowerUpStep = PU_FINGER_CYL_CLOSE_WITH_PALLET;
			}
			else
			{
				m_nNextPowerUpStep = PU_CHECK_WITHOUT_BATTER;
			}
			break;
		}

		//检测无电池
	case PU_CHECK_WITHOUT_BATTER:
		{
			m_strMsg = m_bLangulageEng?_T("Check Without Shell"):_T("检测无电池");
			m_nNextPowerUpStep = PU_FINGER_CYL_OPEN;
			break;
		}

		//夹爪气缸气缸松开
	case PU_FINGER_CYL_OPEN:
		{
			m_strMsg = m_bLangulageEng?_T("Finger Cyl Open"):_T("夹爪打开");
			if(::IsNoHardware() || FingerCylClose(FALSE))
			{
				m_nNextPowerUpStep = PU_MOTOR_HOME;
			}
			break;
		}

		//电机复位
	case PU_MOTOR_HOME:
		{
			m_strMsg = m_bLangulageEng?_T("Motor Home"):_T("电机复位");
			ASSERT(-1 < m_nGroupID && m_nGroupID <= 1);
			if(IsNoHardware() || MACHINE.m_pBatteryInShell != NULL && MACHINE.m_pBatteryInShell[m_nGroupID]->DownLiftCylUp(FALSE))
			{
				if (m_nMotorX > -1 && HomeMotor(MOTORS[m_nMotorX]))
				{
					m_nNextPowerUpStep = PU_MOTOR_X_TO_WAIT_POS;
				}
			}
			break;
		}

		//X轴到等待位
	case PU_MOTOR_X_TO_WAIT_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Motor X To Wait Pos"):_T("X轴到等待位");

			if (m_nMotorX > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorX, X_MOTOR_WAIT_POS))
			{
				m_bHomeMoveSafePos = TRUE;
				m_nNextPowerUpStep = PU_FINISH;
			}
			break;
		}

		//带料启动
		//带料启动，夹爪加紧或松开
	case PU_FINGER_CYL_CLOSE_WITH_PALLET:
		{
			m_strMsg = m_bLangulageEng?_T("Finger Cyl Open"):_T("带料启动，夹爪加紧或松开");

			int nAutoStep =m_nNextAutoStep;
			if (AUTO_FINGER_CYL_CLOSE > nAutoStep)// 没有取料
			{
				if(::IsNoHardware() || FingerCylClose(FALSE))
				{
					m_nNextPowerUpStep = PU_MOTOR_HOME_WITH_PALLET;
				}
			}
			else if (AUTO_FINGER_CYL_CLOSE <= nAutoStep)
			{
				if(::IsNoHardware() || FingerCylClose(TRUE))
				{
					ASSERT(0 <= m_nGroupID && m_nGroupID <= 1);
					if (MACHINE.m_pBatteryInShell[m_nGroupID] != NULL && MACHINE.m_pBatteryInShell[m_nGroupID]->ColumnCylClose(FALSE))
					{
						m_nNextPowerUpStep = PU_MOTOR_HOME_WITH_PALLET;
					}
				}
			}
			break;
		}

		//电机复位
	case PU_MOTOR_HOME_WITH_PALLET:
		{
			m_strMsg = m_bLangulageEng?_T("Motor Home"):_T("带料启动，电机复位");
			ASSERT(-1 < m_nGroupID && m_nGroupID <= 1);
			if(IsNoHardware() || MACHINE.m_pBatteryInShell != NULL && MACHINE.m_pBatteryInShell[m_nGroupID]->DownLiftCylUp(FALSE))
			{
				if (m_nMotorX > -1 && HomeMotor(MOTORS[m_nMotorX]))
				{
					m_nNextPowerUpStep = PU_MOTOR_X_TO_WITH_PALLET;
				}
			}
			break;
		}

		//X轴到等待位
	case PU_MOTOR_X_TO_WITH_PALLET:
		{
			m_strMsg = m_bLangulageEng?_T("Motor X To Wait Pos"):_T("带料启动，X轴到等待位");

			if (m_nMotorX > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorX, X_MOTOR_AVOID_POS))
			{
				m_bHomeMoveSafePos = TRUE;
				m_nNextPowerUpStep = PU_CHECK_PALLET_DATA;
			}
			break;
		}


		//检测料盘数据与实际是否一致
	case PU_CHECK_PALLET_DATA:
		{
			m_strMsg = m_bLangulageEng?_T("Move XY To Onload Pos"):_T("检测料盘数据与实际是否一致");

			if(CheckStartWithPalletDataSame())
			{
				if(m_nStartWithPalletData == 0)
				{
					m_Pallet.Release();
					m_nNextAutoStep = AUTO_SEND_SHELL_CORE_REQUIRE;
					m_nNextPowerUpStep = PU_CHECK_WITHOUT_BATTER;
				}
				else if(m_nStartWithPalletData == 1)
				{
					m_nNextAutoStep = AUTO_MOTOR_X_TO_WIAT_POS;
					m_nNextPowerUpStep = PU_WAIT_JUDGE_FINISH;
				}
				m_bDataJudgeFinish = TRUE;
			}

			break;
		}

		//等待数据判断完成
	case PU_WAIT_JUDGE_FINISH:
		{
			m_strMsg = m_bLangulageEng?_T("Check Without Shell"):_T("等待数据判断完成");
			int nID = m_nGroupID == 0?1:0;
			if (m_bDataJudgeFinish && MACHINE.m_pBatteryOffloadPnp[nID]->m_bDataJudgeFinish)
			{
				Sleep(200);
				m_bResetJudge =TRUE;
				m_nNextPowerUpStep = PU_CHECK_WITHOUT_BATTERY_WAIT_PALLTE;
			}
			break;
		}


		//人工清除壳，检测两个工位电池状态一致
	case PU_CHECK_WITHOUT_BATTERY_WAIT_PALLTE:
		{
			m_strMsg = m_bLangulageEng?_T("Check Without Shell"):_T("人工清除壳，检测两个工位电池状态一致");

			int nID = m_nGroupID == 0?1:0;
			ASSERT(MACHINE.m_pBatteryOffloadPnp[nID] != NULL);
			int IHave = MACHINE.m_pBatteryOffloadPnp[nID]->m_IHaveCore;
			if (m_bResetJudge && MACHINE.m_pBatteryOffloadPnp[nID]->m_bResetJudge) 
			{
				m_bDataJudgeFinish = FALSE;
			}
			else
			{
				break;
			}

			if (IsNoHardware() || (m_IHaveCore < 0 || INPUTS[m_IHaveCore]->IsOn()) && (IHave < 0 || INPUTS[IHave]->IsOn()))
			{
				m_nNextPowerUpStep = PU_FINISH;
			}
			else
			{
				m_Pallet.Release();
				m_bResetJudge = FALSE;
				BLMessageBox(_T("请人工入壳后夹爪上的产品，确保两个工位电池有无状态一致"));
				m_nNextPowerUpStep = PU_CHECK_PALLET_DATA;
			}
			break;
		}

		//初始化完成
	case PU_FINISH:
		{
			CRun::PowerUpOperation();
			m_strMsg = m_bLangulageEng?_T("Power up finished"):_T("初始化完成");
			m_bHomeMoveSafePos = TRUE;
			break;
		}
	default:
		{
			ASSERT(0);
			break;
		}
	}
}

void CRunProcessBatteryInShellOffloadPnP::AutoOperation()
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
		if (m_nSaveLastStep == AUTO_SEND_SHELL_CORE_REQUIRE)
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
		//发送要料信号
	case AUTO_SEND_SHELL_CORE_REQUIRE:
		{
			m_strMsg = m_bLangulageEng?_T("Send Need Core Require"):_T("发送要料信号");
			if(!m_bRequirePallet || (MACHINE.m_pBatteryInShell[m_nGroupID] != NULL && !MACHINE.m_pBatteryInShell[m_nGroupID]->IsModuleEnable()))
			{
				m_bRequirePallet = TRUE;
				m_nNextAutoStep = AUTO_START_EVENT;
				SaveStepInfo(WRITE_TYPE_AUTO_STEP);
			}
			break;
		}	

		//等待开始信号
	case AUTO_START_EVENT:
		{
			m_strMsg = m_bLangulageEng?_T("Start Event"):_T("等待开始信号");

			if(!m_bRequirePallet 
				|| (MACHINE.m_pBatteryInShell[m_nGroupID] != NULL && !MACHINE.m_pBatteryInShell[m_nGroupID]->IsModuleEnable()))
			{
				m_nNextAutoStep = AUTO_MOTOR_X_TO_PICK_POS;				
			}
			break;
		}

		//X轴到取料位
	case AUTO_MOTOR_X_TO_PICK_POS:  
		{
			m_strMsg = m_bLangulageEng?_T("Motor X To Pick Pos"):_T("Y轴到取料位");

			if (m_nMotorX > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorX, X_MOTOR_PICK_POS))
			{
				m_nNextAutoStep = AUTO_FINGER_CYL_CLOSE;	
			}
			break;
		}

		//夹爪夹紧
	case AUTO_FINGER_CYL_CLOSE:
		{
			m_strMsg = m_bLangulageEng?_T("Finger Cyl Close"):_T("夹抓夹紧气缸夹紧");

			if(::IsNoHardware() || FingerCylClose(TRUE))
			{	
				if(MACHINE.m_pBatteryInShell[m_nGroupID] != NULL)
				{
					m_Pallet = MACHINE.m_pBatteryInShell[m_nGroupID]->m_Pallet;
					MACHINE.m_pBatteryInShell[m_nGroupID]->m_Pallet.Release();
					MACHINE.m_pBatteryInShell[m_nGroupID]->m_bShellVaild = FALSE;

					//if (m_nGroupID == 0) //带料无硬件测试
					//{
					//	m_Pallet.m_bIsValid = TRUE;
					//	m_Pallet.m_strBarCode = _T("1");
					//	m_Pallet.m_bIsNg = TRUE;
					//	m_Pallet.m_dPressNormValue[0] = 11;
					//	m_Pallet.m_dPressNormValue[1] = 12;
					//}
					//if (m_nGroupID == 1)
					//{
					//	m_Pallet.m_bIsValid = TRUE;
					//	m_Pallet.m_strBarCode = _T("2");
					//	m_Pallet.m_bIsNg = FALSE;
					//	m_Pallet.m_dPressNormValue[0] = 21;
					//	m_Pallet.m_dPressNormValue[1] = 22;
					//}


					m_nNextAutoStep = AUTO_CLUMMN_CYL_OPEN;		

					SaveStepInfo(WRITE_TYPE_AUTO_STEP|WRITE_TYPE_BATTERY);
					MACHINE.m_pBatteryInShell[m_nGroupID]->SaveStepInfo(WRITE_TYPE_AUTO_STEP|WRITE_TYPE_BATTERY);
				}
			}
			break;
		}

			//整列气缸打开
	case AUTO_CLUMMN_CYL_OPEN:
		{
			m_strMsg = m_bLangulageEng?_T("Clumn Cyl Open"):_T("整列气缸打开");
			ASSERT(0 <= m_nGroupID && m_nGroupID <= 1);
			if (MACHINE.m_pBatteryInShell[m_nGroupID] != NULL && MACHINE.m_pBatteryInShell[m_nGroupID]->ColumnCylClose(FALSE))
			{
				m_nNextAutoStep = AUTO_MOTOR_X_TO_WIAT_POS;
			}
			break;
		}

	
		//X轴到下料位
	case AUTO_MOTOR_X_TO_WIAT_POS:  
		{
			m_strMsg = m_bLangulageEng?_T("Motor X To Wait Pos"):_T("Y轴到下料位,开合气缸关闭");

			if (m_nMotorX > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorX, X_MOTOR_WAIT_POS))
			{
				ASSERT(0 <= m_nGroupID && m_nGroupID <= 1);
				if (MACHINE.m_pBatteryCore[m_nGroupID] != NULL && MACHINE.m_pBatteryCore[m_nGroupID]->OpenCylOpen(FALSE))   
				{
					m_nNextAutoStep = AUTO_SEND_RETURN_SINGLE;
				}
			}
			break;
		}

		//发送回退信号
	case AUTO_SEND_RETURN_SINGLE:
		{
			m_strMsg = m_bLangulageEng?_T("Send Cyl Return Single"):_T("发送回退信号");

			if(MACHINE.m_pBatteryInShell[m_nGroupID]!=NULL)
			{
				int nHaveCore = MACHINE.m_pBatteryInShell[m_nGroupID]->m_IHaveCore;
				if (m_bDryRun || nHaveCore < 0 || CheckInput(INPUTS[nHaveCore],OFF))
				{
					m_bPnPSendCylReturn = TRUE;
					m_nNextAutoStep = AUTO_SEND_OFFLAD_SINGLE;

					SaveStepInfo(WRITE_TYPE_AUTO_STEP|WRITE_TYPE_VARIABLES);
				}
			}
			break;
		}
	//	//等待MES信号
	//case AUTO_WAIT_MES_SINGLE:
	//	{
	//		m_strMsg = m_bLangulageEng?_T("Wait Mes Single"):_T("等待MES信号");
	//		if(MACHINE.m_pUploadMes[m_nGroupID] != NULL && (MACHINE.m_pUploadMes[m_nGroupID]->m_bNextRequire || !MACHINE.m_pUploadMes[m_nGroupID]->IsModuleEnable()))
	//		{
	//			MACHINE.m_pUploadMes[m_nGroupID]->m_Pallet = m_Pallet;
	//			MACHINE.m_pUploadMes[m_nGroupID]->m_bNextRequire = FALSE;
	//			m_nNextAutoStep = AUTO_SEND_OFFLAD_SINGLE;
	//		}
	//		break;
	//	}
		//发送下料信号
	case AUTO_SEND_OFFLAD_SINGLE:
		{
			m_strMsg = m_bLangulageEng?_T("Send Offload Single"):_T("发送下料信号");

			if((MACHINE.m_pBatteryInShell[m_nGroupID] != NULL && !MACHINE.m_pBatteryInShell[m_nGroupID]->IsModuleEnable()))
			{
				m_bPnPSendCylReturn = FALSE;			
			}

			if(!m_bPnPSendCylReturn && !m_bOffloadToRobot)
			{
				m_bOffloadToRobot = TRUE;
				m_nNextAutoStep = AUTO_WAIT_OFFLOAD_SINGLE;

				SaveStepInfo(WRITE_TYPE_AUTO_STEP|WRITE_TYPE_VARIABLES);
			}
			break;
		}

		//等待下料开始信号
	case AUTO_WAIT_OFFLOAD_SINGLE:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Offload Single"):_T("等待下料开始信号");

			if(!m_bOffloadToRobot || (MACHINE.m_pRobotOffload != NULL && !MACHINE.m_pRobotOffload->IsModuleEnable()))
			{
				if(MACHINE.m_pRobotOffload != NULL && !MACHINE.m_pRobotOffload->IsModuleEnable())
				{
					m_bOffloadToRobot = FALSE;
				}
				m_nNextAutoStep = AUTO_FINGER_CYL_OPEN;
				SaveStepInfo(WRITE_TYPE_AUTO_STEP|WRITE_TYPE_VARIABLES);
			}
			break;
		}

		//夹抓气缸打开
	case AUTO_FINGER_CYL_OPEN:
		{
			m_strMsg = m_bLangulageEng?_T("Finger Cyl Open"):_T("夹抓气缸打开");

			if(IsNoHardware() || FingerCylClose(FALSE))
			{
				m_nNextAutoStep = AUTO_SEND_MOVE_AVOID;
			}
			break;
		}
	case AUTO_SEND_MOVE_AVOID:
		{
			m_strMsg = m_bLangulageEng?_T("Send Clamp Open Single"):_T("发送夹爪松开信号和避位回退");

			if(m_nMotorX > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorX, X_MOTOR_AVOID_POS))
			{
				m_bOpenCylOpen = TRUE;
				m_nNextAutoStep = AUTO_WAIT_OFFLAOD_FINISH;			
				SaveStepInfo(WRITE_TYPE_AUTO_STEP|WRITE_TYPE_VARIABLES);
			}
			break;
		}

		//等待下料完成
	case AUTO_WAIT_OFFLAOD_FINISH:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Offload Finish"):_T("等待下料完成");

			if(!m_bOpenCylOpen || (MACHINE.m_pRobotOffload !=NULL  && !MACHINE.m_pRobotOffload->IsModuleEnable()))
			{
				m_Pallet.Release();
				m_nNextAutoStep = AUTO_MOTOR_MOVE_PICK;
				SaveStepInfo(WRITE_TYPE_AUTO_STEP|WRITE_TYPE_VARIABLES);

			}
			break;
		}
	case AUTO_MOTOR_MOVE_PICK:
		{
			m_strMsg = m_bLangulageEng?_T("Send Clamp Open Single"):_T("移动Y轴到下料位");
			if(m_nMotorX > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorX, X_MOTOR_WAIT_POS))
			{
				m_nNextAutoStep = AUTO_WORK_END;
			}
			break;
		}
		//工作完成
	case AUTO_WORK_END:
		{
			m_strMsg = m_bLangulageEng?_T("Work end"):_T("工作完成");
			m_nNextAutoStep = AUTO_SEND_SHELL_CORE_REQUIRE;
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

		if(m_nNextAutoStep == AUTO_SEND_SHELL_CORE_REQUIRE)
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

BOOL CRunProcessBatteryInShellOffloadPnP::InitializeConfig(CString strProcessModule)
{
	if (!CRunProcess::InitializeConfig(strProcessModule))
	{
		return FALSE;
	}	
	return TRUE;
}

void CRunProcessBatteryInShellOffloadPnP::GetHardwareConfig(CArray<int, int> *pInputs, CArray<int, int> *pOutputs, CArray<int, int> *pMotors)
{
	CRunProcess::GetHardwareConfig(pInputs, pOutputs, pMotors);
}


BOOL CRunProcessBatteryInShellOffloadPnP::CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{
//	return TRUE; //去掉保护 201905051106
	if(IsNoHardware())
	{
		return TRUE;
	}

	ASSERT(m_nMotorX > -1);
	if (pMotor == MOTORS[m_nMotorX])
	{
		//机器人
		if (MACHINE.m_pRobotOffload != NULL && !MACHINE.m_pRobotOffload->CheckRobotSafe())
		{
			BLMessageBox(_T("机器人不在安全位，电机不能移动"), MB_ICONWARNING);
			return FALSE;
		}
		//铝壳搬运
		if (MACHINE.m_pCarryShellPnp != NULL)
		{
			int nMotorZ = MACHINE.m_pCarryShellPnp->m_nMotorZ;
			ASSERT(nMotorZ > -1);
			double dPos0 = MOTORS[nMotorZ]->GetCurPos(); 
			if (dPos0 > 5)
			{
				CString strKey;
				strKey.Format(_T("外壳移载Z轴电机(M05)不在安全位，入壳后下料抓手%d电机不能动"), m_nGroupID);
				BLMessageBox(strKey, MB_ICONWARNING);
				return FALSE;
			}
		}

		//入壳工位
		if (MACHINE.m_pBatteryInShell[m_nGroupID] != NULL)
		{
			int ILiftCylUp = MACHINE.m_pBatteryInShell[m_nGroupID]->m_ILiftCylUp;
			int m_ILiftCylDown = MACHINE.m_pBatteryInShell[m_nGroupID]->m_ILiftCylDown;
			ASSERT(ILiftCylUp > -1 && ILiftCylUp > -1);
			if ( INPUTS[ILiftCylUp]->IsOn() || INPUTS[m_ILiftCylDown]->IsOff())
			{
				CString strKey;
				strKey.Format(_T("入铝壳工位%d，外壳顶升气缸没有回退，电机不能移动"), m_nGroupID);
				BLMessageBox(strKey, MB_ICONWARNING);
				return FALSE;
			}

			int ISendOneCylPush = MACHINE.m_pBatteryInShell[m_nGroupID]->m_ISendOneCylPush;
			int ISendOneCylBack = MACHINE.m_pBatteryInShell[m_nGroupID]->m_ISendOneCylBack;
			ASSERT(ISendOneCylPush > -1 && ISendOneCylBack > -1);
			if ( INPUTS[ISendOneCylBack]->IsOff() || INPUTS[ISendOneCylPush]->IsOn())
			{
				CString strKey;
				strKey.Format(_T("入铝壳工位%d，推出气缸没有推出，电机不能移动"), m_nGroupID+1);
				BLMessageBox(strKey, MB_ICONWARNING);
				return FALSE;
			}

			int IColumnCylOpen[2];
			int IColumnCylClose[2];
			for (int i = 0; i < 2; i++)
			{
				IColumnCylClose[i] = MACHINE.m_pBatteryInShell[m_nGroupID]->m_IColumnCylClose[i];
				IColumnCylOpen[i] = MACHINE.m_pBatteryInShell[m_nGroupID]->m_IColumnCylOpen[i];
				ASSERT(IColumnCylOpen[i] > -1 && IColumnCylClose[i] > -1);
			}
			/*if ( INPUTS[IColumnCylClose[0]]->IsOn() || INPUTS[IColumnCylClose[1]]->IsOn() 
				|| INPUTS[IColumnCylOpen[0]]->IsOff() || INPUTS[IColumnCylOpen[1]]->IsOff())
			{
				CString strKey;
				strKey.Format(_T("入铝壳工位%d，外壳定位气缸1、2没有回退，电机不能移动"), m_nGroupID);
				BLMessageBox(strKey, MB_ICONWARNING);
				return FALSE;
			}*/
		}
	}

	return TRUE;
}

BOOL  CRunProcessBatteryInShellOffloadPnP::CheckOutputCanActive(COutput *pOutput)
{
	return TRUE;
}



/****************************************************************************************
函数描述：无杆气缸动作
输入参数：bPush:		TRUE:无杆气缸推出	 FALSE:无杆气缸回退
函数返回：TRUE:执行成功，FALSE:执行失败
*****************************************************************************************/
BOOL CRunProcessBatteryInShellOffloadPnP:: PnPSendCylPush(BOOL bPush)
{
	if(m_IPnPSendCylPush < 0 || m_IPnPSendCylBack < 0 || m_OPnPSendCylPush < 0 || m_OPnPSendCylBack < 0 )
	{
		return FALSE;
	}

	if(bPush)
	{
		OUTPUTS[m_OPnPSendCylBack]->Off();
		OUTPUTS[m_OPnPSendCylPush]->On();
		if(m_bDryRun)
		{
			Sleep(1200);
			return TRUE;
		}
		if(WAIT_OK == WaitInput(INPUTS[m_IPnPSendCylPush], ON, MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_IPnPSendCylBack], OFF, MSG_SHOW_DEFAULT))
		{
			return TRUE;
		}
	}
	else
	{
		OUTPUTS[m_OPnPSendCylPush]->Off();
		OUTPUTS[m_OPnPSendCylBack]->On();
		if(m_bDryRun)
		{
			Sleep(1200);
			return TRUE;
		}
		if(WAIT_OK == WaitInput(INPUTS[m_IPnPSendCylPush], OFF, MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_IPnPSendCylBack], ON, MSG_SHOW_DEFAULT))
		{
			return TRUE;
		}
	}
	return FALSE;
}

/****************************************************************************************
函数描述：夹爪气缸动作
输入参数：bClose:		TRUE:夹爪夹紧	 FALSE:夹爪松开
函数返回：TRUE:执行成功，FALSE:执行失败
*****************************************************************************************/
BOOL CRunProcessBatteryInShellOffloadPnP:: FingerCylClose(BOOL bClose)
{
	ASSERT(m_IFingerCylClose > -1 && m_IFingerCylOpen > -1 && m_OFingerCylClose > -1 && m_OFingerCylOpen > -1);

	if(bClose)
	{
		OUTPUTS[m_OFingerCylOpen]->Off();
		OUTPUTS[m_OFingerCylClose]->On();
		if(m_bDryRun)
		{
			Sleep(200);
			return TRUE;
		}

		if(WAIT_OK == WaitInput(INPUTS[m_IFingerCylClose], ON, MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_IFingerCylOpen], OFF, MSG_SHOW_DEFAULT))
		{
			return TRUE;
		}
	}
	else
	{
		OUTPUTS[m_OFingerCylClose]->Off();
		OUTPUTS[m_OFingerCylOpen]->On();

		if(WAIT_OK == WaitInput(INPUTS[m_IFingerCylClose], OFF, MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_IFingerCylOpen], ON, MSG_SHOW_DEFAULT))
		{
			return TRUE;
		}
	}
	return FALSE;
}

/****************************************************************************************
函数描述：读取数据
输入参数：
函数返回：TRUE:执行成功，FALSE:执行失败
*****************************************************************************************/
void CRunProcessBatteryInShellOffloadPnP::ReadStepInfo()				//读取带料启动数据
{
	CString strPath = GetAbsolutePathName(MODULE_RUN_FOLDER) + m_strName + _T(".cfg");

	m_nNextAutoStep = IniReadInt(_T("Data"), _T("m_nNextAutoStep"), 0, strPath);

	m_Pallet.ReadCfg(_T("Pallet"), strPath);
	return ;
}

/****************************************************************************************
函数描述：保存带料启动数据
输入参数：
函数返回：TRUE:执行成功，FALSE:执行失败
*****************************************************************************************/
BOOL CRunProcessBatteryInShellOffloadPnP::SaveStepInfo(int nWriteType)			//保存带料启动数据
{
	CString strPath = GetAbsolutePathName(MODULE_RUN_FOLDER) + m_strName + _T(".cfg");

	if(nWriteType & WRITE_TYPE_AUTO_STEP)
	{
		IniWriteInt(_T("Data"), _T("m_nNextAutoStep"), m_nNextAutoStep, strPath);
	}

	if(nWriteType & WRITE_TYPE_VARIABLES)
	{
	}

	if(nWriteType & WRITE_TYPE_BATTERY)
	{
		m_Pallet.WriteCfg(_T("Pallet"), strPath);
	}
	return TRUE;
}

/****************************************************************************************
函数描述：检测夹爪料盘数据与实际检测数据一致性,m_nStartWithPalletData为输出参数0 没有料，1 只有壳，2：入壳完成
输入参数：
函数返回：TRUE数据正常  FALSE：数据异常需要重新检测
*****************************************************************************************/
BOOL CRunProcessBatteryInShellOffloadPnP::CheckStartWithPalletDataSame()
{
	CString strMsg = _T("");
	if(m_Pallet.m_bIsValid)
	{
		if((m_IHaveCore > -1 && INPUTS[m_IHaveCore]->IsOff()))
		{
			strMsg.Format(_T("带料启动入壳后下料机械手有料感应与实际数据不一致,数据有料,感应无料,请确认"));
			ShowMessageEX(strMsg);
			strMsg.Format(_T("确认入壳后下料机械手有料吗？有料点YES，无料点NO"));
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
			strMsg.Format(_T("带料启动入壳后下料机械手有料感应与实际数据不一致,数据无料,感应有料,请确认"));
			ShowMessageEX(strMsg);
			strMsg.Format(_T("确认入壳后下料机械手有料吗？有料点YES，无料点NO"));
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

	if (!m_Pallet.m_bIsValid)
	{
		m_nStartWithPalletData = 0 ;
	} 
	else
	{
		m_nStartWithPalletData = 1 ;
	}
	return TRUE;
}

/****************************************************************************************
函数描述：获取X轴偏移位位置
输入参数：
函数返回： 返回X轴偏移位位置
*****************************************************************************************/
double CRunProcessBatteryInShellOffloadPnP::GetMotorXAvoidPOS()
{
	ASSERT(m_nMotorX > -1);

	double dPos1 = GetMotorPos(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorX, X_MOTOR_AVOID_POS);

	return dPos1;
}