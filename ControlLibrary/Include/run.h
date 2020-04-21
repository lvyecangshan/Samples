/*******************************************************************
�ļ�����run.h
���ߣ�              �汾��A1        ���ڣ�
��������������ƽ̨���л���
��ʷ�޸ļ�¼��
*******************************************************************/
#pragma once

#include "RunTask.h"
#include "Evt.h"
#include "Input.h"
#include "Output.h"
#include "motor.h"

#define MSG_SHOW_DEFAULT -1
#define MSG_NOT_NEED   0

class CRunCtrl;

class __declspec(dllexport) CRun : public CRunTask
{
	friend CRunCtrl;

public:
	enum RUN_MODE
	{
		RUN_MODE_POWER_UP,	//��ʼ��
		RUN_MODE_AUTO,		//�Զ�����
		RUN_MODE_STOP,		//ֹͣ
	};
	
	enum RUN_STATE
	{	
		RUN_STATE_RUNNING,	//������
		RUN_STATE_STOP		//ֹͣ
	};

	//����WaitMultipleInputs������ʹ��
    enum WAIT_INPUT_MODE
	{
		WAIT_ALL_STATE_ACTIVE,
		WAIT_ONE_IS_STATE,
		WAIT_ALL_ON,
		WAIT_ONE_IS_ON,
		WAIT_ALL_OFF,
		WAIT_ONE_IS_OFF,
	};

public:
	CRun(int nID);
	virtual ~CRun();

public:
	/*********************************************************************************
	//�����������豸��ʼ�����̺���
	//�����������
	//��������: ��
	**********************************************************************************/
	virtual void PowerUpOperation();

	/*********************************************************************************
	//�����������Զ��������̺���
	//�����������
	//��������: ��
	**********************************************************************************/
	virtual void AutoOperation();

	/*********************************************************************************
	//����������������λ��Ӧ����,ͨ����������������ʼ������
	//�����������
	//��������: ��
	**********************************************************************************/
	virtual void PowerUpRestart() { m_strMsg = _T("Ready"); ClearMessage(); }

	/*********************************************************************************
	//������������ʾָ��ID�ŵ���Ϣ��������Ϣ��Message.cfg����ӺͲ鿴
	//			�����ŵ㣺�����������̣߳�������ֱ����ʾ���ϱ���
	//���������nMsgID		��Ϣ���
	//			data1		������Ϣ1��һ��û��
	//			data2		������Ϣ2��һ��û��
	//��������: ��
	**********************************************************************************/
	void ShowMessage(int nMsgID, UINT data1 = 0, int data2 = 0);

	/*********************************************************************************
	//������������ʾָ���ַ����ĸ澯��Ϣ
	//���������strMsg		��Ϣ�ַ���
	//			nType		��ʾ���� 0����ͨ��ʾ����ɫ�� 1���ʴ���ʾ����ɫ�� 2����ͨԤ������ɫ�� 3�����ϱ�������ɫ��
	//��������: ��
	**********************************************************************************/
	void ShowMessage(CString strMsg, int nType);

	/*********************************************************************************
	//�����������������I/O��״̬
	//			ע�⣺��鵥����㱨���ã������ô˺�����ѭ��״̬�жϣ�����ѭ���жϿ�ֱ����Input����ĺ���������ÿ�ε��û������Ӧ��־��¼
	//���������pInput		����I/Oָ��
	//			bState		����I/OӦ��״̬��һ��ʹ��ON��OFF��ʾ��������TRUE��FALSE
	//			nMsgID		�粻����Ҫ��bState״̬ʱ����쳣��Ϣ(ͬShowMessage())���� δָ������Ĭ����Ϣ����. 
	//��������: TRUE���������bState��ͬ  FALSE���������bState��ͬ
	**********************************************************************************/
	BOOL CheckInput(CInput *pInput, BOOL bState, int nMsgID = MSG_SHOW_DEFAULT);

