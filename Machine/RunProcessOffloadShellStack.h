/****************************************************************************************
文件名：RunProcessOffloadShellStackEx.h
功能描述：下外壳堆栈模组
历史修改记录：

问题：
1. 电机位置数量
*****************************************************************************************/
#pragma once
#include "RunProcess.h"

class CRunProcessOffloadShellStack : public CRunProcess
{
protected:
	enum POWER_UP_STEPS
	{
		PU_LOCK_CAR_OPEN,				//锁车气缸打开
		PU_CHECK_HAVE_CAR,				//检测是否无车
		PU_WAIT_PEOPLE_CHECK_SAFE,
		PU_SAFE_CLY_CLOSE,				//安全气缸关闭
		PU_Z_MOTOR_MOVE_HOME,			//Z轴复位
		PU_CHECK_HAVE_PALLAT,
		PU_MOVE_Z_MTOR_TO_SAFE_POS,
		PU_MOTOR_DOWN_SAFE_DISTANCE,
		PU_CHECK_PALLET_FULL,
		
		PU_FINISH,
		NUM_OF_POWER_UP_STEPS,
	};

	enum AUTO_STEPS
	{
		AUTO_CHECK_CAR_AND_PALLET,
		AUTO_MOVE_Z_MOTOR_TO_SAFE_POS,
		AUTO_MOTOR_Z_DOWN_TO_SAFE_DISTANCE,
		AUTO_CHECK_PALLET_IS_FULL,
		AUTO_WAIT_START_EVENT,
		AUTO_JUDGE_CAR_MOVE,
		AUTO_MOVE_Z_MOTOR_TO_WORK_POS,
		AUTO_WAIT_PUT_PALLET_FINISH,
		AUTO_CHECK_CAR_REMOVE,
		AUTO_MOVE_Z_MOTOR_TO_SAFE_POS_AFTER_PICK_PALLET,
		AUTO_JUDGE_PALLET_FULL,
		AUTO_CHECK_CAR_AND_PALLET_AGAIN,
		AUTO_MOTOR_OFFLOAD_MOVE_TO_SAFE_POS,
		AUTO_SET_OFFLOAD_PALLET_SINGLE,
		AUTO_WAIT_LOCK_CYL_LOCK,
		AUTO_WAIT_PEOPLE_MOVE_CAR_IN,
		AUTO_CHECK_HAVE_CAR,
		AUTO_LOC_CYL_LOCATION_LOCK,
		AUTO_MOVE_Z_MOTOR_TO_RELASE_POS,
		AUTO_LOCK_CYL_OPEN_ON_REMOVE_CAR,
		AUTO_REMOVE_CAR_AFTER_OFFLOAD,
		AUTO_CHECK_HAVE_NO_PALLET,
		AUTO_SAFE_LOCK_CYL_CLOSE,
        AUTO_WORK_END,
	};

	enum MOTOR_WORK_LOC
	{
		STACK_MOTOR_SAFE_POS = 0,//安全位
		STACK_MOTOR_RELASE_CAR_LOC,//分离位
		STACK_MOTOR_RECV_PALLET,	// 接料位(以空料盘为位置)
		NUM_WORK_LOC
	};

public:
	CRunProcessOffloadShellStack(int nID);
	virtual ~CRunProcessOffloadShellStack(void);

protected:
	void PowerUpOperation(void);
	void PowerUpRestart(void);
	void AutoOperation(void);

public:
	BOOL InitializeConfig(CString strProcessModule);
	void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);
	BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove); 
	BOOL MotorHandMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove); 
	BOOL CheckOutputCanActive(COutput *pOutput);
	BOOL SaveParameter();
	BOOL ReadRunParameter();

public:
	BOOL MonitorOut();
	BOOL CheckCarInPos(BOOL bHave);//检查小车到位
	BOOL CheckHaveNoPallet(void);//判断没有料盘
	BOOL CheckCarAndPallet(BOOL bHas = TRUE);//检查是否有料盘或车
	BOOL CheckHaveCar(BOOL bHas = TRUE);//检查是否有车
	BOOL CheckHavePaller(BOOL bHas = TRUE);//检测料盘
	BOOL LockCar(BOOL bLock);//锁车气缸动作函数
	BOOL MotorMoveCheckFullPallet(void);//移动电机检查料盘料满位函数
	BOOL ModuleIsSafe();//检查模组是否安全

public:
	int  m_IButtonReady;//料盘准备好按钮
	int  m_OReadyLight;//按钮灯
	int  m_ICarInPosFront;//小车到位前感应
	int  m_ICarInPosBehind;//小车到位后感应

	//锁车气缸
	int  m_ILockCylPushOut;
	int  m_ILockCylBack;
	int  m_OLockCyl;

	int m_IHavePallet;// 堆栈有无料盘检测
	int m_ITrayTopPresent;//限位感应
	int m_IEmptyCarCheck;		// 空车感应(用于下料时小车推进时检测车上是否有料盘)

	int  m_nMotorZ; //Z轴电机

public:	
	BOOL m_bOffloadEmptyPallet;
	BOOL m_bMotorSafePos;//电机移动到安全位标志
	BOOL m_bMoveMotorFinish;//电机移动完成
	BOOL m_bHomeMoveSafePos;//电机复位完成
	double m_dMotorDownPos;//电机下降距离
	double m_dMotorUpPos;	// 电机上升距离
	double m_dPalletHigh;	// 单个料盘高度
	double m_dlZSafeLimit;//安全限位设置
};