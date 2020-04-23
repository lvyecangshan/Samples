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

// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Machine.h"

#include "MainFrm.h"
//#include "vld.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_COMMAND(ID_VIEW_CAPTION_BAR, &CMainFrame::OnViewCaptionBar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CAPTION_BAR, &CMainFrame::OnUpdateViewCaptionBar)
	ON_COMMAND(ID_TOOLS_OPTIONS, &CMainFrame::OnOptions)
	ON_COMMAND(ID_FILE_PRINT, &CMainFrame::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CMainFrame::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMainFrame::OnFilePrintPreview)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, &CMainFrame::OnUpdateFilePrintPreview)
	ON_COMMAND(ID_VIEW_OUTPUT_VIEW, &CMainFrame::OnViewOutputView)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OUTPUT_VIEW, &CMainFrame::OnUpdateViewOutputView)
	ON_COMMAND(ID_OPERATE_BTN_START, &CMainFrame::OnOperateBtnStart)
	ON_COMMAND(ID_OPERATE_BTN_STOP, &CMainFrame::OnOperateBtnStop)
	ON_COMMAND(ID_OPERATE_BTN_RESET, &CMainFrame::OnOperateBtnReset)
	ON_COMMAND(ID_OPERATE_BTN_RESTART, &CMainFrame::OnOperateBtnRestart)
	ON_COMMAND(ID_SYSTEM_BTN_LOGIN, &CMainFrame::OnSystemBtnLogin)
	ON_COMMAND(ID_SYSTEM_BTN_LOGOUT, &CMainFrame::OnSystemBtnLogout)
	ON_COMMAND(ID_SYSTEM_BTN_PASSWORD, &CMainFrame::OnSystemBtnPassword)
	ON_COMMAND(ID_SYSTEM_BTN_EXIT, &CMainFrame::OnSystemBtnExit)
	ON_WM_CLOSE()
	ON_UPDATE_COMMAND_UI(ID_SYSTEM_BTN_LOGIN, &CMainFrame::OnUpdateSystemBtnLogin)
	ON_UPDATE_COMMAND_UI(ID_SYSTEM_BTN_LOGOUT, &CMainFrame::OnUpdateSystemBtnLogout)
	ON_UPDATE_COMMAND_UI(ID_SYSTEM_BTN_PASSWORD, &CMainFrame::OnUpdateSystemBtnPassword)
	ON_UPDATE_COMMAND_UI(ID_OPERATE_BTN_START, &CMainFrame::OnUpdateOperateBtnStart)
	ON_UPDATE_COMMAND_UI(ID_OPERATE_BTN_RESET, &CMainFrame::OnUpdateOperateBtnReset)
	ON_UPDATE_COMMAND_UI(ID_OPERATE_BTN_RESTART, &CMainFrame::OnUpdateOperateBtnRestart)
	ON_MESSAGE(WM_COMM_INFO_MESSAGE, &CMainFrame::OnCommInfoMessage)
	ON_MESSAGE(WM_SHOW_ALARM_MESSAGE, &CMainFrame::OnCommAlarmMessage)

	// 手动/自动
	ON_COMMAND(ID_CHECK_MANUAL, &CMainFrame::OnCheckManual)
	ON_UPDATE_COMMAND_UI(ID_CHECK_MANUAL, &CMainFrame::OnUpdateCheckManual)
	//清料
	ON_COMMAND(ID_CHECK_CLEAR_MATERIAL, &CMainFrame::OnCheckClearMaterial)
	ON_UPDATE_COMMAND_UI(ID_CHECK_CLEAR_MATERIAL, &CMainFrame::OnUpdateCheckClearMaterial)
	//MES
	ON_COMMAND(ID_CHECK_MES, &CMainFrame::OnCheckMes)
	ON_UPDATE_COMMAND_UI(ID_CHECK_MES, &CMainFrame::OnUpdateCheckMes)
	//安全门
	ON_COMMAND(ID_CHECK_SAFE_DOOR, &CMainFrame::OnCheckSafeDoor)
	ON_UPDATE_COMMAND_UI(ID_CHECK_SAFE_DOOR, &CMainFrame::OnUpdateCheckSafeDoor)
	//CT
	ON_COMMAND(ID_CHECK_CT_LOG, &CMainFrame::OnEnableCTFunc)
	ON_UPDATE_COMMAND_UI(ID_CHECK_CT_LOG, &CMainFrame::OnUpdateEnableCTFunc)
	// 停止铝壳上料
	ON_COMMAND(ID_CHECK_STOP_PICK_SHELL, &CMainFrame::OnCheckStopPickShell)
	ON_UPDATE_COMMAND_UI(ID_CHECK_STOP_PICK_SHELL, &CMainFrame::OnUpdateStopPickShell)
	//屏蔽流水线信号
	ON_COMMAND(ID_CHECK_ENABLE_LINE_SINGLE, &CMainFrame::OnCheckEnableLineSingle)
	ON_UPDATE_COMMAND_UI(ID_CHECK_ENABLE_LINE_SINGLE, &CMainFrame::OnUpdateEnableLineSingle)
	// 屏蔽后段对接
	ON_COMMAND(ID_CHECK_ENBALE_BEHIND_DOCKING, &CMainFrame::OnCheckEnableSingleDocking)
	ON_UPDATE_COMMAND_UI(ID_CHECK_ENBALE_BEHIND_DOCKING, &CMainFrame::OnUpdateEnableSingleDocking)
	// 恢复生产
	ON_COMMAND(ID_CHECK_RECOVER_MACHINE, &CMainFrame::OnCheckRecoverMachine)
	ON_UPDATE_COMMAND_UI(ID_CHECK_RECOVER_MACHINE, &CMainFrame::OnUpdateCheckRecoverMachine)
	// 安东
	ON_COMMAND(ID_CHECK_ENABLE_ANDON, &CMainFrame::OnEnableAndon)
	ON_UPDATE_COMMAND_UI(ID_CHECK_ENABLE_ANDON, &CMainFrame::OnUpdateEnableAndon)
