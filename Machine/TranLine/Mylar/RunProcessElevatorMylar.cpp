/****************************************************************************************
文件名：RunProcessElevatorMylar.h
功能描述：升降机
历史修改记录：
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


//移动到接料位
BOOL CRunProcessElevatorMylar::MoveToReceiveMaterialPostion()
{
	if(IsNoHardware())
	{
		return TRUE;
	}
	return APP_MotorMove(m_nMotorZ, SEND_MOTOR_RECEIVE_MATERIAL_POSTION);
}

//移动到上料位
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
	//如果线体料满，则返回FALSE
	return FALSE;
}

