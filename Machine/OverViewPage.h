#pragma once
#include "afxcmn.h"
#include "animatedlayout.h"


// COverViewPage dialog

class COverViewPage : public CDialogWithLang
{
	DECLARE_DYNAMIC(COverViewPage)

public:
	COverViewPage(CWnd* pParent = NULL);   // standard constructor
	virtual ~COverViewPage();

// Dialog Data
	enum { IDD = IDD_DLG_OVERVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClearCount();
	afx_msg void OnNMRClickOverviewList(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void SetListValue();
	void AdjustLayout();
	void UpdateLanguage();
public:
	CListCtrlCl m_ListView;
	CAnimatedLayout m_AnimatedLayout;
	
};
