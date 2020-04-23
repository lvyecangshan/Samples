/****************************************************************************************
文件名：CRunProcessFeederTray.cpp
功能描述：料盘搬运模组
历史修改记录：
*****************************************************************************************/
#include "StdAfx.h"
#include "RunProcessFeederTray.h"

//防撞感应和光栅检测
UINT CheckMoveSafe(void* lpVoid)
{
	CRunProcessFeederTray *pFeederTray = (CRunProcessFeederTray*)lpVoid;
	if (pFeederTray == NULL)
	{
		return 0;
	}
	pFeederTray->m_bThreadFinished = FALSE;
	pFeederTray->m_bThreadStart = TRUE;

	Sleep(2000);
	while (pFeederTray->m_bThreadStart)
	{
		/*if(!IsNoHardware())
		pFeederTray->WorkWhile();*/
		Sleep(5);
	}
	pFeederTray->m_bThreadFinished = TRUE;
	return 0;
}

CRunProcessFeederTray::CRunProcessFeederTray(int nID) :
CRunProcess(nID)
{
	CString strKey;

	PowerUpRestart();

	for(int i = 0; i < 2; i++)
	{
		strKey.Format(_T("ISeparateClose%d"), i);
		m_InputMap.insert(pair<int*, CString>(&m_ISeparateClose[i], strKey));

		strKey.Format(_T("ISeparateOpen%d"), i);
		m_InputMap.insert(pair<int*, CString>(&m_ISeparateOpen[i], strKey));
	}
	for(int  i = 0; i < 6; i ++)
	{
		strKey.Format(_T("IVacuum%d"), i);
		m_InputMap.insert(pair<int*, CString>(&m_IVacuum[i], strKey));

		strKey.Format(_T("OVacuum%d"), i);
		m_OutputMap.insert(pair<int*, CString>(&m_OVacuum[i], strKey));
	}

	for(int i = 0; i < 3; i++)
	{
		strKey.Format(_T("ISafeRasterCheck%d"), i);
		m_InputMap.insert(pair<int*, CString>(&m_ISafeRasterCheck[i],strKey));

		strKey.Format(_T("ITrayInPosPresent%d"), i);
		m_InputMap.insert(pair<int*, CString>(&m_ITrayInPosPresent[i], strKey));
	}

	m_InputMap.insert(pair<int*, CString>(&m_ISafeCheckPallet, _T("ISafeCheckPallet")));
	m_InputMap.insert(pair<int*, CString>(&m_IPalletInPosition, _T("IPalletInPosition")));

	m_OutputMap.insert(pair<int*, CString>(&m_OSeparateClose, _T("OSeparateClose")));
	m_OutputMap.insert(pair<int*, CString>(&m_OSeparateOpen, _T("OSeparateOpen")));

	m_MotorMap.insert(pair<int*, CString>(&m_nMotorX, _T("MotorX")));
}

CRunProcessFeederTray::~CRunProcessFeederTray(void)
{
	Destory();
}

void CRunProcessFeederTray::PowerUpRestart()
{
	CRunProcess::PowerUpRestart();
	m_strMsg = m_bLangulageEng? _T("Ready"):_T("准备好");

	m_nPickPalletPos = -1;
	m_nMoveMotorIndex = 0;
	m_bPutPalletFinish = FALSE;
	m_bMoveMotorFinish = FALSE;
	m_bPickPalletFinish = FALSE;
	m_bOnloadPnpRequireShell = FALSE;

	m_nNextPowerUpStep = PU_CHECK_ANOTONER_MODULE_SAFE;
	m_nNextAutoStep = AUTO_JUDGE_HAVE_PALLET;
}

