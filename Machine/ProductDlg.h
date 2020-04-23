#pragma once
#include "afxcmn.h"


// CProductDlg dialog

class CProductDlg : public CDialogWithLang
{
	DECLARE_DYNAMIC(CProductDlg)

public:
	CProductDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CProductDlg();

// Dialog Data
	enum { IDD = IDD_DLG_PRODUCT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	int m_nCurIndex;
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_listProduct;
	afx_msg void OnNMDblclkProductEditList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickProductEditList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnProductAdd();
	afx_msg void OnProductDelete();
};
