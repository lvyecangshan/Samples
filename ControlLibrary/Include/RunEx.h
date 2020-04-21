/*******************************************************************
文件名：RunEx.h
作者：              版本：A1        日期：
功能描述：控制平台运行基类的扩展类
历史修改记录：
*******************************************************************/
#pragma once
#include "run.h"
#include "EasyAutoControlExDef.h"
#include "DataBaseRecord.h"
using namespace std;
#include <map>

#define RUN_PROCESS_DLG_TIMER 110

class __declspec(dllexport) CRunEx : public CRun
{
protected:
	enum POWER_UP_STEPS
	{
		PU_FINISH,					//初始化完成流程
		NUM_OF_POWER_UP_STEPS,		//初始化流程总数
	};

	enum AUTO_STEPS
	{
		AUTO_WAIT_WORK_START,		//等待开始工作信号流程
		AUTO_WORK_END,				//工作完成流程
	};

protected:
	/*********************************************************************************
	//函数描述：设备初始化流程函数
	//输入参数：无
	//函数返回: 无
	**********************************************************************************/
	virtual void PowerUpOperation(void);

	/*********************************************************************************
	//函数描述：自动生产流程函数
	//输入参数：无
	//函数返回: 无
	**********************************************************************************/
	virtual void AutoOperation(void);

	/*********************************************************************************
	//函数描述：整机复位响应函数,通常在用于做变量初始化处理
	//输入参数：无
	//函数返回: 无
	**********************************************************************************/
	virtual void PowerUpRestart(void);

public:
	CRunEx(int nID);
	virtual ~CRunEx(void);

	/*********************************************************************************
	//函数描述：加载配置文件，初始化配置
	//输入参数：strProcessModule		模块名称
	//函数返回: TRUE：初始化成功，FALSE：初始化失败
	**********************************************************************************/
	virtual BOOL InitializeConfig(CString strProcessModule);

	/*********************************************************************************
	//函数描述：保存配置到ini文件
	//输入参数：无
	//函数返回: TRUE：保存成功，FALSE：保存失败
	**********************************************************************************/
	virtual BOOL SaveConfig(void);

	/*********************************************************************************
	//函数描述：获取硬件配置：加载输入I/O、输出I/O、伺服电机
	//输入参数：pInputs		输入I/O的容器指针
	//			pOutputs	输出I/O的容器指针
	//			pMotors		伺服电机的容器指针
	//函数返回: 无
	**********************************************************************************/
	virtual void GetHardwareConfig(CArray<int,int> *pInputs, CArray<int,int> *pOutputs, CArray<int,int> *pMotors);

	/*********************************************************************************
	//函数描述：检查伺服电机能否操作
	//输入参数：pMotor		某一伺服电机对应的指针
	//函数返回: TRUE表示允许操作，FALSE表示禁止操作
	**********************************************************************************/
	virtual BOOL CheckMotorCanMove(CMotor *pMotor){ return TRUE; } 

	/*********************************************************************************
	//函数描述：检查输出I/O能否操作
	//输入参数：pOutput		某一输出I/O对应的指针
	//函数返回: TRUE表示允许操作，FALSE表示禁止操作
	**********************************************************************************/
	virtual BOOL CheckOutputCanActive(COutput *pOutput){ return TRUE; }
	
 public:
	int m_nRunMode;				// 运行模式
	BOOL m_bDryRun;				//空运行标识变量

	CString m_strClass;			//类名称
	CString m_strProcess;		//模块名称

	void ShowMessageEX(CString strMsg, int nType = 0, CString strSn = _T("SN"), CDataBaseRecord *pDB = NULL);
	CString m_strAlarmMsg;

public:
	CString m_strMsgEng;		//英文提示信息
	CString m_strMsgChs;		//中文提示信息
	BOOL m_bLangulageEng;		//是否英文语言
	CString m_strMsgEx;

public:
	BOOL IsJamed(){return m_bJam;}				//是否故障

   	virtual BOOL ReadParameter(){ return TRUE; }		//读取参数函数
	virtual BOOL SaveParameter(){ return TRUE; }		//保存参数函数
	virtual BOOL ReadRunParameter(){ return TRUE; }		//读取运行参数函数

	map<int*,CString> m_InputMap;		//输入I/O映射表，使用insert方法插入所需I/O
	map<int*,CString> m_OutputMap;		//输出I/O映射表，使用insert方法插入所需I/O
	map<int*,CString> m_MotorMap;		//伺服电机映射表，使用insert方法插入所需伺服电机
	map<void*,CString> m_VoidMap;		//变量映射表，使用insert方法插入所需变量

	CRunEx *m_pFather;					//父模块指针
	CRunEx *m_pChild;					//子模块指针
	CRunEx *m_pBrother;					//兄弟模块指针

	CRunEx *m_pNormalFront;				//前一模块指针
	CRunEx *m_pBranchFront;				//前一模块的分支模块指针
	CRunEx *m_pNormalNext;				//下一模块指针
	CRunEx *m_pBranchNext;				//下一模块的分支模块指针

	CString m_strParent;				//父模块名称
	CString m_strChild;					//子模块名称
	CString m_strNormalFront;			//前一模块名称
	CString m_strBranchFront;			//前一模块的分支模块名称
	CString m_strNormalNext;			//下一模块名称
	CString m_strBranchNext;			//下一模块的分支模块名称

