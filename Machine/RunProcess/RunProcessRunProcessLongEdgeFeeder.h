/****************************************************************************************
功能描述：长边清洁供料
历史修改记录：
*****************************************************************************************/
#pragma once
#include "RunProcess.h"

//长边清洁供料
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

	int m_nIHaveMaterial[2]; //有料感应
	int m_nINOMaterial[2];   //无料感应

	int m_nOGushAlcohol[2];  //喷酒料电磁阀
	int m_nOblow[2];         //吹气电磁阀

public:

	BOOL GushAlcoholCtrl(BOOL bOpen);   //控制喷吹酒精电磁阀
	BOOL CheckHaveMaterial(); //检测纺布 有
	BOOL CheckNoMaterial(); //检测纺布 无

	BOOL MotorSvOn();    //电机上使能
	BOOL SetCurrentPos(double dPos);  //设置当前电机位置
	BOOL MotorsMoveToWorkLos();   //移动两个电机到工作位

 };
 