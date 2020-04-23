/****************************************************************************************
文件名：RunProcessOnloadShellPNP.cpp
功能描述：上壳机械手
历史修改记录：
*****************************************************************************************/
#pragma once
#include "RunProcess.h"

class CRunProcessOnloadShellPNP : public CRunProcess
{
protected:
	enum POWER_UP_STEPS
	{
		PU_CHECK_FINGER_STATUS, 
		PU_MOVE_MOTOR_Z_HOME, 
		PU_ROTATE_CYL_HOME, 
		PU_CLAMP_CYL_OPEN, 		
		PU_MOVE_MOTOR_Z_TO_SAFE_POS, 
		PU_MOTOR_Y_HOME, 

		PU_JUDGE_IF_WITH_PALLET,//判断带料启动

		PU_MOTOR_Y_MOVE_PICK_POS, 
		PU_FINISH, 
	};

	enum AUTO_STEPS
	{
		AUTO_SEND_FRONT_MODULE_MOVE_MOTOR, 
		AUTO_MOVE_Y_MOTOR_TO_ONLOAD_POS, 
		AUTO_CHECK_ROATE_BACK,
		AUTO_CHECK_PUSH_PUSH,
	    AUTO_WAIT_FRONT_MODULE_MOVE_MOTOR_FINISH, 
		AUTO_MOTOR_Z_MOVE_TO_WAIT_POS,
		AUTO_WAIT_FRONT_MODULE_MOVE_MOTOR_FINISH_AGAIN,

        AUTO_MOVE_MOTOR_Z_MOTOR_TO_PICK_POS,
		AUTO_MOVE_MOTOR_Z_TO_SAFE_POS_ONLOAD, 
		AUTO_CHECK_ALL_VACU_ONLOAD,
		AUTO_MOVE_Y_MOTOR_TO_OFFLOAD_POS, 
		AUTO_CHECK_ALL_VACU_OFFLOAD, 
		AUTO_PUSH_CYL_BACK_TO_OFFLOAD,
		AUTO_ROATE_CYL_RAOTE, 
		AUTO_WAIT_START_EVENT, 
		AUTO_MOVE_Z_TO_WAIT_PUT_POS,
		AUTO_WAIT_START_EVENTEX,
		AUTO_MOVE_Z_MTOR_TO_PUT_POS, 
		AUTO_CLAMP_CYL_OPEN, 
		AUTO_CHECK_DIRECTION,
		AUTO_CHANGE_DIRECTION,
		AUTO_MOVE_Z_MOTOR_SAFE_POS_OFFLOAD,  
		AUTO_SEND_OFFLOAD_FINISH,
		AUTO_ROATE_CYL_RETURN,
		 
		
	    AUTO_WORK_END, 
	};

  enum MOTOR_LOCS
  {
	  Y_MOTOR_PLACE_LOC, // 放料位
	  Y_MOTOR_PICK_LOC1, // Y轴取料位1
	  Y_MOTOR_PICK_LOC2,
 	  Y_MOTOR_PICK_LOC3,
	  Y_MOTOR_PICK_LOC4,
	  Y_MOTOR_PICK_LOC5,
	  Y_MOTOR_PICK_LOC6,
	  Y_MOTOR_PICK_LOC7,
	  Y_MOTOR_PICK_LOC8, 
	  Y_MOTOR_PICK_LOC9,
	  Y_MOTOR_PICK_LOC10,
	  Y_MOTOR_PICK_LOC11,
	  Y_MOTOR_PICK_LOC12,
	 
	  Z_MOTOR_SAFE_POS = 0, //安全位
	  Z_MOTOR_PICK_WAIT_POS,
	  Z_MOTOR_PICK_POS, //取料位
	  Z_MOTOR_WAIT_PLACE_POS, // 放料等待位
	  Z_MOTOR_PLACE_POS, //放料位
	  Z_MOTOR_CHANGE_DIRECTION,//铝壳朝口旋转位

	  R_MOTOR_PICK_POS = 0, //接料位
	  R_MOTOR_ROTATE_NORMAL, //正常旋转
	  R_MOTOR_ROTATE_REVERSE,//铝壳反向旋转
};

public:
	CRunProcessOnloadShellPNP(int nID);
	virtual ~CRunProcessOnloadShellPNP(void);

public:
	BOOL InitializeConfig(CString strProcessModule);
	void GetHardwareConfig(CArray<int, int> *pInputs, CArray<int, int> *pOutputs, CArray<int, int> *pMotors);
	BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove);
	BOOL CheckOutputCanActive(COutput *pOutput);

protected:
	void PowerUpOperation(void);
	void PowerUpRestart(void);
	void AutoOperation(void);

public:
	BOOL CloseVacu();//关闭真空
	BOOL CheckVacuStatus(BOOL bOpen);
	BOOL TurnOut(int nIndex, BOOL bOut =TRUE);//旋转气缸动作
	BOOL PushCylPush(BOOL bPush);//搬运气缸函数
	BOOL ModuleIsSafe();
	BOOL CheckCanMoveMotor(void);

	void  ReadStepInfo();
	BOOL  SaveStepInfo(int nWriteType);

public:
	int m_ICheckVacu[2];    //真空感应
	int m_OOpenVacu[2];//真空电磁阀
	
	//旋转气缸
	int m_ITurnOut[2];    
	int m_ITurnReturn[2];
	int m_OTurnOut[2];
	int m_OTurnReturn[2];

	//外壳变距
	int m_IPushCylPush;
	int m_IPushCylBack;
	int m_OPushCylPush;
	int m_OPushCylReturn;

	int m_nMotorY;//Y轴电机
	int m_nMotorZ;//Z轴电机
	int m_nMotorR[2]; //R电机，用于旋转铝壳
	
	int m_IShellDirection[2];//铝壳朝口方向
	BOOL m_bNeedChangeDirect[2];

public:
	int m_nMotorMoveIndex;//电机移动的位置
	BOOL m_bPickCoverFinish;//取料完成信号
	BOOL m_bHomeMoveSafePos;//到安全位信号
	BOOL m_bEnableDocking;
};