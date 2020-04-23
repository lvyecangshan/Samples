/****************************************************************************************
文件名：RunProcessCleanShell.cpp
功能描述：外壳清洗模组
历史修改记录：
*****************************************************************************************/
#pragma once
#include "RunProcess.h"

class CRunProcessCleanShell : public CRunProcess
{
protected:
	enum POWER_UP_STEPS
	{
		PU_CLOSE_AIR,
		PU_JUDGE_IF_WITH_PALLET,//判断带料启动
		
		PU_CHECK_WITHOUT_SHELL,
		PU_FINGER_CYL_OPEN,
		PU_LIFT_CYL_DOWN,
		
		JUDGE_SHELL_NUM,
		PU_CHECK_FINGER_STATE,
		PU_FINGER_CYL_CLOSE,
		WAIT_FRONT_AFTER_SAFE_SIGNAL,
		PU_LIFT_CYL_UP,
		PU_FINISH,
		NUM_OF_POWER_UP_STEPS,
	};

	enum AUTO_STEPS
	{
		AUTO_CHECK_WITHOUT_SHELL,
		AUTO_FINGER_CYL_OPEN,
		AUTO_LIFT_CYL_DOWN,

		//ONLOAD
		AUTO_SEND_REQUIRE_EVENT,
		AUTO_WAIT_START_EVENT,
		AUTO_LIFT_CYL_UP,
		AUTO_FINGER_CYL_CLOSE,
		AUTO_CHECK_HAVE_SHELL, //打开吹气并计时
		AUTO_SEND_FINGER_CYL_CLOSE_SINGLE,
		AUTO_WAIT_FRONT_MODULE_MOVE_SAFE_POS,
		AUTO_WAIT_AIR_TIME_OUT,

		//OFFLOAD
		AUTO_SEND_OFFLOAD_REQUIRE,
		AUTO_WAIT_REQUIRE_RESPONSE,
		AUTO_FINGER_CYL_OPEN_OFFLOAD,
		AUTO_LIFT_CYL_DOWN_OFFLOAD,
		AUTO_SEND_LIFT_CYL_DOWN_EVENT,
		AUTO_WAIT_NEXT_MODULE_MOVE_SAFE_POS,
		AUTO_WORK_END,
	}; 

public:
	CRunProcessCleanShell(int nID);
	virtual ~CRunProcessCleanShell(void);

	BOOL InitializeConfig(CString strProcessModule);
	void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);\
	BOOL CheckOutputCanActive(COutput *pOutput);
	BOOL SaveParameter();
	BOOL ReadRunParameter();

protected:
	void PowerUpOperation(void);
	void PowerUpRestart(void);
	void AutoOperation(void);

public:
	BOOL FingerCylClose(BOOL bClose);//夹爪动作函数
	BOOL LiftCylUp(BOOL bUp);//顶升气缸动作函数
	int JudgeShellNum();//检测壳数量 

public:
	//顶升气缸
	int m_ILiftCylUp;  
	int m_ILiftCylDown;
	int m_OLiftCylUp; 
	int m_OLiftCylDown;
	//夹抓
	int m_IFingerCylClose[2];
	int m_IFingerCylOpen[2];
	int m_OFingerCylClose;
	int m_OFingerCylOpen; 

	int m_IHaveProduct[2];//夹抓有料感应
	int m_OAirBlow; //吹气电磁阀
	int m_OAirExhaust; //抽气电磁阀  不确定

public:
	DWORD m_nAirTime;     //吹气计时
	int m_nAirTimelimit;//吹气限时
	BOOL m_bOffloadRequire;
	BOOL m_bFingerClose;//夹爪夹紧信号 与前翻转信号交互
	BOOL m_bLiftDown;   //顶升下降信号	与后翻转信号交互
};