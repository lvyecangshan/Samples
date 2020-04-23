#pragma once
#include <RunEx.h>
#include <easyautocontrolexdef.h>
#include "Pallet.h"
//#include "StorageFile.h"

#include "ReportFile.h"  //��������ͳ��
#define CT_WAITSTEP_TOTAL 20 //��ģ���� �ȴ���case����, 
#define STEP_TOTAL 255       //��ģ���� ����case����    

class CRunProcess : public CRunEx
{

public:
	//������������I/O
	//���������I/O����
	//��������: TRUE��ʾ�򿪳ɹ���FALSE��ʾ��ʧ��
	BOOL APP_OUTPUTS_On(int index);
	//===================================================
	//�����������չ�I/O
	//���������I/O����
	//��������: TRUE��ʾ�򿪳ɹ���FALSE��ʾ��ʧ��
	BOOL APP_OUTPUTS_Off(int index);
	//===================================================
	//�����������ж����I/O�Ƿ��
	//���������I/O����
	//��������: TRUE��ʾ�򿪳ɹ���FALSE��ʾ��ʧ��
	BOOL APP_OUTPUTS_IsOn(int index);
	//===================================================
	//�����������ж����I/O�Ƿ�ر�
	//���������I/O����
	//��������: TRUE��ʾ�򿪳ɹ���FALSE��ʾ��ʧ��
	BOOL APP_OUTPUTS_IsOff(int index);
	//===================================================
	//�����������ж�����I/O�Ƿ��
	//���������I/O����
	//��������: TRUE��ʾ�򿪣�FALSE��ʾ�ر�
	BOOL APP_INPUTS_IsOn(int index);
	//===================================================
	//�����������ж�����I/O�Ƿ�ر�
	//���������I/O����
	//��������: TRUE��ʾ�رգ�FALSE��ʾ��
	BOOL APP_INPUTS_IsOff(int index);
	//===================================================	
	//�������������������ᣨ������ƶ�
	//���������nProID ��Ʒ��ţ�һ��Ĭ��Ϊ GetCurPorductID(MACHINE.m_nDefaultProduct)
	//                  nMotorID  �ᣨ������������
	//                  nPos  λ���������
	//                  dlOffset λ��ƫ�ƣ���λ������
	//��������: TRUE��ʾִ�гɹ���FALSE��ʾִ��ʧ��
	BOOL APP_MotorMove( int nMotorID, int nPos, double dlOffset = 0.0);
	//===================================================
	//�����������������I/O��״̬
	// ע�⣺��鵥����㱨���ã������ô˺�����ѭ��״̬�жϣ�����ѭ���жϿ�ֱ����Input����ĺ���������ÿ�ε��û������Ӧ��־��¼
	//���������pInput  ����I/Oָ��
	//  bState  ����I/OӦ��״̬��һ��ʹ��ON��OFF��ʾ��������TRUE��FALSE
	// nMsgID  �粻����Ҫ��bState״̬ʱ����쳣��Ϣ(ͬShowMessage())���� δָ������Ĭ����Ϣ����. 
	//��������: TRUE���������bState��ͬ  FALSE���������bState��ͬ
	BOOL APP_CheckInput(int index, BOOL bState, int nMsgID = MSG_SHOW_DEFAULT);
	//===================================================
	//�����������ȴ�����I/O��״̬�û���ָ��״̬
	//���������pInput  ����I/Oָ��
	// bState  ����I/OӦ��״̬��һ��ʹ��ON��OFF��ʾ��������TRUE��FALSE
	// nMsgID  �粻����Ҫ��bState״̬ʱ����쳣��Ϣ(ͬShowMessage())���� δָ������Ĭ����Ϣ������û��˲���������Ϣ����
	//   MSG_SHOW_DEFAULT ΪĬ�ϣ�MSG_NOT_NEED Ϊ����Ҫ����������Ϣ�����������MSG_NOT_NEEDֵΪ�Զ�����ϢID,�Զ�����ϢID��Ҫע�⣬����Ҫ����easylibdef.h�ļ���LIBRARY_MSG_ID�������ֵ
	//��������: WAIT_OK�� �������bState��ͬ
	// WAIT_ERROR����ʱ(ʱ���趨����Input.cfg)
	// WAIT_STOP�� ��ֹͣ
	int  APP_WaitInput(int index, BOOL bState, int nMsgID = MSG_NOT_NEED);
	//===================================================
	//��������������һ��I/O����
	//���������pVaule  I/O����
	//                 strKey  I/O�������ļ��ж�Ӧ������
	//��������: TRUE��ʾ��ӳɹ���FALSE��ʾ���ʧ��
	void APP_Add_Input(int* pVaule,CString strKey);
	//===================================================
	//��������������һ�����
	//���������pVaule �������
	//                  strKey  ����������ļ��ж�Ӧ������
	//��������: TRUE��ʾ��ӳɹ���FALSE��ʾ���ʧ��
	void APP_Add_Motor(int* pVaule,CString strKey);
	//===================================================
	//��������������һ��I/O���
	//���������pVaule  I/O����
	//                strKey  I/O�������ļ��ж�Ӧ������
	//��������: TRUE��ʾ��ӳɹ���FALSE��ʾ���ʧ��
	void APP_Add_Output(int* pVaule,CString strKey);
	//===================================================
	void APP_Add_Map(map<int*,CString> &Map,int* pVaule,CString strKey);
	//===================================================
	void APP_Add_Map(map<void*,CString> &Map,int* pVaule,CString strKey);
	//===================================================
	void APP_Add_Map(map<void*,CString> &Map,CString* pVaule,CString strKey);
	//===================================================
	//�����е��ü�צ��������ʱ
	void APP_Finger_Sleep();
	//===================================================
	//��Ӳ������ʱ��ʱ
	void APP_NoHardware_Sleep();
	//===================================================
	//�ȴ��ź���ʱ
	void APP_Wait_Signal_Sleep();	
	//===================================================
	void APP_MessageBox(CString strMsg);
	//===================================================
	//������������Ϣ��ʾ��
	//��������� strMsg Ҫ��ʾ����Ϣ
	//           nType  ��ʾͼ������
	//��������: ��
	//��������: IDABORT		��ֹ��ť
	//			IDCANCEL	�˳���ť
	//			IDIGNORE	���԰�ť
	//			IDNO		NO��ť
	//			IDOK		OK��ť
	//			IDRETRY		���԰�ť
	//			IDYES		YES��ť
	int APP_BLMessageBox(LPCTSTR lpszMessage, UINT nType = MB_OK | MB_ICONEXCLAMATION);
	//===================================================
	//���������������߳���Ϣ��ʾ��
	//��������� strMsg Ҫ��ʾ����Ϣ
	//           nType		��ʾ���� 0����ͨ��ʾ����ɫ�� 1���ʴ���ʾ����ɫ�� 2����ͨԤ������ɫ�� 3�����ϱ�������ɫ��
	//��������: ��
	//���ô˺���ʱ��������ʾ��ǰ���Զ�����ģ�����ƣ�strMsg���治Ҫ�ټ�ģ������
	void APP_ShowMessage(LPCTSTR lpszMessage, UINT nType = MB_OK | MB_ICONEXCLAMATION);

public:
	virtual BOOL SetNeedSingal(BOOL bNeedPlate);
	virtual BOOL GetNeedSingal(); 
	virtual BOOL SetFeedSingal(BOOL bFeedPlate);
	virtual BOOL GetFeedSingal();                 //��ȡ��������ź�
	virtual BOOL GetNextNeedSingal();              //��ȡ��һģ��Ҫ���ź�

	//��������ͣ
	virtual void MotorRun(void){return;}   //chen_2019.7.16

	//��е�ּ�צ��ײ
	virtual void Anticollision(void){return;}   //chen_2019.7.17

protected:
	BOOL InitIO();

protected:
	BOOL               m_bNeedPlate; //��Ҫ����
	BOOL               m_bFeedPlate; //����׼����
	int                m_nNextAutoStepEx;  
	BOOL               m_bShowWaitFeedSingal;


protected:
	enum POWER_UP_STEPS
	{
		PU_FINISH,
		NUM_OF_POWER_UP_STEPS,
	};

	enum AUTO_STEPS
	{
		AUTO_WAIT_WORK_START,
		AUTO_WORK_END,
	};


protected:
	virtual void PowerUpOperation(void);

	virtual void PowerUpRestart(void);

	virtual void AutoOperation(void);

protected:

public:
	CRunProcess(int nID);

	virtual ~CRunProcess(void);

	// load from ini file the configurations
	virtual BOOL InitializeConfig(CString strProcessModule);

	// show the tools dialog, return true means need to update display
	virtual BOOL ShowToolsDlg(void);

	virtual BOOL MotorHandMove( CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove );

	// check motor can move
	virtual BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove); 

	// check output can active
	virtual BOOL CheckOutputCanActive(COutput *pOutput);

	// load package data
	virtual BOOL LoadPkgParam(CString strPathName) { return TRUE; }

	// save package data
	virtual BOOL SavePkgParam(CString strPathName) { return TRUE; }

	// get the hardwares
	virtual void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);

	virtual BOOL UserPremissionProtect(USER_PERMISSION_PROTECT_TYPE uprType = USER_PERMISSION_MOTOR_POS_SAVE);

	BOOL ModifyPos(int nPkg,int nMotor,CMotorPos &mPos);
public:

	char* CStringTochar(CString &strResource);

	BOOL CreateFilePathDirectory(CString strFilePath);			//����ָ���ļ���

	BOOL SetParameterEnable(CString strParameter, int nParamLevel);

	virtual BOOL ReadParameter();

	virtual BOOL SaveParameter();

	virtual BOOL ReadRunParameter();

	virtual BOOL SaveStepInfo(int nWriteType);

	virtual void ReadStepInfo();

	virtual void CloseAction();

	virtual void StationEStop();

	virtual BOOL CheckInOutputStatus(BOOL bInput, int nInOutput,BOOL bStatus, BOOL bAlarm = FALSE);

	void MeasureCT(CString strModuleName, CString strStepName, int nStepNum, int nStartStep, int nEndStep, int* waitStep);
    void WriteLog(CString strText);

	public:
		void WriteCTLog(CString strText); //����дCT��־�ĺ���
		int m_nSaveLastStep;   //���ڼ�¼��һ�εĲ���
		DWORD m_nStepStartTime;   //��¼ÿһ���Ŀ�ʼʱ��
		DWORD m_nTotalTime;   //��ʱ��

public:
	CString m_strModuleName;
	BOOL m_bModuleResetFinish;
	int m_nGroupID;	// ͬ�಻ͬģ�����
	BOOL m_bShellVaild;
    BOOL m_bReCoverMachineAuto; //�Զ����̴���  TRUE �������п�ʼ FALSE �������н���
	
	//CT	
	CTFunction m_CTFunc;
	int m_waitStep[CT_WAITSTEP_TOTAL];
	actionClass m_action[STEP_TOTAL];

	//CT	
	DWORD m_dwModuleCT;
	DWORD m_dwModuleStrt;
	DWORD m_dwModuleStepCT;	
	unsigned int m_nAutoNext;
	CString m_strModuleCTtemp;
	
public:
	CPallet m_Pallet;

	vector<PARAMETER_EN> m_vecParameterEN;
	CCriticalSection m_ParameterLock;
	CCriticalSection m_SingleLock;
};