void CRunProcessFeederTray::PowerUpOperation(void)
{
	if (!IsModuleEnable())
	{
		CRun::PowerUpOperation();
		m_strMsg = m_bLangulageEng?_T("Moudle UnEnable"):_T("模块禁用");
		return;
	}

	if(::IsNoHardware())
	{
		Sleep(500);
	}

	switch(m_nNextPowerUpStep)
	{
		//判断其他模组是否安全
	case PU_CHECK_ANOTONER_MODULE_SAFE:
		{
			m_strMsg = m_bLangulageEng?_T("Check Another Module Safe"):_T("判断其他模组是否安全");

			if (CheckCanHome())
			{
				m_bPutPalletFinish = FALSE;
				m_bMoveMotorFinish = FALSE;
				m_nNextPowerUpStep = PU_MOVE_MOTOR_X_HOME;
			}
			break;
		}
		//X轴电机复位
	case PU_MOVE_MOTOR_X_HOME:
		{
			m_strMsg = m_bLangulageEng?_T("Move Motor Y Home"):_T("Y轴电机复位");

			if(m_nMotorX > -1 && HomeMotor(MOTORS[m_nMotorX]))
			{
				ResertHomeSingle();
				m_nNextPowerUpStep = PU_CHEACK_HAVE_PALLET;
			}
			break;
		}

		//检查有无料盘
	case PU_CHEACK_HAVE_PALLET:
		{
			m_strMsg = m_bLangulageEng?_T("Check Have Pallet, Rejust Recover"):_T("检查料盘状态，判断恢复生产");
			if(MACHINE.m_bReCoverMachine)
			{
				ReadStepInfo();
				if(m_IPalletInPosition > -1 && INPUTS[m_IPalletInPosition]->IsOn())	
				{
					m_nNextAutoStep = AUTO_JUDGE_ALL_COVE_PICK_FINISH;
					m_nNextPowerUpStep = PU_MOTOR_TO_ONLOAD_POS;
				}
				else
				{
					m_nPickPalletPos = -1;
					m_nMoveMotorIndex = 0;
					m_nNextAutoStep = AUTO_JUDGE_HAVE_PALLET;
					m_nNextPowerUpStep = PU_OPEN_CLAMP_CYL;
				}
			}
			else
			{
				if(::IsNoHardware() || CheckHaveNoPallet())
				{
					m_nPickPalletPos = -1;
					m_nMoveMotorIndex = 0;
					m_nNextAutoStep = AUTO_JUDGE_HAVE_PALLET;
					m_nNextPowerUpStep = PU_OPEN_CLAMP_CYL;
				}
				else
				{
					// 搬运模组检测到料盘，为保证设备正常运行请人工清走
					ShowMessage(MSG_FEEDERTRAY_CHECK_PALLET_VAILD);
				}
			}
			break;
		}

		//吸盘真空打开
	case PU_OPEN_CLAMP_CYL:
		{
			m_strMsg = m_bLangulageEng?_T("Close Clamp Cyl"):_T("吸盘真空关闭");

			if(::IsNoHardware() || OpenVacuum(FALSE))
			{
				m_nNextPowerUpStep = PU_OPEN_FINGLE_CYL;
			}
			break;
		}

		//托盘气缸打开
	case  PU_OPEN_FINGLE_CYL:
		{
			m_strMsg = m_bLangulageEng?_T("Open Fingle Cyl"):_T("托盘气缸打开");

			if (IsNoHardware() || SeparateClose(FALSE))
			{
				m_nNextPowerUpStep = PU_MOTOR_TO_ONLOAD_POS;
			}
			break;
		}

		//电机移动到上料盘位1
	case PU_MOTOR_TO_ONLOAD_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Motor To Onload Pos"):_T("电机移动到上料盘位1");

			if(m_nMotorX > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorX, X_MOTOR_ONLOAD_LOC1))
			{
				m_nNextPowerUpStep = PU_CHECK_MOTOR_INPOS;
			}
			break;
		}

		//检测电机位置
	case PU_CHECK_MOTOR_INPOS:
		{
			m_strMsg = m_bLangulageEng?_T("Check Motor Inpos"):_T("检测电机位置");

			if(m_ITrayInPosPresent[0] > -1 && WAIT_OK == WaitInput(INPUTS[m_ITrayInPosPresent[0]], ON, MSG_SHOW_DEFAULT)) 
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

void CRunProcessFeederTray::AutoOperation()
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
		if (m_nSaveLastStep == AUTO_JUDGE_HAVE_PALLET)
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
		//检查堆栈模组有无料盘
	case AUTO_JUDGE_HAVE_PALLET:
		{
			m_strMsg = m_bLangulageEng?_T("Judge Have Pallet"):_T("检查堆栈模组有无料盘");

			m_bPutPalletFinish = FALSE;

			ASSERT(m_nPickPalletPos == -1);

			for (int i = 0; i < 2; i++)
			{
				if(MACHINE.m_pOnloadShellStack[i] != NULL && MACHINE.m_pOnloadShellStack[i]->m_bHavePallet)
				{
					m_nPickPalletPos = i;
					break;
				}
			}

			if(-1 < m_nPickPalletPos && m_nPickPalletPos <= 1)
			{
				m_nNextAutoStep = AUTO_MOVE_MOTOR_TO_PICK_PALLET_POS;
			}
			break;
		}

		//移动电机到取料盘位
	case AUTO_MOVE_MOTOR_TO_PICK_PALLET_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Move Motor To Pick Pallet Pos"):_T("移动电机到取料盘位");
			ASSERT(0 <= m_nPickPalletPos && m_nPickPalletPos <= 1);

			if(m_nMotorX > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorX, X_MOTOR_ONLOAD_LOC1 + m_nPickPalletPos))
			{
				m_bPickPalletFinish = FALSE;
				m_nNextAutoStep =AUTO_CHECK_MOTOR_MOVE_ONLOAD_INPOS;
			}
			break;
		}

		//检查电机是否移动到取料盘
	case AUTO_CHECK_MOTOR_MOVE_ONLOAD_INPOS:
		{
			m_strMsg = m_bLangulageEng?_T("Check Motor Move Onload1 Inpos"):_T("检查电机是否移动到取料盘位");

			ASSERT(0 <= m_nPickPalletPos && m_nPickPalletPos <= 1 && m_ITrayInPosPresent[m_nPickPalletPos] > -1);

			if(IsNoHardware() || INPUTS[m_ITrayInPosPresent[m_nPickPalletPos]]->IsOn())
			{
				Sleep(100);
				if(IsNoHardware() || INPUTS[m_ITrayInPosPresent[m_nPickPalletPos]]->IsOn())
				{
					m_nNextAutoStep = AUTO_SEND_NEDD_PALLET_SINGLE_TO_ONLOAD;
					break;
				}
			}
			// 搬运模组电机移动到堆栈位，检测到位感应器状态异常(应为ON)，请检查确认
			ShowMessage(MSG_FEEDERTRAY_MOTOR_MOVE_NOT_INPOS);
			break;
		}

		//给上料工位1发送上料信号
	case AUTO_SEND_NEDD_PALLET_SINGLE_TO_ONLOAD:
		{
			m_strMsg = m_bLangulageEng?_T("Send Need Pallet Single To Onload1"):_T("给上料工位发送上料信号");

			ASSERT(0 <= m_nPickPalletPos && m_nPickPalletPos <= 1);

			if(MACHINE.m_pOnloadShellStack[m_nPickPalletPos] != NULL && !MACHINE.m_pOnloadShellStack[m_nPickPalletPos]-> m_bRequirePallet)
			{
				MACHINE.m_pOnloadShellStack[m_nPickPalletPos]-> m_bRequirePallet = TRUE;
				m_nNextAutoStep = AUTO_WAIT_ONLOAD_MODULE_SEND_PALLET_FINISH;
			}
			break; 
		} 

		//等待上料堆栈1工位将料盘送到位
	case AUTO_WAIT_ONLOAD_MODULE_SEND_PALLET_FINISH:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Onload Module1 Send Pallet Finish"):_T("等待上料堆栈工位将料盘送到位");

			ASSERT(0 <= m_nPickPalletPos && m_nPickPalletPos <= 1);

			if(MACHINE.m_pOnloadShellStack[m_nPickPalletPos] != NULL
				&& !MACHINE.m_pOnloadShellStack[m_nPickPalletPos]-> m_bRequirePallet
				&& (MACHINE.m_pOnloadShellStack[m_nPickPalletPos]->m_bSendPalletFinish) && !m_bPickPalletFinish)
			{
				MACHINE.m_pOnloadShellStack[m_nPickPalletPos]->m_bSendPalletFinish = FALSE;
				m_nNextAutoStep = AUTO_CLAMP_CHECK_HAVE_PALLET;
			}
			break;
		}

		//检查料盘有无到位
	case AUTO_CLAMP_CHECK_HAVE_PALLET:
		{
			m_strMsg = m_bLangulageEng?_T("Clamp Check Have Pallet"):_T("检查料盘有无到位");

			if(::IsNoHardware() || m_bDryRun || !CheckHaveNoPallet())
			{
				m_nNextAutoStep = AUTO_CLMAP_CYL_CLAMP;
			}
			else
			{
				// 搬运模组接收铝壳堆栈料盘时，检测不到料盘，请人工干预处理
				ShowMessage(MSG_FEEDERTRAY_CHECK_PALLET_INVAILD);
			}
			break;
		}
		//真空打开
	case AUTO_CLMAP_CYL_CLAMP:
		{
			m_strMsg = m_bLangulageEng?_T("Vacuum Open"):_T("真空打开");

			//if((::IsNoHardware() || OpenVacuum(TRUE)) && !m_bPickPalletFinish)  调试临时屏蔽——2019年6月21日0:43:46
			{
				m_nNextAutoStep = AUTO_FINGLE_CYL_CLAMP;
			}
			break;
		}
	
		//托盘气缸夹紧
	case AUTO_FINGLE_CYL_CLAMP:
		{
			m_strMsg = m_bLangulageEng?_T("Fingle Cyl Clamp"):_T("托盘气缸夹紧料盘");

			if (IsNoHardware() || SeparateClose(TRUE))
			{
				m_bPickPalletFinish = TRUE;
				m_nNextAutoStep = AUTO_CHECK_HAVE_PALLET_AGANIN;
			}
			break;
		}
		//再次确认有无料盘
	case AUTO_CHECK_HAVE_PALLET_AGANIN:
		{
			m_strMsg = m_bLangulageEng?_T("Check Have Pallet Again"):_T("再次确认有无料盘");

			if(::IsNoHardware() || m_bDryRun || !CheckHaveNoPallet()) 
			{
				m_nNextAutoStep = AUTO_WAIT_ONLOAD_MODULE_MOVE_SAFE_POS;
			}
			else
			{
				// 搬运模组接收铝壳堆栈料盘时，检测不到料盘，请人工干预处理
				ShowMessage(MSG_FEEDERTRAY_CHECK_PALLET_INVAILD);
			}
			break;
		}

		//等待供料盘移动到安全位置
	case AUTO_WAIT_ONLOAD_MODULE_MOVE_SAFE_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Onload Moudle Move Safe Pos"):_T("等待铝壳上料堆栈电机移动到安全位置");

			ASSERT(0 <= m_nPickPalletPos && m_nPickPalletPos <= 1);

			if(!m_bPickPalletFinish && MACHINE.m_pOnloadShellStack[m_nPickPalletPos] != NULL && MACHINE.m_pOnloadShellStack[m_nPickPalletPos]->m_bMotorSafePos)
			{
				MACHINE.m_pOnloadShellStack[m_nPickPalletPos]->m_bMotorSafePos = FALSE;
				m_nPickPalletPos = -1;
				m_nNextAutoStep = AUTO_MOVE_MOTOR_TO_START_FEED_COVER_POS;
				break;
			}
			break;
		}

		//将电机移动到开始上壳位
	case AUTO_MOVE_MOTOR_TO_START_FEED_COVER_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Move Motor To Start Feed Cover Pos"):_T("将电机移动到开始上壳位");

			if(m_nMotorX > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorX, X_PICK_COVER_1))
			{
				m_nMoveMotorIndex = 0;
				m_nNextAutoStep = AUTO_WAIT_START_EVENT;
			}
			break;
		}

		//等待开始工作信号
	case AUTO_WAIT_START_EVENT:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Start Event"):_T("等待开始工作信号");
			//上壳机械手发送取壳信号
			if(m_bOnloadPnpRequireShell && !m_bMoveMotorFinish)
			{
				m_bOnloadPnpRequireShell = FALSE;
				m_nNextAutoStep = AUTO_MOVE_MOTOR_TO_FEED_COVER_POS;
			}
			break;
		}

		//移动电机到取壳位
	case AUTO_MOVE_MOTOR_TO_FEED_COVER_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Move Motor To Feed Cover Pos"):_T("移动电机到取壳位");

			int nLoc = X_PICK_COVER_1 + m_nMoveMotorIndex;
			ASSERT(X_PICK_COVER_1 <= nLoc && nLoc <= X_PICK_COVER_6);
			if(m_nMotorX > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorX, nLoc))
			{
				if(MACHINE.m_pOnloadShellPnp != NULL && !MACHINE.m_pOnloadShellPnp->m_bPickCoverFinish)
				{
					m_bMoveMotorFinish = TRUE;
					m_nNextAutoStep = AUTO_WAIT_PICK_COVER_FINISH;
				}
			}
			break;
		}

		//等待抓手抓料完成
	case AUTO_WAIT_PICK_COVER_FINISH:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Pick Cover Finish"):_T("等待抓手抓料完成");

			if(MACHINE.m_pOnloadShellPnp != NULL && MACHINE.m_pOnloadShellPnp->m_bPickCoverFinish && !m_bMoveMotorFinish)
			{
				m_nMoveMotorIndex++;
				MACHINE.m_pOnloadShellPnp->m_bPickCoverFinish = FALSE;
				m_nNextAutoStep = AUTO_JUDGE_ALL_COVE_PICK_FINISH;
				SaveStepInfo(WRITE_TYPE_VARIABLES);
			}			
			break;
		}

		//判断所用的壳有无抓完
	case AUTO_JUDGE_ALL_COVE_PICK_FINISH:
		{ 
			m_strMsg = m_bLangulageEng?_T("Judge All Cover Pick Finish"):_T("判断一盘料有无抓完");

			if(m_nMoveMotorIndex < 6)
			{
				m_nNextAutoStep = AUTO_WAIT_START_EVENT;
			}
			else
			{
				m_nMoveMotorIndex = 0;
				m_bMoveMotorFinish = FALSE;
				m_bOnloadPnpRequireShell = FALSE;
				m_nNextAutoStep = AUTO_MOVE_PALLET_TO_OFFLAOD_POS;
				SaveStepInfo(WRITE_TYPE_VARIABLES);
			}
			break;
		}

		//移动空盘到下料位
	case AUTO_MOVE_PALLET_TO_OFFLAOD_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Move Pallet To Offload Pos"):_T("移动空盘到下料位");

			if(m_nMotorX > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_nMotorX, X_MOTOR_OFFLOAD_LOC))
			{
				m_nNextAutoStep = AUTO_CHECK_MOTOR_MOVE_OFFLOAD_INPOS;
			}
			break;
		}

		//检查电机是否移动到取料盘2
	case AUTO_CHECK_MOTOR_MOVE_OFFLOAD_INPOS:
		{
			m_strMsg = m_bLangulageEng?_T("Check Motor Move Onload1 Inpos"):_T("检查电机是否移动到下空料盘堆栈位");

			ASSERT(m_ITrayInPosPresent[2] > -1);

			if(IsNoHardware() || CheckInput(INPUTS[m_ITrayInPosPresent[2]],ON))
			{
				Sleep(100);
				if(IsNoHardware() || CheckInput(INPUTS[m_ITrayInPosPresent[2]],ON))
				{
					m_nNextAutoStep = AUTO_FINGLE_CYL_CLAMP_CLOSE;
					break;
				}
			}
			// 搬运模组电机移动到堆栈位，检测到位感应器状态异常(应为ON)，请检查确认
			ShowMessage(MSG_FEEDERTRAY_MOTOR_MOVE_NOT_INPOS);

			break;
		}
		//在下料处托盘气缸松开
	case AUTO_FINGLE_CYL_CLAMP_CLOSE:
		{
			m_strMsg = m_bLangulageEng?_T("Fingle Cyl Clamp"):_T("下空料盘堆栈位，托盘气缸松开");

			if (IsNoHardware() || SeparateClose(FALSE))
			{
				m_nNextAutoStep = AUTO_SEND_OFFLAD_MODUEL_PICK_PALLET;
			}
			break;
		}
		//给下料模组发送接盘信号
	case AUTO_SEND_OFFLAD_MODUEL_PICK_PALLET:
		{
			m_strMsg = m_bLangulageEng?_T("Send Offload Module Pick Pallet"):_T("给下料模组发送接盘信号");

			if(MACHINE.m_pOffloadShellStack != NULL && !MACHINE.m_pOffloadShellStack->m_bOffloadEmptyPallet)
			{
				MACHINE.m_pOffloadShellStack-> m_bOffloadEmptyPallet = TRUE;
				m_nNextAutoStep = AUTO_WAIT_OFFLOAD_MODULE_LIFT_FINISH;
			}
			break;
		}
		//等待下料模组移动到接料位
	case AUTO_WAIT_OFFLOAD_MODULE_LIFT_FINISH:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Offlaod Moduel Lift Finish"):_T("等待下料模组移动到接料位");

			if(MACHINE.m_pOffloadShellStack != NULL && !MACHINE.m_pOffloadShellStack->m_bOffloadEmptyPallet)
			{
				m_nNextAutoStep = AUTO_CLMAP_CYL_OPEN_OFFLOAD;
			}
			break;
		}

		//在下料处真空打开
	case AUTO_CLMAP_CYL_OPEN_OFFLOAD:
		{
			m_strMsg = m_bLangulageEng?_T("Clamp Cyl Open Offload"):_T("在下料处真空打开");

			if(::IsNoHardware() || OpenVacuum(FALSE))
			{
				Sleep(300);
				if (IsNoHardware() ||
					(INPUTS[m_IVacuum[0]]->IsOff() && INPUTS[m_IVacuum[1]]->IsOff() 
					&&INPUTS[m_IVacuum[2]]->IsOff() && INPUTS[m_IVacuum[3]]->IsOff()
					&&INPUTS[m_IVacuum[4]]->IsOff() && INPUTS[m_IVacuum[5]]->IsOff()))
				{
					m_bPutPalletFinish = TRUE;
					m_nNextAutoStep = AUTO_WAIT_OFFLOAD_MOVE_SAFE;	
				}
				else
				{
					ShowMessage(_T("搬运模组料盘没有破真空，请检查"),3);
				}
			}
			break;
		}

		//等待下料模组电机移动到安全位
	case AUTO_WAIT_OFFLOAD_MOVE_SAFE:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Offload Move Safe"):_T("等待下料模组电机移动到安全位");

			if (!m_bPutPalletFinish)
			{
				m_nNextAutoStep = AUTO_CHECK_PALLET_REMOVE;
			}
			break;
		}
	case AUTO_CHECK_PALLET_REMOVE:
		{
			m_strMsg = m_bLangulageEng?_T("Check Pallet Remove"):_T("检测料盘被取走");

			if(::IsNoHardware() || m_bDryRun || (CheckHaveNoPallet()
				&& m_IVacuum[0] > -1 && CheckInput(INPUTS[m_IVacuum[0]], OFF)
				&& m_IVacuum[1] > -1 && CheckInput(INPUTS[m_IVacuum[1]], OFF)
				&&  m_IVacuum[2] > -1 && CheckInput(INPUTS[m_IVacuum[2]], OFF)
				&&  m_IVacuum[3] > -1 && CheckInput(INPUTS[m_IVacuum[3]], OFF)
				&& m_IVacuum[4] > -1 && CheckInput(INPUTS[m_IVacuum[4]], OFF)
				&&  m_IVacuum[5] > -1 && CheckInput(INPUTS[m_IVacuum[5]], OFF)))
			{
				m_nNextAutoStep = AUTO_WORK_END;
			}
			break;
		}
		//工作完成
	case AUTO_WORK_END:
		{
			m_strMsg = m_bLangulageEng?_T("Work end"):_T("工作完成");
			m_nNextAutoStep = AUTO_JUDGE_HAVE_PALLET;
			break;
		}
	default:
		{
			ASSERT(0);
			break;
		}
	}
}