END_MESSAGE_MAP()

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	theApp.m_nAppLook = ID_VIEW_APPLOOK_OFF_2007_BLUE;//theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_OFF_2007_BLUE);
	m_bIsLogin = FALSE;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;
	// set the visual manager and style based on persisted value
	OnApplicationLook(theApp.m_nAppLook);

	m_wndRibbonBar.Create(this);
	m_wndRibbonBar.LoadFromResource(IDR_RIBBON);
	
	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	CString strTitlePane1;
	CString strTitlePane2;
	bNameValid = strTitlePane1.LoadString(IDS_STATUS_PANE1);
	ASSERT(bNameValid);
	bNameValid = strTitlePane2.LoadString(IDS_STATUS_PANE2);
	ASSERT(bNameValid);
	m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE1, strTitlePane1, TRUE), strTitlePane1);
	m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE2, strTitlePane2, TRUE), strTitlePane2);

	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// create docking windows
	if (!CreateDockingWindows())
	{
		TRACE0("Failed to create docking windows\n");
		return -1;
	}


	m_wndModuleView.EnableDocking(CBRS_ALIGN_ANY);
	m_wndOutputView.EnableDocking(CBRS_ALIGN_ANY);	
	DockPane(&m_wndOutputView);
	CDockablePane* pTabbedBar = NULL;
	m_wndModuleView.AttachToTabWnd(&m_wndOutputView, DM_SHOW, TRUE, &pTabbedBar);

	EnableDocking(CBRS_ALIGN_LEFT);
	EnableAutoHidePanes(CBRS_ALIGN_RIGHT);
	// Create a caption bar:

	if (!CreateCaptionBar())
	{
		TRACE0("Failed to create caption bar\n");
		return -1;      // fail to create
	}
	
	//// Outlook bar is created and docking on the left side should be allowed.
	InitializeRibbon();

	MACHINE.m_FramHwnd = m_hWnd;
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	//  the CREATESTRUCT cs
	cs.style&=~FWS_ADDTOTITLE;
	//m_strTitle = _T("海目星设备控制系统");
	m_strTitle = _T("");
	cs.cx=900;   
	cs.cy=600; 

	return TRUE;
}

