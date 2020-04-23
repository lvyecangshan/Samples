/****************************************************************************************
�ļ�����RunProcessDistributorMylarOffLoad1.h
��������������������
��ʷ�޸ļ�¼��
*****************************************************************************************/
#include "StdAfx.h"
#include "RunProcessDistributorMylarOffLoad1.h"
#include "../../MachineCtrl.h"

CRunProcessDistributorMylarOffLoad1::CRunProcessDistributorMylarOffLoad1(int nID) : CRunProcessDistributorOne(nID)
{
	PowerUpRestart();
}

CRunProcessDistributorMylarOffLoad1::~CRunProcessDistributorMylarOffLoad1(void)
{
	
}

void CRunProcessDistributorMylarOffLoad1::PowerUpRestart()
{
	CRunProcessDistributorOne::PowerUpRestart();
	
}

void CRunProcessDistributorMylarOffLoad1::PowerUpOperation(void)
{
	CRunProcessDistributorOne::PowerUpOperation();
}

BOOL CRunProcessDistributorMylarOffLoad1::InitializeConfig(CString strProcessModule)
{
	if (!CRunProcessDistributorOne::InitializeConfig(strProcessModule))
		return FALSE;
	return TRUE;
}


void CRunProcessDistributorMylarOffLoad1::AutoOperation(void)
{
	return CRunProcessDistributorOne::AutoOperation();
}

void CRunProcessDistributorMylarOffLoad1::GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors)
{
	CRunProcessDistributorOne::GetHardwareConfig(pInputs,pOutputs,pMotors);
}

BOOL CRunProcessDistributorMylarOffLoad1::CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{
	return TRUE;
}

BOOL CRunProcessDistributorMylarOffLoad1::CheckOutputCanActive(COutput *pOutput)
{
	return TRUE;
}
//��ȡ�����ź�
BOOL CRunProcessDistributorMylarOffLoad1::GetNextNeedSingal()
{
	if(MACHINE.m_pOffLoadMylar1 == NULL)
	{
		return TRUE;
	}
	if(!MACHINE.m_pOffLoadMylar1->IsModuleEnable())
	{
		return FALSE;
	}
	return MACHINE.m_pOffLoadMylar1->GetNeedSingal();
}

void CRunProcessDistributorMylarOffLoad1::MotorRun()
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