BOOL CRunProcessFeederTray::InitializeConfig(CString strProcessModule)
{
	if (!CRunProcess::InitializeConfig(strProcessModule))
	{
		return FALSE;
	}
	AfxBeginThread(CheckMoveSafe, this);
	return TRUE;
}

void CRunProcessFeederTray::GetHardwareConfig(CArray<int, int> *pInputs, CArray<int, int> *pOutputs, CArray<int, int> *pMotors)
{
	CRunProcess::GetHardwareConfig(pInputs, pOutputs, pMotors);
}

BOOL CRunProcessFeederTray::CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{
#ifdef _TEST_
	return TRUE;
#endif
	CString strKey;
	if (m_nMotorX > -1 && pMotor == MOTORS[m_nMotorX])
	{
		if(MACHINE.m_pOnloadShellPnp != NULL)
		{
			if(!MACHINE.m_pOnloadShellPnp->CheckCanMoveMotor())
			{
				return FALSE;
			}
		}

		for (int i = 0; i < 2; i++)
		{
			if(MACHINE.m_pOnloadShellStack[i] != NULL && !(MACHINE.m_pOnloadShellStack[i])->ModuleIsSafe())
			{
				strKey.Format(_T("(M0%d)外壳上料%dZ轴伺服电机,未在安全位以下"),8+i, 1+i);
				BLMessageBox(strKey, MB_ICONWARNING);
				return FALSE; 
			}
		}

		if(MACHINE.m_pOffloadShellStack != NULL && !MACHINE.m_pOffloadShellStack->ModuleIsSafe())
		{
			BLMessageBox(_T("(M10)空料盘下料Z轴伺服电机未在安全位以下"), MB_ICONWARNING);
			return FALSE; 

		}
	}
	return TRUE;
}

