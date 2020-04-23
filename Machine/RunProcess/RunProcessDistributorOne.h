/****************************************************************************************
�ļ�����RunRunProcessDistributorOne.h
��������������������(һ��ֻ��һ������)
��ʷ�޸ļ�¼��
*****************************************************************************************/
#pragma once
#include "RunProcessDistributor.h"
#include "..\parts\LineMotor.h"

class CRunProcessDistributorOne : public CRunProcessDistributor
{
protected:

public:
	CRunProcessDistributorOne(int nID);
	virtual ~CRunProcessDistributorOne(void);

	BOOL InitializeConfig(CString strProcessModule);
	void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);
	BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove);
	BOOL CheckOutputCanActive(COutput *pOutput);

protected:
	void PowerUpOperation(void);
	void PowerUpRestart(void);
	void AutoOperation(void);
	//�ж��Ƿ�����
	virtual BOOL CheckHasPlate();
	virtual BOOL CheckNoPlate();

public:

	virtual void MotorRun();

public:
	void Destory();
public:
	
	
};