	/*********************************************************************************
	//�����������ȴ�����I/O��״̬�û���ָ��״̬
	//���������pInput		����I/Oָ��
	//			bState		����I/OӦ��״̬��һ��ʹ��ON��OFF��ʾ��������TRUE��FALSE
	//			nMsgID		�粻����Ҫ��bState״̬ʱ����쳣��Ϣ(ͬShowMessage())���� δָ������Ĭ����Ϣ������û��˲���������Ϣ����
	//						MSG_SHOW_DEFAULT ΪĬ�ϣ�MSG_NOT_NEED Ϊ����Ҫ����������Ϣ�����������MSG_NOT_NEEDֵΪ�Զ�����ϢID,�Զ�����ϢID��Ҫע�⣬����Ҫ����easylibdef.h�ļ���LIBRARY_MSG_ID�������ֵ
	//��������: WAIT_OK��	�������bState��ͬ
	//			WAIT_ERROR����ʱ(ʱ���趨����Input.cfg)
	//			WAIT_STOP��	��ֹͣ
	**********************************************************************************/
	int  WaitInput(CInput *pInput, BOOL bState, int nMsgID = MSG_NOT_NEED);

	/*********************************************************************************
	//������������ȡ����ģ��ʹ��״̬
	//�����������
	//��������: TRUE��ʹ��  FALSE����ֹ/�ر�
	**********************************************************************************/
	BOOL IsModuleEnable();

	/*********************************************************************************
	//���������������Ḵλ
	//���������pMotor	���ָ��
	//��������: TRUE�����гɹ�  FALSE������ʧ��
	**********************************************************************************/
	BOOL HomeMotor(CMotor *pMotor);

	/*********************************************************************************
	//���������������ͬʱ��λ
	//���������pMotor	���ָ������
	//			nMotors ������
	//��������: TRUE�����гɹ�  FALSE������ʧ��
	**********************************************************************************/
	BOOL HomeMotors(CMotor **ppMotorArray, int nMotors);
	    
	/*********************************************************************************
	//��������������ָ�����I/O��ֱ��ָ������I/OΪָ��״̬���򷵻أ�����澯
	//���������pInput		����I/Oָ��
	//			bInState	����I/O״̬��һ��ʹ��ON��OFF��ʾ��������TRUE��FALSE
	//			pOutput		���I/Oָ��
	//			bOutState	���I/O״̬��һ��ʹ��ON��OFF��ʾ��������TRUE��FALSE
	//			nMsgID		�粻����Ҫ��bState״̬ʱ����쳣��Ϣ(ͬShowMessage())���� δָ������Ĭ����Ϣ����. 
	//��������: WAIT_OK��	���pInput�ź�״̬ΪbInState
	//			WAIT_ERROR����������
	//			WAIT_STOP��	��ֹͣ
	//			WAIT_TIMEOUT�� ��ʱ(ʱ���趨����Output.cfg)
	**********************************************************************************/
	int IoAction(CInput *pInput, BOOL bInState, COutput* pOutput, BOOL bOutState, int nMsgID = MSG_SHOW_DEFAULT);

	/*********************************************************************************
	//�������������һ�㿿IO������ֱ����������ƶ�
	//���������pInStop		ָ��ֹͣ�ź�������ָ��
	//			pOutMove	ָ������������ָ��
	//			bInStopState ֹͣ�ź�������״̬
	//			pInAlarm	ָ�򱨾��ź�������ָ��
	//			pOutBreaker ָ������ɲ��������ָ��
	//			pSubStop	ָ����ֹͣ�ź�������ָ��
	//��������: WAIT_OK��	���pInStop����pSubStop�ź�ΪON
	//			WAIT_ERROR���������ػ��������
	//			WAIT_STOP��	����ֹ
	//			WAIT_TIMEOUT�� ��ʱ
	**********************************************************************************/
	int IoMotorMove(CInput* pInStop, COutput* pOutMove, BOOL bInStopState = ON, CInput* pInAlarm = NULL, COutput* pOutBreaker = NULL, CInput* pSubStop = NULL);

