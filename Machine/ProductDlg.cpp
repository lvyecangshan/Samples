// ProductDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Machine.h"
#include "ProductDlg.h"
#include "afxdialogex.h"
#include "ParameterInputDlg.h"

// CProductDlg dialog

IMPLEMENT_DYNAMIC(CProductDlg, CDialogWithLang)

CProductDlg::CProductDlg(CWnd* pParent /*=NULL*/)
	: CDialogWithLang(CProductDlg::IDD, pParent)
{
	
}

CProductDlg::~CProductDlg()
{
}

void CProductDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogWithLang::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PRODUCT_EDIT_LIST, m_listProduct);
}


BEGIN_MESSAGE_MAP(CProductDlg, CDialogWithLang)
	ON_NOTIFY(NM_DBLCLK, IDC_PRODUCT_EDIT_LIST, &CProductDlg::OnNMDblclkProductEditList)
	ON_NOTIFY(NM_RCLICK, IDC_PRODUCT_EDIT_LIST, &CProductDlg::OnNMRClickProductEditList)
	ON_COMMAND(ID_PRODUCT_ADD, &CProductDlg::OnProductAdd)
	ON_COMMAND(ID_PRODUCT_DELETE, &CProductDlg::OnProductDelete)
END_MESSAGE_MAP()


// CProductDlg message handlers


BOOL CProductDlg::OnInitDialog()
{
	CDialogWithLang::OnInitDialog();
	m_nCurIndex = -1;
	// TODO:  Add extra initialization here
	if (m_listProduct.GetSafeHwnd() != NULL)
	{
		m_listProduct.SetExtendedStyle( m_listProduct.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
		m_listProduct.DeleteAllItems();

		m_listProduct.InsertColumn(0, _T("序号"), 0, 50);
		m_listProduct.InsertColumn(1, _T("名称"), 0, 200);
		m_listProduct.InsertColumn(2, _T("方案"), 0, 80);
	}
	MACHINE.m_dbRecord.LoadProduct(&m_listProduct);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CProductDlg::OnNMDblclkProductEditList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	
	CProduct pProduct;
	CString strName;
	CString strScheme;
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nCurItem = pNMListView->iItem;
	if(nCurItem > -1)
	{
		if (pNMListView->iSubItem < 1)
		{
			return;
		}
		pProduct.m_nIndex = _ttoi(m_listProduct.GetItemText(nCurItem,0));
		strName = m_listProduct.GetItemText(nCurItem,1);
		strScheme = m_listProduct.GetItemText(nCurItem,2);
		if (pNMListView->iSubItem == 1)
		{
			CParameterInputDlg dlg;
			dlg.m_strValue = strName;
			if (dlg.DoModal()==IDOK)
			{
				strName = dlg.m_strValue;
			}
		}
		else if (pNMListView->iSubItem == 2)
		{
			CParameterInputDlg dlg;
			dlg.m_strValue = strScheme;
			if (dlg.DoModal()==IDOK)
			{
				strScheme = dlg.m_strValue;
			}
		}
		pProduct.m_strName = strName;
		pProduct.m_strScheme = strScheme;
		MACHINE.m_dbRecord.ProductModify(pProduct);
		MACHINE.m_dbRecord.LoadProduct(&m_listProduct);

		MACHINE.ReadMachineParamter();
		for (int i = 0; i < MACHINE.m_pRunCtrl->GetRunCount(); i++)
		{
			MACHINE.m_pModules[i]->ReadParameter();
		}
	}
	//////////////
	
	*pResult = 0;
}


void CProductDlg::OnNMRClickProductEditList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	if (m_listProduct.GetItemCount() < 1 )
	{
		CMenu pMenu;
		pMenu.LoadMenu(IDR_POPUP_OPERATOR);
		CPoint myPoint;  
		ClientToScreen(&myPoint);  
		GetCursorPos(&myPoint);
		((CMenu*)pMenu.GetSubMenu(1))->EnableMenuItem(ID_PRODUCT_DELETE,MF_GRAYED);
		((CMenu*)pMenu.GetSubMenu(1))->TrackPopupMenu(0,myPoint.x,myPoint.y,this);
		return ;
	}
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	m_nCurIndex = pNMListView->iItem;
	if(pNMListView->iItem != -1)
	{
		CMenu pMenu;
		pMenu.LoadMenu(IDR_POPUP_OPERATOR);
		CPoint myPoint;  
		ClientToScreen(&myPoint);  
		GetCursorPos(&myPoint);

		((CMenu*)pMenu.GetSubMenu(1))->TrackPopupMenu(0,myPoint.x,myPoint.y,this);
	}
	//////////////////////////
	*pResult = 0;
}


void CProductDlg::OnProductAdd()
{
	// TODO: Add your command handler code here
	BOOL bHas;
	for ( int i = 1;i < 100;i++)
	{
		bHas = FALSE;
		for (int j = 0;j < m_listProduct.GetItemCount();j++)
		{
			if (i == _ttoi(m_listProduct.GetItemText(j,0)))
			{
				bHas =TRUE;
				break;
			}
		}
		if (bHas == FALSE)
		{
			MACHINE.m_dbRecord.ProductAdd(CProduct(i));
			MACHINE.m_dbRecord.LoadProduct(&m_listProduct);

			MACHINE.ReadMachineParamter();
			for (int j = 0; j < MACHINE.m_pRunCtrl->GetRunCount(); j++)
			{
				MACHINE.m_pModules[j]->ReadParameter();
			}
			break;
		}
	}
}


void CProductDlg::OnProductDelete()
{
	if (m_nCurIndex < 0 || m_nCurIndex >= m_listProduct.GetItemCount())
	{
		return;
	}

	if(m_listProduct.GetItemCount() <= 1)
	{
		BLMessageBox(_T("为保证软件正常运行，需至少保留一个产品文件"),MB_ICONWARNING);
		return;
	}
	MACHINE.m_dbRecord.ProductDelete(_ttoi(m_listProduct.GetItemText(m_nCurIndex,0)));
	MACHINE.m_dbRecord.LoadProduct(&m_listProduct);
	MACHINE.ReadMachineParamter();
	for (int i = 0; i < MACHINE.m_pRunCtrl->GetRunCount(); i++)
	{
		MACHINE.m_pModules[i]->ReadParameter();
	}
}
