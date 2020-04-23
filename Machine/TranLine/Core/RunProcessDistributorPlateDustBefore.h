/****************************************************************************************
�ļ�����RunProcessDistributorPlateDustBefore.h
�������������̳���ǰ����(һ�η���������)
��ʷ�޸ļ�¼��
*****************************************************************************************/
#pragma once
#include "../../RunProcess/RunProcessDistributorTwo.h"
#include "../../MachineCtrl.h"

class CRunProcessDistributorPlateDustBefore : public CRunProcessDistributorTwo
{
protected:
	

public:
	CRunProcessDistributorPlateDustBefore(int nID);
	virtual ~CRunProcessDistributorPlateDustBefore(void);

	BOOL InitializeConfig(CString strProcessModule);
	void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);
	BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove);
	BOOL CheckOutputCanActive(COutput *pOutput);
	BOOL GetNextNeedSingal();

protected:
	void PowerUpOperation(void);
	void PowerUpRestart(void);
	void AutoOperation(void);


public:
	void Destory();
public:
	
	
};