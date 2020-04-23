/****************************************************************************************
�ļ�����RunProcessElevatorMylar.h
����������������
��ʷ�޸ļ�¼��
*****************************************************************************************/
#include "StdAfx.h"
#include "RunProcessElevatorMylar.h"
#include "../../MachineCtrl.h"

CRunProcessElevatorMylar::CRunProcessElevatorMylar(int nID) : CRunProcessElevator(nID)
{
	PowerUpRestart();
	
}

CRunProcessElevatorMylar::~CRunProcessElevatorMylar(void)
{
	
}

void CRunProcessElevatorMylar::PowerUpRestart()
{
	CRunProcessElevator::PowerUpRestart();
	
}

void CRunProcessElevatorMylar::PowerUpOperation(void)
{
	CRunProcessElevator::PowerUpOperation();
}

void CRunProcessElevatorMylar::AutoOperation()
{
	return CRunProcessElevator::AutoOperation();
}

BOOL CRunProcessElevatorMylar::InitializeConfig(CString strProcessModule)
{
	if (!CRunProcess::InitializeConfig(strProcessModule))
		return FALSE;
	return TRUE;
}


void CRunProcessElevatorMylar::GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors)
{
	CRunProcess::GetHardwareConfig(pInputs,pOutputs,pMotors);
}

BOOL CRunProcessElevatorMylar::CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove)
{
	return TRUE;
}

BOOL CRunProcessElevatorMylar::CheckOutputCanActive(COutput *pOutput)
{
	return TRUE;
}


//�ƶ�������λ
BOOL CRunProcessElevatorMylar::MoveToReceiveMaterialPostion()
{
	if(IsNoHardware())
	{
		return TRUE;
	}
	return APP_MotorMove(m_nMotorZ, SEND_MOTOR_RECEIVE_MATERIAL_POSTION);
}

//�ƶ�������λ
BOOL CRunProcessElevatorMylar::MoveToFeedingMaterialPostion()
{
	if(IsNoHardware())
	{
		return TRUE;
	}
	return APP_MotorMove(m_nMotorZ, SEND_MOTOR_FEEDING_MATERIAL_POSTION);
	
}

BOOL CRunProcessElevatorMylar::GetNextNeedSingal()
{
	//��������������򷵻�FALSE
	return FALSE;
}

