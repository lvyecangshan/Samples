// AndonFeedDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Machine.h"
#include "AndonFeedDlg.h"
#include "afxdialogex.h"


// CAndonFeedDlg �Ի���

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


// CAndonFeedDlg ��Ϣ�������


void CAndonFeedDlg::OnBnClickedBtnAndonWaitPmcMaterial()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	MACHINE.AndonMachineStatus(MACHINE.m_strAndonResource,56);
	OnCancel();
}


void CAndonFeedDlg::OnBnClickedBtnAndonWaitWorkroomMaterial()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	MACHINE.AndonMachineStatus(MACHINE.m_strAndonResource,57);
	OnCancel();
}


BOOL CAndonFeedDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->wParam == VK_SPACE || pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


HBRUSH CAndonFeedDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	m_Brush.DeleteObject();
	pDC->SetBkMode(TRANSPARENT);

	if(nCtlColor == CTLCOLOR_DLG)
	{
		m_Brush.CreateSolidBrush(RGB(44, 147, 174));
		return m_Brush;
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}
