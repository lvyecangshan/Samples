/*******************************************************************
文件名：easylibdef.h
作者：              版本：A1        日期：
功能描述：控制平台宏定义头文件
历史修改记录：
*******************************************************************/
#pragma once

#include "windef.h"

//
// invalid run step, all run steps must be between 0 & num_of_run_steps
//
#define INVALID_RUN_STEP	-1

// WaitInput, WaitEvent等函数的返回结果
#define WAIT_ERROR	-1		//等待失败、内部错误、超时
#define WAIT_STOP	0		//被终止
#define WAIT_OK		1		//成功执行

//
// wait style, wait_any = OR, wait_all = AND
//
#define WAIT_ANY	FALSE
#define WAIT_ALL	TRUE

//
// max multi-wait input item count
// 
#define MAX_WAIT_INPUT	10

//
// commonly used equipment words linked to 0 and 1 definition
//
#define		ON			1		//开
#define		OFF			0		//关
#define		ACTIVATE	1		//活动
#define		DEACTIVATE	0		//禁用
#define		OPEN		0		//打开
#define		CLOSE		1		//关闭
#define		HIGH		1		//高
#define		LOW			0		//低

//
// system warning (bee-bee-bee buzzer) type, for user have different warning events
// user defined warning must be 0x0002, 0x0004, 0x0010, ...
//
#define		SYS_WARNING_DOOR	0x0001

// 常用的颜色
const COLORREF WHITE		= RGB(255, 255, 255);
const COLORREF BLACK		= RGB(1, 1, 1);
const COLORREF BROWN		= RGB(128, 0, 0);
const COLORREF PINK			= RGB(255, 0, 255);
const COLORREF BLUE			= RGB(0, 0, 255);
const COLORREF DARKBLUE		= RGB(0, 0, 128);
const COLORREF MIDDLE_BLUE	= RGB(0, 0, 192);
const COLORREF LTBLUE		= RGB(40, 78, 255);
const COLORREF LIGHTBLUE	= RGB(200, 251, 252);
const COLORREF CLOUDBLUE	= RGB(172, 224, 250);
const COLORREF DIRTYBLUE	= RGB(0, 22, 128);
const COLORREF BMDBLUE		= RGB(71, 73, 144);
const COLORREF GRAY			= RGB(128, 128, 128);
const COLORREF LIGHTGRAY	= RGB(192, 192, 192);
const COLORREF DARKGRAY		= RGB(128, 128, 128);
const COLORREF MIDGRAY		= RGB(176, 176, 176);
const COLORREF LTGRAY		= RGB(210, 220, 220);
const COLORREF GREEN		= RGB(0, 255, 0);
const COLORREF LIGHTGREEN	= RGB(128, 255, 128);
const COLORREF DARKGREEN	= RGB(0, 128, 0);
const COLORREF DIRTYGREEN	= RGB(0, 200, 0);
const COLORREF YELLOW		= RGB(255, 255, 0);
const COLORREF LIGHTYELLOW	= RGB(0xFF, 0xFF, 0xE0);
const COLORREF DARKYELLOW	= RGB(128, 128, 0);
const COLORREF RED			= RGB(200, 0, 0);
const COLORREF DARKRED		= RGB(128, 0, 0);
const COLORREF LTRED		= RGB(255, 0, 0);
const COLORREF CYAN			= RGB(0, 255, 255);
const COLORREF LTCYAN		= RGB(200, 255, 255);
const COLORREF DARKCYAN		= RGB(0, 128, 128);
const COLORREF MAGENTA		= RGB(255, 0, 255);
const COLORREF LTMAGENTA	= RGB(255, 128, 255);
const COLORREF DARKMAGENTA	= RGB(128, 0, 128);
const COLORREF PURPLE		= RGB(138, 19, 126);
const COLORREF LTPURPLE		= RGB(231, 78, 216);

