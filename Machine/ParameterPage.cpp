// ParameterPage.cpp : implementation file
//

#include "stdafx.h"
#include "Machine.h"
#include "ParameterPage.h"
#include "afxdialogex.h"
#include "ParameterInputDlg.h"
#include "ProductDlg.h"
// CParameterPage dialog

#define  ENABLE_PARAMETER	1111
IMPLEMENT_DYNAMIC(CParameterPage, CDialogWithLang)

	CParameterPage::CParameterPage(CWnd* pParent /*=NULL*/)
	: CDialogWithLang(CParameterPage::IDD, pParent)
{
	m_nCurModule = -1;
	m_nCurPorduct = -1;
	m_nCurMotor = -1;
}

CParameterPage::~CParameterPage()
{
}

void CParameterPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogWithLang::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PARAMTER_LIST_MODULE, m_listModule);
	DDX_Control(pDX, IDC_PARAMETER_MOTOR_GROUP, m_groupMotor);
	DDX_Control(pDX, IDC_PARAMETER_COMMON_GROUP, m_groupCommon);
	DDX_Control(pDX, IDC_PARAMETER_MOTOR_LIST, m_listMotor);
	DDX_Control(pDX, IDC_PARAMETER_PRODUCT_LIST, m_listProduct);
	DDX_Control(pDX, IDC_PARAMETER_MOTOR_POS_LIST, m_listMotorPos);
	DDX_Control(pDX, IDC_PARAMETER_MOTOR_TITLE, m_listMotorTitle);
	DDX_Control(pDX, IDC_PARAMETER_MFCPROPERTYGRID, m_PropertyGrid);
	DDX_Control(pDX, IDC_PARAMETER_DEFAULT_PRODUCT_LIST, m_listDefaultProduct);
	DDX_Control(pDX, IDC_PARAMETER_DEFAULT_PRODUCT_ENABLE, m_btDefaultEnable);
	DDX_Control(pDX, IDC_PARAMETER_PRODUCT_BTN_EDIT, m_btProductEdit);
	DDX_Control(pDX, IDC_PARAMETER_PRODUCT_TITLE, m_stProduct);
	DDX_Control(pDX, IDC_PARAMETER_DEFAULT_PRODUCT_TITLE, m_stDefaultProduct);
}


BEGIN_MESSAGE_MAP(CParameterPage, CDialogWithLang)
	ON_WM_SIZE()
	ON_LBN_SELCHANGE(IDC_PARAMTER_LIST_MODULE, &CParameterPage::OnLbnSelchangeParamterListModule)
	ON_CBN_SELCHANGE(IDC_PARAMETER_PRODUCT_LIST, &CParameterPage::OnCbnSelchangeParameterProductList)
	ON_CBN_SELCHANGE(IDC_PARAMETER_MOTOR_LIST, &CParameterPage::OnCbnSelchangeParameterMotorList)
	ON_NOTIFY(NM_RCLICK, IDC_PARAMETER_MOTOR_POS_LIST, &CParameterPage::OnNMRClickParameterMotorPosList)
	ON_NOTIFY(NM_DBLCLK, IDC_PARAMETER_MOTOR_POS_LIST, &CParameterPage::OnNMDblclkParameterMotorPosList)
	ON_COMMAND(ID_MOTOR_ADD, &CParameterPage::OnMotorAdd)
	ON_COMMAND(ID_MOTOR_DELETE, &CParameterPage::OnMotorDelete)
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)

	ON_BN_CLICKED(IDC_PARAMETER_DEFAULT_PRODUCT_ENABLE, &CParameterPage::OnBnClickedParameterDefaultProductEnable)
	ON_CBN_SELCHANGE(IDC_PARAMETER_DEFAULT_PRODUCT_LIST, &CParameterPage::OnCbnSelchangeParameterDefaultProductList)
	ON_BN_CLICKED(IDC_PARAMETER_PRODUCT_BTN_EDIT, &CParameterPage::OnBnClickedParameterProductBtnEdit)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CParameterPage message handlers


