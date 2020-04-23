#pragma once
#include "afxcmn.h"
#include "ListCtrlEx.h"

// CModuleMonitorPage dialog

class CModuleMonitorPage : public CDialogWithLang
{
	DECLARE_DYNAMIC(CModuleMonitorPage)

public:
	CModuleMonitorPage(CWnd* pParent = NULL);   // standard constructor
	virtual ~CModuleMonitorPage();
	void UpdateLanguage();
	// Dialog Data
	enum { IDD = IDD_DLG_MODULE_MONITOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()
public:
	void AdjustLayout();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	CListCtrlEx m_listModule;
};
