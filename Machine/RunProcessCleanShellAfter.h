/****************************************************************************************
�ļ�����RunProcessCleanShellAfter.h
���������������ϴǰ��ת
��ʷ�޸ļ�¼��
*****************************************************************************************/
#pragma once
#include "RunProcess.h"

class CRunProcessCleanShellAfter : public CRunProcess
{
protected:
	enum POWER_UP_STEPS
	{
		PU_WATI_NEXT_MODULE_HOME,
		PU_CHECK_WITHOUT_SHELL,
		PU_TRANSFER_CYL_RETURN,
		PU_ROTATE_RETURN,
		PU_CLOSE_VACU_AND_AIR,
		PU_FINISH,
		NUM_OF_POWER_UP_STEPS,
	};

	enum AUTO_STEPS
	{
		AUTO_VACUUM_CLOSE,
		AUTO_TRANSFER_CYL_RETURN,
		AUTO_ROTATE_RETURN,


		AUTO_WAIT_START_EVENT,
		AUTO_TRANSFER_CYL_PUSH,
		AUTO_VACUUM_OPEN,
		AUTO_SEND_PICK_SIGNAL,
		AUTO_WAIT_NEXT_MODULE_LIFT,
		AUTO_ROTATE_PUSH,
		AUTO_SEND_SAFE_SIGNAL,

		AUTO_TRANSFER_CYL_RETURN_OFFLOAD,
		AUTO_SEND_READY_SIGNAL,
		AUTO_WAIT_NEXT_MODULE_SAFE_SIGNAL,
		
		AUTO_WORK_END,
		NUM_OF_AUTO_STEPS,
	}; 

public:
	CRunProcessCleanShellAfter(int nID);
	virtual ~CRunProcessCleanShellAfter(void);

	BOOL InitializeConfig(CString strProcessModule);
	void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);
	BOOL CheckOutputCanActive(COutput *pOutput);
	
protected:
	void PowerUpOperation(void);
    void PowerUpRestart(void);
    void AutoOperation(void);
	
public:
	BOOL RotateClyPush(BOOL bPush);//��ת����
	BOOL TransferCylPush(BOOL bPush);//ƽ�����׶�������
	BOOL OpenVacuum(BOOL bOpen);//�����
	BOOL CheckVacuum(BOOL bHave);//��ռ��

public:
	//��������
	int m_IVacuum[2];  
	int m_OVacuum[2];  
	
	//��ת����
	int m_IRotateCylPush;  
	int m_IRotateCylReturn;
	int m_ORotateCylPush;  
	int m_ORotateCylReturn;

	//ƽ������
	int m_ITransferCylPush;  
	int m_ITransferCylReturn;
	int m_OTransferCylPush;
	int m_OTransferCylReturn;

public:
	BOOL m_bOffloadRequire;
};