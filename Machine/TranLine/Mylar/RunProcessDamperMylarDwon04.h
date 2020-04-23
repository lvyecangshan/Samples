/****************************************************************************************
�ļ�����RunProcessDamperMylarDown04.h
������������Ĥ�赲���ϲ�04��
��ʷ�޸ļ�¼��
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

	//���һ��ģ���Ƿ�����
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