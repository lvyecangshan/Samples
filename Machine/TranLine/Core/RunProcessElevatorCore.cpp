/****************************************************************************************
�ļ�����RunProcessElevatorCore.h
����������������
��ʷ�޸ļ�¼��
*****************************************************************************************/
#include "StdAfx.h"
#include "RunProcessElevatorCore.h"
#include "../../MachineCtrl.h"

CRunProcessElevatorCore::CRunProcessElevatorCore(int nID) : CRunProcessElevator(nID)
{
	PowerUpRestart();
}

CRunProcessElevatorCore::~CRunProcessElevatorCore(void)
{
	
}

void CRunProcessElevatorCore::PowerUpRestart()
{
	CRunProcessElevator::PowerUpRestart();
	
}

void CRunProcessElevatorCore::PowerUpOperation(void)
{
	CRunProcessElevator::PowerUpOperation();
}

void CRunProcessElevatorCore::AutoOperation()
{
	return CRunProcessElevator::AutoOperation();
}

BOOL CRunProcessElevatorCore::InitializeConfig(CString strProcessModule)
{
	if (!CRunProcess::InitializeConfig(strProcessModule))
		return FALSE;
	return TRUE;
}


void CRunProcessElevatorCore::GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors)
{
	CRunProcess::GetHardwareConfig(pInputs,pOutputs,pMotors);
}

BOOL CRunProcessElevatorCore::CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{
	return TRUE;
}

BOOL CRunProcessElevatorCore::CheckOutputCanActive(COutput *pOutput)
{
	return TRUE;
}


//�ƶ�������λ
BOOL CRunProcessElevatorCore::MoveToReceiveMaterialPostion()
{
	if(IsNoHardware())
	{
		return TRUE;
	}
	return APP_MotorMove(m_nMotorZ, SEND_MOTOR_RECEIVE_MATERIAL_POSTION);
}

//�ƶ�������λ
BOOL CRunProcessElevatorCore::MoveToFeedingMaterialPostion()
{
	if(IsNoHardware())
	{
		return TRUE;
	}
	return APP_MotorMove(m_nMotorZ, SEND_MOTOR_FEEDING_MATERIAL_POSTION);
	
}

BOOL CRunProcessElevatorCore::GetNextNeedSingal()
{
	//��������������򷵻�FALSE
	return TRUE;
}







