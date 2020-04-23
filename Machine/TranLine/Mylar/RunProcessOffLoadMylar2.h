/****************************************************************************************
�ļ�����RunRunProcessOffLoadMylar1.h
��������������ģ��
��ʷ�޸ļ�¼��
*****************************************************************************************/
#pragma once
#include "../../RunProcess/RunProcessOnLoadOrOffPlate.h"

class CRunProcessOffLoadMylar2 : public CRunProcessOnLoadOrOffPlate
{
protected:
	
public:
	CRunProcessOffLoadMylar2(int nID);
	virtual ~CRunProcessOffLoadMylar2(void);

	BOOL InitializeConfig(CString strProcessModule);
	void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);
	BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove);
	BOOL CheckOutputCanActive(COutput *pOutput);
	BOOL CheckPlatFormNeedPlate();
	BOOL CheckNextRequirePlate();

	BOOL GetNextNeedSingal();

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
protected:	
	BOOL m_bFeedingMaterial;                    //����
	

	
	
	
	
	
	


	
};