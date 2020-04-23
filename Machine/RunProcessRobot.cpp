/****************************************************************************************
文件名：RunProcessRobot.cpp
功能描述：机器人上料工位
历史修改记录：
*****************************************************************************************/
#include "StdAfx.h"
#include "RunProcessRobot.h"

UINT RobotThread(LPVOID	 lpvoid)
{
	CRunProcessRobot* pRun = (CRunProcessRobot*)lpvoid;

	if (pRun != NULL)
	{
		pRun->m_bThreadStart = TRUE;
		pRun->m_bThreadEnd = FALSE;
		while(pRun->m_bThreadStart)
		{
			if(!IsNoHardware() && pRun->IsModuleEnable())
				pRun->EmgStop();
			Sleep(100);
		}
		pRun->m_bThreadEnd = TRUE;
	}
	return 0;
}


CRunProcessRobot::CRunProcessRobot(int nID) : CRunProcess(nID)
{
	CString strKey;

	PowerUpRestart();

	for(int i = 0; i < 2; i++)
	{
		//夹爪
		strKey.Format(_T("IFingerCylOpen%d"), i);
		m_InputMap.insert(pair<int*, CString>(&m_IFingerCylOpen[i], strKey));

		strKey.Format(_T("IFingerCylClose%d"), i);
		m_InputMap.insert(pair<int*, CString>(&m_IFingerCylClose[i], strKey));

		strKey.Format(_T("OFingerCylClose%d"), i);
		m_OutputMap.insert(pair<int*, CString>(&m_OFingerCylClose[i], strKey));

		strKey.Format(_T("OFingerCylOpen%d"), i);
		m_OutputMap.insert(pair<int*, CString>(&m_OFingerCylOpen[i], strKey));

		strKey.Format(_T("IHasCore%d"), i);//夹爪有料检测
		m_InputMap.insert(pair<int*, CString>(&m_IHasCore[i], strKey));

		strKey.Format(_T("IHasCore%d"), i + 2);//缓存有料检测
		m_InputMap.insert(pair<int*, CString>(&m_IHasCore[i + 2], strKey));

		strKey.Format(_T("ITailFingerSignal%d"), i);
		m_InputMap.insert(pair<int*, CString>(&m_ITailFingerSignal[i], strKey));

		strKey.Format(_T("ITailSafeSignal%d"), i);
		m_InputMap.insert(pair<int*, CString>(&m_ITailSafeSignal[i], strKey));

		strKey.Format(_T("ORobotPutFinishSignal%d"), i);
		m_OutputMap.insert(pair<int*, CString>(&m_ORobotPutFinishSignal[i], strKey));

		strKey.Format(_T("ORobotPutInPosSignal%d"), i);
		m_OutputMap.insert(pair<int*, CString>(& m_ORobotPutInPosSignal[i], strKey));
	}
	m_InputMap.insert(pair<int*, CString>(&m_IZCheckAbnormal, _T("IZCheckAbnormal")));
	//缓存
	m_InputMap.insert(pair<int*, CString>(&m_IBufferCylPush,_T("IBufferCylPush")));
	m_InputMap.insert(pair<int*, CString>(&m_IBufferCylReturn,_T("IBufferCylReturn")));
	m_OutputMap.insert(pair<int*, CString>(&m_OBufferCylPush,_T("OBufferCylPush")));
	m_OutputMap.insert(pair<int*, CString>(&m_OBufferCylReturn, _T("OBufferCylReturn")));
	//机器人
	m_InputMap.insert(pair<int*, CString>(&m_IRobotAlarm, _T("IRobotAlarm")));
	m_InputMap.insert(pair<int*, CString>(&m_IRobotRun, _T("IRobotRun")));
	m_InputMap.insert(pair<int*, CString>(&m_IRobotReset, _T("IRobotReset")));
	m_InputMap.insert(pair<int*, CString>(&m_IRobotEnable, _T("IRobotEnable")));

	m_OutputMap.insert(pair<int*, CString>(&m_ORobotEnable, _T("ORobotEnable")));
	m_OutputMap.insert(pair<int*, CString>(&m_ORobotStop, _T("ORobotStop")));
	m_OutputMap.insert(pair<int*, CString>(&m_ORobotCodeRun, _T("ORobotCodeRun")));
	m_OutputMap.insert(pair<int*, CString>(&m_ORobotAlarmReset, _T("ORobotAlarmReset")));
	m_OutputMap.insert(pair<int*, CString>(&m_ORobotCodeReset, _T("ORobotCodeReset")));
	m_OutputMap.insert(pair<int*, CString>(&m_ORobotEmgStop, _T("ORobotEmgStop")));

	m_InputMap.insert(pair<int*, CString>(&m_INormalRunSingle, _T("INormalRunSingle")));
	m_OutputMap.insert(pair<int*, CString>(&m_ONormalRunSingle, _T("ONormalRunSingle")));

	m_VoidMap.insert(pair<int*, CString>(&m_bEnableRobot, _T("bEnableRobot, 启动机器人")));
	m_VoidMap.insert(pair<void*,CString>(&m_nRobotPort,_T("lRobotPort, 机器人连接端口")));
	m_VoidMap.insert(pair<void*,CString>(&m_strRobotIp,_T("sRobotIp, 机器人连接IP")));

	m_VoidMap.insert(pair<int*, CString>(&m_bShieldServer, _T("b启用客户端, True为启用，False为不启用")));
	m_VoidMap.insert(pair<void*,CString>(&m_nServerPort,_T("l服务器端口, 连接端口")));
	m_VoidMap.insert(pair<void*,CString>(&m_strServerIp,_T("s服务器端IP, 连接IP")));
}

CRunProcessRobot::~CRunProcessRobot(void)
{
	MACHINE.MachineExit = TRUE;
	m_bThreadStart = FALSE;
	DWORD dwTime = GetTickCount();

	while(!m_bThreadEnd)
	{
		if (GetTickCount() - dwTime > 10000)
		{
			break;
		}
		Sleep(1);
	}
	m_Client.Disconnect();

}

void CRunProcessRobot::PowerUpRestart()
{
	CRunProcess::PowerUpRestart();
	m_strMsg = m_bLangulageEng?_T("Ready"):_T("准备好");

	m_nCurRobotLoc = ROBOT_GO_HOME;
	m_unRobotSpeed = ROBOT_LOW_SPEED;
	m_nWorkIndex = -1;
	m_nStartWithPalletData = -1;
	m_bRobotWaitPos = FALSE;
	m_bRobotSafe = FALSE;
	m_bBufferMes = FALSE;
	m_bEnableDocking = TRUE;
	m_pBufferPallet.Release();
	m_pTempPallet[0].Release();
	m_pTempPallet[1].Release();

	m_nNextPowerUpStep = PU_ROBOT_START;
	m_nNextAutoStep = AUTO_IF_CLEAR_MATERIAL;
}

