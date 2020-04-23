// OverViewPage.cpp : implementation file
//

#include "stdafx.h"
#include "Machine.h"
#include "OverViewPage.h"
#include "afxdialogex.h"

// COverViewPage dialog

IMPLEMENT_DYNAMIC(COverViewPage, CDialogWithLang)

COverViewPage::COverViewPage(CWnd* pParent /*=NULL*/)
	: CDialogWithLang(COverViewPage::IDD, pParent)
{
}

COverViewPage::~COverViewPage()
{
}

void COverViewPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogWithLang::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OVERVIEW_LIST, m_ListView);
	DDX_Control(pDX, IDC_OVERVIEW_PIC, m_AnimatedLayout);

}


BEGIN_MESSAGE_MAP(COverViewPage, CDialogWithLang)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_CLEAR_COUNT, &COverViewPage::OnClearCount)
	ON_NOTIFY(NM_RCLICK, IDC_OVERVIEW_LIST, &COverViewPage::OnNMRClickOverviewList)
END_MESSAGE_MAP()

void COverViewPage::AdjustLayout()
{
	CRect rcView;
	GetClientRect(rcView);
	if (m_ListView.GetSafeHwnd() != NULL)
	{
		m_ListView.MoveWindow(0, 0, 320, rcView.bottom, SWP_SHOWWINDOW);
	}
	
	if (m_AnimatedLayout.GetSafeHwnd() != NULL)
	{
		m_AnimatedLayout.MoveWindow(320,0,rcView.right-321,rcView.bottom);
	}
}


// COverViewPage message handlers


void COverViewPage::OnSize(UINT nType, int cx, int cy)
{
	CDialogWithLang::OnSize(nType, cx, cy);
	AdjustLayout();
	// TODO: Add your message handler code here
}


