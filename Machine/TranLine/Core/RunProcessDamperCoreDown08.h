/****************************************************************************************
文件名：RunProcessDamperCoreDown08.h
功能描述：包膜阻挡器上层03段
历史修改记录：
*****************************************************************************************/
#pragma once
#include "../../RunProcess/RunProcessDamper.h"

class CRunProcessDamperCoreDown08 : public CRunProcessDamper
{
protected:


public:
	CRunProcessDamperCoreDown08(int nID);
	virtual ~CRunProcessDamperCoreDown08(void);

	BOOL InitializeConfig(CString strProcessModule);
	void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);
	BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove);
	BOOL CheckOutputCanActive(COutput *pOutput);

	//检测一下模组是否料满
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