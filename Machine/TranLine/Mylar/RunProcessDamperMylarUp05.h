/****************************************************************************************
�ļ�����RunProcessDamperMylarUp05.h
������������Ĥ�赲���ϲ�05��
��ʷ�޸ļ�¼��
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

	//���һ��ģ���Ƿ�����
	BOOL CheckNextLineIsFull();

	virtual void MotorRun();

protected:
	void PowerUpOperation(void);
	void PowerUpRestart(void);
	void AutoOperation(void);

public:
	void Destory();
private:	
	int m_ICheckNextLineFull;    //6������������Ӧ X2500	�������ϸ�Ӧ

	
};