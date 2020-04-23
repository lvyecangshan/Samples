/****************************************************************************************
文件名：RunRunProcessCoreOnLoad2.h
功能描述：入壳机上料
历史修改记录：
*****************************************************************************************/
#include "StdAfx.h"
#include "RunProcessCoreOnLoad2.h"
#include "../../MachineCtrl.h"

CRunProcessCoreOnLoad2::CRunProcessCoreOnLoad2(int nID) : CRunProcessCoreOnLoad(nID)
{
	PowerUpRestart();
	InitIO();
}

CRunProcessCoreOnLoad2::~CRunProcessCoreOnLoad2(void)
{

}
BOOL CRunProcessCoreOnLoad2::InitIO()
{

	return TRUE;

}
void CRunProcessCoreOnLoad2::PowerUpRestart()
{
	CRunProcessCoreOnLoad::PowerUpRestart();



}

void CRunProcessCoreOnLoad2::PowerUpOperation(void)
{
	return CRunProcessCoreOnLoad::PowerUpOperation();
}

void CRunProcessCoreOnLoad2::AutoOperation()
{
	CRunProcessCoreOnLoad::AutoOperation();
}

BOOL CRunProcessCoreOnLoad2::InitializeConfig(CString strProcessModule)
{
	if (!CRunProcessCoreOnLoad::InitializeConfig(strProcessModule))
		return FALSE;
	return TRUE;
}


void CRunProcessCoreOnLoad2::GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors)
{
	CRunProcessCoreOnLoad::GetHardwareConfig(pInputs,pOutputs,pMotors);
}

BOOL CRunProcessCoreOnLoad2::CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{
	return TRUE;
}

BOOL CRunProcessCoreOnLoad2::CheckOutputCanActive(COutput *pOutput)
{
	return TRUE;
}

BOOL CRunProcessCoreOnLoad2::GetNextNeedSingal()
{
	if(!MACHINE.m_pElevatorCore->IsModuleEnable())
	{
		return FALSE;
	}
	return MACHINE.m_pElevatorCore->GetNeedSingal();
}

void CRunProcessCoreOnLoad2::OnPlateLeaved()
{
	if(IsNoHardware() || m_bDryRun)
	{
		MACHINE.m_pElevatorCore->SetNeedSingal(FALSE);
	}
	
}




