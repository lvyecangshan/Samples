/****************************************************************************************
文件名：RunProcessElevator.h
功能描述：升降传送机
历史修改记录：
*****************************************************************************************/
#include "StdAfx.h"
#include "RunProcessElevator.h"

CRunProcessElevator::CRunProcessElevator(int nID) : CRunProcess(nID)
{
	PowerUpRestart();
	InitIO();

	m_Motor.InitMotorThread();
}

CRunProcessElevator::~CRunProcessElevator(void)
{
	
}

//IO初始化
BOOL CRunProcessElevator::InitIO()
{

	m_Elevator.SetParent(this);
	m_Elevator.AddElevator(_T("ILineSafeUp"),_T("ILineSafeDown"),_T("IElevSafe"),
							_T("IStopBackPush"),_T("IStopBackReturn"),_T("IHasPlate0"),_T("IHasPlate1"),
							_T("OStopBackReturn"),_T("OForwardRun"),_T("OReverseRun"));
	m_Motor.SetParent(this);
	m_Motor.AddMotor(_T("OLinemotor0"),_T("OLinemotor1"),_T("OLinemotor2"));

	//电机
	APP_Add_Motor(&m_nMotorZ,_T("MotorZ"));
	
	
	return TRUE;
}

void CRunProcessElevator::PowerUpRestart()
{
	CRunProcess::PowerUpRestart();
	m_Motor.PowerUpRestart();
	m_nNextPowerUpStep = PU_WAIT_STAR_SIGNAL;
	m_nNextAutoStep = AUTO_SEND_REQUIRE_SIGNAL;
	m_bPlateStatus[0] = FALSE;
	m_bPlateStatus[1] = FALSE;
	
}

void CRunProcessElevator::PowerUpOperation(void)
{
	if (!IsModuleEnable())
	{
		CRun::PowerUpOperation();
		m_strMsg = m_bLangulageEng?_T("Moudle UnEnable"):_T("模块禁用");
		return;
	}
	APP_NoHardware_Sleep();

	switch(m_nNextPowerUpStep)
	{
		//等待开始工作信号
	case PU_WAIT_STAR_SIGNAL:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("等待开始工作信号");
 			m_nNextPowerUpStep = PU_STOP_BELT;
			break;
		}
		//停止皮带转动
	case PU_STOP_BELT:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("停止皮带转动");
			if(IsNoHardware() || (StopReceiveMotor() && StopFeedingMotor()))  //上下层皮带停止传动
			{
				m_nNextPowerUpStep = PU_CHECK_SAFE;
			}
			break;
		}
		//检测是否安全
	case PU_CHECK_SAFE:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("检测是否安全");
			if(IsNoHardware() || m_Elevator.CheckWorkSafe())  //确保电机安全
			{
				m_nNextPowerUpStep = PU_OPERATION_BEFORE_MOVE;
			}
			break;
		}
	case PU_OPERATION_BEFORE_MOVE:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("止回气缸推出");
			if(IsNoHardware() || CylinderPush())
			{
				m_nNextPowerUpStep = PU_TO_RECEIVE_MATERIAL_POSTION;
			}
			
			break;
		}
		//Z轴电机移动到接料位
	case PU_TO_RECEIVE_MATERIAL_POSTION:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("Z轴电机移动到接料位");
			APP_Wait_Signal_Sleep();
			if(IsNoHardware())
			{
				m_nNextPowerUpStep = PU_FINISH;
			}
			else 
			{
				if(CylinderReturn() && MoveToReceiveMaterialPostion())
				{
					m_nNextPowerUpStep = PU_FINISH;
				}			
			}
			break;
		}
		//初始化完成
	case PU_FINISH:
		{
			CRun::PowerUpOperation();
			m_strMsg = m_bLangulageEng?_T(""):_T("初始化完成");
			m_nNextAutoStep = AUTO_CHECK_WORK_SAFE;
			break;
		}
	default:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("未定义动作");
			break;
		}
	}

}

