#pragma once

#include "afxwin.h"
#include "afxcmn.h"
#include "afxpropertygridctrl.h"


// CParameterPage dialog

class CParameterPage : public CDialogWithLang
{
	DECLARE_DYNAMIC(CParameterPage)

public:
	CParameterPage(CWnd* pParent = NULL);   // standard constructor
	virtual ~CParameterPage();

	// Dialog Data
	enum { IDD = IDD_DLG_PARAMETER };

public:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLbnSelchangeParamterListModule();
	afx_msg void OnCbnSelchangeParameterProductList();
	afx_msg void OnCbnSelchangeParameterMotorList();
	afx_msg void OnBnClickedParameterDefaultProductEnable();
	afx_msg void OnCbnSelchangeParameterDefaultProductList();
	afx_msg void OnBnClickedParameterProductBtnEdit();
	afx_msg LRESULT OnPropertyChanged( WPARAM wParam,LPARAM lParam );
	afx_msg void OnNMRClickParameterMotorPosList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkParameterMotorPosList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMotorAdd();
	afx_msg void OnMotorDelete();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	void OnSetActive();
	void UpdateLanguage();
	void AdjustLayout();
	void FillMotorList(int nCur = -1);
	void FillMotorView(int nMotor);
	void InitPropList();
	void SetPropListFont();
	void SetPropListValue();
	void ListMoudleInit();
	void EnablePropertyGrid(BOOL bEnable);
	void SetParameterEnable(CRunProcess* pCurProcess);

protected:
	int m_nCurModule;
	int m_nCurPorduct;
	int m_nCurMotor;
public:
	CFont   m_fntPropList;
	CStatic m_groupMotor;
	CStatic m_groupCommon;
	CComboBox m_listDefaultProduct;
	CComboBox m_listMotor;
	CComboBox m_listProduct;
	CListCtrl m_listMotorPos;
	CStatic m_listMotorTitle;
	CButton m_btDefaultEnable;
	CButton m_btProductEdit;
	CStatic m_stProduct;
	CStatic m_stDefaultProduct;
	CModuleListBox m_listModule;
	CDJPPropertyGridCtrl m_PropertyGrid;
};