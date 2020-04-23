/****************************************************************************************
�ļ�����RunProcessCleanShell.cpp
���������������ϴģ��
��ʷ�޸ļ�¼��
*****************************************************************************************/
#pragma once
#include "RunProcess.h"

class CRunProcessCleanShell : public CRunProcess
{
protected:
	enum POWER_UP_STEPS
	{
		PU_CLOSE_AIR,
		PU_JUDGE_IF_WITH_PALLET,//�жϴ�������
		
		PU_CHECK_WITHOUT_SHELL,
		PU_FINGER_CYL_OPEN,
		PU_LIFT_CYL_DOWN,
		
		JUDGE_SHELL_NUM,
		PU_CHECK_FINGER_STATE,
		PU_FINGER_CYL_CLOSE,
		WAIT_FRONT_AFTER_SAFE_SIGNAL,
		PU_LIFT_CYL_UP,
		PU_FINISH,
		NUM_OF_POWER_UP_STEPS,
	};

	enum AUTO_STEPS
	{
		AUTO_CHECK_WITHOUT_SHELL,
		AUTO_FINGER_CYL_OPEN,
		AUTO_LIFT_CYL_DOWN,

		//ONLOAD
		AUTO_SEND_REQUIRE_EVENT,
		AUTO_WAIT_START_EVENT,
		AUTO_LIFT_CYL_UP,
		AUTO_FINGER_CYL_CLOSE,
		AUTO_CHECK_HAVE_SHELL, //�򿪴�������ʱ
		AUTO_SEND_FINGER_CYL_CLOSE_SINGLE,
		AUTO_WAIT_FRONT_MODULE_MOVE_SAFE_POS,
		AUTO_WAIT_AIR_TIME_OUT,

		//OFFLOAD
		AUTO_SEND_OFFLOAD_REQUIRE,
		AUTO_WAIT_REQUIRE_RESPONSE,
		AUTO_FINGER_CYL_OPEN_OFFLOAD,
		AUTO_LIFT_CYL_DOWN_OFFLOAD,
		AUTO_SEND_LIFT_CYL_DOWN_EVENT,
		AUTO_WAIT_NEXT_MODULE_MOVE_SAFE_POS,
		AUTO_WORK_END,
	}; 

public:
	CRunProcessCleanShell(int nID);
	virtual ~CRunProcessCleanShell(void);

	BOOL InitializeConfig(CString strProcessModule);
	void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);\
	BOOL CheckOutputCanActive(COutput *pOutput);
	BOOL SaveParameter();
	BOOL ReadRunParameter();

protected:
	void PowerUpOperation(void);
	void PowerUpRestart(void);
	void AutoOperation(void);

public:
	BOOL FingerCylClose(BOOL bClose);//��צ��������
	BOOL LiftCylUp(BOOL bUp);//�������׶�������
	int JudgeShellNum();//�������� 

public:
	//��������
	int m_ILiftCylUp;  
	int m_ILiftCylDown;
	int m_OLiftCylUp; 
	int m_OLiftCylDown;
	//��ץ
	int m_IFingerCylClose[2];
	int m_IFingerCylOpen[2];
	int m_OFingerCylClose;
	int m_OFingerCylOpen; 

	int m_IHaveProduct[2];//��ץ���ϸ�Ӧ
	int m_OAirBlow; //������ŷ�
	int m_OAirExhaust; //������ŷ�  ��ȷ��

public:
	DWORD m_nAirTime;     //������ʱ
	int m_nAirTimelimit;//������ʱ
	BOOL m_bOffloadRequire;
	BOOL m_bFingerClose;//��צ�н��ź� ��ǰ��ת�źŽ���
	BOOL m_bLiftDown;   //�����½��ź�	���ת�źŽ���
};