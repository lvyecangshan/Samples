// MaintenancePage.cpp : implementation file
//

#include "stdafx.h"
#include "Machine.h"
#include "MaintenancePage.h"
#include "afxdialogex.h"
#include "PictureDlg.h"

// CMaintenancePage dialog

IMPLEMENT_DYNAMIC(CMaintenancePage, CDialogWithLang)

CMaintenancePage::CMaintenancePage(CWnd* pParent /*=NULL*/)
	: CDialogWithLang(CMaintenancePage::IDD, pParent)
{

}

CMaintenancePage::~CMaintenancePage()
{
}

void CMaintenancePage::DoDataExchange(CDataExchange* pDX)
{
	CDialogWithLang::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MAINTENANCE_LIST_MODULE, m_ListModule);
	DDX_Control(pDX, IDC_MAINTENANCE_TAB_IO, m_TabCtrlIO);
}


BEGIN_MESSAGE_MAP(CMaintenancePage, CDialogWithLang)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_MAINTENANCE_TAB_IO, &CMaintenancePage::OnTcnSelchangeMaintenanceTabIo)
	ON_LBN_SELCHANGE(IDC_MAINTENANCE_LIST_MODULE, &CMaintenancePage::OnLbnSelchangeMaintenanceListModule)
	ON_LBN_DBLCLK(IDC_MAINTENANCE_LIST_MODULE, &CMaintenancePage::OnLbnDblclkMaintenanceListModule)
END_MESSAGE_MAP()


// CMaintenancePage message handlers


BOOL CMaintenancePage::OnInitDialog()
{
	CDialogWithLang::OnInitDialog();

	// TODO:  Add extra initialization here
	m_ListModule.LoadModule();

	
	m_TabCtrlIO.Init(MACHINE.m_pRunCtrl);
	m_TabCtrlIO.SetCurPorduct(MACHINE.m_nDefaultProduct);
	if (MODULES.GetNumModule() > 0)
	{
		MODULES.SetCurModule(0);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CMaintenancePage::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	CRect rc;
	GetClientRect(&rc);
	dc.FillSolidRect(rc, RGB(245,252,255));
}


void CMaintenancePage::OnSize(UINT nType, int cx, int cy)
{
	CDialogWithLang::OnSize(nType, cx, cy);
	AdjustLayout();
	// TODO: Add your message handler code here
}

void CMaintenancePage::AdjustLayout()
{
	CRect rcView;
	GetClientRect(rcView);
	if (m_ListModule.GetSafeHwnd() != NULL)
	{
		m_ListModule.MoveWindow(0, 0, 150, rcView.bottom, SWP_SHOWWINDOW);
	}
	if (m_TabCtrlIO.GetSafeHwnd() != NULL)
	{
		m_TabCtrlIO.MoveWindow(150, 0, rcView.right-150, rcView.bottom, SWP_SHOWWINDOW);
	}
}


void CMaintenancePage::OnTcnSelchangeMaintenanceTabIo(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
	int iCurSelTab = m_TabCtrlIO.GetCurSel();
	if (iCurSelTab < 0)
	{
		return;
	}
	char        szTabText[100];
	TC_ITEM     tci;
	memset(szTabText, '\0', sizeof(szTabText));
	tci.mask        = TCIF_TEXT;
	tci.pszText = (LPWSTR)szTabText;
	tci.cchTextMax  = sizeof(szTabText)-1;

	m_TabCtrlIO.GetItem(iCurSelTab,&tci);
	CString strTxt = tci.pszText;

	if (strTxt == _T("输入/输出") || strTxt == _T("Input/Output"))
	{
		m_TabCtrlIO.ShowPage(0);
	}
	else if (strTxt == _T("Motor") || strTxt == _T("电机"))
	{
		m_TabCtrlIO.ShowPage(1);
	}
}


void CMaintenancePage::OnLbnSelchangeMaintenanceListModule()
{
	// TODO: Add your control notification handler code here
	MODULES.SetCurModule(0);
	int nCur = m_ListModule.GetCurSel();
	if (nCur < 0)
	{
		return;
	}

	MODULES.SetCurModule(nCur);
	m_TabCtrlIO.InsertTab();
}


void CMaintenancePage::OnLbnDblclkMaintenanceListModule()
{
	// TODO: 在此添加控件通知处理程序代码

	return;

	PictureDlg Dlg;
	int nCur = m_ListModule.GetCurSel();
	if (nCur < 0)
	{
		return;
	}
	MODULES.SetCurModule(nCur);
	Dlg.ShowDlg(MACHINE.m_pRunCtrl);
}


BOOL CMaintenancePage::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->wParam == VK_SPACE || pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	return CDialogWithLang::PreTranslateMessage(pMsg);
}
