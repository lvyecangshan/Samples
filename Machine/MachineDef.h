#pragma once
#include "EasyAutoControlExDef.h"
#include <map>

// 消息定义
#define WM_SHOW_MESSAGE_EX		WM_USER + 102
#define WM_SHOW_ALARM_MESSAGE	WM_USER + 205
#define WM_COMM_INFO_MESSAGE    WM_USER + 112

#define WM_USER_LOGINOUT		WM_USER + 111
#define WM_OUTPUT_VIEW_MSG		WM_USER	+ 113

// Data文件夹路径
#define MODULE_CT_FOLDER	_T("Data\\ModuleCT")
#define MODULE_RUN_FOLDER  _T("Data\\RunStepInfo\\")

#define RUN_DATA_CFG		_T("Data\\RunData.cfg")
#define DATA_MACHINE		_T("Data\\Machine.cfg")


// Log文件夹路径
#define MSG_HISTROY_XML			_T("Log\\Message History.xml")
#define USER_HISTORY_XML		_T("Log\\User History.xml")
#define CUSTOMER_TRACE_LOG		_T("Log\\Customer Trace\\")


// System文件夹路径
#define MODULE_PICTURE		_T("System\\ModulePicture\\")
#define MODULEEX_CFG		_T("System\\ModuleEx.cfg")
#define PARAMETER_CFG		_T("System\\Parameter.cfg")


// database
#define  DATABASE_FILE			_T("Log\\Machine.mdb")
#define  DATABASE_BACKUP_FILE		_T("Log\\Machine_备份.mdb")

#define  MES_ONLOADCOUNT 20
// enum 

// 用于登录权限登记
enum USER_LEVEL
{
	MACHINE_USER_ADMIN = 0,
	MACHINE_USER_MAINTENANCE,
	MACHINE_USER_OPERATOR,
};

enum MACHINE_EVENTS
{
	NUM_OF_EVENTS
};

enum MACHINE_TIMES
{
	NUM_OF_MACHINE_TIME,
};

enum DATA_TYPE
{
	DATA_TYPE_INT,
	DATA_TYPE_DOUBLE,
	DATA_TYPE_BOOLEAN,
	DATA_TYPE_CSTRING,
	DATA_TYPE_POINTER,
};

enum WRITE_DATA_TYPE
{
	WRITE_TYPE_AUTO_STEP = 0x01<<1,
	WRITE_TYPE_VARIABLES = 0x01<<2,
	WRITE_TYPE_BATTERY   = 0x01<<3,
};

// 电机位置
enum MOTOR_LOC
{
	NUM_OF_MOTOR_LOC
};


enum ROBOTCMD
{
	ROBOT_HIGH_SPEED = 0,
	ROBOT_MID_SPEED,
	ROBOT_LOW_SPEED,
	ROBOT_GO_HOME,
	ROBOT_ONLOAD_WAIT,
	ROBOT_ONLOAD_POS,
	ROBOT_OFFLOAD_BUFF_WAIT_POS1,
	ROBOT_OFFLOAD_BUFF_POS1,
	ROBOT_OFFLOAD_BUFF_WAIT_POS2,
	ROBOT_OFFLOAD_BUFF_POS2,
	ROBOT_OFFLOAD_WAIT_POS,
	ROBOT_OFFLOAD_POS,
	ROBOT_OFFLOAD_NG_WAIT_POS,
	ROBOT_OFFLOAD_NG_POS,
	ALL_ROBOT_POS
};

typedef struct tagROBOT_CMD
{
	unsigned int unCmdIndex;
	CString strSendCmd;
	CString strRecvCmd;
	CString strRemark;
	int nCmdLevel;
}ROBOT_CMD;

const ROBOT_CMD g_robotcmd[]=
{
	{ROBOT_HIGH_SPEED, 				_T("POWER_HIGH\r\n"), 					_T("POWER_HIGH_OK\r\n"),					_T("高速"),					0},
	{ROBOT_MID_SPEED,				_T("POWER_MID\r\n"),					_T("POWER_MID_OK\r\n"),						_T("中速"),					0},
	{ROBOT_LOW_SPEED,				_T("POWER_LOW\r\n"),					_T("POWER_LOW_OK\r\n"),						_T("低速"),					0},
	{ROBOT_GO_HOME,					_T("GO_HOME\r\n"),						_T("GO_HOME_OK\r\n"),						_T("回原点"),				0},
	{ROBOT_ONLOAD_WAIT,				_T("ONLOAD_WAIT_POS\r\n"),				_T("ONLOAD_WAIT_POS_OK\r\n"),				_T("取料等待位"),			0},
	{ROBOT_ONLOAD_POS,				_T("ONLOAD_POS\r\n"),					_T("ONLOAD_POS_OK\r\n"),					_T("取料位"),				1},
	{ROBOT_OFFLOAD_BUFF_WAIT_POS1,	_T("OFFLOAD_BUFF_ONE_WAIT_POS\r\n"),	_T("OFFLOAD_BUFF_ONE_WAIT_POS_OK\r\n"),		_T("缓存等待位#1"),			0},
	{ROBOT_OFFLOAD_BUFF_POS1,		_T("OFFLOAD_BUFF_ONE_POS\r\n"),			_T("OFFLOAD_BUFF_ONE_POS_OK\r\n"),			_T("缓存位#1"),				1},
	{ROBOT_OFFLOAD_BUFF_WAIT_POS2,	_T("OFFLOAD_BUFF_TWO_WAIT_POS\r\n"),	_T("OFFLOAD_BUFF_TWO_WAIT_POS_OK\r\n"),		_T("缓存等待位#2"),			0},
	{ROBOT_OFFLOAD_BUFF_POS2,		_T("OFFLOAD_BUFF_TWO_POS\r\n"),			_T("OFFLOAD_BUFF_TWO_POS_OK\r\n"),			_T("缓存位#2"),				1},
	{ROBOT_OFFLOAD_WAIT_POS,		_T("OFFLOAD_WAIT_POS\r\n"),				_T("OFFLOAD_WAIT_POS_OK\r\n"),				_T("下料等待位"),			0},
	{ROBOT_OFFLOAD_POS,				_T("OFFLOAD_POS\r\n"),					_T("OFFLOAD_POS_OK\r\n"),					_T("下料位"),				1},
	{ROBOT_OFFLOAD_NG_WAIT_POS,		_T("OFFLOAD_NG_WAIT_POS\r\n"),			_T("OFFLOAD_NG_WAIT_POS_OK\r\n"),			_T("Ng下料等待位"),			0},
	{ROBOT_OFFLOAD_NG_POS,			_T("OFFLOAD_NG_POS\r\n"),				_T("OFFLOAD_NG_POS_OK\r\n"),				_T("Ng下料位"),				1},
};

