/****************************************************************************************
文件名：RunRunProcessCoreOnLoad.h
功能描述：入壳机上料
历史修改记录：
*****************************************************************************************/
#include "StdAfx.h"
#include "RunProcessCoreOnLoad.h"
#include "../../MachineCtrl.h"

CRunProcessCoreOnLoad::CRunProcessCoreOnLoad(int nID) : CRunProcessOnLoadOrOffPlate(nID)
{
	PowerUpRestart();
	InitIO();
}

CRunProcessCoreOnLoad::~CRunProcessCoreOnLoad(void)
{
	
}
BOOL CRunProcessCoreOnLoad::InitIO()
{
	
	return TRUE;

}
void CRunProcessCoreOnLoad::PowerUpRestart()
{
	CRunProcessOnLoadOrOffPlate::PowerUpRestart();
	

	
}

void CRunProcessCoreOnLoad::PowerUpOperation(void)
{
	CRunProcessOnLoadOrOffPlate::PowerUpOperation();
}

void CRunProcessCoreOnLoad::AutoOperation()
{
	CRunProcessOnLoadOrOffPlate::AutoOperation();
}

BOOL CRunProcessCoreOnLoad::InitializeConfig(CString strProcessModule)
{
	if (!CRunProcessOnLoadOrOffPlate::InitializeConfig(strProcessModule))
		return FALSE;
	return TRUE;
}


void CRunProcessCoreOnLoad::GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors)
{
	CRunProcessOnLoadOrOffPlate::GetHardwareConfig(pInputs,pOutputs,pMotors);
}

BOOL CRunProcessCoreOnLoad::CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{
	return TRUE;
}

BOOL CRunProcessCoreOnLoad::CheckOutputCanActive(COutput *pOutput)
{
	return TRUE;
}



