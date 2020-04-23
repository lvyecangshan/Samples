/****************************************************************************************
文件名：CRunProcessFeederTray.h
功能描述：料盘搬运模组
历史修改记录：
*****************************************************************************************/
#pragma once
#include "RunProcess.h"

class CRunProcessFeederTray : public CRunProcess
{
protected:
	enum POWER_UP_STEPS
	{
		PU_CHECK_ANOTONER_MODULE_SAFE,
		PU_MOVE_MOTOR_X_HOME,
		PU_CHEACK_HAVE_PALLET,
		PU_OPEN_CLAMP_CYL,
		PU_OPEN_FINGLE_CYL,
		PU_MOTOR_TO_ONLOAD_POS,
		PU_CHECK_MOTOR_INPOS,
		PU_FINISH,
		NUM_OF_POWER_UP_STEPS,
	};

	enum AUTO_STEPS
	{
		AUTO_JUDGE_HAVE_PALLET,
		AUTO_MOVE_MOTOR_TO_PICK_PALLET_POS,
		AUTO_CHECK_MOTOR_MOVE_ONLOAD_INPOS,
		AUTO_SEND_NEDD_PALLET_SINGLE_TO_ONLOAD,
		AUTO_WAIT_ONLOAD_MODULE_SEND_PALLET_FINISH,

        AUTO_CLAMP_CHECK_HAVE_PALLET,
		AUTO_CLMAP_CYL_CLAMP,
		AUTO_FINGLE_CYL_CLAMP,
		AUTO_CHECK_HAVE_PALLET_AGANIN,
		AUTO_WAIT_ONLOAD_MODULE_MOVE_SAFE_POS,
		AUTO_MOVE_MOTOR_TO_START_FEED_COVER_POS,
		AUTO_WAIT_START_EVENT,
		AUTO_MOVE_MOTOR_TO_FEED_COVER_POS,
		AUTO_WAIT_PICK_COVER_FINISH,
		AUTO_JUDGE_ALL_COVE_PICK_FINISH,
		AUTO_MOVE_PALLET_TO_OFFLAOD_POS,
		AUTO_CHECK_MOTOR_MOVE_OFFLOAD_INPOS,
		AUTO_SEND_OFFLAD_MODUEL_PICK_PALLET,
		AUTO_WAIT_OFFLOAD_MODULE_LIFT_FINISH,
		AUTO_CLMAP_CYL_OPEN_OFFLOAD,
		AUTO_FINGLE_CYL_CLAMP_CLOSE,
		AUTO_WAIT_OFFLOAD_MOVE_SAFE,
		AUTO_JUDGE_PICK_PALLET_ALL_FINISH,
		AUTO_CHECK_HAVE_NO_PALLET,
		AUTO_JUDGE_PICK_PALLET_TIMES,
		AUTO_CHECK_PALLET_REMOVE,
		AUTO_WORK_END,
	};
	
	enum MOTOR_WORK_LOC
	{
		X_MOTOR_ONLOAD_LOC1 = 0,
		X_MOTOR_ONLOAD_LOC2,
		X_MOTOR_OFFLOAD_LOC,
		X_PICK_COVER_1,
		X_PICK_COVER_2,
		X_PICK_COVER_3,
		X_PICK_COVER_4,
		X_PICK_COVER_5,
		X_PICK_COVER_6,
		NUM_WORK_LOC
	};

public:
	CRunProcessFeederTray(int nID);
	virtual ~CRunProcessFeederTray(void);

public:
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
	void WorkWhile(void);//防撞感应和光栅检测
	void Destory(void);//清除资源
	BOOL CheckHaveNoPallet(); //检测有无料盘
	BOOL CheckCanHome(void);//判断其他模组是否安全
	void ResertHomeSingle(void);//重置复位信号
	BOOL OpenVacuum(BOOL bOpen);//打开真空
	BOOL SeparateClose(BOOL bClose);//托盘气缸动作
	BOOL CheckPalletInPos();//检测料盘是否到位

public:
	int m_ISeparateClose[2];//托盘气回退感应
	int m_ISeparateOpen[2];//托盘气缸推出感应
	int m_OSeparateOpen;//托盘气缸1、2推出电磁阀
	int m_OSeparateClose;//托盘气缸1、2回退电磁阀

	int m_IPalletInPosition;//有盘感应
	int m_ITrayInPosPresent[3];//0上料1到位，2上料2到位，3下料到位

	int m_IVacuum[6];//吸外壳托盘真空
	int m_OVacuum[6];//吸外壳料盘真空

	int m_ISafeRasterCheck[3];//光栅感应
	int m_ISafeCheckPallet;//防撞感应

	int m_nMotorX;//X电机

public:
	BOOL m_bPutPalletFinish;//放料完成
	BOOL m_bPickPalletFinish;//取料完成

	BOOL m_bMoveMotorFinish;//电机移动完成标记

	int m_nMoveMotorIndex;//取铝壳时电机移动位置
	int m_nPickPalletPos;	// 当前抓取料盘的堆栈序号
	BOOL m_bOnloadPnpRequireShell;	// 取铝壳机械手交互信号

	BOOL m_bThreadFinished;
	BOOL m_bThreadStart;
};