enum COMMON_SYSTEM_INPUT
{
	SWITCH_START,
	SWITCH_STOP,
	SWITCH_RESET,
	SWITCH_DOOR,
	SWITCH_ESTOP,
	SWITCH_VACUUM,
	SWITCH_AIR,
	SWITCH_CURTAIN,
	SWITCH_ENGINEERING,
	NUM_OF_SYSTEM_INPUT,
};

enum COMMON_SYSTEM_OUTPUT
{
	SYS_OUTPUT_START_BUTTON_LED,
	SYS_OUTPUT_STOP_BUTTON_LED,
	SYS_OUTPUT_RESET_BUTTON_LED,
	SYS_OUTPUT_DOOR_LOCK,
	NUM_OF_SYSTEM_OUTPUT,
};

// motor define 
enum MOTOR_TYPE
{
	MOTOR_STEPPER = 0,
	MOTOR_STEPPER_ENC,
	MOTOR_SERVO,
	MOTOR_SERVO_REVERSE,
	NUM_MOTOR_TYPE,
};

enum HOME_MODE
{
	HOME_BY_LIMIT,
	HOME_BY_ORG,
	HOME_BY_IO,
	HOME_BY_EXT,
	NUM_OF_HOME_MODE,
};

enum MOVE_TYPE
{
	MOVE_LINE,
	MOVE_ROTARY,
	NUM_OF_MOVE_TYPE,
};

enum MOTOR_IO
{
	MOTOR_IO_RDY,
	MOTOR_IO_ALM,
	MOTOR_IO_PEL,
	MOTOR_IO_NEL,
	MOTOR_IO_ORG,
	MOTOR_IO_INP,
	MOTOR_IO_SVON,
	NUM_OF_MOTOR_IO,
};

enum M_PULSE_OUTPUT_MODE
{
	M_PLSOP_OUT_DIR,
	M_PLSOP_CW_CCW,
	M_PLSOP_AB,
};

enum M_PULSE_INPUT_MODE
{
	M_PLSIP_1XAB,
	M_PLSIP_2XAB,
	M_PLSIP_4xAB,
	M_PLSIP_CW_CCW,
};

enum SIGNAL_ONOFF
{
	M_OFF,
	M_ON,
};

// whole machine's state
enum MACHINE_STATE
{
	MC_BEGIN,
	MC_INITIALIZING,
	MC_INITIALIZED,
	MC_RUNNING,
	MC_STOP_INIT,
	MC_STOP_RUN,
	MC_INIT_ERR,
	MC_RUN_JAM,
	NUM_MC_STATE,
	MC_INVALID_STATE = -1,
};

//
// motor functions return code, note that due to history reason, M_OK = 0, M_ERROR = 1
//
enum M_CODE
{
	M_FAILED = -1,	// 失败
	M_OK = 0,		// 成功
	M_STOPPED = 0,	// 停止
	M_BUSY,			// 轴运行中
	M_ALARM,		// 轴异常告警
	M_POS_EL_HIT,	// 碰正限位
	M_NEG_EL_HIT,	// 碰负限位
	M_TIMEOUT,		// 超时
	M_BREAK,		// 停止或急停信号被触发，退出当前处于等待状态的函数
	M_SUB_ALM,		// 外部输入辅助告警
	M_OTHERS,		// 其他
	NUM_MOTION_STATUS,
};

//
// tower light and buzzer enum definition
//
#define		LIGHT_RED		0
#define		LIGHT_AMBER		1
#define		LIGHT_GREEN		2
#define		LIGHT_BUZZER	3