BOOL CParameterPage::OnInitDialog()
{
	CDialogWithLang::OnInitDialog();

	// TODO:  Add extra initialization here
	ListMoudleInit();
	if (m_listMotorPos.GetSafeHwnd() != NULL)
	{
		m_listMotorPos.SetExtendedStyle( m_listMotorPos.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
		m_listMotorPos.DeleteAllItems();

		m_listMotorPos.InsertColumn(0, _T("序号"), 0, 50);
		m_listMotorPos.InsertColumn(1, _T("名称"), 0, 300);
		m_listMotorPos.InsertColumn(2, _T("位置"), 0, 100);
	}
	InitPropList();
	OnSetActive();
	MACHINE.m_dbRecord.LoadProduct(&m_listDefaultProduct);
	for (int i = 0;i < m_listDefaultProduct.GetCount();i++)
	{
		if (MACHINE.m_nDefaultProduct == m_listDefaultProduct.GetItemData(i))
		{
			m_listDefaultProduct.SetCurSel(i);
			break;
		}
	}
	m_btDefaultEnable.SetCheck(MACHINE.m_bDefaultEnable);
	m_listDefaultProduct.EnableWindow(MACHINE.m_bDefaultEnable);

 	GetDlgItem(IDC_PARAMETER_PRODUCT_LIST)->EnableWindow(FALSE);
 	GetDlgItem(IDC_PARAMETER_PRODUCT_BTN_EDIT)->EnableWindow(FALSE);
 	GetDlgItem(IDC_PARAMETER_DEFAULT_PRODUCT_LIST)->EnableWindow(FALSE);
 	GetDlgItem(IDC_PARAMETER_DEFAULT_PRODUCT_ENABLE)->EnableWindow(FALSE);

	SetTimer(ENABLE_PARAMETER, 1000, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CParameterPage::AdjustLayout()
{
	CRect rcView;
	GetClientRect(rcView);
	if (m_listModule.GetSafeHwnd() != NULL)
	{
		m_listModule.MoveWindow(0, 0, 150, rcView.bottom, SWP_SHOWWINDOW);
	}

	CRect rcGMotor(CPoint(151, 40), CSize((rcView.right-154)/2, rcView.bottom-40));
	CRect rcGCommon(CPoint(151+(rcView.right-150)/2, 40),CSize(rcView.right-150-(rcView.right-150)/2,rcView.bottom-40));
	if (m_stProduct.GetSafeHwnd() != NULL)
	{
		m_stProduct.MoveWindow(rcGMotor.left +5,6,70,30, SWP_SHOWWINDOW);
	}
	if (m_btProductEdit.GetSafeHwnd() != NULL)
	{
		m_btProductEdit.MoveWindow(rcGMotor.right-80,6 ,80,30, SWP_SHOWWINDOW);
	}
	if (m_listProduct.GetSafeHwnd() != NULL)
	{
		m_listProduct.MoveWindow( rcGMotor.left + 80,8,rcGMotor.Width() - 180,22,SWP_SHOWWINDOW);
	}
	if (m_stDefaultProduct.GetSafeHwnd() != NULL)
	{
		m_stDefaultProduct.MoveWindow(rcGCommon.left + 20,6,70,30, SWP_SHOWWINDOW);
	}
	if (m_btDefaultEnable.GetSafeHwnd() != NULL)
	{
		m_btDefaultEnable.MoveWindow(rcGCommon.right-100,6 ,90,30, SWP_SHOWWINDOW);
	}
	if (m_listDefaultProduct.GetSafeHwnd() != NULL)
	{
		m_listDefaultProduct.MoveWindow( rcGCommon.left + 95,8,rcGCommon.Width() - 215,22,SWP_SHOWWINDOW);
	}
	if (m_groupMotor.GetSafeHwnd() != NULL)
	{
		m_groupMotor.MoveWindow(rcGMotor, SWP_SHOWWINDOW);
	}
	if (m_groupCommon.GetSafeHwnd() != NULL)
	{
		m_groupCommon.MoveWindow(rcGCommon, SWP_SHOWWINDOW);
	}
	if (m_listMotorTitle.GetSafeHwnd() != NULL)
	{
		m_listMotorTitle.MoveWindow(rcGMotor.left +10,rcGMotor.top +18,90,26,SWP_SHOWWINDOW);
	}
	if (m_listMotor.GetSafeHwnd() != NULL)
	{
		m_listMotor.MoveWindow(rcGMotor.left + 102,rcGMotor.top +18,rcGMotor.Width()-110,26,SWP_SHOWWINDOW);
	}
	if (m_listMotorPos.GetSafeHwnd() != NULL)
	{
		m_listMotorPos.MoveWindow(rcGMotor.left+5,rcGMotor.top +50,rcGMotor.Width()-10,rcGMotor.Height() -55);
	}
	if (m_PropertyGrid.GetSafeHwnd() != NULL)
	{
		m_PropertyGrid.MoveWindow(rcGCommon.left+5,rcGCommon.top+5,rcGCommon.Width() -10,rcGCommon.Height()-10);
	}
}


void CParameterPage::OnSize(UINT nType, int cx, int cy)
{
	CDialogWithLang::OnSize(nType, cx, cy);
	AdjustLayout();
	// TODO: Add your message handler code here
}

void CParameterPage::OnSetActive()
{
	MACHINE.m_dbRecord.LoadProduct(&m_listProduct);

	if (m_nCurPorduct > -1)
	{
		for (int i = 0;i < m_listProduct.GetCount();i++)
		{
			if (m_nCurPorduct == m_listProduct.GetItemData(i))
			{
				m_listProduct.SetCurSel(i);
				return;
			}
		}
	}
	if (m_listProduct.GetCount() > 0)
	{
		m_listProduct.SetCurSel(0);
		m_nCurPorduct = m_listProduct.GetItemData(0);
	}
}

void CParameterPage::FillMotorList(int nCur/* = -1*/)
{
	if (m_listMotor.GetSafeHwnd() != NULL)
	{
		m_listMotor.ResetContent();	
	}

	if (nCur < 0 || nCur >= MACHINE.m_pRunCtrl->GetRunCount())
	{
		return;
	}
	CRunProcess * pProcess = (CRunProcess*)MACHINE.m_pRunCtrl->GetRun(nCur);
	if (pProcess == NULL)
	{
		return;
	}
	int nIndex = 0;
	map<int*,CString>::iterator _iter;
	for(_iter =pProcess->m_MotorMap.begin();_iter != pProcess->m_MotorMap.end();_iter++)
	{
		if(*(_iter->first) > -1 && *(_iter->first) < MOTORS.GetNumMotors())
		{
			m_listMotor.AddString(MOTORS[*(_iter->first)]->m_strName);
			m_listMotor.SetItemData(nIndex,*(_iter->first));
			nIndex++;
		}
	}
	if (m_listMotor.GetCount() > 0)
	{
		m_listMotor.SetCurSel(0);
		m_nCurMotor = m_listMotor.GetItemData(0);
	}
	OnCbnSelchangeParameterMotorList();
}

void CParameterPage::OnLbnSelchangeParamterListModule()
{
	// TODO: Add your control notification handler code here
	m_nCurModule = m_listModule.GetCurSel();
	if (m_nCurModule < 0)
	{
		return;
	}
	FillMotorList(m_nCurModule);
	SetPropListValue();
}


void CParameterPage::OnCbnSelchangeParameterProductList()
{
	// TODO: Add your control notification handler code here
	int nCur = m_listProduct.GetCurSel();
	if (nCur < 0 || nCur >= m_listProduct.GetCount() )
	{
		return;
	}
	m_nCurPorduct = m_listProduct.GetItemData(nCur);

	if (m_nCurModule < 0 || m_nCurModule >= m_listModule.GetCount())
	{
		return;
	}
	FillMotorList(m_nCurModule);
	SetPropListValue();
}


void CParameterPage::OnCbnSelchangeParameterMotorList()
{
	// TODO: Add your control notification handler code here
	m_listMotorPos.DeleteAllItems();
	int nCur = m_listMotor.GetCurSel();
	if (nCur < 0 || nCur >= m_listMotor.GetCount())
	{
		return;
	}
	m_nCurMotor = m_listMotor.GetItemData(nCur);
	FillMotorView(m_nCurMotor);
}

void CParameterPage::FillMotorView( int nMotor )
{
	m_listMotorPos.DeleteAllItems();
	if (m_nCurModule < 0 || m_nCurModule >= m_listModule.GetCount()
		|| m_nCurPorduct != m_listProduct.GetItemData(m_listProduct.GetCurSel())
		|| nMotor != m_listMotor.GetItemData(m_listMotor.GetCurSel()))
	{
		return;
	}
	CRunProcess *pProcess = (CRunProcess*)MACHINE.m_pRunCtrl->GetRun(m_nCurModule);
	if (pProcess == NULL)
	{
		return;
	}
	CString strKey;
	for (int i = 0;i < (int)pProcess->m_vecProduct.size();i++)
	{
		if (m_nCurPorduct != pProcess->m_vecProduct[i].m_nIndex)
		{
			continue;
		}
		for (int j = 0;j < (int)pProcess->m_vecProduct[i].m_vecMotorRun.size();j++)
		{
			if (nMotor == pProcess->m_vecProduct[i].m_vecMotorRun[j].m_nIndex)
			{
				for (int k = 0;k < (int)pProcess->m_vecProduct[i].m_vecMotorRun[j].m_vecMotorPos.size();k++)
				{
					strKey.Format(_T("%d"),pProcess->m_vecProduct[i].m_vecMotorRun[j].m_vecMotorPos[k].m_nIndex);
					m_listMotorPos.InsertItem(k,strKey);
					m_listMotorPos.SetItemText(k,1,pProcess->m_vecProduct[i].m_vecMotorRun[j].m_vecMotorPos[k].m_strName);
					strKey.Format(_T("%.2f"),pProcess->m_vecProduct[i].m_vecMotorRun[j].m_vecMotorPos[k].m_dlPos);
					m_listMotorPos.SetItemText(k,2,strKey);
				}
			}
		}
	}
}


void CParameterPage::OnNMRClickParameterMotorPosList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	if (m_listMotorPos.GetItemCount() < 1 )
	{
		CMenu pMenu;
		pMenu.LoadMenu(IDR_POPUP_OPERATOR);
		CPoint myPoint;
		ClientToScreen(&myPoint);
		GetCursorPos(&myPoint);
		((CMenu*)pMenu.GetSubMenu(0))->EnableMenuItem(ID_MOTOR_DELETE,MF_GRAYED);
		((CMenu*)pMenu.GetSubMenu(0))->TrackPopupMenu(0,myPoint.x,myPoint.y,this);
		return;
	}

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if(pNMListView->iItem != -1)
	{
		CMenu pMenu;
		pMenu.LoadMenu(IDR_POPUP_OPERATOR);
		CPoint myPoint;
		ClientToScreen(&myPoint);
		GetCursorPos(&myPoint);

		((CMenu*)pMenu.GetSubMenu(0))->TrackPopupMenu(0,myPoint.x,myPoint.y,this);
	}
	///////////////////////////
	*pResult = 0;
}


void CParameterPage::OnNMDblclkParameterMotorPosList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	if (m_nCurModule < 0 || m_nCurModule >= m_listModule.GetCount()
		|| m_nCurPorduct != m_listProduct.GetItemData(m_listProduct.GetCurSel())
		|| m_nCurMotor != m_listMotor.GetItemData(m_listMotor.GetCurSel()))
	{
		return;
	}
	CRunProcess *pProcess = (CRunProcess*)MACHINE.m_pRunCtrl->GetRun(m_nCurModule);
	if (pProcess == NULL)
	{
		return;
	}
	CMotorPos mPos;
	CString strName;
	CString strPos;
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nCurItem = pNMListView->iItem;
	if(nCurItem > -1)
	{
		if (pNMListView->iSubItem < 1)
		{
			return;
		}
		mPos.m_nIndex = _ttoi(m_listMotorPos.GetItemText(nCurItem,0));
		strName = m_listMotorPos.GetItemText(nCurItem,1);
		strPos = m_listMotorPos.GetItemText(nCurItem,2);
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
			dlg.m_strValue = strPos;
			if (dlg.DoModal()==IDOK)
			{
				strPos = dlg.m_strValue;
			}
		}
		mPos.m_strName = strName;
		mPos.m_dlPos = _ttof(strPos);
		MACHINE.m_dbRecord.MotorPosModify(m_nCurPorduct,m_nCurMotor,mPos);
		pProcess->ReadParameter();
		FillMotorView(m_nCurMotor);
	}
	//////////////
	*pResult = 0;
}


