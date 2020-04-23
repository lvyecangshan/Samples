// UserManageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Machine.h"
#include "UserManageDlg.h"


// CUserManageDlg dialog

IMPLEMENT_DYNAMIC(CUserManageDlg, CDialogEx)

CUserManageDlg::CUserManageDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUserManageDlg::IDD, pParent)
	, m_strUserName(_T(""))
	, m_strPassWord(_T(""))
{

}

CUserManageDlg::~CUserManageDlg()
{
}

void CUserManageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_USER_MANAGE_NAME, m_strUserName);
	DDX_Text(pDX, IDC_USER_MANAGE_PASS, m_strPassWord);
	DDX_Control(pDX, IDC_USER_MANAGE_LEVEL, m_cbUserLevel);
	DDX_Control(pDX, IDC_USER_MANAGE_LIST, m_listUser);
}


BEGIN_MESSAGE_MAP(CUserManageDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BT_USER_MANAGE_ADD, &CUserManageDlg::OnBnClickedBtUserManageAdd)
	ON_BN_CLICKED(IDC_BT_USER_MANAGE_MODIFY, &CUserManageDlg::OnBnClickedBtUserManageModify)
	ON_BN_CLICKED(IDC_BT_USER_MANAGE_DELETE, &CUserManageDlg::OnBnClickedBtUserManageDelete)
	ON_NOTIFY(NM_CLICK, IDC_USER_MANAGE_LIST, &CUserManageDlg::OnNMClickUserManageList)
END_MESSAGE_MAP()


// CUserManageDlg message handlers

BOOL CUserManageDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_listUser.SetExtendedStyle(m_listUser.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listUser.InsertColumn(0,_T("序号"),0,60);
	m_listUser.InsertColumn(1,_T("用户名"),0,120);
	m_listUser.InsertColumn(2,_T("用户密码"),0,120);
	m_listUser.InsertColumn(3,_T("用户等级"),0,100);

	MACHINE.m_dbRecord.ShowUserList(&m_listUser);

    m_cbUserLevel.ResetContent();
	m_cbUserLevel.AddString(_T("管理员"));
	m_cbUserLevel.AddString(_T("维护员"));
	m_cbUserLevel.AddString(_T("操作员"));
        
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CUserManageDlg::OnNMClickUserManageList(NMHDR *pNMHDR, LRESULT *pResult)
{
	int iCur = m_listUser.GetSelectionMark();
    
	if (iCur > -1 && iCur < m_listUser.GetItemCount())
	{
		m_strUserName = m_listUser.GetItemText(iCur,1);
		m_strPassWord = m_listUser.GetItemText(iCur,2);
		m_cbUserLevel.SetCurSel(_tstoi(m_listUser.GetItemText(iCur, 3)));
	}
    
	UpdateData(FALSE);
	*pResult = 0;
}

void CUserManageDlg::OnBnClickedBtUserManageAdd()
{
	UpdateData(TRUE);
	if (m_strUserName.IsEmpty())
	{
		AfxMessageBox(_T("用户名不能为空"));
		return;
	}
    
	CString strName;
	for (int i =0; i < m_listUser.GetItemCount(); i++)
	{
		strName = m_listUser.GetItemText(i, 1);
		if (strName == m_strUserName)
		{
			AfxMessageBox(_T("不能添加同名用户"));
			return;
		}
	}
	
	MACHINE.m_dbRecord.AddUserName(m_strUserName, m_strPassWord, m_cbUserLevel.GetCurSel());
	MACHINE.m_dbRecord.ShowUserList(&m_listUser);
}

void CUserManageDlg::OnBnClickedBtUserManageModify()
{
	UpdateData(TRUE);
	if (m_strUserName.IsEmpty())
	{
		AfxMessageBox(_T("用户名不能为空"));
		return;
	}
	if (m_strUserName == _T("Admin") && MACHINE.m_dbRecord.GetCurUserName() != _T("Admin"))
	{
		AfxMessageBox(_T("不能修改此用户密码！"));
		return;
	}
	MACHINE.m_dbRecord.ModifyUserName(m_strUserName,m_strPassWord,m_strPassWord,m_cbUserLevel.GetCurSel());
	MACHINE.m_dbRecord.ShowUserList(&m_listUser);
}

void CUserManageDlg::OnBnClickedBtUserManageDelete()
{
	UpdateData(TRUE);
	if (m_strUserName.IsEmpty())
	{
		AfxMessageBox(_T("用户名不能为空"));
		return;
	}
	if (m_strUserName == MACHINE.m_dbRecord.GetCurUserName())
	{
		AfxMessageBox(_T("不能删除当前用户名"));
		return;
	}
	if (m_strUserName == _T("Admin") && MACHINE.m_dbRecord.GetCurUserName() != _T("Admin"))
	{
		AfxMessageBox(_T("不能删除此用户！"));
		return;
	}
	MACHINE.m_dbRecord.DeleteUserName(m_strUserName);
	MACHINE.m_dbRecord.ShowUserList(&m_listUser);
}
// UserManageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Machine.h"
#include "UserManageDlg.h"


// CLoginDlg dialog

IMPLEMENT_DYNAMIC(CLoginDlg, CDialogEx)

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLoginDlg::IDD, pParent)
	, m_strPassWord(_T(""))
{
}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LOGIN_USER_NAME, m_cbUserName);
	DDX_Text(pDX, IDC_LOGIN_PASS, m_strPassWord);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BT_LOGIN, &CLoginDlg::OnBnClickedBtLogin)
	ON_BN_CLICKED(IDC_BT_LOGIN_EXIT, &CLoginDlg::OnBnClickedBtLoginExit)
