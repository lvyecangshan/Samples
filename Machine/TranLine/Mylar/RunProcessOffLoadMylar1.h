/****************************************************************************************
�ļ�����RunRunProcessOffLoadMylar1.h
��������������ģ��
��ʷ�޸ļ�¼��
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
	//���ϸ�Ӧ
    int m_ICheckLineFull;



	
};