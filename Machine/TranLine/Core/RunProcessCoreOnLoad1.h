/****************************************************************************************
�ļ�����RunRunProcessCoreOnLoad1.h
������������ǻ�����
��ʷ�޸ļ�¼��
*****************************************************************************************/
#pragma once

#include "RunProcessCoreOnLoad.h"

class CRunProcessCoreOnLoad1 : public CRunProcessCoreOnLoad
{
protected:
	
	

public:
	CRunProcessCoreOnLoad1(int nID);
	virtual ~CRunProcessCoreOnLoad1(void);

	BOOL InitializeConfig(CString strProcessModule);
	void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);
	BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove);
	BOOL CheckOutputCanActive(COutput *pOutput);

	void AutoOperationFeed();
	void OnPlateLeaved();

	BOOL GetNextNeedSingal();

	virtual void MotorRun();

protected:
	void PowerUpOperation(void);
	void PowerUpRestart(void);
	void AutoOperation(void);
	BOOL InitIO();
	

public:
	void Destory();
public:
	//���ϸ�Ӧ
    int m_ICheckLineFull;

protected:	

	

	
};