/****************************************************************************************
�ļ�����RunRunProcessOffLoadMylar1.h
��������������ģ��
��ʷ�޸ļ�¼��
*****************************************************************************************/
#include "StdAfx.h"
#include "RunProcessOffLoadMylar1.h"
#include "../../MachineCtrl.h"

CRunProcessOffLoadMylar1::CRunProcessOffLoadMylar1(int nID) : CRunProcessOnLoadOrOffPlate(nID)
{
	PowerUpRestart();
}

CRunProcessOffLoadMylar1::~CRunProcessOffLoadMylar1(void)
{
	
}

void CRunProcessOffLoadMylar1::PowerUpRestart()
{
	CRunProcessOnLoadOrOffPlate::PowerUpRestart();
	
	
}

void CRunProcessOffLoadMylar1::PowerUpOperation(void)
{
	CRunProcessOnLoadOrOffPlate::PowerUpOperation();
}

void CRunProcessOffLoadMylar1::AutoOperation()
{
	return CRunProcessOnLoadOrOffPlate::AutoOperation();
}
BOOL CRunProcessOffLoadMylar1::InitializeConfig(CString strProcessModule)
{
	if (!CRunProcessOnLoadOrOffPlate::InitializeConfig(strProcessModule))
		return FALSE;
	return TRUE;
}


void CRunProcessOffLoadMylar1::GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors)
{
	CRunProcessOnLoadOrOffPlate::GetHardwareConfig(pInputs,pOutputs,pMotors);
}

BOOL CRunProcessOffLoadMylar1::CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{
	return TRUE;
}

BOOL CRunProcessOffLoadMylar1::CheckOutputCanActive(COutput *pOutput)
{
	return TRUE;
}

void CRunProcessOffLoadMylar1::AutoOperationFeed()
{
	//�����Ĥ��1����Ҫ��,
	SetFeedSingal(FALSE);
	return;
	
	//�����һ������������ֱ�ӷ���
}

BOOL CRunProcessOffLoadMylar1::GetNextNeedSingal()
{
	//�ж��¶������Ƿ��������������������FALSE,���򷵻�TRUE
	return TRUE;
}

void CRunProcessOffLoadMylar1::MotorRun()
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