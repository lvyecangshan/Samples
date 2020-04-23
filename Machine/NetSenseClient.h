// 欧姆龙温度获取模块

#pragma once
#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>

#define SERIAL_SENDBUF	(100)		// 串口服务器发送Buf大小
#define SERIAL_RECVBUF	(100)		// 串口服务器接收Buf大小

class CNetSenseClient
{
public:
	CNetSenseClient(void);
	~CNetSenseClient(void);

public:
	BOOL Connect(CString strIP, int nProt);				//打开串口服务器
	BOOL IsConnect();													//判断串口服务器是否连接
	BOOL Disconnect();												//关闭串口服务器

	// 压力传感器
	BOOL GetPressValue( unsigned int nSite, float * pfPressValue );

	// 获取高压电阻值
	BOOL GetHipotValue(char * cmd, double * dValue);

	// 获取二维码
	
	BOOL ReadCode(char * cmd, CString &strValue);

	unsigned int CRC_Check(unsigned char *data, unsigned char length);

private:
	struct sockaddr_in	m_sockAddr;
	SOCKET m_pSocket;
	CCriticalSection m_PressLock;
	CCriticalSection m_HipotLock;
	CCriticalSection m_CodeLock;
};