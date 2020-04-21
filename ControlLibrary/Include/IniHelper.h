/*******************************************************************
文件名：IniHelper.h
作者：              版本：A1        日期：
功能描述：控制平台Ini文件操作功能函数
历史修改记录：
*******************************************************************/
#pragma once

/*********************************************************************************
//函数描述：从ini文件中读取指定字段的字符串
//输入参数：lpszSection			节名称
//			lpszKey				字段名称
//			lpszDefault			默认值
//			lpszIniFile			ini文件路径和名称
//函数返回: CString字符串
**********************************************************************************/
CString __declspec(dllexport) IniReadString(LPCTSTR lpszSection, LPCTSTR lpszKey, const LPCTSTR lpszDefault, LPCTSTR lpszIniFile);

/*********************************************************************************
//函数描述：从ini文件中读取指定字段的整型数值
//输入参数：lpszSection			节名称
//			lpszKey				字段名称
//			nDefault			默认值
//			lpszIniFile			ini文件路径和名称
//函数返回: ini整型数值
**********************************************************************************/
int		__declspec(dllexport) IniReadInt(LPCTSTR lpszSection, LPCTSTR lpszKey, const int nDefault, LPCTSTR lpszIniFile);

/*********************************************************************************
//函数描述：从ini文件中读取指定字段的Boolean值
//输入参数：lpszSection			节名称
//			lpszKey				字段名称
//			bDefault			默认值
//			lpszIniFile			ini文件路径和名称
//函数返回: BOOL值
**********************************************************************************/
BOOL	__declspec(dllexport) IniReadBoolean(LPCTSTR lpszSection, LPCTSTR lpszKey, const BOOL bDefault, LPCTSTR lpszIniFile);

/*********************************************************************************
//函数描述：从ini文件中读取指定字段的浮点型数值
//输入参数：lpszSection			节名称
//			lpszKey				字段名称
//			dDefault			默认值
//			lpszIniFile			ini文件路径和名称
//函数返回: double浮点型数值
**********************************************************************************/
double	__declspec(dllexport) IniReadDouble(LPCTSTR lpszSection, LPCTSTR lpszKey, const double dDefault, LPCTSTR lpszIniFile);

/*********************************************************************************
//函数描述：从ini文件中读取指定字段的RGB值
//输入参数：lpszSection			节名称
//			lpszKey				字段名称
//			clrDefault			默认值
//			lpszIniFile			ini文件路径和名称
//函数返回: RGB值
**********************************************************************************/
COLORREF __declspec(dllexport) IniReadColor(LPCTSTR lpszSection, LPCTSTR lpszKey, const COLORREF clrDefault, LPCTSTR lpszIniFile);

/*********************************************************************************
//函数描述：从ini文件中读取指定字段的浮点型数值
//输入参数：lpszSection			节名称
//			lpszKey				字段名称
//			nStn				整型数组空间个数
//			pArray				整型数组指针
//			lpszIniFile			ini文件路径和名称
//函数返回: TRUE：读取成功	FALSE：读取失败
**********************************************************************************/
BOOL __declspec(dllexport) IniReadIntArray(LPCTSTR lpszSection, LPCTSTR lpszKey, int nStn, int *pArray, LPCTSTR lpszIniFile);

/*********************************************************************************
//函数描述：向指定ini文件写入字符串
//输入参数：lpszSection			节名称
//			lpszKey				字段名称
//			lpszStrToWrite		写入字符串
//			lpszIniFile			ini文件路径和名称
//函数返回: TRUE：写入成功	FALSE：写入失败
**********************************************************************************/
BOOL __declspec(dllexport) IniWriteString(LPCTSTR lpszSection, LPCTSTR lpszKey, LPCTSTR lpszStrToWrite, LPCTSTR lpszIniFile);

/*********************************************************************************
//函数描述：向指定ini文件写入整型数值
//输入参数：lpszSection			节名称
//			lpszKey				字段名称
//			nValueToWrite		写入整型数值
//			lpszIniFile			ini文件路径和名称
//函数返回: TRUE：写入成功	FALSE：写入失败
**********************************************************************************/
BOOL __declspec(dllexport) IniWriteInt(LPCTSTR lpszSection, LPCTSTR lpszKey, const int nValueToWrite, LPCTSTR lpszIniFile);

/*********************************************************************************
//函数描述：向指定ini文件写入BOOL值
//输入参数：lpszSection			节名称
//			lpszKey				字段名称
//			bValueToWrite		写入BOOL值
//			lpszIniFile			ini文件路径和名称
//函数返回: TRUE：写入成功	FALSE：写入失败
**********************************************************************************/
BOOL __declspec(dllexport) IniWriteBoolean(LPCTSTR lpszSection, LPCTSTR lpszKey, const BOOL bValueToWrite, LPCTSTR lpszIniFile);

/*********************************************************************************
//函数描述：向指定ini文件写入double浮点型数值
//输入参数：lpszSection			节名称
//			lpszKey				字段名称
//			dValueToWrite		写入double浮点型数值
//			lpszIniFile			ini文件路径和名称
//函数返回: TRUE：写入成功	FALSE：写入失败
**********************************************************************************/
BOOL __declspec(dllexport) IniWriteDouble(LPCTSTR lpszSection, LPCTSTR lpszKey, const double dValueToWrite, LPCTSTR lpszIniFile);
