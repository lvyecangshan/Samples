/****************************************************************************************
文件名：RunProcessCleanShell.cpp
功能描述：外壳清洗模组
历史修改记录：
*****************************************************************************************/
#include "StdAfx.h"
#include "RunProcessCleanShell.h"
CRunProcessCleanShell::CRunProcessCleanShell(int nID) : CRunProcess(nID)
{
	PowerUpRestart();
	CString strKey = _T("");
	for(int i = 0; i < 2; i++)
	{
		strKey.Format(_T("IHaveProduct%d"), i);
		m_InputMap.insert(pair<int*, CString>(&m_IHaveProduct[i], strKey));

		strKey.Format(_T("IFingerCylClose%d"), i);
		m_InputMap.insert(pair<int*, CString>(&m_IFingerCylClose[i], strKey));

		strKey.Format(_T("IFingerCylOpen%d"), i);
		m_InputMap.insert(pair<int*, CString>(&m_IFingerCylOpen[i], strKey));
	}

	m_OutputMap.insert(pair<int*, CString>(&m_OFingerCylOpen, _T("OFingerCylOpen")));
	m_OutputMap.insert(pair<int*, CString>(&m_OFingerCylClose, _T("OFingerCylClose")));


	m_InputMap.insert(pair<int*, CString>(&m_ILiftCylDown, _T("ILiftCylDown")));
	m_InputMap.insert(pair<int*, CString>(&m_ILiftCylUp, _T("ILiftCylUp")));
	m_OutputMap.insert(pair<int*, CString>(&m_OLiftCylDown, _T("OLiftCylDown")));
	m_OutputMap.insert(pair<int*, CString>(&m_OLiftCylUp, _T("OLiftCylUp")));

	m_OutputMap.insert(pair<int*, CString>(&m_OAirBlow, _T("OAirBlow")));
	m_OutputMap.insert(pair<int*, CString>(&m_OAirExhaust, _T("OAirExhaust")));

	m_VoidMap.insert(pair<int*, CString>(&m_nAirTimelimit, _T("lAirTimelimit, 除尘时间(ms)")));
}

CRunProcessCleanShell::~CRunProcessCleanShell(void)
{
}

void CRunProcessCleanShell::PowerUpRestart()
{
	CRunProcess::PowerUpRestart();
	m_strMsg = m_bLangulageEng?_T("Ready"):_T("准备好");

	m_bRequirePallet = FALSE;
	m_bFingerClose = FALSE;
	m_bLiftDown = FALSE;
	m_bOffloadRequire = FALSE;

	m_nAirTime = 0;
	m_nNextPowerUpStep = PU_CLOSE_AIR;
	m_nNextAutoStep = AUTO_FINGER_CYL_OPEN;
}

