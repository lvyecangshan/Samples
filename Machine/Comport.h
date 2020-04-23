//所有命令超时时间设定为5秒
//1. 查询状态命令：（0x10）
//发送：
// 起始位   地址   命令   停止位
// 0xAA     0x01   0x10   0xCC
// 声学检测模块回复：
// 起始位   地址   数据         停止位
// 0xAA     0x01   return_sign   0xCC
//参数说明 Return_sign:
//0xB1-----准备好
//0xB2------准备中
//0xEE------没准备好

//2：启动检测命令：（0x20)
//发送：
// 起始位   地址   命令   停止位
// 0xAA     0x01   0x20   0xCC
// 声学检测模块回复：
// 起始位   地址   数据         停止位
// 0xAA     0x01   return_sign   0xCC
//参数说明 Return_sign:
//0xDD-----合格
//0xBB------正在检测
//0x11--------无声（信号无应答）
//0x22--------频响超差
//0x33--------极性超差
//0x44--------失真超差
//0x55--------阻抗超差

#pragma once

#define READ_LIMIT 30

class CComPort
{
public:
	CComPort(void);
	~CComPort(void);
public:
	BOOL OpenPort(int iPort,DWORD dwBaudRate);
	BOOL ClsoePort();

	char Write(char *szCmd);
	BOOL Read(CString &strResult,int iSleepTime = 1000);
	
public:
	int m_iPort;
	BOOL m_bOpen;

private:
	HANDLE m_hCom;
	DWORD m_dwBaudRate;
	byte m_btStopBits;
	byte m_btByteSize;
	byte m_btParity;
	CCriticalSection m_PortLock;
};
