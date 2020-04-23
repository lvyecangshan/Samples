// AndonFeedDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Machine.h"
#include "AndonFeedDlg.h"
#include "afxdialogex.h"


// CAndonFeedDlg 对话框

IMPLEMENT_DYNAMIC(CAndonFeedDlg, CDialogEx)

CAndonFeedDlg::CAndonFeedDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAndonFeedDlg::IDD, pParent)
{
	 
}

CAndonFeedDlg::~CAndonFeedDlg()
{
}

void CAndonFeedDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAndonFeedDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_ANDON_WAIT_PMC_MATERIAL, &CAndonFeedDlg::OnBnClickedBtnAndonWaitPmcMaterial)
	ON_BN_CLICKED(IDC_BTN_ANDON_WAIT_WORKROOM_MATERIAL, &CAndonFeedDlg::OnBnClickedBtnAndonWaitWorkroomMaterial)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CAndonFeedDlg 消息处理程序


void CAndonFeedDlg::OnBnClickedBtnAndonWaitPmcMaterial()
{
	// TODO: 在此添加控件通知处理程序代码
	MACHINE.AndonMachineStatus(MACHINE.m_strAndonResource,56);
	OnCancel();
}


void CAndonFeedDlg::OnBnClickedBtnAndonWaitWorkroomMaterial()
{
	// TODO: 在此添加控件通知处理程序代码
	MACHINE.AndonMachineStatus(MACHINE.m_strAndonResource,57);
	OnCancel();
}


BOOL CAndonFeedDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->wParam == VK_SPACE || pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


HBRUSH CAndonFeedDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	m_Brush.DeleteObject();
	pDC->SetBkMode(TRANSPARENT);

	if(nCtlColor == CTLCOLOR_DLG)
	{
		m_Brush.CreateSolidBrush(RGB(44, 147, 174));
		return m_Brush;
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
