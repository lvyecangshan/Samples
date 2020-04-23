/****************************************************************************************
�ļ�����RunProcessDamperCoreDown06.h
������������Ĥ�赲���ϲ�06��
��ʷ�޸ļ�¼��
*****************************************************************************************/
#include "StdAfx.h"
#include "../../RunProcess/RunProcessDamper.h"
#include "../../MachineCtrl.h"

CRunProcessDamperCoreDown06::CRunProcessDamperCoreDown06(int nID) : CRunProcessDamper(nID)
{
	PowerUpRestart();
}

CRunProcessDamperCoreDown06::~CRunProcessDamperCoreDown06(void)
{
	
}

void CRunProcessDamperCoreDown06::PowerUpRestart()
{
	CRunProcessDamper::PowerUpRestart();

}

void CRunProcessDamperCoreDown06::PowerUpOperation(void)
{
	CRunProcessDamper::PowerUpOperation();
}

void CRunProcessDamperCoreDown06::AutoOperation()
{
	CRunProcessDamper::AutoOperation();
}

BOOL CRunProcessDamperCoreDown06::InitializeConfig(CString strProcessModule)
{
	if (!CRunProcessDamper::InitializeConfig(strProcessModule))
		return FALSE;
	return TRUE;
}


void CRunProcessDamperCoreDown06::GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors)
{
	CRunProcessDamper::GetHardwareConfig(pInputs,pOutputs,pMotors);
}

BOOL CRunProcessDamperCoreDown06::CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{
	return TRUE;
}

BOOL CRunProcessDamperCoreDown06::CheckOutputCanActive(COutput *pOutput)
{
	return TRUE;
}

//���һ��ģ���Ƿ�����
BOOL CRunProcessDamperCoreDown06::GetNextNeedSingal()
{
	/*
	if(MACHINE.m_pDamperMylarUp04 == NULL)
	{
		return TRUE;
	}
	if(!MACHINE.m_pDamperMylarUp04->IsModuleEnable())
	{
		return TRUE;
	}
	return MACHINE.m_pDamperMylarUp04->GetNextNeedSingal();
	*/
	return TRUE;
}

void CRunProcessDamperCoreDown06::MotorRun()
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