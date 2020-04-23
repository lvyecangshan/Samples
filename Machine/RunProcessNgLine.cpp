/****************************************************************************************
文件名：RunProcessNgLine.h
功能描述：压力不良下料线体
历史修改记录：
*****************************************************************************************/
#include "StdAfx.h"
#include "RunProcessNgLine.h"


DWORD WINAPI NgLineThread(LPVOID lpParam)
{
	CRunProcessNgLine *pMotorRun = (CRunProcessNgLine*)lpParam;
	if (pMotorRun == NULL)
	{
		return 0;
	}

	pMotorRun->m_bThreadEnd = FALSE;
	pMotorRun->m_bThreadStart = TRUE;

	while (pMotorRun->m_bThreadStart)
	{
		pMotorRun ->WorkWhile();
		Sleep(1);
	}
	pMotorRun->m_bThreadEnd = TRUE;
	return 0;
}

CRunProcessNgLine::CRunProcessNgLine(int nID) : CRunProcess(nID)
{
	PowerUpRestart();

	m_InputMap.insert(pair<int*, CString>(&m_ICheckBattery[0], _T("ICheckBattery0")));
	m_InputMap.insert(pair<int*, CString>(&m_ICheckBattery[1], _T("ICheckBattery1")));

	m_InputMap.insert(pair<int*, CString>(&m_ICheckInPos, _T("ICheckInPos")));
	m_InputMap.insert(pair<int*, CString>(&m_ICheckLineFull, _T("ICheckLineFull")));
	
	m_OutputMap.insert(pair<int*, CString>(&m_OLineMotor, _T("OLineMotor")));

	m_InputMap.insert(pair<int*, CString>(&m_IManualBtn, _T("IManualBtn")));
	m_OutputMap.insert(pair<int*, CString>(&m_OManualBtnLight, _T("OManualBtnLight")));
}

CRunProcessNgLine::~CRunProcessNgLine(void)
{
	if (m_OLineMotor > -1 )
	{
		OUTPUTS[m_OLineMotor]->Off();
	}
	Destory();
}

void CRunProcessNgLine::PowerUpRestart()
{
	CRunProcess::PowerUpRestart();
	m_strMsg = m_bLangulageEng?_T("Ready"):_T("准备好");

	m_bNextRequire = FALSE;
	m_bRequireNgBattery = FALSE;

	m_nNextPowerUpStep = PU_WAIT_SAFE_SIGNAL;
	m_nNextAutoStep = AUTO_SEND_REQUIRE_SIGNAL;
}

