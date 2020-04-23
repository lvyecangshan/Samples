
// MachineView.cpp : implementation of the CMachineView class
//
#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Machine.h"
#endif

#include "DialogWithLang.h"
#include "MachineDoc.h"
#include "MachineView.h"
#include "MainFrm.h"

GdiplusStartupInput	g_gdiplusStartupInput;
ULONG_PTR		g_gdiplusToken;
#include  "gdiplus.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define  TIMER_MACHINE_VIEW		1112

BOOL CMachineView::m_bShowDialogAlready = FALSE;
BOOL CMachineView::m_bShowMsgBoxAlready = FALSE;
// CMachineView

IMPLEMENT_DYNCREATE(CMachineView, CFormView)

BEGIN_MESSAGE_MAP(CMachineView, CFormView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CMachineView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CMachineView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMachineView::OnFilePrintPreview)
	ON_MESSAGE(WM_SHOW_MESSAGE, &CMachineView::OnShowMessage)
	ON_MESSAGE(WM_SHOW_MESSAGE_EX, &CMachineView::OnShowMessageEX)
	ON_MESSAGE(WM_USER_LOGINOUT, &CMachineView::OnUserLogInOutMsg)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_FORM, &CMachineView::OnTcnSelchangeTabForm)
	ON_COMMAND(ID_LANGUAGE_CHINESE, &CMachineView::OnLanguageChinese)
	ON_COMMAND(ID_LANGUAGE_ENGLISH, &CMachineView::OnLanguageEnglish)
END_MESSAGE_MAP()

// CMachineView construction/destruction

CMachineView::CMachineView()
	: CFormView(CMachineView::IDD)
{
	// TODO: add construction code here
}

CMachineView::~CMachineView()
{
	GdiplusShutdown(g_gdiplusToken);
}

BOOL CMachineView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	return CView::PreCreateWindow(cs);
}

// CMachineView drawing

void CMachineView::OnDraw(CDC* /*pDC*/)
{
	CMachineDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	// TODO: add draw code for native data here
}

// CMachineView printing
void CMachineView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMachineView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMachineView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMachineView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CMachineView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMachineView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
		//theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


	// CMachineView diagnostics

#ifdef _DEBUG
void CMachineView::AssertValid() const
{
	CView::AssertValid();
}

void CMachineView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CMachineDoc* CMachineView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMachineDoc)));
	return (CMachineDoc*)m_pDocument;
}
#endif //_DEBUG

// CMachineView message handlers

BOOL CMachineView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	CRect rc; 
	GetClientRect(&rc);
	pDC->FillSolidRect(rc, RGB(247,252,255));
	return TRUE ;
}


