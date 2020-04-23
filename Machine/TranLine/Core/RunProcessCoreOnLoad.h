/****************************************************************************************
文件名：RunRunProcessCoreOnLoad.h
功能描述：入壳机上料
历史修改记录：
*****************************************************************************************/
#pragma once
#include "..\..\RunProcess\RunProcessOnLoadOrOffPlate.h"

class CRunProcessCoreOnLoad : public CRunProcessOnLoadOrOffPlate
{
protected:
	
public:
	CRunProcessCoreOnLoad(int nID);
	virtual ~CRunProcessCoreOnLoad(void);

	BOOL InitializeConfig(CString strProcessModule);
	void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);
	BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove);
	BOOL CheckOutputCanActive(COutput *pOutput);

	virtual void MotorRun(){return;};

protected:
	void PowerUpOperation(void);
	void PowerUpRestart(void);
	void AutoOperation(void);
	BOOL InitIO();
	
	
public:	
public:
	void Destory();
public:
	

protected:	

	
	
};