/****************************************************************************************
文件名：RunProcessDamperMylarUp04.h
功能描述：包膜阻挡器上层04段
历史修改记录：
*****************************************************************************************/
#include "StdAfx.h"
#include "../../RunProcess/RunProcessDamper.h"

CRunProcessDamperMylarUp04::CRunProcessDamperMylarUp04(int nID) : CRunProcessDamper(nID)
{
	PowerUpRestart();
}

CRunProcessDamperMylarUp04::~CRunProcessDamperMylarUp04(void)
{
	
}

void CRunProcessDamperMylarUp04::PowerUpRestart()
{
	CRunProcessDamper::PowerUpRestart();

}

void CRunProcessDamperMylarUp04::PowerUpOperation(void)
{
	CRunProcessDamper::PowerUpOperation();
}

void CRunProcessDamperMylarUp04::AutoOperation()
{
	CRunProcessDamper::AutoOperation();
}

BOOL CRunProcessDamperMylarUp04::InitializeConfig(CString strProcessModule)
{
	if (!CRunProcessDamper::InitializeConfig(strProcessModule))
		return FALSE;
	return TRUE;
}


void CRunProcessDamperMylarUp04::GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors)
{
	CRunProcessDamper::GetHardwareConfig(pInputs,pOutputs,pMotors);
}

BOOL CRunProcessDamperMylarUp04::CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{
	return TRUE;
}

BOOL CRunProcessDamperMylarUp04::CheckOutputCanActive(COutput *pOutput)
{
	return TRUE;
}

//检测一下模组是否料满
BOOL CRunProcessDamperMylarUp04::CheckNextLineIsFull()
{
	if(MACHINE.m_pDamperMylarUp05 == NULL)
	{
		return TRUE;
	}
	if(!MACHINE.m_pDamperMylarUp05->IsModuleEnable())
	{
		return TRUE;
	}
	return 0;//MACHINE.m_pDamperMylarUp05->CheckLineFull();
}

void CRunProcessDamperMylarUp04::MotorRun()
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