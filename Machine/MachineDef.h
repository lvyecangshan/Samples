#pragma once
#include "EasyAutoControlExDef.h"
#include <map>

// ��Ϣ����
#define WM_SHOW_MESSAGE_EX		WM_USER + 102
#define WM_SHOW_ALARM_MESSAGE	WM_USER + 205
#define WM_COMM_INFO_MESSAGE    WM_USER + 112

#define WM_USER_LOGINOUT		WM_USER + 111
#define WM_OUTPUT_VIEW_MSG		WM_USER	+ 113

// Data�ļ���·��
#define MODULE_CT_FOLDER	_T("Data\\ModuleCT")
#define MODULE_RUN_FOLDER  _T("Data\\RunStepInfo\\")

#define RUN_DATA_CFG		_T("Data\\RunData.cfg")
#define DATA_MACHINE		_T("Data\\Machine.cfg")


// Log�ļ���·��
#define MSG_HISTROY_XML			_T("Log\\Message History.xml")
#define USER_HISTORY_XML		_T("Log\\User History.xml")
#define CUSTOMER_TRACE_LOG		_T("Log\\Customer Trace\\")


// System�ļ���·��
#define MODULE_PICTURE		_T("System\\ModulePicture\\")
#define MODULEEX_CFG		_T("System\\ModuleEx.cfg")
#define PARAMETER_CFG		_T("System\\Parameter.cfg")


// database
#define  DATABASE_FILE			_T("Log\\Machine.mdb")
#define  DATABASE_BACKUP_FILE		_T("Log\\Machine_����.mdb")

#define  MES_ONLOADCOUNT 20
// enum 

// ���ڵ�¼Ȩ�޵Ǽ�
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

// ���λ��
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
	{ROBOT_HIGH_SPEED, 				_T("POWER_HIGH\r\n"), 					_T("POWER_HIGH_OK\r\n"),					_T("����"),					0},
	{ROBOT_MID_SPEED,				_T("POWER_MID\r\n"),					_T("POWER_MID_OK\r\n"),						_T("����"),					0},
	{ROBOT_LOW_SPEED,				_T("POWER_LOW\r\n"),					_T("POWER_LOW_OK\r\n"),						_T("����"),					0},
	{ROBOT_GO_HOME,					_T("GO_HOME\r\n"),						_T("GO_HOME_OK\r\n"),						_T("��ԭ��"),				0},
	{ROBOT_ONLOAD_WAIT,				_T("ONLOAD_WAIT_POS\r\n"),				_T("ONLOAD_WAIT_POS_OK\r\n"),				_T("ȡ�ϵȴ�λ"),			0},
	{ROBOT_ONLOAD_POS,				_T("ONLOAD_POS\r\n"),					_T("ONLOAD_POS_OK\r\n"),					_T("ȡ��λ"),				1},
	{ROBOT_OFFLOAD_BUFF_WAIT_POS1,	_T("OFFLOAD_BUFF_ONE_WAIT_POS\r\n"),	_T("OFFLOAD_BUFF_ONE_WAIT_POS_OK\r\n"),		_T("����ȴ�λ#1"),			0},
	{ROBOT_OFFLOAD_BUFF_POS1,		_T("OFFLOAD_BUFF_ONE_POS\r\n"),			_T("OFFLOAD_BUFF_ONE_POS_OK\r\n"),			_T("����λ#1"),				1},
	{ROBOT_OFFLOAD_BUFF_WAIT_POS2,	_T("OFFLOAD_BUFF_TWO_WAIT_POS\r\n"),	_T("OFFLOAD_BUFF_TWO_WAIT_POS_OK\r\n"),		_T("����ȴ�λ#2"),			0},
	{ROBOT_OFFLOAD_BUFF_POS2,		_T("OFFLOAD_BUFF_TWO_POS\r\n"),			_T("OFFLOAD_BUFF_TWO_POS_OK\r\n"),			_T("����λ#2"),				1},
	{ROBOT_OFFLOAD_WAIT_POS,		_T("OFFLOAD_WAIT_POS\r\n"),				_T("OFFLOAD_WAIT_POS_OK\r\n"),				_T("���ϵȴ�λ"),			0},
	{ROBOT_OFFLOAD_POS,				_T("OFFLOAD_POS\r\n"),					_T("OFFLOAD_POS_OK\r\n"),					_T("����λ"),				1},
	{ROBOT_OFFLOAD_NG_WAIT_POS,		_T("OFFLOAD_NG_WAIT_POS\r\n"),			_T("OFFLOAD_NG_WAIT_POS_OK\r\n"),			_T("Ng���ϵȴ�λ"),			0},
	{ROBOT_OFFLOAD_NG_POS,			_T("OFFLOAD_NG_POS\r\n"),				_T("OFFLOAD_NG_POS_OK\r\n"),				_T("Ng����λ"),				1},
};

// ��Ϣ
enum MSG_ID
{
	MSG_NO_ACCESS_RIGHT						= 1002,
	MSG_MACHINE_STOP_BEFORE_EXIT	= 1003,
	MSG_CONFIRM_EXIT_MACHINE				= 1004,

	// ���϶�ջ
	MSG_ONLOAD_STACK_CHECK_CAR_VAILD		= 2000,	//��ջ����С��
	MSG_ONLOAD_STACK1_CHECK_CAR_AND_PALLET_ABERRANT   = 2001,
	MSG_ONLOAD_STACK2_CHECK_CAR_AND_PALLET_ABERRANT   = 2002,
	MSG_ONLOAD_STACK_PALLET_TOO_MANY		= 2003,
	MSG_ONLOAD_STACK_CHECK_CAR_NOT_REMOVE	=2004,
	MSG_ONLOAD_STACK_CHECK_CAR_STATUS_ILLEGAL	= 2005,

	// ���϶�ջ
	MSG_OFFLOAD_STACK_CHECK_CAR_VAILD		= 2021,
	MSG_OFFLOAD_STACK_PROMPT_OFFLOAD_EMPTY_PALLET  = 2022,
	MSG_OFFLOAD_STACK_CHECK_CAR_NOT_INPOS   = 2023,

	// ����
	MSG_FEEDERTRAY_CHECK_PALLET_VAILD			= 2041,
	MSG_FEEDERTRAY_CHECK_PALLET_INVAILD		= 2042,
	MSG_FEEDERTRAY_MOTOR_MOVE_NOT_INPOS = 2043,
	MSG_FEEDERTRAY_SAFE_RASTER_CHECK_UNNOTMAL = 2044,


	// ����ǰ
	MSG_DUST_BEFORE_CHECK_SHELL_DIRECTION_REVERSE		= 2081,

	//����
	CLEAN_SHELL_FINGER_OPEN_WITHOUT_PALLET             = 2100,//���ģ���צ�򿪣����������
	// ��о���
	MSG_BATTERY_CORE_PRESS_COMMUNICATE_FAIL		= 2181,

	// ��о����
	MSG_ONLOAD_CORE_PNP_PICK_COLLIDE	= 2221,


	// ������
	MSG_ROBOT_MOVE_TIMEOUT					= 2241,

	// ����
	CLEAR_MATERIAL_FINISH 					= 2242,//�������

	MSG_FU_AIR_ALARM,    //��ѹ����
	MSG_AIR_ALARM,     //��ѹ����

	ROBOT_PRESS_ALARM,   //������ѹ�ϱ���
};


// class

// ��Ʒģ������ڴ����
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


// ��Ʒ����
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