BOOL CMainFrame::CreateOutlookBar(CMFCOutlookBar& bar, UINT uiID, CMFCShellTreeCtrl& tree, CCalendarBar& calendar, int nInitialWidth)
{
	bar.SetMode2003();

	BOOL bNameValid;
	CString strTemp;
	bNameValid = strTemp.LoadString(IDS_SHORTCUTS);
	ASSERT(bNameValid);
	if (!bar.Create(strTemp, this, CRect(0, 0, nInitialWidth, 32000), uiID, WS_CHILD | WS_VISIBLE | CBRS_LEFT))
	{
		return FALSE; // fail to create
	}

	CMFCOutlookBarTabCtrl* pOutlookBar = (CMFCOutlookBarTabCtrl*)bar.GetUnderlyingWindow();

	if (pOutlookBar == NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	pOutlookBar->EnableInPlaceEdit(TRUE);

	static UINT uiPageID = 1;

	// can float, can autohide, can resize, CAN NOT CLOSE
	DWORD dwStyle = AFX_CBRS_FLOAT | AFX_CBRS_AUTOHIDE | AFX_CBRS_RESIZE;

	CRect rectDummy(0, 0, 0, 0);
	const DWORD dwTreeStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	tree.Create(dwTreeStyle, rectDummy, &bar, 1200);
	bNameValid = strTemp.LoadString(IDS_FOLDERS);
	ASSERT(bNameValid);
	pOutlookBar->AddControl(&tree, strTemp, 2, TRUE, dwStyle);

	calendar.Create(rectDummy, &bar, 1201);
	bNameValid = strTemp.LoadString(IDS_CALENDAR);
	ASSERT(bNameValid);
	pOutlookBar->AddControl(&calendar, strTemp, 3, TRUE, dwStyle);

	bar.SetPaneStyle(bar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	pOutlookBar->SetImageList(theApp.m_bHiColorIcons ? IDB_PAGES_HC : IDB_PAGES, 24);
	pOutlookBar->SetToolbarImageList(theApp.m_bHiColorIcons ? IDB_PAGES_SMALL_HC : IDB_PAGES_SMALL, 16);
	pOutlookBar->RecalcLayout();

	BOOL bAnimation = theApp.GetInt(_T("OutlookAnimation"), TRUE);
	CMFCOutlookBarTabCtrl::EnableAnimation(bAnimation);

	bar.SetButtonsFont(&afxGlobalData.fontBold);

	return TRUE;
}

BOOL CMainFrame::CreateCaptionBar()
{
	if (!m_wndCaptionBar.Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, this, ID_VIEW_CAPTION_BAR, -1, TRUE))
	{
		TRACE0("Failed to create caption bar\n");
		return FALSE;
	}
	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;

	// Create class view
	CString strModuleView;
	bNameValid = strModuleView.LoadString(IDS_MODULE_VIEW);
	ASSERT(bNameValid);
	if (!m_wndModuleView.Create(strModuleView, this, CRect(0, 0, 200, 200), TRUE,ID_MODULE_VIEW , WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Class View window\n");
		return FALSE; // failed to create
	}

	//// Create file view
	CString strOutputView = _T("OutputView");
	if (!m_wndOutputView.Create(strOutputView, this, CRect(0, 0, 200, 200), TRUE, ID_OUTPUT_VIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create File View window\n");
		return FALSE; // failed to create
	}
	
	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG

// CMainFrame message handlers

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(TRUE);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

void CMainFrame::OnViewCaptionBar()
{
	m_wndCaptionBar.ShowWindow(m_wndCaptionBar.IsVisible() ? SW_HIDE : SW_SHOW);
	RecalcLayout(FALSE);
}

void CMainFrame::OnUpdateViewCaptionBar(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndCaptionBar.IsVisible());
}

void CMainFrame::OnOptions()
{
	CMFCRibbonCustomizeDialog *pOptionsDlg = new CMFCRibbonCustomizeDialog(this, &m_wndRibbonBar);
	ASSERT(pOptionsDlg != NULL);

	pOptionsDlg->DoModal();
	delete pOptionsDlg;
}

void CMainFrame::OnFilePrint()
{
	if (IsPrintPreview())
	{
		PostMessage(WM_COMMAND, AFX_ID_PREVIEW_PRINT);
	}
}

void CMainFrame::OnFilePrintPreview()
{
	if (IsPrintPreview())
	{
		PostMessage(WM_COMMAND, AFX_ID_PREVIEW_CLOSE);  // force Print Preview mode closed
	}
}

void CMainFrame::OnUpdateFilePrintPreview(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(IsPrintPreview());
}

void CMainFrame::OnViewOutputView()
{
	BOOL bAllIsVisible = m_wndModuleView.IsVisible() && m_wndOutputView.IsVisible();
	m_wndModuleView.ShowPane(bAllIsVisible ? FALSE : TRUE,FALSE,TRUE);
	m_wndOutputView.ShowPane(bAllIsVisible ? FALSE : TRUE,FALSE,FALSE);
}

void CMainFrame::OnUpdateViewOutputView(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_wndModuleView.IsVisible()&&m_wndOutputView.IsVisible());
}

void CMainFrame::OnOperateBtnStart()
{

	if(MACHINE.m_bManualAuto)
	{
		BLMessageBox(_T("需将设备切换为自动状态"), MB_ICONWARNING);
		return;
	}
	MACHINE.m_pRunCtrl->Start();
	
	
}

void CMainFrame::OnUpdateOperateBtnStart(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bIsLogin);
}

void CMainFrame::OnOperateBtnStop()
{
	MACHINE.m_pRunCtrl->Stop();
	MACHINE.m_bManualAuto = TRUE;
}

void CMainFrame::OnUpdateOperateBtnReset(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bIsLogin);
}

