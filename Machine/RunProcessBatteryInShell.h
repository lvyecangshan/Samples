/****************************************************************************************
�ļ�����RunProcessBatteryInShell.h
���������������ǹ�λ
��ʷ�޸ļ�¼��
*****************************************************************************************/
#pragma once
#include "RunProcess.h"

class CRunProcessBatteryInShell : public CRunProcess
{
protected:
	enum POWER_UP_STEPS
	{
		PU_WAIT_MODULE_HOME,
		PU_JUDGE_IF_WITH_PALLET,//�жϴ�������

		PU_CHECK_WITHOUT_SHELL,
		PU_CLUMN_CYL_OPEN,
		PU_LIFT_CYL_DOWN,
		PU_SEND_PUSH_CYL_RETURN,
		PU_VACUM_CYL_UP,
		PU_CLOSE_VACUM,
		
		PU_WAIT_BATTER_CORE_SIGNAL_WITH_PALLET,
		PU_CHECK_PALLET_DATA,
		//��֤������ǹ�λ��Ʒ�ɶ�
		PU_WAIT_JUDGE_FINISH,
		PU_SAME_STATION_DATA_PAIR,
		
		PU_FINISH,
		NUM_OF_POWER_UP_STEPS,
	};

	enum AUTO_STEPS
	{
		AUTO_SEND_SHELL_CORE_REQUIRE,
		//���Ƕ�λ
		AUTO_START_EVENT,
		AUTO_CHECK_HAVE_PRODUCT,
		AUTO_LIFT_CYL_UP,
		AUTO_SEND_TWICE_PUSH_CYL_PUSH,
		AUTO_TAIL_VACUUM_OPEN,
		AUTO_WAIT_CARRYSHELL_FINISH,
		
		//�Ϳ�
		AUTO_SEND_ONE_PUSH_CYL_PUSH,
		AUTO_CLUMMN_CYL_OPEN,
		AUTO_OPEN_CYL_CLOSE,
		AUTO_VACUM_CYL_DOWN,
		AUTO_MOUTH_VACUUM_OPEN,
		AUTO_VACUM_CYL_UP,
		AUTO_CHECK_VACUM,
		
		//�ȴ����о
		AUTO_CLOSE_VACUM,
		AUTO_WAIT_CORE_READY,

		//����
		AUTO_LIFT_CYL_DOWN,
		AUTO_WAIT_NEXT_MODULE_REQUIRE,
		AUTO_WAIT_NEXT_MODULE_WAORK_FINISH,
		AUTO_SEND_CYL_RETURN,
		AUTO_CLUMMN_CYL_CLOSE,
		AUTO_CHECK_NO_HAVE_PRODUCT,
		AUTO_WORK_END,
	};	

public:
	CRunProcessBatteryInShell(int nID);
	virtual ~CRunProcessBatteryInShell(void);

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
	BOOL LocateCylPush( BOOL bPush);	// �ǿڶ�λ
	BOOL ColumnCylClose(BOOL bClose);//��������
	BOOL SendOneCylPush(BOOL bPush);//һ�δ�������
	BOOL SendTwoCylPush(BOOL bPush);//���δ�������
	BOOL DownLiftCylUp(BOOL bLift);//���涥������
	BOOL VacumCylPush(BOOL bPush);//������� ���ǿ���������
	BOOL VacumOpenOrCheck(BOOL bOpen , BOOL bCheck); //��մ�����

	
	//�������������ʵ�ʼ������һ����,nCheckResultΪ�������0û���ϣ�1���ϣ�2�пǵ���û�����
	BOOL CheckStartWithPalletDataSame();


public:
	//���Ƕ�λ����
	int m_IColumnCylOpen[2];
	int m_IColumnCylClose[2];
	int m_OColumnCylClose;
	int m_OColumnCylOpen;

	// �ǿڶ�λ
	int m_ILocateShellPush[2];
	int m_ILocateShellBack[2];
	int m_OLocateShellPush;
	int m_OLocateShellBack;

	//��������
	int m_ILiftCylUp;
	int m_ILiftCylDown;
	int m_OLiftCylUp;
	int m_OLiftCylDown;
	
	//һ�����������
	int m_ISendOneCylPush;
	int m_ISendOneCylBack;
	int m_OSendOneCylPush;
	int m_OSendOneCylBack;

	//�������������
	int m_ISendTwoCylPush;
	int m_ISendTwoCylBack;
	int m_OSendTwoCylPush;
	int m_OSendTwoCylBack;

	int m_IHaveCore;// ���ϸ�Ӧ

	//�ǿ�������� ���ǿ���������
	int m_IVacumCylPush[2];
	int m_IVacumCylBack[2];
	int m_OVacumCylPush[2];

	int m_OVacumCyl[2];//������ϡ��¿ǿ����
	int m_ICheckVacum[2];//��ռ��
	int m_OBreakVacum;//�����

	int m_ITailVacum;//�ײ����
	int m_OTailVacum;//�ײ����


public:
	//���ϸ�λ
	BOOL m_bDataJudgeFinish;  //�ȴ���һ�����оģ�������ж����  ˫��λ���ݶԱ�ʹ��
	BOOL m_bResetJudge;   //�������� m_bDataJudgeFinish ��־

	int m_nRetryTimes;
	BOOL m_bCylClosed;
	BOOL m_bShellReady;// ����׼����
	int m_nStartWithPalletData;
};