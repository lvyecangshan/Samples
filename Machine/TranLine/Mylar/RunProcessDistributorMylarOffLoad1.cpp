/****************************************************************************************
文件名：RunProcessDistributorMylarOffLoad1.h
功能描述：分料器基类
历史修改记录：
*****************************************************************************************/
#include "StdAfx.h"
#include "RunProcessDistributorMylarOffLoad1.h"
#include "../../MachineCtrl.h"

CRunProcessDistributorMylarOffLoad1::CRunProcessDistributorMylarOffLoad1(int nID) : CRunProcessDistributorOne(nID)
{
	PowerUpRestart();
}

CRunProcessDistributorMylarOffLoad1::~CRunProcessDistributorMylarOffLoad1(void)
{
	
}

void CRunProcessDistributorMylarOffLoad1::PowerUpRestart()
{
	CRunProcessDistributorOne::PowerUpRestart();
	
}

void CRunProcessDistributorMylarOffLoad1::PowerUpOperation(void)
{
	CRunProcessDistributorOne::PowerUpOperation();
}

BOOL CRunProcessDistributorMylarOffLoad1::InitializeConfig(CString strProcessModule)
{
	if (!CRunProcessDistributorOne::InitializeConfig(strProcessModule))
		return FALSE;
	return TRUE;
}


void CRunProcessDistributorMylarOffLoad1::AutoOperation(void)
{
	return CRunProcessDistributorOne::AutoOperation();
}

void CRunProcessDistributorMylarOffLoad1::GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors)
{
	CRunProcessDistributorOne::GetHardwareConfig(pInputs,pOutputs,pMotors);
}

BOOL CRunProcessDistributorMylarOffLoad1::CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{
	return TRUE;
}

BOOL CRunProcessDistributorMylarOffLoad1::CheckOutputCanActive(COutput *pOutput)
{
	return TRUE;
}
//获取放料信号
BOOL CRunProcessDistributorMylarOffLoad1::GetNextNeedSingal()
{
	if(MACHINE.m_pOffLoadMylar1 == NULL)
	{
		return TRUE;
	}
	if(!MACHINE.m_pOffLoadMylar1->IsModuleEnable())
	{
		return FALSE;
	}
	return MACHINE.m_pOffLoadMylar1->GetNeedSingal();
}

void CRunProcessDistributorMylarOffLoad1::MotorRun()
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