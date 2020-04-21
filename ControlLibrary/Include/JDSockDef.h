#pragma once

#include "winsock2.h"
#pragma comment(lib, "ws2_32.lib")
#include "JDThread.h"


#include "JDLock.h"

#pragma pack(push)
#pragma pack(1)

#define  WM_SERVER_MESSAGE WM_USER + 100
#define  WM_CLIENT_MESSAGE WM_USER + 101
#define JD_SUCCESS 1

#define JD_ADDRESS_SIZE 20
#define JD_STATION_ID_SIZE 4
#define JD_STATION_NAME_SIZE 50
#define JD_MESSAGE_MAX_SIZE 128
#define JD_MAX_STATION_COUNT 128
enum CLIENT_TYPE
{
	CLIENT_TYPE_LINE_STATION,
	CLIENT_TYPE_LINE_MANAGER
};

#define MACHINE_SERVER_PORT						5000
#define MACHINE_PROTOCAL_VERSION				1
//////////
#define MESSAGE_TYPE_OPEATOR					0x9001
#define MESSAGE_TYPE_START						0x0001
#define MESSAGE_TYPE_STOP						0x0002
#define MESSAGE_TYPE_RESET						0x0003
#define MESSAGE_TYPE_RESTART					0x0004

#define MESSAGE_TYPE_LOGIN						0x0001
#define MESSAGE_TYPE_KEEP_ALIVE					0x0002

#define MESSAGE_TYPE_IO_ACTIVE					0x0003
#define MESSAGE_TYPE_MOTOR_ACTIVE				0x0004

#define MESSAGE_TYPE_STATION_STATE_REQ				0x0007
#define MESSAGE_TYPE_STATION_STATE_ANS				0x1007

#define MESSAGE_TYPE_OPERATE_STATION_REQ			0x0008
#define MESSAGE_TYPE_OPERATE_STATION_ANS			0x1008

#define MESSAGE_TYPE_GET_REMOTE_EVENT_STATUS_REQ	0x0005
#define MESSAGE_TYPE_GET_REMOTE_EVENT_STATUS_ANS	0x1005

#define MESSAGE_TYPE_SET_REMOTE_EVENT_STATUS_REQ	0x0006
#define MESSAGE_TYPE_SET_REMOTE_EVENT_STATUS_ANS	0x1006

typedef struct MESSAGE_HEAD
{
	int iType;
	int iLength;
	//int iVersion;
	int iTransCode;
	//int iReserved[4];
}MessageHead;

typedef struct MESSAGE_IO_DATA
{
	MESSAGE_HEAD head;
	int m_nIndexIO;
	int m_nAction;
}MessageIOData;

typedef struct MESSAGE_MOTOR_DATA
{
	MESSAGE_HEAD head;
	int m_nIndexIO;
	int m_nAction;
}MessageMotorData;

typedef struct MESSAGE_MAX
{
	MESSAGE_HEAD head;
	char szMaxMessage[JD_MESSAGE_MAX_SIZE];
}MessageMax;

typedef struct MESSAGE_OPEATOR
{
	MESSAGE_HEAD head;
	int iOpeatorType;
}MessageOpeator;

//typedef struct MESSAGE_LOGIN
//{
//	MESSAGE_HEAD head;
//	int iClientType;
//}MessageLogon;
//
//typedef struct MESSAGE_MACHINE_STATE
//{
//	MESSAGE_HEAD head;
//	int iState;
//}MessageMachineState;
//
typedef struct MESSAGE_KEEP_ALIVE
{
	MESSAGE_HEAD head;
	char szStationId[JD_STATION_ID_SIZE];
	char szStationName[JD_STATION_NAME_SIZE];
	int iClientType;
	int iRunState;
}MessageKeepAlive;

//typedef struct MESSAGE_GET_REMOTE_EVENT_STATUS_REQ
//{
//	MESSAGE_HEAD head;
//	char szStationIdSender[JD_STATION_ID_SIZE];
//	char szStationIdReceiver[JD_STATION_ID_SIZE];
//	int iEventID;
//}MessageGetRemoteEventStatusReq;
//
//typedef struct MESSAGE_GET_REMOTE_EVENT_STATUS_ANS
//{
//	MESSAGE_HEAD head;
//	char szStationIdSender[JD_STATION_ID_SIZE];
//	int iEventStatus;
//}MessageGetRemoteEventStatusAns;
//
//typedef struct MESSAGE_SET_REMOTE_EVENT_STATUS_ANS
//{
//	MESSAGE_HEAD head;
//	char szStationIdSender[JD_STATION_ID_SIZE];
//	int iResult;
//}MessageSetRemoteEventStatusAns;
//
//typedef struct MESSAGE_SET_REMOTE_EVENT_STATUS_REQ
//{
//	MESSAGE_HEAD head;
//	char szStationIdSender[JD_STATION_ID_SIZE];
//	char szStationIdReceiver[JD_STATION_ID_SIZE];
//	int iEventID;
//	int iValue;
//}MessageSetRemoteEventStatusReq;
//
//typedef struct MESSAGE_STATION_INFO
//{
//	char szStationId[JD_STATION_ID_SIZE];
//	char szStationAddress[JD_ADDRESS_SIZE];
//	char szStationName[JD_STATION_NAME_SIZE];
//	int iConnectState;
//	int iRunState;
//}MessageStationInfo;
//
//typedef struct MESSAGE_STATION_LIST
//{
//	int iSize;
//	MESSAGE_STATION_INFO data[JD_MAX_STATION_COUNT];
//}MessageStationList;
//
//typedef struct MESSAGE_CLIENT_INFO
//{
//	char szStationAddress[JD_ADDRESS_SIZE];
//	int iClientType;
//	int iRunState;
//}MessageClientInfo;
//
//typedef struct MESSAGE_CLIENT_LIST
//{
//	int iSize;
//	MESSAGE_CLIENT_INFO data[128];
//}MessageClientList;
//
//typedef struct MESSAGE_STATION_STATE_REQEST
//{
//	MESSAGE_HEAD head;
//	MESSAGE_STATION_LIST list;
//}MessageStationStateRequest;
//
//typedef struct MESSAGE_STATION_STATE_ANSWER
//{
//	MESSAGE_HEAD head;
//	MESSAGE_STATION_LIST list;
//}MessageStationStateAnswer;
//
//enum OPERATE_STATION_TYPE
//{
//	OPERATE_STATION_RESET,
//	OPERATE_STATION_START,
//	OPERATE_STATION_STOP,
//	NUM_OF_OPERATE_STATION
//};
//
//typedef struct MESSAGE_OPERATE_STATION_REQEST
//{
//	MESSAGE_HEAD head;
//	char szStationId[JD_STATION_ID_SIZE];
//	int iState;
//}MessageOperateStateRequest;
//
//typedef struct MESSAGE_OPERATE_STATION_ANSWER
//{
//	MESSAGE_HEAD head;
//	BOOL bRet;
//}MessageOperateStateAnswer;

#pragma pack(pop)