void CRunProcessElevator::AutoOperation()
{
	if (!IsModuleEnable())
	{
		m_strMsg = m_bLangulageEng?_T("Moudle UnEnable"):_T("模块禁用");
		Sleep(500);
		return;
	}

	APP_NoHardware_Sleep();


	switch(m_nNextAutoStep)
	{
		//检测安全
	case AUTO_CHECK_WORK_SAFE:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("检测安全");
			APP_Wait_Signal_Sleep();
			if(IsNoHardware())
			{
				m_nNextAutoStep = AUTO_TO_RECEIVE_MATERIAL_POSTION;
			}
			else
			{
				if(StartReceiveMotor() && StopFeedingMotor()
					&& m_Elevator.CheckWorkSafe() && CylinderPush())  
				{
					m_nNextAutoStep = AUTO_TO_RECEIVE_MATERIAL_POSTION;
				}
			}
			break;
		}
		//Z轴移动到接料位
	case AUTO_TO_RECEIVE_MATERIAL_POSTION:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("Z轴移动到接料位");
			APP_Wait_Signal_Sleep();
			if(IsNoHardware())
			{
				m_nNextAutoStep = AUTO_SEND_REQUIRE_SIGNAL;
			}
			else if(MoveToReceiveMaterialPostion() && CylinderReturn())
			{
				m_nNextAutoStep = AUTO_SEND_REQUIRE_SIGNAL;
			}
			if(IsNoHardware() || m_bDryRun)
			{
				m_bSendNeedCount = 2 ;
			}
			else
			{
				m_bSendNeedCount = 2 - GetCurPlateCount() ;
			}
			
			break;
		}
		//发送要料信号
	case AUTO_SEND_REQUIRE_SIGNAL:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("发送要料信号");
			APP_Wait_Signal_Sleep();
			int nPlateCount = GetCurPlateCount();
			if(IsNoHardware() || m_bDryRun)  
			{
				if(m_bSendNeedCount <= 0)  //料满
				{
					SetNeedSingal(FALSE);
					if(IsNoHardware() || CylinderPush())
					{
						m_nNextAutoStep = AUTO_SEND_RECEIVE_END;
					}
				}
				else 
				{
					if(SetNeedSingal(TRUE))
					{	
						//启动电机
						if(IsNoHardware() || StartReceiveMotor())
						{
							m_nNextAutoStep = AUTO_START_RECEIVE_PLATE;
						}
					}
				}			
			}
			else   //带料运行
			{
				if(nPlateCount < 2)
				{
					if(SetNeedSingal(TRUE))
					{	
						//启动电机
						if(IsNoHardware() || StartReceiveMotor())
						{
							m_nNextAutoStep = AUTO_START_RECEIVE_PLATE;
						}
					}
				}
				else
				{
					m_nNextAutoStep = AUTO_SEND_RECEIVE_END;
				}

			}
			
			
			break;
		}
	case AUTO_START_RECEIVE_PLATE:
		{			
			if(IsNoHardware() || m_bDryRun)
			{
				m_strMsg = m_bLangulageEng?_T(""):_T("开始接收料盘");
				if(!GetNeedSingal())
				{
					m_nNextAutoStep = AUTO_SEND_REQUIRE_SIGNAL;
				}
				else
				{
					m_bSendNeedCount --;
				}
			}
			else
			{
				if(GetCurPlateCount() < 2)   //等待料盘到达
				{
					m_nNextAutoStep = AUTO_SEND_REQUIRE_SIGNAL;
				}	
				else
				{
					m_nNextAutoStep = AUTO_SEND_RECEIVE_END;
				}
			}			
			
			break;
		}
		//料盘接收完成
	case  AUTO_SEND_RECEIVE_END:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("料盘接收完成");
			APP_Wait_Signal_Sleep();
			if(IsNoHardware())
			{
				m_nNextAutoStep = AUTO_TO_FEEDING_POSITION;
				SendReceiveEnd();
			}
			else
			{
				if(m_Elevator.CheckWorkSafe() && CylinderPush() && StopReceiveMotor())
				{
					m_nNextAutoStep = AUTO_TO_FEEDING_POSITION;
					SendReceiveEnd();
				}
			}

			
			break;
		}
	case  AUTO_TO_FEEDING_POSITION:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("Z轴移动到下料位");
			APP_Wait_Signal_Sleep();
			APP_Wait_Signal_Sleep();
			if(IsNoHardware() || MoveToFeedingMaterialPostion())
			{
				m_nNextAutoStep = AUTO_WAIT_FEEDING_SIGNAL;
			}
			break;
		}
	case AUTO_WAIT_FEEDING_SIGNAL:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("等待下料信号");						
			if(GetNextNeedSingal())
			{
				m_nNextAutoStep = AUTO_FEEDING_MATERIAL;
			}
			break;
		}
	case AUTO_FEEDING_MATERIAL:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("开始下料");
			if(CheckCanFeedingMaterial())
			{
				if(IsNoHardware() || StartFeedingMotor())
				{
					m_nNextAutoStep = AUTO_WAIT_FEEDING_END;
				}
				
			}
		}
	case AUTO_WAIT_FEEDING_END:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("等待下料完成");
			APP_Wait_Signal_Sleep();
			m_bPlateStatus[0]=FALSE;
			m_bPlateStatus[1]=FALSE;
			if(CheckFeedingMaterialEnd())
			{
				
				if(IsNoHardware() || StopFeedingMotor())
				{
					m_nNextAutoStep = AUTO_WORK_END;
				}
				
			}
		}
		//工作完成
	case AUTO_WORK_END:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("工作完成");
			APP_Wait_Signal_Sleep();
			m_nNextAutoStep = AUTO_CHECK_WORK_SAFE;
			
			break;
		}

	default:
		{
			ASSERT(0);
			break;
		}
	}
}