BOOL COverViewPage::OnInitDialog()
{
	CDialogWithLang::OnInitDialog();

	// TODO:  Add extra initialization here
	if (m_ListView.GetSafeHwnd() != NULL)
	{
		m_ListView.SetExtendedStyle( m_ListView.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);
		m_ListView.DeleteAllItems();
		m_ListView.DeleteColumn(0);
		m_ListView.InsertColumn(0, _T(""), 0, 180);
		m_ListView.InsertColumn(1, _T(""), 1, 140);
		m_ListView.SetRowHeigt(30);
		m_ListView.SetBkColor(RGB(210, 220, 250));
		m_ListView.SetFontHW(18, 10);
		
		m_ListView.InsertItem(0, _T("上料数量"));
		m_ListView.InsertItem(1, _T("下料数量"));
		m_ListView.InsertItem(2, _T("成品不良数量"));
		m_ListView.InsertItem(3,  _T("良率"));
		m_ListView.InsertItem(4,  _T("扫码不良"));
		m_ListView.InsertItem(5,  _T("MES不良"));
		m_ListView.InsertItem(6,  _T("工位1压力不良"));
		m_ListView.InsertItem(7,  _T("工位2压力不良"));
		m_ListView.InsertItem(8, _T("产能"));

		m_ListView.InsertItem(9, _T("白班各阶段产能"));
		m_ListView.InsertItem(10, _T("8：00 - 10：00"));
		m_ListView.InsertItem(11, _T("10：00 - 12：00"));
		m_ListView.InsertItem(12, _T("12：00 - 14：00"));
		m_ListView.InsertItem(13, _T("14：00 - 16：00"));
		m_ListView.InsertItem(14, _T("16：00 - 18：00"));
		m_ListView.InsertItem(15, _T("18：00 - 20：00"));

		m_ListView.InsertItem(16, _T("晚班各阶段产能"));
		m_ListView.InsertItem(17, _T("20：00 - 22：00"));
		m_ListView.InsertItem(18, _T("22：00 - 24：00"));
		m_ListView.InsertItem(19, _T("0：00 - 2：00"));
		m_ListView.InsertItem(20, _T("2：00 - 4：00"));
		m_ListView.InsertItem(21, _T("4：00 - 6：00"));
		m_ListView.InsertItem(22, _T("6：00 - 8：00"));

		m_ListView.InsertItem(23, _T("设备运行时间"));
		m_ListView.InsertItem(24, _T("设备生产时间"));
		m_ListView.InsertItem(25, _T("待机时间"));
		m_ListView.InsertItem(26, _T("报警时间"));

		SetTimer(1,1000,NULL);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void COverViewPage::SetListValue()
{
	CString strKey;
	// 上料
	strKey.Format(_T("%d"), MACHINE.m_nOnloadCount);
	m_ListView.SetItemText(0, 1, strKey);

	// 下料
	strKey.Format(_T("%d"), MACHINE.m_nOffloadCount);
	m_ListView.SetItemText(1, 1, strKey);

	// Ng产品
	strKey.Format(_T("%d"), MACHINE.m_nNgCount);
	m_ListView.SetItemText(2, 1, strKey);

	// 良率
	if(MACHINE.m_nOffloadCount == 0)
		strKey = _T("100%");
	else
		strKey.Format(_T("%.1f%%"), (MACHINE.m_nOffloadCount - MACHINE.m_nNgCount)/(MACHINE.m_nOffloadCount*0.01));
	m_ListView.SetItemText(3, 1, strKey);

	// 扫码不良
	strKey.Format(_T("%d"), MACHINE.m_nScanNgCount);
	m_ListView.SetItemText(4, 1, strKey);

	// MES不良
	strKey.Format(_T("%d"), MACHINE.m_nUploadMesNgCount);
	m_ListView.SetItemText(5, 1, strKey);

	// 工位1压力不良
	strKey.Format(_T("%d"), MACHINE.m_nShellStationPressNgCount[0]);
	m_ListView.SetItemText(6, 1, strKey);

	// 工位2压力不良
	strKey.Format(_T("%d"), MACHINE.m_nShellStationPressNgCount[1]);
	m_ListView.SetItemText(7, 1, strKey);
	
	// 产能
	if(MACHINE.m_dwProduceTime - MACHINE.m_dwWaitMaterialTime == 0)
	{
		strKey = _T("0");
	}
	else
	{
		strKey.Format(_T("%.1f"),(MACHINE.m_nOffloadCount/((1.0*(MACHINE.m_dwProduceTime - MACHINE.m_dwWaitMaterialTime))/60)));
	}
	m_ListView.SetItemText(8, 1, strKey);

	//8:00-10:00
	strKey.Format(_T("%d"), MACHINE.m_nEachPartOffload[0]);
	m_ListView.SetItemText(10, 1, strKey);

	//10:00-12:00
	strKey.Format(_T("%d"), MACHINE.m_nEachPartOffload[1]);
	m_ListView.SetItemText(11, 1, strKey);

	//12:00-14:00
	strKey.Format(_T("%d"), MACHINE.m_nEachPartOffload[2]);
	m_ListView.SetItemText(12, 1, strKey);

	//14:00-16:00
	strKey.Format(_T("%d"), MACHINE.m_nEachPartOffload[3]);
	m_ListView.SetItemText(13, 1, strKey);

	//16:00-18:00
	strKey.Format(_T("%d"), MACHINE.m_nEachPartOffload[4]);
	m_ListView.SetItemText(14, 1, strKey);

	//18:00-20:00
	strKey.Format(_T("%d"), MACHINE.m_nEachPartOffload[5]);
	m_ListView.SetItemText(15, 1, strKey);

	//20:00-22:00
	strKey.Format(_T("%d"), MACHINE.m_nEachPartOffload[6]);
	m_ListView.SetItemText(17, 1, strKey);

	//22:00-24:00
	strKey.Format(_T("%d"), MACHINE.m_nEachPartOffload[7]);
	m_ListView.SetItemText(18, 1, strKey);

	//0:00-2:00
	strKey.Format(_T("%d"), MACHINE.m_nEachPartOffload[8]);
	m_ListView.SetItemText(19, 1, strKey);

	//2:00-4:00
	strKey.Format(_T("%d"), MACHINE.m_nEachPartOffload[9]);
	m_ListView.SetItemText(20, 1, strKey);

	//4:00-6:00
	strKey.Format(_T("%d"), MACHINE.m_nEachPartOffload[10]);
	m_ListView.SetItemText(21, 1, strKey);

	//6:00-8:00
	strKey.Format(_T("%d"), MACHINE.m_nEachPartOffload[11]);
	m_ListView.SetItemText(22, 1, strKey);

	// 设备运行时间
	strKey.Format(_T("%02d:%02d:%02d"),MACHINE.m_dwMachineRunTime/3600, MACHINE.m_dwMachineRunTime/60%60, MACHINE.m_dwMachineRunTime%60);
	m_ListView.SetItemText(23, 1, strKey);

	// 设备生产时间
	strKey.Format(_T("%02d:%02d:%02d"),MACHINE.m_dwProduceTime/3600, MACHINE.m_dwProduceTime/60%60, MACHINE.m_dwProduceTime%60);
	m_ListView.SetItemText(24, 1, strKey);

	// 设备待料时间
	strKey.Format(_T("%02d:%02d:%02d"),MACHINE.m_dwWaitMaterialTime/3600, MACHINE.m_dwWaitMaterialTime/60%60, MACHINE.m_dwWaitMaterialTime%60);
	m_ListView.SetItemText(25, 1, strKey);

	// 设备报警时间
	strKey.Format(_T("%02d:%02d:%02d"),MACHINE.m_dwMachineAlarmTime/3600, MACHINE.m_dwMachineAlarmTime/60%60, MACHINE.m_dwMachineAlarmTime%60);
	m_ListView.SetItemText(26, 1, strKey);
}


void COverViewPage::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == 1)
	{
		MACHINE.m_dwMachineRunTime++;
		MACHINE.m_ReportFile.RunTimeCount();   //软件运行时间：

		if(MACHINE.m_bMachineAlarm)
		{
			MACHINE.m_dwMachineAlarmTime++;
			MACHINE.m_ReportFile.FaultTimeCount(); //报警时间：   
		}

		int nMcStatus = MACHINE.m_pRunCtrl->GetMcState();

		if(nMcStatus == MC_RUNNING)
		{
			MACHINE.m_dwProduceTime++;
			MACHINE.m_ReportFile.ProdTimeCount();  //生产时间：  
		}

		if (MACHINE.m_bWaitMaterial && nMcStatus == MC_RUNNING)
		{
			MACHINE.m_dwWaitMaterialTime++;
			MACHINE.m_ReportFile.FreeTimeCount();  //待机时间：
		}

		COleDateTime time = COleDateTime::GetCurrentTime();
		if (time.GetHour() == 8 && time.GetMinute() == 0 && time.GetSecond() > 0 &&  time.GetSecond() < 5)
		{
			for (int i = 0; i < 12; i++)
			{
				MACHINE.m_nEachPartOffload[i] = 0;
			}
		}

		SetListValue();
		m_AnimatedLayout.Invalidate();
	}

	CDialogWithLang::OnTimer(nIDEvent);
}

