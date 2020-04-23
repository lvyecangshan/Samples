#pragma once
#include "afxwin.h"


// CProductMotorPage dialog

class CProductMotorPage : public CDialogWithLang
{
	DECLARE_DYNAMIC(CProductMotorPage)

public:
	CProductMotorPage(CWnd* pParent = NULL);   // standard constructor
	virtual ~CProductMotorPage();
	void OnSetActive();
// Dialog Data
	enum { IDD = IDD_DLG_PRODUCT_MOTOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void AdjustLayout();
	void	FillMotorList();
	DECLARE_MESSAGE_MAP()

	int	m_nMotorListCurSel;
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	CComboBox m_cbProduct;
};
