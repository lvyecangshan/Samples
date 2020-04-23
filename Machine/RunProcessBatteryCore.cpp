/****************************************************************************************
文件名：RunProcessBatteryCore.cpp
功能描述：入电芯工位
历史修改记录：
*****************************************************************************************/
#include "StdAfx.h"
#include "RunProcessBatteryCore.h"
#include "locale.h"
#include "imagehlp.h"


UINT BatteryCoreThread(void* lpVoid)
{
	CRunProcessBatteryCore* pRun = (CRunProcessBatteryCore*)lpVoid;
	if (pRun == NULL)
	{
		return 0;
	}

	pRun->m_bThreadStart = TRUE;
	pRun->m_bThreadEnd = FALSE;
	while(pRun->m_bThreadStart)
	{
		pRun->RunWhile();
		Sleep(1);
	}

	pRun->m_bThreadEnd = TRUE;
	return 0;
}

CRunProcessBatteryCore::CRunProcessBatteryCore(int nID) :
CRunProcess(nID)
{
	CString strKey;

	PowerUpRestart();

	for(int i = 0; i < 2; i++)
	{
		strKey.Format(_T("IColumnCylOpen%d"), i);
		m_InputMap.insert(pair<int*, CString>(&m_IColumnCylOpen[i], strKey));

		strKey.Format(_T("IColumnCylClose%d"), i);
		m_InputMap.insert(pair<int*, CString>(&m_IColumnCylClose[i], strKey));

		strKey.Format(_T("IOpenCylOpen%d"), i);
		m_InputMap.insert(pair<int*, CString>(&m_IOpenCylOpen[i],strKey));

		strKey.Format(_T("IOpenCylClose%d"), i);
		m_InputMap.insert(pair<int*, CString>(&m_IOpenCylClose[i], strKey));

		strKey.Format(_T("IOpenCylOpenTwo%d"), i);
		m_InputMap.insert(pair<int*, CString>(&m_IOpenCylOpenTwo[i], strKey));  //开合气缸关闭二次确认感应
	}

	m_InputMap.insert(pair<int*, CString>(&m_ILiftCylUp,_T("ILiftCylUp")));
	m_InputMap.insert(pair<int*, CString>(&m_ILiftCylDown, _T("ILiftCylDown")));
	m_OutputMap.insert(pair<int*, CString>(&m_OLiftCylUp, _T("OLiftCylUp")));
	m_OutputMap.insert(pair<int*, CString>(&m_OLiftCylDown, _T("OLiftCylDown")));

	m_InputMap.insert(pair<int*, CString>(&m_ICheckVacum,_T("ICheckVacum")));
	m_InputMap.insert(pair<int*, CString>(&m_IHaveCore, _T("IHaveCore")));
	m_InputMap.insert(pair<int*, CString>(&m_IReachOpenAndClosePos, _T("IReachOpenAndClosePos")));


	m_OutputMap.insert(pair<int*, CString>(&m_OSetZero, _T("OSetZero")));
	m_OutputMap.insert(pair<int*, CString>(&m_OColumnCylClose, _T("OColumnCylClose")));
	m_OutputMap.insert(pair<int*, CString>(&m_OColumnCylOpen, _T("OColumnCylOpen")));

	m_OutputMap.insert(pair<int*, CString>(&m_OOpenVacum, _T("OOpenVacum")));
	m_OutputMap.insert(pair<int*, CString>(&m_OOpenCylOpen, _T("OOpenCylOpen")));
	m_OutputMap.insert(pair<int*, CString>(&m_OOpenCylClose, _T("OOpenCylClose")));	

	m_MotorMap.insert(pair<int*, CString>(&m_MotorX, _T("MotorX")));

	m_VoidMap.insert(pair<void*, CString>(&m_bEnablePressSenseClient, _T("bEnablePressSenseClient, 是否开启压力仪,TRUE开启，FALSE关闭")));
	m_VoidMap.insert(pair<void*, CString>(&m_strPressSenseIP, _T("sPressSenseIP, 压力仪通讯IP")));
	m_VoidMap.insert(pair<void*, CString>(&m_nPressSensePort, _T("lPressSensePort, 压力仪通讯端口")));
}

CRunProcessBatteryCore::~CRunProcessBatteryCore(void)
{
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
	m_PressClient.Disconnect();
}

void CRunProcessBatteryCore::PowerUpRestart()
{
	CRunProcess::PowerUpRestart();
	m_strMsg = m_bLangulageEng?_T("Ready"):_T("准备好");

	m_bDataJudgeFinish = FALSE;
	m_bResetJudge = FALSE;
	m_bPowerUpFinish = FALSE;

	m_bRequireCore = FALSE;
	m_bOpenLoc = FALSE;
	m_bStartGetValue =FALSE;
	m_bGetValueFinish = FALSE;
	m_nTryReadTimes = 0;
	m_nStartWithPalletData = -1;

	m_nNextPowerUpStep = PU_CHECK_ONLOAD_CORE_HOME_FINISH;
	m_nNextAutoStep = AUTO_SEND_MOTOR_TO_WAIT_POS;
}



