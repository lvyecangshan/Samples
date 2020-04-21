/*******************************************************************
文件名：run.h
作者：              版本：A1        日期：
功能描述：控制平台运行基类
历史修改记录：
*******************************************************************/
#pragma once

#include "RunTask.h"
#include "Evt.h"
#include "Input.h"
#include "Output.h"
#include "motor.h"

#define MSG_SHOW_DEFAULT -1
#define MSG_NOT_NEED   0

class CRunCtrl;

class __declspec(dllexport) CRun : public CRunTask
{
	friend CRunCtrl;

public:
	enum RUN_MODE
	{
		RUN_MODE_POWER_UP,	//初始化
		RUN_MODE_AUTO,		//自动运行
		RUN_MODE_STOP,		//停止
	};
	
	enum RUN_STATE
	{	
		RUN_STATE_RUNNING,	//运行中
		RUN_STATE_STOP		//停止
	};

	//仅在WaitMultipleInputs函数中使用
    enum WAIT_INPUT_MODE
	{
		WAIT_ALL_STATE_ACTIVE,
		WAIT_ONE_IS_STATE,
		WAIT_ALL_ON,
		WAIT_ONE_IS_ON,
		WAIT_ALL_OFF,
		WAIT_ONE_IS_OFF,
	};

public:
	CRun(int nID);
	virtual ~CRun();

public:
	/*********************************************************************************
	//函数描述：设备初始化流程函数
	//输入参数：无
	//函数返回: 无
	**********************************************************************************/
	virtual void PowerUpOperation();

	/*********************************************************************************
	//函数描述：自动生产流程函数
	//输入参数：无
	//函数返回: 无
	**********************************************************************************/
	virtual void AutoOperation();

	/*********************************************************************************
	//函数描述：整机复位响应函数,通常在用于做变量初始化处理
	//输入参数：无
	//函数返回: 无
	**********************************************************************************/
	virtual void PowerUpRestart() { m_strMsg = _T("Ready"); ClearMessage(); }

	/*********************************************************************************
	//函数描述：显示指定ID号的消息，具体消息在Message.cfg中添加和查看
	//			函数优点：不阻塞其他线程，有利于直接显示故障报警
	//输入参数：nMsgID		消息编号
	//			data1		附加信息1，一般没用
	//			data2		附加信息2，一般没用
	//函数返回: 无
	**********************************************************************************/
	void ShowMessage(int nMsgID, UINT data1 = 0, int data2 = 0);

	/*********************************************************************************
	//函数描述：显示指定字符串的告警信息
	//输入参数：strMsg		消息字符串
	//			nType		提示类型 0：普通提示（灰色框） 1：问答提示（蓝色框） 2：普通预警（黄色框） 3：故障报警（红色框）
	//函数返回: 无
	**********************************************************************************/
	void ShowMessage(CString strMsg, int nType);

	/*********************************************************************************
	//函数描述：检查输入I/O的状态
	//			注意：检查单输入点报警用，避免用此函数做循环状态判断（如需循环判断可直接用Input本身的函数），因每次调用会产生相应日志记录
	//输入参数：pInput		输入I/O指针
	//			bState		输入I/O应有状态，一般使用ON或OFF表示，而不用TRUE或FALSE
	//			nMsgID		如不是想要的bState状态时输出异常信息(同ShowMessage())，如 未指定则做默认信息处理. 
	//函数返回: TRUE：检查结果和bState相同  FALSE：检查结果和bState不同
	**********************************************************************************/
	BOOL CheckInput(CInput *pInput, BOOL bState, int nMsgID = MSG_SHOW_DEFAULT);

	/*********************************************************************************
	//函数描述：等待输入I/O的状态置换成指定状态
	//输入参数：pInput		输入I/O指针
	//			bState		输入I/O应有状态，一般使用ON或OFF表示，而不用TRUE或FALSE
	//			nMsgID		如不是想要的bState状态时输出异常信息(同ShowMessage())，如 未指定则做默认信息处理。（没输此参数则不做信息处理）
	//						MSG_SHOW_DEFAULT 为默认，MSG_NOT_NEED 为不需要发出错误信息的情况，大于MSG_NOT_NEED值为自定义消息ID,自定义消息ID需要注意，必须要大于easylibdef.h文件内LIBRARY_MSG_ID定义的数值
	//函数返回: WAIT_OK：	检测结果与bState相同
	//			WAIT_ERROR：超时(时间设定根据Input.cfg)
	//			WAIT_STOP：	被停止
	**********************************************************************************/
	int  WaitInput(CInput *pInput, BOOL bState, int nMsgID = MSG_NOT_NEED);

	/*********************************************************************************
	//函数描述：获取工作模块使能状态
	//输入参数：无
	//函数返回: TRUE：使能  FALSE：禁止/关闭
	**********************************************************************************/
	BOOL IsModuleEnable();

	/*********************************************************************************
	//函数描述：单个轴复位
	//输入参数：pMotor	轴号指针
	//函数返回: TRUE：运行成功  FALSE：运行失败
	**********************************************************************************/
	BOOL HomeMotor(CMotor *pMotor);

	/*********************************************************************************
	//函数描述：多个轴同时复位
	//输入参数：pMotor	轴号指针数组
	//			nMotors 轴总数
	//函数返回: TRUE：运行成功  FALSE：运行失败
	**********************************************************************************/
	BOOL HomeMotors(CMotor **ppMotorArray, int nMotors);
	    
