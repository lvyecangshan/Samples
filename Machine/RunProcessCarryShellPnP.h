/****************************************************************************************
�ļ�����RunProcessCarryShellPnP.h
���������������ǰ��˻�е��
��ʷ�޸ļ�¼��
*****************************************************************************************/
#pragma once
#include "RunProcess.h"

class CRunProcessCarryShellPnP : public CRunProcess
{
protected:
	enum POWER_UP_STEPS
	{
		PU_CHECK_HAVE_SHELL,
		PU_CLOSE_VACUM,
		PU_MOVE_Z_TO_HOME,
		PU_MOVE_Z_TO_SAFE_POS,
		PU_MOVE_X_TO_HOME,
		PU_MOVE_X_TO_TO_ONLOAD_POS,
		PU_CHECK_PALLET_DATA,
		PU_PUSH_CYL_RETURN,
		PU_FINISH,
		NUM_OF_POWER_UP_STEPS,
	};

	enum AUTO_STEPS
	{		
		AUTO_MOVE_X_TO_ONLOAD_POS,
		AUTO_CYL_PUSH_CYL_RETURN,
		AUTO_WAIT_ONLOAD_SINGLE,
		AUTO_MOVE_Z_TO_WAIT_PICK_POS,
		AUTO_WAIT_ONLOAD_SINGLE_EX,
		AUTO_MOVE_Z_TO_PICK_POS,
		AUTO_OPEN_VACUM,
		AUTO_MOVE_Z_TO_SAFE_POS,
		AUTO_SAFE_POS_CHECK_VACUM, 
		AUTO_SEND_MOVE_Z_TO_SAFE_SINGLE_TO_FRONT_MODULE,
		AUTO_PUSH_CYL_PUSH,
		AUTO_JUDGE_WHICH_MODULE_NEED_SHELL,
		AUTO_Z_AXIS_MOVE_TO_PLACE_WAIT_LOC,
		AUTO_CHECK_REQUIRE_AGAIN,
		AUTO_MOVE_X_TO_WAIT_POS,
		AUTO_JUDGE_WHICH_MODULE_NEED_SHELL_EX,
		AUTO_MOVE_X_TO_OFFLAOD_POS,
		AUTO_OFFLOAD_CHECK_VACUM,
		AUTO_MOVE_Z_TO_PUT_POS,
		AUTO_CLOSE_VACUM,
		AUTO_WAIT_BATTERY_CYL_CLOSE,
		AUTO_MOVE_Z_TO_SAFE_POS_AFTER_PLACE,
		AUTO_SEND_PLACE_FINISH_SINGLE,
		AUTO_JUDGE_ALL_PUT_FINISH,
		AUTO_WORK_END,
	};

	enum MOTOR_LOCS
	{
		X_MOTOR_ONLOAD_LOC = 0,//X��ȡ��λ
		X_MOTOR_PNP_PUT_POS,	   //X�����λ
		

		Z_MOTOR_SAFE_POS = 0,//��ȫλ
		Z_MOTOR_WAIT_PICK_POS,//ȡ�ϵȴ�λ
		Z_MOTOR_PICK_POS,//ȡ��λ
		Z_MOTOR_WAIT_PLACE_POS,// ���ϵȴ�λ
		Z_MOTOR_PLACE_POS,//����λ
	};

public:
	CRunProcessCarryShellPnP(int nID);
	virtual ~CRunProcessCarryShellPnP(void);

	BOOL InitializeConfig(CString strProcessModule);
	void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);
	BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove);
	BOOL CheckOutputCanActive(COutput *pOutput);
protected:
	void PowerUpOperation(void);
	void PowerUpRestart(void);
	void AutoOperation(void);

public:
	BOOL BatteryInShellNoNeedShell();  
	BOOL CheckCanPutShell(int nIndex);
	BOOL VacumOpen(int nIndex, BOOL bOpen);
	BOOL BreakVacum( BOOL bBreak);
	BOOL CheckVacum(int nIndex,BOOL bOpen);   //yy-8.4 ��������
	BOOL PushCylPush(BOOL bPush);//�������׺���
	 
	//����
	double GetMotorZPOS(); //��ȡZ�ᰲȫλλ��


public:
	int m_OVacumCyl[2];//�������ŷ�
	int m_ICheckVacum[2];//��ռ��
	int m_OBreakVacum;//�����

	//��Ǳ������
	int m_IPushCylPush;//���������Ƴ����
	int m_IPushCylBack;//�������׻��˼��
	int m_OPushCylPush;//���������Ƴ���ŷ�
	int m_OPushCylBack;//�������׻��˵�ŷ�

	int m_nMotorX;//X����
	int m_nMotorZ;//Z����
public:
	BOOL m_bHomeMoveSafePos;
};