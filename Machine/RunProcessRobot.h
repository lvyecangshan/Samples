/****************************************************************************************
�ļ�����RunProcessRobot.h
�������������������Ϲ�λ
��ʷ�޸ļ�¼��
*****************************************************************************************/
#pragma once
#include "RunProcess.h"
#include "RobotClient.h"

class CRunProcessRobot : public CRunProcess
{
protected:
	enum POWER_UP_STEPS
	{
		PU_ROBOT_START,//����������
		PU_ROBOT_ALARM_RESET,//�����˱�����λ
		PU_ROBOT_ALARM_CHECK,//�������˱����ź�
		PU_ROBOT_CODE_RESET_AND_RUN,//�����˳���λ������
		PU_ROBOT_CONNET,//���ӻ�����
		
		PU_JUDGE_IF_WITH_PALLET,//�жϴ�������
		
		//****���ָ�����***************
		PU_CHECK_WITHOUT_PRODUCT,
		PU_FINGER_CYL_OPEN,//��צ��
		PU_ROBOT_MOVE_TO_SAFE_POS,//�����˵���ȫλ
		PU_ROBOT_MOVE_TO_PICK_WAIT_POS,//�����˵�ȡ�ϵȴ�λ
		PU_BUFFER_CYL_BACK,
		//*****************************

		//****�ָ�����****************
		PU_FINGER_CYL_CLOSE_WITH_PALLET,
		PU_WAIT_FRONT_MODULE_SAFE,
		PU_ROBOT_MOVE_TO_SAFE_POS_WITH_PALLET,//�����˵���ȫλ
		PU_CHECK_BUFFER_PALLET_DATA,
		PU_CHECK_PALLET_DATA,//�жϻָ����������צ���ϸ�Ӧ����Ƿ��Ǻ�
		PU_JUDGE_PRESS_DATE_EXIST,
		//****************************

		PU_JOINT_SIGNAL_RESET,

		PU_WAIT_BACK_MACHINE_READY,
		PU_CONNECT_SERVER,

		PU_FINISH,
		NUM_OF_POWER_UP_STEPS,
	};

	enum AUTO_STEPS
	{  //����˳��������й�
		AUTO_IF_CLEAR_MATERIAL,
		AUTO_ROBOT_TO_PICK_WAIT_POS_INIT,
		AUTO_FINER_CLY_OPEN_INIT,

		//onload
		AUTO_WAIT_START_EVENT_ONLOAD,
		AUTO_ROBOT_TO_PICK_WAIT_POS_ONLOAD,
		AUTO_ROBOT_TO_PICK_POS_ONLOAD,
		AUTO_FINGER_CYL_CLOSE,
		AUTO_SEND_PICK_FINISH,
		AUTO_WAIT_BATTERY_OFFLOAD_PNP_AVOID,
		AUTO_ROBOT_TO_PICK_WAIT_POS_ONLOAD_CHECK,
		AUTO_SEND_PICK_BATTERY_FINISH,
		AUTO_CHECK_HAVE_PRODUCT,
		AUTO_WAIT_MES_SINGLE,
		AUTO_JUDGE_HAVE_NG,

		//offload
		AUTO_WAIT_START_EVENT_OFFLOAD,
		AUTO_ROBOT_TO_PUT_WAIT_POS_OFFLOAD,
		AUTO_ROBOT_TO_PUT_POS_OFFLOAD,
		AUTO_WAIT_MES_SINGLE_FOR_OFFLOAD,
		AUTO_FINGER_CYL_OPEN_OFFLOAD,
		AUTO_SEND_DATE,
		AUTO_ROBOT_TO_PUT_WAIT_POS_OFFLOAD_CHECK,
		AUTO_CHECK_WITHOUT_PRODUCT_OFFLOAD,
		AUTO_SEND_PUT_FINISH_SIGNAL_OFFLOAD,
		AUTO_WAIT_OFFLOAD_TO_NEXT_FINISH,

		//ng
		AUTO_WAIT_START_EVENT_NG,
		AUTO_ROBOT_TO_NG_WAIT_POS,
		AUTO_ROBOT_TO_NG_POS,
		AUTO_WAIT_MES_SINGLE_FOR_NG,
		AUTO_FINGER_CYL_OPEN_NG,
		AUTO_ROBOT_TO_NG_WAIT_POS_CHECK,
		AUTO_CHECK_WITHOUT_PRODUCT_NG,
		AUTO_SEND_NG_FINISH_SIGNAL_NG,

		// buf
		AUTO_ROBOT_TO_BUF_WAIT_POS,
		AUTO_BUFFER_CYL_ACTION_BUF,
		AUTO_CHECK_HAVE_PRODUCT_BUF,
		AUTO_ROBOT_TO_BUF_POS,
		AUTO_FINGER_CYL_ACTION_BUF, 
		AUTO_ROBOT_TO_BUF_WAIT_POS_CHECK,
		AUTO_CHECK_WITHOUT_PRODUCT_BUF,

