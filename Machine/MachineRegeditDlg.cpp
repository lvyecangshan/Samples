// MachineRegeditDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Machine.h"
#include "MachineRegeditDlg.h"
#include "afxdialogex.h"


// CMachineRegeditDlg 对话框

IMPLEMENT_DYNAMIC(CMachineRegeditDlg, CDialogEx)

CMachineRegeditDlg::CMachineRegeditDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMachineRegeditDlg::IDD, pParent)
{

}

CMachineRegeditDlg::~CMachineRegeditDlg()
{
}

void CMachineRegeditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SERIAL_CODE, m_strSerialCode);
	DDX_Text(pDX, IDC_EDIT_REGEDIT_CODE, m_strRegedit);
}


BEGIN_MESSAGE_MAP(CMachineRegeditDlg, CDialogEx)
	ON_BN_CLICKED(IDC_GET_SERIAL_NUMBER, &CMachineRegeditDlg::OnBnClickedGetSerialNumber)
	ON_BN_CLICKED(IDC_REGISTER_MACHINE, &CMachineRegeditDlg::OnBnClickedRegisterMachine)
END_MESSAGE_MAP()


// CMachineRegeditDlg 消息处理程序


void CMachineRegeditDlg::OnBnClickedGetSerialNumber()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(FALSE);
}


void CMachineRegeditDlg::OnBnClickedRegisterMachine()
{
	// TODO: 在此添加控件通知处理程序代码
	
}