void CMachineView::OnInitialUpdate()
{
	ModifyStyleEx( WS_EX_CLIENTEDGE, WS_EX_STATICEDGE);
	CFormView::OnInitialUpdate();
	MACHINE.m_pMachineViewWnd = this;
	if (!MACHINE.m_dbRecord.OpenDataBase())
	{
		BLMessageBox(_T("数据库打开失败，继续操作将不能保存报警及生产信息"),MB_OK);
	}
	for (int i = 0;i < 4;i++)
	{
		MACHINE.m_dbRecord.GetRecordCount(i);
	}

	MACHINE.Initialize(GetSafeHwnd());
	MACHINE.m_ReportFile.Initial(MACHINE.m_dbRecord.m_Connection);

	// TODO: Add your specialized code here and/or call the base class
	m_OverViewPage.Create(IDD_DLG_OVERVIEW,&m_TabCtrl);
	m_ModuleMonitorPage.Create(IDD_DLG_MODULE_MONITOR,&m_TabCtrl);
	m_MaintenacePage.Create(IDD_DLG_MAINTENANCE,&m_TabCtrl);
	m_HistoryPage.Create(IDD_DLG_HISTORY,&m_TabCtrl);
	m_ParameterPage.Create(IDD_DLG_PARAMETER,&m_TabCtrl);
	m_TestToolDlg.Create(IDD_DIG_TEST_TOOL,&m_TabCtrl);
	m_MesSetDlg.Create(IDD_DIG_MES_SET,&m_TabCtrl);

	m_OverViewPage.ShowWindow(SW_SHOW);
	m_ModuleMonitorPage.ShowWindow(SW_HIDE);
	m_MaintenacePage.ShowWindow(SW_HIDE);
	m_HistoryPage.ShowWindow(SW_HIDE);
	m_ParameterPage.ShowWindow(SW_HIDE);
	m_TestToolDlg.ShowWindow(SW_HIDE);
	m_MesSetDlg.ShowWindow(SW_HIDE);

	m_TabCtrl.InsertItem(0,_T("主 界 面"));
	m_TabCtrl.InsertItem(1,_T("监视界面"));
	m_TabCtrl.InsertItem(2,_T("维护界面"));
	m_TabCtrl.InsertItem(3,_T("历史记录"));
	m_TabCtrl.InsertItem(4,_T("参数设置"));
	m_TabCtrl.InsertItem(5,_T("调试工具"));
	m_TabCtrl.InsertItem(6,_T("MES设置"));

	AdjustLayout();

	m_Font.CreateFont(22,0,0,0,400,0,0,0,1,0,0,0,0,_T("Arial"));
	m_TabCtrl.SetFont(&m_Font);

	m_TabCtrl.SetItemSize(CSize(1,40));

	((CMainFrame*)GetParentFrame())->Init(MACHINE.m_pRunCtrl);
	((CMainFrame*)AfxGetMainWnd())->m_wndRibbonBar.InitState(MACHINE.m_pRunCtrl);

	GdiplusStartup(&g_gdiplusToken,&g_gdiplusStartupInput,NULL);
	MACHINE.ReadMachineParamter();
	if(MACHINE.m_nTowerRed < 0 || MACHINE.m_nTowerAmber < 0 || MACHINE.m_nTowerGreen < 0 || MACHINE.m_nTowerBuzzer < 0)
	{

	}
	else
	{
		m_wndTowerLight.Initialize(MACHINE.m_pRunCtrl, OUTPUTS[MACHINE.m_nTowerRed],OUTPUTS[MACHINE.m_nTowerAmber], OUTPUTS[MACHINE.m_nTowerGreen],OUTPUTS[MACHINE.m_nTowerBuzzer]);
	}

	if(MACHINE.m_nRedLight < 0 || MACHINE.m_nYellowLight < 0 || MACHINE.m_nGreenLight < 0 || MACHINE.m_nLighthouseBuzzer < 0)
	{

	}
	else
	{
		m_wndTowerLight2.Initialize(MACHINE.m_pRunCtrl, OUTPUTS[MACHINE.m_nRedLight],OUTPUTS[MACHINE.m_nYellowLight], OUTPUTS[MACHINE.m_nGreenLight],OUTPUTS[MACHINE.m_nLighthouseBuzzer]);
	}

	UpdateLanguage();

	SetTimer(TIMER_MACHINE_VIEW, 1000, NULL);
}

void CMachineView::DoDataExchange(CDataExchange* pDX)
{
	// TODO: Add your specialized code here and/or call the base class
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_FORM, m_TabCtrl);
	DDX_Control(pDX, IDC_TOWER_LIGHT, m_wndTowerLight);
	DDX_Control(pDX, IDC_TOWER_LIGHT2, m_wndTowerLight2);
}

void CMachineView::AdjustLayout()
{
	CRect rcView;
	GetClientRect(rcView);

	if(m_TabCtrl.GetSafeHwnd() != NULL)
	{
		m_TabCtrl.MoveWindow(0, 0, rcView.right, rcView.bottom, SWP_SHOWWINDOW);
		CRect rcTab;
		m_TabCtrl.GetClientRect(rcTab);
		rcTab.top += 30;
		rcTab.bottom -= 3;
		rcTab.left += 3;
		rcTab.right -= 3;

		if(m_OverViewPage.GetSafeHwnd() != NULL)
		{
			m_OverViewPage.MoveWindow(&rcTab);
		}

		if(m_ModuleMonitorPage.GetSafeHwnd() != NULL)
		{
			m_ModuleMonitorPage.MoveWindow(&rcTab);
		}

		if(m_MaintenacePage.GetSafeHwnd() != NULL)
		{
			m_MaintenacePage.MoveWindow(&rcTab);
		}

		if(m_HistoryPage.GetSafeHwnd() != NULL)
		{
			m_HistoryPage.MoveWindow(&rcTab);
		}

		if(m_ParameterPage.GetSafeHwnd() != NULL)
		{
			m_ParameterPage.MoveWindow(&rcTab);
		}
 		if(m_TestToolDlg.GetSafeHwnd() != NULL)
 		{
 			m_TestToolDlg.MoveWindow(&rcTab);
 		}

		if (m_MesSetDlg.GetSafeHwnd() != NULL)
		{
			m_MesSetDlg.MoveWindow(&rcTab);
		}
	}
}


