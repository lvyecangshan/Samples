/****************************************************************************************
�ļ�����RunRunProcessOffLoad.h
��������������������
��ʷ�޸ļ�¼��
*****************************************************************************************/
#pragma once
#include "../../RunProcess.h"

class CRunProcessOffLoad : public CRunProcess
{
protected:
	enum POWER_UP_STEPS
	{
		PU_SET_STOP_STATUS,   //�����赲״̬
		PU_FINISH,
	};

	enum AUTO_STEPS
	{
		AUTO_SEND_STAR_SIGNAL,			//��ʼ����
		AUTO_WAIT_PLATE_ARRIVE,         //�ȴ����̵���
		AUTO_WAIT_MATERIAL_FLOW,		//�ȴ���������
		AUTO_SEND_MATERIAL_READY_SIGNAL,//��������׼�����ź�
		AUTO_WAIT_FEEDING_END_SIGNAL,   //�ȴ���������ź�
		AUTO_WAIT_FEEDING_END,          //�ȴ��������
		AUTO_WORK_END,
	};

public:
	CRunProcessOffLoad(int nID);
	virtual ~CRunProcessOffLoad(void);

	BOOL InitializeConfig(CString strProcessModule);
	void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);
	BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove);
	BOOL CheckOutputCanActive(COutput *pOutput);

protected:
	void PowerUpOperation(void);
	void PowerUpRestart(void);
	void AutoOperation(void);
	BOOL InitIO();
	virtual BOOL SendRequiredSignal()=0;

	//�赲�Ƴ�
	BOOL DamperPush();
	//�赲����
	BOOL DamperReturn();
	//���϶��������Ƴ�
	BOOL JackingPush();
	//���϶������׻���
	BOOL JackingReturn();
	//�ж��Ƿ�������
	BOOL CheckHasPlate();
	//�ж��Ƿ��е�о
	BOOL CheckHasCore();
	//�ж���һģ���Ƿ�Ҫ��
	virtual BOOL CheckNextRequirePlate();
	//��������׼�����ź�
	BOOL SendPlateReadySignal(BOOL bReady);
	//��ȡ��������ź�
	BOOL GetFeedingEndSignal();
	//��̨�Ƿ�Ҫ��
	virtual BOOL CheckPlatFormNeedPlate();
	
public:
	
public:
	void Destory();
public:
	//���ϸ�Ӧ
    int m_ICheckLineFull;

protected:	

	int m_IDamperPush;				//X2011	�����赲�����Ƴ���Ӧ
	int m_IDamperReturn;			//X2012	�����赲���׻��˸�Ӧ
	int m_IJackingPush;				//X2013	���϶��������Ƴ���Ӧ
	int m_IJackingReturn;			//X2014	���϶������׻��˸�Ӧ
	int m_IHasPlate;				//X2015	���϶������ϸ�Ӧ
	int m_ICheckCore;				//X2016	���϶������̷������
	int m_IHasCore;					//X2017	���ϵ�о���ϸ�Ӧ
	int m_ODamperReturn;	        //Y2003	�����赲���׻��˵�ŷ�
	int m_OJackingPush;             //Y2004	���϶��������Ƴ���ŷ�
	int m_OJackingReturn;			//Y2005	���϶������׻��˵�ŷ�

	int m_OReadPlateSignal;			//����׼�����ź�
	int m_IPlatformNeedPlate;        //��̨Ҫ���ź� 
	int m_IFeedingPlateSignal;      //��������ź�
	
};