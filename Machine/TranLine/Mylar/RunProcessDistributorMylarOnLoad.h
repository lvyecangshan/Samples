/****************************************************************************************
�ļ�����RunProcessDistributorMylarOnLoad.h
��������������������(һ��ֻ��һ������)
��ʷ�޸ļ�¼��
*****************************************************************************************/
#pragma once
#include "../../RunProcess/RunProcessDistributorOne.h"

class CRunProcessDistributorMylarOnLoad : public CRunProcessDistributorOne
{

public:
	CRunProcessDistributorMylarOnLoad(int nID);
	virtual ~CRunProcessDistributorMylarOnLoad(void);

	BOOL InitializeConfig(CString strProcessModule);
	void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);
	BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove);
	BOOL CheckOutputCanActive(COutput *pOutput);

	virtual void MotorRun(){return;};

protected:
	void PowerUpOperation(void);
	void PowerUpRestart(void);
	void AutoOperation(void);


public:
	void Destory();
public:
	
	
};