BOOL CRunProcessFeederTray::CheckOutputCanActive(COutput *pOutput)
{
#ifdef _TEST_
	return TRUE;
#endif
	if (m_OSeparateOpen < 0 || pOutput == OUTPUTS[m_OSeparateOpen])
	{
		if (m_IPalletInPosition < 0 || INPUTS[m_IPalletInPosition]->IsOn())
		{
			if (BLMessageBox(_T("X0407工位有料盘感应有料，是否松开，YES ：松开，NO：不松开"),MB_YESNO) == IDNO)
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}

/****************************************************************************************
函数描述：托盘气缸动作
输入参数：bClose:TRUE:托盘气缸夹紧 FALSE:托盘气缸打开
函数返回：TRUE:执行成功，FALSE:执行失败
*****************************************************************************************/
BOOL CRunProcessFeederTray::SeparateClose( BOOL bClose /*= TRUE*/ )
{
	ASSERT(m_ISeparateClose[0] > -1 && m_ISeparateClose[1] > -1 && m_ISeparateOpen[0] > -1
		&& m_ISeparateOpen[1] > -1 && m_OSeparateOpen > -1 && m_OSeparateClose > -1);

	BOOL bRes = FALSE;

	if(bClose)
	{
		OUTPUTS[m_OSeparateOpen]->Off();
		OUTPUTS[m_OSeparateClose]->On();

		if(WAIT_OK == WaitInput(INPUTS[m_ISeparateClose[0]], ON, MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_ISeparateOpen[0]], OFF, MSG_SHOW_DEFAULT)
			&& WAIT_OK == WaitInput(INPUTS[m_ISeparateClose[1]], ON, MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_ISeparateOpen[1]], OFF, MSG_SHOW_DEFAULT))
		{
			bRes = TRUE;
		}
	}
	else
	{
		OUTPUTS[m_OSeparateClose]->Off();
		OUTPUTS[m_OSeparateOpen]->On();

		if(WAIT_OK == WaitInput(INPUTS[m_ISeparateOpen[0]], ON, MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_ISeparateClose[0]], OFF, MSG_SHOW_DEFAULT)
			&& WAIT_OK == WaitInput(INPUTS[m_ISeparateOpen[1]], ON, MSG_SHOW_DEFAULT)
			&& WAIT_OK == WaitInput(INPUTS[m_ISeparateClose[1]], OFF, MSG_SHOW_DEFAULT)) 
		{
			bRes = TRUE;
		}
	}
	return bRes;
}

