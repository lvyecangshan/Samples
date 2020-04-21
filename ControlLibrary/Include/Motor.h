/*******************************************************************
文件名：Motor.h
作者：              版本：A1        日期：
功能描述：控制平台伺服电机功能类
历史修改记录：
*******************************************************************/
#pragma once
#include <vector>
using namespace std;

//includes
#include <afxmt.h>
#include "Input.h"

// invalid motor location
#define INVALID_MOTOR_LOC		-1

enum MOTOR_SPEED_MODE
{
    MOTOR_FAST_SPEED,
    MOTOR_MEDIUM_SPEED,
    MOTOR_SLOW_SPEED,
    NUM_OF_MOTOR_SPEED_MODES,
    MOTOR_AUTO_SPEED = NUM_OF_MOTOR_SPEED_MODES,
};

struct sMotorLocation
{
	CString m_strName;
    double  m_dPosition;
	int		m_numSubLocation;
	double	m_dSubLocationPitch;
	BOOL    m_bEnableEdit;
};

struct sAxisOffset
{
	sAxisOffset();
	double m_dStartPoint;
	double m_dStepSpace;
	int    m_nPointNums;
	vector<double> m_ptOffset;
	sAxisOffset&  operator = (const sAxisOffset &offsetAxis);
	BOOL LoadOffset(int nAxis);
	BOOL SaveOffset(int nAxis);
};

/////////////////////////////////////////////////////////////////////////////////
typedef int (*FUNMOTOR_EXT_HOME)(void); //return info reference M_CODE

class CDeviceManager;
class CCPModuleMotorPage;

class  CMotor
{
	friend CDeviceManager;
	friend CCPModuleMotorPage;

public:
	CMotor();
	~CMotor();

public:

	/*********************************************************************************
	//函数描述：轴复位
	//输入参数：无
	//函数返回: M_OK表示成功，其他参考M_CODE
	**********************************************************************************/
	__declspec(dllexport) int Home();//Input output only used for home by IO;

	/*********************************************************************************
	//函数描述：轴运行到指定位置
	//输入参数：dest_loc	目的位置号（LibrarySupportTool.exe工具位置号）
	//			sub_loc		子位置编号，在一个位置有多个子位置时使用. 比如用在弹夹取料的情形，只需设定第一个点位值，其余位置均可自动算出。
	//			Offset		指定位置的补偿值.
    //          speedMode   移动速度模式，未指定将自动根据位移长度指定速度模式. 
	//函数返回: TRUE：运行成功  FALSE：运行失败
	**********************************************************************************/
	__declspec(dllexport) BOOL MoveLoc(int dest_loc, int sub_loc = 0, double offset = 0.0, int speedMode = -1);

	/*********************************************************************************
	//函数描述：轴T型加减速绝对移动到指定位置
	//输入参数：dest_pos	目的位置值。
    //          loc			指定移动后的目的位置号，未指定将自动根据之前的结果分配位置号。
    //          speedMode   速度模式，未指定将自动根据位移长度指定速度模式. 
	//函数返回: TRUE：运行成功  FALSE：运行失败
	**********************************************************************************/
	__declspec(dllexport) BOOL TMoveAbs(double dest_pos, int loc = INVALID_MOTOR_LOC, int speedMode = -1); 

	/*********************************************************************************
	//函数描述：轴T型加减速相对移动指定距离
	//输入参数：dist		移动距离值。
    //          loc			指定移动后的目的位置号，未指定将自动根据之前的结果分配位置号。
    //          speedMode   速度模式，未指定将自动根据位移长度指定速度模式. 
	//函数返回: TRUE：运行成功  FALSE：运行失败
	**********************************************************************************/
	__declspec(dllexport) BOOL TMoveRel(double dist, int loc = INVALID_MOTOR_LOC, int speedMode = -1);

	/*********************************************************************************
	//函数描述：轴S型加减速绝对移动到指定位置
	//输入参数：dest_pos	目的位置值。
	//			svARange	加速范围，单位为轴的指定单位mm或deg。
    //          svDrange	减速范围，单位为轴的指定单位mm或deg。
    //          loc			指定移动后的目的位置号，未指定将自动根据之前的结果分配位置号。
    //          speedMode   速度模式，未指定将自动根据位移长度指定速度模式. 
	//函数返回: TRUE：运行成功  FALSE：运行失败
	**********************************************************************************/
	__declspec(dllexport) BOOL SMoveAbs(double dest_pos, double svARange, double svDrange, int loc = INVALID_MOTOR_LOC, int speedMode = -1); 

