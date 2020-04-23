/****************************************************************************************
文件名：RunProcessCarryShellPnP.h
功能描述：入铝壳搬运机械手
历史修改记录：
*****************************************************************************************/
#pragma once
#include "RunProcess.h"

class CRunProcessCarryShellPnP : public CRunProcess
{
protected:
	enum POWER_UP_STEPS
	{
		PU_CHECK_HAVE_SHELL,
		PU_CLOSE_VACUM,
		PU_MOVE_Z_TO_HOME,
		PU_MOVE_Z_TO_SAFE_POS,
		PU_MOVE_X_TO_HOME,
		PU_MOVE_X_TO_TO_ONLOAD_POS,
		PU_CHECK_PALLET_DATA,
		PU_PUSH_CYL_RETURN,
		PU_FINISH,
		NUM_OF_POWER_UP_STEPS,
	};

	enum AUTO_STEPS
	{		
		AUTO_MOVE_X_TO_ONLOAD_POS,
		AUTO_CYL_PUSH_CYL_RETURN,
		AUTO_WAIT_ONLOAD_SINGLE,
		AUTO_MOVE_Z_TO_WAIT_PICK_POS,
		AUTO_WAIT_ONLOAD_SINGLE_EX,
		AUTO_MOVE_Z_TO_PICK_POS,
		AUTO_OPEN_VACUM,
		AUTO_MOVE_Z_TO_SAFE_POS,
		AUTO_SAFE_POS_CHECK_VACUM, 
		AUTO_SEND_MOVE_Z_TO_SAFE_SINGLE_TO_FRONT_MODULE,
		AUTO_PUSH_CYL_PUSH,
		AUTO_JUDGE_WHICH_MODULE_NEED_SHELL,
		AUTO_Z_AXIS_MOVE_TO_PLACE_WAIT_LOC,
		AUTO_CHECK_REQUIRE_AGAIN,
		AUTO_MOVE_X_TO_WAIT_POS,
		AUTO_JUDGE_WHICH_MODULE_NEED_SHELL_EX,
		AUTO_MOVE_X_TO_OFFLAOD_POS,
		AUTO_OFFLOAD_CHECK_VACUM,
		AUTO_MOVE_Z_TO_PUT_POS,
		AUTO_CLOSE_VACUM,
		AUTO_WAIT_BATTERY_CYL_CLOSE,
		AUTO_MOVE_Z_TO_SAFE_POS_AFTER_PLACE,
		AUTO_SEND_PLACE_FINISH_SINGLE,
		AUTO_JUDGE_ALL_PUT_FINISH,
		AUTO_WORK_END,
	};

	enum MOTOR_LOCS
	{
		X_MOTOR_ONLOAD_LOC = 0,//X轴取料位
		X_MOTOR_PNP_PUT_POS,	   //X轴放料位
		

		Z_MOTOR_SAFE_POS = 0,//安全位
		Z_MOTOR_WAIT_PICK_POS,//取料等待位
		Z_MOTOR_PICK_POS,//取料位
		Z_MOTOR_WAIT_PLACE_POS,// 放料等待位
		Z_MOTOR_PLACE_POS,//放料位
	};

public:
	CRunProcessCarryShellPnP(int nID);
	virtual ~CRunProcessCarryShellPnP(void);

	BOOL InitializeConfig(CString strProcessModule);
	void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);
	BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove);
	BOOL CheckOutputCanActive(COutput *pOutput);
protected:
	void PowerUpOperation(void);
	void PowerUpRestart(void);
	void AutoOperation(void);

public:
	BOOL BatteryInShellNoNeedShell();  
	BOOL CheckCanPutShell(int nIndex);
	BOOL VacumOpen(int nIndex, BOOL bOpen);
	BOOL BreakVacum( BOOL bBreak);
	BOOL CheckVacum(int nIndex,BOOL bOpen);   //yy-8.4 检查吸真空
	BOOL PushCylPush(BOOL bPush);//搬运气缸函数
	 
	//保护
	double GetMotorZPOS(); //获取Z轴安全位位置


public:
	int m_OVacumCyl[2];//真空吸电磁阀
	int m_ICheckVacum[2];//真空检测
	int m_OBreakVacum;//破真空

	//外壳变距气缸
	int m_IPushCylPush;//搬运气缸推出检测
	int m_IPushCylBack;//搬运气缸回退检测
	int m_OPushCylPush;//搬运气缸推出电磁阀
	int m_OPushCylBack;//搬运气缸回退电磁阀

	int m_nMotorX;//X轴电机
	int m_nMotorZ;//Z轴电机
public:
	BOOL m_bHomeMoveSafePos;
};