/****************************************************************************************
文件名：RunProcessDistributorMylarOffLoad1.h
功能描述：分料器基类(一次只分一个料盘)
历史修改记录：
*****************************************************************************************/
#pragma once
#include "RunProcess/RunProcessDistributorOne.h"

class CRunProcessDistributorMylarOffLoad1 : public CRunProcessDistributorOne
{

public:
	CRunProcessDistributorMylarOffLoad1(int nID);
	virtual ~CRunProcessDistributorMylarOffLoad1(void);

	BOOL InitializeConfig(CString strProcessModule);
	void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);
	BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove);
	BOOL CheckOutputCanActive(COutput *pOutput);
	BOOL GetNextNeedSingal();
	void AutoOperationFeed();

	virtual void MotorRun();

protected:
	void PowerUpOperation(void);
	void PowerUpRestart(void);
	void AutoOperation(void);


public:
	void Destory();
public:
	
	
};