void CMainFrame::OnOperateBtnReset()
{
	MACHINE.m_pRunCtrl->Reset();
}

void CMainFrame::OnUpdateOperateBtnRestart(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bIsLogin);
}

void CMainFrame::OnOperateBtnRestart()
{
	int nStatus = MACHINE.m_pRunCtrl->GetMcState();
	if(nStatus == MC_INITIALIZING || nStatus == MC_RUNNING)
	{
		BLMessageBox(_T("设备运行中，不可进行整机复位"), MB_ICONERROR);
		return;
	}
	if (BLMessageBox(_T("整机复位 YES:是 NO: 不复位"),MB_YESNO) == IDNO)
	{
		return;
	}
	MACHINE.m_pRunCtrl->Restart();
	MACHINE.m_bManualAuto = TRUE;
}

void CMainFrame::OnSystemBtnLogin()
{
#ifdef DEMO
	if(IsNoHardware())
	{
		if (MACHINE.m_dbRecord.Login( DEFAUT_USER,DEFAUT_PWD))
		{
			m_bIsLogin = TRUE;
			::PostMessage(MACHINE.m_pMachineViewWnd->m_hWnd,WM_USER_LOGINOUT,0,0);
			return ;
		}
	}
	
#endif

	// TODO: Add your command handler code here
	CLoginDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		m_bIsLogin = TRUE;
		::PostMessage(MACHINE.m_pMachineViewWnd->m_hWnd,WM_USER_LOGINOUT,0,0);
	}
}

void CMainFrame::OnUpdateSystemBtnLogin(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_bIsLogin);
}

void CMainFrame::OnSystemBtnLogout()
{
	// TODO: Add your command handler code here
	MACHINE.m_dbRecord.Logout();
	m_bIsLogin = FALSE;
	::PostMessage(MACHINE.m_pMachineViewWnd->m_hWnd,WM_USER_LOGINOUT,0,0);
}

void CMainFrame::OnUpdateSystemBtnLogout(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bIsLogin);
}

void CMainFrame::OnSystemBtnPassword()
{
	// TODO: Add your command handler code here
	if (MACHINE.m_dbRecord.GetCurUserLevel() == 0)
	{
		CUserManageDlg dlg;
		dlg.DoModal();
	}
	if (MACHINE.m_dbRecord.GetCurUserLevel() > 0)
	{
		CModifyPassDlg dlg;
		dlg.DoModal();
	}
}

void CMainFrame::OnUpdateSystemBtnPassword(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bIsLogin);
}

void CMainFrame::OnSystemBtnExit()
{
	// TODO: Add your command handler code here
	OnClose();
}

