/****************************************************************************************
�ļ�����RunProcessDistributorMylarOffLoad2.h
��������������������
��ʷ�޸ļ�¼��
*****************************************************************************************/
#include "StdAfx.h"
#include "RunProcessDistributorMylarOffLoad2.h"
#include "../../MachineCtrl.h"

CRunProcessDistributorMylarOffLoad2::CRunProcessDistributorMylarOffLoad2(int nID) : CRunProcessDistributorOne(nID)
{
	PowerUpRestart();
}

CRunProcessDistributorMylarOffLoad2::~CRunProcessDistributorMylarOffLoad2(void)
{
	
}

void CRunProcessDistributorMylarOffLoad2::PowerUpRestart()
{
	CRunProcessDistributorOne::PowerUpRestart();
	
}

void CRunProcessDistributorMylarOffLoad2::PowerUpOperation(void)
{
	CRunProcessDistributorOne::PowerUpOperation();
}

BOOL CRunProcessDistributorMylarOffLoad2::InitializeConfig(CString strProcessModule)
{
	if (!CRunProcessDistributorOne::InitializeConfig(strProcessModule))
		return FALSE;
	return TRUE;
}


void CRunProcessDistributorMylarOffLoad2::AutoOperation(void)
{
	return CRunProcessDistributorOne::AutoOperation();
}

void CRunProcessDistributorMylarOffLoad2::GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors)
{
	CRunProcessDistributorOne::GetHardwareConfig(pInputs,pOutputs,pMotors);
}

BOOL CRunProcessDistributorMylarOffLoad2::CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{
	return TRUE;
}

BOOL CRunProcessDistributorMylarOffLoad2::CheckOutputCanActive(COutput *pOutput)
{
	return TRUE;
}
BOOL CRunProcessDistributorMylarOffLoad2::GetNextNeedSingal()
{
	if(MACHINE.m_pOffLoadMylar2 == NULL)
	{
		return TRUE;
	}
	if(!MACHINE.m_pOffLoadMylar2->IsModuleEnable())
	{
		return FALSE;
	}
	return MACHINE.m_pOffLoadMylar2->GetNeedSingal();
}

void CRunProcessDistributorMylarOffLoad2::MotorRun()
{
	if (m_Motor.GetMachinState()==MC_RUNNING)
	{
		m_Motor.AllMotorStart();

		while(m_Motor.GetMachinState()==MC_RUNNING)
		{
			//�������ж�
			Sleep(10);
		}
	}
	else if (m_Motor.GetMachinState() == MC_STOP_RUN && m_Motor.CountTime())   //��ʱ1000msֹͣ
	{
		m_Motor.AllMotorStop();
	}
}
