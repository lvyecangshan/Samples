#pragma once

#include "MachineStateStatic.h"
#include "BitmapStatic.h"
// CDJPRibbonBar

class __declspec(dllexport) CDJPRibbonBar : public CMFCRibbonBar
{
	DECLARE_DYNAMIC(CDJPRibbonBar)

public:
	CDJPRibbonBar();
	virtual ~CDJPRibbonBar();

protected:
	DECLARE_MESSAGE_MAP()
public:
	CMachineStateStatic m_stMachineState;
	BOOL InitState(CRunCtrl *pRunCtrl = NULL,int nTime = 1000);
	CBitmapStatic  m_stBitmapState;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	BOOL LoadFromResource(UINT uiXMLResID, LPCTSTR lpszResType  = RT_RIBBON , HINSTANCE hInstance  = NULL );
};