void CMainFrame::OnClose()
{
#ifdef DEMO
	if(IsNoHardware())
	{
		exit(0);
	}
#endif
	int iMcState = MACHINE.m_pRunCtrl->GetMcState();

	if(iMcState == MC_INITIALIZING
		|| iMcState == MC_RUNNING
		|| iMcState == MC_INIT_ERR
		|| iMcState == MC_RUN_JAM)
	{
		AfxMessageBox(_T("设备正在运行!\n停止运行设备后退出"));
		return;
	}

	if(AfxMessageBox(_T("确认退出?"),MB_ICONQUESTION | MB_OKCANCEL) != IDOK)
	{
		return;
	}

	

	IniWriteInt(_T("Produ"), _T("m_nOnloadCount"), MACHINE.m_nOnloadCount, ::GetAbsolutePathName(MACHINE_CFG));
	IniWriteInt(_T("Produ"), _T("m_nOffloadCount"), MACHINE.m_nOffloadCount, ::GetAbsolutePathName(MACHINE_CFG));
	IniWriteInt(_T("Produ"), _T("m_nNgCount"), MACHINE.m_nNgCount, ::GetAbsolutePathName(MACHINE_CFG));

	IniWriteInt(_T("StatusInfo"), _T("m_dwMachineRunTime"), MACHINE.m_dwMachineRunTime, ::GetAbsolutePathName(MACHINE_CFG));
	IniWriteInt(_T("StatusInfo"), _T("m_dwProduceTime"), MACHINE.m_dwProduceTime, ::GetAbsolutePathName(MACHINE_CFG));
	IniWriteInt(_T("StatusInfo"), _T("m_dwWaitMaterialTime"), MACHINE.m_dwWaitMaterialTime, ::GetAbsolutePathName(MACHINE_CFG));
	IniWriteInt(_T("StatusInfo"), _T("m_dwMachineAlarmTime"), MACHINE.m_dwMachineAlarmTime, ::GetAbsolutePathName(MACHINE_CFG));

	IniWriteInt(_T("Produ"), _T("ScanNgCount"), MACHINE.m_nScanNgCount, ::GetAbsolutePathName(MACHINE_CFG));
	IniWriteInt(_T("Produ"), _T("UploadMesNgCount"), MACHINE.m_nUploadMesNgCount, ::GetAbsolutePathName(MACHINE_CFG));

	IniWriteInt(_T("Produ"), _T("ShellStation1PressNgCount"), MACHINE.m_nShellStationPressNgCount[0], ::GetAbsolutePathName(MACHINE_CFG));
	IniWriteInt(_T("Produ"), _T("ShellStation2PressNgCount"), MACHINE.m_nShellStationPressNgCount[1], ::GetAbsolutePathName(MACHINE_CFG));

	CString strTemp;
	for (int i = 0; i < 12; i++)
	{
		strTemp.Format(_T("EachPartOffload%d"), i);
		IniWriteInt(_T("Produ"), strTemp, MACHINE.m_nEachPartOffload[i], ::GetAbsolutePathName(MACHINE_CFG));
	}
	CMachineCtrl::Destroy();
	CFrameWndEx::OnClose();
}

LRESULT CMainFrame::OnCommInfoMessage( WPARAM wParam, LPARAM lParam )
{
	if (m_wndOutputView.GetSafeHwnd() != NULL && m_wndOutputView.m_wndOutputCommInfo.GetSafeHwnd() != NULL)
	{
		if (m_wndOutputView.m_wndOutputCommInfo.GetCount() > 100)
		{
			m_wndOutputView.m_wndOutputCommInfo.ResetContent();
		}
		CString strMsg = COleDateTime::GetCurrentTime().Format(_T("%d/%m/%Y %H:%M:%S  ")) + (CString)(LPCTSTR)lParam;
		
		m_wndOutputView.m_wndOutputCommInfo.AddString(strMsg);
	}
	return 0;
}

LRESULT CMainFrame::OnCommAlarmMessage(WPARAM wParam, LPARAM lParam)
{
	if (m_wndOutputView.GetSafeHwnd() != NULL && m_wndOutputView.m_wndOutputAlarmInfo.GetSafeHwnd() != NULL)
	{
		if (m_wndOutputView.m_wndOutputAlarmInfo.GetCount() > 100)
		{
			m_wndOutputView.m_wndOutputAlarmInfo.ResetContent();
		}

		CString strMsg = COleDateTime::GetCurrentTime().Format(_T("%d/%m/%Y %H:%M:%S  ")) + (CString)(LPCTSTR)lParam;

		m_wndOutputView.m_wndOutputAlarmInfo.AddString(strMsg);
	}
	return 0;
}

