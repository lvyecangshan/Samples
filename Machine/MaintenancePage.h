#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CMaintenancePage dialog
#include "ProductMotorPage.h"
class CMaintenancePage : public CDialogWithLang
{
	DECLARE_DYNAMIC(CMaintenancePage)

public:
	CMaintenancePage(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMaintenancePage();

// Dialog Data
	enum { IDD = IDD_DLG_MAINTENANCE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
protected:
	void AdjustLayout();
public:
	CModuleListBox m_ListModule;
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CMaintenanceTabCtrl m_TabCtrlIO;
	CProductMotorPage m_ProductMotorPage;
	afx_msg void OnTcnSelchangeMaintenanceTabIo(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLbnSelchangeMaintenanceListModule();
	afx_msg void OnLbnDblclkMaintenanceListModule();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
