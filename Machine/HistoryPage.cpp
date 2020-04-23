// HistoryPage.cpp : implementation file
//

#include "stdafx.h"
#include "Machine.h"
#include "HistoryPage.h"
#include "afxdialogex.h"


// CHistoryPage dialog
#define  UPDATE_ALARM_DATA		1222

IMPLEMENT_DYNAMIC(CHistoryPage, CDialogWithLang)

	CHistoryPage::CHistoryPage(CWnd* pParent /*=NULL*/)
	: CDialogWithLang(CHistoryPage::IDD, pParent)
	, m_nInfoType(0)
	, m_dtStartData(COleDateTime::GetCurrentTime())
	, m_dtStartTime(COleDateTime::GetCurrentTime().GetYear(), COleDateTime::GetCurrentTime().GetMonth(), COleDateTime::GetCurrentTime().GetDay(), 0, 0, 0)
	, m_dtEndData(COleDateTime::GetCurrentTime())
	, m_dtEndTime(COleDateTime::GetCurrentTime().GetYear(), COleDateTime::GetCurrentTime().GetMonth(), COleDateTime::GetCurrentTime().GetDay(), 23, 59, 59)
	, m_strPageStatus(_T(""))
{
	m_nCurPage = 0;
	m_nPageCount = 0;
}

CHistoryPage::~CHistoryPage()
{
}

void CHistoryPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogWithLang::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_HISTORY_INFO_ALARM, m_nInfoType);
	DDX_DateTimeCtrl(pDX, IDC_HISTORY_INFO_START_DATA, m_dtStartData);
	DDX_DateTimeCtrl(pDX, IDC_HISTORY_INFO_START_TIME, m_dtStartTime);
	DDX_DateTimeCtrl(pDX, IDC_HISTORY_INFO_END_DATA, m_dtEndData);
	DDX_DateTimeCtrl(pDX, IDC_HISTORY_INFO_END_TIME, m_dtEndTime);
	DDX_Text(pDX, IDC_EDIT_HISTORY_INFO_RECORD, m_strPageStatus);
	DDX_Control(pDX, IDC_LIST_HISTORY_INFO, m_ListInfo);
	DDX_Control(pDX, IDC_COMBO_HISTORY_INFO_MODULE, m_cbModule);
	DDX_Control(pDX, IDC_BTN_HISTORY_INFO_FIRST, m_btnFisrt);
	DDX_Control(pDX, IDC_BTN_HISTORY_INFO_PREVIOUS, m_btnPrevious);
	DDX_Control(pDX, IDC_BTN_HISTORY_INFO_NEXT, m_btnNext);
	DDX_Control(pDX, IDC_BTN_HISTORY_INFO_LAST, m_btnLast);
	DDX_Control(pDX, IDC_EDIT_HISTORY_INFO_RECORD, m_editRecord);
	DDX_Control(pDX, IDC_BTN_HISTORY_INFO_QUERY, m_btnQuery);
	DDX_Control(pDX, IDC_BTN_HISTORY_INFO_EXPORT, m_btnExport);
	DDX_Control(pDX, IDC_COMBO_ALMREPORT, m_cbAlmReport);
	//DDX_Control(pDX, IDC_BTN_HISTORY_INFO_DELETE, m_btnDelete);
}


BEGIN_MESSAGE_MAP(CHistoryPage, CDialogWithLang)
	ON_BN_CLICKED(IDC_BTN_HISTORY_INFO_QUERY, &CHistoryPage::OnBnClickedBtnHistoryInfoQuery)
	ON_BN_CLICKED(IDC_BTN_HISTORY_INFO_EXPORT, &CHistoryPage::OnBnClickedBtnHistoryInfoExport)
