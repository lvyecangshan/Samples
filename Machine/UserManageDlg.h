#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "resource.h"

// CUserManageDlg dialog

class CUserManageDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CUserManageDlg)

public:
	CUserManageDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUserManageDlg();

// Dialog Data
	enum { IDD = IDD_USER_MANAGE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strUserName;
	CString m_strPassWord;
	CComboBox m_cbUserLevel;
	virtual BOOL OnInitDialog();
	CListCtrl m_listUser;
	afx_msg void OnBnClickedBtUserManageAdd();
	afx_msg void OnBnClickedBtUserManageModify();
	afx_msg void OnBnClickedBtUserManageDelete();
	afx_msg void OnNMClickUserManageList(NMHDR *pNMHDR, LRESULT *pResult);
};
#pragma once


// CLoginDlg dialog

class CLoginDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLoginDlg)

public:
	CLoginDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLoginDlg();

// Dialog Data
	enum { IDD = IDD_LOGIN_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cbUserName;
	CString m_strPassWord;
	afx_msg void OnBnClickedBtLogin();
	afx_msg void OnBnClickedBtLoginExit();
	virtual BOOL OnInitDialog();
};
#pragma once


// CModifyPassDlg dialog

class CModifyPassDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CModifyPassDlg)

public:
	CModifyPassDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CModifyPassDlg();

// Dialog Data
	enum { IDD = IDD_MODIFY_PASSWORD_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strOldPassWord;
	CString m_strNewPassWord;
	afx_msg void OnBnClickedBtModifyPasswordOk();
	afx_msg void OnBnClickedBtModifyPasswordExit();
	virtual BOOL OnInitDialog();
};
