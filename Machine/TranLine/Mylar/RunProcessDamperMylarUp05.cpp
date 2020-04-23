/****************************************************************************************
文件名：RunProcessDamperMylarUp05.h
功能描述：包膜阻挡器上层05段
历史修改记录：
*****************************************************************************************/
#include "StdAfx.h"
#include "../../RunProcess/RunProcessDamper.h"

CRunProcessDamperMylarUp05::CRunProcessDamperMylarUp05(int nID) : CRunProcessDamper(nID)
{
	PowerUpRestart();
}

CRunProcessDamperMylarUp05::~CRunProcessDamperMylarUp05(void)
{
	
}

void CRunProcessDamperMylarUp05::PowerUpRestart()
{
	CRunProcessDamper::PowerUpRestart();

}

void CRunProcessDamperMylarUp05::PowerUpOperation(void)
{
	CRunProcessDamper::PowerUpOperation();
}

void CRunProcessDamperMylarUp05::AutoOperation()
{
	CRunProcessDamper::AutoOperation();
}

BOOL CRunProcessDamperMylarUp05::InitializeConfig(CString strProcessModule)
{
	if (!CRunProcessDamper::InitializeConfig(strProcessModule))
		return FALSE;
	return TRUE;
}


void CRunProcessDamperMylarUp05::GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors)
{
	CRunProcessDamper::GetHardwareConfig(pInputs,pOutputs,pMotors);
}

BOOL CRunProcessDamperMylarUp05::CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{
	return TRUE;
}

BOOL CRunProcessDamperMylarUp05::CheckOutputCanActive(COutput *pOutput)
{
	return TRUE;
}

//检测一下模组是否料满
BOOL CRunProcessDamperMylarUp05::CheckNextLineIsFull()
{
	BOOL bRet = APP_INPUTS_IsOn(m_ICheckNextLineFull);
	return bRet;
}

void CRunProcessDamperMylarUp05::MotorRun()
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