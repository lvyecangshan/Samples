/****************************************************************************************
�ļ�����RunRunProcessOffLoadMylar1.h
��������������ģ��
��ʷ�޸ļ�¼��
*****************************************************************************************/
#include "StdAfx.h"
#include "RunProcessOffLoadMylar1.h"
#include "../../MachineCtrl.h"

CRunProcessOffLoadMylar2::CRunProcessOffLoadMylar2(int nID) : CRunProcessOnLoadOrOffPlate(nID)
{
	PowerUpRestart();
}

CRunProcessOffLoadMylar2::~CRunProcessOffLoadMylar2(void)
{
	
}

void CRunProcessOffLoadMylar2::PowerUpRestart()
{
	CRunProcessOnLoadOrOffPlate::PowerUpRestart();
	
}

void CRunProcessOffLoadMylar2::PowerUpOperation(void)
{
	return CRunProcessOnLoadOrOffPlate::PowerUpOperation();
}

void CRunProcessOffLoadMylar2::AutoOperation()
{
	return CRunProcessOnLoadOrOffPlate::AutoOperation();
}
BOOL CRunProcessOffLoadMylar2::InitializeConfig(CString strProcessModule)
{
	if (!CRunProcessOnLoadOrOffPlate::InitializeConfig(strProcessModule))
		return FALSE;
	return TRUE;
}


void CRunProcessOffLoadMylar2::GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors)
{
	CRunProcessOnLoadOrOffPlate::GetHardwareConfig(pInputs,pOutputs,pMotors);
}

BOOL CRunProcessOffLoadMylar2::CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{
	return TRUE;
}

BOOL CRunProcessOffLoadMylar2::CheckOutputCanActive(COutput *pOutput)
{
	return TRUE;
}

BOOL CRunProcessOffLoadMylar2::GetNextNeedSingal()
{
	//�ж��¶������Ƿ��������������������FALSE,���򷵻�TRUE
	return TRUE;
}







