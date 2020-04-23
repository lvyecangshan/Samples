#include "StdAfx.h"
#include "LogFile.h"

wstring  g_strPath = L"D:\\机台数据文件\\LogFile\\LogFile.log";
wstring g_strLogFile = g_strPath;
unsigned int g_unSize = 2;
unsigned int g_unDay = 7;


void CLogFile::WriteLog( const char* pszLog )
{
	WriteLog( char2Wchar(pszLog).c_str());
}

void CLogFile::WriteLog( const wchar_t* pszLog )
{
	if (GetFilePath( g_strPath ))
	{
		wstring strTime = GetTimeString("[%Y-%m-%d %H:%M:%S]: ");

		struct _wfinddata_t curfile;
		if (!GetFileInfo(g_strLogFile, curfile) || curfile.size >= g_unSize*1024*1024)
		{
			g_strLogFile = g_strPath;
			g_strLogFile.insert( g_strLogFile.rfind(L".", g_strLogFile.size()), GetTimeString("%Y-%m-%d %H.%M") );

			ClearLogFile(g_strLogFile, g_unDay);
		}

		FILE* fp = NULL;
		if (0 == _wfopen_s(&fp, g_strLogFile.c_str(), L"a+,ccs=UTF-8"))
		{
			fwprintf_s(fp, strTime.c_str());
			fwprintf_s(fp, pszLog);
			fwprintf_s(fp, L"\n");
			fclose(fp);
		}
	}
}

void CLogFile::WriteCSVFile( const char* pFileName, const char* pFileTitle, const char* pLogText )
{
	WriteCSVFile( char2Wchar(pFileName).c_str(), char2Wchar(pFileTitle).c_str(), char2Wchar(pLogText).c_str() );
}

void CLogFile::WriteCSVFile( const wchar_t* pFileName, const wchar_t* pFileTitle, const wchar_t* pLogText )
{
	if (NULL == pFileName)
	{
		return;
	}
	wstring strFile(pFileName);
	if (GetFilePath(strFile))
	{
		struct _wfinddata_t fileInfo;		// 文件信息
		FILE * fp = NULL;
		if (!GetFileInfo(strFile, fileInfo))
		{
			if (0 == _wfopen_s(&fp, pFileName, L"w+,ccs=UTF-8"))
			{
				fwprintf_s(fp, pFileTitle);
				fwprintf_s(fp, L"\n");
				fclose(fp);
			}
		}
		if (0 == _wfopen_s(&fp, pFileName, L"a+,ccs=UTF-8"))
		{
			fwprintf_s(fp, pLogText);
			fwprintf_s(fp, L"\n");
			fclose(fp);
		}
	}
}


void CLogFile::WriteCSV( const wchar_t* pFileName, const wchar_t* pFileTitle, const wchar_t* pLogText )
{
	if (NULL == pFileName)
	{
		return;
	}
	FILE *file = NULL;
	CFileFind fileFind;	

	if (CreateMultipleDirectory(pFileName))
	{
		if(!fileFind.FindFile(pFileName))
		{
			if (_tfopen_s(&file, pFileName, _T("w+, ccs=UTF-8")) != 0)
			{
				return;
			}
			_ftprintf_s(file, pFileTitle);
		}
		else
		{
			if (_tfopen_s(&file, pFileName, _T("a+, ccs=UTF-8")) != 0)
			{
				return;
			}
		}
		_ftprintf_s(file,_T("%s\r"),pLogText);
		fclose(file);
	}
}
BOOL CLogFile::CreateMultipleDirectory(const CString& strPath)
{
	if (PathFileExists(strPath))
	{
		return TRUE;
	}

	CString strDir(strPath);
	vector<CString>vPath;
	CString strTemp;
	BOOL bResult = FALSE;
	for (int i=0; i < strDir.GetLength(); i++)
	{
		if (strDir.GetAt(i) != _T('\\'))
		{
			strTemp.AppendChar(strDir.GetAt(i));
		}
		else
		{
			vPath.push_back(strTemp);
			strTemp.AppendChar(_T('\\'));
		}
	}
	vector<CString>::const_iterator vIter;
	for (vIter = vPath.begin(); vIter != vPath.end(); vIter++)
	{
		bResult = FALSE;
		if (PathFileExists(*vIter))
		{
			bResult = TRUE;
		}
		else if (CreateDirectory(*vIter, NULL))
		{
			bResult = TRUE;
		}
	}
	return bResult;
}
wstring CLogFile::GetTimeString( const char* pszFormat )
{
	time_t tNowTime;
	time(&tNowTime);
	tm* tLocalTime = localtime(&tNowTime);
	char szTime[30] = {'\0'};
	wstring wstr;
	int nMax = wstr.max_size();
	strftime(szTime, 30, pszFormat, tLocalTime);
	wstring strTime = char2Wchar(szTime);
	return strTime;
}

