/****************************************************************************************
文件名：RunProcessOnloadShellStackEx.h
功能描述：上外壳堆栈模组
历史修改记录：
*****************************************************************************************/
#pragma once
#include "RunProcess.h"

class CRunProcessOnloadShellStack : public CRunProcess
{
protected:
	enum POWER_UP_STEPS
	{
		PU_RELASE_CAR_LOCK_CYL_OPEN,
		PU_CHECK_PALLET_DATA,
		PU_SAFE_CYL_CLOSE,
		PU_Z_MOTOR_MOVE_HOME,
		PU_CHECK_HAVE_PALLET,
		PU_Z_MOTOR_MOVE_WAIT_POS,
		PU_FINISH,
	};

	enum AUTO_STEPS
	{
		AUTO_SAFE_CYL_OPEN,
		AUTO_WAIT_PEOPLE_ONLOAD,
		AUTO_CHECK_ONLOAD,
		AUTO_LOCK_CYL_LOCK,
		AUTO_MOVE_Z_MOTOR_TO_RELASE_CAR_LOC,
		AUTO_CHECK_LIMIT_CORE,
		AUTO_LOCK_CYL_LOCK_ONE,
		AUTO_WAIT_PEOPLE_CHECK_SAFE,
		AUTO_WAIT_START_EVENT,
		AUTO_MOVE_Z_MOTOR_POS,
		AUTO_MOTOR_Z_ABSNORMAL_DOWN_TO_DISTANCE,
		AUTO_MOTOR_Z_DOWN_TO_SAFE_DISTANCE,
		AUTO_MOVE_Z_MOVE_TO_ONLOAD_POS,
		AUTO_CAN_MOVE_SHORT_POS,
		AUTO_WAIT_PICK_PALLET_FINISH,
		AUTO_MOVE_Z_MOTOR_TO_SAFE_POS,
		AUTO_JUDGE_STACK_EMPTY,
		AUT0_CHECK_MOVE_SAFE,
		AUTO_MOVE_Z_MOTOR_WAIT_POS,
		AUTO_WORK_END,
	};

	enum MOTOR_TRYE
	{
		ONLOAD1_MOTOR,
		ONLOAD2_MOTOR,
		OFFLOAD3_MOTOR,
		NUM_MOTORS
	};

	enum MOTOR_WORK_LOC
	{
		STACK_MOTOR_WAIT_LOC = 0,//等待位
		STACK_MOTOR_RELASE_CAR_LOC,//小车分离位
		STACK_MOTOR_SEND_PALLET_LOC,//上料盘位
	};

public:
	CRunProcessOnloadShellStack(int nID);
	virtual ~CRunProcessOnloadShellStack(void);

protected:
	void PowerUpOperation(void);
	void PowerUpRestart(void);
	void AutoOperation(void);

public:
	BOOL InitializeConfig(CString strProcessModule);
	void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);
	BOOL ShowToolsDlg();
	BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove); 
	BOOL CheckOutputCanActive(COutput *pOutput);
	BOOL SaveParameter();
	BOOL ReadRunParameter();
	BOOL MonitorOut();
	
public:
	BOOL CheckCarInPos(void);//检测小车到位
	BOOL CheckHaveNoCar(void);// 检测没有小车
	BOOL CheckHavePallet(BOOL bHas = TRUE);//检测料盘
	BOOL CheckCarAndPallet(BOOL bHas = TRUE);//检测料盘和车	
	BOOL MotorHandMove( CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove );
	BOOL LockCar(BOOL bLock);//锁车气缸动作
	BOOL ModuleIsSafe();
	BOOL MotorZMoveToOnload(double dSpeedRatio);

public:	
	int  m_IButtonReady;//料盘准备好按钮
	int  m_OReadyLight;//按钮灯

	int  m_ICarInPosFront;//小车到位前感应
	int  m_ICarInPosBehind;//小车到位后感应
	
	//锁车气缸
	int  m_ILockCylPushOut;
	int  m_ILockCylBack;
	int  m_OLockCyl;

	int m_IPalletMoveInPos;		// 料盘移动到位
	int m_IHavePallet;// 堆栈有无料盘检测
	int m_ITrayTopPresent;//限位感应

	int  m_nMotorZ; //上料升降马达	
	
public:	
	BOOL m_bMotorSafePos;//电机到安全位
	BOOL m_bHavePallet;//有料信号
	BOOL m_bSendPalletFinish;//送料完成信号
	BOOL m_bHomeMoveSafePos;//复位到安全位
	double m_dMotorPos;//安全位置
	double m_dlZSafeLimit;//下降距离
};