void CParameterPage::OnMotorAdd()
{
	if (m_nCurModule < 0 || m_nCurModule >= m_listModule.GetCount()
		|| m_nCurPorduct != m_listProduct.GetItemData(m_listProduct.GetCurSel())
		|| m_nCurMotor != m_listMotor.GetItemData(m_listMotor.GetCurSel()))
	{
		return;
	}
	CRunProcess *pProcess = (CRunProcess*)MACHINE.m_pRunCtrl->GetRun(m_nCurModule);
	if (pProcess == NULL)
	{
		return;
	}
	int nCount = pProcess->m_vecProduct[m_listProduct.GetCurSel()].m_vecMotorRun[m_nCurMotor].m_vecMotorPos.size();
	MACHINE.m_dbRecord.MotorPosAdd(m_nCurPorduct,m_nCurMotor,CMotorPos(nCount));
	pProcess->ReadParameter();
	FillMotorView(m_nCurMotor);
}


void CParameterPage::OnMotorDelete()
{
	if (m_nCurModule < 0 || m_nCurModule >= m_listModule.GetCount()
		||m_nCurPorduct != m_listProduct.GetItemData(m_listProduct.GetCurSel())
		|| m_nCurMotor != m_listMotor.GetItemData(m_listMotor.GetCurSel()))
	{
		return;
	}
	CRunProcess *pProcess = (CRunProcess*)MACHINE.m_pRunCtrl->GetRun(m_nCurModule);
	if (pProcess == NULL)
	{
		return;
	}
	int nCount = pProcess->m_vecProduct[m_listProduct.GetCurSel()].m_vecMotorRun[m_nCurMotor].m_vecMotorPos.size();
	if (nCount < 1)
	{
		return;
	}
	MACHINE.m_dbRecord.MotorPosDelete(m_nCurPorduct,m_nCurMotor,CMotorPos(nCount-1));
	pProcess->ReadParameter();
	FillMotorView(m_nCurMotor);
}

