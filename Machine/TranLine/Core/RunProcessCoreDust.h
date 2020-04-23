/****************************************************************************************
�ļ�����CRunProcessCoreDust.h
������������о����
��ʷ�޸ļ�¼��
*****************************************************************************************/
#pragma once
#include "../../RunProcess/RunProcessOnLoadOrOffPlate.h"

class CRunProcessCoreDust : public CRunProcessOnLoadOrOffPlate
{

protected:
	enum AUTO_STEPSEX
	{
		AUTO_WORK_STAR=0,
		AUTO_WAIT_DUST_END,
		AUTO_WORK_END,
	};

public:
	CRunProcessCoreDust(int nID);
	virtual ~CRunProcessCoreDust(void);

	BOOL InitializeConfig(CString strProcessModule);
	void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);
	BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove);
	BOOL CheckOutputCanActive(COutput *pOutput);
	
protected:
	void PowerUpOperation(void);
	void PowerUpRestart(void);
	void AutoOperation(void);
	BOOL InitIO();
	BOOL GetNextNeedSingal();
	void AutoOperationFeed();
	
	
public:
	void Destory();
protected:
	/*
	    int m_IDamperPush;			//�����赲�����Ƴ���Ӧ
		int m_IDamperReturn;		//�����赲���׻��˸�Ӧ
		int m_IJackingPush;        //�������������Ƴ���Ӧ
		int m_IJackingReturn;      //�����������׻��˸�Ӧ
		int m_IHasPlate01;		   //�����������ϸ�Ӧ1
		int m_IHasPlate02;		   //�����������ϸ�Ӧ2
	    int m_ODamperReturn;       //�����赲���׻��˵�ŷ�
		int m_OJackingPush;       //�������������Ƴ���ŷ�
		int m_OJackingReturn;     //�����������׻��˵�ŷ�
		*/

		
		int m_dwStarDust;
	
};