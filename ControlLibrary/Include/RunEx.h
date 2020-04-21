/*******************************************************************
�ļ�����RunEx.h
���ߣ�              �汾��A1        ���ڣ�
��������������ƽ̨���л������չ��
��ʷ�޸ļ�¼��
*******************************************************************/
#pragma once
#include "run.h"
#include "EasyAutoControlExDef.h"
#include "DataBaseRecord.h"
using namespace std;
#include <map>

#define RUN_PROCESS_DLG_TIMER 110

class __declspec(dllexport) CRunEx : public CRun
{
protected:
	enum POWER_UP_STEPS
	{
		PU_FINISH,					//��ʼ���������
		NUM_OF_POWER_UP_STEPS,		//��ʼ����������
	};

	enum AUTO_STEPS
	{
		AUTO_WAIT_WORK_START,		//�ȴ���ʼ�����ź�����
		AUTO_WORK_END,				//�����������
	};

protected:
	/*********************************************************************************
	//�����������豸��ʼ�����̺���
	//�����������
	//��������: ��
	**********************************************************************************/
	virtual void PowerUpOperation(void);

	/*********************************************************************************
	//�����������Զ��������̺���
	//�����������
	//��������: ��
	**********************************************************************************/
	virtual void AutoOperation(void);

	/*********************************************************************************
	//����������������λ��Ӧ����,ͨ����������������ʼ������
	//�����������
	//��������: ��
	**********************************************************************************/
	virtual void PowerUpRestart(void);

public:
	CRunEx(int nID);
	virtual ~CRunEx(void);

	/*********************************************************************************
	//�������������������ļ�����ʼ������
	//���������strProcessModule		ģ������
	//��������: TRUE����ʼ���ɹ���FALSE����ʼ��ʧ��
	**********************************************************************************/
	virtual BOOL InitializeConfig(CString strProcessModule);

	/*********************************************************************************
	//�����������������õ�ini�ļ�
	//�����������
	//��������: TRUE������ɹ���FALSE������ʧ��
	**********************************************************************************/
	virtual BOOL SaveConfig(void);

	/*********************************************************************************
	//������������ȡӲ�����ã���������I/O�����I/O���ŷ����
	//���������pInputs		����I/O������ָ��
	//			pOutputs	���I/O������ָ��
	//			pMotors		�ŷ����������ָ��
	//��������: ��
	**********************************************************************************/
	virtual void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);

	/*********************************************************************************
	//��������������ŷ�����ܷ����
	//���������pMotor		ĳһ�ŷ������Ӧ��ָ��
	//��������: TRUE��ʾ���������FALSE��ʾ��ֹ����
	**********************************************************************************/
	virtual BOOL CheckMotorCanMove(CMotor *pMotor){ return TRUE; } 

	/*********************************************************************************
	//����������������I/O�ܷ����
	//���������pOutput		ĳһ���I/O��Ӧ��ָ��
	//��������: TRUE��ʾ���������FALSE��ʾ��ֹ����
	**********************************************************************************/
	virtual BOOL CheckOutputCanActive(COutput *pOutput){ return TRUE; }
	
 public:
	int m_nRunMode;				// ����ģʽ
	BOOL m_bDryRun;				//�����б�ʶ����

	CString m_strClass;			//������
	CString m_strProcess;		//ģ������

	void ShowMessageEX(CString strMsg, int nType = 0, CString strSn = _T("SN"), CDataBaseRecord *pDB = NULL);
	CString m_strAlarmMsg;

public:
	CString m_strMsgEng;		//Ӣ����ʾ��Ϣ
	CString m_strMsgChs;		//������ʾ��Ϣ
	BOOL m_bLangulageEng;		//�Ƿ�Ӣ������
	CString m_strMsgEx;

public:
	BOOL IsJamed(){return m_bJam;}				//�Ƿ����

   	virtual BOOL ReadParameter(){ return TRUE; }		//��ȡ��������
	virtual BOOL SaveParameter(){ return TRUE; }		//�����������
	virtual BOOL ReadRunParameter(){ return TRUE; }		//��ȡ���в�������

	map<int*,CString> m_InputMap;		//����I/Oӳ���ʹ��insert������������I/O
	map<int*,CString> m_OutputMap;		//���I/Oӳ���ʹ��insert������������I/O
	map<int*,CString> m_MotorMap;		//�ŷ����ӳ���ʹ��insert�������������ŷ����
	map<void*,CString> m_VoidMap;		//����ӳ���ʹ��insert���������������

	CRunEx *m_pFather;					//��ģ��ָ��
	CRunEx *m_pChild;					//��ģ��ָ��
	CRunEx *m_pBrother;					//�ֵ�ģ��ָ��

	CRunEx *m_pNormalFront;				//ǰһģ��ָ��
	CRunEx *m_pBranchFront;				//ǰһģ��ķ�֧ģ��ָ��
	CRunEx *m_pNormalNext;				//��һģ��ָ��
	CRunEx *m_pBranchNext;				//��һģ��ķ�֧ģ��ָ��

	CString m_strParent;				//��ģ������
	CString m_strChild;					//��ģ������
	CString m_strNormalFront;			//ǰһģ������
	CString m_strBranchFront;			//ǰһģ��ķ�֧ģ������
	CString m_strNormalNext;			//��һģ������
	CString m_strBranchNext;			//��һģ��ķ�֧ģ������

	BOOL m_bRequirePallet;				//�������̣�ԭ�ϣ��ź�
	BOOL m_bNextRequire;				//���͸���һ����λ�������ź�
	BOOL m_bForNextReady;				//���͸���һ����λ��׼�����ź�
	BOOL m_bBranchRequire;				//���͸���֧��λ�������ź�

	BOOL m_bNormalOpen;					//��֧���̴��ź�

	DWORD m_dwStartTime;				//ģ�鿪ʼʱ��

	//Ԥ��
	BOOL m_bChannelOpen;
	BOOL m_bBranchOpen;
	
	//Ԥ��
	BOOL m_bPalletFormNarmal;
	BOOL m_bPalletFormBranch;

	virtual BOOL ModifyPos(int nPkg, int nMotor, CMotorPos& mPos){return TRUE;}
	virtual void UpdateLanguage();
	BOOL IsSuspended(){return m_bSuspended;}
	BOOL IsRunning();