void CParameterPage::InitPropList()
{
	m_PropertyGrid.Init(_T("模组参数"),_T("产品模组参数"),_T("产品参数"));
}

void CParameterPage::SetPropListFont()
{
	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = 20;
	lf.lfWeight = 700;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	m_PropertyGrid.SetFont(&m_fntPropList);
}

void CParameterPage::SetPropListValue()
{
	if (m_nCurModule < 0 || m_nCurModule >= m_listModule.GetCount() || m_nCurPorduct < 0
		||m_nCurPorduct != m_listProduct.GetItemData(m_listProduct.GetCurSel()) )
	{
		return;
	}
	CRunProcess *pProcess = (CRunProcess*)MACHINE.m_pRunCtrl->GetRun(m_nCurModule);
	if (pProcess == NULL || pProcess->m_vecProduct[m_listProduct.GetCurSel()].m_PkgParameter == NULL)
	{
		return;
	}
	if (m_listProduct.GetCurSel() < 0 || m_listProduct.GetCurSel() >= (int)MACHINE.m_vecProduct.size())
	{
		return;
	}
	InitPropList();
	m_PropertyGrid.AddGroup(pProcess->m_VoidMap,0);
	m_PropertyGrid.AddGroup(pProcess->m_vecProduct[m_listProduct.GetCurSel()].m_PkgParameter->m_VoidMap,1);
	m_PropertyGrid.AddGroup(MACHINE.m_vecProduct[m_listProduct.GetCurSel()].m_MachineParameter->m_VoidMap,2);
	SetParameterEnable(pProcess);
	m_PropertyGrid.ExpandAll();
}