//	ON_BN_CLICKED(IDC_BTN_HISTORY_INFO_DELETE, &CHistoryPage::OnBnClickedBtnHistoryInfoDelete)
	ON_BN_CLICKED(IDC_BTN_HISTORY_INFO_FIRST, &CHistoryPage::OnBnClickedBtnHistoryInfoFirst)
	ON_BN_CLICKED(IDC_BTN_HISTORY_INFO_PREVIOUS, &CHistoryPage::OnBnClickedBtnHistoryInfoPrevious)
	ON_BN_CLICKED(IDC_BTN_HISTORY_INFO_NEXT, &CHistoryPage::OnBnClickedBtnHistoryInfoNext)
	ON_BN_CLICKED(IDC_BTN_HISTORY_INFO_LAST, &CHistoryPage::OnBnClickedBtnHistoryInfoLast)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_SAVEALLREPORT, &CHistoryPage::OnBnClickedButtonSaveallreport)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_HISTORY_INFO_START_DATA, &CHistoryPage::OnDtnDatetimechangeHistoryInfoStartData)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_HISTORY_INFO_START_TIME, &CHistoryPage::OnDtnDatetimechangeHistoryInfoStartTime)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_HISTORY_INFO_END_DATA, &CHistoryPage::OnDtnDatetimechangeHistoryInfoEndData)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_HISTORY_INFO_END_TIME, &CHistoryPage::OnDtnDatetimechangeHistoryInfoEndTime)
	ON_BN_CLICKED(IDC_RADIO_HYSTORY_INFO_MODULE_CT, &CHistoryPage::OnBnClickedRadioHystoryInfoModuleCt)
	ON_BN_CLICKED(IDC_RADIO_HYSTORY_INFO_STEP_CT, &CHistoryPage::OnBnClickedRadioHystoryInfoStepCt)
	ON_BN_CLICKED(IDC_RADIO_HYSTORY_INFO_YIELD, &CHistoryPage::OnBnClickedRadioHystoryInfoYield)
	ON_BN_CLICKED(IDC_RADIO_HISTORY_INFO_ALARM, &CHistoryPage::OnBnClickedRadioHistoryInfoAlarm)
END_MESSAGE_MAP()


// CHistoryPage message handlers