BOOL CRunProcessElevator::InitializeConfig(CString strProcessModule)
{
	if (!CRunProcess::InitializeConfig(strProcessModule))
		return FALSE;
	return TRUE;
}


void CRunProcessElevator::GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors)
{
	CRunProcess::GetHardwareConfig(pInputs,pOutputs,pMotors);
}

BOOL CRunProcessElevator::CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{
	return TRUE;
}

BOOL CRunProcessElevator::CheckOutputCanActive(COutput *pOutput)
{
	return TRUE;
}

//移动到接料位
BOOL CRunProcessElevator::MoveToReceiveMaterialPostion()
{
	return FALSE;
}
//移动到上料位
BOOL CRunProcessElevator::MoveToFeedingMaterialPostion()
{
	return FALSE;
}

//检测安全


//判断是否可以给下一线体上料
BOOL CRunProcessElevator::CheckCanFeedingMaterial()
{
	return FALSE;
}

//获取当前升降机上料盘数量
int CRunProcessElevator::GetCurPlateCount()
{
	int nCurPlateCount = 0;
	if(IsNoHardware() || m_bDryRun)
	{
		if(m_bPlateStatus[0])
		{
			nCurPlateCount++;
		}
		if(m_bPlateStatus[1])
		{
			nCurPlateCount++;
		}
	}
	else
	{
		nCurPlateCount = 0;
		if(APP_INPUTS_IsOn(m_IHasPlate1))  //升降料盘1到位感应
		{
			nCurPlateCount ++;
		}
		if(APP_INPUTS_IsOn(m_IHasPlate2))  //升降料盘2到位感应
		{
			nCurPlateCount ++;
		}
	}	
	return nCurPlateCount;
}

//判断下料是否完成
BOOL CRunProcessElevator::CheckFeedingMaterialEnd()
{
	if(IsNoHardware() || m_bDryRun)
	{
		if(!m_bPlateStatus[0] && !m_bPlateStatus[1])
		{
			return TRUE;
		}
		return FALSE;
	}
	else
	{
		if(!APP_INPUTS_IsOn(m_IHasPlate1) && !APP_INPUTS_IsOn(m_IHasPlate2))
		{
			return m_Elevator.CheckWorkSafe();
		}
	}
	return FALSE;
	
	
}


//启动上料电机
BOOL CRunProcessElevator::StartReceiveMotor()
{
	BOOL bRet = m_Elevator.ReverseRun();
	return bRet;
}
//停止上料电机
BOOL CRunProcessElevator::StopReceiveMotor()
{
	BOOL bRet = m_Elevator.ReverseStop();
	return bRet;
}
//启动下料电机
BOOL CRunProcessElevator::StartFeedingMotor()
{
	BOOL bRet = m_Elevator.ForwardRun();
	return bRet;
}
//停止下料电机
BOOL CRunProcessElevator::StopFeedingMotor()
{
	BOOL bRet = m_Elevator.ForwardStop();
	return bRet;
}

//止回气缸推出
BOOL CRunProcessElevator::CylinderPush()
{
	BOOL bRet = APP_OUTPUTS_On(m_OCheckCylinder);
	return bRet;
}
//止回气缸回退
BOOL CRunProcessElevator::CylinderReturn()
{
	BOOL bRet = APP_OUTPUTS_Off(m_OCheckCylinder);
	return bRet;
}

BOOL CRunProcessElevator::SendReceiveEnd()
{
	return FALSE;
}

void CRunProcessElevator::MotorRun()
{
	if (m_Motor.GetMachinState()==MC_RUNNING)
	{
		m_Motor.AllMotorStart();

		while(m_Motor.GetMachinState()==MC_RUNNING)
		{
			//作满料判断
			Sleep(10);
		}
	}
	else if (m_Motor.GetMachinState() == MC_STOP_RUN && m_Motor.CountTime())   //延时1000ms停止
	{
		m_Motor.AllMotorStop();
	}
}