void CParameterPage::SetParameterEnable(CRunProcess* pCurProcess)
{
	int nUserLevel =MACHINE.m_dbRecord.GetCurUserLevel();

	for (int nPro = 0; nPro < 3; nPro++)
	{
		if (pCurProcess && NULL != m_PropertyGrid.GetProperty(nPro))
		{
			int nCount = m_PropertyGrid.GetProperty(nPro)->GetSubItemsCount();
			for (int i = 0; i < nCount; i++)
			{
				for (int j = 0; j < (int)pCurProcess->m_vecParameterEN.size(); j++)
				{
					if(nUserLevel < MACHINE_USER_ADMIN)
					{
						m_PropertyGrid.GetProperty(nPro)->GetSubItem(i)->Enable(FALSE);
						continue;
					}
					CString str = m_PropertyGrid.GetProperty(nPro)->GetSubItem(i)->GetName();
					if (m_PropertyGrid.GetProperty(nPro)->GetSubItem(i)->GetName() == pCurProcess->m_vecParameterEN[j].m_strParameter)
					{
						m_PropertyGrid.GetProperty(nPro)->GetSubItem(i)->Enable((nUserLevel <= pCurProcess->m_vecParameterEN[j].m_nParamLevel)?TRUE:FALSE);
					}
				}
			}
		}
	}
}

