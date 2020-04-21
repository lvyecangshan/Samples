// DemoRunDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DemoRun.h"
#include "DemoRunDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CDemoRunDlg dialog




CDemoRunDlg::CDemoRunDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDemoRunDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDemoRunDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MONITOR_MODULES, m_wndStatusTable);
}

BEGIN_MESSAGE_MAP(CDemoRunDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_START, &CDemoRunDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_STOP, &CDemoRunDlg::OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_RESET, &CDemoRunDlg::OnBnClickedBtnReset)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_MAINTENANCE, &CDemoRunDlg::OnBnClickedBtnMaintenance)
	ON_BN_CLICKED(IDC_BTN_CLOSE, &CDemoRunDlg::OnBnClickedBtnClose)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDemoRunDlg message handlers

BOOL CDemoRunDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
    //_CrtSetBreakAlloc(239);
	// TODO: Add extra initialization here
	MACHINE.Initialize(GetSafeHwnd(), NUM_OF_MOTORS, NUM_OF_INPUTS, NUM_OF_OUTPUTS, NUM_OF_EVENTS, NUM_OF_MACHINE_TIME);
	/////////
	// create the status table
	CRunTask *pRunTask[] = {
		MACHINE.m_pRunCtrl,
		MACHINE.m_pRunCtrl->GetRun(RUN_ONLOAD),
		MACHINE.m_pRunCtrl->GetRun(RUN_PNP_ARM),
		MACHINE.m_pRunCtrl->GetRun(RUN_UNLOAD),
	};
	m_wndStatusTable.Create(4, pRunTask, 0.3);
	////////
	SetTimer(100,500,NULL);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDemoRunDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDemoRunDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDemoRunDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CDemoRunDlg::OnBnClickedBtnStart()
{
	// TODO: Add your control notification handler code here
	MACHINE.m_pRunCtrl->Start();
}

void CDemoRunDlg::OnBnClickedBtnStop()
{
	// TODO: Add your control notification handler code here
	MACHINE.m_pRunCtrl->Stop();
}

void CDemoRunDlg::OnBnClickedBtnReset()
{
	// TODO: Add your control notification handler code here
}

void CDemoRunDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

		// update machine message
	    CStatic* pMessage = (CStatic*)GetDlgItem(IDC_MAIN_MESSAGE);
		switch(MACHINE.m_pRunCtrl->GetMcState())
		{
		case MC_BEGIN:
			pMessage->SetWindowText(_T("Begin"));
			//m_wndMachineStatus.SetBkColor(m_bBlinkStep ? RGB(255, 255, 255) : RGB(127,127,127));
			break;
		case MC_INITIALIZING:
			pMessage->SetWindowText(_T("Initializing"));
			break;
		case MC_RUNNING:
			pMessage->SetWindowText(_T("Running"));
			break;
		case MC_INITIALIZED:
		case MC_STOP_INIT:
		case MC_STOP_RUN:
			pMessage->SetWindowText(_T("Stop"));
			break;
		default:
			ASSERT(0);
			break;
		}

}

void CDemoRunDlg::OnBnClickedBtnMaintenance()
{
	// TODO: Add your control notification handler code here
	CRect rect;
	GetWindowRect(&rect);
	MACHINE.ShowMaintenanceDialog(this,rect);
}

void CDemoRunDlg::OnBnClickedBtnClose()
{
	// TODO: Add your control notification handler code here
	if( MACHINE.m_pRunCtrl->GetMcState() == MC_RUNNING )
	{
		AfxMessageBox( _T("Stop Machine Run before you close!"), MB_ICONSTOP);
		return;
	}
	//CDialogWithLang::OnDestroy();
	OnOK();;
}

void CDemoRunDlg::OnDestroy()
{
	MACHINE.Destroy();
	CLogWrite::Destroy();
	CDialog::OnDestroy();
	// TODO: Add your message handler code here
}
