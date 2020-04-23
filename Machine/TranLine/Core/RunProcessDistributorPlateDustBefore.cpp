/****************************************************************************************
文件名：RunProcessDistributorPlateDustBefore.h
功能描述：电芯除尘前分料(一次分两个料盘)
历史修改记录：
*****************************************************************************************/
#include "StdAfx.h"
#include "RunProcessDistributorPlateDustBefore.h"

CRunProcessDistributorPlateDustBefore::CRunProcessDistributorPlateDustBefore(int nID) : CRunProcessDistributorTwo(nID)
{
	PowerUpRestart();
}

CRunProcessDistributorPlateDustBefore::~CRunProcessDistributorPlateDustBefore(void)
{
	
}

void CRunProcessDistributorPlateDustBefore::PowerUpRestart()
{
	CRunProcessDistributorTwo::PowerUpRestart();
	
}

void CRunProcessDistributorPlateDustBefore::PowerUpOperation(void)
{
	CRunProcessDistributorTwo::PowerUpOperation();
}

BOOL CRunProcessDistributorPlateDustBefore::InitializeConfig(CString strProcessModule)
{
	if (!CRunProcessDistributorTwo::InitializeConfig(strProcessModule))
		return FALSE;
	return TRUE;
}


void CRunProcessDistributorPlateDustBefore::AutoOperation(void)
{
	return CRunProcessDistributorTwo::AutoOperation();
}

void CRunProcessDistributorPlateDustBefore::GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors)
{
	CRunProcessDistributorTwo::GetHardwareConfig(pInputs,pOutputs,pMotors);
}

BOOL CRunProcessDistributorPlateDustBefore::CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{
	return TRUE;
}

BOOL CRunProcessDistributorPlateDustBefore::CheckOutputCanActive(COutput *pOutput)
{
	return TRUE;
}

//获取放料信号
BOOL CRunProcessDistributorPlateDustBefore::GetNextNeedSingal()
{
	if(MACHINE.m_pPlateDust == NULL)
	{
		return TRUE;
	}
	if(!MACHINE.m_pPlateDust->IsModuleEnable())
	{
		return TRUE;

	}
	return MACHINE.m_pPlateDust->GetNextNeedSingal();
}