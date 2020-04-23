/****************************************************************************************
文件名：RunRunProcessOffLoad.h
功能描述：下料模组
历史修改记录：
*****************************************************************************************/
#include "StdAfx.h"
#include "RunProcessOffLoad.h"
#include "../../MachineCtrl.h"

CRunProcessOffLoad::CRunProcessOffLoad(int nID) : CRunProcess(nID)
{
	PowerUpRestart();
	InitIO();
}

CRunProcessOffLoad::~CRunProcessOffLoad(void)
{
	
}
BOOL CRunProcessOffLoad::InitIO()
{
	
	m_IDamperPush = -1;
	m_IDamperReturn=-1;
	m_IJackingPush=-1;
	m_IJackingReturn=-1;
	m_IHasPlate=-1;
	m_ICheckCore=-1;
	m_IHasCore=-1;
	m_ODamperReturn=-1;
	m_OJackingPush=-1;
	m_OJackingReturn=-1;

	APP_Add_Input(&m_IDamperPush,_T("IDamperPush"));
	APP_Add_Input(&m_IDamperReturn,_T("IDamperReturn"));
	APP_Add_Input(&m_IJackingPush,_T("IJackingPush"));
	APP_Add_Input(&m_IJackingReturn,_T("IJackingReturn"));
	APP_Add_Input(&m_IHasPlate,_T("IHasPlate"));
	APP_Add_Input(&m_ICheckCore,_T("ICheckCore"));
	APP_Add_Input(&m_IHasCore,_T("IHasCore"));

	APP_Add_Output(&m_ODamperReturn,_T("ODamperReturn"));
	APP_Add_Output(&m_OJackingPush,_T("OJackingPush"));
	APP_Add_Output(&m_OJackingReturn,_T("OJackingReturn"));

	return TRUE;
}

void CRunProcessOffLoad::PowerUpRestart()
{
	CRunProcess::PowerUpRestart();
	m_nNextPowerUpStep = PU_SET_STOP_STATUS;
	m_nNextAutoStep = AUTO_SEND_STAR_SIGNAL;
	m_bRequirePallet = FALSE;
	
}

void CRunProcessOffLoad::PowerUpOperation(void)
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
	case PU_SET_STOP_STATUS:
		{
			m_strMsg = m_bLangulageEng?_T("Wait Safe Signal"):_T("阻挡开启");
			if(IsNoHardware() || (DamperPush() && JackingReturn()))
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
			m_nNextAutoStep = AUTO_SEND_STAR_SIGNAL;
			break;
		}
	default:
		{
			ASSERT(0);
			break;
		}
	}
}

void CRunProcessOffLoad::AutoOperation()
{
	if (!IsModuleEnable())
	{
		m_strMsg = m_bLangulageEng?_T("Moudle UnEnable"):_T("模块禁用");
		APP_NoHardware_Sleep();
		return;
	}

	APP_NoHardware_Sleep();
	
	switch(m_nNextAutoStep)
	{
		
	case AUTO_SEND_STAR_SIGNAL:
		{
			m_strMsg = m_bLangulageEng?_T("Send Require Signal"):_T("开始工作信号");
			APP_Wait_Signal_Sleep();
			if(IsNoHardware()  || (DamperPush() && JackingReturn()))
			{
				m_nNextAutoStep = AUTO_WAIT_PLATE_ARRIVE;
			}
			break;
		}
	case AUTO_WAIT_PLATE_ARRIVE:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("等待料盘到达");
			APP_Wait_Signal_Sleep();
			if(IsNoHardware() || m_bDryRun || CheckHasPlate())
			{
				if( CheckHasCore())   //有电芯
				{
					if(CheckNextRequirePlate())  //下一模组是否需要料
					{
						if(JackingReturn())
						{
							m_nNextAutoStep = AUTO_WAIT_MATERIAL_FLOW;
						}						
					}
				}
				else   //无电芯
				{
					if(CheckPlatFormNeedPlate())  //当前工位需要料盘
					{
						if(IsNoHardware() || JackingPush())
						{
							m_nNextAutoStep = AUTO_SEND_MATERIAL_READY_SIGNAL;  //发送料盘准备好信号
						}
					}
					else    //当前工位不需要料盘
					{
						if(CheckNextRequirePlate())  //下一模组是需要料
						{
							m_nNextAutoStep = AUTO_WAIT_MATERIAL_FLOW;  //空料盘流向下一模组
						}
						else
						{
							if(JackingPush())
							{
								m_nNextAutoStep = AUTO_SEND_MATERIAL_READY_SIGNAL;  //发送料盘准备好信号
							}
						}

					}
				}
			}
			break;
		}
	case AUTO_WAIT_MATERIAL_FLOW:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("等待料盘流向一下模组");
			APP_Wait_Signal_Sleep();
			//给上一模组发送可下料信号
			if(IsNoHardware() || m_bDryRun )
			{
				m_nNextAutoStep = AUTO_WORK_END;
			}
			else 
			{
				if(JackingReturn())
				{
					if(!CheckHasPlate())
					{
						m_nNextAutoStep = AUTO_WORK_END;
					}
				}
			}
			break;
		}
	case AUTO_SEND_MATERIAL_READY_SIGNAL:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("发送料盘准备好信号");
			APP_Wait_Signal_Sleep();
			if(IsNoHardware() || SendPlateReadySignal(TRUE))
			{
				m_nNextAutoStep = AUTO_WAIT_FEEDING_END_SIGNAL;
			}
			break;
		}
	case AUTO_WAIT_FEEDING_END_SIGNAL:
		{
			m_strMsg = m_bLangulageEng?_T(""):_T("等待取料完成信号");

			APP_Wait_Signal_Sleep();
			if(IsNoHardware())
			{
				APP_Wait_Signal_Sleep();
				m_nNextAutoStep = AUTO_WAIT_MATERIAL_FLOW;
				break;
			}
			if(GetFeedingEndSignal())
			{
				m_nNextAutoStep = AUTO_WAIT_MATERIAL_FLOW;
			}
			break;
		}
	
		//工作完成
	case AUTO_WORK_END:
		{
			m_strMsg = m_bLangulageEng?_T("Work end"):_T("工作完成");
			APP_Wait_Signal_Sleep();
			if(IsNoHardware() || SendPlateReadySignal(FALSE))
			{
				if(SendRequiredSignal())
				{
					m_nNextAutoStep = AUTO_SEND_STAR_SIGNAL;
				}
			}
			
			break;
		}

	default:
		{
			ASSERT(0);
			break;
		}
	}
}

