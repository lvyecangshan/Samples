/****************************************************************************************
����������������๩��
��ʷ�޸ļ�¼��
*****************************************************************************************/
#pragma once
#include "RunProcess.h"

//������๩��
class CRunProcessRunProcessLongEdgeFeeder:public CRunProcess
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

	enum MOTOR_R1
	{
		R1_WORK_POS,
	};

	enum MOTOR_R2
	{
		R2_WORK_POS,
	};

protected:
    void PowerUpOperation(void);
    void PowerUpRestart(void);
	void AutoOperation(void);
    BOOL CheckConfigValid(void);
public:
    CRunProcessRunProcessLongEdgeFeeder(int nID);
    virtual ~CRunProcessRunProcessLongEdgeFeeder();
public:
    BOOL InitializeConfig(CString strProcessModule);
    BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove);
    BOOL CheckOutputCanActive(COutput *pOutput);

	BOOL InitIO();

public:

	int m_nMotorR[2];   //R1R2

	int m_nIHaveMaterial[2]; //���ϸ�Ӧ
	int m_nINOMaterial[2];   //���ϸ�Ӧ

	int m_nOGushAlcohol[2];  //����ϵ�ŷ�
	int m_nOblow[2];         //������ŷ�

public:

	BOOL GushAlcoholCtrl(BOOL bOpen);   //�����紵�ƾ���ŷ�
	BOOL CheckHaveMaterial(); //���Ĳ� ��
	BOOL CheckNoMaterial(); //���Ĳ� ��

	BOOL MotorSvOn();    //�����ʹ��
	BOOL SetCurrentPos(double dPos);  //���õ�ǰ���λ��
	BOOL MotorsMoveToWorkLos();   //�ƶ��������������λ

 };
 