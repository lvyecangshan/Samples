/****************************************************************************************
文件名：RunProcessRobot.h
功能描述：机器人上料工位
历史修改记录：
*****************************************************************************************/
#pragma once
#include "RunProcess.h"
#include "RobotClient.h"

class CRunProcessRobot : public CRunProcess
{
protected:
	enum POWER_UP_STEPS
	{
		PU_ROBOT_START,//启动机器人
		PU_ROBOT_ALARM_RESET,//机器人报警复位
		PU_ROBOT_ALARM_CHECK,//检测机器人报警信号
		PU_ROBOT_CODE_RESET_AND_RUN,//机器人程序复位并运行
		PU_ROBOT_CONNET,//连接机器人
		
		PU_JUDGE_IF_WITH_PALLET,//判断带料启动
		
		//****不恢复数据***************
		PU_CHECK_WITHOUT_PRODUCT,
		PU_FINGER_CYL_OPEN,//夹爪打开
		PU_ROBOT_MOVE_TO_SAFE_POS,//机器人到安全位
		PU_ROBOT_MOVE_TO_PICK_WAIT_POS,//机器人到取料等待位
		PU_BUFFER_CYL_BACK,
		//*****************************

		//****恢复数据****************
		PU_FINGER_CYL_CLOSE_WITH_PALLET,
		PU_WAIT_FRONT_MODULE_SAFE,
		PU_ROBOT_MOVE_TO_SAFE_POS_WITH_PALLET,//机器人到安全位
		PU_CHECK_BUFFER_PALLET_DATA,
		PU_CHECK_PALLET_DATA,//判断恢复的数据与夹爪有料感应检测是否吻合
		PU_JUDGE_PRESS_DATE_EXIST,
		//****************************

		PU_JOINT_SIGNAL_RESET,

		PU_WAIT_BACK_MACHINE_READY,
		PU_CONNECT_SERVER,

		PU_FINISH,
		NUM_OF_POWER_UP_STEPS,
	};

	enum AUTO_STEPS
	{  //流程顺序与带料有关
		AUTO_IF_CLEAR_MATERIAL,
		AUTO_ROBOT_TO_PICK_WAIT_POS_INIT,
		AUTO_FINER_CLY_OPEN_INIT,

		//onload
		AUTO_WAIT_START_EVENT_ONLOAD,
		AUTO_ROBOT_TO_PICK_WAIT_POS_ONLOAD,
		AUTO_ROBOT_TO_PICK_POS_ONLOAD,
		AUTO_FINGER_CYL_CLOSE,
		AUTO_SEND_PICK_FINISH,
		AUTO_WAIT_BATTERY_OFFLOAD_PNP_AVOID,
		AUTO_ROBOT_TO_PICK_WAIT_POS_ONLOAD_CHECK,
		AUTO_SEND_PICK_BATTERY_FINISH,
		AUTO_CHECK_HAVE_PRODUCT,
		AUTO_WAIT_MES_SINGLE,
		AUTO_JUDGE_HAVE_NG,

		//offload
		AUTO_WAIT_START_EVENT_OFFLOAD,
		AUTO_ROBOT_TO_PUT_WAIT_POS_OFFLOAD,
		AUTO_ROBOT_TO_PUT_POS_OFFLOAD,
		AUTO_WAIT_MES_SINGLE_FOR_OFFLOAD,
		AUTO_FINGER_CYL_OPEN_OFFLOAD,
		AUTO_SEND_DATE,
		AUTO_ROBOT_TO_PUT_WAIT_POS_OFFLOAD_CHECK,
		AUTO_CHECK_WITHOUT_PRODUCT_OFFLOAD,
		AUTO_SEND_PUT_FINISH_SIGNAL_OFFLOAD,
		AUTO_WAIT_OFFLOAD_TO_NEXT_FINISH,

		//ng
		AUTO_WAIT_START_EVENT_NG,
		AUTO_ROBOT_TO_NG_WAIT_POS,
		AUTO_ROBOT_TO_NG_POS,
		AUTO_WAIT_MES_SINGLE_FOR_NG,
		AUTO_FINGER_CYL_OPEN_NG,
		AUTO_ROBOT_TO_NG_WAIT_POS_CHECK,
		AUTO_CHECK_WITHOUT_PRODUCT_NG,
		AUTO_SEND_NG_FINISH_SIGNAL_NG,

