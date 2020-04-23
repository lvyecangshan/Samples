/****************************************************************************************
文件名：RunProcessDamperMylarDown04.h
功能描述：包膜阻挡器上层04段
历史修改记录：
*****************************************************************************************/
#pragma once
#include "../../RunProcess/RunProcessDamper.h"

class CRunProcessDamperMylarDown04 : public CRunProcessDamper
{
protected:


public:
	CRunProcessDamperMylarDown04(int nID);
	virtual ~CRunProcessDamperMylarDown04(void);

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