void CMainFrame::UpdateLanguage()
{
	if (m_wndModuleView.GetSafeHwnd() != NULL)
	{
		if (GetLanguage() == _T("CHS"))
		{
			m_wndModuleView.SetWindowTextW(_T("模组窗口"));
		}
		else
		{
			m_wndModuleView.SetWindowTextW(_T("ModuleView"));
		}
		m_wndModuleView.UpdateLanguage();
	}
	if (m_wndOutputView.GetSafeHwnd() != NULL)
	{
		if (GetLanguage() == _T("CHS"))
		{
			m_wndOutputView.SetWindowTextW(_T("输出窗口"));
		}
		else
		{
			m_wndOutputView.SetWindowTextW(_T("OutputView"));
		}
		m_wndOutputView.UpdateLanguage();
	}
	RecalcLayout();
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN || pMsg->message == VK_SPACE )
	{
		return TRUE;
	}
	return CFrameWndEx::PreTranslateMessage(pMsg);
}

void CMainFrame::InitializeRibbon()
{
}

void CMainFrame::DoDataExchange(CDataExchange* pDX)
{
	// TODO: 在此添加专用代码和/或调用基类
	CFrameWndEx::DoDataExchange(pDX);
}

void CMainFrame::OnCheckManual()
{
	// TODO: Add your command handler code here
	if(MACHINE.m_bManualAuto)
	{
		MACHINE.m_bManualAuto = FALSE;
		//BLMessageBox(_T("设备转为自动状态"), MB_ICONWARNING);
	}
	else
	{
		MACHINE.m_bManualAuto = TRUE;
		//BLMessageBox(_T("设备转为人工手动状态"), MB_ICONWARNING);
	}
}

void CMainFrame::OnUpdateCheckManual(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if (MACHINE.m_pRunCtrl == NULL)
	{
		return;
	}
	int nStatues = MACHINE.m_pRunCtrl->GetMcState();
	pCmdUI->SetCheck(MACHINE.m_bManualAuto);
	if (MACHINE.m_dbRecord.GetCurUserLevel() < 0 || MACHINE.m_dbRecord.GetCurUserLevel() >= 3 || nStatues == MC_RUNNING || nStatues == MC_INITIALIZING)
	{
		pCmdUI->Enable(FALSE);
	}
}

void CMainFrame::OnCheckClearMaterial()
{
	// TODO: Add your command handler code here
	if(MACHINE.m_bClearMaterial)
	{
		if(BLMessageBox(_T("设备将转为正常生产状态？"), MB_ICONWARNING|MB_YESNO) != IDYES)
		{
			return;
		}
		MACHINE.m_bClearMaterial = FALSE;
	}
	else
	{
		if(BLMessageBox(_T("设备将转为清料状态？"), MB_ICONWARNING|MB_YESNO) != IDYES)
		{
			return;
		}
		MACHINE.m_bClearMaterial = TRUE;
	}
}

void CMainFrame::OnUpdateCheckClearMaterial(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if (MACHINE.m_pRunCtrl == NULL)
	{
		return;
	}
	int nStatues = MACHINE.m_pRunCtrl->GetMcState();
	pCmdUI->SetCheck(MACHINE.m_bClearMaterial);
	if (MACHINE.m_dbRecord.GetCurUserLevel() < 0 || MACHINE.m_dbRecord.GetCurUserLevel() >= 3 || nStatues == MC_RUNNING || nStatues == MC_INITIALIZING)
	{
		pCmdUI->Enable(FALSE);
	}
}

void CMainFrame::OnCheckMes()
{
	if(MACHINE.m_bCheckMes)
	{
		if(BLMessageBox(_T("取消数据上传功能?"), MB_ICONWARNING|MB_YESNO) != IDYES)
		{
			return;
		}
		MACHINE.m_bCheckMes = FALSE;
	}
	else
	{
		MACHINE.m_bCheckMes = TRUE;
	}
	::IniWriteBoolean(_T("Machine"), _T("m_bCheckMes"), MACHINE.m_bCheckMes, GetAbsolutePathName(MACHINE_CFG));
}

void CMainFrame::OnUpdateCheckMes(CCmdUI *pCmdUI)
{
	if (MACHINE.m_pRunCtrl == NULL)
	{
		return;
	}
	int nStatues = MACHINE.m_pRunCtrl->GetMcState();
	pCmdUI->SetCheck(MACHINE.m_bCheckMes);
	if (MACHINE.m_dbRecord.GetCurUserLevel() < 0 || MACHINE.m_dbRecord.GetCurUserLevel() >= 3 || nStatues == MC_RUNNING || nStatues == MC_INITIALIZING)
	{
		pCmdUI->Enable(FALSE);
	}
}

