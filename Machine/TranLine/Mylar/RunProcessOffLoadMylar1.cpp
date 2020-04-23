/****************************************************************************************
文件名：RunRunProcessOffLoadMylar1.h
功能描述：下料模组
历史修改记录：
*****************************************************************************************/
#include "StdAfx.h"
#include "RunProcessOffLoadMylar1.h"
#include "../../MachineCtrl.h"

CRunProcessOffLoadMylar1::CRunProcessOffLoadMylar1(int nID) : CRunProcessOnLoadOrOffPlate(nID)
{
	PowerUpRestart();
}

CRunProcessOffLoadMylar1::~CRunProcessOffLoadMylar1(void)
{
	
}

void CRunProcessOffLoadMylar1::PowerUpRestart()
{
	CRunProcessOnLoadOrOffPlate::PowerUpRestart();
	
	
}

void CRunProcessOffLoadMylar1::PowerUpOperation(void)
{
	CRunProcessOnLoadOrOffPlate::PowerUpOperation();
}

void CRunProcessOffLoadMylar1::AutoOperation()
{
	return CRunProcessOnLoadOrOffPlate::AutoOperation();
}
BOOL CRunProcessOffLoadMylar1::InitializeConfig(CString strProcessModule)
{
	if (!CRunProcessOnLoadOrOffPlate::InitializeConfig(strProcessModule))
		return FALSE;
	return TRUE;
}


void CRunProcessOffLoadMylar1::GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors)
{
	CRunProcessOnLoadOrOffPlate::GetHardwareConfig(pInputs,pOutputs,pMotors);
}

BOOL CRunProcessOffLoadMylar1::CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{
	return TRUE;
}

BOOL CRunProcessOffLoadMylar1::CheckOutputCanActive(COutput *pOutput)
{
	return TRUE;
}

void CRunProcessOffLoadMylar1::AutoOperationFeed()
{
	//如果包膜机1不需要料,
	SetFeedSingal(FALSE);
	return;
	
	//如果下一线体已满，则直接返回
}

BOOL CRunProcessOffLoadMylar1::GetNextNeedSingal()
{
	//判断下段线体是否料满，如果料满，返回FALSE,否则返回TRUE
	return TRUE;
}

void CRunProcessOffLoadMylar1::MotorRun()
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