/****************************************************************************************
�ļ�����RunProcessDamperMylarUp03.h
������������Ĥ�赲���ϲ�03��
��ʷ�޸ļ�¼��
*****************************************************************************************/
#include "StdAfx.h"
#include "../../RunProcess/RunProcessDamper.h"
#include "../../MachineCtrl.h"

CRunProcessDamperMylarUp03::CRunProcessDamperMylarUp03(int nID) : CRunProcessDamper(nID)
{
	PowerUpRestart();
}

CRunProcessDamperMylarUp03::~CRunProcessDamperMylarUp03(void)
{
	
}

void CRunProcessDamperMylarUp03::PowerUpRestart()
{
	CRunProcessDamper::PowerUpRestart();

}

void CRunProcessDamperMylarUp03::PowerUpOperation(void)
{
	CRunProcessDamper::PowerUpOperation();
}

void CRunProcessDamperMylarUp03::AutoOperation()
{
	CRunProcessDamper::AutoOperation();
}

BOOL CRunProcessDamperMylarUp03::InitializeConfig(CString strProcessModule)
{
	if (!CRunProcessDamper::InitializeConfig(strProcessModule))
		return FALSE;
	return TRUE;
}


void CRunProcessDamperMylarUp03::GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors)
{
	CRunProcessDamper::GetHardwareConfig(pInputs,pOutputs,pMotors);
}

BOOL CRunProcessDamperMylarUp03::CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{
	return TRUE;
}

BOOL CRunProcessDamperMylarUp03::CheckOutputCanActive(COutput *pOutput)
{
	return TRUE;
}

//���һ��ģ���Ƿ�����
BOOL CRunProcessDamperMylarUp03::CheckNextLineIsFull()
{
	if(MACHINE.m_pDamperMylarUp04 == NULL)
	{
		return TRUE;
	}
	if(!MACHINE.m_pDamperMylarUp04->IsModuleEnable())
	{
		return TRUE;
	}
	return 0;//MACHINE.m_pDamperMylarUp04->CheckLineFull();
}

void CRunProcessDamperMylarUp03::MotorRun()
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