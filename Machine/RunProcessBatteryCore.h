/****************************************************************************************
文件名：RunProcessBatteryCore.h
功能描述：入电芯工位
历史修改记录：
*****************************************************************************************/
#pragma once
#include "RunProcess.h"
#include "NetSenseClient.h"
#include "CCDCheckClient.h"

class CRunProcessBatteryCore : public CRunProcess
{
protected:
	enum POWER_UP_STEPS
	{
		PU_CHECK_ONLOAD_CORE_HOME_FINISH,
		PU_OPEN_PRESS_COMPORT,
		PU_COLUMNCYL_OPEN,
		PU_OPEN_CYL_OPEN,	
		PU_LIFT_CLY_DOWN,
		PU_CLOSE_VACUM,
		PU_SEND_MOTOR_HOME,
		PU_SEND_MOTOR_MOVE_TO_WAIT_POS,
		PU_ZERO_PRESS_DATA,

		PU_JUDGE_IF_WITH_PALLET,
		PU_CHECK_WITHOUT_CORE,

		PU_CHECK_PALLET_DATA,
		PU_WAIT_JUDGE_FINISH,
		PU_SAME_STATION_DATA_PAIR,

		
		PU_FINISH,
		NUM_OF_POWER_UP_STEPS,
	};

	enum AUTO_STEPS
	{
		AUTO_SEND_MOTOR_TO_WAIT_POS,
		AUTO_SEND_NEED_CORE_REQUIRE,
		AUTO_ClEAR_PRESS,
		AUTO_START_EVENT,
		AUTO_CHECK_HAE_PRODUCT,
		AUTO_CLUMMN_CYL_CLOSE,
		//推电芯
		AUTO_SEND_MOTOR_TO_PICK_POS,
		AUTO_LIFT_CYL_UP,
		AUTO_CEHCK_VACUM_STATUS,
		AUTO_CLUMN_CYL_OPEN,
		
		AUTO_JUDGE_HAVE_SHELL,
		AUTO_SEND_MOTOR_TO_INSHELL_WAIT_POS,
		AUTO_WAIT_SHELL_READY,
		AUTO_CHECK_OPEN_CYL_CLOSE,
		AUTO_SEND_MOTOR_TO_OPEN_CYL_OPEN_POS,
		//入壳
		AUTO_TWICE_CLY_RETURN,
		AUTO_OPEN_CYL_OPEN,
		AUTO_CHECK_OPEN_CYL_OPEN,
		AUTO_SEND_MOTOR_TO_INSHELL_POS,
		AUTO_ANALYZE_PREESS_DATA,
		AUTO_SAVE_ALL_PRESS_DATA,
		AUTO_SEND_INSHELL_FINISH_SINGLE,
		AUTO_SEND_MOTOR_TO_WAIT_POS_EX,

		AUTO_WORK_END,
	};

	enum MOTOR_LOC
	{
		SEND_MOTOR_WAIT_LOC=0,  //开始位
		SEND_MOTOR_PICK_POS,//接电芯位
		SEND_MOTOR_INSHELL_WAIT_LOC,//入壳等待位
		SEND_MOTOR_OPEN_CYL_OPEN_LOC,//开合气缸张开位
		SEND_MOTOR_INSHELL_LOC,//入壳位
    };

public:
	CRunProcessBatteryCore(int nID);
	virtual ~CRunProcessBatteryCore(void);
	BOOL InitializeConfig(CString strProcessModule);
	void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);
	BOOL  CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove); 
	BOOL CheckOutputCanActive(COutput *pOutput);
	BOOL SaveParameter();
	BOOL ReadRunParameter();
	void ReadStepInfo();
	BOOL SaveStepInfo(int nWriteType);

protected:
	void PowerUpOperation(void);
	void PowerUpRestart(void);
	void AutoOperation(void);

public:
	BOOL ColumnCylClose(BOOL bClose);//整列气缸
	BOOL OpenCylOpen(BOOL bOpen);//开合气缸
	BOOL CheckCylClose(); //检查开关气缸关闭
	BOOL CheckCylOpen(BOOL bOpen);//检查开合气缸打开
	BOOL LiftCylUp(BOOL bUp);//顶升气缸
	BOOL CloseOpenCyl(void);//关闭开合气缸

public:
	void RunWhile();
	double ReadPress();
	double GetPressMaxValue();
	BOOL SavePressValue();// 保存压力值 

	//检测料盘数据与实际检测数据一致性,nCheckResult为输出参数0没有料，1有料
	BOOL CheckStartWithPalletDataSame();
	
	
public:
	//整列气缸
	int m_IColumnCylOpen[2];
	int m_IColumnCylClose[2];
	int m_OColumnCylClose;
	int m_OColumnCylOpen;

	//开合气缸
	int m_IOpenCylOpen[2];
	int m_IOpenCylClose[2];
	int m_OOpenCylOpen;  //开合气缸打开
	int m_OOpenCylClose; //开合气缸关闭
	
	//顶升气缸
	int m_ILiftCylUp;
	int m_ILiftCylDown;
	int m_OLiftCylUp;
	int m_OLiftCylDown;

	int m_IOpenCylOpenTwo[2]; //开合气缸关闭二次确认感应

	//真空检测
	int m_ICheckVacum;//真空检测
	int m_OOpenVacum;//真空电磁阀

	int m_IHaveCore;//有料检测	
	
	int m_OSetZero;//压力表清零 

	int m_MotorX;//送电芯电机

	int m_IReachOpenAndClosePos;   //电机到达开合气缸打开位感应

public:
	//带料启动
	BOOL m_bDataJudgeFinish;  //等待另一个入电芯模组数据判断完成  双工位数据对比使用
	BOOL m_bResetJudge;   //可以重置 m_bDataJudgeFinish 标志
	BOOL m_bPowerUpFinish;//给入壳工位带料复位完成

	BOOL m_bRequireCore;
	BOOL m_bOpenLoc;   //开合气缸打开
	int m_nTryReadTimes;//读压力次数
	int m_nStartWithPalletData;// 带料启动料盘状态分类

	BOOL  m_bThreadStart;
	BOOL  m_bThreadEnd;

public:
	// 压力相关
	double m_fPressNormValue[2];	// 设定值 0:min  1:max
	BOOL m_bEnablePressSenseClient;
	CString m_strPressSenseIP;
	int m_nPressSensePort;
	CCriticalSection m_PressDataLock;
	BOOL  m_bStartGetValue;	//开始取压力值   
	BOOL  m_bGetValueFinish;	//取值结束
	CNetSenseClient m_PressSenseClient;
	vector<double> m_vecPressValue;
	CCCDCheckClient m_PressClient;
};