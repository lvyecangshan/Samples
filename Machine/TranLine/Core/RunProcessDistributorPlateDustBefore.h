/****************************************************************************************
文件名：RunProcessDistributorPlateDustBefore.h
功能描述：料盘除尘前分料(一次分两个料盘)
历史修改记录：
*****************************************************************************************/
#pragma once
#include "../../RunProcess/RunProcessDistributorTwo.h"
#include "../../MachineCtrl.h"

class CRunProcessDistributorPlateDustBefore : public CRunProcessDistributorTwo
{
protected:
	

public:
	CRunProcessDistributorPlateDustBefore(int nID);
	virtual ~CRunProcessDistributorPlateDustBefore(void);

	BOOL InitializeConfig(CString strProcessModule);
	void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);
	BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove);
	BOOL CheckOutputCanActive(COutput *pOutput);
	BOOL GetNextNeedSingal();

protected:
	void PowerUpOperation(void);
	void PowerUpRestart(void);
	void AutoOperation(void);


public:
	void Destory();
public:
	
	
};