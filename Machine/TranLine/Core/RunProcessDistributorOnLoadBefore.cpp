/****************************************************************************************
�ļ�����RunProcessDistributorOnLoadBefore.h
�����������������ǰ����(һ��ֻ��һ������)
��ʷ�޸ļ�¼��
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