void CRunProcessRobot::PowerUpOperation(void)
{
	if (!IsModuleEnable())
	{
		CRun::PowerUpOperation();
		m_strMsg = m_bLangulageEng?_T("Moudle UnEnable"):_T("模块禁用");		
		return;
	}

	switch(m_nNextPowerUpStep)
	{
		//启动机器人
	case PU_ROBOT_START:
		{
			m_strMsg = m_bLangulageEng?_T("Start Robot Run"):_T("启动机器人，同时给段发送不安全信号");
			if (IsNoHardware() || RobotStop())
			{
				SendSafeSignal(FALSE);
				m_nNextPowerUpStep = PU_ROBOT_ALARM_RESET;
			}
			break;
		}
		//机器人报警复位
	case PU_ROBOT_ALARM_RESET:
		{
			m_strMsg = m_bLangulageEng?_T("Reset Robot Alarm"):_T("机器人报警复位");

			if (IsNoHardware() || RobotAlarmReset())
			{
				m_nNextPowerUpStep = PU_ROBOT_ALARM_CHECK;
			}
			break;
		}
		//检测机器人报警信号
	case PU_ROBOT_ALARM_CHECK:
		{
			m_strMsg = m_bLangulageEng?_T("Check Robot Alarm Signal"):_T("检测机器人报警信号");

			if (IsNoHardware() || (m_IRobotAlarm > -1 && CheckInput(INPUTS[m_IRobotAlarm],OFF)))
			{
				m_nNextPowerUpStep = PU_ROBOT_CODE_RESET_AND_RUN;
			}
			else
			{
				m_nNextPowerUpStep =PU_ROBOT_ALARM_RESET;
			}
			break;
		}
		//启动机器人程序
	case PU_ROBOT_CODE_RESET_AND_RUN:
		{
			m_strMsg = m_bLangulageEng?_T("Start Robot Run"):_T("启动机器人");

			if (IsNoHardware() || RobotCodeReset())
			{
				if (IsNoHardware() || RobotCodeRun())
				{
					m_nNextPowerUpStep = PU_ROBOT_CONNET;
				}
			}
			break;
		}
		//连接机器人
	case PU_ROBOT_CONNET:
		{
			m_strMsg = m_bLangulageEng?_T("Connect Robot"):_T("连接机器人");

			if (IsNoHardware() || !m_bEnableRobot || m_pRobotClient.Connect(m_strRobotIp, m_nRobotPort))
			{
				m_nNextPowerUpStep = PU_JUDGE_IF_WITH_PALLET;
			}
			break;
		}
		//判断是否带料
	case PU_JUDGE_IF_WITH_PALLET:
		{
			m_strMsg = m_bLangulageEng?_T("Check Have Product"):_T("判断是否带料");

			if(MACHINE.m_bReCoverMachine)
			{
				ReadStepInfo();
				m_nNextPowerUpStep = PU_FINGER_CYL_CLOSE_WITH_PALLET;
			}
			else
			{
				m_nNextPowerUpStep = PU_CHECK_WITHOUT_PRODUCT;
			}
			break;
		}
		//检测无产品
	case PU_CHECK_WITHOUT_PRODUCT:
		{
			m_strMsg = m_bLangulageEng?_T("Check Without Product"):_T("检测无产品");

			if (IsNoHardware() || CheckHasVaild(-1, FALSE))
			{
				m_nNextPowerUpStep = PU_FINGER_CYL_OPEN;		 							 
			}
			break;
		}
		//夹爪打开
	case PU_FINGER_CYL_OPEN:
		{
			m_strMsg = m_bLangulageEng?_T("FingerCyl Cyl Open"):_T("夹爪打开");

			if (IsNoHardware() || FingerCylClose(-1, FALSE))
			{
				m_nNextPowerUpStep = PU_ROBOT_MOVE_TO_SAFE_POS;
			}
			break;
		}
		//复位机器人
		m_strMsg = m_bLangulageEng?_T("Reset Robot"):_T("机器人到安全位");
	case PU_ROBOT_MOVE_TO_SAFE_POS:
		{

			if (IsNoHardware() || !m_bEnableRobot || m_pRobotClient.SendToDeviceAndWait(ROBOT_GO_HOME))
			{
				if(m_bEnableRobot)
					m_nCurRobotLoc = ROBOT_GO_HOME;
				m_bRobotSafe = TRUE;
				if(IsNoHardware())
				{
					m_nNextPowerUpStep = PU_BUFFER_CYL_BACK;
					break;
				}
				SendSafeSignal(TRUE); //给后段发安全信号

				m_nNextPowerUpStep = PU_BUFFER_CYL_BACK;
			}
			else
			{
				//机器人移动超时
				ShowMessage(MSG_ROBOT_MOVE_TIMEOUT);
			}
			break;
		}
		//缓存气缸回退
	case PU_BUFFER_CYL_BACK:
		{
			m_strMsg = m_bLangulageEng?_T("Reserve Cyl Back"):_T("缓存气缸回退");
			if(IsNoHardware() || BufferCylPush(FALSE))
			{
				//清除数据
				m_nWorkIndex = -1;
				m_pTempPallet[0].Release();
				m_pTempPallet[1].Release();
				m_pBufferPallet.Release();

				m_nNextPowerUpStep = PU_JOINT_SIGNAL_RESET;
				m_nNextAutoStep = AUTO_IF_CLEAR_MATERIAL;
			}
			break;
		}

		//带料启动，夹爪加紧或松开  
		//防治夹爪未加紧，在检测料盘数据一致时,改变料盘数据，导致其他数据丢失
	case PU_FINGER_CYL_CLOSE_WITH_PALLET:
		{
			m_strMsg = m_bLangulageEng?_T("Finger Cyl Open"):_T("带料启动，夹爪加紧或松开");

			if (m_pTempPallet[0].m_bIsValid || m_pTempPallet[1].m_bIsValid)// 已经取料 只在刚取料时控制夹爪动作，其他直接复位机器人
			{
				m_nNextPowerUpStep = PU_ROBOT_MOVE_TO_SAFE_POS_WITH_PALLET;

				int nAutoStep = m_nNextAutoStep;
				if (AUTO_FINGER_CYL_CLOSE < nAutoStep && nAutoStep <= AUTO_SEND_PICK_BATTERY_FINISH)  //刚取料
				{
					if(::IsNoHardware() || FingerCylClose(-1, TRUE))
					{
						m_nNextPowerUpStep = PU_WAIT_FRONT_MODULE_SAFE;
					}
				}
			}
			else
			{
				if(::IsNoHardware() || FingerCylClose(-1, FALSE))
				{
					m_nNextPowerUpStep = PU_ROBOT_MOVE_TO_SAFE_POS_WITH_PALLET;
				}
			}
			break;
		}

		//控制取电池夹爪Y轴到安全位
	case PU_WAIT_FRONT_MODULE_SAFE:
		{
			m_strMsg = m_bLangulageEng?_T("Reset Robot"):_T("控制取电池夹爪Y轴到安全位");

			if (MACHINE.m_pBatteryOffloadPnp[0] != NULL && MACHINE.m_pBatteryOffloadPnp[0]->FingerCylClose(FALSE) 
				&& MACHINE.m_pBatteryOffloadPnp[1] != NULL  && MACHINE.m_pBatteryOffloadPnp[1]->FingerCylClose(FALSE))
			{
				int nMotor[2] = {MACHINE.m_pBatteryOffloadPnp[0]->m_nMotorX, MACHINE.m_pBatteryOffloadPnp[1]->m_nMotorX};
				if(MotorsHome(nMotor, 2))
				{
					m_nNextPowerUpStep = PU_ROBOT_MOVE_TO_SAFE_POS_WITH_PALLET;
				}
			}
			break;
		}

		//复位机器人
	case PU_ROBOT_MOVE_TO_SAFE_POS_WITH_PALLET:
		{
			m_strMsg = m_bLangulageEng?_T("Reset Robot"):_T("机器人到安全位");

			if (IsNoHardware() || !m_bEnableRobot || m_pRobotClient.SendToDeviceAndWait(ROBOT_GO_HOME))
			{
				if(m_bEnableRobot)
				{
					m_nCurRobotLoc = ROBOT_GO_HOME;
				}
				if (!m_bRobotSafe)
				{
					m_bRobotSafe = TRUE;
					m_nNextPowerUpStep = PU_CHECK_BUFFER_PALLET_DATA;
				}
			}
			else
			{
				ShowMessage(MSG_ROBOT_MOVE_TIMEOUT);
			}
			break;
		}

		//检测缓存数据与实际是否一致
	case PU_CHECK_BUFFER_PALLET_DATA:
		{
			m_strMsg = m_bLangulageEng?_T("Move XY To Onload Pos"):_T("检测缓存数据与实际是否一致");
			if (CheckStartWithPalletDataSameForBuffer())
			{ 
				if (m_nBufferWithPalletData ==0)
				{
					m_pBufferPallet.Release();
				}
				m_nNextAutoStep = PU_CHECK_PALLET_DATA;
			}
			else
			{
				m_nNextAutoStep = PU_CHECK_BUFFER_PALLET_DATA;
			}
		}

		//检测料盘数据与实际是否一致
	case PU_CHECK_PALLET_DATA:
		{
			m_strMsg = m_bLangulageEng?_T("Move XY To Onload Pos"):_T("检测料盘数据与实际是否一致");
			if(CheckStartWithPalletDataSame())
			{
				if(m_nStartWithPalletData == 0)
				{
					for(int i = 0; i < 2; i++)
					{
						m_pTempPallet[i].Release();
					}
					m_nWorkIndex = -1;
					m_nNextAutoStep = AUTO_IF_CLEAR_MATERIAL;
					m_nNextPowerUpStep = PU_JOINT_SIGNAL_RESET;
				}
				else if(m_nStartWithPalletData == 1)//12夹爪有料
				{
					m_nNextAutoStep = AUTO_JUDGE_HAVE_NG;
					m_nNextPowerUpStep = PU_JUDGE_PRESS_DATE_EXIST;
				}
				else if(m_nStartWithPalletData == 2)  //1夹爪有料
				{
					m_nNextAutoStep = AUTO_JUDGE_HAVE_NG;
					m_nNextPowerUpStep = PU_JUDGE_PRESS_DATE_EXIST;
				}
				else if(m_nStartWithPalletData == 3)//2夹爪有料
				{
					m_nNextAutoStep = AUTO_JUDGE_HAVE_NG;
					m_nNextPowerUpStep = PU_JUDGE_PRESS_DATE_EXIST;
				}
			}
			break;
		}

		//判断压力数据是否存在  防止电池无数据确标记位良品
	case PU_JUDGE_PRESS_DATE_EXIST:
		{
			m_strMsg = m_bLangulageEng?_T("Judge Press Date Exist"):_T("判断压力数据是否存在");

			if (m_pTempPallet[0].m_bIsValid 
				&& (m_pTempPallet[0].m_dPressNormValue[0] ==0 || m_pTempPallet[0].m_dPressNormValue[1] == 0 
				|| m_pTempPallet[0].m_strBarCode.GetLength() == 0)) 
			{
				m_pTempPallet[0].m_bIsNg = TRUE;
			}
			if (m_pTempPallet[1].m_bIsValid
				&& (m_pTempPallet[1].m_dPressNormValue[0] == 0 || m_pTempPallet[1].m_dPressNormValue[1] == 0 
				|| m_pTempPallet[1].m_strBarCode.GetLength() == 0))
			{
				m_pTempPallet[1].m_bIsNg = TRUE;
			}

			if (m_pBufferPallet.m_bIsValid && (m_pBufferPallet.m_dPressNormValue[0] == 0 || m_pBufferPallet.m_strBarCode.GetLength() == 0))
			{
				m_pBufferPallet.m_bIsNg = TRUE;
			}

			m_nNextPowerUpStep = PU_JOINT_SIGNAL_RESET;
			break;
		}

		//复位与后段对接信号
	case PU_JOINT_SIGNAL_RESET:
		{
			m_strMsg = m_bLangulageEng?_T("Reset Signal"):_T("复位放料信号");

			if(IsNoHardware() || MACHINE.m_bUnEnableDocking || SendPlaceFinishSignal(FALSE))
			{
				m_nNextPowerUpStep = PU_CONNECT_SERVER;
			}
			break;
		}

	case PU_CONNECT_SERVER:
		{
			m_strMsg = m_bLangulageEng?_T("Connect Server"):_T("连接后段服务器");
			
			if (!m_bShieldServer || m_Client.Connect(m_strServerIp,m_nServerPort))
			{
				m_nNextPowerUpStep = PU_FINISH;
			}
			else
			{
				ShowMessage(_T("连接后段服务器失败，请检查后段是否初始化！"),3);
			}
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

void CRunProcessRobot::AutoOperation()
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
		if (m_nSaveLastStep == AUTO_IF_CLEAR_MATERIAL)
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
	case AUTO_IF_CLEAR_MATERIAL:
		{
			m_strMsg = m_bLangulageEng?_T("Send Front Module Move Motor"):_T("判断是否清料");

			if (MACHINE.m_bClearMaterial)
			{
				int nCoreNum =MACHINE.CoreCountForClear(TRUE);
				if (nCoreNum <= 1)
				{
					//清料完成
					ShowMessage(CLEAR_MATERIAL_FINISH);
					MACHINE.m_bClearMaterial = FALSE;
					MACHINE.m_pRunCtrl->Stop();
				    Sleep(100);
					break;
				}
				else
				{
					m_nNextAutoStep = AUTO_ROBOT_TO_PICK_WAIT_POS_INIT;
					SaveStepInfo(WRITE_TYPE_AUTO_STEP);
				}
			}
			else
			{
				m_nNextAutoStep = AUTO_ROBOT_TO_PICK_WAIT_POS_INIT;
				SaveStepInfo(WRITE_TYPE_AUTO_STEP);
			}
			break;
		}

		//机器人到取料等待位
	case AUTO_ROBOT_TO_PICK_WAIT_POS_INIT:
		{
			m_strMsg = m_bLangulageEng?_T("Robot To Pick Wait Pos"):_T("机器人到取料等待位");

			if (IsNoHardware() || !m_bEnableRobot || m_pRobotClient.SendToDeviceAndWait(ROBOT_ONLOAD_WAIT))
			{
				if(m_bEnableRobot)
				{
					m_nCurRobotLoc = ROBOT_ONLOAD_WAIT;
				}
				m_nNextAutoStep = AUTO_FINER_CLY_OPEN_INIT;
			}
			else
			{
				//机器人移动超时
				ShowMessage(MSG_ROBOT_MOVE_TIMEOUT);
			}
			break;
		}

		//夹爪气缸张开
	case AUTO_FINER_CLY_OPEN_INIT:       
		{
			m_strMsg = m_bLangulageEng?_T("FingerCyl Cly Open"):_T("夹爪气缸张开");

			if (IsNoHardware() || FingerCylClose(-1, FALSE))
			{
				m_nNextAutoStep = AUTO_WAIT_START_EVENT_ONLOAD;
				SaveStepInfo(WRITE_TYPE_AUTO_STEP);
			}
			break;
		}

		//onload
		//等待取电池夹爪下料信号
	case AUTO_WAIT_START_EVENT_ONLOAD:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Start Event"):_T("等待取电池夹爪下料信号");

			if (MACHINE.m_pBatteryOffloadPnp[0] != NULL && MACHINE.m_pBatteryOffloadPnp[1] != NULL 
				&& (MACHINE.m_pBatteryOffloadPnp[0]->m_bOffloadToRobot || !MACHINE.m_pBatteryOffloadPnp[0]->IsModuleEnable())
				&& (MACHINE.m_pBatteryOffloadPnp[1]->m_bOffloadToRobot || !MACHINE.m_pBatteryOffloadPnp[1]->IsModuleEnable()))
			{
				m_bRobotWaitPos = FALSE;
				m_nNextAutoStep = AUTO_ROBOT_TO_PICK_POS_ONLOAD;
				SaveStepInfo(WRITE_TYPE_AUTO_STEP);
			}
			else
			{
				if (!m_bRobotWaitPos)
				{
					m_nNextAutoStep = AUTO_ROBOT_TO_PICK_WAIT_POS_ONLOAD;
				}
			}
			break;
		}

		//机器人到取料等待位
	case AUTO_ROBOT_TO_PICK_WAIT_POS_ONLOAD:
		{
			m_strMsg = m_bLangulageEng?_T("Robot To Pick Wait Pos"):_T("机器人到取料等待位");

			if (IsNoHardware() || !m_bEnableRobot || m_pRobotClient.SendToDeviceAndWait(ROBOT_ONLOAD_WAIT))
			{
				if(m_bEnableRobot)
				{
					m_nCurRobotLoc = ROBOT_ONLOAD_WAIT;
				}

				if (!m_bRobotWaitPos)
				{
					m_bRobotWaitPos = TRUE;
					m_nNextAutoStep = AUTO_WAIT_START_EVENT_ONLOAD;
				}
			}
			else
			{
				//机器人移动超时
				ShowMessage(MSG_ROBOT_MOVE_TIMEOUT);
			}
			break;
		}

		//机器人到取料位
	case AUTO_ROBOT_TO_PICK_POS_ONLOAD:
		{
			m_strMsg = m_bLangulageEng?_T("Robot To Pick Pos"):_T("机器人到取料位");
			if (m_bRobotSafe)
			{
				if (IsNoHardware() || !m_bEnableRobot || m_pRobotClient.SendToDeviceAndWait(ROBOT_ONLOAD_POS))
				{
					if(m_bEnableRobot)
					{
						m_nCurRobotLoc = ROBOT_ONLOAD_POS;
					}
					m_bRobotSafe = FALSE;
					m_nNextAutoStep = AUTO_FINGER_CYL_CLOSE;
				}
				else
				{
					//机器人移动超时 MSG_ROBOT_MOVE_TIMEOUT 2241
					ShowMessage(MSG_ROBOT_MOVE_TIMEOUT);
				}
			}
			break;
		}

		//夹爪气缸夹紧
	case AUTO_FINGER_CYL_CLOSE:       
		{
			m_strMsg = m_bLangulageEng?_T("FingerCyl Cly Close"):_T("夹爪气缸夹紧");

			if (IsNoHardware() || FingerCylClose(-1, TRUE))
			{
				//取电池夹爪传递数据给机器人
				if(MACHINE.m_pBatteryOffloadPnp[0] != NULL && MACHINE.m_pBatteryOffloadPnp[1] != NULL)
				{
					m_pTempPallet[0] = MACHINE.m_pBatteryOffloadPnp[0]->m_Pallet;
					m_pTempPallet[1] = MACHINE.m_pBatteryOffloadPnp[1]->m_Pallet;

					MACHINE.m_pBatteryOffloadPnp[0]->m_Pallet.Release();
					MACHINE.m_pBatteryOffloadPnp[1]->m_Pallet.Release();

					//控制取电池夹爪松开,电机后退
					m_nNextAutoStep = AUTO_SEND_PICK_FINISH;

					SaveStepInfo(WRITE_TYPE_AUTO_STEP|WRITE_TYPE_BATTERY);

					MACHINE.m_pBatteryOffloadPnp[0]->SaveStepInfo(WRITE_TYPE_AUTO_STEP|WRITE_TYPE_BATTERY);
					MACHINE.m_pBatteryOffloadPnp[1]->SaveStepInfo(WRITE_TYPE_AUTO_STEP|WRITE_TYPE_BATTERY);
				}
			}
			break;
		}

		/*发送取料料完成*/
	case AUTO_SEND_PICK_FINISH:
		{
			m_strMsg = m_bLangulageEng?_T("Send Onload Finish"):_T("发送取料料完成");

			if (MACHINE.m_pBatteryOffloadPnp[0] != NULL && MACHINE.m_pBatteryOffloadPnp[1] != NULL 
				&& (MACHINE.m_pBatteryOffloadPnp[0]->m_bOffloadToRobot || !MACHINE.m_pBatteryOffloadPnp[0]->IsModuleEnable())
				&& (MACHINE.m_pBatteryOffloadPnp[1]->m_bOffloadToRobot || !MACHINE.m_pBatteryOffloadPnp[1]->IsModuleEnable()))
			{
				MACHINE.m_pBatteryOffloadPnp[0]->m_bOffloadToRobot = FALSE;
				MACHINE.m_pBatteryOffloadPnp[1]->m_bOffloadToRobot = FALSE;
				m_nNextAutoStep = AUTO_WAIT_BATTERY_OFFLOAD_PNP_AVOID;

				MACHINE.m_pBatteryOffloadPnp[0]->SaveStepInfo(WRITE_TYPE_AUTO_STEP|WRITE_TYPE_VARIABLES);
				MACHINE.m_pBatteryOffloadPnp[1]->SaveStepInfo(WRITE_TYPE_AUTO_STEP|WRITE_TYPE_VARIABLES);
			}
			break;
		}
	case AUTO_WAIT_BATTERY_OFFLOAD_PNP_AVOID:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Offload Pnp Avoid"):_T("等待取电池夹爪避让");
			if (MACHINE.m_pBatteryOffloadPnp[0] != NULL && MACHINE.m_pBatteryOffloadPnp[1] != NULL 
				&& (MACHINE.m_pBatteryOffloadPnp[0]->m_bOpenCylOpen || !MACHINE.m_pBatteryOffloadPnp[0]->IsModuleEnable())
				&& (MACHINE.m_pBatteryOffloadPnp[1]->m_bOpenCylOpen || !MACHINE.m_pBatteryOffloadPnp[1]->IsModuleEnable()))
			{
				m_nNextAutoStep = AUTO_WAIT_MES_SINGLE;
				SaveStepInfo(WRITE_TYPE_AUTO_STEP);
			}
			break;
		}

		//等待MES信号
	case AUTO_WAIT_MES_SINGLE:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Mes Single"):_T("等待MES信号1");
			if((MACHINE.m_pUploadMes != NULL && (MACHINE.m_pUploadMes->m_bNextRequire || !MACHINE.m_pUploadMes->IsModuleEnable())))
			{
				MACHINE.m_pUploadMes->m_Pallet = m_pTempPallet[0];
				MACHINE.m_pUploadMes->m_bNextRequire = FALSE;
				m_nNextAutoStep = AUTO_ROBOT_TO_PICK_WAIT_POS_ONLOAD_CHECK;
			}
			break;
		}

		//取料完成，机器人到取料等待位
	case AUTO_ROBOT_TO_PICK_WAIT_POS_ONLOAD_CHECK:
		{
			m_strMsg = m_bLangulageEng?_T("Robot Move To Pick Wait Pos"):_T("取料完成，机器人到取料等待位");

			if (IsNoHardware() ||  !m_bEnableRobot || m_pRobotClient.SendToDeviceAndWait(ROBOT_ONLOAD_WAIT))
			{
				if(m_bEnableRobot)
				{
					m_nCurRobotLoc = ROBOT_ONLOAD_WAIT;
				}

				if (!m_bRobotSafe)
				{
					m_bRobotSafe = TRUE;
					m_nNextAutoStep = AUTO_SEND_PICK_BATTERY_FINISH;
					SaveStepInfo(WRITE_TYPE_AUTO_STEP);
				}
			}
			else
			{
				//机器人移动超时
				ShowMessage(MSG_ROBOT_MOVE_TIMEOUT);
			}
			break;
		}
	case AUTO_SEND_PICK_BATTERY_FINISH:
		{
			m_strMsg = m_bLangulageEng?_T("Check Have Product"):_T("重置取电池夹爪避让信号");

			if (MACHINE.m_pBatteryOffloadPnp[0] != NULL && MACHINE.m_pBatteryOffloadPnp[1] != NULL 
				&& (MACHINE.m_pBatteryOffloadPnp[0]->m_bOpenCylOpen || !MACHINE.m_pBatteryOffloadPnp[0]->IsModuleEnable())
				&& (MACHINE.m_pBatteryOffloadPnp[1]->m_bOpenCylOpen || !MACHINE.m_pBatteryOffloadPnp[1]->IsModuleEnable()))
			{
				MACHINE.m_pBatteryOffloadPnp[0]->m_bOpenCylOpen = FALSE;
				MACHINE.m_pBatteryOffloadPnp[1]->m_bOpenCylOpen = FALSE;
				m_nNextAutoStep = AUTO_CHECK_HAVE_PRODUCT;

				MACHINE.m_pBatteryOffloadPnp[0]->SaveStepInfo(WRITE_TYPE_AUTO_STEP|WRITE_TYPE_VARIABLES);
				MACHINE.m_pBatteryOffloadPnp[1]->SaveStepInfo(WRITE_TYPE_AUTO_STEP|WRITE_TYPE_VARIABLES);
			}
			break;
		}
		//检测有料
	case AUTO_CHECK_HAVE_PRODUCT:
		{
			m_strMsg = m_bLangulageEng?_T("Check Have Product"):_T("取料完成，检测有料");

			if (IsNoHardware() || m_bDryRun || (CheckHasVaild(0, TRUE) && CheckHasVaild(1, TRUE)))                         
			{
				//m_nWorkIndex = -1;    //测试不良？
				/*	m_pTempPallet[0].m_bIsNg =TRUE;
				m_pTempPallet[1].m_bIsNg=TRUE;
				m_pTempPallet[0].m_bIsValid = TRUE;
				m_pTempPallet[1].m_bIsValid = TRUE;*/
				m_nNextAutoStep = AUTO_WAIT_MES_SINGLE_FOR_OFFLOAD;
			}
			break;
		}

		//等待MES信号2
	case AUTO_WAIT_MES_SINGLE_FOR_OFFLOAD:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Mes Single"):_T("等待MES信号2");
			if ((MACHINE.m_pUploadMes != NULL && (MACHINE.m_pUploadMes->m_bNextRequire || !MACHINE.m_pUploadMes->IsModuleEnable())))
			{	
				MACHINE.m_pUploadMes->m_Pallet = m_pTempPallet[1];
				MACHINE.m_pUploadMes->m_bNextRequire = FALSE;
				m_nNextAutoStep = AUTO_JUDGE_HAVE_NG;
			}
			break;
		}


		//判断有无NG
	case AUTO_JUDGE_HAVE_NG:
		{
			m_strMsg = m_bLangulageEng?_T("Judge Have Ng"):_T("判断有无NG");
			if(m_bDryRun)
			{
				m_nWorkIndex = -1;
				m_nNextAutoStep = AUTO_WAIT_START_EVENT_OFFLOAD;
				break;
			}


			if(m_pTempPallet[0].m_bIsValid && m_pTempPallet[1].m_bIsValid)
			{
				if(  !MACHINE.m_pPressNgLine->IsModuleEnable() || !m_pTempPallet[0].m_bIsNg && !m_pTempPallet[1].m_bIsNg)  //压力不良工位不开时直接良品下料
				{
					m_nWorkIndex = -1;
					m_nNextAutoStep = AUTO_WAIT_START_EVENT_OFFLOAD;
				}
				else
				{
					if(m_pTempPallet[0].m_bIsNg && m_pTempPallet[1].m_bIsNg)
						m_nWorkIndex = -1;
					else if(m_pTempPallet[0].m_bIsNg)
						m_nWorkIndex = 0;
					else if(m_pTempPallet[1].m_bIsNg)
						m_nWorkIndex = 1;
					m_nNextAutoStep = AUTO_WAIT_START_EVENT_NG;

				}
			}
			else if(m_pTempPallet[0].m_bIsValid || m_pTempPallet[1].m_bIsValid)
			{
				if(m_pTempPallet[0].m_bIsNg || m_pTempPallet[1].m_bIsNg)
				{
					if(m_pTempPallet[0].m_bIsNg)
						m_nWorkIndex = 0;
					else
						m_nWorkIndex = 1;
					m_nNextAutoStep = AUTO_WAIT_START_EVENT_NG;
				}
				else
				{
					if(!m_pBufferPallet.m_bIsValid)
					{// 放暂存
						if(m_pTempPallet[0].m_bIsValid)
							m_nWorkIndex = 0;
						else
							m_nWorkIndex = 1;
						m_nNextAutoStep = AUTO_ROBOT_TO_BUF_WAIT_POS;
					}
					else
					{// 取暂存
						if(!m_pTempPallet[0].m_bIsValid)
							m_nWorkIndex = 0;
						else
							m_nWorkIndex = 1;
						m_nNextAutoStep = AUTO_ROBOT_TO_BUF_WAIT_POS;
					}
				}	
			}
			else
			{
				if(m_bDryRun)
				{
					m_nWorkIndex = -1;
					m_nNextAutoStep = AUTO_WAIT_START_EVENT_OFFLOAD;
				}
				else
				{
					m_nNextAutoStep = AUTO_WORK_END;
				}
				
			}
			break;
		}

		//offload
		//等待后段要料信号
	case AUTO_WAIT_START_EVENT_OFFLOAD:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Next Module Require"):_T("等待后段要料信号");
			if(MACHINE.m_bUnEnableDocking || CheckNeedBatteryEvent(TRUE) && SendSafeSignal(FALSE)) //后段要料的同时打开不安全信号
			{
				m_bRobotWaitPos = FALSE;
				m_nNextAutoStep = AUTO_ROBOT_TO_PUT_POS_OFFLOAD;
			}
			else
			{
				if (!m_bRobotWaitPos)
				{
					m_nNextAutoStep = AUTO_ROBOT_TO_PUT_WAIT_POS_OFFLOAD;
				}
			}
			break;
		}
		//机器人到放料等待位
	case AUTO_ROBOT_TO_PUT_WAIT_POS_OFFLOAD:
		{
			m_strMsg = m_bLangulageEng?_T("Robot To Put Wait Pos"):_T("机器人到放料等待位");
			if (IsNoHardware() ||  !m_bEnableRobot || m_pRobotClient.SendToDeviceAndWait(ROBOT_OFFLOAD_WAIT_POS))
			{
				if(m_bEnableRobot)
				{
					m_nCurRobotLoc = ROBOT_OFFLOAD_WAIT_POS;
				}

				if (!m_bRobotWaitPos)
				{
					m_bRobotWaitPos = TRUE;
					m_nNextAutoStep = AUTO_WAIT_START_EVENT_OFFLOAD;
				}
			}
			else
			{
				//机器人移动超时
				ShowMessage(MSG_ROBOT_MOVE_TIMEOUT);
			}
			break;
		}

		//机器人到放料位
	case AUTO_ROBOT_TO_PUT_POS_OFFLOAD:
		{
			m_strMsg = m_bLangulageEng?_T("Robot To Put Pos"):_T("机器人到放料位");

			if (IsNoHardware() ||  !m_bEnableRobot || m_pRobotClient.SendToDeviceAndWait(ROBOT_OFFLOAD_POS))
			{
				if(m_bEnableRobot)
				{
					m_nCurRobotLoc = ROBOT_OFFLOAD_POS;
				}

				m_nNextAutoStep = AUTO_FINGER_CYL_OPEN_OFFLOAD;
			}
			else
			{
				//机器人移动超时
				ShowMessage(MSG_ROBOT_MOVE_TIMEOUT);
			}
			break;
		}

	//	//等待MES信号
	//case AUTO_WAIT_MES_SINGLE_FOR_OFFLOAD:
	//	{
	//		m_strMsg = m_bLangulageEng?_T("Wait Mes Single"):_T("等待MES信号");
	//		if(m_bBufferMes  || (MACHINE.m_pUploadMes != NULL && (MACHINE.m_pUploadMes->m_bNextRequire || !MACHINE.m_pUploadMes->IsModuleEnable())))
	//		{
	//			if (!m_bBufferMes)
	//			{
	//				MACHINE.m_pUploadMes->m_Pallet = m_pTempPallet[1];
	//				MACHINE.m_pUploadMes->m_bNextRequire = FALSE;
	//			}
	//			m_bBufferMes = FALSE;
	//			m_nNextAutoStep = AUTO_FINGER_CYL_OPEN_OFFLOAD;
	//		}
	//		break;
	//	}

		//夹爪打开
	case AUTO_FINGER_CYL_OPEN_OFFLOAD:
		{
			m_strMsg = m_bLangulageEng?_T("FingerCyl Cyl Open"):_T("放料位，夹爪打开");

			if(IsNoHardware() || FingerCylClose(-1, FALSE))
			{
				m_nNextAutoStep = AUTO_SEND_DATE;
			}
			break;
		}

		//给后段发送数据
	case AUTO_SEND_DATE:
		{
			m_strMsg = m_bLangulageEng?_T("Send Date"):_T("给后段发送数据");

			CString Result=_T(""); //服务器反馈
			CString BarCode = _T("");
			GetLocalPalletData(m_pTempPallet,BarCode);   //获得发送条码
			if (!m_bShieldServer || (m_Client.SendToDeviceAndWait(BarCode,Result) && Result == _T("02")))  //后段回复符号为02
			{
				MACHINE.m_strShowRecv = Result; // 调试界面显示
				//给后段发送数据
				m_pTempPallet[0].Release();
				m_pTempPallet[1].Release();
				MACHINE.m_nOffloadCount += 2;
			}
			else
			{
				ShowMessage(_T("发送条码给后段服务器失败，请检查!"),3);
				break;
			}
			//MACHINE.m_ReportFile.OkCount();
			//MACHINE.m_ReportFile.OkCount();
			m_nNextAutoStep = AUTO_ROBOT_TO_PUT_WAIT_POS_OFFLOAD_CHECK;
			SaveStepInfo(WRITE_TYPE_AUTO_STEP|WRITE_TYPE_VARIABLES|WRITE_TYPE_BATTERY);
			break;
		}

		//机器人回到安全位
	case AUTO_ROBOT_TO_PUT_WAIT_POS_OFFLOAD_CHECK:
		{
			m_strMsg = m_bLangulageEng?_T("Robot Move To Safe Loc"):_T("放料完成，机器人到良品放料等待位");

			if (IsNoHardware() || !m_bEnableRobot || m_pRobotClient.SendToDeviceAndWait(ROBOT_OFFLOAD_WAIT_POS))
			{
				if(m_bEnableRobot)
				{
					m_nCurRobotLoc = ROBOT_OFFLOAD_WAIT_POS;
				}
				m_nNextAutoStep = AUTO_CHECK_WITHOUT_PRODUCT_OFFLOAD;
			}
			else
			{
				//机器人移动超时
				ShowMessage(MSG_ROBOT_MOVE_TIMEOUT);
			}
			break;
		}

		//检测是否有料
	case AUTO_CHECK_WITHOUT_PRODUCT_OFFLOAD:
		{
			m_strMsg = m_bLangulageEng?_T("Check Without Product"):_T("放料完成，检测无料");

			if (IsNoHardware() || (CheckHasVaild(0,FALSE) && CheckHasVaild(1, FALSE)))                         
			{
				m_nNextAutoStep = AUTO_SEND_PUT_FINISH_SIGNAL_OFFLOAD;
			}
			break;
		}

		/*发送放料料完成*/
	case AUTO_SEND_PUT_FINISH_SIGNAL_OFFLOAD:
		{
			m_strMsg = m_bLangulageEng?_T("Send Onload Finish"):_T("给后段发送放料完成及安全信号");

			if (SendPlaceFinishSignal(TRUE) && SendSafeSignal(TRUE))
			{
				m_nNextAutoStep = AUTO_WAIT_OFFLOAD_TO_NEXT_FINISH;
				SaveStepInfo(WRITE_TYPE_AUTO_STEP|WRITE_TYPE_VARIABLES);
			}
			break;
		}
	case AUTO_WAIT_OFFLOAD_TO_NEXT_FINISH:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Offload To Next Finish"):_T("等待后段取料完成");
			if (MACHINE.m_bUnEnableDocking || CheckNeedBatteryEvent(FALSE) && SendPlaceFinishSignal(FALSE)) //双方信号都灭了
			{
				m_nNextAutoStep = AUTO_JUDGE_BATTERY_ALL_PUT_FINISH;
			}
			break;
		}
		//ng
		//等待NG可放料信号
	case AUTO_WAIT_START_EVENT_NG:
		{
			m_strMsg = m_bLangulageEng?_T("Wait NG Require"):_T("等待NG可放料信号");

			if(MACHINE.m_pPressNgLine != NULL && (MACHINE.m_pPressNgLine->m_bRequireNgBattery || !MACHINE.m_pPressNgLine->IsModuleEnable()))
			{
				m_bRobotWaitPos = FALSE;
				m_nNextAutoStep = AUTO_ROBOT_TO_NG_POS;
			}
			else
			{
				if (!m_bRobotWaitPos)
				{
					m_nNextAutoStep = AUTO_ROBOT_TO_NG_WAIT_POS;
				}
			}
			break;
		}
		//机器人到放NG等待位
	case AUTO_ROBOT_TO_NG_WAIT_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Robot To Put NG Wait Pos"):_T("机器人到放NG等待位");

			if (IsNoHardware() || !m_bEnableRobot || m_pRobotClient.SendToDeviceAndWait(ROBOT_OFFLOAD_NG_WAIT_POS))
			{
				if(m_bEnableRobot)
				{
					m_nCurRobotLoc = ROBOT_OFFLOAD_NG_WAIT_POS;
				}

				if (!m_bRobotWaitPos)
				{
					m_bRobotWaitPos = TRUE;
					m_nNextAutoStep = AUTO_WAIT_START_EVENT_NG;
				}
			}
			else
			{
				//机器人移动超时
				ShowMessage(MSG_ROBOT_MOVE_TIMEOUT);
			}
			break;
		}

		//机器人到NG位
	case AUTO_ROBOT_TO_NG_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Robot To Put Pos"):_T("机器人到放NG位");

			if (IsNoHardware() || !m_bEnableRobot || m_pRobotClient.SendToDeviceAndWait(ROBOT_OFFLOAD_NG_POS))
			{
				if(m_bEnableRobot)
				{
					m_nCurRobotLoc = ROBOT_OFFLOAD_NG_POS;
				}
				m_nNextAutoStep = AUTO_FINGER_CYL_OPEN_NG;
			}
			else
			{
				//机器人移动超时
				ShowMessage(MSG_ROBOT_MOVE_TIMEOUT);
			}
			break;
		}

	//	//等待MES信号
	//case AUTO_WAIT_MES_SINGLE_FOR_NG:
	//	{
	//		m_strMsg = m_bLangulageEng?_T("Wait Mes Single"):_T("等待MES信号");
	//		if((MACHINE.m_pUploadMes != NULL && (MACHINE.m_pUploadMes->m_bNextRequire || !MACHINE.m_pUploadMes->IsModuleEnable())))
	//		{
	//			MACHINE.m_pUploadMes->m_Pallet = m_pTempPallet[1];
	//			MACHINE.m_pUploadMes->m_bNextRequire = FALSE;
	//			m_nNextAutoStep = AUTO_FINGER_CYL_OPEN_NG;
	//		}
	//		break;
	//	}

		//NG位夹爪打开
	case AUTO_FINGER_CYL_OPEN_NG:
		{
			m_strMsg = m_bLangulageEng?_T("FingerCyl Cyl Open"):_T("NG位夹爪打开");
			ASSERT(-1 <= m_nWorkIndex && m_nWorkIndex <= 1);
			if(IsNoHardware() || FingerCylClose(m_nWorkIndex, FALSE))
			{
				if(0 <= m_nWorkIndex && m_nWorkIndex <= 1)
				{
					m_pTempPallet[m_nWorkIndex].Release();
					MACHINE.m_nShellStationPressNgCount[m_nWorkIndex]++;
					/*MACHINE.m_nNgCount++;*/
					//ASSERT(1 <= m_pTempPallet[m_nWorkIndex].m_nShellID && m_pTempPallet[m_nWorkIndex].m_nShellID <= 2);
					//MACHINE.m_nShellStationPressNgCount[m_pTempPallet[m_nWorkIndex].m_nShellID - 1]++;
					//MACHINE.m_ReportFile.NgCount();
				}
				else if(m_nWorkIndex == -1)
				{
					m_pTempPallet[0].Release();
					m_pTempPallet[1].Release();
					MACHINE.m_nNgCount += 2;
					MACHINE.m_nShellStationPressNgCount[0]++;
					MACHINE.m_nShellStationPressNgCount[1]++;
					//MACHINE.m_ReportFile.NgCount();
					//MACHINE.m_ReportFile.NgCount();
				}
				else
				{
					ASSERT(0);
				}
				m_nNextAutoStep = AUTO_ROBOT_TO_NG_WAIT_POS_CHECK;
				SaveStepInfo(WRITE_TYPE_AUTO_STEP|WRITE_TYPE_BATTERY);
			}
			break;
		}

		//机器人回到安全位
	case AUTO_ROBOT_TO_NG_WAIT_POS_CHECK:
		{
			m_strMsg = m_bLangulageEng?_T("Robot Move To Safe Loc"):_T("NG放料完成，机器人到NG等待位");

			if (IsNoHardware() || !m_bEnableRobot || m_pRobotClient.SendToDeviceAndWait(ROBOT_OFFLOAD_NG_WAIT_POS))
			{
				if(m_bEnableRobot)
				{
					m_nCurRobotLoc = ROBOT_OFFLOAD_NG_WAIT_POS;
				}
				m_nNextAutoStep = AUTO_CHECK_WITHOUT_PRODUCT_NG;
			}
			else
			{
				//机器人移动超时
				ShowMessage(MSG_ROBOT_MOVE_TIMEOUT);
			}
			break;
		}

		//检测是否有料
	case AUTO_CHECK_WITHOUT_PRODUCT_NG:
		{
			m_strMsg = m_bLangulageEng?_T("check FingerCyl Status"):_T("NG放料完成，检测无料");

			if(m_nWorkIndex == -1)
			{
				if (IsNoHardware() || (CheckHasVaild(0, FALSE) && CheckHasVaild(1, FALSE)))
				{
					m_nNextAutoStep = AUTO_SEND_NG_FINISH_SIGNAL_NG;
					SaveStepInfo(WRITE_TYPE_AUTO_STEP|WRITE_TYPE_VARIABLES);
				}
			}
			else
			{
				ASSERT(0 <= m_nWorkIndex && m_nWorkIndex <= 1);
				if (IsNoHardware() || CheckHasVaild(m_nWorkIndex, FALSE))
				{
					m_nNextAutoStep = AUTO_SEND_NG_FINISH_SIGNAL_NG;
					SaveStepInfo(WRITE_TYPE_AUTO_STEP|WRITE_TYPE_VARIABLES);
				}
			}
			break;
		}

		/*发送放料料完成*/
	case AUTO_SEND_NG_FINISH_SIGNAL_NG:
		{
			m_strMsg = m_bLangulageEng?_T("Send Onload Finish"):_T("发送放料完成");
			if (MACHINE.m_pPressNgLine != NULL && (MACHINE.m_pPressNgLine->m_bRequireNgBattery || !MACHINE.m_pPressNgLine->IsModuleEnable()))
			{
				MACHINE.m_pPressNgLine->m_bRequireNgBattery = FALSE;
				m_nNextAutoStep = AUTO_JUDGE_BATTERY_ALL_PUT_FINISH;
				SaveStepInfo(WRITE_TYPE_AUTO_STEP);
			}
			break;
		}

	case AUTO_ROBOT_TO_BUF_WAIT_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Robot To BufferCyl Wait Pos"):_T("机器人到缓存等待位");
			ASSERT(0 <= m_nWorkIndex && m_nWorkIndex <= 1);
			int nLoc = ((m_nWorkIndex == 0)?(ROBOT_OFFLOAD_BUFF_WAIT_POS1):(ROBOT_OFFLOAD_BUFF_WAIT_POS2));

			if (IsNoHardware() ||  !m_bEnableRobot || m_pRobotClient.SendToDeviceAndWait(nLoc))
			{
				if(m_bEnableRobot)
				{
					m_nCurRobotLoc = nLoc;
				}
				m_nNextAutoStep = AUTO_BUFFER_CYL_ACTION_BUF;
			}
			else
			{
				//机器人移动超时 
				ShowMessage(MSG_ROBOT_MOVE_TIMEOUT);
			}
			break;
		}

		//缓存位气缸动作
	case AUTO_BUFFER_CYL_ACTION_BUF:
		{
			m_strMsg = m_bLangulageEng?_T("Reserve Cyl Action"):_T("缓存位气缸动作");
			// 此处根据实际调试修改
			ASSERT(0 <= m_nWorkIndex && m_nWorkIndex <= 1);

			if(IsNoHardware() || BufferCylPush(m_nWorkIndex == 1?TRUE:FALSE))
			{
				m_nNextAutoStep = AUTO_CHECK_HAVE_PRODUCT_BUF;
			}
			break;
		}

		//检测缓存电芯状态
	case AUTO_CHECK_HAVE_PRODUCT_BUF:
		{
			m_strMsg = m_bLangulageEng?_T("Check Have Product BufferCyl"):_T("检测缓存电芯状态");
			// 此处根据实际调试修改
			ASSERT(0 <= m_nWorkIndex && m_nWorkIndex <= 1);
			if (IsNoHardware() || m_bDryRun || CheckHasVaild((m_nWorkIndex+2), m_pBufferPallet.m_bIsValid))
			{
				m_nNextAutoStep = AUTO_ROBOT_TO_BUF_POS;
			}
			break;
		}

		//机器人到缓存位
	case AUTO_ROBOT_TO_BUF_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Robot To BufferCyl Pos"):_T("机器人到缓存位");
			ASSERT(0 <= m_nWorkIndex && m_nWorkIndex <= 1);
			int nLoc = ((m_nWorkIndex == 0)?(ROBOT_OFFLOAD_BUFF_POS1):(ROBOT_OFFLOAD_BUFF_POS2));

			if (IsNoHardware() || !m_bEnableRobot || m_pRobotClient.SendToDeviceAndWait(nLoc))
			{
				if(m_bEnableRobot)
				{
					m_nCurRobotLoc = nLoc;
				}
				m_nNextAutoStep = AUTO_FINGER_CYL_ACTION_BUF;
			}
			else
			{
				//机器人移动超时
				ShowMessage(MSG_ROBOT_MOVE_TIMEOUT);
			}
			break;
		}

		//在缓存位夹抓动作
	case AUTO_FINGER_CYL_ACTION_BUF:
		{
			m_strMsg = m_bLangulageEng?_T("FingerCyl Cyl For BufferCyl Pos"):_T("在缓存位夹抓动作");
			ASSERT(0 <= m_nWorkIndex && m_nWorkIndex <= 1);
			if(IsNoHardware() || FingerCylClose(m_nWorkIndex, m_pBufferPallet.m_bIsValid))
			{
				BOOL bBufPalletRelease;   //TRUE 缓存清除数据 FALSE夹爪清除数据

				bBufPalletRelease = m_pBufferPallet.m_bIsValid?TRUE:FALSE;

				m_pBufferPallet.m_bIsValid?(m_pTempPallet[m_nWorkIndex] = m_pBufferPallet)
					:(m_pBufferPallet = m_pTempPallet[m_nWorkIndex]);

				bBufPalletRelease?m_pBufferPallet.Release():m_pTempPallet[m_nWorkIndex].Release();

				m_nNextAutoStep =AUTO_ROBOT_TO_BUF_WAIT_POS_CHECK;
				SaveStepInfo(WRITE_TYPE_AUTO_STEP|WRITE_TYPE_BATTERY);
			}
			break;
		}

		//机器人到缓存等待位
	case AUTO_ROBOT_TO_BUF_WAIT_POS_CHECK:
		{
			m_strMsg = m_bLangulageEng?_T("Robot Move To Safe Loc"):_T("机器人到缓存等待位");

			ASSERT(0 <= m_nWorkIndex && m_nWorkIndex <= 1);
			int nLoc = ((m_nWorkIndex == 0)?(ROBOT_OFFLOAD_BUFF_WAIT_POS1):(ROBOT_OFFLOAD_BUFF_WAIT_POS2));

			if (IsNoHardware() || !m_bEnableRobot || m_pRobotClient.SendToDeviceAndWait(nLoc))
			{
				if(m_bEnableRobot)
				{
					m_nCurRobotLoc = nLoc;
				}
				/*	if (m_pBufferPallet.m_bIsValid)
				{
				m_bBufferMes = TRUE;
				}*/
				m_nNextAutoStep = AUTO_CHECK_WITHOUT_PRODUCT_BUF;
			}
			else
			{
				//机器人移动超时
				ShowMessage(MSG_ROBOT_MOVE_TIMEOUT);
			}
			break;
		}

		//检测是否有料
	case AUTO_CHECK_WITHOUT_PRODUCT_BUF:
		{
			m_strMsg = m_bLangulageEng?_T("check FingerCyl Status"):_T("缓存取/放料完成，检测夹爪和缓存料状态");
			ASSERT(0 <= m_nWorkIndex && m_nWorkIndex <= 1);
			if (IsNoHardware() || m_bDryRun || (CheckHasVaild(m_nWorkIndex, m_pTempPallet[m_nWorkIndex].m_bIsValid) && CheckHasVaild(m_nWorkIndex + 2, m_pBufferPallet.m_bIsValid)))                         
			{
				m_nNextAutoStep = AUTO_JUDGE_BATTERY_ALL_PUT_FINISH;
			}
			break;
		}

		//判断所有电池放完成
	case AUTO_JUDGE_BATTERY_ALL_PUT_FINISH:
		{
			m_strMsg = m_bLangulageEng?_T("Judeg Put Ok All Finish"):_T("判断所有电池放完成");
			if(m_bDryRun)
			{
				m_nNextAutoStep = AUTO_WORK_END;
			}
			else
			{
				m_nNextAutoStep = AUTO_JUDGE_HAVE_NG;

			}
			
			break;
		}
		//工作完成
	case AUTO_WORK_END:
		{
			m_strMsg = m_bLangulageEng?_T("Work end"):_T("工作完成"); 
			m_nNextAutoStep = AUTO_IF_CLEAR_MATERIAL;
			break;
		}
	default:
		{
			ASSERT(0);
			break;
		}
	}
}