void CRunProcessBatteryCore::PowerUpOperation(void)
{
	if (!IsModuleEnable())
	{
		CRun::PowerUpOperation();
		m_strMsg = m_bLangulageEng?_T("Moudle UnEnable"):_T("模块禁用");
		return;
	}

	switch(m_nNextPowerUpStep)
	{
	case PU_CHECK_ONLOAD_CORE_HOME_FINISH:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Onload Core Home Finish"):_T("等待上电芯模组复位完成");
			if(MACHINE.m_pOnloadCore != NULL && (MACHINE.m_pOnloadCore->m_bHomeMoveSafePos || !MACHINE.m_pOnloadCore->IsModuleEnable()))
			{
				m_PressClient.Disconnect();
				Sleep(2000);
				m_nNextPowerUpStep = PU_OPEN_PRESS_COMPORT;
			}
			break;
		}
	case PU_OPEN_PRESS_COMPORT:
		{
			m_strMsg = m_bLangulageEng?_T("Open Comport"):_T("连接压力测试仪");

			if (IsNoHardware() || !m_bEnablePressSenseClient || m_PressClient.Connect(m_strPressSenseIP, m_nPressSensePort))
			{
				m_nNextPowerUpStep = PU_COLUMNCYL_OPEN;
			}
			else
			{
				m_PressClient.Disconnect();
				Sleep(2000);
			}
			break;
		}
	
		//整列气缸打开
	case PU_COLUMNCYL_OPEN:
		{
			m_strMsg = m_bLangulageEng?_T("ColumnCyl Open"):_T("整列气缸打开");

			if(::IsNoHardware() ||  ColumnCylClose( FALSE))
			{
				m_nNextPowerUpStep = PU_OPEN_CYL_OPEN;
			}
			break;
		}

		//开合气缸打开
	case PU_OPEN_CYL_OPEN:
		{
			m_strMsg = m_bLangulageEng?_T("Open Cyl Close"):_T("开合气缸关闭");

			if(::IsNoHardware() || OpenCylOpen(FALSE))
			{
				m_nNextPowerUpStep = PU_LIFT_CLY_DOWN;
			}
			break;
		}
	case PU_LIFT_CLY_DOWN:
		{
			m_strMsg = m_bLangulageEng?_T("Open Cyl Close"):_T("顶升气缸下降");
			if(IsNoHardware() || LiftCylUp(FALSE))
			{
				m_nNextPowerUpStep=PU_CLOSE_VACUM;
			}
			break;
		}
		//真空关闭
	case PU_CLOSE_VACUM:
		{
			m_strMsg = m_bLangulageEng?_T("Close Vacum"):_T("真空关闭");

			if(m_OOpenVacum > -1)
			{
				OUTPUTS[m_OOpenVacum]->Off();
			}
			m_nNextPowerUpStep = PU_SEND_MOTOR_HOME;
			break;
		}

		//送料电机复位
	case PU_SEND_MOTOR_HOME:
		{
			m_strMsg = m_bLangulageEng?_T("Send Motor Home"):_T("送料电机复位");

			if (m_MotorX > -1 && HomeMotor(MOTORS[m_MotorX]))
			{
				m_nNextPowerUpStep = PU_SEND_MOTOR_MOVE_TO_WAIT_POS;
			}
			break;
		}

		//送料电机移动到开始位
	case PU_SEND_MOTOR_MOVE_TO_WAIT_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Send Motor Move To Wait Pos"):_T("送料电机移动到开始位");

			if (m_MotorX > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_MotorX, SEND_MOTOR_WAIT_LOC))
			{
				m_nNextPowerUpStep = PU_ZERO_PRESS_DATA;
			}
			break;
		}

		//数据重置
	case PU_ZERO_PRESS_DATA:
		{
			m_strMsg = m_bLangulageEng?_T("Data Release"):_T("数据重置");
			if(m_OSetZero > -1)
			{
				OUTPUTS[m_OSetZero]->On();
				Sleep(200);
				OUTPUTS[m_OSetZero]->Off();
			}
			m_PressDataLock.Lock();
			m_vecPressValue.clear();
			m_PressDataLock.Unlock();

			m_nNextPowerUpStep = PU_JUDGE_IF_WITH_PALLET;
			break;
		}
		//带料复位 须在电机复位之后防止感应器误判
	case PU_JUDGE_IF_WITH_PALLET:  
		{
			m_strMsg = m_bLangulageEng?_T("Check Recover data"):_T("带料复位");
			if(MACHINE.m_bReCoverMachine)
			{
				ReadStepInfo();
				m_nNextPowerUpStep = PU_CHECK_PALLET_DATA;
			}
			else
			{
				if(::IsNoHardware() || (m_IHaveCore > -1 && CheckInput(INPUTS[m_IHaveCore], OFF)))
				{
					m_Pallet.Release();
					m_nNextPowerUpStep = PU_CHECK_WITHOUT_CORE;
				}
			}
			break;
		}

		//检测有无电芯
	case PU_CHECK_WITHOUT_CORE:
		{
			m_strMsg = m_bLangulageEng?_T("Check Without Shell"):_T("检测无壳");

			if (IsNoHardware() || (m_IHaveCore > -1 && CheckInput(INPUTS[m_IHaveCore], OFF)))
			{
				m_nNextPowerUpStep = PU_FINISH;
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
					m_nNextAutoStep = AUTO_SEND_MOTOR_TO_WAIT_POS;
					m_nNextPowerUpStep = PU_WAIT_JUDGE_FINISH;
				}
				else if(m_nStartWithPalletData == 1)
				{
					m_nNextAutoStep = AUTO_CHECK_HAE_PRODUCT;
					m_nNextPowerUpStep = PU_WAIT_JUDGE_FINISH;
				}
				m_bDataJudgeFinish = TRUE;
			}
			else
			{
				m_nNextPowerUpStep = PU_CHECK_PALLET_DATA;
			}
			break;
		}

	case PU_WAIT_JUDGE_FINISH:
		{
			m_strMsg = m_bLangulageEng?_T("Check Without Shell"):_T("等待另一个入电芯模组数据判断完成");
			int nID = m_nGroupID == 0?1:0;
			if (m_bDataJudgeFinish && MACHINE.m_pBatteryCore[nID]->m_bDataJudgeFinish)
			{   
				m_bResetJudge = TRUE;
				
				m_nNextPowerUpStep = PU_SAME_STATION_DATA_PAIR;
			}
			break;
		}

		//检测双工位数据是否一致
	case PU_SAME_STATION_DATA_PAIR:
		{
			m_strMsg = m_bLangulageEng?_T("Move XY To Onload Pos"):_T("检测双工位数据是否一致");
		
			int nID = m_nGroupID == 0?1:0;
			ASSERT(MACHINE.m_pBatteryCore[nID] != NULL);
			
			if (m_bResetJudge && MACHINE.m_pBatteryCore[nID]->m_bResetJudge)
			{
				m_bDataJudgeFinish = FALSE;
			}
			else
			{
				break;
			}

			if(m_nStartWithPalletData == 0 && MACHINE.m_pBatteryCore[nID]->m_nStartWithPalletData == 0
				|| m_nStartWithPalletData == 1 && MACHINE.m_pBatteryCore[nID]->m_nStartWithPalletData == 1)
			{
				m_nNextPowerUpStep = PU_FINISH;
			}
			else 
			{
				m_Pallet.Release();
				MACHINE.m_pBatteryCore[nID]->m_Pallet.Release();

				BLMessageBox(_T("两个入电芯工位数据不一致，清除数据，请人工清除电芯"), MB_ICONWARNING);
				m_nNextPowerUpStep = PU_CHECK_PALLET_DATA;
			}
			break;
		}

		//初始化完成
	case PU_FINISH:
		{
			CRun::PowerUpOperation();
			m_strMsg = m_bLangulageEng?_T("Power up finished"):_T("初始化完成");
			m_bPowerUpFinish = TRUE;
			break;
		}
	default:
		{
			ASSERT(0);
			break;
		}
	}
}

