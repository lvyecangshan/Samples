#include "stdafx.h"
#include "GenerateDump.h"

LONG WINAPI MyUnhandledExceptionFilter(struct _EXCEPTION_POINTERS* lpExceptionInfo)
{
	TCHAR szProgramPath[MAX_PATH] = {0};

	if(GetModuleFileName(NULL, szProgramPath, MAX_PATH))
	{
		LPTSTR lpSlash = _tcsrchr(szProgramPath, '//');

		if(lpSlash)
		{
			*(lpSlash + 1) = '/0';
		}
	}

	TCHAR szDumpFile[MAX_PATH] = {0};

	_stprintf_s(szDumpFile, _T("%s%d.dmp"), szProgramPath, time(NULL));

	TCHAR szReportFile[MAX_PATH] = {0};

	_stprintf_s(szReportFile, _T("%sBugReport.exe"), szProgramPath);

	HANDLE hDumpFile = CreateFile(szDumpFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL ,NULL);

	MINIDUMP_EXCEPTION_INFORMATION stMiniDumpExceptionInfo;

	stMiniDumpExceptionInfo.ExceptionPointers = lpExceptionInfo;

	stMiniDumpExceptionInfo.ThreadId = GetCurrentThreadId();

	stMiniDumpExceptionInfo.ClientPointers = TRUE;

	MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, 

		MiniDumpNormal, &stMiniDumpExceptionInfo, NULL, NULL);

	CloseHandle(hDumpFile);

	::ShellExecute(NULL, NULL, szReportFile, szDumpFile, NULL, SW_HIDE);

	//Use your own user-friendly ui prompt here

	MessageBox(NULL, _T("Program crashed, generate dump file and call bug report!"), _T("Oops"), MB_OK);

	return EXCEPTION_EXECUTE_HANDLER;
}