void CParameterPage::EnablePropertyGrid(BOOL bEnable)
{
	if (m_nCurModule < 0 || m_nCurModule >= m_listModule.GetCount())
	{
		return ;
	}
	int nUserLevel = MACHINE.m_dbRecord.GetCurUserLevel();
	CRunProcess *pProcess = (CRunProcess*)MACHINE.m_pRunCtrl->GetRun(m_nCurModule);
	if (NULL != pProcess)
	{
		for (int i = 0; i < 3; i++)
		{
			if (m_PropertyGrid.GetProperty(i))
			{
				int nCount = m_PropertyGrid.GetProperty(i)->GetSubItemsCount();
				for (int j = 0; j < nCount; j++)
				{
					if (bEnable)
					{
						int nFindIndex = -1;
						for (int n = 0; n < (int)pProcess->m_vecParameterEN.size(); n++)
						{
							if (m_PropertyGrid.GetProperty(i)->GetSubItem(j)->GetName() == pProcess->m_vecParameterEN[n].m_strParameter)
							{
								nFindIndex = n;
								break;
							}
						}
						if (nFindIndex > -1)
						{
							m_PropertyGrid.GetProperty(i)->GetSubItem(j)->Enable((nUserLevel <= pProcess->m_vecParameterEN[nFindIndex].m_nParamLevel)?TRUE:FALSE);
						}
						else
						{
							m_PropertyGrid.GetProperty(i)->GetSubItem(j)->Enable(bEnable);
						}
					}
					else
					{
						m_PropertyGrid.GetProperty(i)->GetSubItem(j)->Enable(bEnable);
					}
				}
			}
		}
	}
}