/****************************************************************************************
函数描述：判断没有料盘
输入参数：
函数返回：TRUE:执行成功，FALSE:执行失败
*****************************************************************************************/
BOOL CRunProcessFeederTray::CheckHaveNoPallet()
{	
	ASSERT(m_IPalletInPosition > -1);

	BOOL bCheckRes = TRUE;

	for(int i = 0; i < 3; i++)
	{
		if(INPUTS[m_IPalletInPosition]->IsOn())
		{
			bCheckRes = FALSE;
			break;
		}
		Sleep(100);
	}

	return bCheckRes;
}

/****************************************************************************************
函数描述：判断其他模组是否安全
输入参数：
函数返回：TRUE:安全，FALSE:不安全
*****************************************************************************************/
BOOL CRunProcessFeederTray:: CheckCanHome(void)
{
	BOOL bTrue[4] = {FALSE, FALSE, FALSE, FALSE};

	if(MACHINE.m_pOnloadShellStack[0] != NULL && (MACHINE.m_pOnloadShellStack[0]->m_bHomeMoveSafePos || !MACHINE.m_pOnloadShellStack[0]->IsModuleEnable()))
	{// 铝壳堆栈1
		bTrue[0] = TRUE;
	}

	if(MACHINE.m_pOnloadShellStack[1] != NULL && (MACHINE.m_pOnloadShellStack[1]->m_bHomeMoveSafePos || !MACHINE.m_pOnloadShellStack[1]->IsModuleEnable()))
	{// 铝壳堆栈2
		bTrue[1] = TRUE;
	}

	if(MACHINE.m_pOffloadShellStack != NULL && (MACHINE.m_pOffloadShellStack->m_bHomeMoveSafePos || !MACHINE.m_pOffloadShellStack->IsModuleEnable()))
	{// 铝壳下料堆栈
		bTrue[2] = TRUE;
	}

	if(MACHINE.m_pOnloadShellPnp != NULL && (MACHINE.m_pOnloadShellPnp->m_bHomeMoveSafePos || !MACHINE.m_pOnloadShellPnp->IsModuleEnable()))
	{//取铝壳
		bTrue[3] = TRUE;
	}

	return (bTrue[0] && bTrue[1] && bTrue[2]  && bTrue[3]);
}