void CMainFrame::OnCheckSafeDoor()
{
	// TODO: Add your command handler code here
	if (MACHINE.m_bUseSafeDoor)
	{
		MACHINE.m_bUseSafeDoor = FALSE;
	}
	else
	{
		MACHINE.m_bUseSafeDoor = TRUE;
	}
}

void CMainFrame::OnUpdateCheckSafeDoor(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if (MACHINE.m_pRunCtrl == NULL)
	{
		return;
	}
	int nStatues = MACHINE.m_pRunCtrl->GetMcState();
	pCmdUI->SetCheck(MACHINE.m_bUseSafeDoor);
	if (MACHINE.m_dbRecord.GetCurUserLevel() < 0 || MACHINE.m_dbRecord.GetCurUserLevel() >= 2 || nStatues == MC_RUNNING || nStatues == MC_INITIALIZING)
	{
		pCmdUI->Enable(FALSE);
	}
}

//CT
void CMainFrame::OnEnableCTFunc()
{
	if(!MACHINE.m_bCreateCTLog)
	{
		if(BLMessageBox(_T("确认添加模组CT统计?"), MB_ICONWARNING|MB_YESNO) != IDYES)
		{
			return;
		}
		MACHINE.m_bCreateCTLog = TRUE;
	}
	else
	{
		MACHINE.m_bCreateCTLog = FALSE;
	}
}

void CMainFrame::OnUpdateEnableCTFunc(CCmdUI *pCmdUI)
{
	if (MACHINE.m_pRunCtrl == NULL)
	{
		return;
	}
	int nStatues = MACHINE.m_pRunCtrl->GetMcState();		
	pCmdUI->SetCheck(MACHINE.m_bCreateCTLog);
	if (MACHINE.m_dbRecord.GetCurUserLevel() < 0 || MACHINE.m_dbRecord.GetCurUserLevel() >= 2 || nStatues == MC_RUNNING || nStatues == MC_INITIALIZING)
	{
		pCmdUI->Enable(FALSE);
	}
}

// 停止铝壳上料
void CMainFrame::OnCheckStopPickShell()
{
	if(!MACHINE.m_bStopPickShell)
	{
		if(BLMessageBox(_T("确认停止抓取铝壳?"), MB_ICONWARNING|MB_YESNO) != IDYES)
		{
			return;
		}
		MACHINE.m_bStopPickShell = TRUE;
	}
	else
	{
		MACHINE.m_bStopPickShell = FALSE;
	}
}

void CMainFrame::OnUpdateStopPickShell(CCmdUI *pCmdUI)
{
	if (MACHINE.m_pRunCtrl == NULL)
	{
		return;
	}
	int nStatues = MACHINE.m_pRunCtrl->GetMcState();
	pCmdUI->SetCheck(MACHINE.m_bStopPickShell);
	if (MACHINE.m_dbRecord.GetCurUserLevel() < 0 || MACHINE.m_dbRecord.GetCurUserLevel() >= 3 || nStatues == MC_RUNNING || nStatues == MC_INITIALIZING)
	{
		pCmdUI->Enable(FALSE);
	}
}

//屏蔽流水线信号
void CMainFrame::OnCheckEnableLineSingle()
{
	if(!MACHINE.m_bUnEnableLineSingle)
	{
		if(BLMessageBox(_T("确认屏蔽流水线对接?"), MB_ICONWARNING|MB_YESNO) != IDYES)
		{
			return;
		}
		MACHINE.m_bUnEnableLineSingle = TRUE;
	}
	else
	{
		MACHINE.m_bUnEnableLineSingle = FALSE;
	}
}

void CMainFrame::OnUpdateEnableLineSingle(CCmdUI *pCmdUI)
{
	if (MACHINE.m_pRunCtrl == NULL)
	{
		return;
	}
	int nStatues = MACHINE.m_pRunCtrl->GetMcState();
	pCmdUI->SetCheck(MACHINE.m_bUnEnableLineSingle);
	if (MACHINE.m_dbRecord.GetCurUserLevel() < 0 || MACHINE.m_dbRecord.GetCurUserLevel() >= 2 || nStatues == MC_RUNNING || nStatues == MC_INITIALIZING)
	{
		pCmdUI->Enable(FALSE);
	}
}

