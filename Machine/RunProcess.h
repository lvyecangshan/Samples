#pragma once
#include <RunEx.h>
#include <easyautocontrolexdef.h>
#include "Pallet.h"
//#include "StorageFile.h"

#include "ReportFile.h"  //验收数据统计
#define CT_WAITSTEP_TOTAL 20 //单模组里 等待的case总数, 
#define STEP_TOTAL 255       //单模组里 所有case总数    

class CRunProcess : public CRunEx
{

public:
	//函数描述：打开I/O
	//输入参数：I/O索引
	//函数返回: TRUE表示打开成功，FALSE表示打开失败
	BOOL APP_OUTPUTS_On(int index);
	//===================================================
	//函数描述：闭关I/O
	//输入参数：I/O索引
	//函数返回: TRUE表示打开成功，FALSE表示打开失败
	BOOL APP_OUTPUTS_Off(int index);
	//===================================================
	//函数描述：判断输出I/O是否打开
	//输入参数：I/O索引
	//函数返回: TRUE表示打开成功，FALSE表示打开失败
	BOOL APP_OUTPUTS_IsOn(int index);
	//===================================================
	//函数描述：判断输出I/O是否关闭
	//输入参数：I/O索引
	//函数返回: TRUE表示打开成功，FALSE表示打开失败
	BOOL APP_OUTPUTS_IsOff(int index);
	//===================================================
	//函数描述：判断输入I/O是否打开
	//输入参数：I/O索引
	//函数返回: TRUE表示打开，FALSE表示关闭
	BOOL APP_INPUTS_IsOn(int index);
	//===================================================
	//函数描述：判断输入I/O是否关闭
	//输入参数：I/O索引
	//函数返回: TRUE表示关闭，FALSE表示打开
	BOOL APP_INPUTS_IsOff(int index);
	//===================================================	
	//函数描述：操作单个轴（电机）移动
	//输入参数：nProID 产品编号，一般默认为 GetCurPorductID(MACHINE.m_nDefaultProduct)
	//                  nMotorID  轴（电机）索引编号
	//                  nPos  位置索引编号
	//                  dlOffset 位置偏移，单位：毫米
	//函数返回: TRUE表示执行成功，FALSE表示执行失败
	BOOL APP_MotorMove( int nMotorID, int nPos, double dlOffset = 0.0);
	//===================================================
	//函数描述：检查输入I/O的状态
	// 注意：检查单输入点报警用，避免用此函数做循环状态判断（如需循环判断可直接用Input本身的函数），因每次调用会产生相应日志记录
	//输入参数：pInput  输入I/O指针
	//  bState  输入I/O应有状态，一般使用ON或OFF表示，而不用TRUE或FALSE
	// nMsgID  如不是想要的bState状态时输出异常信息(同ShowMessage())，如 未指定则做默认信息处理. 
	//函数返回: TRUE：检查结果和bState相同  FALSE：检查结果和bState不同
	BOOL APP_CheckInput(int index, BOOL bState, int nMsgID = MSG_SHOW_DEFAULT);
	//===================================================
	//函数描述：等待输入I/O的状态置换成指定状态
	//输入参数：pInput  输入I/O指针
	// bState  输入I/O应有状态，一般使用ON或OFF表示，而不用TRUE或FALSE
	// nMsgID  如不是想要的bState状态时输出异常信息(同ShowMessage())，如 未指定则做默认信息处理。（没输此参数则不做信息处理）
	//   MSG_SHOW_DEFAULT 为默认，MSG_NOT_NEED 为不需要发出错误信息的情况，大于MSG_NOT_NEED值为自定义消息ID,自定义消息ID需要注意，必须要大于easylibdef.h文件内LIBRARY_MSG_ID定义的数值
	//函数返回: WAIT_OK： 检测结果与bState相同
	// WAIT_ERROR：超时(时间设定根据Input.cfg)
	// WAIT_STOP： 被停止
	int  APP_WaitInput(int index, BOOL bState, int nMsgID = MSG_NOT_NEED);
	//===================================================
	//函数描述：增加一个I/O输入
	//输入参数：pVaule  I/O索引
	//                 strKey  I/O在配置文件中对应的名称
	//函数返回: TRUE表示添加成功，FALSE表示添加失败
	void APP_Add_Input(int* pVaule,CString strKey);
	//===================================================
	//函数描述：增加一个电机
	//输入参数：pVaule 电机索引
	//                  strKey  电机在配置文件中对应的名称
	//函数返回: TRUE表示添加成功，FALSE表示添加失败
	void APP_Add_Motor(int* pVaule,CString strKey);
	//===================================================
	//函数描述：增加一个I/O输出
	//输入参数：pVaule  I/O索引
	//                strKey  I/O在配置文件中对应的名称
	//函数返回: TRUE表示添加成功，FALSE表示添加失败
	void APP_Add_Output(int* pVaule,CString strKey);
	//===================================================
	void APP_Add_Map(map<int*,CString> &Map,int* pVaule,CString strKey);
	//===================================================
	void APP_Add_Map(map<void*,CString> &Map,int* pVaule,CString strKey);
	//===================================================
	void APP_Add_Map(map<void*,CString> &Map,CString* pVaule,CString strKey);
	//===================================================
	//空运行调用夹爪动作后延时
	void APP_Finger_Sleep();
	//===================================================
	//无硬件运行时延时
	void APP_NoHardware_Sleep();
	//===================================================
	//等待信号延时
	void APP_Wait_Signal_Sleep();	
	//===================================================
	void APP_MessageBox(CString strMsg);
	//===================================================
	//函数描述：消息提示框
	//输入参数： strMsg 要显示的消息
	//           nType  提示图标类型
	//函数返回: 无
	//函数返回: IDABORT		中止按钮
	//			IDCANCEL	退出按钮
	//			IDIGNORE	忽略按钮
	//			IDNO		NO按钮
	//			IDOK		OK按钮
	//			IDRETRY		重试按钮
	//			IDYES		YES按钮
	int APP_BLMessageBox(LPCTSTR lpszMessage, UINT nType = MB_OK | MB_ICONEXCLAMATION);
	//===================================================
	//函数描述：阻塞线程消息提示框
	//输入参数： strMsg 要显示的消息
	//           nType		提示类型 0：普通提示（灰色框） 1：问答提示（蓝色框） 2：普通预警（黄色框） 3：故障报警（红色框）
	//函数返回: 无
	//调用此函数时，会在提示的前面自动加上模组名称，strMsg里面不要再加模组名称
	void APP_ShowMessage(LPCTSTR lpszMessage, UINT nType = MB_OK | MB_ICONEXCLAMATION);

public:
	virtual BOOL SetNeedSingal(BOOL bNeedPlate);
	virtual BOOL GetNeedSingal(); 
	virtual BOOL SetFeedSingal(BOOL bFeedPlate);
	virtual BOOL GetFeedSingal();                 //获取上料完成信号
	virtual BOOL GetNextNeedSingal();              //获取下一模组要料信号

