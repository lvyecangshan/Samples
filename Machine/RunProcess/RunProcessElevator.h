/****************************************************************************************
文件名：RunProcessElevator.h
功能描述：升降传送机
历史修改记录：
*****************************************************************************************/
#pragma once
#include "../RunProcess.h"
#include "../parts/Elevator.h"
#include "..\parts\LineMotor.h"

class CRunProcessElevator : public CRunProcess
{
protected:
	enum POWER_UP_STEPS
	{
		PU_WAIT_STAR_SIGNAL,				 //等待开始工作信号
		PU_STOP_BELT,						 //电机皮带转动
		PU_CHECK_SAFE,						 //检测是否安全
		PU_OPERATION_BEFORE_MOVE,            //电机移动前相关操作
		PU_TO_RECEIVE_MATERIAL_POSTION,      //移动电机到接料位
		PU_FINISH,							 //初始化结束
	};

	enum AUTO_STEPS
	{
		AUTO_CHECK_WORK_SAFE,						//检测安全
		AUTO_TO_RECEIVE_MATERIAL_POSTION,			//移动电机到接料位
		//AUTO_PREPARATIONS_BEFORE_RECEIVEMATERIAL,	//接料前准备工作
		AUTO_SEND_REQUIRE_SIGNAL,					//发送要料信号
		AUTO_START_RECEIVE_PLATE,					//开始接收料盘
		AUTO_SEND_RECEIVE_END,                      //接收完成
		AUTO_TO_FEEDING_POSITION,                   //移动到上料位
		AUTO_WAIT_FEEDING_SIGNAL,					//等待上料信号
		AUTO_FEEDING_MATERIAL,					    //上料
		AUTO_WAIT_FEEDING_END,                      //等待上料完成
		
		AUTO_RUNNING,
		AUTO_WORK_END,
	};

public:
	CRunProcessElevator(int nID);
	virtual ~CRunProcessElevator(void);

	BOOL InitializeConfig(CString strProcessModule);
	void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);
	BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove);
	BOOL CheckOutputCanActive(COutput *pOutput);

protected:
	void PowerUpOperation(void);
	void PowerUpRestart(void);
	void AutoOperation(void);
	//IO初始化
	virtual BOOL InitIO();
public:
	//移动到接料位
	virtual BOOL MoveToReceiveMaterialPostion();
	//移动到上料位
	virtual BOOL MoveToFeedingMaterialPostion();
	
	//获取当前升降机上料盘数量
	virtual int GetCurPlateCount();
	//判断是否可以下料
	virtual BOOL CheckCanFeedingMaterial();
	//判断下料是否完成
	virtual BOOL CheckFeedingMaterialEnd();	

	//启动上料电机
	virtual BOOL StartReceiveMotor();
	//停止上料电机
	virtual BOOL StopReceiveMotor();
	//启动下料电机
	virtual BOOL StartFeedingMotor();
	//停止下料电机
	virtual BOOL StopFeedingMotor();
	//止回气缸推出
	BOOL CylinderPush();
	//止回气缸回退
	BOOL CylinderReturn();

	virtual BOOL SendReceiveEnd();
	
	virtual void MotorRun();

public:
	void Destory();

protected:

	CLineMotor m_Motor;

	CElevator  m_Elevator;

	int m_IUpLineSafe;			 //X2005	线体料盘安全感应
	int m_IDownLineSafe;		 //X2024	回流线体料盘安全感应
	int m_ILiftingSafe;			 //X2004	升降料盘安全感应
	int m_ICheckCylinderPush;    //X2000	止回气缸推出感应
	int m_ICheckCylinderReturn;  //X2001	止回气缸回退感应
	int m_OCheckCylinder;        //Y2000	止回气缸推出电磁阀
	int m_IHasPlate1;            //X2002	升降料盘1到位感应
	int m_IHasPlate2;            //X2003	升降料盘2到位感应
	int m_OMotorForeward;        //Y2008	前升降交流调速电机正转启动（60W）
	int m_OMotorReverse;		 //Y2009	前升降交流调速电机反转启动
	int m_nMotorZ;               //入壳前升降Z轴（400W）M14

	int m_bPlateStatus[2];       //升降机上是否有料盘

	int m_bSendNeedCount;       //发送请求料盘次数

	

	
};