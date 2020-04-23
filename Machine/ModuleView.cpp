
// ModuleView.cpp : implementation file
//
#include "stdafx.h"
#include "Machine.h"
#include "ModuleView.h"


// CModuleView

IMPLEMENT_DYNAMIC(CModuleView, CDockablePane)

	CModuleView::CModuleView()
{

}

CModuleView::~CModuleView()
{
}


BEGIN_MESSAGE_MAP(CModuleView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_MODULE_ENABLE, &CModuleView::OnModuleEnable)
	ON_COMMAND(ID_MODULE_DRYRUN, &CModuleView::OnModuleDryrun)
	ON_COMMAND(ID_MODULE_SAVE, &CModuleView::OnModuleSave)
END_MESSAGE_MAP()



// CModuleView message handlers


int CModuleView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create views:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndTreeView.Create(dwViewStyle, rectDummy, this, 2))
	{
		TRACE0("Failed to create Class View\n");
		return -1;      // fail to create
	}
	m_wndTreeView.Init();
	// Load images:
	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_MODULE);
	m_wndToolBar.LoadToolBar(IDR_MODULE, 0, 0, TRUE /* Is locked */);
	m_wndToolBar.SetSizes(CSize(48,48),CSize(48,48));
	//OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// All commands will be routed via this control , not via the parent frame:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	CMenu menuSort;
	//menuSort.LoadMenu(IDR_POPUP_SORT);
	return 0;
}


void CModuleView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CModuleView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndTreeView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}


void CModuleView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	CRect rectTree;
	m_wndTreeView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}


void CModuleView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndTreeView.SetFocus();
	// TODO: Add your message handler code here
}

void CModuleView::OnModuleEnable()
{
	m_wndTreeView.m_nType = 0;
	m_wndTreeView.Init(TRUE);
}


void CModuleView::OnModuleDryrun()
{
	m_wndTreeView.m_nType = 1;
	m_wndTreeView.Init(TRUE);
}

void CModuleView::OnModuleSave()
{
	m_wndTreeView.SaveConfig();
}

void CModuleView::UpdateLanguage()
{
	OnModuleEnable();
}
