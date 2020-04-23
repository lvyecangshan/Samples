/****************************************************************************************
文件名：RunProcessOnloadCorePNPEx.cpp
功能描述：入壳上电芯机械手
历史修改记录：
*****************************************************************************************/
#pragma once
#include "RunProcess.h"
#include "NetSenseClient.h"

class CRunProcessOnloadCore : public CRunProcess
{
protected:
	enum POWER_UP_STEPS
	{
		PU_PRESS_CYL_PUSH,  //防止带料启动电芯移动
		PU_MOVE_Z_TO_HOME,
		PU_MOVE_Y_TO_HOME,
		PU_JUDGE_IF_WITH_PALLET,//判断带料启动
		PU_CHECK_WITHOUT_PRODUCT,
		PU_PRESS_CYL_RETURN,
		PU_FINGER_CYL_OPEN,
		PU_ROTATE_CYL_RETURN,
		PU_MOVE_Z_TO_SAFE_POS,
		PU_MOVE_Y_TO_ONLOAD_POS,
		PU_BUFFER_CYL_BACK,//暂存气缸回退

		PU_CHECK_BUFFER_PALLET_DATA,
		PU_CHECK_PALLET_DATA,
		PU_PRESS_CLY_ACTION_WITH_PALLET,
		PU_ROTATE_CYL_PUSH_WITH_PALLET,
		PU_JUDGE_BARCODE_EXIST,

		PU_FINISH,
		NUM_OF_POWER_UP_STEPS,
	};

	enum AUTO_STEPS
	{
		AUTO_MOVE_Y_TO_ONLOAD_POS,
		AUTO_ROTATE_CLY_RETURN_INIT,
		AUTO_PRESS_CLY_RETURN_INIT,
		AUTO_FINER_CLY_OPEN_INIT,
		AUTO_WAIT_READY_ONLOAD_SIGNAL,
		AUTO_WAIT_START_EVENT_LINE,

		//onload
		AUTO_MOVE_Z_TO_ONLOAD_POS,
		AUTO_CHECK_AXIS_STATE,
		AUTO_FINGER_CYL_CLOSE,
		AUTO_PRESS_CYL_PUSH,
		AUTO_MOVE_Z_TO_SAFE_POS,
		AUTO_CHECK_HAVE_PRODUCT,
		AUTO_SEND_ONLOAD_FINISH,
		AUTO_WAIT_REPLY_SIGNAL,
		AUTO_ROTATE_CYL_PUSH,
		AUTO_JUDGE_HAVE_NG,

		//offload
		AUTO_MOVE_Y_TO_OFFLOAD_POS,
		AUTO_WAIT_NEXT_MOUDLE_REQUIRE,
		AUTO_MOVE_Z_TO_OFFLOAD_POS,
		AUTO_PRESS_CYL_RETURN_OK,
		AUTO_FINGER_CYL_OPEN_OK,
		AUTO_SEND_DATE,
		AUTO_MOVE_Z_TO_SAFE_POS_OK,

		//ng
		AUTO_MOVE_Y_TO_NG_POS,
		AUTO_WAIT_NG_MODULE_REQUIRE,
		AUTO_MOVE_Z_TO_NG_POS,
		AUTO_PRESS_CYL_RETURN_NG,
		AUTO_FINGER_OPEN_NG,
		AUTO_MOVE_Z_TO_SAFE_PO_NG,
		AUTO_RESET_NG_LINE_REQUIRE,

		// buf
		AUTO_MOVE_Y_TO_BUFFER_POS,
		AUTO_BUFFER_CYL_ACTION,
		AUTO_CHECK_HAVE_PRODUCT_BUFFER,
		AUTO_MOVE_Z_TO_BUFFER_POS,
		AUTO_PRESS_CYL_RETURN_BUF_EX,
		AUTO_PRESS_CYL_RETURN_BUF,
		AUTO_FINGER_CYL_OPEN_BUF,
		AUTO_MOVE_Z_TO_SAFE_POS_BUF,
		AUTO_CHECK_WITHOUT_PRODUCT_BUF,
		AUTO_JUDGE_BATTERY_ALL_PUT_FINISH,//判断所有电池放完成
		AUTO_WORK_END,
	};