	/*********************************************************************************
	//函数描述：轴S型加减速相对移动指定距离
	//输入参数：dist		移动距离值。
	//			svARange	加速范围，单位为轴的指定单位mm或deg。
    //          svDrange	减速范围，单位为轴的指定单位mm或deg。
    //          loc			指定移动后的目的位置号，未指定将自动根据之前的结果分配位置号。
    //          speedMode   速度模式，未指定将自动根据位移长度指定速度模式. 
	//函数返回: TRUE：运行成功  FALSE：运行失败
	**********************************************************************************/
	__declspec(dllexport) BOOL SMoveRel(double dist,  double svARange, double svDrange, int loc = INVALID_MOTOR_LOC, int speedMode = -1);

	/*********************************************************************************
	//函数描述：轴按指定方向连续移动
	//输入参数：bPositive	移动方向。
    //          nSpeedMode  速度模式，未指定将以归零速度执行. 
	//函数返回: TRUE：运行成功  FALSE：运行失败
	**********************************************************************************/
	__declspec(dllexport) BOOL StartMove(BOOL bPositive, int nSpeedMode = -1);

	/*********************************************************************************
	//函数描述：等待当前轴运行结束
	//输入参数：timeout		超时时间
    //          pInExtAlm	外部报警输入点，在等待中如看到此信号为ON,立即返回并发出报警信息
    //          bState      外部报警点状态
	//函数返回: 成功返回 M_STOPPED，其他参考M_CODE
	**********************************************************************************/
	__declspec(dllexport) int  WaitMotionDone(DWORD timeout = INFINITE, CInput* pInExtAlm = NULL, BOOL bState = TRUE);

	/*********************************************************************************
	//函数描述：获取当前轴运行状态
	//输入参数：无
	//函数返回: 参考M_CODE
	**********************************************************************************/
	__declspec(dllexport) int  GetMotionStatus();

	/*********************************************************************************
	//函数描述：轴停止运行
	//输入参数：tdec   轴减速时间
	//函数返回: TRUE：运行成功  FALSE：运行失败
	**********************************************************************************/
	__declspec(dllexport) BOOL Stop(double tdec = 0.1);

	/*********************************************************************************
	//函数描述：清掉轴归零完成信号标志
	//输入参数：无
	//函数返回: 无
	**********************************************************************************/
	__declspec(dllexport) void ResetOriRefKnownFlag() { m_bOriRefIsKnown = FALSE; }

	/*********************************************************************************
	//函数描述：获取轴归零完成标志状态
	//输入参数：无
	//函数返回: TRUE：轴已完成初始归零  FALSE：轴未完成初始归零
	**********************************************************************************/
	__declspec(dllexport) BOOL IsOriRefKnown() { return m_bOriRefIsKnown; }

	/*********************************************************************************
	//函数描述：设置轴使能状态
	//输入参数：on_off  TRUE：使能  FALSE：禁止/关闭
	//函数返回: TRUE：使能完成  FALSE：使能失败
	**********************************************************************************/
	__declspec(dllexport) BOOL SetSvOn(BOOL on_off);

	// IO related
	__declspec(dllexport) BOOL IOStatus(enum MOTOR_IO which_io);
	__declspec(dllexport) BOOL Save(int nIndex);

	// location related
    __declspec(dllexport) sMotorLocation & GetLocation(int loc);
	__declspec(dllexport) int GetCurLoc() { return m_nCurLoc; }
	__declspec(dllexport) int GetLocations(){return m_numLocations;}

	// 
	__declspec(dllexport) BOOL    IsRotaryType(){return m_nMoveType == MOVE_ROTARY;}
	__declspec(dllexport) int     GetMotorType(){return m_nMotorType;}
	__declspec(dllexport) double  GetPulsePerUint(){return m_dPulsePerUint;}
	__declspec(dllexport) double  GetVirtualHome(){return m_dVirtualHome;}
	__declspec(dllexport) double  GetHomeGoBackDist(){return m_dHomeGoBackDist;}
	__declspec(dllexport) void    SetHomeByExtFun(FUNMOTOR_EXT_HOME pHomeFun){m_pFunExtHome = pHomeFun;}

