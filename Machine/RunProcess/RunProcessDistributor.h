/****************************************************************************************
�ļ�����RunRunProcessDistributor.h
��������������������
��ʷ�޸ļ�¼��
*****************************************************************************************/
#pragma once
#include "..\RunProcess.h"
#include "..\parts\Distributor.h"
#include "..\parts\LineMotor.h"



class CRunProcessDistributor : public CRunProcess
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
		AUTO_SEND_WAIT_PLATE,           //�ȴ����̵���
		AUTO_SEND_DISTRIBUTOR,           //����
		AUTO_SEND_READY_SINGAL,         //����׼�����ź�
		AUTO_WAIT_FEEDING_END,          //�ȴ��������
		AUTO_WORK_END,
	};

public:
	CRunProcessDistributor(int nID);
	virtual ~CRunProcessDistributor(void);

	BOOL InitializeConfig(CString strProcessModule);
	void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);
	BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove);
	BOOL CheckOutputCanActive(COutput *pOutput);

	//�����Ƿ�����CheckLineFull
	BOOL CheckLineFull();

protected:
	void PowerUpOperation(void);
	void PowerUpRestart(void);
	void AutoOperation(void);
	BOOL InitIO();	
	//�ж��Ƿ�����
	virtual BOOL CheckHasPlate()=0;
	virtual BOOL CheckNoPlate()=0;
	//����
	BOOL DistributorPlate();
	

public:
	void Destory();
public:
	/*
	int  m_IDistributionCylinderPush[2];		//X2006	 X2008  ��������1��2�Ƴ���Ӧ
	int  m_IDistributionCylinderReturn[2];       //X2007	 X2009  ��������1��2���˸�Ӧ
	int  m_IHasPlate[2];                         //       X2010	��������2���ϸ�Ӧ
	int  m_ODistributionCylinder[2];             //Y2001  Y2002	��������1��2���˵�ŷ�
	*/

	CDistributor  m_Distriutor;                 //������

	CLineMotor m_Motor;


	int  m_ICheckLineFull;                      //X2100	�������ϸ�Ӧ

	BOOL m_bFeedingMaterial;                    //����

	
	
};