BOOL CRunProcessRobot::InitializeConfig(CString strProcessModule)
{
	if (!CRunProcess::InitializeConfig(strProcessModule))
	{
		return FALSE;
	}
	AfxBeginThread(RobotThread,this);
	return TRUE;
}

void CRunProcessRobot::GetHardwareConfig(CArray<int, int> *pInputs, CArray<int, int> *pOutputs, CArray<int, int> *pMotors)
{
	CRunProcess::GetHardwareConfig(pInputs, pOutputs, pMotors);
}
/****************************************************************************************
函数描述：检查电机能否移动
输入参数：
函数返回：TRUE:执行成功，FALSE:执行失败
*****************************************************************************************/
BOOL  CRunProcessRobot::CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{
	return TRUE;
}

BOOL  CRunProcessRobot::CheckOutputCanActive(COutput *pOutput)
{
#ifdef _TEST_
	return TRUE;
#endif
	ASSERT(m_OBufferCylPush > -1 && m_OBufferCylReturn > -1);

	if (pOutput == OUTPUTS[m_OBufferCylPush] || pOutput == OUTPUTS[m_OBufferCylReturn])
	{
		if (!CheckRobotSafe())
		{
			BLMessageBox(_T("机器人不在安全位，无杆气缸不能移动"), MB_ICONWARNING);
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CRunProcessRobot::SaveParameter()
{
	CRunProcess::SaveParameter();

	if (!IniWriteInt(m_strProcess, _T("RobotPort"), m_nRobotPort, GetAbsolutePathName(PARAMETER_CFG)))
		return FALSE;

	if (!IniWriteString(m_strProcess, _T("RobotIp"), m_strRobotIp, GetAbsolutePathName(PARAMETER_CFG)))
		return FALSE;

	if (!IniWriteBoolean(m_strProcess, _T("EnableRobot"), m_bEnableRobot, GetAbsolutePathName(PARAMETER_CFG)))
		return FALSE;

	if (!IniWriteBoolean(m_strProcess, _T("启用客户端"), m_bShieldServer, GetAbsolutePathName(PARAMETER_CFG)))
		return FALSE;

	if (!IniWriteInt(m_strProcess, _T("服务器端口"), m_nServerPort, GetAbsolutePathName(PARAMETER_CFG)))
		return FALSE;

	if (!IniWriteString(m_strProcess, _T("服务器端IP"), m_strServerIp, GetAbsolutePathName(PARAMETER_CFG)))
		return FALSE;

	return TRUE;
}

BOOL CRunProcessRobot::ReadRunParameter()
{
	CRunProcess:: ReadRunParameter();

	m_bEnableRobot = IniReadBoolean(m_strProcess, _T("EnableRobot"), FALSE, GetAbsolutePathName(PARAMETER_CFG));
	m_nRobotPort = IniReadInt(m_strProcess, _T("RobotPort"), 8880, GetAbsolutePathName(PARAMETER_CFG));
	m_strRobotIp = IniReadString(m_strProcess, _T("RobotIp"), _T("192.168.1.5"), GetAbsolutePathName(PARAMETER_CFG));

	m_bShieldServer = IniReadBoolean(m_strProcess, _T("启用客户端"),TRUE, GetAbsolutePathName(PARAMETER_CFG));
	m_nServerPort = IniReadInt(m_strProcess, _T("服务器端口"), 6668, GetAbsolutePathName(PARAMETER_CFG));
	m_strServerIp = IniReadString(m_strProcess, _T("服务器端IP"), _T("192.168.1.5"), GetAbsolutePathName(PARAMETER_CFG));

	return TRUE;
}

/****************************************************************************************
函数描述：夹爪气缸动作
输入参数：nIndex:-1代表所有夹爪气缸，为0-1时分别代表夹爪气缸1和2
bClose:TRUE:夹爪气缸夹紧 FALSE:夹爪气缸松开
函数返回：TRUE:执行成功，FALSE:执行失败
*****************************************************************************************/
BOOL CRunProcessRobot::FingerCylClose(int nIndex, BOOL bClose)
{
	ASSERT(-1 <= nIndex && nIndex < 2);
	for (int i = 0; i < 2; i++)
	{
		ASSERT(m_IFingerCylClose[i] > -1 && m_IFingerCylOpen[i] > -1 && m_OFingerCylClose[i] > -1 && m_OFingerCylOpen[i] > -1);
	}

	BOOL bRes = FALSE;

	if(nIndex == -1)
	{
		bClose?OUTPUTS[m_OFingerCylClose[0]]->On():OUTPUTS[m_OFingerCylClose[0]]->Off();
		bClose?OUTPUTS[m_OFingerCylClose[1]]->On():OUTPUTS[m_OFingerCylClose[1]]->Off();
		bClose?OUTPUTS[m_OFingerCylOpen[0]]->Off():OUTPUTS[m_OFingerCylOpen[0]]->On();
		bClose?OUTPUTS[m_OFingerCylOpen[1]]->Off():OUTPUTS[m_OFingerCylOpen[1]]->On();

		if (bClose && m_bDryRun)
		{
			Sleep(200);
			return TRUE;
		}

		if(WAIT_OK == WaitInput(INPUTS[m_IFingerCylClose[0]], bClose?ON:OFF, MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_IFingerCylClose[1]], bClose?ON:OFF, MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_IFingerCylOpen[0]], bClose?OFF:ON, MSG_SHOW_DEFAULT)
			&& WAIT_OK == WaitInput(INPUTS[m_IFingerCylOpen[1]], bClose?OFF:ON, MSG_SHOW_DEFAULT))
		{
			bRes = TRUE;
		}
	}
	else
	{
		bClose?OUTPUTS[m_OFingerCylClose[nIndex]]->On():OUTPUTS[m_OFingerCylClose[nIndex]]->Off();
		bClose?OUTPUTS[m_OFingerCylOpen[nIndex]]->Off():OUTPUTS[m_OFingerCylOpen[nIndex]]->On();

		if(WAIT_OK == WaitInput(INPUTS[m_IFingerCylClose[nIndex]], bClose?ON:OFF, MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_IFingerCylOpen[nIndex]], bClose?OFF:ON, MSG_SHOW_DEFAULT))
		{
			bRes = TRUE;
		}
	}
	return bRes;

}