	/*********************************************************************************
	//�����������ȴ��¼���ָ���ź�״̬
	//���������pEvt		ָ�򵥸��¼���ָ��
    //			bState		Ҫ����״̬
    //			nFailMsgID	�粻����Ҫ��bState״̬ʱ�������Ӧ����Ϣ(ͬShowMessage())����û��˲���������Ϣ����
	//��������: WAIT_OK��	�������bState��ͬ
	//			WAIT_ERROR���������ػ��������
	//			WAIT_STOP��	����ֹ
	//			WAIT_TIMEOUT�� ��ʱ(ʱ����Ե����¼�����SetTimeout()�趨��Ĭ��ΪINFINITE����λΪ����)
	**********************************************************************************/
	int  WaitEvent(CEvt *pEvt, BOOL bState = TRUE, int nMsgID = MSG_NOT_NEED);

	int  WaitMultipleInputs(CInput **ppInputArray, BOOL *bStateArray, int nNums, BOOL bWaitMode = WAIT_ALL_ON, int nMsgID = MSG_NOT_NEED);
	int  WaitResetEvent(CEvt *pEvt, BOOL bState = TRUE, int nMsgID = MSG_NOT_NEED);
	
	// ���Ƽ�ʹ��
	BOOL MoveMotor(CMotor *pMotor, int loc, int nSubIndex = 0, double dOffset = 0.0);
	BOOL MoveMotors(CMotor **ppMotorArray, int *pLocArray, int nMotors);
	BOOL MoveMotorPos(CMotor *pMotor, double dPos, int setloc = -1, double svDrange = 0.0, double svARange = 0.0);
	BOOL MoveMotorsPos(CMotor **ppMotorArray, double *pdlPos, int nMotors, int *pSetlocs = NULL);
	BOOL JogMotor(CMotor *pMotor, double dDist, int setloc = -1);

	void Suspend() { m_bSuspended = TRUE; }
	void ClearMessage(int nMsgID = -1);

	int GetMode();
	int	GetID()		{ return m_nRunID; }
	BOOL IsJam()		{ return m_bJam; }
	void Enable(BOOL bEnable) { m_bEnabled = bEnable; }

protected:
	///////////////////////////////////
	//�ڲ�����
	UINT RunThread();
	void RunPowerUpMode();
	void RunAutoMode();

	void PowerUp();
	void Run();
	void Stop();
	void Restart();

	void ResetPowerUpComplete(void);
	void OnJamReset(void);
	void SetMsgProcessWnd(HWND hwnd);
	void Resume() { m_bSuspended = FALSE; ClearMessage(); }

	BOOL IsInPowerUpMode() { return m_nMode == RUN_MODE_POWER_UP; }
	BOOL IsInRunMode() { return m_nMode == RUN_MODE_AUTO; }
	BOOL IsSuspended() { return m_bSuspended; }
	
protected:
	int	m_nNextAutoStep;			//�Զ��������̲����ʶ���������ڱ�ʶ��ǰ�Զ����̲���
	int	m_nNextPowerUpStep;			//�豸��ʼ�����̲����ʶ���������ڱ�ʶ��ǰ��ʼ�����̲���
	int	m_nMode;					//����ģʽ�����ϡ������̡���ȫ�����У�
	int	m_eRunStatus;				//����״̬���ο�RUN_STATEö������
	int	m_nRunID;					//

	BOOL m_bEnabled;				//ģ���Ƿ�����
	BOOL m_bJam;					//�Ƿ�澯
	BOOL m_bPowerUpComplete;		//��ʼ���Ƿ����
	BOOL m_bStepMode;				//

	HWND m_hwndMainApp;				// �þ���ڲ�ʹ�ã������������巢��ϵͳ��Ϣ

	CEvt m_evtMcStop;				//
	CEvt m_evtEol;					//

	CArray<int, int> m_lstMsg;		//
};