/*******************************************************************
文件名：BLMessageBox.h
作者：              版本：A1        日期：
功能描述：控制平台弹框消息功能函数
历史修改记录：
*******************************************************************/
#pragma once

/*********************************************************************************
//函数描述：弹出提示框
//输入参数：lpszMessage	提示内容
//			nType 提示框样式 
//				MB_ABORTRETRYIGNORE 显示Abort、Retry、Ignore按钮
//				MB_OK				显示OK按钮
//				MB_OKCANCEL			显示OK、Cancel按钮
//				MB_RETRYCANCEL		显示Retry、Cancel按钮
//				MB_YESNO			显示Yes、No按钮
//				MB_YESNOCANCEL		显示Yes、No、Cancel按钮
//				MB_ICONINFORMATION	显示一个i图标，表示提示
//				MB_ICONEXCLAMATION	显示一个惊叹号，表示警告
//				MB_ICONSTOP			显示手形图标，表示警告或严重错误
//				MB_ICONQUESTION		显示问号图标，表示疑问
//函数返回: IDABORT		中止按钮
//			IDCANCEL	退出按钮
//			IDIGNORE	忽略按钮
//			IDNO		NO按钮
//			IDOK		OK按钮
//			IDRETRY		重试按钮
//			IDYES		YES按钮
**********************************************************************************/
int __declspec(dllexport) BLMessageBox(LPCTSTR lpszMessage, UINT nType = MB_OK | MB_ICONEXCLAMATION);

/*********************************************************************************
//函数描述：弹出提示框
//输入参数：lpszMessage	提示内容
//			nType	提示框样式 
//				MB_ABORTRETRYIGNORE 显示Abort、Retry、Ignore按钮
//				MB_OK				显示OK按钮
//				MB_OKCANCEL			显示OK、Cancel按钮
//				MB_RETRYCANCEL		显示Retry、Cancel按钮
//				MB_YESNO			显示Yes、No按钮
//				MB_YESNOCANCEL		显示Yes、No、Cancel按钮
//				MB_ICONINFORMATION	显示一个i图标，表示提示
//				MB_ICONEXCLAMATION	显示一个惊叹号，表示警告
//				MB_ICONSTOP			显示手形图标，表示警告或严重错误
//				MB_ICONQUESTION		显示问号图标，表示疑问
//			pastrBtnText	用于更换按钮提示名称
//函数返回: IDABORT		中止按钮
//			IDCANCEL	退出按钮
//			IDIGNORE	忽略按钮
//			IDNO		NO按钮
//			IDOK		OK按钮
//			IDRETRY		重试按钮
//			IDYES		YES按钮
**********************************************************************************/
int __declspec(dllexport) BLMessageBox(LPCTSTR lpszMessage, UINT nType, CStringArray *pastrBtnText);


