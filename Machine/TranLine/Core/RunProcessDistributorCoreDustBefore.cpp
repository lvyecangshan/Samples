/****************************************************************************************
文件名：RunProcessDistributorCoreDustBefore.h
功能描述：电芯除尘前分料(一次分两个料盘)
历史修改记录：
*****************************************************************************************/
#include "StdAfx.h"
#include "RunProcessDistributorCoreDustBefore.h"
#include "../../MachineCtrl.h"

CRunProcessDistributorCoreDustBefore::CRunProcessDistributorCoreDustBefore(int nID) : CRunProcessDistributorTwo(nID)
{
	PowerUpRestart();
}

CRunProcessDistributorCoreDustBefore::~CRunProcessDistributorCoreDustBefore(void)
{
	
}

void CRunProcessDistributorCoreDustBefore::PowerUpRestart()
{
	CRunProcessDistributorTwo::PowerUpRestart();
	
}

void CRunProcessDistributorCoreDustBefore::PowerUpOperation(void)
{
	CRunProcessDistributorTwo::PowerUpOperation();
}

BOOL CRunProcessDistributorCoreDustBefore::InitializeConfig(CString strProcessModule)
{
	if (!CRunProcessDistributorTwo::InitializeConfig(strProcessModule))
		return FALSE;
	return TRUE;
}


void CRunProcessDistributorCoreDustBefore::AutoOperation(void)
{
	return CRunProcessDistributorTwo::AutoOperation();
}

void CRunProcessDistributorCoreDustBefore::GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors)
{
	CRunProcessDistributorTwo::GetHardwareConfig(pInputs,pOutputs,pMotors);
}

BOOL CRunProcessDistributorCoreDustBefore::CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{
	return TRUE;
}

BOOL CRunProcessDistributorCoreDustBefore::CheckOutputCanActive(COutput *pOutput)
{
	return TRUE;
}

BOOL CRunProcessDistributorCoreDustBefore::GetNextNeedSingal()
{	
	if(!MACHINE.m_pCoreDust->IsModuleEnable())
	{
		return TRUE;
	}
	return MACHINE.m_pCoreDust->GetNeedSingal();
}

void CRunProcessDistributorCoreDustBefore::MotorRun()
{
	if (m_Motor.GetMachinState()==MC_RUNNING)
	{
		m_Motor.AllMotorStart();

		while(m_Motor.GetMachinState()==MC_RUNNING)
		{
			//加满料判断
			Sleep(10);
		}
	}
	else if (m_Motor.GetMachinState() == MC_STOP_RUN && m_Motor.CountTime())   //延时1000ms停止
	{
		m_Motor.AllMotorStop();
	}
}