/****************************************************************************************
文件名：RunRunProcessCoreOnLoad1.h
功能描述：入壳机上料
历史修改记录：
*****************************************************************************************/
#pragma once

#include "RunProcessCoreOnLoad.h"

class CRunProcessCoreOnLoad1 : public CRunProcessCoreOnLoad
{
protected:
	
	

public:
	CRunProcessCoreOnLoad1(int nID);
	virtual ~CRunProcessCoreOnLoad1(void);

	BOOL InitializeConfig(CString strProcessModule);
	void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);
	BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove);
	BOOL CheckOutputCanActive(COutput *pOutput);

	void AutoOperationFeed();
	void OnPlateLeaved();

	BOOL GetNextNeedSingal();

	virtual void MotorRun();

protected:
	void PowerUpOperation(void);
	void PowerUpRestart(void);
	void AutoOperation(void);
	BOOL InitIO();
	

public:
	void Destory();
public:
	//满料感应
    int m_ICheckLineFull;

protected:	

	

	
};