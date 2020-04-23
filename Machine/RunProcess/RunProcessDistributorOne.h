/****************************************************************************************
文件名：RunRunProcessDistributorOne.h
功能描述：分料器基类(一次只分一个料盘)
历史修改记录：
*****************************************************************************************/
#pragma once
#include "RunProcessDistributor.h"
#include "..\parts\LineMotor.h"

class CRunProcessDistributorOne : public CRunProcessDistributor
{
protected:

public:
	CRunProcessDistributorOne(int nID);
	virtual ~CRunProcessDistributorOne(void);

	BOOL InitializeConfig(CString strProcessModule);
	void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);
	BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove);
	BOOL CheckOutputCanActive(COutput *pOutput);

protected:
	void PowerUpOperation(void);
	void PowerUpRestart(void);
	void AutoOperation(void);
	//判断是否有料
	virtual BOOL CheckHasPlate();
	virtual BOOL CheckNoPlate();

public:

	virtual void MotorRun();

public:
	void Destory();
public:
	
	
};