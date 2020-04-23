/****************************************************************************************
�ļ�����RunProcessBatteryCore.h
�������������о��λ
��ʷ�޸ļ�¼��
*****************************************************************************************/
#pragma once
#include "RunProcess.h"
#include "NetSenseClient.h"
#include "CCDCheckClient.h"

class CRunProcessBatteryCore : public CRunProcess
{
protected:
	enum POWER_UP_STEPS
	{
		PU_CHECK_ONLOAD_CORE_HOME_FINISH,
		PU_OPEN_PRESS_COMPORT,
		PU_COLUMNCYL_OPEN,
		PU_OPEN_CYL_OPEN,	
		PU_LIFT_CLY_DOWN,
		PU_CLOSE_VACUM,
		PU_SEND_MOTOR_HOME,
		PU_SEND_MOTOR_MOVE_TO_WAIT_POS,
		PU_ZERO_PRESS_DATA,

		PU_JUDGE_IF_WITH_PALLET,
		PU_CHECK_WITHOUT_CORE,

		PU_CHECK_PALLET_DATA,
		PU_WAIT_JUDGE_FINISH,
		PU_SAME_STATION_DATA_PAIR,

		
		PU_FINISH,
		NUM_OF_POWER_UP_STEPS,
	};

	enum AUTO_STEPS
	{
		AUTO_SEND_MOTOR_TO_WAIT_POS,
		AUTO_SEND_NEED_CORE_REQUIRE,
		AUTO_ClEAR_PRESS,
		AUTO_START_EVENT,
		AUTO_CHECK_HAE_PRODUCT,
		AUTO_CLUMMN_CYL_CLOSE,
		//�Ƶ�о
		AUTO_SEND_MOTOR_TO_PICK_POS,
		AUTO_LIFT_CYL_UP,
		AUTO_CEHCK_VACUM_STATUS,
		AUTO_CLUMN_CYL_OPEN,
		
		AUTO_JUDGE_HAVE_SHELL,
		AUTO_SEND_MOTOR_TO_INSHELL_WAIT_POS,
		AUTO_WAIT_SHELL_READY,
		AUTO_CHECK_OPEN_CYL_CLOSE,
		AUTO_SEND_MOTOR_TO_OPEN_CYL_OPEN_POS,
		//���
		AUTO_TWICE_CLY_RETURN,
		AUTO_OPEN_CYL_OPEN,
		AUTO_CHECK_OPEN_CYL_OPEN,
		AUTO_SEND_MOTOR_TO_INSHELL_POS,
		AUTO_ANALYZE_PREESS_DATA,
		AUTO_SAVE_ALL_PRESS_DATA,
		AUTO_SEND_INSHELL_FINISH_SINGLE,
		AUTO_SEND_MOTOR_TO_WAIT_POS_EX,

		AUTO_WORK_END,
	};

	enum MOTOR_LOC
	{
		SEND_MOTOR_WAIT_LOC=0,  //��ʼλ
		SEND_MOTOR_PICK_POS,//�ӵ�оλ
		SEND_MOTOR_INSHELL_WAIT_LOC,//��ǵȴ�λ
		SEND_MOTOR_OPEN_CYL_OPEN_LOC,//���������ſ�λ
		SEND_MOTOR_INSHELL_LOC,//���λ
    };

public:
	CRunProcessBatteryCore(int nID);
	virtual ~CRunProcessBatteryCore(void);
	BOOL InitializeConfig(CString strProcessModule);
	void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);
	BOOL  CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove); 
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
	BOOL ColumnCylClose(BOOL bClose);//��������
	BOOL OpenCylOpen(BOOL bOpen);//��������
	BOOL CheckCylClose(); //��鿪�����׹ر�
	BOOL CheckCylOpen(BOOL bOpen);//��鿪�����״�
	BOOL LiftCylUp(BOOL bUp);//��������
	BOOL CloseOpenCyl(void);//�رտ�������

public:
	void RunWhile();
	double ReadPress();
	double GetPressMaxValue();
	BOOL SavePressValue();// ����ѹ��ֵ 

	//�������������ʵ�ʼ������һ����,nCheckResultΪ�������0û���ϣ�1����
	BOOL CheckStartWithPalletDataSame();
	
	
public:
	//��������
	int m_IColumnCylOpen[2];
	int m_IColumnCylClose[2];
	int m_OColumnCylClose;
	int m_OColumnCylOpen;

	//��������
	int m_IOpenCylOpen[2];
	int m_IOpenCylClose[2];
	int m_OOpenCylOpen;  //�������״�
	int m_OOpenCylClose; //�������׹ر�
	
	//��������
	int m_ILiftCylUp;
	int m_ILiftCylDown;
	int m_OLiftCylUp;
	int m_OLiftCylDown;

	int m_IOpenCylOpenTwo[2]; //�������׹رն���ȷ�ϸ�Ӧ

	//��ռ��
	int m_ICheckVacum;//��ռ��
	int m_OOpenVacum;//��յ�ŷ�

	int m_IHaveCore;//���ϼ��	
	
	int m_OSetZero;//ѹ�������� 

	int m_MotorX;//�͵�о���

	int m_IReachOpenAndClosePos;   //������￪�����״�λ��Ӧ

public:
	//��������
	BOOL m_bDataJudgeFinish;  //�ȴ���һ�����оģ�������ж����  ˫��λ���ݶԱ�ʹ��
	BOOL m_bResetJudge;   //�������� m_bDataJudgeFinish ��־
	BOOL m_bPowerUpFinish;//����ǹ�λ���ϸ�λ���

	BOOL m_bRequireCore;
	BOOL m_bOpenLoc;   //�������״�
	int m_nTryReadTimes;//��ѹ������
	int m_nStartWithPalletData;// ������������״̬����

	BOOL  m_bThreadStart;
	BOOL  m_bThreadEnd;

public:
	// ѹ�����
	double m_fPressNormValue[2];	// �趨ֵ 0:min  1:max
	BOOL m_bEnablePressSenseClient;
	CString m_strPressSenseIP;
	int m_nPressSensePort;
	CCriticalSection m_PressDataLock;
	BOOL  m_bStartGetValue;	//��ʼȡѹ��ֵ   
	BOOL  m_bGetValueFinish;	//ȡֵ����
	CNetSenseClient m_PressSenseClient;
	vector<double> m_vecPressValue;
	CCCDCheckClient m_PressClient;
};