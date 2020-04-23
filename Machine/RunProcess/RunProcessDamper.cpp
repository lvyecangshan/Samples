/****************************************************************************************
文件名：RunProcessDamper.h
功能描述：阻挡器基类
历史修改记录：
*****************************************************************************************/
#include "StdAfx.h"
#include "RunProcessDamper.h"

CRunProcessDamper::CRunProcessDamper(int nID) : CRunProcess(nID)
{
	PowerUpRestart();
	InitIO();
	
	m_Motor.InitMotorThread();
}

CRunProcessDamper::~CRunProcessDamper(void)
{
	//m_Motor.ReleaseMotorThread();
}
BOOL CRunProcessDamper::InitIO()
{

	m_Damper.SetParent(this);
	m_Damper.AddDamper(_T("IDamperPush"),_T("IDamperReturn"),_T("ODamperReturn"),_T("IHasPlate"),_T("ICheckDirection"),_T("IFullCheck0"),_T("IFullCheck1"));
	
	m_Motor.SetParent(this);
	m_Motor.AddMotor(_T("OLinemotor0"),_T("OLinemotor1"),_T("OLinemotor2"));
	
	return  TRUE;
	
}

void CRunProcessDamper::PowerUpRestart()
{
	CRunProcess::PowerUpRestart();
	m_Motor.PowerUpRestart();
	m_nNextPowerUpStep = PU_SET_STOP_STATUS;
	m_nNextAutoStep = AUTO_SET_STOP_STATUS;
	m_nNextAutoStepEx = 0;

	m_bNeedPlate = FALSE;
	m_bFeedPlate = FALSE; 
	m_nNextAutoStepEx = 0; 
	
}

void CRunProcessDamper::PowerUpOperation(void)
{
	if (!IsModuleEnable())
	{
		CRun::PowerUpOperation();
		m_strMsg = m_bLangulageEng?_T("Moudle UnEnable"):_T("模块禁用");
		return;
	}

	switch(m_nNextPowerUpStep)
	{
	case PU_SET_STOP_STATUS:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("设置阻挡");
			if(IsNoHardware() || m_Damper.Push())
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
			m_nNextAutoStep = AUTO_SET_STOP_STATUS;
			break;
		}
	default:
		{
			ASSERT(0);
			break;
		}
	}
}

void CRunProcessDamper::AutoOperation()
{
	if (!IsModuleEnable())
	{
		m_strMsg = m_bLangulageEng?_T("Moudle UnEnable"):_T("模块禁用");
		Sleep(500);
		return;
	}

	APP_NoHardware_Sleep();
	static DWORD dwTime = 0;

	switch(m_nNextAutoStep)
	{
		//向发送要料信号
	case AUTO_SET_STOP_STATUS:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("设置阻挡");
			if(IsNoHardware() || m_Damper.Push())
			{
				SetNeedSingal(TRUE);
				dwTime = GetTickCount();
				m_nNextAutoStep = AUTO_WAIT_PLATE_ARRIVE;
			}
			break;
		}
	case AUTO_WAIT_PLATE_ARRIVE:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("等待料盘到达");
			if(IsNoHardware() || m_bDryRun)
			{
				if(GetTickCount() - dwTime > 1000)
				{
					SetNeedSingal(FALSE);					
					if(SetFeedSingal(TRUE))
					{
						m_nNextAutoStepEx = 0;
						m_nNextAutoStep = AUTO_WAIT_FEED_END_SINGAL;
					}					
				}
			}
			else if(m_Damper.HasMaterial(0))
			{	
				SetNeedSingal(FALSE);
				APP_Wait_Signal_Sleep();
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
			AutoOperationFeed();
			if(!GetFeedSingal())
			{
				m_nNextAutoStepEx = 0;
				m_nNextAutoStep = ATUO_WAIT_NEXT_NEED_PLATE_SINGAL;
			}
			break;
		}
	case ATUO_WAIT_NEXT_NEED_PLATE_SINGAL:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("等待下一模组要料信号");
			if(!m_Damper.IsMaterialFull() && GetNextNeedSingal())
			{
				if(IsNoHardware() || (m_Damper.Return()))
				{
					m_nNextAutoStep = ATUO_WAIT_FOLLOW_PLATE;
				}
			}			
			break;
		}
	case ATUO_WAIT_FOLLOW_PLATE:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("料盘离开");
			if(IsNoHardware() || m_bDryRun || !m_Damper.HasMaterial(0))
			{
				OnPlateLeaved();
				if(IsNoHardware() || m_Damper.Return(0))
				{
					
					m_nNextAutoStep = AUTO_WORK_END;
				}
			}
			break;
		}
	case AUTO_WORK_END:
		{
			m_strMsg = m_bLangulageEng?_T("Work end"):_T("工作完成");
			m_nNextAutoStep = AUTO_SET_STOP_STATUS;
			break;
		}
	default:
		{
			ASSERT(0);
			break;
		}
	}
}

void CRunProcessDamper::OnPlateLeaved()
{

}
void CRunProcessDamper::AutoOperationFeed(void)
{
	SetFeedSingal(FALSE);
}
BOOL CRunProcessDamper::InitializeConfig(CString strProcessModule)
{
	if (!CRunProcess::InitializeConfig(strProcessModule))
		return FALSE;
	return TRUE;
}


void CRunProcessDamper::GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors)
{
	CRunProcess::GetHardwareConfig(pInputs,pOutputs,pMotors);
}

BOOL CRunProcessDamper::CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{
	return TRUE;
}

BOOL CRunProcessDamper::CheckOutputCanActive(COutput *pOutput)
{
	return TRUE;
}

void CRunProcessDamper::MotorRun()
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