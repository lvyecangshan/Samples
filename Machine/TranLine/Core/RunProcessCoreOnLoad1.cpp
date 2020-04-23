/****************************************************************************************
文件名：RunRunProcessCoreOnLoad1.h
功能描述：入壳机上料
历史修改记录：
*****************************************************************************************/
#include "StdAfx.h"
#include "RunProcessCoreOnLoad1.h"
#include "../../MachineCtrl.h"

CRunProcessCoreOnLoad1::CRunProcessCoreOnLoad1(int nID) : CRunProcessCoreOnLoad(nID)
{
	PowerUpRestart();
	InitIO();
}

CRunProcessCoreOnLoad1::~CRunProcessCoreOnLoad1(void)
{
	
}
BOOL CRunProcessCoreOnLoad1::InitIO()
{
	
	return TRUE;

}
void CRunProcessCoreOnLoad1::PowerUpRestart()
{
	CRunProcessCoreOnLoad::PowerUpRestart();
	

	
}

void CRunProcessCoreOnLoad1::PowerUpOperation(void)
{
	return CRunProcessCoreOnLoad::PowerUpOperation();
}

void CRunProcessCoreOnLoad1::AutoOperation()
{
	CRunProcessCoreOnLoad::AutoOperation();
}

BOOL CRunProcessCoreOnLoad1::InitializeConfig(CString strProcessModule)
{
	if (!CRunProcessCoreOnLoad::InitializeConfig(strProcessModule))
		return FALSE;
	return TRUE;
}


void CRunProcessCoreOnLoad1::GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors)
{
	CRunProcessCoreOnLoad::GetHardwareConfig(pInputs,pOutputs,pMotors);
}

BOOL CRunProcessCoreOnLoad1::CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{
	return TRUE;
}


BOOL CRunProcessCoreOnLoad1::CheckOutputCanActive(COutput *pOutput)
{
	return TRUE;
}


void CRunProcessCoreOnLoad1::OnPlateLeaved()
{
	if(!MACHINE.m_pCoreOnLoad2->IsModuleEnable())
	{
		return ;
	}
	
	MACHINE.m_pCoreOnLoad2->SetNeedSingal(FALSE);
	return ;	
}
void CRunProcessCoreOnLoad1::AutoOperationFeed()
{
	if(!MACHINE.m_pCoreOnLoad2->IsModuleEnable())
	{
		return;
	}
	if(MACHINE.m_pCoreOnLoad2->GetNeedSingal())
	{
		SetFeedSingal(FALSE);
	}
}

BOOL CRunProcessCoreOnLoad1::GetNextNeedSingal()
{
	if(!MACHINE.m_pCoreOnLoad2->IsModuleEnable())
	{
		return FALSE;
	}
	return MACHINE.m_pCoreOnLoad2->GetNeedSingal();
}

void CRunProcessCoreOnLoad1::MotorRun()
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