/****************************************************************************************
函数描述：缓存气缸动作
输入参数：
bPush:TRUE:旋转气缸推出 FALSE:旋转气缸回退
函数返回：TRUE:执行成功，FALSE:执行失败
*****************************************************************************************/
BOOL CRunProcessRobot::BufferCylPush(BOOL bPush)
{
	ASSERT(m_IBufferCylPush > -1 && m_IBufferCylReturn > -1 && m_OBufferCylPush > -1 && m_OBufferCylReturn > -1);

	BOOL bRes = FALSE;
	bPush?(OUTPUTS[m_OBufferCylPush]->On()):(OUTPUTS[m_OBufferCylPush]->Off());
	bPush?(OUTPUTS[m_OBufferCylReturn]->Off()):(OUTPUTS[m_OBufferCylReturn]->On());

	if(WAIT_OK == WaitInput(INPUTS[m_IBufferCylPush], bPush?ON:OFF, MSG_SHOW_DEFAULT) 
		&& WAIT_OK == WaitInput(INPUTS[m_IBufferCylReturn], bPush?OFF:ON, MSG_SHOW_DEFAULT))
	{
		bRes = TRUE;
	}
	return bRes;
}

/****************************************************************************************
函数描述：检查是否有产品
输入参数：nIndex:-1代表所有工位，为0-1时分别代表夹爪1和2   为2-3时分别为缓存有料检测 
bHave:：TRUE:检查有料    FALSE:检查没有料
函数返回：TRUE:与bHave一致，FALSE:与bHave相反
*****************************************************************************************/
BOOL CRunProcessRobot::CheckHasVaild(int nIndex, BOOL bHave)
{
	ASSERT(-1 <= nIndex && nIndex <= 3);
	ASSERT(m_IHasCore[0] > -1 && m_IHasCore[1] > -1 && m_IHasCore[2] > -1 && m_IHasCore[3] > -1);

	BOOL bRes = FALSE;

	if(nIndex < 0)
	{
		bRes = TRUE;
		for (int i = 0; i < 4; i++)
		{
			if(WAIT_OK != CheckInput(INPUTS[m_IHasCore[i]], bHave?ON:OFF, MSG_SHOW_DEFAULT))
			{
				bRes = FALSE;
			}
		}
	}
	else
	{
		if(WAIT_OK == CheckInput(INPUTS[m_IHasCore[nIndex]], bHave?ON:OFF, MSG_SHOW_DEFAULT))
		{
			bRes = TRUE;
		}
	}
	return bRes;
}