void CHistoryPage::OnBnClickedBtnHistoryInfoQuery()
{
	UpdateData();
	vector<CListHead> vecHead;
	CListHead listhead;
	CString strSql = _T("");
	CString strModule;

	int nRunCount =MACHINE.m_pRunCtrl->GetRunCount();
	int nCur =m_cbModule.GetCurSel();
	if(nCur <=nRunCount && nCur!=0)
	{
		strModule =((CRunProcess*)MACHINE.m_pRunCtrl->GetRun(nCur-1))->m_strProcess;
	}
	else
	{
		m_cbModule.GetWindowText(strModule);
	}

	if (m_nInfoType == 0)
	{
		CString strModuleName;
		m_cbModule.GetLBText(m_cbModule.GetCurSel(), strModuleName);
		CString strStartTime = m_dtStartData.Format(_T("%Y-%m-%d "))+m_dtStartTime.Format(_T("%H:%M:%S"));
		CString strEndtTime = m_dtEndData.Format(_T("%Y-%m-%d "))+m_dtEndTime.Format(_T("%H:%M:%S"));

		if (m_cbAlmReport.GetCurSel() == 0)
		{
			if (m_cbModule.GetCurSel() == 0)
			{
				strSql.Format(_T("select *  from Alarm_Info where OPERATOR_TIME Between #%s# and #%s#"),strStartTime,strEndtTime);
			}
			else
			{
				strSql.Format(_T("select *  from Alarm_Info where MODULE_NAME = '%s' and OPERATOR_TIME Between #%s# and #%s#"),strModuleName,strStartTime,strEndtTime);
			}

			CString strHead[] = {_T("ID"),_T("INFO"),_T("TYPE"),_T("MODULE_NAME"),_T("OPERATOR_TIME"),_T("PRODUCT_SN")};
			CString strHeadC[] = {_T("报警ID"),_T("报警信息"),_T("报警类型"),_T("模组名称"),_T("报警时间"),_T("持续时间（毫秒）")};
			for (int i =0;i < 6;i++)
			{
				listhead.m_strName = strHead[i];
				listhead.m_strCName = strHeadC[i];
				if (i == 1)
				{
					listhead.m_iNameLen = 500;
				}
				else if (i == 4 || i == 6)
				{
					listhead.m_iNameLen = 160;
				}
				else if (i == 2)
				{
					listhead.m_iNameLen = 70;
				}
				else
				{
					listhead.m_iNameLen = 80;
				}
				vecHead.push_back(listhead);
			}

			MACHINE.m_dbRecord.ShowSinglePage(strSql,vecHead,&m_ListInfo);

			for (int n = 0; n < m_ListInfo.GetItemCount(); n++)
			{
				CString strType = m_ListInfo.GetItemText(n, 2);
				m_ListInfo.SetItemText(n, 2, strType == 2? _T("提示"):_T("故障"));
			}
		}
		else if (m_cbAlmReport.GetCurSel() == 1)
		{
			MACHINE.m_ReportFile.QueryAlmDuration(&m_ListInfo, m_dtStartData, m_dtStartTime, m_dtEndData, m_dtEndTime);
		}
		else
		{
			MACHINE.m_ReportFile.QueryAlmCount(&m_ListInfo, m_dtStartData, m_dtStartTime, m_dtEndData, m_dtEndTime);
		}
	}
	else if (m_nInfoType == 1)
	{
		CString strStartTime = m_dtStartData.Format(_T("%Y-%m-%d "));
		CString strEndtTime = m_dtEndData.Format(_T("%Y-%m-%d "));

		strSql.Format(_T("select *  from Production where ProdDate Between #%s# and #%s#"),strStartTime,strEndtTime);
		CString strHead[] = {_T("ProdDate"),_T("RunTime"),_T("ProdTime"),_T("FreeTime"),_T("FaultTime"),_T("temp"),_T("OKTotal"),_T("NGTotal"),_T("temp"),_T("temp"),_T("temp")};
		CString strHeadC[] = {_T("日期"),_T("运行时间"),_T("生产时间"),_T("待机时间"),_T("故障时间"),_T("生产总数"),_T("良品个数"),_T("NG个数"),_T("良率"),_T("PPM"),_T("故障率")};
		for (int i =0;i < 11;i++)
		{
			listhead.m_strName = strHead[i];
			listhead.m_strCName = strHeadC[i];
			listhead.m_iNameLen = 100;

			if (i == 0)
			{
				listhead.m_iNameLen = 150;
			}

			vecHead.push_back(listhead);
		}
		MACHINE.m_dbRecord.ShowSinglePage(strSql,vecHead,&m_ListInfo);

		for (int i = 0; i < m_ListInfo.GetItemCount(); i++)
		{
			CString strValue;
			DWORD dwTime;
			int nOkTotal = _wtoi(m_ListInfo.GetItemText(i, 6));
			int nNgTotal = _wtoi(m_ListInfo.GetItemText(i, 7));
			strValue.Format(_T("%d"), nOkTotal+nNgTotal);
			m_ListInfo.SetItemText(i, 5, strValue);

			strValue.Format(_T("%.2f%%"), double(nOkTotal)/double(nOkTotal+nNgTotal)*100);
			m_ListInfo.SetItemText(i, 8, strValue);

			dwTime = _wtoi(m_ListInfo.GetItemText(i, 2));
			strValue.Format(_T("%.2f"), double(nOkTotal+nNgTotal)/double(dwTime/60000.0));
			m_ListInfo.SetItemText(i, 9, strValue);

			DWORD dwFaultTime = _wtoi(m_ListInfo.GetItemText(i, 4));
			strValue.Format(_T("%.2f%%"), double(dwFaultTime)/double(dwTime)*100.0);
			m_ListInfo.SetItemText(i, 10, strValue);

			dwTime = _wtoi(m_ListInfo.GetItemText(i, 1));
			strValue = MACHINE.m_ReportFile.MillisecToTime(dwTime);
			m_ListInfo.SetItemText(i, 1, strValue);

			dwTime = _wtoi(m_ListInfo.GetItemText(i, 2));
			strValue = MACHINE.m_ReportFile.MillisecToTime(dwTime);
			m_ListInfo.SetItemText(i, 2, strValue);

			dwTime = _wtoi(m_ListInfo.GetItemText(i, 3));
			strValue = MACHINE.m_ReportFile.MillisecToTime(dwTime);
			m_ListInfo.SetItemText(i, 3, strValue);

			dwTime = _wtoi(m_ListInfo.GetItemText(i, 4));
			strValue = MACHINE.m_ReportFile.MillisecToTime(dwTime);
			m_ListInfo.SetItemText(i, 4, strValue);
		}
	}
	else if (m_nInfoType == 2)
	{
		MACHINE.m_ReportFile.QueryModuleCT(&m_ListInfo, m_dtStartData, m_dtStartTime, m_dtEndData, m_dtEndTime, &m_cbModule);
	}
	else if (m_nInfoType == 3)
	{
		CString strModuleName;
		m_cbModule.GetLBText(m_cbModule.GetCurSel(), strModuleName);

		if (m_cbModule.GetCurSel() == 0)
		{
			AfxMessageBox(_T("请选择对应的模组"));
			return;
		}
		else
		{
			MACHINE.m_ReportFile.QueryStepCT(&m_ListInfo, m_dtStartData, m_dtStartTime, m_dtEndData, m_dtEndTime, strModuleName);
		}
	}

	if (MACHINE.m_dbRecord.m_RecordsetShow != NULL)
	{
		m_nPageCount = MACHINE.m_dbRecord.m_RecordsetShow->GetPageCount();
		m_nCurPage =  (m_nPageCount > 0) ? 1: 0;
	}
	m_vecHead = vecHead;
	m_strPageStatus.Format(_T("%d/%d"),m_nCurPage,m_nPageCount);
	UpdateData(FALSE);
}


