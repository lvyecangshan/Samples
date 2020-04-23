/****************************************************************************************
文件名：RunProcessDamperCoreDown06.h
功能描述：包膜阻挡器上层06段
历史修改记录：
*****************************************************************************************/
#include "StdAfx.h"
#include "../../RunProcess/RunProcessDamper.h"
#include "../../MachineCtrl.h"

CRunProcessDamperCoreDown06::CRunProcessDamperCoreDown06(int nID) : CRunProcessDamper(nID)
{
	PowerUpRestart();
}

CRunProcessDamperCoreDown06::~CRunProcessDamperCoreDown06(void)
{
	
}

void CRunProcessDamperCoreDown06::PowerUpRestart()
{
	CRunProcessDamper::PowerUpRestart();

}

void CRunProcessDamperCoreDown06::PowerUpOperation(void)
{
	CRunProcessDamper::PowerUpOperation();
}

void CRunProcessDamperCoreDown06::AutoOperation()
{
	CRunProcessDamper::AutoOperation();
}

BOOL CRunProcessDamperCoreDown06::InitializeConfig(CString strProcessModule)
{
	if (!CRunProcessDamper::InitializeConfig(strProcessModule))
		return FALSE;
	return TRUE;
}


void CRunProcessDamperCoreDown06::GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors)
{
	CRunProcessDamper::GetHardwareConfig(pInputs,pOutputs,pMotors);
}

BOOL CRunProcessDamperCoreDown06::CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{
	return TRUE;
}

BOOL CRunProcessDamperCoreDown06::CheckOutputCanActive(COutput *pOutput)
{
	return TRUE;
}

//检测一下模组是否料满
BOOL CRunProcessDamperCoreDown06::GetNextNeedSingal()
{
	/*
	if(MACHINE.m_pDamperMylarUp04 == NULL)
	{
		return TRUE;
	}
	if(!MACHINE.m_pDamperMylarUp04->IsModuleEnable())
	{
		return TRUE;
	}
	return MACHINE.m_pDamperMylarUp04->GetNextNeedSingal();
	*/
	return TRUE;
}

void CRunProcessDamperCoreDown06::MotorRun()
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