BOOL CRunProcessOffLoad::InitializeConfig(CString strProcessModule)
{
	if (!CRunProcess::InitializeConfig(strProcessModule))
		return FALSE;
	return TRUE;
}


void CRunProcessOffLoad::GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors)
{
	CRunProcess::GetHardwareConfig(pInputs,pOutputs,pMotors);
}

BOOL CRunProcessOffLoad::CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{
	return TRUE;
}

BOOL CRunProcessOffLoad::CheckOutputCanActive(COutput *pOutput)
{
	return TRUE;
}

//阻挡推出
BOOL CRunProcessOffLoad::DamperPush()
{
	BOOL bRet = APP_OUTPUTS_Off(m_ODamperReturn);
	return bRet;
}
//阻挡回退
BOOL CRunProcessOffLoad::DamperReturn()
{
	BOOL bRet = APP_OUTPUTS_On(m_ODamperReturn);
	return bRet;
	
}

//下料顶升气缸推出
BOOL CRunProcessOffLoad::JackingPush()
{
	BOOL bRet = APP_OUTPUTS_On(m_OJackingPush) && APP_OUTPUTS_Off(m_OJackingReturn);
	return bRet;
}
//下料顶升气缸回退
BOOL CRunProcessOffLoad::JackingReturn()
{
	BOOL bRet = APP_OUTPUTS_Off(m_OJackingPush) && APP_OUTPUTS_On(m_OJackingReturn);
	return bRet;
	
}

//判断是否有料盘
BOOL CRunProcessOffLoad::CheckHasPlate()
{
	return APP_INPUTS_IsOn(m_IHasPlate);
}

//判断是否有料盘
BOOL CRunProcessOffLoad::CheckHasCore()
{
	if(IsNoHardware() || m_bDryRun)
	{
		return FALSE;
	}
	return APP_INPUTS_IsOn(m_IHasCore);
}
//判断下一模组是否要料
BOOL CRunProcessOffLoad::CheckNextRequirePlate()
{
	return FALSE;
}

//发送料盘准备好信号
BOOL CRunProcessOffLoad::SendPlateReadySignal(BOOL bReady)
{
	BOOL bRet = FALSE;
	if(bReady)
	{
		bRet = APP_OUTPUTS_On(m_OReadPlateSignal);
	}
	else
	{
		bRet = APP_OUTPUTS_Off(m_OReadPlateSignal);
	}
	 
	return bRet;
}
//获取下料完成信号
BOOL CRunProcessOffLoad::GetFeedingEndSignal()
{	
	BOOL bRet = APP_INPUTS_IsOn(m_IFeedingPlateSignal);
	return bRet;
}

//机台是否要料
BOOL CRunProcessOffLoad::CheckPlatFormNeedPlate()
{
	BOOL bRet = APP_INPUTS_IsOn(m_IFeedingPlateSignal);
	return bRet;
}