void CRunProcessCleanShell::PowerUpOperation(void)
{
	if (!IsModuleEnable())
	{
		CRun::PowerUpOperation();
		m_strMsg = m_bLangulageEng?_T("Moudle UnEnable"):_T("模块禁用");
		return;
	}

	switch(m_nNextPowerUpStep)
	{
		//关闭吹气和吸气
	case PU_CLOSE_AIR:
		{
			m_strMsg = m_bLangulageEng?_T("Close Air"):_T("关闭吹气");

			if(m_OAirBlow > -1)
			{
				OUTPUTS[m_OAirBlow]->Off();
			}
			if(m_OAirExhaust > -1)
			{
				OUTPUTS[m_OAirExhaust]->Off();
			}

			m_nNextPowerUpStep = PU_JUDGE_IF_WITH_PALLET;
			break;
		}

		//判断是否带料
	case PU_JUDGE_IF_WITH_PALLET:
		{
			m_strMsg = m_bLangulageEng?_T("Judge If With Pallet"):_T("判断是否带料");

			if(MACHINE.m_bReCoverMachine)
			{
				m_nNextPowerUpStep = JUDGE_SHELL_NUM;
			}
			else
			{
				m_nNextPowerUpStep = PU_FINGER_CYL_OPEN;
			}
			break;
		}

		//夹紧气缸打开
	case PU_FINGER_CYL_OPEN:
		{
			m_strMsg = m_bLangulageEng?_T("Finger Cyl Open"):_T("夹紧气缸打开");

			if (IsNoHardware() || FingerCylClose(FALSE))
			{
				m_nNextPowerUpStep = PU_CHECK_WITHOUT_SHELL;
			}
			break;
		}

		//检测有无壳
	case PU_CHECK_WITHOUT_SHELL:
		{
			m_strMsg = m_bLangulageEng?_T("Check Without Shell"):_T("检测无壳");

			if (IsNoHardware() || m_bDryRun || ((m_IHaveProduct[0] > -1 && CheckInput(INPUTS[m_IHaveProduct[0]], OFF)) 
				&& (m_IHaveProduct[1] > -1 && CheckInput(INPUTS[m_IHaveProduct[1]], OFF))))
			{
				m_nNextPowerUpStep = PU_LIFT_CYL_DOWN;
			}
			break;
		}

		//顶升气缸下降
	case PU_LIFT_CYL_DOWN:
		{
			m_strMsg = m_bLangulageEng?_T("Lift Cyl Down"):_T("顶升气缸下降");

			if (IsNoHardware() || LiftCylUp(FALSE))
			{
				//清除数据
				m_bRequirePallet = FALSE;
				m_bForNextReady = FALSE;
				m_bFingerClose = FALSE;//夹爪夹紧信号
				m_bLiftDown = FALSE;//顶升下降信号

				m_nNextAutoStep = AUTO_FINGER_CYL_OPEN;
				m_nNextPowerUpStep = PU_FINISH;
			}
			break;
		}

		//检测壳数量
	case JUDGE_SHELL_NUM:
		{
			m_strMsg = m_bLangulageEng?_T("Check Pallet Date"):_T("检测壳数量");
			int nShellNum = JudgeShellNum();
			if (2 == nShellNum)
			{
				m_nNextPowerUpStep = PU_CHECK_FINGER_STATE;
			}
			else if (0 == nShellNum || 1 == nShellNum)
			{
				m_nNextPowerUpStep = PU_CHECK_WITHOUT_SHELL;
			}
			break;
		}

		//检测夹爪状态
	case PU_CHECK_FINGER_STATE:
		{
			m_strMsg = m_bLangulageEng?_T("Check Pallet Date"):_T("检测夹爪状态");

			ASSERT(m_IFingerCylClose[0] > -1 && m_IFingerCylOpen[0] > -1
				&& m_IFingerCylClose[1] > -1 && m_IFingerCylOpen[1] > -1);

			if (INPUTS[m_IFingerCylClose[0]]->IsOff() || INPUTS[m_IFingerCylOpen[0]]->IsOn()
				&& INPUTS[m_IFingerCylClose[1]]->IsOff() || INPUTS[m_IFingerCylOpen[1]]->IsOn())
			{
				//清洁模组夹爪打开，请清除铝壳
				ShowMessage(CLEAN_SHELL_FINGER_OPEN_WITHOUT_PALLET);
				m_nNextPowerUpStep = PU_CHECK_WITHOUT_SHELL;
			}
			else
			{
			    m_nAirTime = 0;
				m_nNextAutoStep = AUTO_WAIT_AIR_TIME_OUT;
				m_nNextPowerUpStep = PU_FINGER_CYL_CLOSE;
			}
			break;
		}

		
		//夹爪关闭
	case PU_FINGER_CYL_CLOSE:
		{
			m_strMsg = m_bLangulageEng?_T("Finger Cyl Open"):_T("夹爪关闭");

			if (IsNoHardware() || FingerCylClose(TRUE))
			{
				m_nNextPowerUpStep = PU_LIFT_CYL_UP;
			}
			break;
		}

		//顶升气缸上升
	case PU_LIFT_CYL_UP:
		{
			m_strMsg = m_bLangulageEng?_T("Lift Cyl Down"):_T("顶升气缸上升");

			if (IsNoHardware() || LiftCylUp(TRUE))
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

void CRunProcessCleanShell::AutoOperation()
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
		if (m_nSaveLastStep == AUTO_CHECK_WITHOUT_SHELL)
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
		//检测无壳
	case AUTO_CHECK_WITHOUT_SHELL:
		{
			m_strMsg = m_bLangulageEng?_T("Check Without Shell"):_T("检测无壳");

			if (IsNoHardware() || (m_IHaveProduct[0] > -1 && CheckInput(INPUTS[m_IHaveProduct[0]],OFF)
				&& m_IHaveProduct[1] > -1 && CheckInput(INPUTS[m_IHaveProduct[1]],OFF)))
			{
				m_nNextAutoStep = AUTO_FINGER_CYL_OPEN;	
			}
			break;
		}
		//夹抓气缸松开
	case AUTO_FINGER_CYL_OPEN:
		{
			m_strMsg = m_bLangulageEng?_T("Finger Cyl Open"):_T("夹抓气缸松开");

			if (IsNoHardware() || FingerCylClose(FALSE))
			{
				m_nNextAutoStep = AUTO_LIFT_CYL_DOWN;
			}
			break;
		}

		//顶升气缸回退
	case AUTO_LIFT_CYL_DOWN:
		{
			m_strMsg = m_bLangulageEng?_T("Lift Cyl Down"):_T("顶升气缸下降");

			if(IsNoHardware() || LiftCylUp(FALSE))
			{
				m_nNextAutoStep = AUTO_SEND_REQUIRE_EVENT;
			}
			break;
		}

		//发送要料请求信号
	case AUTO_SEND_REQUIRE_EVENT:
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

			if (!m_bRequirePallet  || (MACHINE.m_pCleanBefore != NULL && !MACHINE.m_pCleanBefore->IsModuleEnable()))
			{
				m_nNextAutoStep = AUTO_LIFT_CYL_UP;
			}
			break;
		}

		//顶升气缸上升
	case AUTO_LIFT_CYL_UP:
		{
			m_strMsg = m_bLangulageEng?_T("Lift Cyl Up"):_T("顶升气缸上升");

			if(IsNoHardware() || LiftCylUp(TRUE))
			{
				Sleep(200);
				m_nNextAutoStep = AUTO_CHECK_HAVE_SHELL;
			}
			break;
		}

		//检测有壳
	case AUTO_CHECK_HAVE_SHELL:
		{
			m_strMsg = m_bLangulageEng?_T("Check Have Shell"):_T("检测有壳");

			if (IsNoHardware() || m_bDryRun
				|| (m_IHaveProduct[0] > -1 && CheckInput(INPUTS[m_IHaveProduct[0]],ON)
				&& m_IHaveProduct[1] > -1 && CheckInput(INPUTS[m_IHaveProduct[1]],ON)))
			{
				m_nNextAutoStep = AUTO_SEND_FINGER_CYL_CLOSE_SINGLE;
			}
			break;
		}

		//发送夹紧信号
	case AUTO_SEND_FINGER_CYL_CLOSE_SINGLE:
		{
			m_strMsg = m_bLangulageEng?_T("Send Finger Cyl Close Single"):_T("发送夹紧信号");
			if (!m_bFingerClose)
			{
				m_bFingerClose = TRUE;
				m_nNextAutoStep = AUTO_WAIT_FRONT_MODULE_MOVE_SAFE_POS;
			}
			break;
		}

		//等待清洗前翻转工位移动到安全位置
	case AUTO_WAIT_FRONT_MODULE_MOVE_SAFE_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Front Module Move Safe Pos"):_T("等待清洗前翻转工位移动到安全位置");

			if (!m_bFingerClose || (MACHINE.m_pCleanBefore != NULL && !MACHINE.m_pCleanBefore->IsModuleEnable()))
			{
				m_bShellVaild = TRUE;
				m_nNextAutoStep = AUTO_FINGER_CYL_CLOSE;
			}
			break;
		}
		//夹抓气缸夹紧
	case AUTO_FINGER_CYL_CLOSE:
		{
			m_strMsg = m_bLangulageEng?_T("Finger Cyl Close"):_T("夹抓气缸夹紧");
			if (IsNoHardware() || FingerCylClose(TRUE))
			{
				if (IsNoHardware() || FingerCylClose(FALSE))
				{
					Sleep(200);
					if (IsNoHardware() || FingerCylClose(TRUE))
					{
						if (m_OAirBlow > -1 )
						{
							OUTPUTS[m_OAirBlow]->On();
						}
						if(m_OAirExhaust > -1)
						{
							OUTPUTS[m_OAirExhaust]->On();
						}
						m_nAirTime = GetTickCount();
						m_nNextAutoStep = AUTO_WAIT_AIR_TIME_OUT;
					}
				}

			}
			break;
		}
		//等待吹气完成
	case AUTO_WAIT_AIR_TIME_OUT:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Air Time Out"):_T("等待吹气完成");
			if (0 == m_nAirTime)  //带料启动重新计时
			{
				m_nAirTime = GetTickCount();  
			}

			if(GetTickCount() - m_nAirTime > (DWORD)m_nAirTimelimit)
			{
				if(m_OAirBlow > -1)
				{
					OUTPUTS[m_OAirBlow]->Off();
				}
				if(m_OAirExhaust > -1)
				{
					OUTPUTS[m_OAirExhaust]->Off();
				}
				m_nNextAutoStep = AUTO_SEND_OFFLOAD_REQUIRE;	
			}
			break;
		}

		//发送下料准备好信号
	case AUTO_SEND_OFFLOAD_REQUIRE:
		{
			m_strMsg = m_bLangulageEng?_T("Send Ready Event"):_T("发送下料准备好信号");
			if (!m_bOffloadRequire)
			{
				m_bOffloadRequire = TRUE;
				m_nNextAutoStep = AUTO_WAIT_REQUIRE_RESPONSE;
			}
			break;
		}

		//等待开始信号
	case AUTO_WAIT_REQUIRE_RESPONSE:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Start Event"):_T("等待后翻转开始信号");
			if (!m_bOffloadRequire)
			{
				m_nNextAutoStep = AUTO_FINGER_CYL_OPEN_OFFLOAD;
			}
			break;   
		}

		//夹抓气缸松开
	case AUTO_FINGER_CYL_OPEN_OFFLOAD:
		{
			m_strMsg = m_bLangulageEng?_T("Finger Cyl Open"):_T("后翻转要料，夹抓气缸松开");

			if (IsNoHardware() || FingerCylClose(FALSE))
			{
				m_nNextAutoStep = AUTO_LIFT_CYL_DOWN_OFFLOAD;
			}
			break;
		}

		//顶升气缸回退
	case AUTO_LIFT_CYL_DOWN_OFFLOAD:
		{
			m_strMsg = m_bLangulageEng?_T("Lift Cyl Down"):_T("后翻转要料，顶升气缸下降");

			if(IsNoHardware() || LiftCylUp(FALSE))
			{
				m_nNextAutoStep = AUTO_SEND_LIFT_CYL_DOWN_EVENT;
			}
			break;
		}

		//发送顶升下降信号
	case AUTO_SEND_LIFT_CYL_DOWN_EVENT:
		{
			m_strMsg = m_bLangulageEng?_T("Send Lift Cyl Down Event"):_T("发送顶升下降信号");
			if (!m_bLiftDown)
			{
				m_bLiftDown = TRUE;
				m_nNextAutoStep = AUTO_WAIT_NEXT_MODULE_MOVE_SAFE_POS;
				SaveStepInfo(WRITE_TYPE_AUTO_STEP|WRITE_TYPE_VARIABLES);
			}
			break;
		}

		//等待后翻转安全信号
	case AUTO_WAIT_NEXT_MODULE_MOVE_SAFE_POS:
		{
			m_strMsg = m_bLangulageEng?_T(" Wait Next Module Move Safe Pos"):_T("等待后翻转安全信号");

			if (!m_bLiftDown  || (MACHINE.m_pCleanAfter != NULL && !MACHINE.m_pCleanAfter->IsModuleEnable()))
			{
				m_bShellVaild = FALSE;
				m_nNextAutoStep = AUTO_WORK_END;
			}
			break;
		}

		//工作完成
	case AUTO_WORK_END:
		{
			m_strMsg = m_bLangulageEng?_T("Work end"):_T("工作完成");
			m_nAirTime = 0;
			m_nNextAutoStep = AUTO_CHECK_WITHOUT_SHELL;
			break;
		}
	default:
		{
			ASSERT(0);
			break;
		}
	}
}

