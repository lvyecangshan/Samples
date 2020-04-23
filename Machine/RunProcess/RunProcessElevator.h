/****************************************************************************************
�ļ�����RunProcessElevator.h
�����������������ͻ�
��ʷ�޸ļ�¼��
*****************************************************************************************/
#pragma once
#include "../RunProcess.h"
#include "../parts/Elevator.h"
#include "..\parts\LineMotor.h"

class CRunProcessElevator : public CRunProcess
{
protected:
	enum POWER_UP_STEPS
	{
		PU_WAIT_STAR_SIGNAL,				 //�ȴ���ʼ�����ź�
		PU_STOP_BELT,						 //���Ƥ��ת��
		PU_CHECK_SAFE,						 //����Ƿ�ȫ
		PU_OPERATION_BEFORE_MOVE,            //����ƶ�ǰ��ز���
		PU_TO_RECEIVE_MATERIAL_POSTION,      //�ƶ����������λ
		PU_FINISH,							 //��ʼ������
	};

	enum AUTO_STEPS
	{
		AUTO_CHECK_WORK_SAFE,						//��ⰲȫ
		AUTO_TO_RECEIVE_MATERIAL_POSTION,			//�ƶ����������λ
		//AUTO_PREPARATIONS_BEFORE_RECEIVEMATERIAL,	//����ǰ׼������
		AUTO_SEND_REQUIRE_SIGNAL,					//����Ҫ���ź�
		AUTO_START_RECEIVE_PLATE,					//��ʼ��������
		AUTO_SEND_RECEIVE_END,                      //�������
		AUTO_TO_FEEDING_POSITION,                   //�ƶ�������λ
		AUTO_WAIT_FEEDING_SIGNAL,					//�ȴ������ź�
		AUTO_FEEDING_MATERIAL,					    //����
		AUTO_WAIT_FEEDING_END,                      //�ȴ��������
		
		AUTO_RUNNING,
		AUTO_WORK_END,
	};

public:
	CRunProcessElevator(int nID);
	virtual ~CRunProcessElevator(void);

	BOOL InitializeConfig(CString strProcessModule);
	void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);
	BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove);
	BOOL CheckOutputCanActive(COutput *pOutput);

protected:
	void PowerUpOperation(void);
	void PowerUpRestart(void);
	void AutoOperation(void);
	//IO��ʼ��
	virtual BOOL InitIO();
public:
	//�ƶ�������λ
	virtual BOOL MoveToReceiveMaterialPostion();
	//�ƶ�������λ
	virtual BOOL MoveToFeedingMaterialPostion();
	
	//��ȡ��ǰ����������������
	virtual int GetCurPlateCount();
	//�ж��Ƿ��������
	virtual BOOL CheckCanFeedingMaterial();
	//�ж������Ƿ����
	virtual BOOL CheckFeedingMaterialEnd();	

	//�������ϵ��
	virtual BOOL StartReceiveMotor();
	//ֹͣ���ϵ��
	virtual BOOL StopReceiveMotor();
	//�������ϵ��
	virtual BOOL StartFeedingMotor();
	//ֹͣ���ϵ��
	virtual BOOL StopFeedingMotor();
	//ֹ�������Ƴ�
	BOOL CylinderPush();
	//ֹ�����׻���
	BOOL CylinderReturn();

	virtual BOOL SendReceiveEnd();
	
	virtual void MotorRun();

public:
	void Destory();

protected:

	CLineMotor m_Motor;

	CElevator  m_Elevator;

	int m_IUpLineSafe;			 //X2005	�������̰�ȫ��Ӧ
	int m_IDownLineSafe;		 //X2024	�����������̰�ȫ��Ӧ
	int m_ILiftingSafe;			 //X2004	�������̰�ȫ��Ӧ
	int m_ICheckCylinderPush;    //X2000	ֹ�������Ƴ���Ӧ
	int m_ICheckCylinderReturn;  //X2001	ֹ�����׻��˸�Ӧ
	int m_OCheckCylinder;        //Y2000	ֹ�������Ƴ���ŷ�
	int m_IHasPlate1;            //X2002	��������1��λ��Ӧ
	int m_IHasPlate2;            //X2003	��������2��λ��Ӧ
	int m_OMotorForeward;        //Y2008	ǰ�����������ٵ����ת������60W��
	int m_OMotorReverse;		 //Y2009	ǰ�����������ٵ����ת����
	int m_nMotorZ;               //���ǰ����Z�ᣨ400W��M14

	int m_bPlateStatus[2];       //���������Ƿ�������

	int m_bSendNeedCount;       //�����������̴���

	

	
};