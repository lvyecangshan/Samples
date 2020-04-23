/****************************************************************************************
文件名：RunProcessDistributorCoreDustBefore.h
功能描述：电芯除尘前分料(一次分两个料盘)
历史修改记录：
*****************************************************************************************/
#pragma once
#include "../../RunProcess/RunProcessDistributorTwo.h"

class CRunProcessDistributorCoreDustBefore : public CRunProcessDistributorTwo
{
protected:
	

public:
	CRunProcessDistributorCoreDustBefore(int nID);
	virtual ~CRunProcessDistributorCoreDustBefore(void);

	BOOL InitializeConfig(CString strProcessModule);
	void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);
	BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove);
	BOOL CheckOutputCanActive(COutput *pOutput);
	BOOL GetNextNeedSingal();

	virtual void MotorRun();

protected:
	void PowerUpOperation(void);
	void PowerUpRestart(void);
	void AutoOperation(void);


public:
	void Destory();
public:
	
	
};