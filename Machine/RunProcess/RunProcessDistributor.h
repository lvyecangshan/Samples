/****************************************************************************************
文件名：RunRunProcessDistributor.h
功能描述：分料器基类
历史修改记录：
*****************************************************************************************/
#pragma once
#include "..\RunProcess.h"
#include "..\parts\Distributor.h"
#include "..\parts\LineMotor.h"



class CRunProcessDistributor : public CRunProcess
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
		AUTO_SEND_WAIT_PLATE,           //等待料盘到达
		AUTO_SEND_DISTRIBUTOR,           //分料
		AUTO_SEND_READY_SINGAL,         //发送准备好信号
		AUTO_WAIT_FEEDING_END,          //等待放料完成
		AUTO_WORK_END,
	};

public:
	CRunProcessDistributor(int nID);
	virtual ~CRunProcessDistributor(void);

	BOOL InitializeConfig(CString strProcessModule);
	void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);
	BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove);
	BOOL CheckOutputCanActive(COutput *pOutput);

	//线体是否料满CheckLineFull
	BOOL CheckLineFull();

protected:
	void PowerUpOperation(void);
	void PowerUpRestart(void);
	void AutoOperation(void);
	BOOL InitIO();	
	//判断是否有料
	virtual BOOL CheckHasPlate()=0;
	virtual BOOL CheckNoPlate()=0;
	//分料
	BOOL DistributorPlate();
	

public:
	void Destory();
public:
	/*
	int  m_IDistributionCylinderPush[2];		//X2006	 X2008  分料气缸1、2推出感应
	int  m_IDistributionCylinderReturn[2];       //X2007	 X2009  分料气缸1、2回退感应
	int  m_IHasPlate[2];                         //       X2010	分料气缸2有料感应
	int  m_ODistributionCylinder[2];             //Y2001  Y2002	分料气缸1、2回退电磁阀
	*/

	CDistributor  m_Distriutor;                 //分料器

	CLineMotor m_Motor;


	int  m_ICheckLineFull;                      //X2100	线体满料感应

	BOOL m_bFeedingMaterial;                    //放料

	
	
};