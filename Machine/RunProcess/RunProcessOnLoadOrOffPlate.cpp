/****************************************************************************************
文件名：RunProcessOnLoadOrOffPlate.h
功能描述：上下料工位
历史修改记录：
*****************************************************************************************/
#include "StdAfx.h"
#include "RunProcessOnLoadOrOffPlate.h"

CRunProcessOnLoadOrOffPlate::CRunProcessOnLoadOrOffPlate(int nID) : CRunProcess(nID)
{
	PowerUpRestart();
	m_bOnLoadMode = TRUE;
	InitIO();
	m_Motor.InitMotorThread(); //初始化电机线程
}

CRunProcessOnLoadOrOffPlate::~CRunProcessOnLoadOrOffPlate(void)
{
	
}

//IO初始化
BOOL CRunProcessOnLoadOrOffPlate::InitIO()
{	
	m_Plate.SetParent(this);
    m_Plate.AddOnOrOffLoadPlate(_T("IDamperPush"),_T("IDamperReturn"),_T("IJackingPush"),_T("IJackingReturn"),_T("IHasPlate"),_T("IHasCore"),_T("ICheckDirection"),
		                        _T("ODamperReturn"),_T("OJackingPush"),_T("OJackingReturn"),_T("IHasPlate2"),_T("IHasCore2"),_T("ICheckDirection2"),
								_T("IFullCheck0"),_T("IFullCheck1"));
	m_Motor.SetParent(this);
	m_Motor.AddMotor(_T("OLinemotor0"),_T("OLinemotor1"),_T("OLinemotor2"));

	return TRUE;
}

//设置上料模式
void CRunProcessOnLoadOrOffPlate::SetOnLoadMode()
{
	m_bOnLoadMode = TRUE;
}
BOOL CRunProcessOnLoadOrOffPlate::IsOnLoadMode()
{
	return m_bOnLoadMode;
}
//设置下料模式
void CRunProcessOnLoadOrOffPlate::SetOffLoadMode()
{
	m_bOnLoadMode = FALSE;
}
BOOL CRunProcessOnLoadOrOffPlate::IsOffLoadMode()
{
	return !m_bOnLoadMode;
}



void CRunProcessOnLoadOrOffPlate::PowerUpRestart()
{
	CRunProcess::PowerUpRestart();
	m_Motor.PowerUpRestart();
	m_bRequirePallet = FALSE;
	m_nNextPowerUpStep = PU_WAIT_RESET;
	m_nNextAutoStep = AUTO_WORK_STAR;	
	m_bNeedPlate = FALSE;
	m_bFeedPlate = FALSE; 
	m_bFeedEnd   = TRUE;   
}

void CRunProcessOnLoadOrOffPlate::PowerUpOperation(void)
{
	if (!IsModuleEnable())
	{
		CRun::PowerUpOperation();
		m_strMsg = m_bLangulageEng?_T("Moudle UnEnable"):_T("模块禁用");
		return;
	}
	APP_NoHardware_Sleep();
	if(IsOnLoadMode())
	{
		PowerUpOperationOnLoad();
	}
	else if(IsOffLoadMode())
	{
		PowerUpOperationOffLoad();
	}
}

void CRunProcessOnLoadOrOffPlate::PowerUpOperationOnLoad(void)
{
	switch(m_nNextPowerUpStep)
	{
		//等待开始工作信号
	case PU_WAIT_RESET:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("开始初始化");
			if(m_Plate.DamperPush() && m_Plate.JackingReturn())
			{
				if(!m_Plate.NoPlate(0))
				{
					APP_BLMessageBox(_T("上料位有料，请拿走"));					
				}
				else
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
			m_nNextAutoStep = AUTO_WORK_STAR;
			break;
		}
	default:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("未定义动作");
			break;
		}
	}

}
void CRunProcessOnLoadOrOffPlate::PowerUpOperationOffLoad(void)
{
	switch(m_nNextPowerUpStep)
	{
		//等待开始工作信号
	case PU_WAIT_RESET:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("开始初始化");
			if(m_Plate.DamperPush() && m_Plate.JackingReturn())
			{
				if(!m_Plate.NoCore(0))
				{
					APP_BLMessageBox(_T("下料位有电芯，请拿走"));					
				}
				else
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
			m_nNextAutoStep = AUTO_WORK_STAR;
			break;
		}
	default:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("未定义动作");
			break;
		}
	}

}

