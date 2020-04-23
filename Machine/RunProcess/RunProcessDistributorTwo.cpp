/****************************************************************************************
�ļ�����RunRunProcessDistributorTwo.h
��������������������(һ�η���������)
��ʷ�޸ļ�¼��
*****************************************************************************************/
#include "StdAfx.h"
#include "RunProcessDistributorTwo.h"

CRunProcessDistributorTwo::CRunProcessDistributorTwo(int nID) : CRunProcessDistributor(nID)
{
	PowerUpRestart();

}

CRunProcessDistributorTwo::~CRunProcessDistributorTwo(void)
{
	
}

void CRunProcessDistributorTwo::PowerUpRestart()
{
	CRunProcessDistributor::PowerUpRestart();
	
}

void CRunProcessDistributorTwo::PowerUpOperation(void)
{
	return CRunProcessDistributor::PowerUpOperation();
}

void CRunProcessDistributorTwo::AutoOperation()
{
	CRunProcessDistributor::AutoOperation();
}

BOOL CRunProcessDistributorTwo::InitializeConfig(CString strProcessModule)
{
	if (!CRunProcessDistributor::InitializeConfig(strProcessModule))
		return FALSE;
	return TRUE;
}


void CRunProcessDistributorTwo::GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors)
{
	CRunProcessDistributor::GetHardwareConfig(pInputs,pOutputs,pMotors);
}

BOOL CRunProcessDistributorTwo::CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{
	return TRUE;
}

BOOL CRunProcessDistributorTwo::CheckOutputCanActive(COutput *pOutput)
{
	return TRUE;
}


BOOL CRunProcessDistributorTwo::CheckHasPlate()
{
	if(m_Distriutor.HasMaterial(0) && m_Distriutor.HasMaterial(1))
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CRunProcessDistributorTwo::CheckNoPlate()
{

	if(!m_Distriutor.HasMaterial(0) && !m_Distriutor.HasMaterial(1))
	{
		return TRUE;
	}
	return FALSE;
}

void CRunProcessDistributorTwo::MotorRun()
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