// 屏蔽后段对接
void CMainFrame::OnCheckEnableSingleDocking()
{
	if(!MACHINE.m_bUnEnableDocking)
	{
		if(BLMessageBox(_T("确认屏蔽与入壳后段对接?"), MB_ICONWARNING|MB_YESNO) != IDYES)
		{
			return;
		}
		MACHINE.m_bUnEnableDocking = TRUE;

		if(MACHINE.m_pRobotOffload != NULL)
		{
			MACHINE.m_pRobotOffload->m_bEnableDocking = FALSE;
		}
	}
	else
	{
		MACHINE.m_bUnEnableDocking = FALSE;

		if(MACHINE.m_pRobotOffload != NULL)
		{
			MACHINE.m_pRobotOffload->m_bEnableDocking = TRUE;
		}
	}
}

void CMainFrame::OnUpdateEnableSingleDocking(CCmdUI *pCmdUI)
{
	if (MACHINE.m_pRunCtrl == NULL)
	{
		return;
	}
	int nStatues = MACHINE.m_pRunCtrl->GetMcState();
	pCmdUI->SetCheck(MACHINE.m_bUnEnableDocking);
	if (MACHINE.m_dbRecord.GetCurUserLevel() < 0 || MACHINE.m_dbRecord.GetCurUserLevel() >= 2 || nStatues == MC_RUNNING || nStatues == MC_INITIALIZING)
	{
		pCmdUI->Enable(FALSE);
	}
}


void CMainFrame::OnCheckRecoverMachine()
{
	// TODO: 在此添加命令处理程序代码
	if (!MACHINE.m_bReCoverMachine)
	{
		if(BLMessageBox(_T("恢复生产?"), MB_ICONWARNING|MB_YESNO) != IDYES)
		{
			return;
		}
		MACHINE.m_bReCoverMachine = TRUE;
	}
	else
	{
		if(BLMessageBox(_T("取消恢复生产?"), MB_ICONWARNING|MB_YESNO) != IDYES)
		{
			return;
		}
		MACHINE.m_bReCoverMachine = FALSE;
	}
}


void CMainFrame::OnUpdateCheckRecoverMachine(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if (MACHINE.m_pRunCtrl == NULL)
	{
		return;
	}
	int nStatues = MACHINE.m_pRunCtrl->GetMcState();
	pCmdUI->SetCheck(MACHINE.m_bReCoverMachine);

	if (MACHINE.m_dbRecord.GetCurUserLevel() < 0 || MACHINE.m_dbRecord.GetCurUserLevel() >= 2 || nStatues == MC_RUNNING || nStatues == MC_INITIALIZING)
	{
		pCmdUI->Enable(FALSE);
	}
}

void CMainFrame::OnEnableAndon()
{
	if (MACHINE.m_bEnableAndon)
	{
		if(BLMessageBox(_T("取消安东上传功能?"), MB_ICONWARNING|MB_YESNO) != IDYES)
		{
			return;
		}
		MACHINE.m_bEnableAndon = FALSE;
	}
	else
	{
		if(BLMessageBox(_T("开启安东上传功能?"), MB_ICONWARNING|MB_YESNO) != IDYES)
		{
			return;
		}
		MACHINE.m_bEnableAndon = TRUE;
	}
	IniWriteBoolean(_T("Andon"), _T("EnableAndon"), MACHINE.m_bEnableAndon, ::GetAbsolutePathName(MACHINE_CFG));
}

void CMainFrame::OnUpdateEnableAndon(CCmdUI *pCmdUI)
{
	if (MACHINE.m_pRunCtrl == NULL)
	{
		return;
	}
	int nStatues = MACHINE.m_pRunCtrl->GetMcState();
	pCmdUI->SetCheck(MACHINE.m_bEnableAndon);

	if (MACHINE.m_dbRecord.GetCurUserLevel() < 0 || MACHINE.m_dbRecord.GetCurUserLevel() >= 2 || nStatues == MC_RUNNING || nStatues == MC_INITIALIZING)
	{
		pCmdUI->Enable(FALSE);
	}
}
