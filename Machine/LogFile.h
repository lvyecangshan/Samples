#pragma once

/*
CLogFile用法说明：
提供以下宏定义供调用Log方法：

1， SETLOGINFO(szPath, nSize, nDay)
设置文件信息
szPath路径，默认：D:\\LogFile\\LogFile.log
nSize文件保存大小（兆M），默认2M
nDay文件备份时间（天），默认7天

2， WRITELOG( pLog )
写Log文件

3，WRITECSVFILE(pfName, pfTitle, pfText)
写UTF-8编码的.csv文件，必须提供文件路径

4，获取文件路径、文件名、行号、编译日期时间、的宏
( __FILE__, __FUNCTION__, __LINE__, __DATE__, __TIME__ )

*/

#include <string>
#include<io.h>
#include<ctime>

using  std::string;
using  std::wstring;

#define  WRITE_LOG_FILE

class CLogFile
{
public:
	static void  SetLogFileInfo(char* pPath, int nSize, int nDay);
	static void  SetLogFileInfo(wchar_t* pPath, int nSize, int nDay);

	// 写Log文件
	static void  WriteLog( const char* pszLog );
	static void  WriteLog( const wchar_t* pszLog );

	// 写CSV文件
	static void  WriteCSVFile(const char* pFileName, const char* pFileTitle, const char* pLogText);
	static void  WriteCSVFile(const wchar_t* pFileName, const wchar_t* pFileTitle, const wchar_t* pLogText);
	static void  WriteCSV(const wchar_t* pFileName, const wchar_t* pFileTitle, const wchar_t* pLogText);

	static BOOL  CreateMultipleDirectory(const CString& strPath);


protected:
	static wstring  GetTimeString( const char* pszFormat );		// "[%Y-%m-%d %H:%M:%S]"
	static string  Wchar2char( const wchar_t* wstr, unsigned int unCode = CP_ACP );
	static wstring  char2Wchar( const char* str, unsigned int unCode = CP_ACP );
	static bool GetFilePath( wstring strPath );
	static bool GetFileInfo( wstring strPath, struct _wfinddata_t &fileInfo );
	static void ClearLogFile( wstring strPath, int nDays );

};

#ifdef WRITE_LOG_FILE

#define SETLOGINFO( szPath, nSize, nDay )		CLogFile::SetLogFileInfo( szPath, nSize, nDay )
#define WRITELOG( pLog )		CLogFile::WriteLog( pLog )

#define WRITECSVFILE( pfName, pfTitle, pfText )		CLogFile::WriteCSVFile( pfName, pfTitle, pfText )
#define WRITECSV( pfName, pfTitle, pfText )		CLogFile::WriteCSV( pfName, pfTitle, pfText )

#else

#define SETLOGINFO( szPath, nSize, nDay )
#define WRITELOG( pLog )

#define WRITECSVFILE( pfName, pfTitle, pfText )		CLogFile::WriteCSVFile( pfName, pfTitle, pfText )
#define WRITECSV( pfName, pfTitle, pfText )		CLogFile::WriteCSV( pfName, pfTitle, pfText )

#endif