void CMachineView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	AdjustLayout();
	// TODO: Add your message handler code here
}


void CMachineView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CFormView::OnPaint() for painting messages
	CRect rc;
	GetClientRect(&rc);
	dc.FillSolidRect(rc, RGB(247,252,255));
}


void CMachineView::OnTcnSelchangeTabForm(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

	int iCurSelTab = m_TabCtrl.GetCurSel();
	int nLevel = MACHINE.m_dbRecord.GetCurUserLevel();
	int nShowTabSel = MAIN_OVER_VIEW;

	if(MAIN_OVER_VIEW <= iCurSelTab && iCurSelTab <= MES_PAGE_VIEW)
	{
		if(iCurSelTab == MAIN_OVER_VIEW || iCurSelTab == MODULE_MONITOR_VIEW || iCurSelTab == MAINTENACE_PAGE_VIEW || iCurSelTab == HISTORY_PAGE_VIEW)
		{
			if (MACHINE.m_nIStop > -1 && INPUTS[MACHINE.m_nIStop]->IsOn() && iCurSelTab == MAINTENACE_PAGE_VIEW)
			{
				BLMessageBox(_T("设备【停止】按钮被按下，不能进入维护界面"));
				return;
			}
			nShowTabSel = iCurSelTab;
			m_OverViewPage.ShowWindow(MAIN_OVER_VIEW == nShowTabSel);
			m_ModuleMonitorPage.ShowWindow(MODULE_MONITOR_VIEW == nShowTabSel);
			m_MaintenacePage.ShowWindow(MAINTENACE_PAGE_VIEW == nShowTabSel);
			m_HistoryPage.ShowWindow(HISTORY_PAGE_VIEW == nShowTabSel);
			m_ParameterPage.ShowWindow(PARAMTER_PAGE_VIEW == nShowTabSel);
			m_TestToolDlg.ShowWindow(DEBUG_PAGE_VIEW == nShowTabSel);
			m_MesSetDlg.ShowWindow(MES_PAGE_VIEW == nShowTabSel);
		}
		else
		{
			nShowTabSel = iCurSelTab;

			if(nLevel > 1 || nLevel < 0)
			{
				m_TabCtrl.SetCurSel(MAIN_OVER_VIEW);
				nShowTabSel = MAIN_OVER_VIEW;
				BLMessageBox(_T("无权限进入此界面"));
			}
			else
			{
				if(iCurSelTab == DEBUG_PAGE_VIEW)
				{
					if(MACHINE.m_pRobotOffload != NULL)
					{
						BOOL bConnected = MACHINE.m_pRobotOffload->m_pRobotClient.IsConnect();
						if(bConnected)
						{
							//BOOL bRes = MACHINE.m_pRobotOffload->m_pRobotClient.SendToDeviceAndWait(ROBOT_LOW_SPEED);
// 							if(!bRes)
// 							{
// 								BLMessageBox(_T("更改机器人速度失败"), MB_ICONWARNING);
// 							}
// 							WRITELOG(bRes?_T("更改机器人速度为低速成功"):_T("更改机器人速度为低速失败"));
						}
					}
				}
			}

			m_OverViewPage.ShowWindow(MAIN_OVER_VIEW == nShowTabSel);
			m_ModuleMonitorPage.ShowWindow(MODULE_MONITOR_VIEW == nShowTabSel);
			m_MaintenacePage.ShowWindow(MAINTENACE_PAGE_VIEW == nShowTabSel);
			m_HistoryPage.ShowWindow(HISTORY_PAGE_VIEW == nShowTabSel);
			m_ParameterPage.ShowWindow(PARAMTER_PAGE_VIEW == nShowTabSel);
			m_TestToolDlg.ShowWindow(DEBUG_PAGE_VIEW == nShowTabSel);
			m_MesSetDlg.ShowWindow(MES_PAGE_VIEW == nShowTabSel);
		}
	}
}