	// position related
	__declspec(dllexport) double GetCurPos();
	__declspec(dllexport) double GetCmdPos();
	__declspec(dllexport) double GetPosErr();
    __declspec(dllexport) double GetCurSpeed();
	__declspec(dllexport) BOOL   ChangeCurSpeed(double dSpeed);
	__declspec(dllexport) void   SetCurLoc(int loc);
	__declspec(dllexport) BOOL   SetCurPos(double dPos);

public:
    int	    m_nIndex;			// MotorX.cfg索引编号
	int		m_nSpeedMode;		// 轴速度模式
	CString m_strName;			// 轴名称
	CString	m_strForwardName;	// 轴第一按钮名称
	CString m_strBackwardName;	// 轴第二按钮名称
	DWORD   m_tHomeTimeout;		// 轴回原超时时间
    DWORD   m_tMoveTimeout;		// 轴移动超时时间

	double	m_dTolerance;								//used check if move pos error
	double	m_dStartup[NUM_OF_MOTOR_SPEED_MODES];		// start speed, in pulse/s, must be double
	double	m_dCruise[NUM_OF_MOTOR_SPEED_MODES];		// max speed, in pulse/s, must be double
	double	m_dTAcc[NUM_OF_MOTOR_SPEED_MODES];			// acceleration	time
	double	m_dTdec[NUM_OF_MOTOR_SPEED_MODES];			// deceleration time
	double  m_dAloneSpeedRatio;

	BOOL	m_bEnablePosOffsetParam;		//used to enable show dialog for offset or limit parameter
    static double  sm_dAllSpeedRatio;		// the whole machine speed ratio
	sAxisOffset m_sAxisOffset;
	vector<sMotorLocation> m_pLocations;

	FUNMOTOR_EXT_HOME m_pFunExtHome;		// only used in HOME_BY_EXT

protected:
	int		m_nAxisNo;		//实际对应硬件轴号
	int		m_nMotorType;	//电机类型：伺服或步进
	int		m_nHomeDir;		//轴回原点方向
	int     m_nHomeMode;	//轴回原点模式
	int 	m_nMoveType;	//轴移动类型
	int		m_nPlsOpMode;	//驱动模式 OUT/DIR 或 CW/CCW
	int		m_nEncoderType;	//编码类型, 1xAB, 2xAB, 4xAB, CW/CCW
	BOOL	m_bALM;			//轴报警信号
	BOOL	m_bINP;			//轴到位信号
	BOOL	m_bEL;			//轴限位信号
	BOOL	m_bORG;			//轴原点信号
	BOOL	m_bRDY;			//轴准备好信号
	BOOL	m_bSON;			//轴使能信号

	// the speed ratio, which is used for machine speed tuning (normally value between 0 ~ 1.0)
	double  m_dVirtualHome;
	double  m_dHomeGoBackDist;		// leave steps for homing
	double  m_dMinStep;				//
	double	m_dPulsePerUint;		//1mm or 1deg
	double	m_dEncPulsePerUint;		//1mm or 1deg

	// common used
	double m_dSoftLimitP;			//reserve
	double m_dSoftLimitN;			//reserve
	double m_dBackLash;				//only used in card has this function
	double m_dForwardOffset;
	double m_dBackwardOffset;
    int    m_nFromLib;				// only used in dll

	// option only used for home by IO mode
	int    m_nHomeOut;				// only used in HOME_BY_IO
	int    m_nHomeIn;				// only used in HOME_BY_IO

protected:
	//////////////////////////////////////////////////
	//内部函数
	int DetermineSpeedMode(double dDist);
	int HomeByLimit();
	int HomeByOrg();
	int HomeByIO();

protected:
	//////////////////////////////////////////////////
	//内部变量
	int		m_nCurLoc;
	int		m_nSavedLoc;
	int		m_numLocations;
	BOOL	m_bOriRefIsKnown;
	BOOL    m_bMotionBreak;

protected:
	//////////////////////////////////////////////////
	//内部函数
	BOOL    ApplyAxisConfig();
	BOOL    IsSavedLocChanged();
	BOOL    ReadConfig(int nIndex);
	int     GetSavedLoc();
	void    SaveLoc();
	void    MotionBreak(BOOL bBreak = TRUE);// break out in wait motion done
	static  BOOL ConfigModule(int nTotalMotor);
	double  CalcOffset(double destPos, double curPos);
};