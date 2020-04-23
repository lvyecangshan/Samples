/****************************************************************************************
文件名：RunProcessDamperMylarUp05.h
功能描述：包膜阻挡器上层05段
历史修改记录：
*****************************************************************************************/
#pragma once
#include "../../RunProcess/RunProcessDamper.h"

class CRunProcessDamperMylarUp05 : public CRunProcessDamper
{
protected:


public:
	CRunProcessDamperMylarUp05(int nID);
	virtual ~CRunProcessDamperMylarUp05(void);

	BOOL InitializeConfig(CString strProcessModule);
	void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);
	BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove);
	BOOL CheckOutputCanActive(COutput *pOutput);

	//检测一下模组是否料满
	BOOL CheckNextLineIsFull();

	virtual void MotorRun();

protected:
	void PowerUpOperation(void);
	void PowerUpRestart(void);
	void AutoOperation(void);

public:
	void Destory();
private:	
	int m_ICheckNextLineFull;    //6段线体料满感应 X2500	线体满料感应

	
};