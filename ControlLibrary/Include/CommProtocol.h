// CommProtocol.h : main header file for the CommProtocol DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CCommProtocolApp
// See CommProtocol.cpp for the implementation of this class
//

class CCommProtocolApp : public CWinApp
{
public:
	CCommProtocolApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
