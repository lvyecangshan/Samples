/****************************************************************************************
�ļ�����RunProcessDamper.h
�����������赲������
��ʷ�޸ļ�¼��
*****************************************************************************************/
#pragma once
#include "..\RunProcess.h"
#include "../parts/Damper.h"
#include "..\parts\LineMotor.h"

class CRunProcessDamper : public CRunProcess
{
protected:
	enum POWER_UP_STEPS
	{
		PU_SET_STOP_STATUS,//�����赲״̬
		PU_FINISH,
	};

	enum AUTO_STEPS
	{
		AUTO_SET_STOP_STATUS,   //�����赲
		AUTO_WAIT_PLATE_ARRIVE, //�ȴ����̵���
		AUTO_WAIT_FEED_END_SINGAL,//�ȴ�ι������ź�
		ATUO_WAIT_NEXT_NEED_PLATE_SINGAL,//�ȴ���һģ��Ҫ���ź�
		ATUO_WAIT_FOLLOW_PLATE,      //����
		AUTO_WORK_END,
	};

public:

	virtual void OnPlateLeaved();
	
	CRunProcessDamper(int nID);
	virtual ~CRunProcessDamper(void);

	BOOL InitializeConfig(CString strProcessModule);
	void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);
	BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove);
	BOOL CheckOutputCanActive(COutput *pOutput);	

	virtual void MotorRun();

protected:
	void PowerUpOperation(void);
	void PowerUpRestart(void);
	void AutoOperation(void);
	

	virtual void AutoOperationFeed(void);
	               //�Ƿ�Ҫ��
	
	
	//IO��ʼ��
	virtual BOOL InitIO();
	
	
	
public:
	void Destory();

protected:
	CDamper			   m_Damper;
	CLineMotor m_Motor;

protected:
	
	BOOL               m_bNeedPlate; //��Ҫ����
	BOOL               m_bFeedPlate; //����׼����
	int                m_nNextAutoStepEx;  
	
};