/****************************************************************************************
功能描述：短边清洁供料
历史修改记录：
*****************************************************************************************/
#pragma once
#include "RunProcess.h"

//短边清洁供料
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

	int m_nIHaveMaterial; //有料感应
	int m_nINOMaterial;   //无料感应

	int m_nOGushAlcohol;  //喷酒料电磁阀
	int m_nOblow;         //吹气电磁阀

public:
    BOOL InitializeConfig(CString strProcessModule);
    BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove);
    BOOL CheckOutputCanActive(COutput *pOutput);
	BOOL InitIO();

	//初始化指针
	void SetCleanStation(CRunProcess* CleanStation);

public:

public:

	BOOL GushAlcoholCtrl(BOOL bOpen);   //控制喷吹酒精电磁阀
	BOOL CheckHaveNOMaterial(BOOL bHaveNo); //检测纺布是否用完

private:

	CRunProcess* m_pCleanStation;   //清洁工位

 };
 