void CRunProcessBatteryCore::AutoOperation()
{
	if (!IsModuleEnable())
	{
		m_strMsg = m_bLangulageEng?_T("Moudle UnEnable"):_T("模块禁用");
		Sleep(500);
		return;
	}

	if (IsNoHardware())
	{
		Sleep(2000);
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
		if (m_nSaveLastStep == AUTO_SEND_MOTOR_TO_WAIT_POS)
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
		//送料电机到等待位
	case AUTO_SEND_MOTOR_TO_WAIT_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Send Motor To Wait Pos"):_T("送料电机到等待位");
			if(m_MotorX > -1)
			{
				MOTORS[m_MotorX]->m_dAloneSpeedRatio=1;
			}
			if(m_MotorX > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_MotorX, SEND_MOTOR_WAIT_LOC))
			{
				m_nNextAutoStep = AUTO_SEND_NEED_CORE_REQUIRE;
			}
			break;
		}

		//发送要料信号
	case AUTO_SEND_NEED_CORE_REQUIRE:
		{
			m_strMsg = m_bLangulageEng?_T("Send Need Core Require"):_T("向电芯机械手发送要料信号");
			if(!m_bRequireCore || (MACHINE.m_pOnloadCore != NULL && !MACHINE.m_pOnloadCore->IsModuleEnable()))
			{

				m_bRequireCore = TRUE;
				m_nNextAutoStep = AUTO_ClEAR_PRESS;
			}
			break;
		}

		//清零压力值
	case AUTO_ClEAR_PRESS:       
		{
			m_strMsg = m_bLangulageEng?_T("Clear Press"):_T("清零压力值");

			if(m_OSetZero > -1)//压力表归零
			{
				Sleep(1000);
				OUTPUTS[m_OSetZero]->On();
				Sleep(500);
				OUTPUTS[m_OSetZero]->Off();
				
			}
			
			m_nNextAutoStep = AUTO_START_EVENT;
			break;
		}

		//等待开始信号
	case AUTO_START_EVENT:
		{
			m_strMsg = m_bLangulageEng?_T("Start Event"):_T("等待开始信号");

		
			if (!m_bRequireCore || (MACHINE.m_pOnloadCore != NULL && !MACHINE.m_pOnloadCore->IsModuleEnable()))
			{
				m_nNextAutoStep = AUTO_CHECK_HAE_PRODUCT;
				SaveStepInfo(WRITE_TYPE_AUTO_STEP|WRITE_TYPE_BATTERY);
			}
			break;
		}

		//检查产品是否到位
	case AUTO_CHECK_HAE_PRODUCT:
		{
			m_strMsg = m_bLangulageEng?_T("Check Have Product"):_T("检查产品到位");
			
			if (IsNoHardware() || m_bDryRun || (m_IHaveCore >-1 && CheckInput(INPUTS[m_IHaveCore], ON)))
			{
				m_nNextAutoStep = AUTO_CLUMMN_CYL_CLOSE;
			}
			break;
		}

		//整列气缸夹紧
	case AUTO_CLUMMN_CYL_CLOSE:
		{
			m_strMsg = m_bLangulageEng?_T("Clumn Cyl Close"):_T("整列气缸夹紧");

			if(::IsNoHardware()  ||  ColumnCylClose( TRUE))
			{
				m_nNextAutoStep = AUTO_SEND_MOTOR_TO_PICK_POS;
			}			
			break;
		}

		//移动电机到接电芯位
	case AUTO_SEND_MOTOR_TO_PICK_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Send Motor To Pick Pos"):_T("移动电机到接电芯位");
			if(m_MotorX > -1&& MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_MotorX, SEND_MOTOR_PICK_POS))					
			{
				if(m_OOpenVacum > -1)
				{
					OUTPUTS[m_OOpenVacum]->On();
					Sleep(200);
				}
				m_nNextAutoStep = AUTO_CEHCK_VACUM_STATUS;
			}
			break;
		}
		
		//检测顶升真空状态
	case AUTO_CEHCK_VACUM_STATUS:
		{
			m_strMsg = m_bLangulageEng?_T("Check Vacum Status"):_T("检测真空状态");

			if(m_OOpenVacum > -1 && OUTPUTS[m_OOpenVacum]->IsOff())
			{
				OUTPUTS[m_OOpenVacum]->On();
			}

			if(IsNoHardware() || m_bDryRun || (m_ICheckVacum > -1 && WAIT_OK == WaitInput(INPUTS[m_ICheckVacum], ON, MSG_SHOW_DEFAULT)))
			{
				m_nNextAutoStep = AUTO_CLUMN_CYL_OPEN;
			}
			break;
		}
		//整列气缸打开
	case AUTO_CLUMN_CYL_OPEN:
		{
			m_strMsg = m_bLangulageEng?_T("Clumn Cyl Open"):_T("整列气缸打开");

			if(::IsNoHardware()  || ColumnCylClose( FALSE))
			{	
				m_nNextAutoStep = AUTO_LIFT_CYL_UP;	
			}
			break;
		}
		//顶升气缸上升
	case AUTO_LIFT_CYL_UP:
		{
			m_strMsg = m_bLangulageEng?_T("Clumn Cyl Open"):_T("顶升气缸上升");
			if (IsNoHardware() || LiftCylUp(TRUE))
			{
				if(m_OSetZero > -1)//压力表归零
				{
					OUTPUTS[m_OSetZero]->On();
					Sleep(500);
					OUTPUTS[m_OSetZero]->Off();
				}
				m_PressDataLock.Lock();
				m_vecPressValue.clear();
				m_PressDataLock.Unlock();

				m_bGetValueFinish = FALSE;
				m_nNextAutoStep = AUTO_JUDGE_HAVE_SHELL;
			}
			break;
		}
	
		//判断壳是否到位
	case AUTO_JUDGE_HAVE_SHELL:
		{
			m_strMsg = m_bLangulageEng?_T("Judge Have Shell"):_T("判断铝壳准备好");

			ASSERT(0 <= m_nGroupID && m_nGroupID <= 1);
			if(MACHINE.m_pBatteryInShell[m_nGroupID] !=NULL
				&& (MACHINE.m_pBatteryInShell[m_nGroupID]->m_bShellReady || !MACHINE.m_pBatteryInShell[m_nGroupID]->IsModuleEnable()))//有壳
			{
				m_nNextAutoStep = AUTO_SEND_MOTOR_TO_OPEN_CYL_OPEN_POS;
			}
			else
			{
				m_nNextAutoStep = AUTO_SEND_MOTOR_TO_INSHELL_WAIT_POS;
			}
			break;
		}

		//移动电机到入壳等待位
	case AUTO_SEND_MOTOR_TO_INSHELL_WAIT_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Send Motor To Wait Pos"):_T("移动电机到入壳等待位");

			if(m_MotorX > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_MotorX, SEND_MOTOR_INSHELL_WAIT_LOC))
			{
				m_nNextAutoStep = AUTO_WAIT_SHELL_READY;
			}
			break;
		}

		//在等待位等待壳准备好
	case AUTO_WAIT_SHELL_READY:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Shell Ready"):_T("在等待位等待壳准备好");
			ASSERT(0 <= m_nGroupID && m_nGroupID <= 1);
			if (MACHINE.m_pBatteryInShell[m_nGroupID]!= NULL && (MACHINE.m_pBatteryInShell[m_nGroupID]->m_bShellReady || !MACHINE.m_pBatteryInShell[m_nGroupID]->IsModuleEnable()))
			{
				m_nNextAutoStep = AUTO_CHECK_OPEN_CYL_CLOSE;
			}
			break;
		}

	case AUTO_CHECK_OPEN_CYL_CLOSE: 
		{
			m_strMsg = m_bLangulageEng?_T("Check Open Cyl Close"):_T("检查开合气缸关闭");

			if (IsNoHardware() || CheckCylClose()) //入电芯前再次确认开合气缸有没有合紧
			{
				m_nNextAutoStep = AUTO_SEND_MOTOR_TO_OPEN_CYL_OPEN_POS;
			}
			break;
		}

		//移动电机到开合气缸打开位
	case AUTO_SEND_MOTOR_TO_OPEN_CYL_OPEN_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Send Motor To Open Cyl Open Pos"):_T("移动电机到开合气缸打开位");

			m_bStartGetValue = TRUE;
			if(m_MotorX > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_MotorX, SEND_MOTOR_OPEN_CYL_OPEN_LOC))
			{
				m_nNextAutoStep = AUTO_TWICE_CLY_RETURN;
			}
			break;
		}
		//控制入铝壳二次气缸回退
	case AUTO_TWICE_CLY_RETURN:
		{
			m_strMsg = m_bLangulageEng?_T("Open Cyl Open"):_T("控制入铝壳二次气缸回退");
			ASSERT(0 <= m_nGroupID && m_nGroupID <= 1);
			if (MACHINE.m_pBatteryInShell[m_nGroupID] != NULL && MACHINE.m_pBatteryInShell[m_nGroupID]->SendTwoCylPush(FALSE)
				&& MACHINE.m_pBatteryInShell[m_nGroupID]->LocateCylPush(FALSE))
			{
				m_nNextAutoStep =AUTO_OPEN_CYL_OPEN;
			}
			break;
		}
		//开合气缸打开
	case AUTO_OPEN_CYL_OPEN:
		{
			m_strMsg = m_bLangulageEng?_T("Open Cyl Open"):_T("开合气缸打开，顶升气缸回退");

			if (IsNoHardware() || (OpenCylOpen(TRUE) && LiftCylUp(FALSE)))
			{
				m_nNextAutoStep = AUTO_CHECK_OPEN_CYL_OPEN; 
			}
			break;
		}

		//检查开合气缸打开
	case AUTO_CHECK_OPEN_CYL_OPEN:
		{
			m_strMsg = m_bLangulageEng?_T("Check Open Cyl Close"):_T("检查开合气缸打开");

			if (IsNoHardware() || CheckCylOpen(TRUE))
			{
				m_bOpenLoc = TRUE;
				m_nNextAutoStep = AUTO_SEND_MOTOR_TO_INSHELL_POS;
				SaveStepInfo(WRITE_TYPE_AUTO_STEP|WRITE_TYPE_VARIABLES);
			}
			break;
		}

		//移动电机到入壳完成位
	case AUTO_SEND_MOTOR_TO_INSHELL_POS:
		{
			m_strMsg = m_bLangulageEng?_T("Send Motor To InShell Pos"):_T("移动电机到入壳完成位");
			if (!m_bOpenLoc || (MACHINE.m_pBatteryInShell[m_nGroupID] != NULL && !MACHINE.m_pBatteryInShell[m_nGroupID]->IsModuleEnable()))
			{
				if(m_MotorX > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_MotorX, SEND_MOTOR_INSHELL_LOC))
				{
					m_bStartGetValue = FALSE;//在开合位结束取值
					m_nNextAutoStep = AUTO_ANALYZE_PREESS_DATA;
				}
			}
			break;
		}
		//分析压力数据
	case AUTO_ANALYZE_PREESS_DATA:
		{
			m_strMsg = m_bLangulageEng?_T("Analyze Press Data"):_T("分析压力数据");
			if(!m_bGetValueFinish)
				break;
			if (m_bDryRun)
			{
				m_vecPressValue.clear();
				m_nNextAutoStep = AUTO_SEND_INSHELL_FINISH_SINGLE;
				break;
			}

			ASSERT(0 <= m_nGroupID && m_nGroupID <= 1);

			m_Pallet.m_dLimitPressValue = GetPressMaxValue();
			

			if(!IsNoHardware() && (m_Pallet.m_dLimitPressValue < m_fPressNormValue[0] || m_Pallet.m_dLimitPressValue > m_fPressNormValue[1]))
			{
				MACHINE.m_nNgCount++;
				m_Pallet.m_bIsNg = TRUE;
			}

			m_Pallet.m_nShellID = (m_nGroupID + 1);
			m_Pallet.m_dPressNormValue[0] = m_fPressNormValue[0];
			m_Pallet.m_dPressNormValue[1] = m_fPressNormValue[1];

			m_nNextAutoStep = AUTO_SAVE_ALL_PRESS_DATA;
			
			SaveStepInfo(WRITE_TYPE_BATTERY);
			break;
		}
	case AUTO_SAVE_ALL_PRESS_DATA:
		{
			m_strMsg = m_bLangulageEng?_T("Save All PressData"):_T("保存压力数据");
			
			if(SavePressValue())
			{
				m_vecPressValue.clear();
				m_nNextAutoStep = AUTO_SEND_INSHELL_FINISH_SINGLE;
			}
			
			break;
		}
		//发送入壳完成信号
	case AUTO_SEND_INSHELL_FINISH_SINGLE:
		{
			m_strMsg = m_bLangulageEng?_T("Send InShell Finish Single"):_T("发送入壳完成信号");

			if (m_OOpenVacum > -1)
			{
				OUTPUTS[m_OOpenVacum]->Off();
			}
			Sleep(200);
			if (MACHINE.m_pBatteryInShell[m_nGroupID] != NULL)
			{
				MACHINE.m_pBatteryInShell[m_nGroupID]->m_Pallet = m_Pallet;
				MACHINE.m_pBatteryInShell[m_nGroupID]->m_bShellReady = FALSE;
				m_Pallet.Release();
				m_nNextAutoStep = AUTO_SEND_MOTOR_TO_WAIT_POS_EX;

				SaveStepInfo(WRITE_TYPE_BATTERY);
				MACHINE.m_pBatteryInShell[m_nGroupID]->SaveStepInfo(WRITE_TYPE_BATTERY);
			}
			break;
		}

		//送料电机到等待位
	case AUTO_SEND_MOTOR_TO_WAIT_POS_EX:
		{
			m_strMsg = m_bLangulageEng?_T("Send Motor To Wait Pos"):_T("送料电机到等待位");
			if(m_MotorX > -1)
			{
				MOTORS[m_MotorX]->m_dAloneSpeedRatio=4;
			}
			if(m_MotorX > -1 && MotorMove(GetCurPorductID(MACHINE.m_nDefaultProduct), m_MotorX, SEND_MOTOR_WAIT_LOC))
			{
				m_nNextAutoStep = AUTO_WORK_END;
			}
			break;
		}

		//工作完成
	case AUTO_WORK_END:
		{
			
			m_strMsg = m_bLangulageEng?_T("Work end"):_T("工作完成");
			m_nNextAutoStep = AUTO_SEND_MOTOR_TO_WAIT_POS;
			break;
		}
	default:
		{
			ASSERT(0);
			break;
		}
	}
}

