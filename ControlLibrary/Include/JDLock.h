#pragma once
#include "Windows.h"
class __declspec(dllexport) CJDCS
{
public:
	CJDCS(void);
	~CJDCS(void);
	CRITICAL_SECTION cs;
	void Lock();
	void UnLock();
};
class __declspec(dllexport) CJDLock
{
public:
	CJDLock(CJDCS *pLock);
	~CJDLock(void);
	CJDCS *m_pLock;
};
