/****************************************************************************************
文件名：RunProcessBatteryInShell.h
功能描述：入铝壳工位
历史修改记录：
*****************************************************************************************/
#pragma once
#include "RunProcess.h"

class CRunProcessBatteryInShell : public CRunProcess
{
protected:
	enum POWER_UP_STEPS
	{
		PU_WAIT_MODULE_HOME,
		PU_JUDGE_IF_WITH_PALLET,//判断带料启动

		PU_CHECK_WITHOUT_SHELL,
		PU_CLUMN_CYL_OPEN,
		PU_LIFT_CYL_DOWN,
		PU_SEND_PUSH_CYL_RETURN,
		PU_VACUM_CYL_UP,
		PU_CLOSE_VACUM,
		
		PU_WAIT_BATTER_CORE_SIGNAL_WITH_PALLET,
		PU_CHECK_PALLET_DATA,
		//保证两个入壳工位产品成对
		PU_WAIT_JUDGE_FINISH,
		PU_SAME_STATION_DATA_PAIR,
		
		PU_FINISH,
		NUM_OF_POWER_UP_STEPS,
	};

	enum AUTO_STEPS
	{
		AUTO_SEND_SHELL_CORE_REQUIRE,
		//铝壳定位
		AUTO_START_EVENT,
		AUTO_CHECK_HAVE_PRODUCT,
		AUTO_LIFT_CYL_UP,
		AUTO_SEND_TWICE_PUSH_CYL_PUSH,
		AUTO_TAIL_VACUUM_OPEN,
		AUTO_WAIT_CARRYSHELL_FINISH,
		
		//送壳
		AUTO_SEND_ONE_PUSH_CYL_PUSH,
		AUTO_CLUMMN_CYL_OPEN,
		AUTO_OPEN_CYL_CLOSE,
		AUTO_VACUM_CYL_DOWN,
		AUTO_MOUTH_VACUUM_OPEN,
		AUTO_VACUM_CYL_UP,
		AUTO_CHECK_VACUM,
		
		//等待入电芯
		AUTO_CLOSE_VACUM,
		AUTO_WAIT_CORE_READY,

		//下料
		AUTO_LIFT_CYL_DOWN,
		AUTO_WAIT_NEXT_MODULE_REQUIRE,
		AUTO_WAIT_NEXT_MODULE_WAORK_FINISH,
		AUTO_SEND_CYL_RETURN,
		AUTO_CLUMMN_CYL_CLOSE,
		AUTO_CHECK_NO_HAVE_PRODUCT,
		AUTO_WORK_END,
	};	

public:
	CRunProcessBatteryInShell(int nID);
	virtual ~CRunProcessBatteryInShell(void);

	BOOL InitializeConfig(CString strProcessModule);
	void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);
	BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove); 
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
	BOOL LocateCylPush( BOOL bPush);	// 壳口定位
	BOOL ColumnCylClose(BOOL bClose);//整列气缸
	BOOL SendOneCylPush(BOOL bPush);//一次传送气缸
	BOOL SendTwoCylPush(BOOL bPush);//二次传送气缸
	BOOL DownLiftCylUp(BOOL bLift);//下面顶升气缸
	BOOL VacumCylPush(BOOL bPush);//真空气缸 吸壳口气缸上升
	BOOL VacumOpenOrCheck(BOOL bOpen , BOOL bCheck); //真空打开与检测

	
	//检测料盘数据与实际检测数据一致性,nCheckResult为输出参数0没有料，1有料，2有壳但还没有入壳
	BOOL CheckStartWithPalletDataSame();


public:
	//铝壳定位气缸
	int m_IColumnCylOpen[2];
	int m_IColumnCylClose[2];
	int m_OColumnCylClose;
	int m_OColumnCylOpen;

	// 壳口定位
	int m_ILocateShellPush[2];
	int m_ILocateShellBack[2];
	int m_OLocateShellPush;
	int m_OLocateShellBack;

	//顶升气缸
	int m_ILiftCylUp;
	int m_ILiftCylDown;
	int m_OLiftCylUp;
	int m_OLiftCylDown;
	
	//一次推外壳气缸
	int m_ISendOneCylPush;
	int m_ISendOneCylBack;
	int m_OSendOneCylPush;
	int m_OSendOneCylBack;

	//二次推外壳气缸
	int m_ISendTwoCylPush;
	int m_ISendTwoCylBack;
	int m_OSendTwoCylPush;
	int m_OSendTwoCylBack;

	int m_IHaveCore;// 有料感应

	//壳口真空气缸 吸壳口气缸上升
	int m_IVacumCylPush[2];
	int m_IVacumCylBack[2];
	int m_OVacumCylPush[2];

	int m_OVacumCyl[2];//吸外壳上、下壳口真空
	int m_ICheckVacum[2];//真空检测
	int m_OBreakVacum;//破真空

	int m_ITailVacum;//底部真空
	int m_OTailVacum;//底部真空


public:
	//带料复位
	BOOL m_bDataJudgeFinish;  //等待另一个入电芯模组数据判断完成  双工位数据对比使用
	BOOL m_bResetJudge;   //可以重置 m_bDataJudgeFinish 标志

	int m_nRetryTimes;
	BOOL m_bCylClosed;
	BOOL m_bShellReady;// 铝壳准备好
	int m_nStartWithPalletData;
};