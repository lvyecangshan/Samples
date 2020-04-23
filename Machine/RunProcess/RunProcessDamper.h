/****************************************************************************************
文件名：RunProcessDamper.h
功能描述：阻挡器基类
历史修改记录：
*****************************************************************************************/
#pragma once
#include "..\RunProcess.h"
#include "../parts/Damper.h"
#include "..\parts\LineMotor.h"

class CRunProcessDamper : public CRunProcess
{
protected:
	enum POWER_UP_STEPS
	{
		PU_SET_STOP_STATUS,//设置阻挡状态
		PU_FINISH,
	};

	enum AUTO_STEPS
	{
		AUTO_SET_STOP_STATUS,   //设置阻挡
		AUTO_WAIT_PLATE_ARRIVE, //等待料盘到达
		AUTO_WAIT_FEED_END_SINGAL,//等待喂料完成信号
		ATUO_WAIT_NEXT_NEED_PLATE_SINGAL,//等待下一模组要料信号
		ATUO_WAIT_FOLLOW_PLATE,      //放料
		AUTO_WORK_END,
	};

public:

	virtual void OnPlateLeaved();
	
	CRunProcessDamper(int nID);
	virtual ~CRunProcessDamper(void);

	BOOL InitializeConfig(CString strProcessModule);
	void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);
	BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove);
	BOOL CheckOutputCanActive(COutput *pOutput);	

	virtual void MotorRun();

protected:
	void PowerUpOperation(void);
	void PowerUpRestart(void);
	void AutoOperation(void);
	

	virtual void AutoOperationFeed(void);
	               //是否要料
	
	
	//IO初始化
	virtual BOOL InitIO();
	
	
	
public:
	void Destory();

protected:
	CDamper			   m_Damper;
	CLineMotor m_Motor;

protected:
	
	BOOL               m_bNeedPlate; //需要料盘
	BOOL               m_bFeedPlate; //料盘准备好
	int                m_nNextAutoStepEx;  
	
};