END_MESSAGE_MAP()


BOOL CLoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	MACHINE.m_dbRecord.LoadUserName(&m_cbUserName);



	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

// CLoginDlg message handlers

void CLoginDlg::OnBnClickedBtLogin()
{
	UpdateData(TRUE);
	CString strUser;
	m_cbUserName.GetLBText(m_cbUserName.GetCurSel(),strUser);


	if (MACHINE.m_dbRecord.Login(strUser.Trim(),m_strPassWord))
	{
		OnOK();
	}
	else
	{
		AfxMessageBox(_T("密码错误"));
	}
}

void CLoginDlg::OnBnClickedBtLoginExit()
{
	//exit(0);// TODO: Add your control notification handler code here
	OnCancel();
}
// UserManageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Machine.h"
#include "UserManageDlg.h"


// CModifyPassDlg dialog

IMPLEMENT_DYNAMIC(CModifyPassDlg, CDialogEx)

CModifyPassDlg::CModifyPassDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CModifyPassDlg::IDD, pParent)
	, m_strOldPassWord(_T(""))
	, m_strNewPassWord(_T(""))
{

}

CModifyPassDlg::~CModifyPassDlg()
{
}

void CModifyPassDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MODIFY_OLD_PASSWORD, m_strOldPassWord);
	DDX_Text(pDX, IDC_MODIFY_NEW_PASSWORD, m_strNewPassWord);
}


BEGIN_MESSAGE_MAP(CModifyPassDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BT_MODIFY_PASSWORD_OK, &CModifyPassDlg::OnBnClickedBtModifyPasswordOk)
	ON_BN_CLICKED(IDC_BT_MODIFY_PASSWORD_EXIT, &CModifyPassDlg::OnBnClickedBtModifyPasswordExit)
END_MESSAGE_MAP()


BOOL CModifyPassDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

// CModifyPassDlg message handlers

void CModifyPassDlg::OnBnClickedBtModifyPasswordOk()
{
	UpdateData(TRUE);
	if (MACHINE.m_dbRecord.ModifyUserName(MACHINE.m_dbRecord.GetCurUserName(),m_strOldPassWord,m_strNewPassWord,-1))
	{
		AfxMessageBox(_T("密码修改成功"));
		OnOK();
	}
	
}

void CModifyPassDlg::OnBnClickedBtModifyPasswordExit()
{
	OnCancel();
}