BOOL CRunProcessBatteryCore::InitializeConfig(CString strProcessModule)
{
	if (!CRunProcess::InitializeConfig(strProcessModule))
	{
		return FALSE;
	}
	CString strTemp[2];

	if(m_nGroupID == 0)
	{
		strTemp[0] = _T("Station1PressMinValue");
		strTemp[1] = _T("Station1PressMaxValue");
	}
	else if(m_nGroupID == 1)
	{
		strTemp[0] = _T("Station1PressMinValue");
		strTemp[1] = _T("Station1PressMaxValue");
	}
	else
		ASSERT(0);
	
	m_fPressNormValue[0] = ::IniReadDouble(_T("MESSET"),  strTemp[0],  0,GetAbsolutePathName(MACHINE_CFG));
	m_fPressNormValue[1] = ::IniReadDouble(_T("MESSET"), strTemp[1],  0,GetAbsolutePathName(MACHINE_CFG));

	AfxBeginThread(BatteryCoreThread, this);
	return TRUE;
}

void CRunProcessBatteryCore::GetHardwareConfig(CArray<int, int> *pInputs, CArray<int, int> *pOutputs, CArray<int, int> *pMotors)
{
	CRunProcess::GetHardwareConfig(pInputs, pOutputs, pMotors);
}