/****************************************************************************************
函数描述：启动机器人，点亮停止信号，若已是点亮再点亮无效，操作机器人之前必须先点亮停止信号
输入参数：
函数返回：TRUE:执行成功，FALSE:执行失败
*****************************************************************************************/
BOOL CRunProcessRobot::RobotStop()
{
	BOOL bRes = FALSE;
	if(m_ORobotEmgStop > -1 && m_ORobotStop > -1)
	{
		OUTPUTS[m_ORobotEmgStop]->On();
		OUTPUTS[m_ORobotStop]->Off();
		Sleep(300);
		OUTPUTS[m_ORobotStop]->On();
		bRes = TRUE;
	}
	return bRes;
}

/****************************************************************************************
函数描述：复位机器人报警，除急停报警外
输入参数：
函数返回：TRUE:执行成功，FALSE:执行失败
*****************************************************************************************/
BOOL CRunProcessRobot::RobotAlarmReset()
{
	BOOL bRes = FALSE;
	if(m_ORobotAlarmReset > -1)
	{
		OUTPUTS[m_ORobotAlarmReset]->Off();
		Sleep(300);
		OUTPUTS[m_ORobotAlarmReset]->On();
		Sleep(300);
		OUTPUTS[m_ORobotAlarmReset]->Off();
		bRes = TRUE;
	}
	return bRes;
}

