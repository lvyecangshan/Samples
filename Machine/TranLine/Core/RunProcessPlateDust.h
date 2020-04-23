/****************************************************************************************
�ļ�����CRunProcessPlateDust.h
������������о����
��ʷ�޸ļ�¼��
*****************************************************************************************/
#pragma once
#include "../../RunProcess/RunProcessOnLoadOrOffPlate.h"

class CRunProcessPlateDust : public CRunProcessOnLoadOrOffPlate
{
protected:
	

	enum AUTO_STEPSEX
	{
		AUTO_WORK_STAR=0,
		AUTO_WAIT_DUST_END,
		AUTO_WORK_END,
	};

public:
	CRunProcessPlateDust(int nID);
	virtual ~CRunProcessPlateDust(void);

	BOOL InitializeConfig(CString strProcessModule);
	void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);
	BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove);
	BOOL CheckOutputCanActive(COutput *pOutput);
	void AutoOperationFeed();
	BOOL GetNextNeedSingal();

protected:
	void PowerUpOperation(void);
	void PowerUpRestart(void);
	void AutoOperation(void);
	BOOL InitIO();	
public:
	void Destory();
protected:
	    int m_IDamperPush;			//X2607 �����赲�����Ƴ���Ӧ
		int m_IDamperReturn;		//X2608	�����赲���׻��˸�Ӧ
		int m_IJackingPush;        //X2609	�������������Ƴ���Ӧ
		int m_IJackingReturn;      //X2610	�����������׻��˸�Ӧ
		int m_IHasPlate01;		   //X2611	�����������ϸ�Ӧ1
		int m_IHasPlate02;		   //X2612	�����������ϸ�Ӧ2
	    int m_ODamperReturn;       //Y2602	�����赲���׻��˵�ŷ�
		int m_OJackingPush;       //Y2603	�������������Ƴ���ŷ�
		int m_OJackingReturn;     //Y2604	�����������׻��˵�ŷ�
		int m_ODustOpen;         //Y2410    ������ŷ�

		DWORD m_dwStarCoreDustTime;  //��ʼ����ʱ��
		DWORD m_dwCoreDustTime;      //��������ʱ��

		int m_dwStarDust;
	
};