void COverViewPage::UpdateLanguage()
{
	if (m_ListView.GetSafeHwnd() != NULL)
	{
		if (GetLanguage() == _T("CHS"))
		{
			m_ListView.SetItemText(0, 0, _T("上料数量"));
			m_ListView.SetItemText(1, 0, _T("下料数量"));
			m_ListView.SetItemText(2, 0, _T("成品不良数量"));
			m_ListView.SetItemText(3, 0,  _T("良率"));
			m_ListView.SetItemText(4, 0,  _T("扫码不良"));
			m_ListView.SetItemText(5, 0,  _T("MES不良"));
			m_ListView.SetItemText(6, 0,  _T("工位1压力不良"));
			m_ListView.SetItemText(7, 0,  _T("工位2压力不良"));
			m_ListView.SetItemText(8, 0, _T("产能"));

			m_ListView.SetItemText(9,  0, _T("白班各阶段产能"));
			m_ListView.SetItemText(10, 0,  _T("8：00 - 10：00"));
			m_ListView.SetItemText(11, 0,  _T("10：00 - 12：00"));
			m_ListView.SetItemText(12, 0,  _T("12：00 - 14：00"));
			m_ListView.SetItemText(13, 0,  _T("14：00 - 16：00"));
			m_ListView.SetItemText(14, 0,  _T("16：00 - 18：00"));
			m_ListView.SetItemText(15, 0,  _T("18：00 - 20：00"));

			m_ListView.SetItemText(16, 0, _T("晚班各阶段产能"));
			m_ListView.SetItemText(17, 0, _T("20：00 - 22：00"));
			m_ListView.SetItemText(18, 0, _T("22：00 - 24：00"));
			m_ListView.SetItemText(19, 0, _T("0：00 - 2：00"));
			m_ListView.SetItemText(20, 0, _T("2：00 - 4：00"));
			m_ListView.SetItemText(21, 0, _T("4：00 - 6：00"));
			m_ListView.SetItemText(22, 0, _T("6：00 - 8：00"));

			m_ListView.SetItemText(23, 0, _T("设备运行时间"));
			m_ListView.SetItemText(24, 0, _T("设备生产时间"));
			m_ListView.SetItemText(25, 0, _T("待机时间"));
			m_ListView.SetItemText(26, 0, _T("报警时间"));

		}
		else
		{
			m_ListView.SetItemText(0, 0, _T("onload num"));
			m_ListView.SetItemText(1, 0, _T("offload num"));
			m_ListView.SetItemText(2, 0, _T("offload NG num"));
			m_ListView.SetItemText(3, 0, _T("Yield(%)"));
			m_ListView.SetItemText(4, 0, _T("Scan Ng Num"));
			m_ListView.SetItemText(5, 0, _T("MES Ng Num"));
			m_ListView.SetItemText(6, 0, _T("Press1 Ng Num"));
			m_ListView.SetItemText(7, 0, _T("Press2 Ng Num"));

			m_ListView.SetItemText(8,0,_T("PPM"));

			m_ListView.SetItemText(9, 0, _T("Day Offload Num"));
			m_ListView.SetItemText(10, 0,  _T("8：00 - 10：00"));
			m_ListView.SetItemText(11, 0,  _T("10：00 - 12：00"));
			m_ListView.SetItemText(12, 0,  _T("12：00 - 14：00"));
			m_ListView.SetItemText(13, 0,  _T("14：00 - 16：00"));
			m_ListView.SetItemText(14, 0,  _T("16：00 - 18：00"));
			m_ListView.SetItemText(15, 0,  _T("18：00 - 20：00"));

			m_ListView.SetItemText(16, 0,  _T("Night Offload Num"));
			m_ListView.SetItemText(17, 0,  _T("20：00 - 22：00"));
			m_ListView.SetItemText(18, 0,  _T("22：00 - 24：00"));
			m_ListView.SetItemText(19, 0,  _T("0：00 - 2：00"));
			m_ListView.SetItemText(20, 0,  _T("2：00 - 4：00"));
			m_ListView.SetItemText(21, 0,  _T("4：00 - 6：00"));
			m_ListView.SetItemText(22, 0,  _T("6：00 - 8：00"));

			m_ListView.SetItemText(23, 0, _T("running time"));
			m_ListView.SetItemText(24, 0, _T("produced time"));
			m_ListView.SetItemText(25, 0, _T("wait time"));
			m_ListView.SetItemText(26, 0, _T("alarm time"));
		}
	}
	CDialogWithLang::UpdateLanguage();
}

