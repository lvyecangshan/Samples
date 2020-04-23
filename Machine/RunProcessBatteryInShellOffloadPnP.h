/****************************************************************************************
�ļ�����RunProcessBatteryInShellOffloadPnP.h
������������Ǻ�����ץ��
��ʷ�޸ļ�¼��
*****************************************************************************************/
#pragma once
#include "RunProcess.h"

class CRunProcessBatteryInShellOffloadPnP : public CRunProcess
{
protected:
	enum POWER_UP_STEPS
	{
		PU_WAIT_ROBOT_HOME_FINISH,
		PU_JUDGE_IF_WITH_PALLET,//�жϴ�������
		

		PU_CHECK_WITHOUT_BATTER,
		PU_FINGER_CYL_OPEN,//��צ�򿪣�������ǹ�λ�����½�
		PU_MOTOR_HOME,
		PU_MOTOR_X_TO_WAIT_POS,
		//����
		PU_CHECK_PALLET_DATA,
		PU_FINGER_CYL_CLOSE_WITH_PALLET,
		PU_MOTOR_HOME_WITH_PALLET,
		PU_MOTOR_X_TO_WITH_PALLET,
		//��֤������λ��Ʒ�ɶ�
		PU_WAIT_JUDGE_FINISH,
		PU_CHECK_WITHOUT_BATTERY_WAIT_PALLTE,

		PU_FINISH,
		NUM_OF_POWER_UP_STEPS,
	};

	enum AUTO_STEPS
	{//����˳��������й�
		AUTO_SEND_SHELL_CORE_REQUIRE,
		AUTO_START_EVENT,
		AUTO_MOTOR_X_TO_PICK_POS,  
		AUTO_PNP_SEND_CYL_OUT,
		AUTO_PNP_SEND_CYL_PUSH,
		AUTO_FINGER_CYL_CLOSE,
		AUTO_CLUMMN_CYL_OPEN,
		AUTO_MOTOR_X_TO_WIAT_POS,  
		
		
		AUTO_PNP_SEND_CYL_RETURN,
		AUTO_WAIT_MES_SINGLE,
		AUTO_SEND_RETURN_SINGLE,
		AUTO_SEND_OFFLAD_SINGLE,
		AUTO_WAIT_OFFLOAD_SINGLE,
		AUTO_FINGER_CYL_OPEN,
		AUTO_SEND_CLAMP_SINGLE,
		AUTO_SEND_MOVE_AVOID,
		AUTO_WAIT_OFFLAOD_FINISH,
		AUTO_MOTOR_MOVE_PICK,
		AUTO_FORONT_MODULE_FINISH,
		AUTO_WORK_END,
	};	
public:
	enum MOTOR_X_LOC
	{
		X_MOTOR_PICK_POS,  //ȡ��λ
		X_MOTOR_WAIT_POS,  // ����λ
		X_MOTOR_AVOID_POS, //ƫ��λ 

	};

public:
	CRunProcessBatteryInShellOffloadPnP(int nID);
	virtual ~CRunProcessBatteryInShellOffloadPnP(void);

	BOOL InitializeConfig(CString strProcessModule);
	void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);
	BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove); 
	BOOL CheckOutputCanActive(COutput *pOutput);
	void ReadStepInfo();
	BOOL SaveStepInfo(int nWriteType);


protected:
	void PowerUpOperation(void);
	void PowerUpRestart(void);
	void AutoOperation(void);


public:
	BOOL PnPSendCylPush(BOOL bPush);//��λ����
	BOOL FingerCylClose(BOOL bClose);//��ץ����
	//�������������ʵ�ʼ������һ����,nCheckResultΪ�������0û���ϣ�1����
	BOOL CheckStartWithPalletDataSame();

	//����
	double GetMotorXAvoidPOS();


public:	
	int m_IPnPSendCylPush;//��λ�����Ƴ���Ӧ
	int m_IPnPSendCylBack;//��λ���׻��˸�Ӧ
	int m_OPnPSendCylPush;//��λ���׵�ŷ�
	int m_OPnPSendCylBack;//��λ���׻��˵�ŷ�
	
	int m_IFingerCylClose;//��צ���׼н���Ӧ
	int m_IFingerCylOpen;//��צ�����ɿ���Ӧ
	int m_OFingerCylClose;//��צ���׼н���ŷ�
	int m_OFingerCylOpen;//��צ�����ɿ���ŷ�

	int m_nMotorX;   //   X���
	int m_IHaveCore;//���ϸ�Ӧ

public:
	//���ϸ�λ
	BOOL m_bDataJudgeFinish;  //�ȴ���һ�����оģ�������ж����  ˫��λ���ݶԱ�ʹ��
	BOOL m_bResetJudge;   //�������� m_bDataJudgeFinish ��־

	int m_nStartWithPalletData;
	BOOL m_bHomeMoveSafePos;
	BOOL m_bOpenCylOpen;//��צ�ɿ��ź� ��������
	BOOL m_bPnPSendCylReturn; //��צץ���ϻ����ź�  �����
	BOOL m_bOffloadToRobot; //�������������ź�

	BOOL m_bOffload;
};