LRESULT CParameterPage::OnPropertyChanged( WPARAM wParam,LPARAM lParam )
{
	if (m_nCurModule < 0 || m_nCurModule >= m_listModule.GetCount()
		||m_nCurPorduct != m_listProduct.GetItemData(m_listProduct.GetCurSel()))
	{
		return 0;
	}
	CRunProcess *pProcess = (CRunProcess*)MACHINE.m_pRunCtrl->GetRun(m_nCurModule);
	if (pProcess == NULL || m_PropertyGrid.GetPropertyCount() < 3)
	{
		return 0;
	}
	//wParam is the property ctrl ID
	//lParam is the Property Pointer
	CMFCPropertyGridProperty* pProp = (CMFCPropertyGridProperty*) lParam;
	int nType=0;
	if (m_PropertyGrid.GetProperty(0) == pProp->GetParent())
	{
		nType = pProp->GetData();
		COleVariant var = pProp->GetValue();

		if (nType == 1)
		{
			BOOL bValue = (var.boolVal == 0) ? FALSE: TRUE;
			::IniWriteBoolean(pProcess->m_strProcess,pProp->GetName(),bValue,GetAbsolutePathName(PARAMETER_CFG));
		}
		else if (nType == 2)
		{
			::IniWriteDouble(pProcess->m_strProcess,pProp->GetName(),(double)(_variant_t)var,GetAbsolutePathName(PARAMETER_CFG));
		}
		else if (nType == 3)
		{
			::IniWriteInt(pProcess->m_strProcess,pProp->GetName(),(long)(_variant_t)var,GetAbsolutePathName(PARAMETER_CFG));
		}
		else if (nType == 4)
		{
			::IniWriteString(pProcess->m_strProcess,pProp->GetName(),(CString)(_variant_t)var,GetAbsolutePathName(PARAMETER_CFG));
		}
		else
		{
			return 0;
		}
		if(!pProcess->ReadRunParameter())
		{
			OnLbnSelchangeParamterListModule();
		}
	}
	else if (m_PropertyGrid.GetProperty(1) == pProp->GetParent())
	{
		nType = pProp->GetData();
		COleVariant var = pProp->GetValue();
		CString strKey = pProcess->m_vecProduct[m_listProduct.GetCurSel()].m_strName + _T("-")+pProp->GetName();
		if (nType == 1)
		{
			BOOL bValue = (var.boolVal == 0) ? FALSE: TRUE;
			::IniWriteBoolean(pProcess->m_strProcess,strKey,bValue,GetAbsolutePathName(PARAMETER_CFG));
		}
		else if (nType == 2)
		{
			::IniWriteDouble(pProcess->m_strProcess,strKey,(double)(_variant_t)var,GetAbsolutePathName(PARAMETER_CFG));
		}
		else if (nType == 3)
		{
			::IniWriteInt(pProcess->m_strProcess,strKey,(long)(_variant_t)var,GetAbsolutePathName(PARAMETER_CFG));
		}
		else if (nType == 4)
		{
			::IniWriteString(pProcess->m_strProcess,strKey,(CString)(_variant_t)var,GetAbsolutePathName(PARAMETER_CFG));
		}
		else
		{
			return 0;
		}
		pProcess->ReadRunParameter();
	}
	else if (m_PropertyGrid.GetProperty(2) == pProp->GetParent())
	{
		nType = pProp->GetData();
		COleVariant var = pProp->GetValue();
		CString strKey;
		strKey.Format(_T("Machine%d"),MACHINE.m_vecProduct[m_listProduct.GetCurSel()].m_nIndex);
		if (nType == 1)
		{
			BOOL bValue = (var.boolVal == 0) ? FALSE: TRUE;
			::IniWriteBoolean(strKey,pProp->GetName(),bValue,GetAbsolutePathName(PARAMETER_CFG));
		}
		else if (nType == 2)
		{
			::IniWriteDouble(strKey,pProp->GetName(),(double)(_variant_t)var,GetAbsolutePathName(PARAMETER_CFG));
		}
		else if (nType == 3)
		{
			::IniWriteInt(strKey,pProp->GetName(),(long)(_variant_t)var,GetAbsolutePathName(PARAMETER_CFG));
		}
		else if (nType == 4)
		{
			::IniWriteString(strKey,pProp->GetName(),(CString)(_variant_t)var,GetAbsolutePathName(PARAMETER_CFG));
		}
		else
		{
			return 0;
		}
		MACHINE.ReadMachineParamter();
	}
	return 0;
}



