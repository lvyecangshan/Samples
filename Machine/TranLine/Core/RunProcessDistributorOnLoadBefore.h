/****************************************************************************************
文件名：RunProcessDistributorOnLoadBefore.h
功能描述：入壳上料前分料(一次只分一个料盘)
历史修改记录：
*****************************************************************************************/
#pragma once
#include "../../RunProcess/RunProcessDistributorOne.h"

class CRunProcessDistributorOnLoadBefore : public CRunProcessDistributorOne
{
protected:
	

public:
	CRunProcessDistributorOnLoadBefore(int nID);
	virtual ~CRunProcessDistributorOnLoadBefore(void);

	BOOL InitializeConfig(CString strProcessModule);
	void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);
	BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove);
	BOOL CheckOutputCanActive(COutput *pOutput);
	

protected:
	void PowerUpOperation(void);
	void PowerUpRestart(void);
	void AutoOperation(void);


public:
	void Destory();
public:
	
	
};