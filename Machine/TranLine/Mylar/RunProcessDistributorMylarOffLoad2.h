/****************************************************************************************
�ļ�����RunProcessDistributorMylarOffLoad2.h
��������������������(һ��ֻ��һ������)
��ʷ�޸ļ�¼��
*****************************************************************************************/
#pragma once
#include "../../RunProcess/RunProcessDistributorOne.h"

class CRunProcessDistributorMylarOffLoad2 : public CRunProcessDistributorOne
{
protected:
	

public:
	CRunProcessDistributorMylarOffLoad2(int nID);
	virtual ~CRunProcessDistributorMylarOffLoad2(void);

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