void CHistoryPage::OnBnClickedBtnHistoryInfoExport()
{
	int nMcStatus = MACHINE.m_pRunCtrl->GetMcState();
	if(nMcStatus == MC_INITIALIZING || nMcStatus == MC_RUNNING)
	{
		MessageBox(_T("设备运行期间不可导出记录"),_T("历史记录"),MB_ICONWARNING|MB_ICONERROR);
		return;
	}
	try
	{
		CFileDialog fileDlg(FALSE,_T("*.xls"),_T("Report"),OFN_OVERWRITEPROMPT,
			_T("Excel Files (*.xls)|*.xls|cvs Files (*.cvs)|*.cvs|Text Files (*.txt)|*.txt|All Files (*.*)|*.*||"),NULL);
		if (fileDlg.DoModal() == IDOK)
		{
			CString strPathName = fileDlg.GetPathName();
			CFileFind fFind;
			if (fFind.FindFile(strPathName))
			{
				CFile pFile;
				pFile.Remove(strPathName);
			}
			if (fileDlg.GetFileExt().Find(_T("xls")) != -1)
			{
				MACHINE.m_dbRecord.ExportExcel(strPathName,MACHINE.m_dbRecord.m_RecordsetShow);
			}
		}
	}
	catch (CException* e)
	{
		e->ReportError();
	}
}


