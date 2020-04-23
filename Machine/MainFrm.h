// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// http://msdn.microsoft.com/officeui.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// MainFrm.h : interface of the CMainFrame class
//

#pragma once
#include "CalendarBar.h"
#include "Resource.h"
#include "ModuleView.h"
#include "OutputView.h"

class COutlookBar : public CMFCOutlookBar
{
	virtual BOOL AllowShowOnPaneMenu() const { return TRUE; }
	virtual void GetPaneName(CString& strName) const { BOOL bNameValid = strName.LoadString(IDS_OUTLOOKBAR); ASSERT(bNameValid); if (!bNameValid) strName.Empty(); }
};

class CMainFrame : public CFrameWndEx
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	BOOL m_bIsLogin;
// Operations
public:
	void UpdateLanguage();
// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void Init(CRunCtrl *pRunCTrl = NULL){m_wndModuleView.init(TRUE,pRunCTrl);}
// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnViewCaptionBar();
	afx_msg void OnUpdateViewCaptionBar(CCmdUI* pCmdUI);
	afx_msg void OnOptions();
	afx_msg void OnFilePrint();
	afx_msg void OnFilePrintPreview();
	afx_msg void OnUpdateFilePrintPreview(CCmdUI* pCmdUI);
	afx_msg LRESULT OnCommInfoMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnViewOutputView();
	afx_msg void OnUpdateViewOutputView(CCmdUI *pCmdUI);
	afx_msg void OnOperateBtnStart();
	afx_msg void OnOperateBtnStop();
	afx_msg void OnOperateBtnReset();
	afx_msg void OnOperateBtnRestart();
	afx_msg void OnSystemBtnLogin();
	afx_msg void OnSystemBtnLogout();
	afx_msg void OnSystemBtnPassword();
	afx_msg void OnSystemBtnExit();
	afx_msg void OnClose();
	afx_msg void OnUpdateSystemBtnLogin(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSystemBtnLogout(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSystemBtnPassword(CCmdUI *pCmdUI);
	afx_msg void OnUpdateOperateBtnStart(CCmdUI *pCmdUI);
	afx_msg void OnUpdateOperateBtnReset(CCmdUI *pCmdUI);
	afx_msg void OnUpdateOperateBtnRestart(CCmdUI *pCmdUI);
	afx_msg LRESULT OnCommAlarmMessage(WPARAM wParam, LPARAM lParam);
	//MES
	afx_msg void OnCheckMes();
	afx_msg void OnUpdateCheckMes(CCmdUI *pCmdUI);
	//自动/手动
	afx_msg void OnCheckManual();
	afx_msg void OnUpdateCheckManual(CCmdUI *pCmdUI);
	//清料
	afx_msg void OnCheckClearMaterial();
	afx_msg void OnUpdateCheckClearMaterial(CCmdUI *pCmdUI);
	//安全门
	afx_msg void OnCheckSafeDoor();
	afx_msg void OnUpdateCheckSafeDoor(CCmdUI *pCmdUI);
	//CT
	afx_msg void OnEnableCTFunc();
	afx_msg void OnUpdateEnableCTFunc(CCmdUI *pCmdUI);
   // 停止铝壳上料
	afx_msg void OnCheckStopPickShell();
	afx_msg void OnUpdateStopPickShell(CCmdUI *pCmdUI);
	//屏蔽流水线信号
	afx_msg void OnCheckEnableLineSingle();
	afx_msg void OnUpdateEnableLineSingle(CCmdUI *pCmdUI);
	// 屏蔽后段对接
	afx_msg void OnCheckEnableSingleDocking();
	afx_msg void OnUpdateEnableSingleDocking(CCmdUI *pCmdUI);
	// 恢复生产
	afx_msg void OnCheckRecoverMachine();
	afx_msg void OnUpdateCheckRecoverMachine(CCmdUI *pCmdUI);
	// 安东
	afx_msg void OnEnableAndon();
	afx_msg void OnUpdateEnableAndon(CCmdUI *pCmdUI);
	
	DECLARE_MESSAGE_MAP()

public:
	//初始化Ribbon
	void InitializeRibbon();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	BOOL CreateCaptionBar();
	int FindFocusedOutlookWnd(CMFCOutlookBarTabCtrl** ppOutlookWnd);
	CMFCOutlookBarTabCtrl* FindOutlookParent(CWnd* pWnd);
	BOOL CreateOutlookBar(CMFCOutlookBar& bar, UINT uiID, CMFCShellTreeCtrl& tree, CCalendarBar& calendar, int nInitialWidth);
	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);

	// Generated message map functions

public:
	// 自定义函数
protected:
	CMFCOutlookBarTabCtrl* m_pCurrOutlookWnd;
	CMFCOutlookBarPane*    m_pCurrOutlookPage;
	
public:
	CDJPRibbonBar   m_wndRibbonBar;
	CModuleView		m_wndModuleView;
	COutputView		m_wndOutputView;

protected:
	CMFCRibbonApplicationButton m_MainButton;
	CMFCToolBarImages m_PanelImages;
	CMFCRibbonStatusBar  m_wndStatusBar;
	COutlookBar       m_wndNavigationBar;
	CMFCShellTreeCtrl m_wndTree;
	CCalendarBar      m_wndCalendar;
	CMFCCaptionBar    m_wndCaptionBar;

public:
	// 自定义变量
	
};


