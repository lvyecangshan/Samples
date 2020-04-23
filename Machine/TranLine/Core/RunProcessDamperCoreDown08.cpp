/****************************************************************************************
�ļ�����RunProcessDamperCoreDown08.h
������������Ĥ�赲���ϲ�08��
��ʷ�޸ļ�¼��
*****************************************************************************************/
#include "StdAfx.h"
#include "../../RunProcess/RunProcessDamper.h"
#include "../../MachineCtrl.h"

CRunProcessDamperCoreDown08::CRunProcessDamperCoreDown08(int nID) : CRunProcessDamper(nID)
{
	PowerUpRestart();
}

CRunProcessDamperCoreDown08::~CRunProcessDamperCoreDown08(void)
{
	
}

void CRunProcessDamperCoreDown08::PowerUpRestart()
{
	CRunProcessDamper::PowerUpRestart();

}

void CRunProcessDamperCoreDown08::PowerUpOperation(void)
{
	CRunProcessDamper::PowerUpOperation();
}

void CRunProcessDamperCoreDown08::AutoOperation()
{
	CRunProcessDamper::AutoOperation();
}

BOOL CRunProcessDamperCoreDown08::InitializeConfig(CString strProcessModule)
{
	if (!CRunProcessDamper::InitializeConfig(strProcessModule))
		return FALSE;
	return TRUE;
}


void CRunProcessDamperCoreDown08::GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors)
{
	CRunProcessDamper::GetHardwareConfig(pInputs,pOutputs,pMotors);
}

BOOL CRunProcessDamperCoreDown08::CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{
	return TRUE;
}

BOOL CRunProcessDamperCoreDown08::CheckOutputCanActive(COutput *pOutput)
{
	return TRUE;
}

//���һ��ģ���Ƿ�����
BOOL CRunProcessDamperCoreDown08::GetNextNeedSingal()
{
	/*
	if(MACHINE.m_pDamperCoreDown06 == NULL)
	{
		return TRUE;
	}
	if(!MACHINE.m_pDamperCoreDown06->IsModuleEnable())
	{
		return TRUE;
	}
	return MACHINE.m_pDamperCoreDown06->GetNextNeedSingal();
	*/
	return TRUE;
}

void CRunProcessDamperCoreDown08::MotorRun()
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