		AUTO_JUDGE_BATTERY_ALL_PUT_FINISH,//�ж����е�ط����
		AUTO_WORK_END,
	};

public:
	CRunProcessRobot(int nID);
	virtual ~CRunProcessRobot(void);

	BOOL InitializeConfig(CString strProcessModule);
	void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);
	BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove);
	BOOL CheckOutputCanActive(COutput *pOutput);
	BOOL SaveParameter();
	BOOL ReadRunParameter();
	void ReadStepInfo();
	BOOL SaveStepInfo(int nWriteType);

protected:
	void PowerUpOperation(void);
	void PowerUpRestart(void);
	void AutoOperation(void);

public:
	BOOL FingerCylClose (int nIndex,BOOL bClose);//��צ��������
	BOOL BufferCylPush(BOOL bPush);  

	BOOL CheckHasVaild(int nIndex, BOOL bHave);//����צ�Ƿ��в�Ʒ

	BOOL ConnectRobot();   //���ӻ�����
	BOOL DisconnectRobot();   //�Ͽ����ӻ�����
	BOOL RobotStop();
	BOOL RobotAlarmReset();
	BOOL RobotCodeReset();
	BOOL RobotCodeRun();
	void GetLocalPalletData(CPallet* pallet,CString & strResult);  //��ñ���������

	BOOL CheckRobotSafe(); //��������Z���Ƿ��ڰ�ȫλ

	BOOL CheckStartWithPalletDataSame();  //�����˼�צ�����ж�
	BOOL CheckStartWithPalletDataSameForBuffer(); //�������ж�
	void EmgStop();
	void NomalRunEvent(); //����η��������ź�

	BOOL CheckBackSafe();  //����ΰ�ȫ�ź�
	BOOL CheckNeedBatteryEvent(BOOL bNeed);//���Ҫ���ź�

	BOOL SendPlaceFinishSignal(BOOL bFinish);  //�����ź�
	BOOL SendSafeSignal(BOOL bSafe);//����ȫ�ź�
public:
	//��������
	int m_IBufferCylPush;
	int m_IBufferCylReturn;
	int m_OBufferCylPush;
	int m_OBufferCylReturn;

	//��ץ����  0---��λ1  1---��λ2
	int m_IFingerCylClose[2];
	int m_IFingerCylOpen[2];
	int m_OFingerCylClose[2];
	int m_OFingerCylOpen[2];

	// ���ϼ�� 0-1:��צ���ϼ��  2-3:�������ϼ��
	int m_IHasCore[4];	
	int m_IZCheckAbnormal;

	int m_IRobotAlarm;//�����˱���
	int m_IRobotRun;//������������
	int m_IRobotReset;//�����˸�λ״̬
	int m_IRobotEnable;//�������ŷ�

	int m_ORobotEnable;////�������ŷ�(����δ��)
	int m_ORobotStop;//������ֹͣ
	int m_ORobotCodeRun;//�����˳�����������
	int m_ORobotAlarmReset;//�����˱�����λ
	int m_ORobotCodeReset;//�����˳���λ
	int m_ORobotEmgStop;//�����˼�ͣ(��ʹ�ã������˶ϵ�ʹ�û�̨��ͣ)


	int m_ITailFingerSignal[2];//��ι�λ��������ź�

	int m_ITailSafeSignal[2];  //��ι�λ��ȫλ�ź� 

	int m_ORobotPutFinishSignal[2];//��������ź�

	int m_ORobotPutInPosSignal[2];//��ط��ϵ�λ�ź�  ��ȫ
	
	int m_INormalRunSingle; //��λ�̨���������ź�
	int m_ONormalRunSingle; //ǰ�λ�̨���������ź�

public:
	BOOL m_bEnableDocking;	// �Ƿ�����ζԽ�
	BOOL m_bRobotWaitPos;		//�������ڵȴ�λ �ȴ��ź�ʱʹ��
	BOOL m_bRobotSafe;			//��ȡ��ؼ�צ�������˰�ȫ�ź�
	unsigned int m_unRobotSpeed;
	unsigned int m_nCurRobotLoc;

	int m_nWorkIndex;					//��λ�������
	int m_nStartWithPalletData;	//������������״̬����
	int m_nBufferWithPalletData;//��������״̬����

	BOOL m_bThreadStart;
	BOOL m_bThreadEnd;
	BOOL m_bEnableRobot; //���û�����
	BOOL m_bBufferMes;    
	int m_nRobotPort;
	CString m_strRobotIp;
	CRobotClient m_pRobotClient;

	CPallet m_pBufferPallet;
	CPallet m_pTempPallet[2];

	BOOL m_bShieldServer;
	int m_nServerPort;
	CString m_strServerIp;
	CCCDCheckClient m_Client;   //��������ͻ���

};