/****************************************************************************************
函数描述：重置复位信号
输入参数：
函数返回：
*****************************************************************************************/
void CRunProcessFeederTray:: ResertHomeSingle(void)
{
	if(MACHINE.m_pOnloadShellStack[0] != NULL)
	{
		MACHINE.m_pOnloadShellStack[0]->m_bHomeMoveSafePos = FALSE;
	}

	if(MACHINE.m_pOnloadShellStack[1] != NULL)
	{
		MACHINE.m_pOnloadShellStack[1]->m_bHomeMoveSafePos = FALSE;
	}

	if(MACHINE.m_pOffloadShellStack != NULL)
	{
		MACHINE.m_pOffloadShellStack->m_bHomeMoveSafePos = FALSE;
	}

	if(MACHINE.m_pOnloadShellPnp != NULL)
	{
		MACHINE.m_pOnloadShellPnp->m_bHomeMoveSafePos = FALSE;
	}
}

/****************************************************************************************
函数描述：打开真空
输入参数：bOpen:TRUE:打开真空 FALSE:关闭真空
函数返回：TRUE:执行成功，FALSE:执行失败
*****************************************************************************************/
BOOL CRunProcessFeederTray::OpenVacuum(BOOL bOpen)
{
	int nVacuumCount = 6;
	for(int i = 0; i < nVacuumCount; i ++)
	{
		ASSERT(m_IVacuum[i] > -1  && m_OVacuum[i] > -1);
	}
	

	BOOL bRes = FALSE;
	if(bOpen)
	{
		for(int i =0; i < nVacuumCount; i ++)
		{
			OUTPUTS[m_OVacuum[i]]->On();
		}
		
		if (m_bDryRun)
		{
			Sleep(500);
		}

		if (m_bDryRun
			|| (WAIT_OK == WaitInput(INPUTS[m_IVacuum[0]], ON, MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_IVacuum[1]], ON, MSG_SHOW_DEFAULT)
			&& WAIT_OK == WaitInput(INPUTS[m_IVacuum[2]], ON, MSG_SHOW_DEFAULT)
			&& WAIT_OK == WaitInput(INPUTS[m_IVacuum[3]], ON, MSG_SHOW_DEFAULT)
			&& WAIT_OK == WaitInput(INPUTS[m_IVacuum[4]], ON, MSG_SHOW_DEFAULT)
			&& WAIT_OK == WaitInput(INPUTS[m_IVacuum[5]], ON, MSG_SHOW_DEFAULT)))
		{
			bRes = TRUE;
		}
	}
	else
	{
		for(int i =0; i < nVacuumCount; i ++)
		{
			OUTPUTS[m_OVacuum[i]]->Off();
		}
		

		if (WAIT_OK == WaitInput(INPUTS[m_IVacuum[0]], OFF, MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_IVacuum[1]], OFF, MSG_SHOW_DEFAULT)
			&& WAIT_OK == WaitInput(INPUTS[m_IVacuum[2]], OFF, MSG_SHOW_DEFAULT)
			&& WAIT_OK == WaitInput(INPUTS[m_IVacuum[3]], OFF, MSG_SHOW_DEFAULT)
			&& WAIT_OK == WaitInput(INPUTS[m_IVacuum[4]], OFF, MSG_SHOW_DEFAULT)
			&& WAIT_OK == WaitInput(INPUTS[m_IVacuum[5]], OFF, MSG_SHOW_DEFAULT))
		{
			bRes = TRUE;
		}
	}

	return bRes;
}

