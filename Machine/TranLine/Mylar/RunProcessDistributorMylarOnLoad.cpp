/****************************************************************************************
文件名：RunProcessDistributorMylarOnLoad.h
功能描述：分料器基类
历史修改记录：
*****************************************************************************************/
#include "StdAfx.h"
#include "RunProcessDistributorMylarOnLoad.h"

CRunProcessDistributorMylarOnLoad::CRunProcessDistributorMylarOnLoad(int nID) : CRunProcessDistributorOne(nID)
{
	PowerUpRestart();
}

CRunProcessDistributorMylarOnLoad::~CRunProcessDistributorMylarOnLoad(void)
{
	
}

void CRunProcessDistributorMylarOnLoad::PowerUpRestart()
{
	CRunProcessDistributorOne::PowerUpRestart();
	
}

void CRunProcessDistributorMylarOnLoad::PowerUpOperation(void)
{
	CRunProcessDistributorOne::PowerUpOperation();
}

BOOL CRunProcessDistributorMylarOnLoad::InitializeConfig(CString strProcessModule)
{
	if (!CRunProcessDistributorOne::InitializeConfig(strProcessModule))
		return FALSE;
	return TRUE;
}


void CRunProcessDistributorMylarOnLoad::AutoOperation(void)
{
	return CRunProcessDistributorOne::AutoOperation();
}

void CRunProcessDistributorMylarOnLoad::GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors)
{
	CRunProcessDistributorOne::GetHardwareConfig(pInputs,pOutputs,pMotors);
}

BOOL CRunProcessDistributorMylarOnLoad::CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{
	return TRUE;
}

BOOL CRunProcessDistributorMylarOnLoad::CheckOutputCanActive(COutput *pOutput)
{
	return TRUE;
}