	BOOL m_bRequirePallet;				//请求料盘（原料）信号
	BOOL m_bNextRequire;				//发送给下一个工位的请求信号
	BOOL m_bForNextReady;				//发送给下一个工位的准备好信号
	BOOL m_bBranchRequire;				//发送给分支工位的请求信号

	BOOL m_bNormalOpen;					//分支流程打开信号

	DWORD m_dwStartTime;				//模块开始时间

	//预留
	BOOL m_bChannelOpen;
	BOOL m_bBranchOpen;
	
	//预留
	BOOL m_bPalletFormNarmal;
	BOOL m_bPalletFormBranch;

	virtual BOOL ModifyPos(int nPkg, int nMotor, CMotorPos& mPos){return TRUE;}
	virtual void UpdateLanguage();
	BOOL IsSuspended(){return m_bSuspended;}
	BOOL IsRunning();

public:

	/*********************************************************************************
	//函数描述：操作单个轴（电机）移动
	//输入参数：nProID	产品编号，一般默认为 GetCurPorductID(MACHINE.m_nDefaultProduct)
	//			nMotorID  轴（电机）索引编号
	//			nPos  位置索引编号
	//			dlOffset 位置偏移，单位：毫米
	//函数返回: TRUE表示执行成功，FALSE表示执行失败
	**********************************************************************************/
	BOOL MotorMove(int nProID, int nMotorID, int nPos, double dlOffset = 0.0);

	/*********************************************************************************
	//函数描述：操作多个轴（电机）移动
	//输入参数：nProID	产品编号，一般默认为 GetCurPorductID(MACHINE.m_nDefaultProduct)
	//			nMotorID  轴（电机）索引编号数组
	//			nPos  位置索引编号数组
	//			nNum  操作的轴（电机）总数
	//函数返回: TRUE表示执行成功，FALSE表示执行失败
	**********************************************************************************/
	BOOL MotorsMove(int nProID, int* nMotorID, int* nPos, int nNum);

	/*********************************************************************************
	//函数描述：操作多个轴（电机）移动
	//输入参数：nProID	产品编号，一般默认为 GetCurPorductID(MACHINE.m_nDefaultProduct)
	//			nMotorID  轴（电机）索引编号数组
	//			nPos  位置索引编号数组
	//			dlOffset 位置偏移，单位：毫米
	//			nNum  操作的轴（电机）总数
	//函数返回: TRUE表示执行成功，FALSE表示执行失败
	**********************************************************************************/
	BOOL MotorsMove(int nProID,int* nMotorID, int* nPos, double* dlOffsetPos, int nNum);

	/*********************************************************************************
	//函数描述：操作多个轴（电机）复位
	//输入参数：nMotorID  轴（电机）索引编号数组
	//			nNum  操作的轴（电机）总数
	//函数返回: TRUE表示执行成功，FALSE表示执行失败
	**********************************************************************************/
	BOOL MotorsHome(int* nMotorID, int nNum);

	/*********************************************************************************
	//函数描述：获取对应轴（电机）指定位置索引的具体位置值
	//输入参数：nProID	产品编号，一般默认为 GetCurPorductID(MACHINE.m_nDefaultProduct)
	//			nMotorID  轴（电机）索引编号数组
	//			nPos  位置索引编号数组
	//函数返回: 具体位置值(浮点型)
	**********************************************************************************/
	double GetMotorPos(int nProID, int nMotorID, int nPos);

	/*********************************************************************************
	//函数描述：获取指定索引号产品的编号
	//输入参数：nProductIndex	产品索引号，一般使用默认，即变量：MACHINE.m_nDefaultProduct
	//函数返回: 产品编号
	**********************************************************************************/
	int GetCurPorductID(int nProductIndex);

	double		m_dlModuleUseTime;				//模块当前运行时间
	double		m_dlModuleTragetTime;			//模块目标运行时间


	/////////////////// 以下使用频率均较低
	virtual BOOL UserPremissionProtect(USER_PERMISSION_PROTECT_TYPE uprType = USER_PERMISSION_MOTOR_POS_SAVE);
	void SetNextStep(int nNextStep, CString strSn = _T("SN"), CDataBaseRecord *pDB = NULL, BOOL bWrite = FALSE);
	void ModuleStartTime();
	void ModuleEndTime(CString strSn = _T("SN"), CDataBaseRecord *pDB = NULL, BOOL bWrite = FALSE);
	void AddYieldRecord(int nType, CString strInfo, int nNum, CString strSn = _T("SN"), CDataBaseRecord *pDB = NULL, BOOL bWrite = FALSE);

	CString		m_strModuleStartTime;
	CString		m_strModuleEndTime;
	SYSTEMTIME	m_stModeleStartTime;
	SYSTEMTIME	m_stModeleEndTime;
	BOOL		m_bModuleTimeEnable;

	CString		m_strStepStartTime;
	CString		m_strStepEndTime;
	SYSTEMTIME	m_stStepStartTime;
	SYSTEMTIME	m_stStepEndTime;
	double		m_dlStepUseTime;
	BOOL		m_bStepTimeEnable;
	SYSTEMTIME	m_stCurTime;

	vector<CProduct> m_vecProduct;

	//不推荐使用
	BOOL Motors3Move(int nProID, int* nMotorID, int* nPos);

	//不推荐使用
	BOOL Motors2Move(int nProID, int* nMotorID, int* nPos);
};
