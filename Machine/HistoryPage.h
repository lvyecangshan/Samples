#pragma once
#include "atlcomtime.h"
#include "afxcmn.h"
#include "afxwin.h"


// CHistoryPage dialog

class CHistoryPage : public CDialogWithLang
{ 
	DECLARE_DYNAMIC(CHistoryPage)

public:
	CHistoryPage(CWnd* pParent = NULL);   // standard constructor
	virtual ~CHistoryPage();

// Dialog Data
	enum { IDD = IDD_DLG_HISTORY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	void AdjustLayout();
public:
	int m_nInfoType;
	COleDateTime m_dtStartData;
	COleDateTime m_dtStartTime;
	COleDateTime m_dtEndData;
	COleDateTime m_dtEndTime;
	CString m_strPageStatus;
	CListCtrl m_ListInfo;
	CComboBox m_cbModule;
	int m_nCurPage;
	int m_nPageCount;
	vector<CListHead> m_vecHead;
	afx_msg void OnBnClickedBtnHistoryInfoQuery();
	afx_msg void OnBnClickedBtnHistoryInfoExport();
	afx_msg void OnBnClickedBtnHistoryInfoDelete();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnHistoryInfoFirst();
	afx_msg void OnBnClickedBtnHistoryInfoPrevious();
	afx_msg void OnBnClickedBtnHistoryInfoNext();
	afx_msg void OnBnClickedBtnHistoryInfoLast();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CButton m_btnFisrt;
	CButton m_btnPrevious;
	CButton m_btnNext;
	CButton m_btnLast;
	CEdit m_editRecord;
	CButton m_btnQuery;
	CButton m_btnExport;
	CButton m_btnDelete;


    afx_msg void OnTimer(UINT_PTR nIDEvent);
	CComboBox m_cbAlmReport;
	afx_msg void OnBnClickedButtonSaveallreport();
	
	afx_msg void OnDtnDatetimechangeHistoryInfoStartData(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDtnDatetimechangeHistoryInfoStartTime(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDtnDatetimechangeHistoryInfoEndData(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDtnDatetimechangeHistoryInfoEndTime(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedRadioHystoryInfoModuleCt();
	afx_msg void OnBnClickedRadioHystoryInfoStepCt();
	afx_msg void OnBnClickedRadioHystoryInfoYield();
	afx_msg void OnBnClickedRadioHistoryInfoAlarm();
};