/****************************************************************************************
函数描述：程序运行时一直监控是否安全
输入参数：
函数返回：
*****************************************************************************************/
void CRunProcessFeederTray::WorkWhile()
{
	if(MACHINE.m_pRunCtrl == NULL)
	{
		return;
	}
	if (IsNoHardware())
	{
		return;
	}
	ASSERT(m_ISafeRasterCheck[0] > -1 && m_ISafeRasterCheck[1] > -1&& m_ISafeRasterCheck[2] > -1 && m_ISafeCheckPallet > -1);
	int nStatues = MACHINE.m_pRunCtrl->GetMcState();
	if( !MACHINE.m_bSafeStatckDoor[0] && !MACHINE.m_bSafeStatckDoor[1]  && !MACHINE.m_bSafeStatckDoor[2] && (INPUTS[m_ISafeRasterCheck[0]]->IsOff() || INPUTS[m_ISafeRasterCheck[1]]->IsOff()|| INPUTS[m_ISafeRasterCheck[2]]->IsOff())
		&& (nStatues == MC_RUNNING || nStatues == MC_INITIALIZING))
		//光栅感应到信号后处理,上下料时不检测
	{
		if (m_nMotorX > -1)
		{
			MOTORS[m_nMotorX] -> Stop(1);
		}

		if(MACHINE.m_pOnloadShellStack[0] !=NULL && MACHINE.m_pOnloadShellStack[0]->m_nMotorZ > -1)
		{
			MOTORS[MACHINE.m_pOnloadShellStack[0]->m_nMotorZ]->Stop(1);
		}

		if(MACHINE.m_pOnloadShellStack[1] !=NULL && MACHINE.m_pOnloadShellStack[1]->m_nMotorZ > -1)
		{
			MOTORS[MACHINE.m_pOnloadShellStack[1]->m_nMotorZ]->Stop(1);
		}

		if(MACHINE.m_pOffloadShellStack !=NULL && MACHINE.m_pOffloadShellStack->m_nMotorZ > -1)
		{
			MOTORS[MACHINE.m_pOffloadShellStack->m_nMotorZ]->Stop(1);
		}

		if(MACHINE.m_pOnloadShellPnp != NULL && MACHINE.m_pOnloadShellPnp->m_nMotorZ > -1)
		{
			MOTORS[MACHINE.m_pOnloadShellPnp->m_nMotorZ]->Stop(1);
		}

		if(MACHINE.m_pOnloadShellPnp != NULL && MACHINE.m_pOnloadShellPnp->m_nMotorY > -1)
		{
			MOTORS[MACHINE.m_pOnloadShellPnp->m_nMotorY]->Stop(1);
		}
		//铝壳堆栈模块检测安全光栅被触发，请注意安全
		ShowMessage(MSG_FEEDERTRAY_SAFE_RASTER_CHECK_UNNOTMAL);
	}

	if(INPUTS[m_ISafeCheckPallet]->IsOff())
	{
		Sleep(50);
		if(INPUTS[m_ISafeCheckPallet]->IsOff())
		{
			if (m_nMotorX > -1)
			{
				MOTORS[m_nMotorX] -> Stop();
			}

			if(MACHINE.m_pOnloadShellStack[0] !=NULL && MACHINE.m_pOnloadShellStack[0]->m_nMotorZ > -1)
			{
				MOTORS[MACHINE.m_pOnloadShellStack[0]->m_nMotorZ]->Stop(1);
			}

			if(MACHINE.m_pOnloadShellStack[1] !=NULL && MACHINE.m_pOnloadShellStack[1]->m_nMotorZ > -1)
			{
				MOTORS[MACHINE.m_pOnloadShellStack[1]->m_nMotorZ]->Stop(1);
			}

			if(MACHINE.m_pOffloadShellStack !=NULL && MACHINE.m_pOffloadShellStack->m_nMotorZ > -1)
			{
				MOTORS[MACHINE.m_pOffloadShellStack->m_nMotorZ]->Stop(1);
			}

			if(MACHINE.m_pOnloadShellPnp != NULL && MACHINE.m_pOnloadShellPnp->m_nMotorZ > -1)
			{
				MOTORS[MACHINE.m_pOnloadShellPnp->m_nMotorZ]->Stop(1);
			}

			if(MACHINE.m_pOnloadShellPnp != NULL && MACHINE.m_pOnloadShellPnp->m_nMotorY > -1)
			{
				MOTORS[MACHINE.m_pOnloadShellPnp->m_nMotorY]->Stop(1);
			}

			MACHINE.m_pRunCtrl->Stop();

			if(BLMessageBox(_T("防撞感应器亮了，请确定是否撞机"),MB_YESNO|MB_ICONWARNING) == IDYES)
			{
				if (MOTORS[m_nMotorX]->GetCurLoc() == X_MOTOR_ONLOAD_LOC1)
				{// 确定撞击以后堆栈电机复位
					if(MACHINE.m_pOnloadShellStack[0] != NULL && MACHINE.m_pOnloadShellStack[0]->m_nMotorZ > -1)
					{
						HomeMotor(MOTORS[MACHINE.m_pOnloadShellStack[0]->m_nMotorZ]);
					}
				}

				if (MOTORS[m_nMotorX]->GetCurLoc() == X_MOTOR_ONLOAD_LOC2)
				{// 确定撞击以后堆栈电机复位
					if(MACHINE.m_pOnloadShellStack[1] != NULL && MACHINE.m_pOnloadShellStack[1]->m_nMotorZ > -1)
					{
						HomeMotor(MOTORS[MACHINE.m_pOnloadShellStack[1]->m_nMotorZ]);
					}
				}

				if (MOTORS[m_nMotorX]->GetCurLoc() == X_MOTOR_OFFLOAD_LOC)
				{
					if(MACHINE.m_pOffloadShellStack !=NULL && MACHINE.m_pOffloadShellStack->m_nMotorZ > -1)
					{
						HomeMotor(MOTORS[MACHINE.m_pOffloadShellStack->m_nMotorZ]);
					}
				}
			}
		}
	}
}

