/****************************************************************************************
�ļ�����RunProcessCoreOnLoad2.h
������������ǻ�����
��ʷ�޸ļ�¼��
*****************************************************************************************/
#pragma once
#include "..\..\RunProcess\RunProcessOnLoadOrOffPlate.h"

class CRunProcessCoreOnLoad2 : public CRunProcessCoreOnLoad
{
protected:



public:
	CRunProcessCoreOnLoad2(int nID);
	virtual ~CRunProcessCoreOnLoad2(void);

	BOOL InitializeConfig(CString strProcessModule);
	void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);
	BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove);
	BOOL CheckOutputCanActive(COutput *pOutput);
	BOOL GetNextNeedSingal();
	void OnPlateLeaved();

protected:
	void PowerUpOperation(void);
	void PowerUpRestart(void);
	void AutoOperation(void);
	BOOL InitIO();

public:
	
public:
	void Destory();
public:
	//���ϸ�Ӧ
	int m_ICheckLineFull;

protected:	




};