/****************************************************************************************
�ļ�����RunProcessOnloadShellPNP.cpp
�����������Ͽǻ�е��
��ʷ�޸ļ�¼��
*****************************************************************************************/
#pragma once
#include "RunProcess.h"

class CRunProcessOnloadShellPNP : public CRunProcess
{
protected:
	enum POWER_UP_STEPS
	{
		PU_CHECK_FINGER_STATUS, 
		PU_MOVE_MOTOR_Z_HOME, 
		PU_ROTATE_CYL_HOME, 
		PU_CLAMP_CYL_OPEN, 		
		PU_MOVE_MOTOR_Z_TO_SAFE_POS, 
		PU_MOTOR_Y_HOME, 

		PU_JUDGE_IF_WITH_PALLET,//�жϴ�������

		PU_MOTOR_Y_MOVE_PICK_POS, 
		PU_FINISH, 
	};

	enum AUTO_STEPS
	{
		AUTO_SEND_FRONT_MODULE_MOVE_MOTOR, 
		AUTO_MOVE_Y_MOTOR_TO_ONLOAD_POS, 
		AUTO_CHECK_ROATE_BACK,
		AUTO_CHECK_PUSH_PUSH,
	    AUTO_WAIT_FRONT_MODULE_MOVE_MOTOR_FINISH, 
		AUTO_MOTOR_Z_MOVE_TO_WAIT_POS,
		AUTO_WAIT_FRONT_MODULE_MOVE_MOTOR_FINISH_AGAIN,

        AUTO_MOVE_MOTOR_Z_MOTOR_TO_PICK_POS,
		AUTO_MOVE_MOTOR_Z_TO_SAFE_POS_ONLOAD, 
		AUTO_CHECK_ALL_VACU_ONLOAD,
		AUTO_MOVE_Y_MOTOR_TO_OFFLOAD_POS, 
		AUTO_CHECK_ALL_VACU_OFFLOAD, 
		AUTO_PUSH_CYL_BACK_TO_OFFLOAD,
		AUTO_ROATE_CYL_RAOTE, 
		AUTO_WAIT_START_EVENT, 
		AUTO_MOVE_Z_TO_WAIT_PUT_POS,
		AUTO_WAIT_START_EVENTEX,
		AUTO_MOVE_Z_MTOR_TO_PUT_POS, 
		AUTO_CLAMP_CYL_OPEN, 
		AUTO_CHECK_DIRECTION,
		AUTO_CHANGE_DIRECTION,
		AUTO_MOVE_Z_MOTOR_SAFE_POS_OFFLOAD,  
		AUTO_SEND_OFFLOAD_FINISH,
		AUTO_ROATE_CYL_RETURN,
		 
		
	    AUTO_WORK_END, 
	};

  enum MOTOR_LOCS
  {
	  Y_MOTOR_PLACE_LOC, // ����λ
	  Y_MOTOR_PICK_LOC1, // Y��ȡ��λ1
	  Y_MOTOR_PICK_LOC2,
 	  Y_MOTOR_PICK_LOC3,
	  Y_MOTOR_PICK_LOC4,
	  Y_MOTOR_PICK_LOC5,
	  Y_MOTOR_PICK_LOC6,
	  Y_MOTOR_PICK_LOC7,
	  Y_MOTOR_PICK_LOC8, 
	  Y_MOTOR_PICK_LOC9,
	  Y_MOTOR_PICK_LOC10,
	  Y_MOTOR_PICK_LOC11,
	  Y_MOTOR_PICK_LOC12,
	 
	  Z_MOTOR_SAFE_POS = 0, //��ȫλ
	  Z_MOTOR_PICK_WAIT_POS,
	  Z_MOTOR_PICK_POS, //ȡ��λ
	  Z_MOTOR_WAIT_PLACE_POS, // ���ϵȴ�λ
	  Z_MOTOR_PLACE_POS, //����λ
	  Z_MOTOR_CHANGE_DIRECTION,//���ǳ�����תλ

	  R_MOTOR_PICK_POS = 0, //����λ
	  R_MOTOR_ROTATE_NORMAL, //������ת
	  R_MOTOR_ROTATE_REVERSE,//���Ƿ�����ת
};

public:
	CRunProcessOnloadShellPNP(int nID);
	virtual ~CRunProcessOnloadShellPNP(void);

public:
	BOOL InitializeConfig(CString strProcessModule);
	void GetHardwareConfig(CArray<int, int> *pInputs, CArray<int, int> *pOutputs, CArray<int, int> *pMotors);
	BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove);
	BOOL CheckOutputCanActive(COutput *pOutput);

protected:
	void PowerUpOperation(void);
	void PowerUpRestart(void);
	void AutoOperation(void);

public:
	BOOL CloseVacu();//�ر����
	BOOL CheckVacuStatus(BOOL bOpen);
	BOOL TurnOut(int nIndex, BOOL bOut =TRUE);//��ת���׶���
	BOOL PushCylPush(BOOL bPush);//�������׺���
	BOOL ModuleIsSafe();
	BOOL CheckCanMoveMotor(void);

	void  ReadStepInfo();
	BOOL  SaveStepInfo(int nWriteType);

public:
	int m_ICheckVacu[2];    //��ո�Ӧ
	int m_OOpenVacu[2];//��յ�ŷ�
	
	//��ת����
	int m_ITurnOut[2];    
	int m_ITurnReturn[2];
	int m_OTurnOut[2];
	int m_OTurnReturn[2];

	//��Ǳ��
	int m_IPushCylPush;
	int m_IPushCylBack;
	int m_OPushCylPush;
	int m_OPushCylReturn;

	int m_nMotorY;//Y����
	int m_nMotorZ;//Z����
	int m_nMotorR[2]; //R�����������ת����
	
	int m_IShellDirection[2];//���ǳ��ڷ���
	BOOL m_bNeedChangeDirect[2];

public:
	int m_nMotorMoveIndex;//����ƶ���λ��
	BOOL m_bPickCoverFinish;//ȡ������ź�
	BOOL m_bHomeMoveSafePos;//����ȫλ�ź�
	BOOL m_bEnableDocking;
};