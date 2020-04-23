/****************************************************************************************
文件名：RunProcessElevatorCore.h
功能描述：升降机
历史修改记录：
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


//移动到接料位
BOOL CRunProcessElevatorCore::MoveToReceiveMaterialPostion()
{
	if(IsNoHardware())
	{
		return TRUE;
	}
	return APP_MotorMove(m_nMotorZ, SEND_MOTOR_RECEIVE_MATERIAL_POSTION);
}

//移动到上料位
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
	//如果线体料满，则返回FALSE
	return TRUE;
}







