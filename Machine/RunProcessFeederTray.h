/****************************************************************************************
�ļ�����CRunProcessFeederTray.h
�������������̰���ģ��
��ʷ�޸ļ�¼��
*****************************************************************************************/
#pragma once
#include "RunProcess.h"

class CRunProcessFeederTray : public CRunProcess
{
protected:
	enum POWER_UP_STEPS
	{
		PU_CHECK_ANOTONER_MODULE_SAFE,
		PU_MOVE_MOTOR_X_HOME,
		PU_CHEACK_HAVE_PALLET,
		PU_OPEN_CLAMP_CYL,
		PU_OPEN_FINGLE_CYL,
		PU_MOTOR_TO_ONLOAD_POS,
		PU_CHECK_MOTOR_INPOS,
		PU_FINISH,
		NUM_OF_POWER_UP_STEPS,
	};

	enum AUTO_STEPS
	{
		AUTO_JUDGE_HAVE_PALLET,
		AUTO_MOVE_MOTOR_TO_PICK_PALLET_POS,
		AUTO_CHECK_MOTOR_MOVE_ONLOAD_INPOS,
		AUTO_SEND_NEDD_PALLET_SINGLE_TO_ONLOAD,
		AUTO_WAIT_ONLOAD_MODULE_SEND_PALLET_FINISH,

        AUTO_CLAMP_CHECK_HAVE_PALLET,
		AUTO_CLMAP_CYL_CLAMP,
		AUTO_FINGLE_CYL_CLAMP,
		AUTO_CHECK_HAVE_PALLET_AGANIN,
		AUTO_WAIT_ONLOAD_MODULE_MOVE_SAFE_POS,
		AUTO_MOVE_MOTOR_TO_START_FEED_COVER_POS,
		AUTO_WAIT_START_EVENT,
		AUTO_MOVE_MOTOR_TO_FEED_COVER_POS,
		AUTO_WAIT_PICK_COVER_FINISH,
		AUTO_JUDGE_ALL_COVE_PICK_FINISH,
		AUTO_MOVE_PALLET_TO_OFFLAOD_POS,
		AUTO_CHECK_MOTOR_MOVE_OFFLOAD_INPOS,
		AUTO_SEND_OFFLAD_MODUEL_PICK_PALLET,
		AUTO_WAIT_OFFLOAD_MODULE_LIFT_FINISH,
		AUTO_CLMAP_CYL_OPEN_OFFLOAD,
		AUTO_FINGLE_CYL_CLAMP_CLOSE,
		AUTO_WAIT_OFFLOAD_MOVE_SAFE,
		AUTO_JUDGE_PICK_PALLET_ALL_FINISH,
		AUTO_CHECK_HAVE_NO_PALLET,
		AUTO_JUDGE_PICK_PALLET_TIMES,
		AUTO_CHECK_PALLET_REMOVE,
		AUTO_WORK_END,
	};
	
	enum MOTOR_WORK_LOC
	{
		X_MOTOR_ONLOAD_LOC1 = 0,
		X_MOTOR_ONLOAD_LOC2,
		X_MOTOR_OFFLOAD_LOC,
		X_PICK_COVER_1,
		X_PICK_COVER_2,
		X_PICK_COVER_3,
		X_PICK_COVER_4,
		X_PICK_COVER_5,
		X_PICK_COVER_6,
		NUM_WORK_LOC
	};

public:
	CRunProcessFeederTray(int nID);
	virtual ~CRunProcessFeederTray(void);

public:
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
	void WorkWhile(void);//��ײ��Ӧ�͹�դ���
	void Destory(void);//�����Դ
	BOOL CheckHaveNoPallet(); //�����������
	BOOL CheckCanHome(void);//�ж�����ģ���Ƿ�ȫ
	void ResertHomeSingle(void);//���ø�λ�ź�
	BOOL OpenVacuum(BOOL bOpen);//�����
	BOOL SeparateClose(BOOL bClose);//�������׶���
	BOOL CheckPalletInPos();//��������Ƿ�λ

public:
	int m_ISeparateClose[2];//���������˸�Ӧ
	int m_ISeparateOpen[2];//���������Ƴ���Ӧ
	int m_OSeparateOpen;//��������1��2�Ƴ���ŷ�
	int m_OSeparateClose;//��������1��2���˵�ŷ�

	int m_IPalletInPosition;//���̸�Ӧ
	int m_ITrayInPosPresent[3];//0����1��λ��2����2��λ��3���ϵ�λ

	int m_IVacuum[6];//������������
	int m_OVacuum[6];//������������

	int m_ISafeRasterCheck[3];//��դ��Ӧ
	int m_ISafeCheckPallet;//��ײ��Ӧ

	int m_nMotorX;//X���

public:
	BOOL m_bPutPalletFinish;//�������
	BOOL m_bPickPalletFinish;//ȡ�����

	BOOL m_bMoveMotorFinish;//����ƶ���ɱ��

	int m_nMoveMotorIndex;//ȡ����ʱ����ƶ�λ��
	int m_nPickPalletPos;	// ��ǰץȡ���̵Ķ�ջ���
	BOOL m_bOnloadPnpRequireShell;	// ȡ���ǻ�е�ֽ����ź�

	BOOL m_bThreadFinished;
	BOOL m_bThreadStart;
};