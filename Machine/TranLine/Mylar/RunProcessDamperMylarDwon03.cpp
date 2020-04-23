/****************************************************************************************
�ļ�����RunProcessDamperMylarDown03.h
������������Ĥ�赲���ϲ�03��
��ʷ�޸ļ�¼��
*****************************************************************************************/
#include "StdAfx.h"
#include "../../RunProcess/RunProcessDamper.h"

CRunProcessDamperMylarDown03::CRunProcessDamperMylarDown03(int nID) : CRunProcessDamper(nID)
{
	PowerUpRestart();
}

CRunProcessDamperMylarDown03::~CRunProcessDamperMylarDown03(void)
{
	
}

void CRunProcessDamperMylarDown03::PowerUpRestart()
{
	CRunProcessDamper::PowerUpRestart();

}

void CRunProcessDamperMylarDown03::PowerUpOperation(void)
{
	CRunProcessDamper::PowerUpOperation();
}

void CRunProcessDamperMylarDown03::AutoOperation()
{
	CRunProcessDamper::AutoOperation();
}

BOOL CRunProcessDamperMylarDown03::InitializeConfig(CString strProcessModule)
{
	if (!CRunProcessDamper::InitializeConfig(strProcessModule))
		return FALSE;
	return TRUE;
}


void CRunProcessDamperMylarDown03::GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors)
{
	CRunProcessDamper::GetHardwareConfig(pInputs,pOutputs,pMotors);
}

BOOL CRunProcessDamperMylarDown03::CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{
	return TRUE;
}

BOOL CRunProcessDamperMylarDown03::CheckOutputCanActive(COutput *pOutput)
{
	return TRUE;
}

//���һ��ģ���Ƿ�����
BOOL CRunProcessDamperMylarDown03::GetNextNeedSingal()
{
	if(MACHINE.m_pDamperMylarUp05 == NULL)
	{
		return TRUE;
	}
	if(!MACHINE.m_pDamperMylarUp05->IsModuleEnable())
	{
		return TRUE;
	}
	return TRUE;//MACHINE.m_pDamperMylarUp05->CheckLineFull();
}

void CRunProcessDamperMylarDown03::MotorRun()
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