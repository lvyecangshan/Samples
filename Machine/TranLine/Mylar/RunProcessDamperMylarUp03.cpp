/****************************************************************************************
文件名：RunProcessDamperMylarUp03.h
功能描述：包膜阻挡器上层03段
历史修改记录：
*****************************************************************************************/
#include "StdAfx.h"
#include "../../RunProcess/RunProcessDamper.h"
#include "../../MachineCtrl.h"

CRunProcessDamperMylarUp03::CRunProcessDamperMylarUp03(int nID) : CRunProcessDamper(nID)
{
	PowerUpRestart();
}

CRunProcessDamperMylarUp03::~CRunProcessDamperMylarUp03(void)
{
	
}

void CRunProcessDamperMylarUp03::PowerUpRestart()
{
	CRunProcessDamper::PowerUpRestart();

}

void CRunProcessDamperMylarUp03::PowerUpOperation(void)
{
	CRunProcessDamper::PowerUpOperation();
}

void CRunProcessDamperMylarUp03::AutoOperation()
{
	CRunProcessDamper::AutoOperation();
}

BOOL CRunProcessDamperMylarUp03::InitializeConfig(CString strProcessModule)
{
	if (!CRunProcessDamper::InitializeConfig(strProcessModule))
		return FALSE;
	return TRUE;
}


void CRunProcessDamperMylarUp03::GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors)
{
	CRunProcessDamper::GetHardwareConfig(pInputs,pOutputs,pMotors);
}

BOOL CRunProcessDamperMylarUp03::CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{
	return TRUE;
}

BOOL CRunProcessDamperMylarUp03::CheckOutputCanActive(COutput *pOutput)
{
	return TRUE;
}

//检测一下模组是否料满
BOOL CRunProcessDamperMylarUp03::CheckNextLineIsFull()
{
	if(MACHINE.m_pDamperMylarUp04 == NULL)
	{
		return TRUE;
	}
	if(!MACHINE.m_pDamperMylarUp04->IsModuleEnable())
	{
		return TRUE;
	}
	return 0;//MACHINE.m_pDamperMylarUp04->CheckLineFull();
}

void CRunProcessDamperMylarUp03::MotorRun()
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