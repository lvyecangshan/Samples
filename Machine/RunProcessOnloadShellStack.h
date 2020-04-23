/****************************************************************************************
�ļ�����RunProcessOnloadShellStackEx.h
��������������Ƕ�ջģ��
��ʷ�޸ļ�¼��
*****************************************************************************************/
#pragma once
#include "RunProcess.h"

class CRunProcessOnloadShellStack : public CRunProcess
{
protected:
	enum POWER_UP_STEPS
	{
		PU_RELASE_CAR_LOCK_CYL_OPEN,
		PU_CHECK_PALLET_DATA,
		PU_SAFE_CYL_CLOSE,
		PU_Z_MOTOR_MOVE_HOME,
		PU_CHECK_HAVE_PALLET,
		PU_Z_MOTOR_MOVE_WAIT_POS,
		PU_FINISH,
	};

	enum AUTO_STEPS
	{
		AUTO_SAFE_CYL_OPEN,
		AUTO_WAIT_PEOPLE_ONLOAD,
		AUTO_CHECK_ONLOAD,
		AUTO_LOCK_CYL_LOCK,
		AUTO_MOVE_Z_MOTOR_TO_RELASE_CAR_LOC,
		AUTO_CHECK_LIMIT_CORE,
		AUTO_LOCK_CYL_LOCK_ONE,
		AUTO_WAIT_PEOPLE_CHECK_SAFE,
		AUTO_WAIT_START_EVENT,
		AUTO_MOVE_Z_MOTOR_POS,
		AUTO_MOTOR_Z_ABSNORMAL_DOWN_TO_DISTANCE,
		AUTO_MOTOR_Z_DOWN_TO_SAFE_DISTANCE,
		AUTO_MOVE_Z_MOVE_TO_ONLOAD_POS,
		AUTO_CAN_MOVE_SHORT_POS,
		AUTO_WAIT_PICK_PALLET_FINISH,
		AUTO_MOVE_Z_MOTOR_TO_SAFE_POS,
		AUTO_JUDGE_STACK_EMPTY,
		AUT0_CHECK_MOVE_SAFE,
		AUTO_MOVE_Z_MOTOR_WAIT_POS,
		AUTO_WORK_END,
	};

	enum MOTOR_TRYE
	{
		ONLOAD1_MOTOR,
		ONLOAD2_MOTOR,
		OFFLOAD3_MOTOR,
		NUM_MOTORS
	};

	enum MOTOR_WORK_LOC
	{
		STACK_MOTOR_WAIT_LOC = 0,//�ȴ�λ
		STACK_MOTOR_RELASE_CAR_LOC,//С������λ
		STACK_MOTOR_SEND_PALLET_LOC,//������λ
	};

public:
	CRunProcessOnloadShellStack(int nID);
	virtual ~CRunProcessOnloadShellStack(void);

protected:
	void PowerUpOperation(void);
	void PowerUpRestart(void);
	void AutoOperation(void);

public:
	BOOL InitializeConfig(CString strProcessModule);
	void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);
	BOOL ShowToolsDlg();
	BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove); 
	BOOL CheckOutputCanActive(COutput *pOutput);
	BOOL SaveParameter();
	BOOL ReadRunParameter();
	BOOL MonitorOut();
	
public:
	BOOL CheckCarInPos(void);//���С����λ
	BOOL CheckHaveNoCar(void);// ���û��С��
	BOOL CheckHavePallet(BOOL bHas = TRUE);//�������
	BOOL CheckCarAndPallet(BOOL bHas = TRUE);//������̺ͳ�	
	BOOL MotorHandMove( CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove );
	BOOL LockCar(BOOL bLock);//�������׶���
	BOOL ModuleIsSafe();
	BOOL MotorZMoveToOnload(double dSpeedRatio);

public:	
	int  m_IButtonReady;//����׼���ð�ť
	int  m_OReadyLight;//��ť��

	int  m_ICarInPosFront;//С����λǰ��Ӧ
	int  m_ICarInPosBehind;//С����λ���Ӧ
	
	//��������
	int  m_ILockCylPushOut;
	int  m_ILockCylBack;
	int  m_OLockCyl;

	int m_IPalletMoveInPos;		// �����ƶ���λ
	int m_IHavePallet;// ��ջ�������̼��
	int m_ITrayTopPresent;//��λ��Ӧ

	int  m_nMotorZ; //�����������	
	
public:	
	BOOL m_bMotorSafePos;//�������ȫλ
	BOOL m_bHavePallet;//�����ź�
	BOOL m_bSendPalletFinish;//��������ź�
	BOOL m_bHomeMoveSafePos;//��λ����ȫλ
	double m_dMotorPos;//��ȫλ��
	double m_dlZSafeLimit;//�½�����
};