LRESULT CMachineView::OnShowMessage( WPARAM wParam, LPARAM lParam )
{
	if(m_bShowDialogAlready)
	{
		return 0;
	}

	m_bShowDialogAlready = TRUE;

	int iMsgID = (int)(wParam & 0xFFFF);
	int iModule = (int)(lParam & 0xFFFF);

	if(iModule < 0 || iModule > MACHINE.m_numModules)
	{
		iModule = 0;
	}

	CString strSection;
	strSection.Format(_T("M%04d"), iMsgID);

	// get the information and send to overview panel
	CString strMessage = LoadWarnMessage(iMsgID);
	int abc = 1;
	if (iMsgID < 1000)
	{
		if (iMsgID >= MSG_MOTOR_HOME_FAIL && iMsgID <= MSG_MOTOR_MOVE_REL_FAIL)
		{
			int iMotor = (int)((wParam >> 16) & 0xFFFF);
			int iLoc = (int)((lParam >> 16) & 0xFFFF);

			CString strMsg = MOTORS[iMotor]->m_strName; 
			if (MACHINE.m_pModules[iModule] != NULL)   //
			{
				strMsg.Format(_T("%s【%s】"), MOTORS[iMotor]->m_strName, MACHINE.m_pModules[iModule]->m_strName);
			}
			strMessage.Replace(_T("#NAME#"), strMsg);
			if(iLoc > -1 && iLoc < MOTORS[iMotor]->GetLocations())
			{
				strMessage.Replace(_T("#LOCATION#"), MOTORS[iMotor]->GetLocation(iLoc).m_strName);
			}

			CString strMesId, strMes, strTemp;
			strMesId.Format(_T("%d"), iMsgID);
			strMes = strMessage.Mid(strMessage.Find(_T("("))+1 ,strMessage.Find(_T(")"))-strMessage.Find(_T("("))-1);
			strMes = strMes.Right(strMes.GetLength()-1);
			strTemp = strMesId + strMes;
			m_vecAlarmList.push_back(strTemp);
		}
		else if (iMsgID >= MSG_INPUT_NOT_ON && iMsgID <= MSG_WAIT_INPUT_OFF_TIMEOUT)
		{
			int iInput = (int)((wParam >> 16) & 0xFFFF);
			CString strMsg = INPUTS[iInput]->m_strName; 
			if (MACHINE.m_pModules[iModule] != NULL)   //
			{
				strMsg.Format(_T("%s【%s】"), INPUTS[iInput]->m_strName, MACHINE.m_pModules[iModule]->m_strName);
			}
			strMessage.Replace(_T("#NAME#"), strMsg);
			CString strMesId, strMes, strTemp;
			strMes=INPUTS[iInput]->m_strName.Left(5);
			strMes=strMes.Right(4);
			strMesId.Format(_T("%d"),iMsgID);
			strTemp = strMesId + strMes;
			m_vecAlarmList.push_back(strTemp);
		}
		else if (iMsgID >= MSG_WAIT_EVT_ON_TIMEOUT && iMsgID <= MSG_WAIT_EVT_OFF_TIMEOUT)
		{
			int iEvt = (int)((wParam >> 16) & 0xFFFF);
			if(iEvt < EVENTS.GetNumEvents())
			{
				strMessage.Replace(_T("#NAME#"), EVENTS[iEvt]->m_strName);
			}
			{
				strMessage.Replace(_T("#NAME#"), _T("未知事件"));
			}
			CString strMesId, strMes, strTemp;
			strMes=strMessage.Left(5);
			strMes=strMes.Right(4);
			CString strMsgid;
			strMesId.Format(_T("%d"),iMsgID);
			strTemp = strMesId + strMes;
			m_vecAlarmList.push_back(strTemp);
		}
			else
		{
			CString strMesId;
			strMesId.Format(_T("%d"), iMsgID);
			m_vecAlarmList.push_back(strMesId);
		}
	}
		else if ( iMsgID >= 1000 && iMsgID < 5000 )
    {
		CString strMesId;
		strMesId.Format(_T("%d"), iMsgID);
		m_vecAlarmList.push_back(strMesId);
    }
	CString strModuleName;
	if(0 <= iModule && iModule < MACHINE.m_numModules)
	{
		strModuleName = MACHINE.m_pModules[iModule]->m_strName;
	}
	int	iMsgType = ::IniReadInt(strSection, _T("Type"), MSG_MESSAGE,::GetAbsolutePathName(MESSAGE_CFG));
	BOOL bShowInList = ::IniReadBoolean(strSection, _T("ShowInList"), TRUE,::GetAbsolutePathName(MESSAGE_CFG));
	CString strShowMsg;
	strShowMsg.Format(_T("【%s】\r\n%d %s"), strModuleName, iMsgID, strMessage);
	PRINTF(_T("Message: ") + strShowMsg);

	MACHINE.m_vecAlarmList.push_back(strMessage);

	// if show the dialog box, show it
	BOOL bShowDialog = ::IniReadBoolean(strSection, _T("ShowDialog"), TRUE,::GetAbsolutePathName(MESSAGE_CFG));
	DWORD dwStartTime = GetTickCount();

	if (bShowDialog)
	{
		UINT nType = MB_OK;
		switch(iMsgType)
		{
		case MSG_MESSAGE:
			nType = MB_OK | MB_ICONINFORMATION;
			break;
		case MSG_WARNING:
			nType = MB_OK | MB_ICONEXCLAMATION;
			break;
		case MSG_QUESTION:
			nType = MB_YESNO | MB_ICONQUESTION;
			break;
		case MSG_ALARM:
			nType = MB_OK | MB_ICONSTOP;
			break;
		default:
			ASSERT(0);
			break;
		}

		MACHINE.m_ReportFile.UpdateAlmRecord(CTime::GetCurrentTime(), iMsgID, strMessage, iMsgType, iModule, strModuleName); //验收数据功能
		int res = BLMessageBox(strMessage, nType);
		if (m_vecAlarmList.size() > 0)
		{
			sort(m_vecAlarmList.begin(), m_vecAlarmList.end()); //排序
			m_vecAlarmList.erase(unique(m_vecAlarmList.begin(), m_vecAlarmList.end()), m_vecAlarmList.end());  //删除相同的
			MACHINE.AndonMachineAlarms(MACHINE.m_strResource,45, m_vecAlarmList);
			m_vecAlarmList.clear();
		}
		if (MACHINE.m_vecAlarmList.size() > 0)
		{
			sort(MACHINE.m_vecAlarmList.begin(), MACHINE.m_vecAlarmList.end()); //排序
			MACHINE.m_vecAlarmList.erase(unique(MACHINE.m_vecAlarmList.begin(), MACHINE.m_vecAlarmList.end()), MACHINE.m_vecAlarmList.end());  //删除相同的
		}

		for (int i = 0; i < (int)MACHINE.m_vecAlarmList.size(); i++)
		{
			MACHINE.m_strAlarmMes.Format(_T("%s\r\n"),MACHINE.m_vecAlarmList[i]);
		}
	}

	if (MACHINE.m_bCheckMes && 1 == MACHINE.m_objMes.X_isConnected())
	{
		MACHINE.m_MachineStatus.strRun = L"0";
		MACHINE.m_MachineStatus.strStop = L"0";
		MACHINE.m_MachineStatus.strFault = L"1";
		MACHINE.m_MachineStatus.strWaiting = L"0";
		MACHINE.m_MachineStatus.strErrCode = strMessage;
		MACHINE.m_objMes.X_UpdateStatus(MACHINE.m_MachineStatus);
	}
	// record the message, if it specified or it is an alarm message
	if (::IniReadBoolean(strSection, _T("Record"), FALSE, ::GetAbsolutePathName(MESSAGE_CFG))|| iMsgType == MSG_ALARM)
	{
		CString strTmp; 
		strTmp.Format(_T(""), GetTickCount()-dwStartTime);
	}

	m_bShowDialogAlready = FALSE;
	return 0;
}