BOOL CRunProcessCleanShell::InitializeConfig(CString strProcessModule)
{
	if (!CRunProcess::InitializeConfig(strProcessModule))
	{
		return FALSE;
	}
	return TRUE;
}

void CRunProcessCleanShell::GetHardwareConfig(CArray<int, int> *pInputs, CArray<int, int> *pOutputs, CArray<int, int> *pMotors)
{
	CRunProcess::GetHardwareConfig(pInputs, pOutputs, pMotors);
}

BOOL CRunProcessCleanShell:: CheckOutputCanActive(COutput *pOutput)
{
	return TRUE;
}

BOOL CRunProcessCleanShell::SaveParameter()
{
	if(!IniWriteInt(m_strProcess, _T("AirTimelimit"), 2000, GetAbsolutePathName(PARAMETER_CFG)))
	{
		return FALSE;
	}
	return CRunProcess::SaveParameter();
}

BOOL  CRunProcessCleanShell::ReadRunParameter()
{
	CRunProcess::ReadRunParameter();

	m_nAirTimelimit = IniReadInt(m_strProcess, _T("AirTimelimit"), 2000, GetAbsolutePathName(PARAMETER_CFG));
	return TRUE;
}

/****************************************************************************************
函数描述：夹爪气缸动作
输入参数：bClose:TRUE:夹爪气缸夹紧 FALSE:夹爪气缸松开
函数返回：TRUE:执行成功，FALSE:执行失败
*****************************************************************************************/
BOOL CRunProcessCleanShell:: FingerCylClose(BOOL bClose)
{
	ASSERT(m_IFingerCylOpen[0] > -1 
		&& m_IFingerCylOpen[1] > -1 
		&& m_IFingerCylClose[0] > -1 
		&& m_IFingerCylClose[1] > -1
		&& m_OFingerCylOpen > -1 
		&& m_OFingerCylClose > -1);

	BOOL Res = FALSE;

	bClose?OUTPUTS[m_OFingerCylClose]->On():OUTPUTS[m_OFingerCylClose]->Off();
	bClose?OUTPUTS[m_OFingerCylOpen]->Off():OUTPUTS[m_OFingerCylOpen]->On();

	if (WAIT_OK == WaitInput(INPUTS[m_IFingerCylClose[0]], bClose?ON:OFF, MSG_SHOW_DEFAULT) 
		&& WAIT_OK == WaitInput(INPUTS[m_IFingerCylOpen[0]], bClose?OFF:ON, MSG_SHOW_DEFAULT)
		&& WAIT_OK == WaitInput(INPUTS[m_IFingerCylClose[1]], bClose?ON:OFF, MSG_SHOW_DEFAULT) 
		&& WAIT_OK == WaitInput(INPUTS[m_IFingerCylOpen[1]], bClose?OFF:ON, MSG_SHOW_DEFAULT))
	{
		Res = TRUE;
	}
	return Res;
}

