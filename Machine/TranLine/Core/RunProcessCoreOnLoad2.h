/****************************************************************************************
文件名：RunProcessCoreOnLoad2.h
功能描述：入壳机上料
历史修改记录：
*****************************************************************************************/
#pragma once
#include "..\..\RunProcess\RunProcessOnLoadOrOffPlate.h"

class CRunProcessCoreOnLoad2 : public CRunProcessCoreOnLoad
{
protected:



public:
	CRunProcessCoreOnLoad2(int nID);
	virtual ~CRunProcessCoreOnLoad2(void);

	BOOL InitializeConfig(CString strProcessModule);
	void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);
	BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove);
	BOOL CheckOutputCanActive(COutput *pOutput);
	BOOL GetNextNeedSingal();
	void OnPlateLeaved();

protected:
	void PowerUpOperation(void);
	void PowerUpRestart(void);
	void AutoOperation(void);
	BOOL InitIO();

public:
	
public:
	void Destory();
public:
	//满料感应
	int m_ICheckLineFull;

protected:	




};