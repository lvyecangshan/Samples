#pragma once
#include "afxmt.h"
class __declspec(dllexport) CLogWrite
{
protected:
	CLogWrite();

public:
	 virtual ~CLogWrite();

   static CLogWrite* Instance();

   static void Destroy();

   void WriteInfo( const TCHAR *pszfmt, ... );
   void WriteCInfo( const TCHAR *pszfmt, ... );

   void WriteInfo( CString& str ) {WriteInfo( str.GetBuffer() );str.ReleaseBuffer(); }
   void WriteCInfo( CString& str ) {WriteCInfo( str.GetBuffer() ); str.ReleaseBuffer(); }
   void WriteInfo( CString* str ) {WriteInfo( str->GetBuffer() ); str->ReleaseBuffer(); }
   void WriteCInfo( CString* str ) {WriteCInfo( str->GetBuffer() ); str->ReleaseBuffer();}

private:
	HANDLE			 m_hMFile;
	HANDLE			 m_hCFile;
	HANDLE			 m_hMutex;	
	CRITICAL_SECTION m_hLock;		

	CCriticalSection  m_singleCLock;
	CCriticalSection  m_singleMLock;
	DWORD			 m_dwStartTime; 
    DWORD            m_dMFileSize;
	DWORD            m_dCFileSize;

	BOOL CreateLogFile();
    static CLogWrite *sm_pInstance;
};

#define PRINTF (*CLogWrite::Instance()).WriteInfo

#define CPRINTF (*CLogWrite::Instance()).WriteCInfo

#ifdef _DEBUG
#define DPRINTF PRINTF
#else
#define DPRINTF 
#endif