//void CHistoryPage::OnBnClickedBtnHistoryInfoDelete()
//{
//	CString strSql = _T("");
//	CString strStartTime = m_dtStartData.Format(_T("%Y-%m-%d "))+m_dtStartTime.Format(_T("%H:%M:%S"));
//	CString strEndtTime = m_dtEndData.Format(_T("%Y-%m-%d "))+m_dtEndTime.Format(_T("%H:%M:%S"));
//	CString strMsg;
//	if (m_nInfoType == 0)
//	{
//		strMsg.Format(_T("请确认是否删除%s到%s时间段报警记录，确认请按‘是’，取消请按‘否’"),strStartTime,strEndtTime);
//		if (BLMessageBox(strMsg,MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2) != IDYES)
//		{
//			return;
//		}
//		strSql.Format(_T("delete *  from Alarm_Info where OPERATOR_TIME Between #%s# and #%s#"),strStartTime,strEndtTime);
//		MACHINE.m_dbRecord.DeleteSingleTable(strSql);
//	}
//	else if (m_nInfoType == 1)
//	{
//		strMsg.Format(_T("请确认是否删除%s到%s时间段生产记录，确认请按‘是’，取消请按‘否’"),strStartTime,strEndtTime);
//		if (BLMessageBox(strMsg,MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2) != IDYES)
//		{
//			return;
//		}
//		//strSql.Format(_T("delete *  from Yield_Info where OPERATOR_TIME Between #%s# and #%s#"),strStartTime,strEndtTime);
//		strSql.Format(_T("delete *  from Production where ProdDate Between #%s# and #%s#"),strStartTime,strEndtTime);
//		MACHINE.m_dbRecord.DeleteSingleTable(strSql);
//	}
//	else if (m_nInfoType == 2)
//	{
//		strMsg.Format(_T("请确认是否删除%s到%s时间段模组CT记录，确认请按‘是’，取消请按‘否’"),strStartTime,strEndtTime);
//		if (BLMessageBox(strMsg,MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2) != IDYES)
//		{
//			return;
//		}
//		//strSql.Format(_T("delete *  from Module_Cycle_Time where OPERATOR_TIME Between #%s# and #%s#"),strStartTime,strEndtTime);
//		strSql.Format(_T("delete *  from ModuleCycleTime where ProdDate Between #%s# and #%s#"),strStartTime,strEndtTime);
//		MACHINE.m_dbRecord.DeleteSingleTable(strSql);
//	}
//	else if (m_nInfoType == 3)
//	{
//		strMsg.Format(_T("请确认是否删除%s到%s时间段单步记录，确认请按‘是’，取消请按‘否’"),strStartTime,strEndtTime);
//		if (BLMessageBox(strMsg,MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2) != IDYES)
//		{
//			return;
//		}
//		//strSql.Format(_T("delete *  from Module_Step_Cycle_Time where OPERATOR_TIME Between #%s# and #%s#"),strStartTime,strEndtTime);
//		strSql.Format(_T("delete *  from ModuleStepCycleTime where ProdDate Between #%s# and #%s#"),strStartTime,strEndtTime);
//		MACHINE.m_dbRecord.DeleteSingleTable(strSql);
//	}
//
//	OnBnClickedBtnHistoryInfoQuery();
//}


