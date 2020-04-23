/****************************************************************************************
�ļ�����RunRunProcessDistributorOne.h
��������������������(һ��ֻ��һ����)
��ʷ�޸ļ�¼��
*****************************************************************************************/
#include "StdAfx.h"
#include "RunProcessDistributorOne.h"

CRunProcessDistributorOne::CRunProcessDistributorOne(int nID) : CRunProcessDistributor(nID)
{
	PowerUpRestart();
}

CRunProcessDistributorOne::~CRunProcessDistributorOne(void)
{
	
}

void CRunProcessDistributorOne::PowerUpRestart()
{
	CRunProcessDistributor::PowerUpRestart();
	
}

void CRunProcessDistributorOne::PowerUpOperation(void)
{
	return CRunProcessDistributor::PowerUpOperation();
}

void CRunProcessDistributorOne::AutoOperation()
{
	CRunProcessDistributor::AutoOperation();
}

BOOL CRunProcessDistributorOne::InitializeConfig(CString strProcessModule)
{
	if (!CRunProcessDistributor::InitializeConfig(strProcessModule))
		return FALSE;
	return TRUE;
}


void CRunProcessDistributorOne::GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors)
{
	CRunProcessDistributor::GetHardwareConfig(pInputs,pOutputs,pMotors);
}

BOOL CRunProcessDistributorOne::CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{
	return TRUE;
}

BOOL CRunProcessDistributorOne::CheckOutputCanActive(COutput *pOutput)
{
	return TRUE;
}


BOOL CRunProcessDistributorOne::CheckHasPlate()
{
	if(m_Distriutor.HasMaterial(1))
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CRunProcessDistributorOne::CheckNoPlate()
{
	
	if(!m_Distriutor.HasMaterial(1))
	{
		return TRUE;
	}
	return FALSE;
}

void CRunProcessDistributorOne::MotorRun()
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