string CLogFile::Wchar2char(const wchar_t* wstr, unsigned int unCode /*= CP_ACP*/)
{
	string strDes;
	if (NULL != wstr)
	{
		int nLen=::WideCharToMultiByte(unCode, 0, wstr, wcslen(wstr), NULL, 0, NULL, NULL);
		if ( nLen > 0 )
		{
			char* pBuffer=new char[nLen+1];
			memset(pBuffer, 0, nLen+1);
			::WideCharToMultiByte(unCode, 0, wstr, wcslen(wstr), pBuffer, nLen, NULL, NULL);
			pBuffer[nLen]='\0';
			strDes.append(pBuffer);
			delete[] pBuffer;
		}
	}
	return strDes;
}

wstring CLogFile::char2Wchar(const char* str, unsigned int unCode /*= CP_ACP*/)
{
	wstring strDes;
	if (NULL != str)
	{
		int nLen=::MultiByteToWideChar(unCode, 0, str, strlen(str), NULL, 0);
		if ( nLen > 0 )
		{
			wchar_t* pBuffer=new wchar_t[nLen+1];
			memset(pBuffer, 0, nLen+1);
			::MultiByteToWideChar(unCode, 0, str, strlen(str), pBuffer, nLen);
			pBuffer[nLen]='\0';
			strDes.append(pBuffer);
			delete[] pBuffer;
		}
	}
	return strDes;
}

void CLogFile::SetLogFileInfo( char* pPath, int nSize, int nDay )
{
	if (NULL != pPath) g_strPath = char2Wchar(pPath);
	if (nSize > 0) g_unSize = nSize;
	if (nDay > 0) g_unDay = nDay;
}

void CLogFile::SetLogFileInfo( wchar_t* pPath, int nSize, int nDay )
{
	if (NULL != pPath) g_strPath = (pPath);
	if (nSize > 0) g_unSize = nSize;
	if (nDay > 0) g_unDay = nDay;
}

void CLogFile::ClearLogFile( wstring strPath, int nDays )
{
	long   hFile   =   0;			// 文件句柄
	struct _wfinddata_t fileInfo;		// 文件信息
	hFile = strPath.rfind(L"\\", strPath.size());
	strPath.erase(hFile+1, (strPath.size() - hFile));
	strPath += L"*.*";
	time_t sTime;
	if((hFile = _wfindfirst(strPath.c_str(), &fileInfo)) !=  -1)
	{
		do
		{
			if(!(fileInfo.attrib & _A_NORMAL))
			{
				__time64_t ti64 = fileInfo.time_access;
				ti64 = fileInfo.time_create;
				ti64 = fileInfo.time_write;
				ti64 = time(&sTime);
				if (ti64 - fileInfo.time_access > nDays*24*60*60
					|| ti64 - fileInfo.time_create > nDays*24*60*60
					|| ti64 - fileInfo.time_write > nDays*24*60*60)
				{
					wstring strDelete(strPath);
					strDelete.erase((strDelete.rfind(L"\\", strDelete.size()))+1, (strPath.size() - strDelete.rfind(L"\\", strDelete.size())));
					strDelete += fileInfo.name;

					int nRes = _wremove(strDelete.c_str());
					nRes = errno;
				}
			}
		}while(_wfindnext(hFile, &fileInfo) == 0);
		_findclose(hFile);
	}
}

bool CLogFile::GetFilePath( wstring strPath )
{
	bool bResult = false;
	int nLen = strPath.size();
	wstring  str1 = L"\\";
	for (int it = 0; it < nLen; it++)
	{
		if (str1.at(0) == strPath.at(it))
		{
			bResult = false;
			wstring wPath( strPath.substr(0, it) );
			if (PathFileExists( wPath.c_str() ) || CreateDirectory( wPath.c_str(), NULL ))
			{
				bResult = true;
			}
		}
	}
	return bResult;
}

bool CLogFile::GetFileInfo( wstring strPath, struct _wfinddata_t &fileInfo )
{
	long   hFile   =   0;			// 文件句柄
	if((hFile = _wfindfirst( strPath.c_str(), &fileInfo )) !=  -1)
	{
		_findclose(hFile);
		return true;
	}
	return false;
}

