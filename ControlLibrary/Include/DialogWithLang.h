
#pragma once
#include "GeneralHelper.h"
// CDialogWithLang dialog
class __declspec(dllexport) CDialogWithLang : public CDialog
{
	DECLARE_DYNAMIC(CDialogWithLang)

public:
	CDialogWithLang(UINT nIDTemplate, CWnd* pParentWnd = NULL);   // standard constructor
	virtual ~CDialogWithLang();

	virtual void UpdateLanguage();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	DECLARE_MESSAGE_MAP()

protected:
	CString m_strTag;
	BOOL	m_bWithInLibrary;
};
