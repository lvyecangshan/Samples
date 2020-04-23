/****************************************************************************************
文件名：RunProcessElevatorMylar.h
功能描述：升降机
历史修改记录：
*****************************************************************************************/
#pragma once
#include "../../RunProcess/RunProcessElevator.h"

class CRunProcessElevatorMylar : public CRunProcessElevator
{
protected:
	
	enum MOTOR_LOC_Z
	{
		SEND_MOTOR_WAIT_POSTIN=0,  //等待
		SEND_MOTOR_RECEIVE_MATERIAL_POSTION,//接料位
		SEND_MOTOR_FEEDING_MATERIAL_POSTION,//送料位
	};

public:
	CRunProcessElevatorMylar(int nID);
	virtual ~CRunProcessElevatorMylar(void);

	BOOL InitializeConfig(CString strProcessModule);
	void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);
	BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove);
	BOOL CheckOutputCanActive(COutput *pOutput);

protected:
	void PowerUpOperation(void);
	void PowerUpRestart(void);
	void AutoOperation(void);

	//移动到接料位
	BOOL MoveToReceiveMaterialPostion();
	//移动到上料位
	BOOL MoveToFeedingMaterialPostion();



  
   BOOL GetNextNeedSingal();


public:
	void Destory();
public:
	//满料感应
    int m_ICheckLineFull;
private:
	
};