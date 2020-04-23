#pragma once

/*
CLogFile�÷�˵����
�ṩ���º궨�幩����Log������

1�� SETLOGINFO(szPath, nSize, nDay)
�����ļ���Ϣ
szPath·����Ĭ�ϣ�D:\\LogFile\\LogFile.log
nSize�ļ������С����M����Ĭ��2M
nDay�ļ�����ʱ�䣨�죩��Ĭ��7��

2�� WRITELOG( pLog )
дLog�ļ�

3��WRITECSVFILE(pfName, pfTitle, pfText)
дUTF-8�����.csv�ļ��������ṩ�ļ�·��

4����ȡ�ļ�·�����ļ������кš���������ʱ�䡢�ĺ�
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

	// дLog�ļ�
	static void  WriteLog( const char* pszLog );
	static void  WriteLog( const wchar_t* pszLog );

	// дCSV�ļ�
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
