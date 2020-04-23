/****************************************************************************************
功能描述：长边清洁
历史修改记录：
*****************************************************************************************/
#pragma once
#include "RunProcess.h"
#include "RunProcessRunProcessShortEdgeClean.h"
#include "..\parts\PressCoreCylinder.h"

//长边清洁
class CRunProcessRunProcessLongEdgeClean : public CRunProcessRunProcessShortEdgeClean
{
protected:
    enum POWER_UP_STEPS
    {
		PU_CHECK_FINGER_HAVE_CORE,
		PU_FINGER_CY_OPEN,
		PU_JACKING_CY_UP,
		PU_RODLESS_CY_RETURN,
		PU_CLEAN_CY_RETURN,
		PU_CLEAN_SIDESWAY_CY_RETURN,
		PU_FINISH,
    };

    enum AUTO_STEPS
    {
		AUTO_WAIT_BEGIN_WORK_EVEND,
		AUTO_CHECK_INDUCTOR,
		AUTO_JACKING_CY_DOWN,
		AUTO_FINGER_CY_CLOSE,
		AUTO_JACKING_CY_UP,
		AUTO_CHECK_HAVE_CORE,
		AUTO_RODLESS_CY_PUSH,
		AUTO_CLEAN_CY_PUSH,
		AUTO_JUDGE_FEEDER_READY,
		AUTO_CLEAN_CY_RETURN,
		AUTO_CLEAN_SIDESWAY_CY_PUSH,
		AUTO_CLEAN_SIDESWAY_CY_RETURN,
		AUTO_SEND_FEEDER_WORD_FINISH,
		AUTO_RODLESS_CY_RETURN,
		AUTO_JACKING_CY_REPEAT_DOWN,
		AUTO_FINGER_CY_OPEN,
		AUTO_SEND_CLEAN_FINISH_EVENT,
		AUTO_JACKING_CY_REPEAT_UP,
		AUTO_WORK_END,
    };
protected:
    void PowerUpOperation(void);
    void PowerUpRestart(void);
	void AutoOperation(void);
public:
    CRunProcessRunProcessLongEdgeClean(int nID);
    virtual  ~CRunProcessRunProcessLongEdgeClean();
public:
   
public:
    BOOL InitializeConfig(CString strProcessModule);
    BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove);
    BOOL CheckOutputCanActive(COutput *pOutput);

public:

	virtual void Anticollision();   //夹爪防撞线程 

	//初始化指针
	void SetCleanOnLoad(CRunProcess* CleanOnLoad);

private:

	CRunProcess* m_pCleanOnLoad;  //上料位

public:

	PressCoreCylinder m_CleanCylinder;   //清洁横移气缸

 };
 