public:

	/*********************************************************************************
	//�������������������ᣨ������ƶ�
	//���������nProID	��Ʒ��ţ�һ��Ĭ��Ϊ GetCurPorductID(MACHINE.m_nDefaultProduct)
	//			nMotorID  �ᣨ������������
	//			nPos  λ���������
	//			dlOffset λ��ƫ�ƣ���λ������
	//��������: TRUE��ʾִ�гɹ���FALSE��ʾִ��ʧ��
	**********************************************************************************/
	BOOL MotorMove(int nProID, int nMotorID, int nPos, double dlOffset = 0.0);

	/*********************************************************************************
	//������������������ᣨ������ƶ�
	//���������nProID	��Ʒ��ţ�һ��Ĭ��Ϊ GetCurPorductID(MACHINE.m_nDefaultProduct)
	//			nMotorID  �ᣨ����������������
	//			nPos  λ�������������
	//			nNum  �������ᣨ���������
	//��������: TRUE��ʾִ�гɹ���FALSE��ʾִ��ʧ��
	**********************************************************************************/
	BOOL MotorsMove(int nProID, int* nMotorID, int* nPos, int nNum);

	/*********************************************************************************
	//������������������ᣨ������ƶ�
	//���������nProID	��Ʒ��ţ�һ��Ĭ��Ϊ GetCurPorductID(MACHINE.m_nDefaultProduct)
	//			nMotorID  �ᣨ����������������
	//			nPos  λ�������������
	//			dlOffset λ��ƫ�ƣ���λ������
	//			nNum  �������ᣨ���������
	//��������: TRUE��ʾִ�гɹ���FALSE��ʾִ��ʧ��
	**********************************************************************************/
	BOOL MotorsMove(int nProID,int* nMotorID, int* nPos, double* dlOffsetPos, int nNum);

	/*********************************************************************************
	//������������������ᣨ�������λ
	//���������nMotorID  �ᣨ����������������
	//			nNum  �������ᣨ���������
	//��������: TRUE��ʾִ�гɹ���FALSE��ʾִ��ʧ��
	**********************************************************************************/
	BOOL MotorsHome(int* nMotorID, int nNum);

	/*********************************************************************************
	//������������ȡ��Ӧ�ᣨ�����ָ��λ�������ľ���λ��ֵ
	//���������nProID	��Ʒ��ţ�һ��Ĭ��Ϊ GetCurPorductID(MACHINE.m_nDefaultProduct)
	//			nMotorID  �ᣨ����������������
	//			nPos  λ�������������
	//��������: ����λ��ֵ(������)
	**********************************************************************************/
	double GetMotorPos(int nProID, int nMotorID, int nPos);

	/*********************************************************************************
	//������������ȡָ�������Ų�Ʒ�ı��
	//���������nProductIndex	��Ʒ�����ţ�һ��ʹ��Ĭ�ϣ���������MACHINE.m_nDefaultProduct
	//��������: ��Ʒ���
	**********************************************************************************/
	int GetCurPorductID(int nProductIndex);

	double		m_dlModuleUseTime;				//ģ�鵱ǰ����ʱ��
	double		m_dlModuleTragetTime;			//ģ��Ŀ������ʱ��


	/////////////////// ����ʹ��Ƶ�ʾ��ϵ�
	virtual BOOL UserPremissionProtect(USER_PERMISSION_PROTECT_TYPE uprType = USER_PERMISSION_MOTOR_POS_SAVE);
	void SetNextStep(int nNextStep, CString strSn = _T("SN"), CDataBaseRecord *pDB = NULL, BOOL bWrite = FALSE);
	void ModuleStartTime();
	void ModuleEndTime(CString strSn = _T("SN"), CDataBaseRecord *pDB = NULL, BOOL bWrite = FALSE);
	void AddYieldRecord(int nType, CString strInfo, int nNum, CString strSn = _T("SN"), CDataBaseRecord *pDB = NULL, BOOL bWrite = FALSE);

	CString		m_strModuleStartTime;
	CString		m_strModuleEndTime;
	SYSTEMTIME	m_stModeleStartTime;
	SYSTEMTIME	m_stModeleEndTime;
	BOOL		m_bModuleTimeEnable;

	CString		m_strStepStartTime;
	CString		m_strStepEndTime;
	SYSTEMTIME	m_stStepStartTime;
	SYSTEMTIME	m_stStepEndTime;
	double		m_dlStepUseTime;
	BOOL		m_bStepTimeEnable;
	SYSTEMTIME	m_stCurTime;

	vector<CProduct> m_vecProduct;

	//���Ƽ�ʹ��
	BOOL Motors3Move(int nProID, int* nMotorID, int* nPos);

	//���Ƽ�ʹ��
	BOOL Motors2Move(int nProID, int* nMotorID, int* nPos);
};