	//线体电机启停
	virtual void MotorRun(void){return;}   //chen_2019.7.16

	//机械手夹爪防撞
	virtual void Anticollision(void){return;}   //chen_2019.7.17

protected:
	BOOL InitIO();

protected:
	BOOL               m_bNeedPlate; //需要料盘
	BOOL               m_bFeedPlate; //料盘准备好
	int                m_nNextAutoStepEx;  
	BOOL               m_bShowWaitFeedSingal;


protected:
	enum POWER_UP_STEPS
	{
		PU_FINISH,
		NUM_OF_POWER_UP_STEPS,
	};

	enum AUTO_STEPS
	{
		AUTO_WAIT_WORK_START,
		AUTO_WORK_END,
	};


protected:
	virtual void PowerUpOperation(void);

	virtual void PowerUpRestart(void);

	virtual void AutoOperation(void);

protected:

public:
	CRunProcess(int nID);

	virtual ~CRunProcess(void);

	// load from ini file the configurations
	virtual BOOL InitializeConfig(CString strProcessModule);

	// show the tools dialog, return true means need to update display
	virtual BOOL ShowToolsDlg(void);

	virtual BOOL MotorHandMove( CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove );

	// check motor can move
	virtual BOOL CheckMotorCanMove(CMotor *pMotor, int iLoc, double lfValue, BOOL bAbsMove); 

	// check output can active
	virtual BOOL CheckOutputCanActive(COutput *pOutput);

	// load package data
	virtual BOOL LoadPkgParam(CString strPathName) { return TRUE; }

	// save package data
	virtual BOOL SavePkgParam(CString strPathName) { return TRUE; }

	// get the hardwares
	virtual void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);

	virtual BOOL UserPremissionProtect(USER_PERMISSION_PROTECT_TYPE uprType = USER_PERMISSION_MOTOR_POS_SAVE);

	BOOL ModifyPos(int nPkg,int nMotor,CMotorPos &mPos);
public:

	char* CStringTochar(CString &strResource);

	BOOL CreateFilePathDirectory(CString strFilePath);			//创建指定文件夹

	BOOL SetParameterEnable(CString strParameter, int nParamLevel);

	virtual BOOL ReadParameter();

	virtual BOOL SaveParameter();

	virtual BOOL ReadRunParameter();

	virtual BOOL SaveStepInfo(int nWriteType);

	virtual void ReadStepInfo();

	virtual void CloseAction();

	virtual void StationEStop();

	virtual BOOL CheckInOutputStatus(BOOL bInput, int nInOutput,BOOL bStatus, BOOL bAlarm = FALSE);

	void MeasureCT(CString strModuleName, CString strStepName, int nStepNum, int nStartStep, int nEndStep, int* waitStep);
    void WriteLog(CString strText);

	public:
		void WriteCTLog(CString strText); //用于写CT日志的函数
		int m_nSaveLastStep;   //用于记录上一次的步骤
		DWORD m_nStepStartTime;   //记录每一步的开始时间
		DWORD m_nTotalTime;   //总时间

public:
	CString m_strModuleName;
	BOOL m_bModuleResetFinish;
	int m_nGroupID;	// 同类不同模组分类
	BOOL m_bShellVaild;
    BOOL m_bReCoverMachineAuto; //自动流程带料  TRUE 带料运行开始 FALSE 带料运行结束
	
	//CT	
	CTFunction m_CTFunc;
	int m_waitStep[CT_WAITSTEP_TOTAL];
	actionClass m_action[STEP_TOTAL];

	//CT	
	DWORD m_dwModuleCT;
	DWORD m_dwModuleStrt;
	DWORD m_dwModuleStepCT;	
	unsigned int m_nAutoNext;
	CString m_strModuleCTtemp;
	
public:
	CPallet m_Pallet;

	vector<PARAMETER_EN> m_vecParameterEN;
	CCriticalSection m_ParameterLock;
	CCriticalSection m_SingleLock;
};