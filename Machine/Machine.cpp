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

// Machine.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "Machine.h"
#include "MainFrm.h"

#include "MachineDoc.h"
#include "MachineView.h"
#include "GenerateDump.h"

#include "MachineAbout.h"
#include "MachineRegeditDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMachineApp

BEGIN_MESSAGE_MAP(CMachineApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CMachineApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CMachineApp construction

CMachineApp::CMachineApp()
{
	m_bHiColorIcons = TRUE;

	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("Machine.AppID.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CMachineApp object

CMachineApp theApp;


// CMachineApp initialization

BOOL CMachineApp::InitInstance()
{
	LOGFONT logfont = {0};
	:: SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(LOGFONT), &logfont, 0);
	logfont.lfHeight = LONG(logfont.lfHeight * 1.5);
	//strcpy(logfont.lfFaceName , _T("微软雅黑"));
	afxGlobalData.SetMenuFont(&logfont,true);
	//JDSERVER.JD_Create(5544);
	SetPriorityClass(GetCurrentProcess(),HIGH_PRIORITY_CLASS);
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	HANDLE hMutex =CreateMutex(0,TRUE,_T("WB"));
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		ReleaseMutex(hMutex);
		CloseHandle(hMutex);
		AfxMessageBox(_T("系统中已经运行该程序!"));
		exit(0);
	}
	CWinAppEx::InitInstance();
	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	EnableTaskbarInteraction(FALSE);

	// AfxInitRichEdit2() is required to use RichEdit control	
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)


	InitContextMenuManager();
	InitShellManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CMachineDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CMachineView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	SetUnhandledExceptionFilter(MyUnhandledExceptionFilter);
	return TRUE;
}

int CMachineApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// CMachineApp message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	void AdjustLayout();
private:
	CFont m_Font;
	CTabCtrlD m_TabCtrl;
	CMachineRegeditDlg	m_DlgRegedit;
	CMachineAbout		m_DlgMachineAbout;
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB, m_TabCtrl);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CAboutDlg::OnTcnSelchangeTab)
	ON_WM_SIZE()
END_MESSAGE_MAP()

// App command to run the dialog
void CMachineApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CMachineApp customization load/save methods

void CMachineApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CMachineApp::LoadCustomState()
{

}
void CMachineApp::SaveCustomState()
{
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_DlgMachineAbout.Create(IDD_DLG_MACHINE_ABOUT,&m_TabCtrl);
	m_DlgRegedit.Create(IDD_DLG_MACHINE_REGEDIT,&m_TabCtrl);
	

	m_DlgMachineAbout.ShowWindow(SW_SHOW);
	m_DlgRegedit.ShowWindow(SW_HIDE);
	

	m_TabCtrl.InsertItem(0,_T("软 件 信 息"));
	m_TabCtrl.InsertItem(1,_T("序列号注册"));

	AdjustLayout();

	m_Font.CreateFont(20,8,0,0,400,0,0,0,1,0,0,0,0,_T("微软雅黑"));
	m_TabCtrl.SetFont(&m_Font);

	m_TabCtrl.SetItemSize(CSize(1,30));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CAboutDlg::AdjustLayout()
{
	CRect rcView;
	GetClientRect(rcView);

	if(m_TabCtrl.GetSafeHwnd() != NULL)
	{
		m_TabCtrl.MoveWindow(0, 0, rcView.right, rcView.bottom, SWP_SHOWWINDOW);
		CRect rcTab;
		m_TabCtrl.GetClientRect(rcTab);
		rcTab.top += 30;

		if(m_DlgMachineAbout.GetSafeHwnd() != NULL)
		{
			m_DlgMachineAbout.MoveWindow(&rcTab);
		}

		if(m_DlgRegedit.GetSafeHwnd() != NULL)
		{
			m_DlgRegedit.MoveWindow(&rcTab);
		}
	}
}

void CAboutDlg::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码

	int iCurSelTab = m_TabCtrl.GetCurSel();
	
	if(iCurSelTab == 0)
	{
		m_DlgMachineAbout.ShowWindow(SW_SHOW);
		m_DlgRegedit.ShowWindow(SW_HIDE);
	}
	else
	{
		m_DlgMachineAbout.ShowWindow(SW_HIDE);
		m_DlgRegedit.ShowWindow(SW_SHOW);
	}

	*pResult = 0;
}


void CAboutDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	AdjustLayout();
	// TODO: 在此处添加消息处理程序代码
}