// 消息
enum MSG_ID
{
	MSG_NO_ACCESS_RIGHT						= 1002,
	MSG_MACHINE_STOP_BEFORE_EXIT	= 1003,
	MSG_CONFIRM_EXIT_MACHINE				= 1004,

	// 上料堆栈
	MSG_ONLOAD_STACK_CHECK_CAR_VAILD		= 2000,	//堆栈存在小车
	MSG_ONLOAD_STACK1_CHECK_CAR_AND_PALLET_ABERRANT   = 2001,
	MSG_ONLOAD_STACK2_CHECK_CAR_AND_PALLET_ABERRANT   = 2002,
	MSG_ONLOAD_STACK_PALLET_TOO_MANY		= 2003,
	MSG_ONLOAD_STACK_CHECK_CAR_NOT_REMOVE	=2004,
	MSG_ONLOAD_STACK_CHECK_CAR_STATUS_ILLEGAL	= 2005,

	// 下料堆栈
	MSG_OFFLOAD_STACK_CHECK_CAR_VAILD		= 2021,
	MSG_OFFLOAD_STACK_PROMPT_OFFLOAD_EMPTY_PALLET  = 2022,
	MSG_OFFLOAD_STACK_CHECK_CAR_NOT_INPOS   = 2023,

	// 搬运
	MSG_FEEDERTRAY_CHECK_PALLET_VAILD			= 2041,
	MSG_FEEDERTRAY_CHECK_PALLET_INVAILD		= 2042,
	MSG_FEEDERTRAY_MOTOR_MOVE_NOT_INPOS = 2043,
	MSG_FEEDERTRAY_SAFE_RASTER_CHECK_UNNOTMAL = 2044,


	// 除尘前
	MSG_DUST_BEFORE_CHECK_SHELL_DIRECTION_REVERSE		= 2081,

	//除尘
	CLEAN_SHELL_FINGER_OPEN_WITHOUT_PALLET             = 2100,//清洁模组夹爪打开，请清除铝壳
	// 电芯入壳
	MSG_BATTERY_CORE_PRESS_COMMUNICATE_FAIL		= 2181,

	// 电芯上料
	MSG_ONLOAD_CORE_PNP_PICK_COLLIDE	= 2221,


	// 机器人
	MSG_ROBOT_MOVE_TIMEOUT					= 2241,

	// 清料
	CLEAR_MATERIAL_FINISH 					= 2242,//清料完成

	MSG_FU_AIR_ALARM,    //负压报警
	MSG_AIR_ALARM,     //气压报警

	ROBOT_PRESS_ALARM,   //机器人压料报警
};


// class

// 产品模组参数在此添加
//class CPkgParamter:public CPkgPara
class CMachineParamter:public CPkgPara
{
public:
	CMachineParamter()		
	{
		m_bEnable = FALSE;
	}

	~CMachineParamter()
	{
	}

	void Init()
	{
		m_bEnable = FALSE;
	}
public:
	BOOL m_bEnable;
};


// 产品参数
//class CMachineParamter:public CPkgPara
class CPkgParamter:public CPkgPara
{
public:
	CPkgParamter()
	{
		m_bEnable = FALSE;
	}

	~CPkgParamter()
	{
	}

	void Init()
	{
		m_bEnable = FALSE;
	}
public:
	BOOL m_bEnable;
};

class CPropetyPkg
{
public:
	CPropetyPkg()
	{
		m_nPara1 = 0;
		m_nPara2 = 0;
	}
	
public:
	LONG m_nPara1;
	LONG m_nPara2;
};

typedef struct tagPARAMETER_EN 
{
	CString m_strParameter;
	int		m_nParamLevel;

	tagPARAMETER_EN()
	{
		m_strParameter.Empty();
		m_nParamLevel = 2;
	}
}PARAMETER_EN;