/****************************************************************************************
函数描述：顶升气缸动作
输入参数：bPush:TRUE:顶升气缸推出 FALSE:顶升气缸回退
函数返回：TRUE:执行成功，FALSE:执行失败
*****************************************************************************************/
BOOL CRunProcessCleanShell:: LiftCylUp(BOOL bUp)
{
	ASSERT(m_ILiftCylUp > -1 && m_ILiftCylDown > -1 && m_OLiftCylUp > -1 && m_OLiftCylDown > -1);
	BOOL Res = FALSE;

	bUp?OUTPUTS[m_OLiftCylUp]->On():OUTPUTS[m_OLiftCylUp]->Off();
	bUp?OUTPUTS[m_OLiftCylDown]->Off():OUTPUTS[m_OLiftCylDown]->On();

	if (WAIT_OK == WaitInput(INPUTS[m_ILiftCylUp], bUp?ON:OFF, MSG_SHOW_DEFAULT) 
		&& WAIT_OK == WaitInput(INPUTS[m_ILiftCylDown], bUp?OFF:ON, MSG_SHOW_DEFAULT))
	{
		Res = TRUE;
	}
	return Res;
}

/****************************************************************************************
函数描述：检测是否有壳
输入参数：
函数返回：nRes = 0:没有料 1：检测到一个壳 2;检测到两个壳
*****************************************************************************************/
int CRunProcessCleanShell::JudgeShellNum()
{
	ASSERT(m_IHaveProduct[0] > -1 && m_IHaveProduct[1] > -1);
	int nRes = 0;

	if (INPUTS[m_IHaveProduct[0]]->IsOn())
	{
		nRes++;
	} 
	if (INPUTS[m_IHaveProduct[1]]->IsOn())
	{
		nRes++;
	} 
	return nRes;
}
