// ParameterInputDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Machine.h"
#include "ParameterInputDlg.h"
#include "afxdialogex.h"


// CParameterInputDlg dialog

IMPLEMENT_DYNAMIC(CParameterInputDlg, CDialogEx)

CParameterInputDlg::CParameterInputDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CParameterInputDlg::IDD, pParent)
	, m_strValue(_T(""))
{

}

CParameterInputDlg::~CParameterInputDlg()
{
}

void CParameterInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PARAMETER_INPUT_TEXT, m_strValue);
}


BEGIN_MESSAGE_MAP(CParameterInputDlg, CDialogEx)
	ON_BN_CLICKED(IDC_PARAMETER_INPUT_BTN_OK, &CParameterInputDlg::OnBnClickedParameterInputBtnOk)
END_MESSAGE_MAP()


// CParameterInputDlg message handlers


void CParameterInputDlg::OnBnClickedParameterInputBtnOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);	
	if (m_strValue.Trim().IsEmpty())
	{
		return;
	}
	OnOK();
}


BOOL CParameterInputDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
