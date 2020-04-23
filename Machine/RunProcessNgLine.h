/****************************************************************************************
�ļ�����RunProcessNgLine.h
����������ѹ��������������
��ʷ�޸ļ�¼��
*****************************************************************************************/
#pragma once
#include "RunProcess.h"

class CRunProcessNgLine : public CRunProcess
{
protected:
	enum POWER_UP_STEPS
	{
		PU_WAIT_SAFE_SIGNAL,
		PU_MANUAL_RELEASE,
		PU_CHECK_WITHOUT_PRODUCT,
		PU_WAIT_MANUAL_CLEAR_BATTERY,
		PU_FINISH,
		NUM_OF_POWER_UP_STEPS,
	};

	enum AUTO_STEPS
	{
		AUTO_SEND_REQUIRE_SIGNAL,
		AUTO_WAIT_OFFLOAD_FINISH_SIGNAL,
		AUTO_CHECK_HAVE_CORE,
		AUTO_MOTOR_MOVE_TO_RECV_NG,
		AUTO_CHECK_FULL,
		AUTO_WAIT_MANUAL_OPERATOR,
		AUTO_WORK_END,
	};

public:
	CRunProcessNgLine(int nID);
	virtual ~CRunProcessNgLine(void);

	BOOL InitializeConfig(CString strProcessModule);
	void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);
	BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove);
	BOOL CheckOutputCanActive(COutput *pOutput);

protected:
	void PowerUpOperation(void);
	void PowerUpRestart(void);
	void AutoOperation(void);


public:
	BOOL CheckHaveProduct();//��ⲻ��������
	void WorkWhile();
	void Destory();

public:	
	///�Ų�������λ��Ӧ
	int m_ICheckBattery[2];

	//��ص�λ��Ӧ
    int m_ICheckInPos;

	//���ϸ�Ӧ
    int m_ICheckLineFull;
    
	int m_OLineMotor;//Ƥ�����

	// �Ų�����ť
	int m_IManualBtn;
	int m_OManualBtnLight;
	
public:
	int m_nNgLineID;
	int m_nDelayTime;
	BOOL m_bRequireNgBattery;
	BOOL m_bThreadEnd;
	BOOL m_bThreadStart;
};