void CRunProcessOnLoadOrOffPlate::AutoOperation()
{
	if (!IsModuleEnable())
	{
		m_strMsg = m_bLangulageEng?_T("Moudle UnEnable"):_T("模块禁用");
		Sleep(500);
		return;
	}
	APP_NoHardware_Sleep();
	if(IsOnLoadMode())
	{
		AutoOperationOnLoad();
	}
	else if(IsOffLoadMode())
	{
		AutoOperationOffLoad();
	}	
}
void CRunProcessOnLoadOrOffPlate::AutoOperationOnLoad()
{
	switch(m_nNextAutoStep)
	{
		//开始工作
	case AUTO_WORK_STAR:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("开始工作");

			if(IsNoHardware() || m_Plate.DamperPush() && m_Plate.JackingReturn())
			{
				if(IsNoHardware() || m_Plate.GetCurPlateCount(0) < m_Plate.GetPlateCount(0))
				{
					//设置要料信号
					if(SetNeedSingal(TRUE))
					{
						m_nNextAutoStep = AUTO_WAIT_PLATE_ARRIVED;
					}				
				}
				else
				{
					m_nNextAutoStep = AUTO_CHECK_HAS_CORE;
				}
			}			
			break;
		}
	case AUTO_WAIT_PLATE_ARRIVED:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("等待料盘到达");
			
			if(IsNoHardware() || m_bDryRun)
			{
				Sleep(1000);
			}
			
			if(IsNoHardware() || m_bDryRun || m_Plate.HasPlate(0))
			{
				SetNeedSingal(FALSE);
				m_nNextAutoStep = AUTO_CHECK_HAS_CORE;
			}
			break;
		}
	case AUTO_CHECK_HAS_CORE:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("检查是否有电芯");
			if(IsNoHardware() || m_bDryRun || m_Plate.HasCore(0))
			{
				m_nNextAutoStep = AUTO_JACKING_PLATE;
			}
			else
			{
				m_nNextAutoStep = ATUO_WAIT_NEXT_NEED_PLATE_SINGAL;
			}
			break;
		}
	case AUTO_JACKING_PLATE:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("顶升料盘");
			if(IsNoHardware() || m_Plate.JackingPush(0))
			{
				//发送准备好信号
				if(SetFeedSingal(TRUE))
				{
					m_nNextAutoStepEx = 0;
					m_nNextAutoStep = AUTO_WAIT_FEED_END_SINGAL;
				}
			}
			break;
		}
	case AUTO_WAIT_FEED_END_SINGAL:
		{
			if(m_bShowWaitFeedSingal)
			{
				m_strMsg = m_bLangulageEng?_T(""):_T("等待上料完成");	
			}
					
			AutoOperationFeed();
			if(!GetFeedSingal())
			{
				SetFeedSingal(FALSE);
				m_nNextAutoStep = ATUO_WAIT_NEXT_NEED_PLATE_SINGAL;
			}
			break;
		}
	case ATUO_WAIT_NEXT_NEED_PLATE_SINGAL:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("等待下一模组要料信号");
			if(!m_Plate.IsMaterialFull() && GetNextNeedSingal())
			{
				if(IsNoHardware() || (m_Plate.JackingReturn(0) && m_Plate.DamperReturn(0)))
				{
					m_nNextAutoStep = ATUO_WAIT_FOLLOW_PLATE;
				}
			}
			break;
		}
	case ATUO_WAIT_FOLLOW_PLATE:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("料盘离开");
			if(IsNoHardware() || m_bDryRun || m_Plate.NoPlate(0))
			{
				OnPlateLeaved();
				if(IsNoHardware() || m_Plate.DamperPush(0))
				{
					m_nNextAutoStep = AUTO_WORK_END;
				}
			}
		}
		//工作完成
	case AUTO_WORK_END:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("工作完成");
			m_nNextAutoStep = AUTO_WORK_STAR;
			break;
		}
	default:
		{
			ASSERT(0);
			break;
		}
	}
}
void CRunProcessOnLoadOrOffPlate::AutoOperationOffLoad()
{
	switch(m_nNextAutoStep)
	{
		//开始工作
	case AUTO_WORK_STAR:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("开始工作");
			if(IsNoHardware() && !m_Plate.NoCore(0))  //有电芯，直接流向下一模组
			{
				m_nNextAutoStep = ATUO_WAIT_NEXT_NEED_PLATE_SINGAL;
				break;
			}
			if(IsNoHardware() || (m_Plate.DamperPush() && m_Plate.JackingReturn()))
			{
				if(IsNoHardware() || m_bDryRun || (m_Plate.GetCurPlateCount(0) < m_Plate.GetPlateCount(0)))
				{
					//发送要空料盘信号
					if(SetNeedSingal(TRUE))
					{
						m_nNextAutoStep = AUTO_WAIT_PLATE_ARRIVED;
					}				
				}
				else
				{
					m_nNextAutoStep = AUTO_CHECK_NO_CORE;
				}
			}			
			break;
		}
	case AUTO_WAIT_PLATE_ARRIVED:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("等待空料盘到达");
			if(IsNoHardware() || m_bDryRun || m_Plate.HasPlate(0))
			{
				SetNeedSingal(FALSE);
				m_nNextAutoStep = AUTO_CHECK_NO_CORE;
			}			
			break;
		}
	case AUTO_CHECK_NO_CORE:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("检查无电芯");
			if(IsNoHardware() || m_bDryRun || m_Plate.NoCore(0))
			{
				m_nNextAutoStep = AUTO_JACKING_PLATE;
			}
			else
			{
				m_nNextAutoStep = ATUO_WAIT_NEXT_NEED_PLATE_SINGAL;
			}
			break;
		}
	case AUTO_JACKING_PLATE:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("顶升料盘");
			if(IsNoHardware() || m_Plate.JackingPush(0))
			{
				//发送准备好信号
				if(SetFeedSingal(TRUE))
				{
					m_nNextAutoStep = AUTO_WAIT_FEED_END_SINGAL;
				}
			}
			break;
		}
	case AUTO_WAIT_FEED_END_SINGAL:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("等待下料完成");
			m_nNextAutoStepEx = 0;
			AutoOperationFeed();
			if(!GetFeedSingal())
			{
				m_nNextAutoStep = ATUO_WAIT_NEXT_NEED_PLATE_SINGAL;
			}
			break;
		}
	case ATUO_WAIT_NEXT_NEED_PLATE_SINGAL:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("等待下一模组要料信号");
			if(GetNextNeedSingal())
			{
				if(IsNoHardware() || (m_Plate.JackingReturn(0) && m_Plate.DamperReturn(0)))
				{
					m_nNextAutoStep = ATUO_WAIT_FOLLOW_PLATE;
				}
			}
			break;
		}
	case ATUO_WAIT_FOLLOW_PLATE:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("料盘离开");
			APP_Wait_Signal_Sleep();
			if(IsNoHardware() || m_bDryRun || m_Plate.NoPlate(0))
			{
				if(IsNoHardware() || m_Plate.DamperPush(0))
				{
					m_nNextAutoStep = AUTO_WORK_END;
				}
			}
			break;
		}
		//工作完成
	case AUTO_WORK_END:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("工作完成");
			m_nNextAutoStep = AUTO_WORK_STAR;
			break;
		}
	default:
		{
			ASSERT(0);
			break;
		}
	}
}

BOOL CRunProcessOnLoadOrOffPlate::InitializeConfig(CString strProcessModule)
{
	if (!CRunProcess::InitializeConfig(strProcessModule))
		return FALSE;
	return TRUE;
}


void CRunProcessOnLoadOrOffPlate::GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors)
{
	CRunProcess::GetHardwareConfig(pInputs,pOutputs,pMotors);
}

BOOL CRunProcessOnLoadOrOffPlate::CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{
	return TRUE;
}

BOOL CRunProcessOnLoadOrOffPlate::CheckOutputCanActive(COutput *pOutput)
{
	return TRUE;
}
//上料（不需要考虑其他模组）
void CRunProcessOnLoadOrOffPlate::AutoOperationFeed()
{
	return ;
}
BOOL CRunProcessOnLoadOrOffPlate::HasCore()
{
	if(m_Plate.HasCore(-1))
	{
		return TRUE;
	}
	return FALSE;
}
void CRunProcessOnLoadOrOffPlate::OnPlateLeaved()
{

}

void CRunProcessOnLoadOrOffPlate::MotorRun()
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
