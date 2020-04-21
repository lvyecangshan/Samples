// EasyAutoControlEx.h : main header file for the EasyAutoControlEx DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CEasyAutoControlExApp
// See EasyAutoControlEx.cpp for the implementation of this class
//

class CEasyAutoControlExApp : public CWinApp
{
public:
	CEasyAutoControlExApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
HINSTANCE GetHinstance();