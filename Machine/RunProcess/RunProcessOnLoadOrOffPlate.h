/****************************************************************************************
�ļ�����RunProcessOnLoadOrOffPlate.h
���������������Ϲ�λ
��ʷ�޸ļ�¼��
*****************************************************************************************/
#pragma once
#include "../RunProcess.h"
#include "../parts/OnOrOffLoadPlate.h"
#include "..\parts\LineMotor.h"

class CRunProcessOnLoadOrOffPlate : public CRunProcess
{
protected:
	enum POWER_UP_STEPS
	{
		PU_WAIT_RESET,				 //�ȴ���ʼ�����ź�		
		PU_FINISH,					 //��ʼ������
	};

	enum AUTO_STEPS
	{
		AUTO_WORK_STAR,				//��ʼ����	
		AUTO_WAIT_PLATE_ARRIVED,    //�ȴ����̵���
		AUTO_JACKING_PLATE,         //��������
		AUTO_WAIT_FEED_END_SINGAL,  //�ȴ�ȡ������ź�
		ATUO_WAIT_NEXT_NEED_PLATE_SINGAL,//�ȴ���һģ��Ҫ�����ź�
		ATUO_WAIT_FOLLOW_PLATE,      //�ȴ���������
		AUTO_CHECK_HAS_CORE,        //�ж��Ƿ��е�о
		AUTO_CHECK_NO_CORE,        //�ж��޵�о
		AUTO_WORK_END,              //��������
	};


public:
	CRunProcessOnLoadOrOffPlate(int nID);
	virtual ~CRunProcessOnLoadOrOffPlate(void);
	//��������ģʽ
	void SetOnLoadMode();
	//�Ƿ�������ģʽ
	BOOL IsOnLoadMode();
	//��������ģʽ
	void SetOffLoadMode();
	//�Ƿ�������ģʽ
	BOOL IsOffLoadMode();
	BOOL HasCore();
public:
	
	virtual void MotorRun();

protected:
	BOOL InitializeConfig(CString strProcessModule);
	void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);
	BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove);
	BOOL CheckOutputCanActive(COutput *pOutput);
	virtual void  OnPlateLeaved();

protected:
	void PowerUpOperation(void);
	void PowerUpOperationOnLoad(void);
	void PowerUpOperationOffLoad(void);
	void PowerUpRestart(void);
	void AutoOperation(void);
	void AutoOperationOnLoad(void);
	void AutoOperationOffLoad(void);
	virtual void AutoOperationFeed();//���ϣ�����Ҫ��������ģ�飩
	//IO��ʼ��
	virtual BOOL InitIO();
	BOOL m_bOnLoadMode;  //�Ƿ�������ģʽ
protected:

	CLineMotor m_Motor;

	COnOrOffLoadPlate  m_Plate;	
	BOOL               m_bNeedPlate; //��Ҫ����
	BOOL               m_bFeedPlate; //����׼����
	BOOL               m_bFeedEnd;   //ι����ɣ���������ɣ�

};