void CRunProcessFeederTray::Destory()
{
	m_bThreadStart = FALSE;

	DWORD dwTime = GetTickCount();

	while(!m_bThreadFinished)
	{
		if (GetTickCount() - dwTime > 20000)
		{
			break;
		}
		Sleep(1);
	}
}

/****************************************************************************************
函数描述：判断料盘搬运模组状态
输入参数：
函数返回：
*****************************************************************************************/
BOOL CRunProcessFeederTray:: CheckPalletInPos()
{
	if(m_IPalletInPosition  < 0 )
	{
		return FALSE;
	}
	BOOL m_bCheckCore = FALSE;
	DWORD m_WordTime = GetTickCount();
	for (;;)
	{
		if (GetTickCount() - m_WordTime > 15000)
		{
			BLMessageBox(_T("上料堆栈料盘传送超时！"));
			break;
		}

		if(INPUTS[m_IPalletInPosition]->IsOn())//表示有托盘
		{
			m_bCheckCore = TRUE;
			break;
		}

	}
	return m_bCheckCore;
}

//读取带料启动数据
void CRunProcessFeederTray::ReadStepInfo()
{
	CString strPath = GetAbsolutePathName(MODULE_RUN_FOLDER) + m_strName + _T(".cfg");

	m_nMoveMotorIndex = IniReadInt(_T("Data"), _T("m_nMoveMotorIndex"), 0, strPath);

	return ;
}

//保存带料启动数据
BOOL CRunProcessFeederTray::SaveStepInfo(int nWriteType)
{
	CString strPath = GetAbsolutePathName(MODULE_RUN_FOLDER) + m_strName + _T(".cfg");

	IniWriteInt(_T("Data"), _T("m_nMoveMotorIndex"), m_nMoveMotorIndex, strPath);
	return TRUE;
}