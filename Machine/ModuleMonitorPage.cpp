// ModuleMonitorPage.cpp : implementation file
//

#include "stdafx.h"
#include "Machine.h"
#include "ModuleMonitorPage.h"
#include "afxdialogex.h"


// CModuleMonitorPage dialog
#define UPDATE_TIME_TIMER 99

IMPLEMENT_DYNAMIC(CModuleMonitorPage, CDialogWithLang)

	CModuleMonitorPage::CModuleMonitorPage(CWnd* pParent /*=NULL*/)
	: CDialogWithLang(CModuleMonitorPage::IDD, pParent)
{

}

CModuleMonitorPage::~CModuleMonitorPage()
{
}

void CModuleMonitorPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogWithLang::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MODULE_MONITOR_LIST, m_listModule);
}


BEGIN_MESSAGE_MAP(CModuleMonitorPage, CDialogWithLang)
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CModuleMonitorPage message handlers


void CModuleMonitorPage::OnSize(UINT nType, int cx, int cy)
{
	CDialogWithLang::OnSize(nType, cx, cy);
	AdjustLayout();
	// TODO: Add your message handler code here
}

void CModuleMonitorPage::AdjustLayout()
{
	CRect rcView;
	GetClientRect(rcView);
	if (m_listModule.GetSafeHwnd() != NULL)
	{
		m_listModule.MoveWindow(0, 0, rcView.right, rcView.bottom, SWP_SHOWWINDOW);
	}
}


BOOL CModuleMonitorPage::OnInitDialog()
{
	CDialogWithLang::OnInitDialog();

	// TODO:  Add extra initialization here
	if (m_listModule.GetSafeHwnd() != NULL)
	{
		m_listModule.SetExtendedStyle( m_listModule.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
		m_listModule.DeleteAllItems();

		m_listModule.DeleteColumn(0);
		m_listModule.InsertColumn(0, _T("模块名称"), 0, 200);
		m_listModule.InsertColumn(1, _T("运行信息"), 0, 550);
		m_listModule.InsertColumn(2, _T("使能状态"), 0, 100);

		for(int i = 0; i < MACHINE.m_pRunCtrl->GetRunCount(); i++)
		{
			m_listModule.InsertItem(i,MACHINE.m_pRunCtrl->GetRun(i)->m_strName);
			if (!MACHINE.m_pRunCtrl->GetRun(i)->IsModuleEnable())
			{
				m_listModule.SetItemText(i,2,_T("停 用"));
			}
			else
			{
				if (((CRunProcess*)MACHINE.m_pRunCtrl->GetRun(i))->m_bDryRun)
				{
					m_listModule.SetItemText(i,2,_T("空运行"));
				}
				else
				{
					m_listModule.SetItemText(i,2,_T("启用中"));
				}
			}
		}
	}
	SetTimer(UPDATE_TIME_TIMER, 1000, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CModuleMonitorPage::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	CDialogWithLang::OnTimer(nIDEvent);

	if (m_listModule.GetSafeHwnd() != NULL)
	{
		for(int i = 0; i < MACHINE.m_pRunCtrl->GetRunCount(); i++)
		{
			CString strMsg;
			strMsg.Format(_T("%s:%s"),MACHINE.m_pRunCtrl->GetRun(i)->GetMsg(),((CRunProcess*)MACHINE.m_pRunCtrl->GetRun(i))->IsRunning() ?  _T("Running") :_T("Stop"));
			m_listModule.SetItemText(i, 1,strMsg);
			if (!MACHINE.m_pRunCtrl->GetRun(i)->IsModuleEnable())
			{
				m_listModule.SetItemText(i,2,(GetLanguage() == _T("CHS")) ?_T("停 用"): _T("Disable"));
			}
			else
			{
				if (((CRunProcess*)MACHINE.m_pRunCtrl->GetRun(i))->m_bDryRun)
				{
					m_listModule.SetItemText(i,2,(GetLanguage() == _T("CHS")) ?_T("空运行"):_T("DryRun"));
				}
				else
				{
					m_listModule.SetItemText(i,2,(GetLanguage() == _T("CHS")) ? _T("启用中"): _T("Enable"));
				}
			}
		}
	}
}

void CModuleMonitorPage::UpdateLanguage()
{
	for(int i = 0; i < MACHINE.m_pRunCtrl->GetRunCount(); i++)
	{
		CString strMsg;
		strMsg = (GetLanguage() == _T("CHS"))? MACHINE.m_pRunCtrl->GetRun(i)->m_strName : ((CRunProcess*)MACHINE.m_pRunCtrl->GetRun(i))->m_strProcess;
		m_listModule.SetItemText(i, 0,strMsg);
	}
	CListCtrl *pCtrl = (CListCtrl*)GetDlgItem(IDC_MODULE_MONITOR_LIST);
	if (pCtrl == NULL)
	{
		return;
	}
	LVCOLUMN   cl1,cl2,cl3;
	if (GetLanguage() == _T("CHS"))
	{
		cl1.pszText = _T("模块名称"); 
		cl2.pszText = _T("运行信息");
		cl3.pszText = _T("使能状态");
	}
	else
	{
		cl1.pszText = _T("ModuleName"); 
		cl2.pszText = _T("Message"); 
		cl3.pszText = _T("Run Model");
	}

	pCtrl->SetColumn(0,&cl1);
	pCtrl->SetColumn(1,&cl2);
	pCtrl->SetColumn(2,&cl3);
	CDialogWithLang::UpdateLanguage();
}


BOOL CModuleMonitorPage::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->wParam == VK_SPACE || pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	return CDialogWithLang::PreTranslateMessage(pMsg);
}
