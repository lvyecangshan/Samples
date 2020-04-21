/*******************************************************************
文件名：Input.h
作者：              版本：A1        日期：
功能描述：控制平台输入I/O功能类
历史修改记录：
*******************************************************************/
#pragma once
// class CInput
class CDeviceManager;

class  CInput
{
	friend CDeviceManager;

public:
	CInput();
	~CInput();

public:

	/*********************************************************************************
	//函数描述：判断输入I/O是否关闭
	//输入参数：无
	//函数返回: TRUE表示关闭，FALSE表示打开
	**********************************************************************************/
	_declspec(dllexport) BOOL IsOff(void);

	/*********************************************************************************
	//函数描述：判断输入I/O是否打开
	//输入参数：无
	//函数返回: TRUE表示打开，FALSE表示关闭
	**********************************************************************************/
	_declspec(dllexport) BOOL IsOn(void);

	_declspec(dllexport) BOOL operator == (const BOOL bState);
	_declspec(dllexport) BOOL operator != (const BOOL bState);

public:
	int	    m_nIndex;		// 索引号
	CString m_strName;		// I/O名称
	DWORD   m_dwTimeout;	// 超时时间，单位：毫秒

protected:
	BOOL	m_bOnState;		// 相位
	int		m_nCard;		// 卡号
	int		m_nPoint;		// 点位
    int     m_nFromLib;		// 库

protected:
	static BOOL ConfigModule(int nTotalInput);
	BOOL ReadConfig(int nIndex);
};