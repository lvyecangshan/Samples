// ProductMotorPage.cpp : implementation file
//

#include "stdafx.h"
#include "Machine.h"
#include "ProductMotorPage.h"
#include "afxdialogex.h"


// CProductMotorPage dialog

IMPLEMENT_DYNAMIC(CProductMotorPage, CDialogWithLang)

CProductMotorPage::CProductMotorPage(CWnd* pParent /*=NULL*/)
	: CDialogWithLang(CProductMotorPage::IDD, pParent)
{
	m_nMotorListCurSel = -1;
}

CProductMotorPage::~CProductMotorPage()
{
}

void CProductMotorPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogWithLang::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PRODUCT_PT_COMBO, m_cbProduct);
}


BEGIN_MESSAGE_MAP(CProductMotorPage, CDialogWithLang)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CProductMotorPage message handlers


void CProductMotorPage::OnSize(UINT nType, int cx, int cy)
{
	CDialogWithLang::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
}


BOOL CProductMotorPage::OnInitDialog()
{
	CDialogWithLang::OnInitDialog();

	// TODO:  Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CProductMotorPage::AdjustLayout()
{
	
}

void CProductMotorPage::FillMotorList()
{
	sMODULE *pModule = &MODULES[MODULES.GetCurModule()];
	while(((CComboBox*)GetDlgItem(IDC_PRODUCT_MOTOR_LIST))->GetCount() > 0 )
		((CComboBox*)GetDlgItem(IDC_PRODUCT_MOTOR_LIST))->DeleteString(0);
	for( int i=0; i<pModule->m_nMotors; i++ )
	{
		if(i < MODULES.GetNumModule())
		{
			CString strMotorName = MOTORS[pModule->m_pMotors[i]]->m_strName;
			int index = ((CComboBox*)GetDlgItem(IDC_PRODUCT_MOTOR_LIST))->AddString(strMotorName);
			((CComboBox*)GetDlgItem(IDC_PRODUCT_MOTOR_LIST))->SetItemData(index, pModule->m_pMotors[i]);
		}
	}

	if (m_nMotorListCurSel < 0 || m_nMotorListCurSel >= pModule->m_nMotors)
		m_nMotorListCurSel = 0;
	((CComboBox*)GetDlgItem(IDC_PRODUCT_MOTOR_LIST))->SetCurSel(m_nMotorListCurSel);
}

void CProductMotorPage::OnSetActive()
{
	FillMotorList();
	MACHINE.m_dbRecord.LoadProduct(&m_cbProduct);
	m_cbProduct.SetCurSel(0);
}