BOOL CRunProcessBatteryCore::  ReadRunParameter()
{
	CRunProcess:: ReadRunParameter();
	m_bEnablePressSenseClient =::IniReadBoolean(m_strProcess, _T("EnablePressSenseClient"), TRUE, GetAbsolutePathName(PARAMETER_CFG));
	m_strPressSenseIP = IniReadString(m_strProcess, _T("PressSenseIP"), _T("192.168.1.5"), GetAbsolutePathName(PARAMETER_CFG));
	m_nPressSensePort = IniReadInt(m_strProcess, _T("PressSensePort"),  8008, GetAbsolutePathName(PARAMETER_CFG));
	return TRUE;
}

BOOL CRunProcessBatteryCore::SaveParameter()
{
	if (!IniWriteBoolean(m_strProcess, _T("EnablePressSenseClient"), m_bEnablePressSenseClient, GetAbsolutePathName(PARAMETER_CFG)))
	{
		return FALSE;
	}
	if (!IniWriteString(m_strProcess, _T("PressSenseIP"), m_strPressSenseIP, GetAbsolutePathName(PARAMETER_CFG)))
	{
		return FALSE;
	}
	if (!IniWriteInt(m_strProcess, _T("PressSensePort"), m_nPressSensePort, GetAbsolutePathName(PARAMETER_CFG)))
	{
		return FALSE;
	}

	CRunProcess::SaveParameter();
	return TRUE;
}