/****************************************************************************************
函数描述：复位机器人程序，时机器人程序从头开始执行；不复位，将从机器人由m_ORobotStop（DI06）导致机器人停止时，机器人程序所停在的代码行处开始
输入参数：
函数返回：TRUE:执行成功，FALSE:执行失败
*****************************************************************************************/
BOOL CRunProcessRobot::RobotCodeReset()
{
	BOOL bRes = FALSE;
	if(m_ORobotCodeReset > -1)
	{
		OUTPUTS[m_ORobotCodeReset]->Off();
		Sleep(300);
		OUTPUTS[m_ORobotCodeReset]->On();
		Sleep(300);
		OUTPUTS[m_ORobotCodeReset]->Off();
		bRes = TRUE;
	}
	return bRes;
}

/****************************************************************************************
函数描述：机器人程序运行，开始接受上位机命令
输入参数：
函数返回：TRUE:执行成功，FALSE:执行失败
*****************************************************************************************/
BOOL CRunProcessRobot::RobotCodeRun()
{
	BOOL bRes = FALSE;
	if(m_ORobotCodeRun > -1)
	{
		OUTPUTS[m_ORobotCodeRun]->Off();
		Sleep(300);
		OUTPUTS[m_ORobotCodeRun]->On();
		bRes = TRUE;
	}
	return bRes;
}

