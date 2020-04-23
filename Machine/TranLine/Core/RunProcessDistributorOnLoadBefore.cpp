/****************************************************************************************
文件名：RunProcessDistributorOnLoadBefore.h
功能描述：入壳上料前分料(一次只分一个料盘)
历史修改记录：
*****************************************************************************************/
#include "StdAfx.h"
#include "RunProcessDistributorOnLoadBefore.h"
#include "../../MachineCtrl.h"

CRunProcessDistributorOnLoadBefore::CRunProcessDistributorOnLoadBefore(int nID) : CRunProcessDistributorOne(nID)
{
	PowerUpRestart();
}

CRunProcessDistributorOnLoadBefore::~CRunProcessDistributorOnLoadBefore(void)
{
	
}

void CRunProcessDistributorOnLoadBefore::PowerUpRestart()
{
	CRunProcessDistributorOne::PowerUpRestart();
	
}

void CRunProcessDistributorOnLoadBefore::PowerUpOperation(void)
{
	CRunProcessDistributorOne::PowerUpOperation();
}

BOOL CRunProcessDistributorOnLoadBefore::InitializeConfig(CString strProcessModule)
{
	if (!CRunProcessDistributorOne::InitializeConfig(strProcessModule))
		return FALSE;
	return TRUE;
}


void CRunProcessDistributorOnLoadBefore::AutoOperation(void)
{
	return CRunProcessDistributorOne::AutoOperation();
}

void CRunProcessDistributorOnLoadBefore::GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors)
{
	CRunProcessDistributorOne::GetHardwareConfig(pInputs,pOutputs,pMotors);
}

BOOL CRunProcessDistributorOnLoadBefore::CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{
	return TRUE;
}

BOOL CRunProcessDistributorOnLoadBefore::CheckOutputCanActive(COutput *pOutput)
{
	return TRUE;
}
