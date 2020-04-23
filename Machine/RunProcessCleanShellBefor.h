/****************************************************************************************
�ļ�����RunProcessCleanShellBefor.h
���������������ϴǰ��ת
��ʷ�޸ļ�¼��
*****************************************************************************************/
#pragma once
#include "RunProcess.h"

class CRunProcessCleanShellBefor : public CRunProcess
{
protected:
	enum POWER_UP_STEPS
	{
		PU_WATI_FRONT_MODULE_HOME,
		PU_CHECK_HAVE_SHELL,
		PU_TRANSFER_CYL_RETURN,
		PU_ROTATE_RETURN,
		PU_CLOSE_VACU_AND_AIR,
		PU_BUTTON_RESET,
		PU_FINISH,
		NUM_OF_POWER_UP_STEPS,
	};

	enum AUTO_STEPS
	{
		AUTO_TRANSFER_CYL_RETURN,
		AUTO_ROTATE_RETURN,
		AUTO_SEND_NEED_REQUIRE_EVENT,
		AUTO_WAIT_START_EVENT,
		AUTO_VACUUM_OPEN,
		AUTO_CHECK_SHELL_DIRECTION,
		AUTO_MANUAL_PICK_SHELL_CLOSE_VACUUM,//�˹�ȡ�ǹ����
		AUTO_WAIT_MANUAL_PICK_SHELL_FINISH,//�ȴ��˹�ȡ�����
		AUTO_TRANSFER_CYL_PUSH,
		AUTO_WAIT_NEXT_MODULE_PICK,
		AUTO_ROTATE_PUSH,//���Ƴ���ģ��
		AUTO_WAIT_NEXT_MODULE_FINGER_CLOSE,
		AUTO_VACUUM_CLOSE,
		AUTO_SEND_SAFE_SIGNAL,
		AUTO_TRANSFER_CYL_RETURN_OFFLOAD,
		AUTO_WORK_END,
		NUM_OF_AUTO_STEPS,
	}; 

public:
	CRunProcessCleanShellBefor(int nID);
	virtual ~CRunProcessCleanShellBefor(void);

	BOOL InitializeConfig(CString strProcessModule);
	void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);
	BOOL CheckOutputCanActive(COutput *pOutput);

protected:
public:
	void PowerUpOperation(void);
	void PowerUpRestart(void);
	void AutoOperation(void);

public:
	BOOL RotateClyPush(BOOL bPush);//��ת����
	BOOL TransferCylPush(BOOL bPush);//�޸����׶�������
	BOOL OpenVacuum(BOOL bOpen);//�����
	BOOL CheckVacuum(BOOL bHave);//��ռ��
	BOOL CheckDirection();//�ǿڼ��

	BOOL ButtonCheck();//��ť���¼��

public:
	//�ǿڳ�����
	int m_IShellDirection[2];
	//��������
	int m_IVacuum[2];  
	int m_OVacuum[2];  

	//�޸�����
	int m_ITransferCylPush;  
	int m_ITransferCylReturn;
	int m_OTransferCylPush;  
	int m_OTransferCylReturn;

	//��ת����
	int m_IRotateCylPush;  
	int m_IRotateCylReturn;
	int m_ORotateCylPush;  
	int m_ORotateCylReturn;

	//��ť��
	int m_IManualBtn;
	int m_OManualBtnLight;
};