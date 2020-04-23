/****************************************************************************************
文件名：RunRunProcessOffLoadMylar1.h
功能描述：下料模组
历史修改记录：
*****************************************************************************************/
#pragma once
#include "../../RunProcess/RunProcessOnLoadOrOffPlate.h"

class CRunProcessOffLoadMylar1 : public CRunProcessOnLoadOrOffPlate
{
protected:
public:
	CRunProcessOffLoadMylar1(int nID);
	virtual ~CRunProcessOffLoadMylar1(void);

	BOOL InitializeConfig(CString strProcessModule);
	void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);
	BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove);
	BOOL CheckOutputCanActive(COutput *pOutput);
	void AutoOperationFeed();
	BOOL GetNextNeedSingal();

	virtual void MotorRun();

protected:
	void PowerUpOperation(void);
	void PowerUpRestart(void);
	
	void AutoOperation(void);
	
public:
	
public:
	void Destory();
public:
	//满料感应
    int m_ICheckLineFull;



	
};