BOOL CHistoryPage::OnInitDialog()
{
	CDialogWithLang::OnInitDialog();

	m_cbModule.AddString(_T("所有"));

	for (int i = 0;i < MACHINE.m_pRunCtrl->GetRunCount();i++)
	{
		m_cbModule.AddString(((CRunProcess*)MACHINE.m_pRunCtrl->GetRun(i))->m_strName);
	}
	m_cbModule.SetCurSel(0);

	m_cbAlmReport.AddString(_T("报警记录"));
	m_cbAlmReport.AddString(_T("故障率统计"));
	m_cbAlmReport.AddString(_T("报警记录top10"));
	m_cbAlmReport.SetCurSel(0);

	SetTimer(NULL,1000,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CHistoryPage::OnBnClickedBtnHistoryInfoFirst()
{
	m_nCurPage = 1;
	MACHINE.m_dbRecord.ShowPage(m_vecHead,&m_ListInfo,m_nCurPage);
	m_strPageStatus.Format(_T("%d/%d"),m_nCurPage,m_nPageCount);
	UpdateData(FALSE);
}


void CHistoryPage::OnBnClickedBtnHistoryInfoPrevious()
{
	m_nCurPage -= 1;
	if (m_nCurPage < 1)
	{
		m_nCurPage = 1;
	}
	MACHINE.m_dbRecord.ShowPage(m_vecHead,&m_ListInfo,m_nCurPage);
	m_strPageStatus.Format(_T("%d/%d"),m_nCurPage,m_nPageCount);
	UpdateData(FALSE);
}


void CHistoryPage::OnBnClickedBtnHistoryInfoNext()
{
	m_nCurPage += 1;
	if (m_nCurPage > m_nPageCount)
	{
		m_nCurPage = m_nPageCount;
	}
	MACHINE.m_dbRecord.ShowPage(m_vecHead,&m_ListInfo,m_nCurPage);
	m_strPageStatus.Format(_T("%d/%d"),m_nCurPage,m_nPageCount);
	UpdateData(FALSE);
}


void CHistoryPage::OnBnClickedBtnHistoryInfoLast()
{
	m_nCurPage = m_nPageCount;
	MACHINE.m_dbRecord.ShowPage(m_vecHead,&m_ListInfo,m_nCurPage);
	m_strPageStatus.Format(_T("%d/%d"),m_nCurPage,m_nPageCount);
	UpdateData(FALSE);
}


void CHistoryPage::OnSize(UINT nType, int cx, int cy)
{
	CDialogWithLang::OnSize(nType, cx, cy);
	AdjustLayout();
	// TODO: Add your message handler code here
}

void CHistoryPage::AdjustLayout()
{
	CRect rcView;
	GetClientRect(rcView);
	if (m_ListInfo.GetSafeHwnd() != NULL)
	{
		m_ListInfo.MoveWindow(rcView.left,rcView.top+100,rcView.right,rcView.bottom-150);
	}
	int nWidth = (rcView.Width()-160-50)/6;
	if (m_btnFisrt.GetSafeHwnd() != NULL)
	{
		m_btnFisrt.MoveWindow(rcView.left+80,rcView.bottom-45,nWidth,40);
	}
	if (m_btnPrevious.GetSafeHwnd() != NULL)
	{
		m_btnPrevious.MoveWindow(rcView.left+80+nWidth+20,rcView.bottom-45,nWidth,40);
	}
	if (m_btnNext.GetSafeHwnd() != NULL)
	{
		m_btnNext.MoveWindow(rcView.left+80+nWidth*4+30,rcView.bottom-45,nWidth,40);
	}
	if (m_btnLast.GetSafeHwnd() != NULL)
	{
		m_btnLast.MoveWindow(rcView.left+80+nWidth*5+50,rcView.bottom-45,nWidth,40);
	}
	if (m_editRecord.GetSafeHwnd() != NULL)
	{
		m_editRecord.MoveWindow(rcView.left+80+nWidth*2+25,rcView.bottom-45,nWidth,40);
	}
	nWidth = (rcView.Width()-460-40)/3;
	if (m_btnQuery.GetSafeHwnd() != NULL)
	{
		m_btnQuery.MoveWindow(rcView.left+450,rcView.top+10,nWidth,40);
	}
	if (m_btnExport.GetSafeHwnd()!= NULL)
	{
		m_btnExport.MoveWindow(rcView.left+450+nWidth+20,rcView.top+10,nWidth,40);
	}

	if (GetDlgItem(IDC_BUTTON_SAVEALLREPORT))
	{
		GetDlgItem(IDC_BUTTON_SAVEALLREPORT)->MoveWindow(rcView.left+450+(nWidth+20)*2,rcView.top+10,nWidth,40);
	}
// 	if (m_btnDelete.GetSafeHwnd() != NULL)
// 	{
// 		m_btnDelete.MoveWindow(rcView.left+450+(nWidth+20)*2,rcView.top+10,nWidth,40);
// 	}
}


void CHistoryPage::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	BOOL bEnableCtrl = TRUE;
	int nMcStatus = MACHINE.m_pRunCtrl->GetMcState();

	if(nMcStatus == MC_INITIALIZING || nMcStatus == MC_RUNNING)
		bEnableCtrl = FALSE;

	CWnd* pWnd = GetWindow(GW_CHILD);  //GW_HWNDFIRST  //GW_CHILD
	while(pWnd != NULL)
	{
		GetDlgItem(pWnd->GetDlgCtrlID())->EnableWindow(bEnableCtrl);
		pWnd = pWnd->GetWindow(GW_HWNDNEXT);
	}
	CDialogWithLang::OnTimer(nIDEvent);
}


void CHistoryPage::OnBnClickedButtonSaveallreport()
{
	// TODO: 在此添加控件通知处理程序代码
	CTime time = CTime::GetCurrentTime();
	CString strPath;
	strPath.Format(_T(".\\Report\\%04d%02d%02d_%02d%02d%02d\\"), time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());

	m_cbModule.SetCurSel(0);
	m_nInfoType = 0;
	m_cbAlmReport.SetCurSel(0);
	UpdateData(FALSE);
	OnBnClickedBtnHistoryInfoQuery();

	for (int n = 0; n < m_ListInfo.GetItemCount(); n++)
	{
		CString strID = m_ListInfo.GetItemText(n, 0);
		CString strInfo = m_ListInfo.GetItemText(n, 1);
		CString strType = m_ListInfo.GetItemText(n, 2);
		CString strModuleName = m_ListInfo.GetItemText(n, 3);
		CString strDate = m_ListInfo.GetItemText(n, 4);
		CString strDuration = m_ListInfo.GetItemText(n, 5);

		MACHINE.m_ReportFile.WriteSingleFault(strPath, _T("报警信息.csv"), strID, strInfo, strType, strModuleName, strDate, strDuration);
	}

	m_nInfoType = 0;
	m_cbAlmReport.SetCurSel(1);
	UpdateData(FALSE);
	OnBnClickedBtnHistoryInfoQuery();
	faultDurationClass* fdc = new faultDurationClass[m_ListInfo.GetItemCount()];

	for (int i = 2; i < m_ListInfo.GetHeaderCtrl()->GetItemCount(); i++)
	{
		LVCOLUMN p;
		TCHAR w[255];
		p.pszText = w;
		p.cchTextMax = 255;
		p.mask = LVCF_TEXT;
		m_ListInfo.GetColumn(i, &p);

		fdc[0].strDate[i-2] = p.pszText;
	}
	fdc[0].nDay = m_ListInfo.GetHeaderCtrl()->GetItemCount()-2;

	for (int n = 0; n < m_ListInfo.GetItemCount(); n++)
	{
		for (int i = 0; i < m_ListInfo.GetHeaderCtrl()->GetItemCount(); i++)
		{
			fdc[n].strDuration[i] = m_ListInfo.GetItemText(n, i);
		}
	}

	MACHINE.m_ReportFile.WriteFaultDurationAndTotal(strPath, _T("报警时间统计.csv"), fdc, m_ListInfo.GetItemCount());

	delete[] fdc;

	m_nInfoType = 0;
	m_cbAlmReport.SetCurSel(2);
	UpdateData(FALSE);
	OnBnClickedBtnHistoryInfoQuery();
	faultCountClass* fcc = new faultCountClass[m_ListInfo.GetItemCount()];

	for (int i = 2; i < m_ListInfo.GetHeaderCtrl()->GetItemCount(); i++)
	{
		LVCOLUMN p;
		TCHAR w[255];
		p.pszText = w;
		p.cchTextMax = 255;
		p.mask = LVCF_TEXT;
		m_ListInfo.GetColumn(i, &p);

		fcc[0].strDate[i-2] = p.pszText;
	}
	fcc[0].nDay = m_ListInfo.GetHeaderCtrl()->GetItemCount()-2;

	for (int n = 0; n < m_ListInfo.GetItemCount(); n++)
	{
		for (int i = 0; i < m_ListInfo.GetHeaderCtrl()->GetItemCount(); i++)
		{
			fcc[n].strCount[i] = m_ListInfo.GetItemText(n, i);
		}
	}

	MACHINE.m_ReportFile.WriteFaultCountAndTotal(strPath, _T("报警次数统计.csv"), fcc, m_ListInfo.GetItemCount());

	delete[] fcc;

	m_nInfoType = 1;
	UpdateData(FALSE);
	OnBnClickedBtnHistoryInfoQuery();

	for (int n = 0; n < m_ListInfo.GetItemCount(); n++)
	{
		CString strDate = m_ListInfo.GetItemText(n, 0);
		CString strRunTime = m_ListInfo.GetItemText(n, 1);
		CString strProdTime = m_ListInfo.GetItemText(n, 2);
		CString strFreeTime = m_ListInfo.GetItemText(n, 3);
		CString strFaultTime = m_ListInfo.GetItemText(n, 4);
		CString strProdCount = m_ListInfo.GetItemText(n, 5);
		CString strOkCount = m_ListInfo.GetItemText(n, 6);
		CString strNgCount = m_ListInfo.GetItemText(n, 7);
		CString strYield = m_ListInfo.GetItemText(n, 8);
		CString strPPM = m_ListInfo.GetItemText(n, 9);
		CString strFaultRate = m_ListInfo.GetItemText(n, 10);

		MACHINE.m_ReportFile.WriteProdInfo(strPath, _T("设备生产信息.csv"), strDate, strRunTime, strProdTime, strFreeTime, strFaultTime, strProdCount, strOkCount, strNgCount, strYield, strPPM, strFaultRate);
	}	

	m_nInfoType = 2;
	UpdateData(FALSE);
	OnBnClickedBtnHistoryInfoQuery();

	for (int n = 0; n < m_ListInfo.GetItemCount(); n++)
	{
		int i = 0;
		moduleClass mc[255];

		for (i = 0; i < m_ListInfo.GetHeaderCtrl()->GetItemCount(); i++)
		{
			LVCOLUMN p;
			TCHAR w[255];
			p.pszText = w;
			p.cchTextMax = 255;
			p.mask = LVCF_TEXT;
			m_ListInfo.GetColumn(i, &p);

			mc[i].strModuleName.Format(_T("%s"), p.pszText);
			mc[i].dwModuleTime = _wtoi(m_ListInfo.GetItemText(n, i));
		}

		MACHINE.m_ReportFile.WriteModultCT(strPath, _T("模组CT.csv"), mc, i);
	}

	m_nInfoType = 3;
	UpdateData(FALSE);
	CString strActionPath = strPath + _T("StepCT\\");

	for (int m = 1; m < m_cbModule.GetCount(); m++)
	{
		m_cbModule.SetCurSel(m);
		OnBnClickedBtnHistoryInfoQuery();

		for (int n = 0; n < m_ListInfo.GetItemCount(); n++)
		{
			int i = 0;
			actionClass ac[255];

			for (i = 0; i < m_ListInfo.GetHeaderCtrl()->GetItemCount(); i++)
			{
				LVCOLUMN p;
				TCHAR w[255];
				p.pszText = w;
				p.cchTextMax = 255;
				p.mask = LVCF_TEXT;
				m_ListInfo.GetColumn(i, &p);

				ac[i].strActionName.Format(_T("%s"), p.pszText);
				ac[i].dwActionTime = _wtoi(m_ListInfo.GetItemText(n, i));
			}

			CString strFileName, strModuleName;
			m_cbModule.GetLBText(m, strModuleName);
			strFileName.Format(_T("%s-CT.csv"), strModuleName);
			MACHINE.m_ReportFile.WriteActionCT(strActionPath, strFileName, ac, i);
		}
	}

	m_ListInfo.DeleteAllItems();
	while (m_ListInfo.DeleteColumn(0));

	CString strMsg;
	WCHAR szCurPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szCurPath);
	strMsg.Format(_T("导出成功。路径：%s%s"), szCurPath, strPath.Mid(1, strPath.GetLength()-1));
	BLMessageBox(strMsg);
}


void CHistoryPage::OnDtnDatetimechangeHistoryInfoStartData(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	*pResult = 0;
}


void CHistoryPage::OnDtnDatetimechangeHistoryInfoStartTime(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	*pResult = 0;
}


void CHistoryPage::OnDtnDatetimechangeHistoryInfoEndData(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	*pResult = 0;
}


void CHistoryPage::OnDtnDatetimechangeHistoryInfoEndTime(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	*pResult = 0;
}


void CHistoryPage::OnBnClickedRadioHystoryInfoModuleCt()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
}


void CHistoryPage::OnBnClickedRadioHystoryInfoStepCt()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
}


void CHistoryPage::OnBnClickedRadioHystoryInfoYield()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
}


void CHistoryPage::OnBnClickedRadioHistoryInfoAlarm()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
}