/****************************************************************************************
函数描述：检查电机能否移动
输入参数：
函数返回：TRUE:执行成功，FALSE:执行失败
*****************************************************************************************/
BOOL  CRunProcessBatteryCore::CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{
	ASSERT(m_MotorX > -1 && MACHINE.m_pOnloadCore != NULL && MACHINE.m_pOnloadCore->m_nMotorZ > -1);

	//判断整列气缸是否回退
	if (pMotor == MOTORS[m_MotorX])
	{
		double dPos0 = MOTORS[MACHINE.m_pOnloadCore->m_nMotorZ]->GetCurPos();
		if (dPos0 > 5)
		{
			BLMessageBox(_T("(M07)电芯来料抓手Z轴位置大于5，电机不能移动"), MB_ICONWARNING);
			return FALSE;
		}

		//调试暂时去掉保护  Yao 4.4
	/*	if ((m_IColumnCylClose[0] > -1 && INPUTS[m_IColumnCylClose[0]]->IsOn()) 
			|| (m_IColumnCylClose[1] > -1 && INPUTS[m_IColumnCylClose[1]]->IsOn())
			|| (m_IColumnCylOpen[0] > -1 && INPUTS[m_IColumnCylOpen[0]]->IsOff()) 
			|| (m_IColumnCylOpen[1] > -1 && INPUTS[m_IColumnCylOpen[1]]->IsOff()))
		{
			BLMessageBox(_T("电芯定位气缸推出夹紧，电机不能移动"), MB_ICONWARNING);
			return FALSE;
		}*/
		
		dPos0 = MOTORS[m_MotorX]->GetCurPos();
		double dPos1 = GetMotorPos(GetCurPorductID(MACHINE.m_nDefaultProduct), m_MotorX, SEND_MOTOR_OPEN_CYL_OPEN_LOC);
		if ((m_IOpenCylClose[0] > -1 && INPUTS[m_IOpenCylClose[0]]->IsOn()) 
			|| (m_IOpenCylClose[1] > -1 && INPUTS[m_IOpenCylClose[1]]->IsOn())
			|| (m_IOpenCylOpen[0] > -1 && INPUTS[m_IOpenCylOpen[0]]->IsOff()) 
			|| (m_IOpenCylOpen[1] > -1 && INPUTS[m_IOpenCylOpen[1]]->IsOff()))
		{
			/*if (bAbsMove == 0)
			{
			if (dPos0 + lfValue > dPos1)
			{
			BLMessageBox(_T("开合气缸没有打开,电机移动位置不能大于开合气缸张开位"), MB_ICONWARNING);
			return FALSE;
			}
			}
			if (bAbsMove == 1)
			{
			if (lfValue > dPos1)
			{
			BLMessageBox(_T("开合气缸没有打开,电机移动位置不能大于开合气缸张开位"), MB_ICONWARNING);
			return FALSE;
			}
			}*/
		}
	}
	return TRUE;
}

BOOL  CRunProcessBatteryCore::CheckOutputCanActive(COutput *pOutput)
{
	return TRUE;
}

/****************************************************************************************
函数描述：程序运行时一直调用
输入参数：
函数返回：TRUE:执行成功，FALSE:执行失败
*****************************************************************************************/
void CRunProcessBatteryCore::RunWhile()
{
	if (!m_bEnablePressSenseClient)
	{
		if(!m_bGetValueFinish)
		{
			m_bGetValueFinish = TRUE;
		}
		return ;
	}
	float dPressValue = 0.0;
	CString strCmd = _T("#01\r\n");
	CString strTemp = _T("");
	CString strResult;
	if (m_bStartGetValue)
	{
		dPressValue = 0.0;

		if(m_bGetValueFinish)
		{
			m_bGetValueFinish = FALSE;
		}

		//if (m_PressSenseClient.GetPressValue(1, &dPressValue))
		if (m_PressClient.SendToDeviceAndWait(strCmd,strResult))
		{
			m_nTryReadTimes = 0;
  			if (strResult.GetLength() > 0)
			{
				strTemp = strResult.Mid(1);
				int nLength = strTemp.GetLength();
				strTemp = strTemp.Left(nLength - 1);
				dPressValue = _wtof(strTemp);
				m_PressDataLock.Lock();
				m_vecPressValue.push_back(dPressValue);
				m_PressDataLock.Unlock();
			}	
		}
		else
		{
			if(::IsNoHardware())
			{
				return;
			}
			m_nTryReadTimes++;
			m_PressClient.Disconnect();
			BOOL bStatus = m_PressClient.Connect(m_strPressSenseIP, m_nPressSensePort);
			if(!bStatus || m_nTryReadTimes >= 3)
			{
				m_nTryReadTimes =0;
				// 压力传感器发送测试命令失败, 请检查通信连接
				ShowMessage(MSG_BATTERY_CORE_PRESS_COMMUNICATE_FAIL);
			}
		}
	}
	else
	{
		if(!m_bGetValueFinish)
		{
			m_bGetValueFinish = TRUE;
		}
	}
}

/****************************************************************************************
函数描述：取压力最大值
输入参数：
函数返回：99999.9:读取失败，其它:最大值
*****************************************************************************************/
double CRunProcessBatteryCore::GetPressMaxValue()
{
	m_PressDataLock.Lock();//加锁保护
	int nCount = m_vecPressValue.size();

	if (nCount <= 0)
	{
		m_PressDataLock.Unlock();
		return 99999.9;
	}
	double dResult =0.00;

	if (nCount > 1)
	{
		dResult = 0;
		for (int i = 0; i < (int)m_vecPressValue.size(); i++)
		{
			if(dResult < m_vecPressValue[i])
			{
				dResult = m_vecPressValue[i];
			}
		}
		m_PressDataLock.Unlock();
		return dResult;
	}
	else
	{
		dResult =m_vecPressValue[0];
		m_PressDataLock.Unlock();
		return dResult;
	}
}