void CRunProcessRobot::GetLocalPalletData(CPallet* pallet,CString & strResult)
{
	if (pallet[0].m_strBarCode.IsEmpty() && pallet[1].m_strBarCode.IsEmpty())  //如果条码是空的
	{
		CString m_pallet = _T("");
		CTime TIME = CTime::GetCurrentTime();
		m_pallet = TIME.Format(_T("%X"));
		pallet[0].m_strBarCode = m_pallet;
		pallet[1].m_strBarCode = m_pallet;
		strResult.Format(_T("%s,%s"),pallet[0].m_strBarCode,pallet[1].m_strBarCode);
	}
	else
	{
		strResult.Format(_T("%s,%s"),m_pTempPallet[0].m_strBarCode,m_pTempPallet[1].m_strBarCode);
	}
	MACHINE.m_strShowSend = strResult;  //调试界面显示条码

}

/****************************************************************************************
函数描述：检查机器人Z轴是否在安全位 只检测不移动
输入参数：
函数返回：TRUE:执行成功，FALSE:执行失败
*****************************************************************************************/
BOOL CRunProcessRobot::CheckRobotSafe()
{
	if (!m_bEnableRobot || g_robotcmd[m_nCurRobotLoc].nCmdLevel != 1)
	{
		return TRUE;
	}
	return FALSE;
}

/****************************************************************************************
函数描述：读取数据
输入参数：
函数返回：TRUE:执行成功，FALSE:执行失败
*****************************************************************************************/
void CRunProcessRobot::ReadStepInfo()				//读取带料启动数据
{
	CString strPath = GetAbsolutePathName(MODULE_RUN_FOLDER) + m_strName + _T(".cfg");

	m_nNextAutoStep = IniReadInt(_T("Data"), _T("m_nNextAutoStep"), 0, strPath);
	m_nWorkIndex = IniReadInt(_T("Data"), _T("m_nWorkIndex"), -1, strPath);

	m_pBufferPallet.ReadCfg(_T("BufferCylPallet"), strPath);
	m_pTempPallet[0].ReadCfg(_T("TempPallet0"), strPath);
	m_pTempPallet[1].ReadCfg(_T("TempPallet1"), strPath);

	return;
}

/****************************************************************************************
函数描述：保存数据
输入参数：
函数返回：TRUE:执行成功，FALSE:执行失败
*****************************************************************************************/
BOOL CRunProcessRobot::SaveStepInfo(int nWriteType)   	//保存带料启动数据
{
	CString strPath = GetAbsolutePathName(MODULE_RUN_FOLDER) + m_strName + _T(".cfg");

	if(nWriteType & WRITE_TYPE_AUTO_STEP)
	{
		IniWriteInt(_T("Data"), _T("m_nNextAutoStep"), m_nNextAutoStep, strPath);
	}

	if(nWriteType & WRITE_TYPE_VARIABLES)
	{
		IniWriteInt(_T("Data"), _T("m_nWorkIndex"), m_nWorkIndex, strPath);
	}

	if(nWriteType & WRITE_TYPE_BATTERY)
	{
		m_pBufferPallet.WriteCfg(_T("BufferCylPallet"), strPath);
		m_pTempPallet[0].WriteCfg(_T("TempPallet0"), strPath);
		m_pTempPallet[1].WriteCfg(_T("TempPallet1"), strPath);
	}
	return TRUE;
}