	/*********************************************************************************
	//函数描述：操作指定输出I/O，直到指定输入I/O为指定状态，则返回，否则告警
	//输入参数：pInput		输入I/O指针
	//			bInState	输入I/O状态，一般使用ON或OFF表示，而不用TRUE或FALSE
	//			pOutput		输出I/O指针
	//			bOutState	输出I/O状态，一般使用ON或OFF表示，而不用TRUE或FALSE
	//			nMsgID		如不是想要的bState状态时输出异常信息(同ShowMessage())，如 未指定则做默认信息处理. 
	//函数返回: WAIT_OK：	如果pInput信号状态为bInState
	//			WAIT_ERROR：参数错误
	//			WAIT_STOP：	被停止
	//			WAIT_TIMEOUT： 超时(时间设定根据Output.cfg)
	**********************************************************************************/
	int IoAction(CInput *pInput, BOOL bInState, COutput* pOutput, BOOL bOutState, int nMsgID = MSG_SHOW_DEFAULT);

	/*********************************************************************************
	//函数描述：针对一般靠IO驱动的直流或交流电机移动
	//输入参数：pInStop		指向停止信号输入点的指针
	//			pOutMove	指向驱动输出点的指针
	//			bInStopState 停止信号输入点的状态
	//			pInAlarm	指向报警信号输入点的指针
	//			pOutBreaker 指向驱动刹车输出点的指针
	//			pSubStop	指向辅助停止信号输入点的指针
	//函数返回: WAIT_OK：	如果pInStop或者pSubStop信号为ON
	//			WAIT_ERROR：报警返回或参数错误
	//			WAIT_STOP：	被终止
	//			WAIT_TIMEOUT： 超时
	**********************************************************************************/
	int IoMotorMove(CInput* pInStop, COutput* pOutMove, BOOL bInStopState = ON, CInput* pInAlarm = NULL, COutput* pOutBreaker = NULL, CInput* pSubStop = NULL);

	/*********************************************************************************
	//函数描述：等待事件的指定信号状态
	//输入参数：pEvt		指向单个事件的指针
    //			bState		要检测的状态
    //			nFailMsgID	如不是想要的bState状态时则输出对应的信息(同ShowMessage())，如没输此参数则不做信息处理
	//函数返回: WAIT_OK：	检测结果与bState相同
	//			WAIT_ERROR：报警返回或参数错误
	//			WAIT_STOP：	被终止
	//			WAIT_TIMEOUT： 超时(时间可以调用事件函数SetTimeout()设定，默认为INFINITE，单位为毫秒)
	**********************************************************************************/
	int  WaitEvent(CEvt *pEvt, BOOL bState = TRUE, int nMsgID = MSG_NOT_NEED);

	int  WaitMultipleInputs(CInput **ppInputArray, BOOL *bStateArray, int nNums, BOOL bWaitMode = WAIT_ALL_ON, int nMsgID = MSG_NOT_NEED);
	int  WaitResetEvent(CEvt *pEvt, BOOL bState = TRUE, int nMsgID = MSG_NOT_NEED);
	
	// 不推荐使用
	BOOL MoveMotor(CMotor *pMotor, int loc, int nSubIndex = 0, double dOffset = 0.0);
	BOOL MoveMotors(CMotor **ppMotorArray, int *pLocArray, int nMotors);
	BOOL MoveMotorPos(CMotor *pMotor, double dPos, int setloc = -1, double svDrange = 0.0, double svARange = 0.0);
	BOOL MoveMotorsPos(CMotor **ppMotorArray, double *pdlPos, int nMotors, int *pSetlocs = NULL);
	BOOL JogMotor(CMotor *pMotor, double dDist, int setloc = -1);

	void Suspend() { m_bSuspended = TRUE; }
	void ClearMessage(int nMsgID = -1);

	int GetMode();
	int	GetID()		{ return m_nRunID; }
	BOOL IsJam()		{ return m_bJam; }
	void Enable(BOOL bEnable) { m_bEnabled = bEnable; }

protected:
	///////////////////////////////////
	//内部函数
	UINT RunThread();
	void RunPowerUpMode();
	void RunAutoMode();

	void PowerUp();
	void Run();
	void Stop();
	void Restart();

	void ResetPowerUpComplete(void);
	void OnJamReset(void);
	void SetMsgProcessWnd(HWND hwnd);
	void Resume() { m_bSuspended = FALSE; ClearMessage(); }

	BOOL IsInPowerUpMode() { return m_nMode == RUN_MODE_POWER_UP; }
	BOOL IsInRunMode() { return m_nMode == RUN_MODE_AUTO; }
	BOOL IsSuspended() { return m_bSuspended; }
	
protected:
	int	m_nNextAutoStep;			//自动生产流程步骤标识变量，用于标识当前自动流程步骤
	int	m_nNextPowerUpStep;			//设备初始化流程步骤标识变量，用于标识当前初始化流程步骤
	int	m_nMode;					//运行模式（带料、带料盘、完全空运行）
	int	m_eRunStatus;				//运行状态，参考RUN_STATE枚举类型
	int	m_nRunID;					//

	BOOL m_bEnabled;				//模块是否启用
	BOOL m_bJam;					//是否告警
	BOOL m_bPowerUpComplete;		//初始化是否完成
	BOOL m_bStepMode;				//

	HWND m_hwndMainApp;				// 该句柄内部使用，用于向主窗体发送系统消息

	CEvt m_evtMcStop;				//
	CEvt m_evtEol;					//

	CArray<int, int> m_lstMsg;		//
};