/****************************************************************************************
函数描述：整列气缸动作
bClose:TRUE:开合气缸关闭 FALSE:开合气缸打开
函数返回：TRUE:执行成功，FALSE:执行失败
*****************************************************************************************/
BOOL CRunProcessBatteryCore::ColumnCylClose( BOOL bClose)
{
	ASSERT(m_IColumnCylOpen[0] > -1
		&& m_IColumnCylOpen[1] > -1
		&& m_IColumnCylClose[0] > -1
		&& m_IColumnCylClose[1] > -1 
		&& m_OColumnCylClose > -1 
		&& m_OColumnCylOpen > -1);

	BOOL Res = FALSE;

	if(bClose)
	{			
		OUTPUTS[m_OColumnCylClose]->On();
		OUTPUTS[m_OColumnCylOpen]->Off();

		if (m_bDryRun)
		{
			Sleep(500);
			Res = TRUE;
		}
		else
		{
			if(WAIT_OK == WaitInput(INPUTS[m_IColumnCylClose[0]], ON, MSG_SHOW_DEFAULT) 
				&& WAIT_OK == WaitInput(INPUTS[m_IColumnCylClose[1]], ON, MSG_SHOW_DEFAULT) 
				&& WAIT_OK == WaitInput(INPUTS[m_IColumnCylOpen[0]], OFF, MSG_SHOW_DEFAULT)
				&& WAIT_OK == WaitInput(INPUTS[m_IColumnCylOpen[1]], OFF, MSG_SHOW_DEFAULT))
			{
				Res = TRUE;
			}
		}
	}
	else
	{
		OUTPUTS[m_OColumnCylClose]->Off();
		OUTPUTS[m_OColumnCylOpen]->On();

		if(WAIT_OK == WaitInput(INPUTS[m_IColumnCylClose[0]], OFF, MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_IColumnCylClose[1]], OFF, MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_IColumnCylOpen[0]], ON, MSG_SHOW_DEFAULT)
			&& WAIT_OK == WaitInput(INPUTS[m_IColumnCylOpen[1]], ON, MSG_SHOW_DEFAULT))
		{
			Res = TRUE;
		}
	}
	return Res;
}

/****************************************************************************************
函数描述：开合气缸动作
输入参数：bOpen  TRUE:表示打开   FALSE：闭合
函数返回：TRUE:执行成功，FALSE:执行失败
*****************************************************************************************/
BOOL CRunProcessBatteryCore::OpenCylOpen(BOOL bOpen)
{
	ASSERT(m_IOpenCylOpen[0] > -1 
		&& m_IOpenCylClose[0] > -1
		&& m_IOpenCylOpen[1] > -1
		&& m_IOpenCylClose[1] > -1
		&& m_OOpenCylOpen > -1 
		&& m_OOpenCylClose > -1);

	BOOL Res = FALSE;

	if(bOpen)
	{
		OUTPUTS[m_OOpenCylClose]->Off();
		OUTPUTS[m_OOpenCylOpen]->On();

		if(WAIT_OK == WaitInput(INPUTS[m_IOpenCylOpen[0]],ON,MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_IOpenCylClose[0]],OFF,MSG_SHOW_DEFAULT)
			&& WAIT_OK == WaitInput(INPUTS[m_IOpenCylOpen[1]],ON,MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_IOpenCylClose[1]],OFF,MSG_SHOW_DEFAULT))
		{
			Res = TRUE;
		}
	}
	else
	{
		OUTPUTS[m_OOpenCylOpen]->Off();
		OUTPUTS[m_OOpenCylClose]->On();

		if(WAIT_OK == WaitInput(INPUTS[m_IOpenCylOpen[0]],OFF,MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_IOpenCylClose[0]],ON,MSG_SHOW_DEFAULT)
			&&WAIT_OK == WaitInput(INPUTS[m_IOpenCylOpen[1]],OFF,MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_IOpenCylClose[1]],ON,MSG_SHOW_DEFAULT))
		{
			Res = TRUE;
		}
	}

	return Res;
}

/****************************************************************************************
函数描述：顶升气缸
输入参数：
函数返回：TRUE:执行成功，FALSE:执行失败
*****************************************************************************************/
BOOL CRunProcessBatteryCore::LiftCylUp(BOOL bUp)
{
	ASSERT(m_ILiftCylUp > -1
		&& m_ILiftCylDown > -1
		&& m_OLiftCylUp > -1
		&& m_OLiftCylDown > -1);

	BOOL Res = FALSE;

	if (bUp)
	{
		OUTPUTS[m_OLiftCylUp]->On();
		OUTPUTS[m_OLiftCylDown]->Off();

		if(WAIT_OK == WaitInput(INPUTS[m_ILiftCylDown],OFF,MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_ILiftCylUp],ON,MSG_SHOW_DEFAULT)) 
		{
			Res = TRUE;
		}
	}
	else
	{
		OUTPUTS[m_OLiftCylUp]->Off();
		OUTPUTS[m_OLiftCylDown]->On();

		if(WAIT_OK == WaitInput(INPUTS[m_ILiftCylDown],ON,MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_ILiftCylUp],OFF,MSG_SHOW_DEFAULT)) 
		{
			Res = TRUE;
		}
	}
	return Res;
}

/****************************************************************************************
函数描述：开合气缸关闭
输入参数：
函数返回：TRUE:执行成功，FALSE:执行失败
*****************************************************************************************/
BOOL CRunProcessBatteryCore::CloseOpenCyl(void)
{	
	ASSERT(m_OOpenCylOpen > -1 && m_OOpenCylClose > -1);

	OUTPUTS[m_OOpenCylOpen]->Off();
	OUTPUTS[m_OOpenCylClose]->On();
	return TRUE;
}

