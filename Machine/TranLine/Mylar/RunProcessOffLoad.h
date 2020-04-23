/****************************************************************************************
文件名：RunRunProcessOffLoad.h
功能描述：分料器基类
历史修改记录：
*****************************************************************************************/
#pragma once
#include "../../RunProcess.h"

class CRunProcessOffLoad : public CRunProcess
{
protected:
	enum POWER_UP_STEPS
	{
		PU_SET_STOP_STATUS,   //设置阻挡状态
		PU_FINISH,
	};

	enum AUTO_STEPS
	{
		AUTO_SEND_STAR_SIGNAL,			//开始工作
		AUTO_WAIT_PLATE_ARRIVE,         //等待料盘到达
		AUTO_WAIT_MATERIAL_FLOW,		//等待料盘流走
		AUTO_SEND_MATERIAL_READY_SIGNAL,//发送料盘准备好信号
		AUTO_WAIT_FEEDING_END_SIGNAL,   //等待下料完成信号
		AUTO_WAIT_FEEDING_END,          //等待放料完成
		AUTO_WORK_END,
	};

public:
	CRunProcessOffLoad(int nID);
	virtual ~CRunProcessOffLoad(void);

	BOOL InitializeConfig(CString strProcessModule);
	void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);
	BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove);
	BOOL CheckOutputCanActive(COutput *pOutput);

protected:
	void PowerUpOperation(void);
	void PowerUpRestart(void);
	void AutoOperation(void);
	BOOL InitIO();
	virtual BOOL SendRequiredSignal()=0;

	//阻挡推出
	BOOL DamperPush();
	//阻挡回退
	BOOL DamperReturn();
	//下料顶升气缸推出
	BOOL JackingPush();
	//下料顶升气缸回退
	BOOL JackingReturn();
	//判断是否有料盘
	BOOL CheckHasPlate();
	//判断是否有电芯
	BOOL CheckHasCore();
	//判断下一模组是否要料
	virtual BOOL CheckNextRequirePlate();
	//发送料盘准备好信号
	BOOL SendPlateReadySignal(BOOL bReady);
	//获取下料完成信号
	BOOL GetFeedingEndSignal();
	//机台是否要料
	virtual BOOL CheckPlatFormNeedPlate();
	
public:
	
public:
	void Destory();
public:
	//满料感应
    int m_ICheckLineFull;

protected:	

	int m_IDamperPush;				//X2011	下料阻挡气缸推出感应
	int m_IDamperReturn;			//X2012	下料阻挡气缸回退感应
	int m_IJackingPush;				//X2013	下料顶升气缸推出感应
	int m_IJackingReturn;			//X2014	下料顶升气缸回退感应
	int m_IHasPlate;				//X2015	下料顶升有料感应
	int m_ICheckCore;				//X2016	下料顶升料盘防呆检测
	int m_IHasCore;					//X2017	下料电芯有料感应
	int m_ODamperReturn;	        //Y2003	下料阻挡气缸回退电磁阀
	int m_OJackingPush;             //Y2004	下料顶升气缸推出电磁阀
	int m_OJackingReturn;			//Y2005	下料顶升气缸回退电磁阀

	int m_OReadPlateSignal;			//料盘准备好信号
	int m_IPlatformNeedPlate;        //机台要料信号 
	int m_IFeedingPlateSignal;      //放料完成信号
	
};