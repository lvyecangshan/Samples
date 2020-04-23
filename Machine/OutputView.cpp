// OutputView.cpp : implementation file
//

#include "stdafx.h"
#include "Machine.h"
#include "OutputView.h"

COutputList::COutputList()
{
}

COutputList::~COutputList()
{
}

BEGIN_MESSAGE_MAP(COutputList, CListBox)
	ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()
// COutputView

IMPLEMENT_DYNAMIC(COutputView, CDockablePane)

	COutputView::COutputView()
{

}

COutputView::~COutputView()
{
}


BEGIN_MESSAGE_MAP(COutputView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_MESSAGE(WM_OUTPUT_VIEW_MSG, &COutputView::OnListCtrlShowMsg)
	//	ON_WM_CLOSE()
END_MESSAGE_MAP()



// COutputView message handlers


int COutputView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create tabs window:
	if (!m_wndTabs.Create(CMFCTabCtrl::STYLE_FLAT, rectDummy, this, 1))
	{
		TRACE0("Failed to create output tab window\n");
		return -1;      // fail to create
	}

	// Create output panes:
	const DWORD dwStyle = LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;

	if (!m_wndOutputCommInfo.Create(dwStyle, rectDummy, &m_wndTabs, 2) ||
		!m_wndOutputAlarmInfo.Create(dwStyle, rectDummy, &m_wndTabs, 3))
	{
		TRACE0("Failed to create output windows\n");
		return -1;      // fail to create
	}

	UpdateFonts();

	CString strTabName;

	// Attach list windows to tab:
	strTabName = _T("Communication");
	m_wndTabs.AddTab(&m_wndOutputCommInfo, strTabName, (UINT)0);
	strTabName = _T("Alarm");
	m_wndTabs.AddTab(&m_wndOutputAlarmInfo, strTabName, (UINT)1);
	return 0;
}


void COutputView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	m_wndTabs.SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}


void COutputView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
}

void COutputView::UpdateFonts()
{
	m_wndOutputCommInfo.SetFont(&afxGlobalData.fontRegular);
	m_wndOutputAlarmInfo.SetFont(&afxGlobalData.fontRegular);
}

void COutputView::UpdateLanguage()
{
	if (GetLanguage() == _T("CHS"))
	{
		m_wndTabs.SetTabLabel(0,_T("通讯信息"));
		m_wndTabs.SetTabLabel(1,_T("报警信息"));
	}
	else
	{
		m_wndTabs.SetTabLabel(0,_T("CommunicationInfo"));
		m_wndTabs.SetTabLabel(1,_T("AlarmInfo"));
	}
}

LRESULT COutputView::OnListCtrlShowMsg(WPARAM wparam, LPARAM lparam)
{
	AfxMessageBox(_T("ASd"));
	return 0;
}