//
// directory definition, pre-defined path for common used files
//
#define MACHINE_TRACE_FOLDER	_T("Log\\Machine Trace\\")
#define CUSTOMER_TRACE_FOLDER	_T("Log\\Customer Trace\\")
#define LOT_HISTORY_FOLDER		_T("Log\\Lot History\\")
#define MOTOR_CFG_FOLDER		_T("System\\Motor\\")
#define HARDWARE_CFG			_T("System\\Hardware.cfg")
#define INPUT_CFG				_T("System\\Input.cfg")
#define OUTPUT_CFG				_T("System\\Output.cfg")
#define MACHINE_TIME_CFG		_T("System\\MachineTime.cfg")
#define EVENT_CFG				_T("System\\Event.cfg")
#define MODULE_CFG	            _T("System\\Module.cfg")
#define TOWER_LIGHT_CFG         _T("System\\Towerlight.cfg")
#define MACHINE_CFG				_T("Data\\Machine.cfg")
#define PACKAGE_FOLDER			_T("Data\\Package\\")
#define PACKAGE_FILE_EXT		_T(".pkg")
#define PACKAGE_MOTOR_PARAM		_T("positions.cfg")
#define MESSAGE_CFG             _T("System\\Message.cfg")
#define LANG_CFG_FOR_APP		_T("System\\Language\\App.")
#define LANG_CFG_FOR_LIBRARY	_T("System\\Language\\Lib.")
#define LOAD_STRING_DEFAULT		_T("#ERROR#")

//
// common shown message type for message box (especially custom message box like BLMessageBox)
//
enum MESSAGE_TYPE
{
    MSG_MESSAGE,
    MSG_QUESTION,
    MSG_WARNING,
    MSG_ALARM,
    NUM_OF_MSG_TYPE,
};

//
// predefined message which will be common to all machines
//  note that all library related code use msg id <= 99, all machine code use > 100
// 
enum LIBRARY_MSG_ID
{
	MSG_OPEN_LOT_BEFORE_START = 1,
	MSG_ESTOP_PRESSED		  = 2,
	MSG_SAFE_DOOR_IS_OPENED	  = 3,

	MSG_MAINTENANCE_WARNING   = 5,

	MSG_MOTOR_HOME_FAIL		  = 10,
	MSG_MOTOR_MOVE_LOC_FAIL   = 11,
	MSG_MOTOR_MOVE_ABS_FAIL	  = 12,
	MSG_MOTOR_MOVE_REL_FAIL	  = 13,
    MSG_MOTOR_NOT_HOME_WARNING= 15,

	MSG_INPUT_NOT_ON		  = 20,
	MSG_INPUT_NOT_OFF		  = 21,
	MSG_WAIT_INPUT_ON_TIMEOUT = 22,
	MSG_WAIT_INPUT_OFF_TIMEOUT= 23,

	MSG_WAIT_EVT_ON_TIMEOUT	  = 30,
	MSG_WAIT_EVT_OFF_TIMEOUT  = 31,
};

// user message define
#define USERDEFINED_ATTENTION                       1
#define USERDEFINED_ERROR	                        2
#define USERDEFINED_QUESTION                        3
#define USERDEFINED_INFORMATION                     4
#define USERDEFINED_MANUAL_MAINTENANCE              5
#define USERDEFINED_MODULE				            6
#define USERDEFINED_PACKAGE				            7
#define USERDEFINED_M_OK			                8
#define USERDEFINED_M_ERROR			                9
#define USERDEFINED_M_BUSY			                10
#define USERDEFINED_M_STOPPED		                11
#define USERDEFINED_M_ALARM			                12
#define USERDEFINED_M_POS_EL_HIT	                13
#define USERDEFINED_M_NEG_EL_HIT	                14
#define USERDEFINED_M_DIST_NOT_VALID                15
#define USERDEFINED_MOTOR_LOC                       16
#define USERDEFINED_OUTPUT_CHANGED                  17
#define USERDEFINED_MAINTENANCE_BUSY			    18
#define USERDEFINED_MAINTENANCE_MOTOR_BUSY		    19
#define USERDEFINED_MAINTENANCE_OUTPUT_BUSY		    20
#define USERDEFINED_CANNOT_SAVE_DUE_TO_MOTOR_BUSY	21
#define USERDEFINED_CANNOT_SAVE_DUE_TO_MOTOR_ERROR	22
#define USERDEFINED_CONFIRM_SAVE_LOC				23
// all library messages post out to the message process window through this 2 messages
//  note: machine code should not re-define the ID used here
//
#define WM_SHOW_MESSAGE				WM_USER + 100
#define WM_CLEAR_MESSAGE			WM_USER + 101
