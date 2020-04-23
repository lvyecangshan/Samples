/****************************************************************************************
文件名：RunprocessRunprocessShortEdgeclean.h
作者：chenshengjin					版本：A1						日期：2019/7/12
功能描述: 顶盖长边清洁
历史修改记录：
*****************************************************************************************/
#pragma once
#include "RunProcess.h"
#include "..\parts\PressCoreCylinder.h"
#include "..\parts\Finger.h"
#include "..\parts\SCtrolCylinder.h"
#include "..\parts\Cylinder.h"

//顶盖长边清洁
class CRunProcessRunProcessShortEdgeClean : public CRunProcess
{
protected:
    enum POWER_UP_STEPS
    {
		PU_CHECK_FINGER_HAVE_CORE,
		PU_FINGER_CY_OPEN,
		PU_JACKING_CY_UP,
		PU_RODLESS_CY_RETURN,
		PU_CLEAN_CY_RETURN,
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
    BOOL CheckConfigValid(void);
public:
    CRunProcessRunProcessShortEdgeClean(int nID);
    virtual ~CRunProcessRunProcessShortEdgeClean();
public:
    BOOL InitializeConfig(CString strProcessModule);
    BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove);
    BOOL CheckOutputCanActive(COutput *pOutput);

	virtual void Anticollision();   //夹爪防撞线程 

	void Release();

	//初始化指针
	void SetCleanOnLoad(CRunProcess* CleanOnLoad); 

public: 

	PressCoreCylinder     m_PressCoreCy;  //压断气缸
	CFinger               m_Finger;     //夹爪
	CCylinder             m_JackingCylinder;   //顶升气缸  
	SCtrolCylinder        m_RodlessCylinder;  //无杆气缸
	PressCoreCylinder    m_CleanPush;    //清洗推出

private:

	CRunProcess* m_pCleanOnLoad;  //上料位

 };
 