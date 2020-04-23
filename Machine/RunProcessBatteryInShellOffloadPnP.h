/****************************************************************************************
文件名：RunProcessBatteryInShellOffloadPnP.h
功能描述：入壳后下料抓手
历史修改记录：
*****************************************************************************************/
#pragma once
#include "RunProcess.h"

class CRunProcessBatteryInShellOffloadPnP : public CRunProcess
{
protected:
	enum POWER_UP_STEPS
	{
		PU_WAIT_ROBOT_HOME_FINISH,
		PU_JUDGE_IF_WITH_PALLET,//判断带料启动
		

		PU_CHECK_WITHOUT_BATTER,
		PU_FINGER_CYL_OPEN,//夹爪打开，控制入壳工位顶升下降
		PU_MOTOR_HOME,
		PU_MOTOR_X_TO_WAIT_POS,
		//带料
		PU_CHECK_PALLET_DATA,
		PU_FINGER_CYL_CLOSE_WITH_PALLET,
		PU_MOTOR_HOME_WITH_PALLET,
		PU_MOTOR_X_TO_WITH_PALLET,
		//保证两个工位产品成对
		PU_WAIT_JUDGE_FINISH,
		PU_CHECK_WITHOUT_BATTERY_WAIT_PALLTE,

		PU_FINISH,
		NUM_OF_POWER_UP_STEPS,
	};

	enum AUTO_STEPS
	{//流程顺序与带料有关
		AUTO_SEND_SHELL_CORE_REQUIRE,
		AUTO_START_EVENT,
		AUTO_MOTOR_X_TO_PICK_POS,  
		AUTO_PNP_SEND_CYL_OUT,
		AUTO_PNP_SEND_CYL_PUSH,
		AUTO_FINGER_CYL_CLOSE,
		AUTO_CLUMMN_CYL_OPEN,
		AUTO_MOTOR_X_TO_WIAT_POS,  
		
		
		AUTO_PNP_SEND_CYL_RETURN,
		AUTO_WAIT_MES_SINGLE,
		AUTO_SEND_RETURN_SINGLE,
		AUTO_SEND_OFFLAD_SINGLE,
		AUTO_WAIT_OFFLOAD_SINGLE,
		AUTO_FINGER_CYL_OPEN,
		AUTO_SEND_CLAMP_SINGLE,
		AUTO_SEND_MOVE_AVOID,
		AUTO_WAIT_OFFLAOD_FINISH,
		AUTO_MOTOR_MOVE_PICK,
		AUTO_FORONT_MODULE_FINISH,
		AUTO_WORK_END,
	};	
public:
	enum MOTOR_X_LOC
	{
		X_MOTOR_PICK_POS,  //取料位
		X_MOTOR_WAIT_POS,  // 下料位
		X_MOTOR_AVOID_POS, //偏移位 

	};

public:
	CRunProcessBatteryInShellOffloadPnP(int nID);
	virtual ~CRunProcessBatteryInShellOffloadPnP(void);

	BOOL InitializeConfig(CString strProcessModule);
	void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);
	BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove); 
	BOOL CheckOutputCanActive(COutput *pOutput);
	void ReadStepInfo();
	BOOL SaveStepInfo(int nWriteType);


protected:
	void PowerUpOperation(void);
	void PowerUpRestart(void);
	void AutoOperation(void);


public:
	BOOL PnPSendCylPush(BOOL bPush);//避位气缸
	BOOL FingerCylClose(BOOL bClose);//夹抓气缸
	//检测料盘数据与实际检测数据一致性,nCheckResult为输出参数0没有料，1有料
	BOOL CheckStartWithPalletDataSame();

	//保护
	double GetMotorXAvoidPOS();


public:	
	int m_IPnPSendCylPush;//避位气缸推出感应
	int m_IPnPSendCylBack;//避位气缸回退感应
	int m_OPnPSendCylPush;//避位气缸电磁阀
	int m_OPnPSendCylBack;//避位气缸回退电磁阀
	
	int m_IFingerCylClose;//夹爪气缸夹紧感应
	int m_IFingerCylOpen;//夹爪气缸松开感应
	int m_OFingerCylClose;//夹爪气缸夹紧电磁阀
	int m_OFingerCylOpen;//夹爪气缸松开电磁阀

	int m_nMotorX;   //   X电机
	int m_IHaveCore;//有料感应

public:
	//带料复位
	BOOL m_bDataJudgeFinish;  //等待另一个入电芯模组数据判断完成  双工位数据对比使用
	BOOL m_bResetJudge;   //可以重置 m_bDataJudgeFinish 标志

	int m_nStartWithPalletData;
	BOOL m_bHomeMoveSafePos;
	BOOL m_bOpenCylOpen;//夹爪松开信号 给机器人
	BOOL m_bPnPSendCylReturn; //夹爪抓完料回退信号  给入壳
	BOOL m_bOffloadToRobot; //给机器人下料信号

	BOOL m_bOffload;
};