		// buf
		AUTO_ROBOT_TO_BUF_WAIT_POS,
		AUTO_BUFFER_CYL_ACTION_BUF,
		AUTO_CHECK_HAVE_PRODUCT_BUF,
		AUTO_ROBOT_TO_BUF_POS,
		AUTO_FINGER_CYL_ACTION_BUF, 
		AUTO_ROBOT_TO_BUF_WAIT_POS_CHECK,
		AUTO_CHECK_WITHOUT_PRODUCT_BUF,

		AUTO_JUDGE_BATTERY_ALL_PUT_FINISH,//判断所有电池放完成
		AUTO_WORK_END,
	};

public:
	CRunProcessRobot(int nID);
	virtual ~CRunProcessRobot(void);

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
	BOOL FingerCylClose (int nIndex,BOOL bClose);//夹爪动作函数
	BOOL BufferCylPush(BOOL bPush);  

	BOOL CheckHasVaild(int nIndex, BOOL bHave);//检查夹爪是否有产品

	BOOL ConnectRobot();   //连接机器人
	BOOL DisconnectRobot();   //断开连接机器人
	BOOL RobotStop();
	BOOL RobotAlarmReset();
	BOOL RobotCodeReset();
	BOOL RobotCodeRun();
	void GetLocalPalletData(CPallet* pallet,CString & strResult);  //获得本地盘数据

	BOOL CheckRobotSafe(); //检查机器人Z轴是否在安全位

	BOOL CheckStartWithPalletDataSame();  //机器人夹爪数据判断
	BOOL CheckStartWithPalletDataSameForBuffer(); //缓数据判断
	void EmgStop();
	void NomalRunEvent(); //给后段发送运行信号

	BOOL CheckBackSafe();  //检测后段安全信号
	BOOL CheckNeedBatteryEvent(BOOL bNeed);//检测要料信号

	BOOL SendPlaceFinishSignal(BOOL bFinish);  //放料信号
	BOOL SendSafeSignal(BOOL bSafe);//发安全信号
public:
	//缓存气缸
	int m_IBufferCylPush;
	int m_IBufferCylReturn;
	int m_OBufferCylPush;
	int m_OBufferCylReturn;

	//夹抓气缸  0---工位1  1---工位2
	int m_IFingerCylClose[2];
	int m_IFingerCylOpen[2];
	int m_OFingerCylClose[2];
	int m_OFingerCylOpen[2];

	// 有料检测 0-1:夹爪有料检测  2-3:缓存有料检测
	int m_IHasCore[4];	
	int m_IZCheckAbnormal;

	int m_IRobotAlarm;//机器人报警
	int m_IRobotRun;//机器人运行中
	int m_IRobotReset;//机器人复位状态
	int m_IRobotEnable;//机器人伺服

	int m_ORobotEnable;////机器人伺服(代码未用)
	int m_ORobotStop;//机器人停止
	int m_ORobotCodeRun;//机器人程序启动运行
	int m_ORobotAlarmReset;//机器人报警复位
	int m_ORobotCodeReset;//机器人程序复位
	int m_ORobotEmgStop;//机器人急停(不使用，机器人断电使用机台急停)


	int m_ITailFingerSignal[2];//后段工位夹料完成信号

	int m_ITailSafeSignal[2];  //后段工位安全位信号 

	int m_ORobotPutFinishSignal[2];//放料完成信号

	int m_ORobotPutInPosSignal[2];//电池放料到位信号  安全
	
	int m_INormalRunSingle; //后段机台正常运行信号
	int m_ONormalRunSingle; //前段机台正常运行信号

public:
	BOOL m_bEnableDocking;	// 是否开启后段对接
	BOOL m_bRobotWaitPos;		//机器人在等待位 等待信号时使用
	BOOL m_bRobotSafe;			//给取电池夹爪发机器人安全信号
	unsigned int m_unRobotSpeed;
	unsigned int m_nCurRobotLoc;

	int m_nWorkIndex;					//工位动作编号
	int m_nStartWithPalletData;	//带料启动料盘状态分类
	int m_nBufferWithPalletData;//缓存料盘状态分类

	BOOL m_bThreadStart;
	BOOL m_bThreadEnd;
	BOOL m_bEnableRobot; //启用机器人
	BOOL m_bBufferMes;    
	int m_nRobotPort;
	CString m_strRobotIp;
	CRobotClient m_pRobotClient;

	CPallet m_pBufferPallet;
	CPallet m_pTempPallet[2];

	BOOL m_bShieldServer;
	int m_nServerPort;
	CString m_strServerIp;
	CCCDCheckClient m_Client;   //传递条码客户端

};