/****************************************************************************************
函数描述：检查开合气缸打开
输入参数：bOpen:TRUE 判断开合气缸打开 FALSE:判断开合气缸关闭
函数返回：TRUE:开合气缸打开，FALSE:开合气缸关闭
*****************************************************************************************/
BOOL CRunProcessBatteryCore::CheckCylOpen(BOOL bOpen)
{
	ASSERT(m_IOpenCylOpen[0] > -1
		&& m_IOpenCylClose[0] > -1
		&& m_IOpenCylOpen[1] > -1
		&& m_IOpenCylClose[1] > -1);

	BOOL bRes = FALSE;

	if(bOpen)
	{
		if(WAIT_OK == WaitInput(INPUTS[m_IOpenCylOpen[0]],ON,MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_IOpenCylClose[0]],OFF,MSG_SHOW_DEFAULT)
			&& WAIT_OK == WaitInput(INPUTS[m_IOpenCylOpen[1]],ON,MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_IOpenCylClose[1]],OFF,MSG_SHOW_DEFAULT)) 
		{
			bRes = TRUE;
		}
	}
	else
	{
		if(WAIT_OK == WaitInput(INPUTS[m_IOpenCylOpen[0]],OFF,MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_IOpenCylClose[0]],ON,MSG_SHOW_DEFAULT)
			&&WAIT_OK == WaitInput(INPUTS[m_IOpenCylOpen[1]],OFF,MSG_SHOW_DEFAULT) 
			&& WAIT_OK == WaitInput(INPUTS[m_IOpenCylClose[1]],ON,MSG_SHOW_DEFAULT))
		{
			bRes = TRUE;
		}
	}

	return bRes;
}

/****************************************************************************************
函数描述：检查开合气缸关闭
输入参数： void
函数返回：TRUE:开合气缸关闭，FALSE:开合气缸打开
*****************************************************************************************/
BOOL CRunProcessBatteryCore::CheckCylClose()
{

	for (int i=0;i<2;i++)
	{
		if (m_IOpenCylOpenTwo[i] < 0)
		{
			return TRUE;
		}

		if (WaitInput(INPUTS[m_IOpenCylOpenTwo[i]],ON,MSG_SHOW_DEFAULT))
		{
			continue;
		}
		return FALSE;
	}

	return TRUE;
}
/****************************************************************************************
函数描述：保存压力值
*****************************************************************************************/
BOOL CRunProcessBatteryCore::SavePressValue()
{
	m_PressDataLock.Lock();

	CString strTime, strFilePath, strFileName, strTemp;
	CStringA strDirectory;

	strFilePath = _T("D:\\PressData\\");

	strDirectory = strFilePath;
	MakeSureDirectoryPathExists(strDirectory.GetBuffer());
	strDirectory.ReleaseBuffer();

	SYSTEMTIME currentTime;
	GetLocalTime(&currentTime);

	strTime.Format(_T("%4d%02d%02dPressEx.csv"), currentTime.wYear, currentTime.wMonth, currentTime.wDay);
	strFileName = strFilePath + strTime;


	CStdioFile file;
	if (!file.Open(strFileName, CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate|CFile::shareDenyWrite))
	{
		BLMessageBox(_T("MES本地数据被打开，请关闭！"));
		m_PressDataLock.Unlock();
		return FALSE;
	}

	char* old_locale = _strdup(setlocale(LC_CTYPE, NULL));
	setlocale(LC_CTYPE, "chs");

	strTemp = _T("工位号,生产时间,二维码,最大压力值,所有压力值\n");

	file.WriteString(strTemp);
	file.SeekToEnd();

	strTime.Format(_T("%4d-%02d-%02d-%02d:%02d:%02d"),currentTime.wYear, currentTime.wMonth, currentTime.wDay, currentTime.wHour, currentTime.wMinute, currentTime.wSecond);
	
	strTemp.Format(_T("工位%d,%s,\'%s,%.3f,"),(m_nGroupID+1),strTime,m_Pallet.m_strBarCode,m_Pallet.m_dLimitPressValue);

	file.WriteString(strTemp);

	for(int i = 0; i < (int)m_vecPressValue.size(); i ++)
	{
		strTemp.Format(_T("%.3f,"),m_vecPressValue[i]);
		file.WriteString(strTemp);
	}
	file.WriteString(_T("\n"));

	setlocale(LC_CTYPE, old_locale);
	free(old_locale);
	file.Close();

	m_PressDataLock.Unlock();
	return TRUE;
}


void CRunProcessBatteryCore::ReadStepInfo()//读取带料启动数据
{
	CString strPath = GetAbsolutePathName(MODULE_RUN_FOLDER) + m_strName + _T(".cfg");
	m_nNextAutoStep = IniReadInt(_T("Data"), _T("m_nNextAutoStep"), 0, strPath);
	m_Pallet.ReadCfg(_T("Pallet"), strPath);
	return ;
}

BOOL CRunProcessBatteryCore::SaveStepInfo(int nWriteType)//保存带料启动数据
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

//检测料盘数据与实际检测数据一致性,m_nStartWithPalletData为输出参数0没有料，1有料,2有壳但还没有入壳
BOOL CRunProcessBatteryCore::CheckStartWithPalletDataSame()
{
	CString strMsg = _T("");
	if(m_Pallet.m_bIsValid)
	{
		if(m_IHaveCore > -1 && INPUTS[m_IHaveCore]->IsOff())
		{
			strMsg.Format(_T("带料启动入电芯模组有料感应与实际数据不一致,数据有料,感应无料,请确认"));
			ShowMessageEX(strMsg);
			strMsg.Format(_T("确认入电芯模组有料吗？有料点YES，无料点NO，若电芯不在有料感应器感应位置请点NO"));
			if(BLMessageBox(strMsg, MB_YESNO) == IDYES)
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
		if(m_IHaveCore > -1 && INPUTS[m_IHaveCore]->IsOn())
		{
			strMsg.Format(_T("带料启动入电芯模组有料感应与实际数据不一致,数据无料,感应有料,请确认"));
			ShowMessageEX(strMsg);
			strMsg.Format(_T("确认入电芯模组有料吗？有料点YES，无料点NO"));
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

	if(!m_Pallet.m_bIsValid)
	{
		m_nStartWithPalletData = 0;
	}
	else
	{
		m_nStartWithPalletData = 1;
	}
	return TRUE;
}