void COverViewPage::OnClearCount()
{
	// TODO: Add your command handler code here
	if (MACHINE.m_dbRecord.GetCurUserLevel() < 0 || MACHINE.m_dbRecord.GetCurUserLevel() > 1)
	{
		CString strInfo = _T("权限不够，请提高权限");
		AfxMessageBox(strInfo);
		return ;
	}
	
	POSITION pos = m_ListView.GetFirstSelectedItemPosition();
	int nCurSelect = m_ListView.GetNextSelectedItem(pos);

	if(nCurSelect == 0)
	{
		//上料数量
		MACHINE.m_nOnloadCount = 0;
	}
	else if(nCurSelect == 1)
	{
		// 下料数量
		MACHINE.m_nOffloadCount = 0;
	}
	else if(nCurSelect == 2)
	{
		// 成品不良数量
		MACHINE.m_nNgCount = 0;
	}
	else if(nCurSelect == 4)
	{
		// 扫码不良
		MACHINE.m_nScanNgCount = 0;
	}
	else if(nCurSelect == 5)
	{
		// MES不良
		MACHINE.m_nUploadMesNgCount = 0;
	}
	else if(nCurSelect == 6)
	{
		// 工位1压力不良
		MACHINE.m_nShellStationPressNgCount[0] = 0;
	}
	else if(nCurSelect == 7)
	{
		// 工位2压力不良
		MACHINE.m_nShellStationPressNgCount[1] = 0;
	}
	else if(nCurSelect == 23)
	{
		// 生产时间
		MACHINE.m_dwMachineRunTime = 0;
	}
	else if(nCurSelect == 24)
	{
		// 生产时间
		MACHINE.m_dwProduceTime = 0;
	}
	else if(nCurSelect == 25)
	{
		// 停止时间
		MACHINE.m_dwWaitMaterialTime = 0;
	}
	else if(nCurSelect == 26)
	{
		// 报警时间
		MACHINE.m_dwMachineAlarmTime = 0;
	}


	if(nCurSelect == 9)
	{
		for (int i = 0; i < 6; i++)
		{
			MACHINE.m_nEachPartOffload[i] = 0;
		}
	}

	if(nCurSelect == 16)
	{
		for (int i = 0; i < 6; i++)
		{
			MACHINE.m_nEachPartOffload[6+i] = 0;
		}
	}

	if(10 <= nCurSelect && nCurSelect <= 15)
	{
		MACHINE.m_nEachPartOffload[nCurSelect - 10] = 0;
	}
	if(17 <= nCurSelect && nCurSelect <= 22)
	{
		MACHINE.m_nEachPartOffload[nCurSelect - 11] = 0;
	}
}


void COverViewPage::OnNMRClickOverviewList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem != -1 && pNMListView->iItem != 3 && pNMListView->iItem != 8)
	{
		CMenu pMenu;
		pMenu.LoadMenu(IDR_MENU_OVERVIEW);
		CPoint myPoint;  
		ClientToScreen(&myPoint);  
		GetCursorPos(&myPoint);
		((CMenu*)pMenu.GetSubMenu(0))->TrackPopupMenu(0,myPoint.x,myPoint.y,this);
	}
}
