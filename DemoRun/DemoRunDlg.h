// DemoRunDlg.h : header file
//

#pragma once
#include <ModuleStatusCtrl.h>

// CDemoRunDlg dialog
class CDemoRunDlg : public CDialog
{
// Construction
public:
	CDemoRunDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_DEMORUN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	CModuleStatusCtrl m_wndStatusTable;

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnBnClickedBtnReset();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtnMaintenance();
	afx_msg void OnBnClickedBtnClose();
	afx_msg void OnDestroy();
};
