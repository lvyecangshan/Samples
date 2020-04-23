/****************************************************************************************
�����������̱���๩��
��ʷ�޸ļ�¼��
*****************************************************************************************/
#pragma once
#include "RunProcess.h"

//�̱���๩��
class CRunProcessRunProcessShortEdgeFeeder:public CRunProcess
{
protected:
    enum POWER_UP_STEPS
    {
		PU_CLOSE_GUSHALCOHOL,
        PU_CHECK_HAVE_MATERIAL,
		PU_MOTOR_ENABLE_ON,
		PU_MOTOR_POS_MAKE_ZERO,
        PU_FINISH,
    };
    enum AUTO_STEPS
    {
        AUTO_GUSHALCOHOL_OPEN,
		AUTO_MOTOR_MOVE_WORK_POS,
		AUTO_OTOR_POS_MAKE_ZERO,
		AUTO_GUSHALCOHOL_CLOSE,
		AUTO_SEND_BEGIN_WORK_EVENT,
		AUTO_CHECK_HAVE_MATERIAL1,
		AUTO_WAIT_WORK_FINISH,
		AUTO_CHECK_HAVE_MATERIAL2,
		AUTO_MANUAL_CHANGE_MATERIAL,
        AUTO_WORK_END,
    };

	enum MOTOR_R
	{
		R_WORK_POS,
	};

protected:
    void PowerUpOperation(void);
    void PowerUpRestart(void);
	void AutoOperation(void);
public:
    CRunProcessRunProcessShortEdgeFeeder(int nID);
    virtual ~CRunProcessRunProcessShortEdgeFeeder();
public:
    
	int m_nMotorR;

	int m_nIHaveMaterial; //���ϸ�Ӧ
	int m_nINOMaterial;   //���ϸ�Ӧ

	int m_nOGushAlcohol;  //����ϵ�ŷ�
	int m_nOblow;         //������ŷ�

public:
    BOOL InitializeConfig(CString strProcessModule);
    BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove);
    BOOL CheckOutputCanActive(COutput *pOutput);
	BOOL InitIO();

	//��ʼ��ָ��
	void SetCleanStation(CRunProcess* CleanStation);

public:

public:

	BOOL GushAlcoholCtrl(BOOL bOpen);   //�����紵�ƾ���ŷ�
	BOOL CheckHaveNOMaterial(BOOL bHaveNo); //���Ĳ��Ƿ�����

private:

	CRunProcess* m_pCleanStation;   //��๤λ

 };
 