LRESULT CMachineView::OnShowMessageEX( WPARAM wParam, LPARAM lParam )
{
	if(m_bShowMsgBoxAlready)
	{
		return 0;
	}
	m_bShowMsgBoxAlready = TRUE;

	int iMsgType = (int)wParam;
	CString strMsg = (CString)(LPCTSTR)lParam;

	UINT nType = MB_OK;
	switch(iMsgType)
	{
	case MSG_MESSAGE:
		nType = MB_OK | MB_ICONINFORMATION;
		break;
	case MSG_WARNING:
		nType = MB_OK | MB_ICONEXCLAMATION;
		break;
	case MSG_QUESTION:
		nType = MB_YESNO | MB_ICONQUESTION;
		break;
	case MSG_ALARM:
		nType = MB_OK | MB_ICONSTOP;
		break;
	default:
		ASSERT(0);
		break;
	}
	BLMessageBox(strMsg, nType);

	m_bShowMsgBoxAlready = FALSE;
	return 0;
}

void CMachineView::OnLanguageChinese()
{
	// TODO: Add your command handler code here
	SetLanguage(_T("CHS"));
	UpdateLanguage();
}


void CMachineView::OnLanguageEnglish()
{
	// TODO: Add your command handler code here
	SetLanguage(_T("ENG"));
	UpdateLanguage();
}