	enum MOTOR_LOC
	{
		Y_MOTOR_PICK_LOC=0,      //取料位
		Y_MOTOR_PNP_OFFLOAD_LOC,//放良品位
		Y_MOTOR_NG_POS,          //排不良位
		Y_MOTOR_BUFFER_POS,      //缓存位置


		Z_MOTOR_SAFE_LOC= 0,  //Z轴安全位
		Z_MOTOR_PICK_LOC,     //Z轴取料位
		Z_MOTOR_PUT_OK_LOC,   //Z轴放良品位
		Z_MOTOR_PUT_NG_LOC,   //排不良位
		Z_MOTOR_BUFFER_LOC,	  //缓存位
	};

public:
	CRunProcessOnloadCore(int nID);
	virtual ~CRunProcessOnloadCore(void);

	BOOL InitializeConfig(CString strProcessModule);
	void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);
	BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove);
	BOOL CheckOutputCanActive(COutput *pOutput);

protected:
	void PowerUpOperation(void);
	void PowerUpRestart(void);
	void AutoOperation(void);

public:
	BOOL FingerClose (int nIndex,BOOL bClose);//夹爪动作函数
	BOOL RotatePush (int nIndex,BOOL bPush);  //旋转气缸动作函数
	BOOL PressPush (int nIndex,BOOL bPush);   //压电芯气缸动作函数
	BOOL BufferPush(BOOL bPush);               //缓存气缸

	BOOL CheckHasVaild(int nIndex, BOOL bHave);//检查夹爪是否有产品
	BOOL CheckZSafe();//检查Z轴是否压料
	void EmgStop();  //轴压料急停
    void ThreadRelease();//轴压料急停线程释放

	void  ReadStepInfo();
	BOOL  SaveStepInfo(int nWriteType);
	BOOL CheckStartWithPalletDataSame();  //机械手数据判断
	BOOL CheckStartWithPalletDataSameForBuffer(); //缓数据判断

	double GetMotorZSafePos();//获取Z轴安全位位置

public:
	//夹抓气缸  01---工位1  23---工位2
	int m_IFingerClose[4];
	int m_IFingerOpen[4];
	int m_OFingerClose[2];
	int m_OFingerOpen[2];

	//压料气缸 感应  01---工位1 23---工位2
	int m_IPressPush[4];
	int m_IPressReturn[4];
	int m_OPressPush[2];   //抓手压料
	int m_OPressReturn[2];   //抓手压料

	//缓存气缸
	int m_IBufferCylPush;
	int m_IBufferCylReturn;
	int m_OBufferCylPush;
	int m_OBufferCylReturn;

	// 有料检测 0-1:夹爪有料检测  2-3:缓存有料检测
	int m_IHasCore[4];
	int m_IZCheckAbnormal[4];		//压料感应

	int m_nMotorY;
	int m_nMotorZ;

public:
	BOOL m_bThreadStart;
	BOOL m_bThreadEnd;
	BOOL m_bEnableDocking;
	int m_nWorkIndex;//工位动作编号
	int m_nStartWithPalletData;//带料启动料盘状态分类
	int m_nBufferWithPalletData;
	BOOL m_bHomeMoveSafePos;
	CPallet m_pBufferPallet;
	CPallet m_pTempPallet[2];


	int m_nWaitTime;
	// 条码枪
	int m_nScanPort;
	CString m_strScanIp;
	CString m_strScanCmd;
	CNetSenseClient  m_ScanCodeClient;
	
	//添加线体对接信号 Yao  5.6
	int m_IReadyOnloadSignal;//线体准备好信号
	int m_IPLCRunningSignal;//线体正常运行信号
	int m_ORequiredOnloadSignal;//入壳机上料完成信号
	int m_OPLCRunningSignal;//入壳机正常运行信号
};