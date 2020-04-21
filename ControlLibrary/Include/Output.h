/*******************************************************************
文件名：Output.h
作者：              版本：A1        日期：
功能描述：控制平台输出I/O功能类
历史修改记录：
*******************************************************************/
#pragma once

class CDeviceManager;

class  COutput
{
	friend CDeviceManager;

// constructors / destructors
public:
	COutput();
	~COutput();

public:
	/*********************************************************************************
	//函数描述：关闭输出I/O
	//输入参数：无
	//函数返回: TRUE表示关闭成功，FALSE表示关闭失败
	**********************************************************************************/
	_declspec(dllexport) BOOL Off(void);

	/*********************************************************************************
	//函数描述：打开输出I/O
	//输入参数：无
	//函数返回: TRUE表示打开成功，FALSE表示打开失败
	**********************************************************************************/
	_declspec(dllexport) BOOL On(void);

	/*********************************************************************************
	//函数描述：判断输出I/O是否关闭
	//输入参数：无
	//函数返回: TRUE表示关闭，FALSE表示打开
	**********************************************************************************/
	_declspec(dllexport) BOOL IsOff(void);

	/*********************************************************************************
	//函数描述：判断输出I/O是否打开
	//输入参数：无
	//函数返回: TRUE表示打开，FALSE表示关闭
	**********************************************************************************/
	_declspec(dllexport) BOOL IsOn(void);

	_declspec(dllexport) BOOL operator =(const BOOL bState);
	_declspec(dllexport) BOOL operator ==(const BOOL bState);
	_declspec(dllexport) BOOL operator !=(const BOOL bState);

public:
	int	    m_nIndex;		//索引号
	CString m_strName;		//I/O名称
	BOOL	m_bMonitorChange;	//设备运行过程中，是否不允许人工修改输出I/O状态

private:
	int		m_nCard;		//卡号
	int		m_nPoint;		//点位
	int		m_nFromLib;		//库
	BOOL	m_bOnState;		//感应器高/低有效

	// the old state during machine stop
	BOOL	m_bSavedState;

protected:

	static BOOL ConfigModule(int nTotalOutput);
	BOOL ReadConfig(int nIndex);

	// monitor the change of status
	void SaveState(void);
	BOOL IsStateChanged(void);
};