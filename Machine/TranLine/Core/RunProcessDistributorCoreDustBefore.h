/****************************************************************************************
�ļ�����RunProcessDistributorCoreDustBefore.h
������������о����ǰ����(һ�η���������)
��ʷ�޸ļ�¼��
*****************************************************************************************/
#pragma once
#include "../../RunProcess/RunProcessDistributorTwo.h"

class CRunProcessDistributorCoreDustBefore : public CRunProcessDistributorTwo
{
protected:
	

public:
	CRunProcessDistributorCoreDustBefore(int nID);
	virtual ~CRunProcessDistributorCoreDustBefore(void);

	BOOL InitializeConfig(CString strProcessModule);
	void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);
	BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove);
	BOOL CheckOutputCanActive(COutput *pOutput);
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