void CRunProcessNgLine::PowerUpOperation(void)
{
	if (!IsModuleEnable())
	{
		CRun::PowerUpOperation();
		m_strMsg = m_bLangulageEng?_T("Moudle UnEnable"):_T("模块禁用");
		return;
	}

	switch(m_nNextPowerUpStep)
	{
	case PU_WAIT_SAFE_SIGNAL:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Safe Signal"):_T("等待安全信号");
			if (m_nNgLineID == 0 && MACHINE.m_pOnloadCore != NULL)
			{
				if (MACHINE.m_pOnloadCore->m_bHomeMoveSafePos)
				{
					m_nNextPowerUpStep = PU_MANUAL_RELEASE;
				}
			}

 			if (m_nNgLineID == 1 && MACHINE.m_pRobotOffload != NULL)
 			{
 				if (MACHINE.m_pRobotOffload->m_bRobotSafe)
 				{
 					m_nNextPowerUpStep = PU_MANUAL_RELEASE;	
 				}
 			}
			break;
		}

		//启动皮带
	case PU_MANUAL_RELEASE:
		{
			m_strMsg = m_bLangulageEng?_T("Release All Status"):_T("重置全部状态");
			if (m_OLineMotor > -1 && OUTPUTS[m_OLineMotor]->IsOn())
			{
				OUTPUTS[m_OLineMotor]->Off();
			}

			if(m_OManualBtnLight > -1)
			{
				OUTPUTS[m_OManualBtnLight]->Off();
			}
			m_nNextPowerUpStep = PU_CHECK_WITHOUT_PRODUCT;			
			break;
		}

		//检测是否有产品
	case PU_CHECK_WITHOUT_PRODUCT:
		{
			m_strMsg = m_bLangulageEng?_T("Check Without Product"):_T("检测无产品");		

			if (m_OLineMotor > -1 && OUTPUTS[m_OLineMotor]->IsOff())
			{
				OUTPUTS[m_OLineMotor]->On();
			}

			DWORD dwCurTime = GetTickCount();
			BOOL bCheckBattery = FALSE;

			while(GetTickCount() - dwCurTime < 3000)
			{
				if(CheckHaveProduct())
				{
					bCheckBattery = TRUE;
					break;
				}
			}

			if (m_OLineMotor > -1 && OUTPUTS[m_OLineMotor]->IsOn())
			{
				OUTPUTS[m_OLineMotor]->Off();
			}			

			if(bCheckBattery)
			{
				if(m_OManualBtnLight > -1)
					OUTPUTS[m_OManualBtnLight]->On();
				if(MACHINE.m_nTowerBuzzer > -1)
					OUTPUTS[MACHINE.m_nTowerBuzzer]->On();

				BLMessageBox(_T("皮带线检查有产品，请处理"));
				m_nNextPowerUpStep = PU_WAIT_MANUAL_CLEAR_BATTERY;
			}
			else
			{
				m_nNextPowerUpStep = PU_FINISH;
			}
			break;
		}

	case PU_WAIT_MANUAL_CLEAR_BATTERY:
		{
			m_strMsg = m_bLangulageEng?_T("Motor Stop"):_T("皮带停止");		

			if (IsNoHardware() || m_bDryRun || (m_IManualBtn > -1 && INPUTS[m_IManualBtn]->IsOn()))
			{
				if(m_OManualBtnLight > -1)
					OUTPUTS[m_OManualBtnLight]->Off();

				if(MACHINE.m_nTowerBuzzer > -1)
					OUTPUTS[MACHINE.m_nTowerBuzzer]->Off();

				m_nNextPowerUpStep = PU_MANUAL_RELEASE;
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

void CRunProcessNgLine::AutoOperation()
{
	if (!IsModuleEnable())
	{
		m_strMsg = m_bLangulageEng?_T("Moudle UnEnable"):_T("模块禁用");
		Sleep(500);
		return;
	}

	if (IsNoHardware())
	{
		Sleep(100);
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
		if (m_nSaveLastStep == AUTO_SEND_REQUIRE_SIGNAL)
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
		//向发送要料信号
	case AUTO_SEND_REQUIRE_SIGNAL:
		{
			m_strMsg = m_bLangulageEng?_T("Send Require Signal"):_T("发送要料信号");		

			if (!CheckHaveProduct())
			{
				if (m_OLineMotor > -1 && OUTPUTS[m_OLineMotor]->IsOn())
				{
					OUTPUTS[m_OLineMotor]->Off();
				}

				if (m_ICheckLineFull > -1 && INPUTS[m_ICheckLineFull]->IsOn())
				{
					m_nNextAutoStep = AUTO_CHECK_FULL;
				}
				else
				{
					if (INPUTS[m_ICheckBattery[0]]->IsOff() && INPUTS[m_ICheckBattery[1]]->IsOff())
					{
						if(!m_bRequireNgBattery)
						{
							m_bRequireNgBattery = TRUE;
							m_nNextAutoStep = AUTO_WAIT_OFFLOAD_FINISH_SIGNAL;	
						}
					}
				}
			}
			else
			{
				ShowMessage(_T("NG线体有产品，不能发送要料信号，请处理"),3);
			}
			break;
		}

		//等待下料完成信号
	case AUTO_WAIT_OFFLOAD_FINISH_SIGNAL:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Offload Signal"):_T("等待NG放料完成");

			if (!m_bRequireNgBattery)
			{
				m_nNextAutoStep = AUTO_CHECK_HAVE_CORE;
			}
			break;
		}

		//检测来料
	case AUTO_CHECK_HAVE_CORE:
		{		
			m_strMsg = m_bLangulageEng?_T("Move XY Solder Pos"):_T("放料完成，检测有料");

			ASSERT(m_ICheckBattery[0] > -1 && m_ICheckBattery[1] > -1);

			if (IsNoHardware() || m_bDryRun || (INPUTS[m_ICheckBattery[0]]->IsOn() || INPUTS[m_ICheckBattery[1]]->IsOn()))
			{
				m_nNextAutoStep = AUTO_MOTOR_MOVE_TO_RECV_NG;
			}
			break;
		}

		//皮带启动
	case AUTO_MOTOR_MOVE_TO_RECV_NG:
		{
			m_strMsg = m_bLangulageEng?_T("Motor Move"):_T("皮带转动，接收NG品");

			if (m_OLineMotor > -1 && OUTPUTS[m_OLineMotor]->IsOff())
			{
				OUTPUTS[m_OLineMotor]->On();
			}
			m_nDelayTime = GetTickCount();
			m_nNextAutoStep = AUTO_CHECK_FULL;
			break;
		}

		//检测是否满料
	case AUTO_CHECK_FULL:
		{
			m_strMsg = m_bLangulageEng?_T("Check Full"):_T("检测是否满料");

			if(IsNoHardware() || m_bDryRun)
			{
				m_nNextAutoStep = AUTO_WORK_END;
				break;
			}

			if (m_ICheckLineFull > -1 && INPUTS[m_ICheckLineFull]->IsOn())
			{
				if(m_OManualBtnLight > -1)
				{
					OUTPUTS[m_OManualBtnLight]->On();
				}
				//Sleep(1000);
				if (m_OLineMotor > -1 && OUTPUTS[m_OLineMotor]->IsOn())
				{
					OUTPUTS[m_OLineMotor]->Off();
				}
				MACHINE.SetBuzzerStatue(TRUE);
				CString strMsg;
				strMsg = (m_strProcess == _T("PressNgLine"))?_T("压力不良线体有料请拿走"):_T("扫码不良线体有料请拿走");
				BLMessageBox(strMsg);
				m_nNextAutoStep = AUTO_WAIT_MANUAL_OPERATOR;
			}
			else if (GetTickCount() - m_nDelayTime > 10000)
			{
				CString strMsg;
				strMsg = (m_strProcess == _T("PressNgLine"))?_T("压力不良线体满料感应10S没有接收到产品，请检查是否卡料或皮带异常"):_T("扫码不良线体满料感应10S没有接收到产品，请检查是否卡料或皮带异常");
				ShowMessage(strMsg,3);
			}
			break;
		}

	case AUTO_WAIT_MANUAL_OPERATOR:
		{
			m_strMsg = m_bLangulageEng?_T("manual operator"):_T("等待人工处理");

			if(IsNoHardware() || m_bDryRun || (m_IManualBtn > -1 && INPUTS[m_IManualBtn]->IsOn()))
			{
				MACHINE.SetBuzzerStatue(FALSE);
				if(m_OManualBtnLight > -1)
				{
					OUTPUTS[m_OManualBtnLight]->Off();
				}
				m_nNextAutoStep = AUTO_WORK_END;
			}
			break;
		}
		//工作完成
	case AUTO_WORK_END:
		{
			m_strMsg = m_bLangulageEng?_T("Work end"):_T("工作完成");

			m_nNextAutoStep = AUTO_SEND_REQUIRE_SIGNAL;
			break;
		}

	default:
		{
			ASSERT(0);
			break;
		}
	}
}

BOOL CRunProcessNgLine::InitializeConfig(CString strProcessModule)
{
	if (!CRunProcess::InitializeConfig(strProcessModule))
		return FALSE;

	m_nNgLineID = -1;
	if(m_strModuleName == _T("BarCodeNgLine"))
		m_nNgLineID = 0;
	else if(m_strModuleName == _T("PressNgLine"))
		m_nNgLineID = 1;
	else
		ASSERT(0);

	AfxBeginThread((AFX_THREADPROC)NgLineThread,(LPVOID)this);
	return TRUE;
}


void CRunProcessNgLine::GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors)
{
	CRunProcess::GetHardwareConfig(pInputs,pOutputs,pMotors);
}

BOOL CRunProcessNgLine::CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{
	return TRUE;
}

BOOL CRunProcessNgLine::CheckOutputCanActive(COutput *pOutput)
{
	return TRUE;
}


/****************************************************************************************
函数描述：检查是否有料
函数返回：TRUE:有料，FALSE:没料
*****************************************************************************************/
BOOL CRunProcessNgLine::CheckHaveProduct()
{

	/*ASSERT(m_ICheckBattery[0] > -1 && m_ICheckBattery[1] > -1 && m_ICheckLineFull > -1 && m_ICheckInPos > -1);
	
	if (INPUTS[m_ICheckBattery[0]]->IsOn() || INPUTS[m_ICheckBattery[1]]->IsOn()
		|| INPUTS[m_ICheckInPos]->IsOn() || INPUTS[m_ICheckLineFull]->IsOn())
	{
		return TRUE;
	}*/
	return FALSE;
}

/****************************************************************************************
//函数描述：检测电机移动
函数描述：不良按钮控制电机
函数返回：
*****************************************************************************************/
void CRunProcessNgLine::WorkWhile()   
{
	return;
}

/****************************************************************************************
函数描述：清除资源
函数返回：
*****************************************************************************************/
void CRunProcessNgLine::Destory()
{
	DWORD dwTime = GetTickCount();

	m_bThreadStart = FALSE;
	
	while(!m_bThreadEnd)
	{
		if (GetTickCount() - dwTime > 10000)
		{
			break;
		}
		Sleep(1);
	}
}