//检测料盘数据与实际检测数据一致性,nCheckResult为输出参数0夹抓没有料，1夹抓都有料，2：夹住1有料夹抓2没料，3：夹住1无料夹抓2有料
BOOL CRunProcessRobot::CheckStartWithPalletDataSame()
{
	CString strMsg = _T("");
	for(int i=0;i<2;i++)
	{
		if(m_pTempPallet[i].m_bIsValid )
		{
			if((m_IHasCore[i] > -1 && INPUTS[m_IHasCore[i]]->IsOff()))
			{
				strMsg.Format(_T("带料启动,机器人有料感应与实际数据不一致,数据有料,感应无料,请确认"));
				ShowMessageEX(strMsg);
				strMsg.Format(_T("确认机器人有料吗？有料点YES，无料点NO"));
				if(BLMessageBox(strMsg,MB_YESNO) == IDYES)
				{
					m_pTempPallet[i].m_bIsValid = TRUE;
				}
				else
				{
					m_pTempPallet[i].m_bIsValid = FALSE;
				}
				return FALSE;
			}
		}
		else
		{
			if((m_IHasCore[i] > -1 && INPUTS[m_IHasCore[i]]->IsOn()))
			{
				strMsg.Format(_T("带料启动,机器人有料感应与实际数据不一致,数据无料,感应有料,请确认"));
				ShowMessageEX(strMsg);
				strMsg.Format(_T("确认机器人有料吗？有料点YES，无料点NO"));
				if(BLMessageBox(strMsg,MB_YESNO) == IDYES)
				{
					m_pTempPallet[i].m_bIsValid = TRUE;
				}
				else
				{
					m_pTempPallet[i].m_bIsValid = FALSE;
				}
				return FALSE;
			}
		}
	}

	if(!m_pTempPallet[0].m_bIsValid && !m_pTempPallet[1].m_bIsValid)
	{
		m_nStartWithPalletData = 0;
	}
	else if(m_pTempPallet[0].m_bIsValid && m_pTempPallet[1].m_bIsValid)
	{
		m_nStartWithPalletData = 1;
	}
	else if(m_pTempPallet[0].m_bIsValid && !m_pTempPallet[1].m_bIsValid)
	{
		m_nStartWithPalletData = 2;
	}
	else if(!m_pTempPallet[0].m_bIsValid && m_pTempPallet[1].m_bIsValid)
	{
		m_nStartWithPalletData = 3;
	}
	return TRUE;
}


BOOL CRunProcessRobot::CheckStartWithPalletDataSameForBuffer()
{
	CString strMsg = _T("");
	if(m_pBufferPallet.m_bIsValid)
	{
		if(m_IHasCore[2] > -1 && INPUTS[m_IHasCore[2]]->IsOff()
			&& m_IHasCore[3] > -1 && INPUTS[m_IHasCore[3]]->IsOff())
		{
			strMsg.Format(_T("带料启动,机器人缓存有料感应与实际数据不一致,数据有料,感应无料,请确认"));
			ShowMessageEX(strMsg);
			strMsg.Format(_T("确认机器人缓存有料吗？有料点YES，无料点NO"));
			if(BLMessageBox(strMsg,MB_YESNO) == IDYES)
			{
				m_pBufferPallet.m_bIsValid = TRUE;
			}
			else
			{
				m_pBufferPallet.m_bIsValid = FALSE;
			}
			return FALSE;
		}
	}
	else
	{
		if(m_IHasCore[2] > -1 && INPUTS[m_IHasCore[2]]->IsOn()
			|| m_IHasCore[3] > -1 && INPUTS[m_IHasCore[3]]->IsOn())
		{
			strMsg.Format(_T("带料启动,机器人缓存有料感应与实际数据不一致,数据有料,感应无料,请确认"));
			ShowMessageEX(strMsg);
			strMsg.Format(_T("确认机器人缓存有料吗？有料点YES，无料点NO"));
			if(BLMessageBox(strMsg,MB_YESNO) == IDYES)
			{
				m_pBufferPallet.m_bIsValid = TRUE;
			}
			else
			{
				m_pBufferPallet.m_bIsValid = FALSE;
			}
			return FALSE;
		}
	}

	if (!m_pBufferPallet.m_bIsValid)
	{
		m_nBufferWithPalletData = 0;
	}
	else if (m_pBufferPallet.m_bIsValid)
	{
		m_nBufferWithPalletData = 1;
	}
	return TRUE;
}

/****************************************************************************************
函数描述：连接机器人
输入参数：
函数返回：TRUE:执行成功，FALSE:执行失败
*****************************************************************************************/
BOOL CRunProcessRobot::ConnectRobot()
{
	BOOL bRes = FALSE;
	if(m_ORobotEmgStop > -1 && m_ORobotStop > -1 && m_ORobotAlarmReset > -1 
		&& m_ORobotCodeReset > -1 && m_ORobotCodeRun > -1)
	{
		OUTPUTS[m_ORobotEmgStop]->On();
		Sleep(100);
		OUTPUTS[m_ORobotStop]->On();
		Sleep(100);
		OUTPUTS[m_ORobotAlarmReset]->On();
		Sleep(300);
		OUTPUTS[m_ORobotAlarmReset]->Off();
		Sleep(100);
		OUTPUTS[m_ORobotCodeReset]->On();
		Sleep(300);
		OUTPUTS[m_ORobotCodeReset]->Off();
		Sleep(100);
		OUTPUTS[m_ORobotCodeRun]->On();
		bRes = TRUE;
	}
	return bRes;
}

/****************************************************************************************
函数描述：断开连接机器人
输入参数：
函数返回：TRUE:执行成功，FALSE:执行失败
*****************************************************************************************/
BOOL CRunProcessRobot::DisconnectRobot()
{
	BOOL bRes = FALSE;
	if(m_ORobotStop > -1 && m_ORobotEmgStop > -1 && m_ORobotCodeRun > -1)
	{
		OUTPUTS[m_ORobotStop]->Off();
		OUTPUTS[m_ORobotEmgStop]->Off();
		OUTPUTS[m_ORobotCodeRun]->Off();
		bRes = TRUE;
	}
	return bRes;
}

/****************************************************************************************
函数描述：轴压料急停
输入参数：
函数返回：TRUE:夹爪安全，FALSE:夹爪压料
*****************************************************************************************/
void CRunProcessRobot::EmgStop()
{
	if(MACHINE.m_pRunCtrl == NULL)
	{		
		return;
	}
	int nStatues = MACHINE.m_pRunCtrl->GetMcState();
	if (nStatues == MC_RUNNING)
	{
		if (m_IZCheckAbnormal > -1 && INPUTS[m_IZCheckAbnormal]->IsOff())
		{
			if (m_ORobotEmgStop > -1)
			{
				OUTPUTS[m_ORobotEmgStop]->Off();
			}

			MACHINE.m_pRunCtrl->Stop();
			ShowMessage(ROBOT_PRESS_ALARM);
			return ;
		}
		else
		{
			if (m_ORobotEmgStop > -1 && OUTPUTS[m_ORobotEmgStop]->IsOff())
			{
				OUTPUTS[m_ORobotEmgStop]->On();
			}
		}
	}
	return;
}

/****************************************************************************************
函数描述： 发送机器人是否在安全位信号
输入参数： bFinish:TRUE:安全信号  FALSE:不安全信号
函数返回：TRUE:执行成功，FALSE:执行失败
*****************************************************************************************/


BOOL  CRunProcessRobot::SendSafeSignal(BOOL bSafe)
{
	if (m_ORobotPutInPosSignal[0] < 0 || m_ORobotPutInPosSignal[1] < 0)
	{
		return FALSE;
	}
	
	if (bSafe)
	{
		if (OUTPUTS[m_ORobotPutInPosSignal[0]]->On() && OUTPUTS[m_ORobotPutInPosSignal[1]]->On())
		{
			return TRUE;
		}
	}
	else
	{
		if (OUTPUTS[m_ORobotPutInPosSignal[0]]->Off() && OUTPUTS[m_ORobotPutInPosSignal[1]]->Off())  //不安全信号
		{
			return TRUE;
		}
	}

	return FALSE;
}

/****************************************************************************************
函数描述： 发送放料完成信号
输入参数： bFinish:TRUE:放料完成   FALSE:复位放料信号
函数返回：TRUE:执行成功，FALSE:执行失败
*****************************************************************************************/

BOOL CRunProcessRobot::SendPlaceFinishSignal(BOOL bFinish)
{
	if (m_ORobotPutFinishSignal[0] < 0 && m_ORobotPutFinishSignal[1] < 0)
	{
		return FALSE;
	}

	if (bFinish)
	{
		if (OUTPUTS[m_ORobotPutFinishSignal[0]]->On() && OUTPUTS[m_ORobotPutFinishSignal[1]]->On())
		{
			return TRUE;
		}
	}
	else
	{
		if (OUTPUTS[m_ORobotPutFinishSignal[0]]->Off() && OUTPUTS[m_ORobotPutFinishSignal[1]]->Off())
		{
			return TRUE;
		}
	}

	return FALSE;

}

/****************************************************************************************
函数描述： 检查后段是否要料
输入参数： bNeed:TRUE:检查要料   FALSE:检查复位要料信号
函数返回：TRUE:执行成功，FALSE:执行失败
*****************************************************************************************/

BOOL CRunProcessRobot::CheckNeedBatteryEvent(BOOL bNeed)
{
	if (m_ITailFingerSignal[0] < 0 || m_ITailFingerSignal[1] < 0)
	{
		return FALSE;
	}

	if (bNeed)
	{
		if (INPUTS[m_ITailFingerSignal[0]]->IsOn() && INPUTS[m_ITailFingerSignal[1]]->IsOn() &&INPUTS[m_INormalRunSingle]->IsOn())
		{
			return TRUE;
		}
	}
	else
	{
		if (INPUTS[m_ITailFingerSignal[0]]->IsOff() && INPUTS[m_ITailFingerSignal[1]]->IsOff() && INPUTS[m_INormalRunSingle]->IsOn())
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CRunProcessRobot::CheckBackSafe()
{
	return FALSE;
}

void CRunProcessRobot::NomalRunEvent()
{
	if (!MACHINE.MachineExit)
	{
		int nStatus =  MACHINE.m_pRunCtrl->GetMcState();
		if (MC_INITIALIZING == nStatus || MC_RUNNING == nStatus )
		{
			if (m_ONormalRunSingle > -1 && OUTPUTS[m_ONormalRunSingle]->IsOff()) //给后段正常运行信号
			{
				OUTPUTS[m_ONormalRunSingle]->On();
			}
		}
		else if(m_ONormalRunSingle > -1 && OUTPUTS[m_ONormalRunSingle]->IsOn())
		{
			OUTPUTS[m_ONormalRunSingle]->Off();
		}
	}
}