/****************************************************************************************
�ļ�����RunProcessOffloadShellStackEx.h
��������������Ƕ�ջģ��
��ʷ�޸ļ�¼��

���⣺
1. ���λ������
*****************************************************************************************/
#pragma once
#include "RunProcess.h"

class CRunProcessOffloadShellStack : public CRunProcess
{
protected:
	enum POWER_UP_STEPS
	{
		PU_LOCK_CAR_OPEN,				//�������״�
		PU_CHECK_HAVE_CAR,				//����Ƿ��޳�
		PU_WAIT_PEOPLE_CHECK_SAFE,
		PU_SAFE_CLY_CLOSE,				//��ȫ���׹ر�
		PU_Z_MOTOR_MOVE_HOME,			//Z�Ḵλ
		PU_CHECK_HAVE_PALLAT,
		PU_MOVE_Z_MTOR_TO_SAFE_POS,
		PU_MOTOR_DOWN_SAFE_DISTANCE,
		PU_CHECK_PALLET_FULL,
		
		PU_FINISH,
		NUM_OF_POWER_UP_STEPS,
	};

	enum AUTO_STEPS
	{
		AUTO_CHECK_CAR_AND_PALLET,
		AUTO_MOVE_Z_MOTOR_TO_SAFE_POS,
		AUTO_MOTOR_Z_DOWN_TO_SAFE_DISTANCE,
		AUTO_CHECK_PALLET_IS_FULL,
		AUTO_WAIT_START_EVENT,
		AUTO_JUDGE_CAR_MOVE,
		AUTO_MOVE_Z_MOTOR_TO_WORK_POS,
		AUTO_WAIT_PUT_PALLET_FINISH,
		AUTO_CHECK_CAR_REMOVE,
		AUTO_MOVE_Z_MOTOR_TO_SAFE_POS_AFTER_PICK_PALLET,
		AUTO_JUDGE_PALLET_FULL,
		AUTO_CHECK_CAR_AND_PALLET_AGAIN,
		AUTO_MOTOR_OFFLOAD_MOVE_TO_SAFE_POS,
		AUTO_SET_OFFLOAD_PALLET_SINGLE,
		AUTO_WAIT_LOCK_CYL_LOCK,
		AUTO_WAIT_PEOPLE_MOVE_CAR_IN,
		AUTO_CHECK_HAVE_CAR,
		AUTO_LOC_CYL_LOCATION_LOCK,
		AUTO_MOVE_Z_MOTOR_TO_RELASE_POS,
		AUTO_LOCK_CYL_OPEN_ON_REMOVE_CAR,
		AUTO_REMOVE_CAR_AFTER_OFFLOAD,
		AUTO_CHECK_HAVE_NO_PALLET,
		AUTO_SAFE_LOCK_CYL_CLOSE,
        AUTO_WORK_END,
	};

	enum MOTOR_WORK_LOC
	{
		STACK_MOTOR_SAFE_POS = 0,//��ȫλ
		STACK_MOTOR_RELASE_CAR_LOC,//����λ
		STACK_MOTOR_RECV_PALLET,	// ����λ(�Կ�����Ϊλ��)
		NUM_WORK_LOC
	};

public:
	CRunProcessOffloadShellStack(int nID);
	virtual ~CRunProcessOffloadShellStack(void);

protected:
	void PowerUpOperation(void);
	void PowerUpRestart(void);
	void AutoOperation(void);

public:
	BOOL InitializeConfig(CString strProcessModule);
	void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);
	BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove); 
	BOOL MotorHandMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove); 
	BOOL CheckOutputCanActive(COutput *pOutput);
	BOOL SaveParameter();
	BOOL ReadRunParameter();

public:
	BOOL MonitorOut();
	BOOL CheckCarInPos(BOOL bHave);//���С����λ
	BOOL CheckHaveNoPallet(void);//�ж�û������
	BOOL CheckCarAndPallet(BOOL bHas = TRUE);//����Ƿ������̻�
	BOOL CheckHaveCar(BOOL bHas = TRUE);//����Ƿ��г�
	BOOL CheckHavePaller(BOOL bHas = TRUE);//�������
	BOOL LockCar(BOOL bLock);//�������׶�������
	BOOL MotorMoveCheckFullPallet(void);//�ƶ���������������λ����
	BOOL ModuleIsSafe();//���ģ���Ƿ�ȫ

public:
	int  m_IButtonReady;//����׼���ð�ť
	int  m_OReadyLight;//��ť��
	int  m_ICarInPosFront;//С����λǰ��Ӧ
	int  m_ICarInPosBehind;//С����λ���Ӧ

	//��������
	int  m_ILockCylPushOut;
	int  m_ILockCylBack;
	int  m_OLockCyl;

	int m_IHavePallet;// ��ջ�������̼��
	int m_ITrayTopPresent;//��λ��Ӧ
	int m_IEmptyCarCheck;		// �ճ���Ӧ(��������ʱС���ƽ�ʱ��⳵���Ƿ�������)

	int  m_nMotorZ; //Z����

public:	
	BOOL m_bOffloadEmptyPallet;
	BOOL m_bMotorSafePos;//����ƶ�����ȫλ��־
	BOOL m_bMoveMotorFinish;//����ƶ����
	BOOL m_bHomeMoveSafePos;//�����λ���
	double m_dMotorDownPos;//����½�����
	double m_dMotorUpPos;	// �����������
	double m_dPalletHigh;	// �������̸߶�
	double m_dlZSafeLimit;//��ȫ��λ����
};