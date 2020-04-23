
// MachineView.h : interface of the CMachineView class

#pragma once
#include "OverViewPage.h"
#include "MaintenancePage.h"
#include "ModuleMonitorPage.h"
#include "HistoryPage.h"
#include "ParameterPage.h"
#include "afxcmn.h"
#include "afxtabctrl.h"
#include  "gdiplus.h"
#include "TestToolDlg.h"
#include "TowerLightCtrl.h"
#include "MesSetDlg.h"


//#include "TabCtrlD.h"
class CMachineView : public CFormView
{
protected: // create from serialization only
	CMachineView();
	DECLARE_DYNCREATE(CMachineView)

public:
	enum{ IDD = IDD_DLG_MACHINE_FORM };
	// Attributes
public:
	CMachineDoc* GetDocument() const;

	// Operations
public:
	enum VIEW_TAB_INDEX
	{
		MAIN_OVER_VIEW = 0,		// 主界面
		MODULE_MONITOR_VIEW,	// 监视界面
		MAINTENACE_PAGE_VIEW,	// 维护界面
		HISTORY_PAGE_VIEW,		// 历史界面
		PARAMTER_PAGE_VIEW,		// 参数设置
		DEBUG_PAGE_VIEW,		// 调试界面
		MES_PAGE_VIEW,			// MES
	};
	// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	
	// Implementation
public:
	virtual ~CMachineView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


protected:
	void AdjustLayout();
	virtual void OnInitialUpdate();
	void UpdateLanguage();
	
	// Generated message map functions
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg LRESULT OnShowMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnShowMessageEX(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnTcnSelchangeTabForm(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLanguageChinese();
	afx_msg void OnLanguageEnglish();
	afx_msg LRESULT OnUserLogInOutMsg(WPARAM wparam, LPARAM lparam);
	DECLARE_MESSAGE_MAP()
	
protected:
	CFont m_Font;
	CTabCtrlD m_TabCtrl;
	CTowerLightCtrl m_wndTowerLight;
	CTowerLightCtrl m_wndTowerLight2;
	CDJPRibbonBar     m_wndRibbonBar;

public:	// Tab窗口
	COverViewPage		m_OverViewPage;
	CModuleMonitorPage	m_ModuleMonitorPage;
	CMaintenancePage	m_MaintenacePage;
	CHistoryPage		m_HistoryPage;
	CParameterPage		m_ParameterPage;
	CTestToolDlg		m_TestToolDlg;
	CMesSetDlg			m_MesSetDlg;

public:
	vector<CString> m_vecAlarmList;

private:
	static BOOL m_bShowDialogAlready;
	static BOOL m_bShowMsgBoxAlready;
public:
};

#ifndef _DEBUG  // debug version in MachineView.cpp
inline CMachineDoc* CMachineView::GetDocument() const
{ return reinterpret_cast<CMachineDoc*>(m_pDocument); }
#endif