void CMachineView::UpdateLanguage()
{
	TCITEMW tw;
	tw.mask = TCIF_TEXT;
	if (GetLanguage() == _T("CHS") )
	{
		tw.pszText = _T("主 界 面");
		m_TabCtrl.SetItem(0,&tw);
		tw.pszText = _T("监视界面");
		m_TabCtrl.SetItem(1,&tw);
		tw.pszText = _T("维护界面");
		m_TabCtrl.SetItem(2,&tw);
		tw.pszText = _T("历史记录");
		m_TabCtrl.SetItem(3,&tw);
		tw.pszText = _T("参数设置");
		m_TabCtrl.SetItem(4,&tw);
 		tw.pszText = _T("调试工具");
 		m_TabCtrl.SetItem(5,&tw);
 		tw.pszText = _T("MES设置");
 		m_TabCtrl.SetItem(6,&tw);
	}
	else
	{
		tw.pszText = _T("MainView");
		m_TabCtrl.SetItem(0,&tw);
		tw.pszText = _T("MonitorView");
		m_TabCtrl.SetItem(1,&tw);
		tw.pszText = _T("Maintenance");
		m_TabCtrl.SetItem(2,&tw);
		tw.pszText = _T("History");
		m_TabCtrl.SetItem(3,&tw);
		tw.pszText = _T("Parameter");
 		m_TabCtrl.SetItem(4,&tw);
 		tw.pszText = _T("TOOL DLG");
 		m_TabCtrl.SetItem(5,&tw);
 		tw.pszText = _T("MES DLG");
		m_TabCtrl.SetItem(6,&tw);
	}
	if (m_OverViewPage.GetSafeHwnd() != NULL)
	{
		m_OverViewPage.UpdateLanguage();
	}
	if (m_ModuleMonitorPage.GetSafeHwnd() != NULL)
	{
		m_ModuleMonitorPage.UpdateLanguage();
	}
	if (m_MaintenacePage.GetSafeHwnd()!= NULL)
	{
		m_MaintenacePage.UpdateLanguage();
	}
	if (m_HistoryPage.GetSafeHwnd() != NULL)
	{
		m_HistoryPage.UpdateLanguage();
	}
	if (m_ParameterPage.GetSafeHwnd() != NULL)
	{
		m_ParameterPage.UpdateLanguage();
	}

	MACHINE.UpdateLanguage();
	((CMainFrame*)GetParentFrame())->UpdateLanguage();
}

LRESULT CMachineView::OnUserLogInOutMsg(WPARAM wparam, LPARAM lparam)
{
	NMHDR nmhrd;
	LRESULT lresult;

	m_TabCtrl.SetCurSel(0);
	OnTcnSelchangeTabForm(&nmhrd, &lresult);
	m_ParameterPage.OnCbnSelchangeParameterProductList();
	return 0;
}