void CParameterPage::OnBnClickedParameterDefaultProductEnable()
{
	// TODO: Add your control notification handler code here
	MACHINE.m_bDefaultEnable = m_btDefaultEnable.GetCheck();
	m_listDefaultProduct.EnableWindow(MACHINE.m_bDefaultEnable);
	::IniWriteBoolean(_T("Parameter"), _T("DefaultEnable"), MACHINE.m_bDefaultEnable, ::GetAbsolutePathName(MACHINE_CFG));
}


void CParameterPage::OnCbnSelchangeParameterDefaultProductList()
{
	int nCur = m_listDefaultProduct.GetCurSel();
	if (nCur > -1)
	{
		MACHINE.m_nDefaultProduct = m_listDefaultProduct.GetItemData(nCur);
		::IniWriteInt(_T("Parameter"), _T("DefaultProduct"),MACHINE.m_nDefaultProduct, ::GetAbsolutePathName(MACHINE_CFG));
	}
}


void CParameterPage::OnBnClickedParameterProductBtnEdit()
{
	CProductDlg dlg;
	dlg.DoModal();
	int nProduct = m_listProduct.GetCurSel();
	int nDefProduct = m_listDefaultProduct.GetCurSel();
	MACHINE.m_dbRecord.LoadProduct(&m_listProduct);
	MACHINE.m_dbRecord.LoadProduct(&m_listDefaultProduct);
	m_listProduct.SetCurSel(nProduct);
	m_listDefaultProduct.SetCurSel(nDefProduct);
}

void CParameterPage::UpdateLanguage()
{
	ListMoudleInit();
}

void CParameterPage::ListMoudleInit()
{
	if (m_listModule.GetSafeHwnd() == NULL)
	{
		return;
	}
	int nCur = m_listModule.GetCurSel();
	m_listModule.ResetContent();
	CString strText;
	for (int i=0;i <MACHINE.m_pRunCtrl->GetRunCount();i++)
	{
		if (GetLanguage() == _T("CHS"))
		{
			strText = MACHINE.m_pRunCtrl->GetRun(i)->m_strName;
		}
		else
		{
			strText = ((CRunProcess*)MACHINE.m_pRunCtrl->GetRun(i))->m_strProcess;
		}
		m_listModule.InsertString(i,strText);
	}
	m_listModule.SetCurSel(nCur);
	OnLbnSelchangeParamterListModule();
}

void CParameterPage::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	if (MACHINE.m_pRunCtrl->GetMcState() == MC_BEGIN		||
		MACHINE.m_pRunCtrl->GetMcState() == MC_INITIALIZED	||
		MACHINE.m_pRunCtrl->GetMcState() == MC_STOP_INIT	||
		MACHINE.m_pRunCtrl->GetMcState() == MC_STOP_RUN		||
		MACHINE.m_pRunCtrl->GetMcState() == MC_INIT_ERR		||
		MACHINE.m_pRunCtrl->GetMcState() == MC_RUN_JAM)
	{
		int nUserLevel = MACHINE.m_dbRecord.GetCurUserLevel();
		if (nUserLevel > -1 && nUserLevel < 2)
		{
			EnablePropertyGrid(TRUE);
		}
		else
		{
			EnablePropertyGrid(FALSE);
		}
	}
	else
	{
		EnablePropertyGrid(FALSE);
	}

	CDialogWithLang::OnTimer(nIDEvent);
}

BOOL CParameterPage::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->wParam == VK_SPACE || pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	return CDialogWithLang::PreTranslateMessage(pMsg);
}
