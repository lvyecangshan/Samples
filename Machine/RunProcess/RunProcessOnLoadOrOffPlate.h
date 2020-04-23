/****************************************************************************************
文件名：RunProcessOnLoadOrOffPlate.h
功能描述：上下料工位
历史修改记录：
*****************************************************************************************/
#pragma once
#include "../RunProcess.h"
#include "../parts/OnOrOffLoadPlate.h"
#include "..\parts\LineMotor.h"

class CRunProcessOnLoadOrOffPlate : public CRunProcess
{
protected:
	enum POWER_UP_STEPS
	{
		PU_WAIT_RESET,				 //等待开始工作信号		
		PU_FINISH,					 //初始化结束
	};

	enum AUTO_STEPS
	{
		AUTO_WORK_STAR,				//开始工作	
		AUTO_WAIT_PLATE_ARRIVED,    //等待料盘到达
		AUTO_JACKING_PLATE,         //顶升料盘
		AUTO_WAIT_FEED_END_SINGAL,  //等待取料完成信号
		ATUO_WAIT_NEXT_NEED_PLATE_SINGAL,//等待下一模组要空盘信号
		ATUO_WAIT_FOLLOW_PLATE,      //等待料盘流走
		AUTO_CHECK_HAS_CORE,        //判断是否有电芯
		AUTO_CHECK_NO_CORE,        //判断无电芯
		AUTO_WORK_END,              //结束工作
	};


public:
	CRunProcessOnLoadOrOffPlate(int nID);
	virtual ~CRunProcessOnLoadOrOffPlate(void);
	//设置上料模式
	void SetOnLoadMode();
	//是否是上料模式
	BOOL IsOnLoadMode();
	//设置下料模式
	void SetOffLoadMode();
	//是否是下料模式
	BOOL IsOffLoadMode();
	BOOL HasCore();
public:
	
	virtual void MotorRun();

protected:
	BOOL InitializeConfig(CString strProcessModule);
	void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);
	BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove);
	BOOL CheckOutputCanActive(COutput *pOutput);
	virtual void  OnPlateLeaved();

protected:
	void PowerUpOperation(void);
	void PowerUpOperationOnLoad(void);
	void PowerUpOperationOffLoad(void);
	void PowerUpRestart(void);
	void AutoOperation(void);
	void AutoOperationOnLoad(void);
	void AutoOperationOffLoad(void);
	virtual void AutoOperationFeed();//上料（不需要考虑其他模组）
	//IO初始化
	virtual BOOL InitIO();
	BOOL m_bOnLoadMode;  //是否是上料模式
protected:

	CLineMotor m_Motor;

	COnOrOffLoadPlate  m_Plate;	
	BOOL               m_bNeedPlate; //需要料盘
	BOOL               m_bFeedPlate; //料盘准备好
	BOOL               m_bFeedEnd;   //喂料完成（上下料完成）

};