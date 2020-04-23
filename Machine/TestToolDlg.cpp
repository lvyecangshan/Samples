// TestToolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Machine.h"
#include "TestToolDlg.h"
#include "afxdialogex.h"
#include <windows.h>  
#include <strsafe.h>  

#define UPDATE_CONTRL_STATUS  111
// CTestToolDlg 对话框

IMPLEMENT_DYNAMIC(CTestToolDlg, CDialogEx)

	CTestToolDlg::CTestToolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestToolDlg::IDD, pParent)
{
	m_nCount = 0;
	m_nCountEx = 0;
}

CTestToolDlg::~CTestToolDlg()
{
	
}

void CTestToolDlg::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_LIST_ROBOT_CMD_LIST, m_RobotCmdList);
	DDX_Control(pDX, IDC_LIST_ROBOT_RESPONSE_LIST, m_RobotResponseList);
	DDX_Control(pDX, IDC_LIST_BARCODE_LIST, m_ScanCodeList);
	DDX_Control(pDX, IDC_LIST_SHOW_RECV, m_ShowRecv);
	DDX_Control(pDX, IDC_LIST_SHOW_SEND, m_ShowSend);
}


BEGIN_MESSAGE_MAP(CTestToolDlg, CDialogEx)
	ON_WM_VSCROLL()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_OPNE_CLOSE_PRESS, &CTestToolDlg::OnBnClickedBtnOpneClosePress)
	ON_BN_CLICKED(IDC_BTN_ZERO_PRESS_VALUE, &CTestToolDlg::OnBnClickedBtnZeroPressValue)
	ON_BN_CLICKED(IDC_BTN_GET_PRESS_VALUE, &CTestToolDlg::OnBnClickedBtnGetPressValue)
	ON_BN_CLICKED(IDC_RADIO_SHELL_STATION1, &CTestToolDlg::OnBnClickedRadioShellStation1)
	ON_BN_CLICKED(IDC_RADIO_SHELL_STATION2, &CTestToolDlg::OnBnClickedRadioShellStation2)
	ON_BN_CLICKED(IDC_BTN_OPNE_CLOSE_ROBOT, &CTestToolDlg::OnBnClickedBtnOpneCloseRobot)
	ON_BN_CLICKED(IDC_BTN_SEND_ROBOT_CMD, &CTestToolDlg::OnBnClickedBtnSendRobotCmd)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RADIO_HIGH_SPEED, &CTestToolDlg::OnBnClickedRadioHighSpeed)
	ON_BN_CLICKED(IDC_RADIO_MIDDLE_SPEED, &CTestToolDlg::OnBnClickedRadioMiddleSpeed)
	ON_BN_CLICKED(IDC_RADIO_SLOW_SPEED, &CTestToolDlg::OnBnClickedRadioSlowSpeed)
	ON_BN_CLICKED(IDC_BTN_OPEN_CLOSE_BARCODE, &CTestToolDlg::OnBnClickedBtnOpenCloseBarcode)
	ON_BN_CLICKED(IDC_BTN_SCAN_BARCODE, &CTestToolDlg::OnBnClickedBtnScanBarcode)
	ON_BN_CLICKED(IDC_BUTTON1, &CTestToolDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON4, &CTestToolDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON3, &CTestToolDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON_SERVER_CONNECT, &CTestToolDlg::OnBnClickedButtonServerConnect)
	ON_BN_CLICKED(IDC_BUTTON_DISCONNECT_SERVER, &CTestToolDlg::OnBnClickedButtonDisconnectServer)
	ON_BN_CLICKED(IDC_BUTTON_TEST_SEND, &CTestToolDlg::OnBnClickedButtonTestSend)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_CONTENT, &CTestToolDlg::OnBnClickedButtonClearContent)
	ON_BN_CLICKED(IDC_CHECK_REBUILD_CT_LOG, &CTestToolDlg::OnBnClickedCheckRebuildCtLog)
END_MESSAGE_MAP()

void CTestToolDlg::ErrorExit(LPTSTR lpszFunction)
{   
	// Retrieve the system error message for the last-error code  

	LPVOID lpMsgBuf;  
	LPVOID lpDisplayBuf;  
	DWORD dw = GetLastError();   

	FormatMessage(  
		FORMAT_MESSAGE_ALLOCATE_BUFFER |   
		FORMAT_MESSAGE_FROM_SYSTEM |  
		FORMAT_MESSAGE_IGNORE_INSERTS,  
		NULL,  
		dw,  
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),  
		(LPTSTR) &lpMsgBuf,  
		0, NULL );  

	// Display the error message and exit the process  

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,   
		(lstrlen((LPCTSTR)lpMsgBuf)+lstrlen((LPCTSTR)lpszFunction)+40)*sizeof(TCHAR));   
	StringCchPrintf((LPTSTR)lpDisplayBuf,   
		LocalSize(lpDisplayBuf),  
		TEXT("%s failed with error %d: %s"),   
		lpszFunction, dw, lpMsgBuf);   
	MessageBoxW((LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

	LocalFree(lpMsgBuf);  
	LocalFree(lpDisplayBuf);  
	ExitProcess(dw);   
}  

BOOL CTestToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CheckRadioButton(IDC_RADIO_SHELL_STATION1, IDC_RADIO_SHELL_STATION2, IDC_RADIO_SHELL_STATION1);

	if(MACHINE.m_pRobotOffload != NULL)
	{
		CheckRadioButton(IDC_RADIO_HIGH_SPEED, IDC_RADIO_SLOW_SPEED, IDC_RADIO_HIGH_SPEED + MACHINE.m_pRobotOffload->m_unRobotSpeed);

		for (int i = ROBOT_GO_HOME; i < ALL_ROBOT_POS; i++)
		{
			m_RobotCmdList.AddString(g_robotcmd[i].strRemark);
		}
	}

	((CButton*)GetDlgItem(IDC_READ_ONE))->SetCheck(1);

	CRect Rct;

	m_ShowRecv.GetClientRect(&Rct);
	m_ShowRecv.SetExtendedStyle(m_ShowRecv.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_ShowRecv.InsertColumn(0,_T("序号"),LVCFMT_CENTER,Rct.Width()/7,0);
	m_ShowRecv.InsertColumn(1,_T("接收的内容"),LVCFMT_CENTER,Rct.Width()/2,1);
	m_ShowRecv.InsertColumn(2,_T("时间"),LVCFMT_CENTER,Rct.Width()/4,2);
	m_ShowRecv.InsertItem(0,_T("1"));
	m_ShowRecv.InsertItem(1,_T("2"));
	m_ShowRecv.InsertItem(2,_T("3"));
	m_ShowRecv.InsertItem(3,_T("4"));

	m_ShowSend.GetClientRect(&Rct);
	m_ShowSend.SetExtendedStyle(m_ShowRecv.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_ShowSend.InsertColumn(0,_T("序号"),LVCFMT_CENTER,Rct.Width()/7,0);
	m_ShowSend.InsertColumn(1,_T("发送的内容"),LVCFMT_CENTER,Rct.Width()/1.5,1);
	m_ShowSend.InsertColumn(2,_T("时间"),LVCFMT_CENTER,Rct.Width()/4,2);
	m_ShowSend.InsertItem(0,_T("1"));
	m_ShowSend.InsertItem(1,_T("2"));
	m_ShowSend.InsertItem(2,_T("3"));
	m_ShowSend.InsertItem(3,_T("4"));
	m_ShowSend.InsertItem(4,_T("5"));
	m_ShowSend.InsertItem(5,_T("6"));


	SetTimer(UPDATE_CONTRL_STATUS, 500, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CTestToolDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	SCROLLINFO Scrinfo;
	GetScrollInfo(SB_VERT,&Scrinfo,SIF_ALL);
	switch(nSBCode)
	{
	case SB_BOTTOM:
		{
			ScrollWindow(0,(Scrinfo.nPos-Scrinfo.nMax)*30);
			Scrinfo.nPos = Scrinfo.nMax;
			SetScrollInfo(SB_VERT,&Scrinfo,SIF_ALL);
			break;
		}
	case SB_TOP:
		{
			ScrollWindow(0,(Scrinfo.nPos-Scrinfo.nMin)*30);
			Scrinfo.nPos = Scrinfo.nMin;
			SetScrollInfo(SB_VERT,&Scrinfo,SIF_ALL);
			break;
		}
	case SB_LINEUP:
		{
			Scrinfo.nPos -= 1;
			if(Scrinfo.nPos < Scrinfo.nMin)
			{
				Scrinfo.nPos = Scrinfo.nMin;
				break;
			}
			SetScrollInfo(SB_VERT,&Scrinfo,SIF_ALL);
			ScrollWindow(0,10);
			break;
		}
	case SB_LINEDOWN:
		{
			Scrinfo.nPos += 1;
			if(Scrinfo.nPos > Scrinfo.nMax)
			{
				Scrinfo.nPos = Scrinfo.nMax;
				break;
			}
			SetScrollInfo(SB_VERT,&Scrinfo,SIF_ALL);
			ScrollWindow(0,-10);
			break;
		}
	case SB_PAGEUP:
		{
			Scrinfo.nPos -= 5;
			if(Scrinfo.nPos < Scrinfo.nMin)
			{
				Scrinfo.nPos = Scrinfo.nMin;
				break;
			}
			SetScrollInfo(SB_VERT,&Scrinfo,SIF_ALL);
			ScrollWindow(0,10*5);
			break;
		}
	case SB_PAGEDOWN:
		{
			Scrinfo.nPos += 5;
			if(Scrinfo.nPos > Scrinfo.nMax)
			{
				Scrinfo.nPos = Scrinfo.nMax;
				break;
			}
			SetScrollInfo(SB_VERT,&Scrinfo,SIF_ALL);
			ScrollWindow(0,-10*5);
			break;
		}
	case SB_ENDSCROLL:
		{
			break;
		}
	case SB_THUMBPOSITION:
		{
			break;
		}
	case SB_THUMBTRACK:
		{
			ScrollWindow(0,(Scrinfo.nPos - nPos)*10);
			Scrinfo.nPos = nPos;
			SetScrollInfo(SB_VERT,&Scrinfo,SIF_ALL);
			break;
		}
	default:
		break;
	}
	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CTestToolDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
}


void CTestToolDlg::OnBnClickedBtnOpneClosePress()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strMsg;
	int nCurSel = GetCheckedRadioButton(IDC_RADIO_SHELL_STATION1, IDC_RADIO_SHELL_STATION2) - IDC_RADIO_SHELL_STATION1;
	if(0 <= nCurSel && nCurSel <= 1 && MACHINE.m_pBatteryCore[nCurSel] != NULL)
	{
		BOOL bConnect = MACHINE.m_pBatteryCore[nCurSel]->m_PressClient.IsConnect();

		CListBox *pList = (CListBox *)GetDlgItem(IDC_LIST_PRESS_VALUE);

		if(bConnect)
		{
			MACHINE.m_pBatteryCore[nCurSel]->m_PressClient.Disconnect();
			GetDlgItem(IDC_BTN_OPNE_CLOSE_PRESS)->SetWindowText(_T("连接"));
			if(pList != NULL)
			{
				if(pList->GetCount() > 10)
					pList->ResetContent();
				pList->InsertString(pList->GetCount(), _T("断开连接"));
				pList->SetCurSel(pList->GetCount() -1);
			}
			strMsg.Format(_T("用户断开入壳%d压力仪连接"), nCurSel + 1);
			WRITELOG(strMsg);
		}
		else
		{
			Sleep(1000);
			CString strIP = MACHINE.m_pBatteryCore[nCurSel]->m_strPressSenseIP;
			int nPort = MACHINE.m_pBatteryCore[nCurSel]->m_nPressSensePort;
			BOOL bRes = MACHINE.m_pBatteryCore[nCurSel]->m_PressClient.Connect(strIP, nPort);

			GetDlgItem(IDC_BTN_OPNE_CLOSE_PRESS)->SetWindowText(bRes?_T("断开"):_T("连接"));

			if(pList != NULL)
			{
				if(pList->GetCount() > 10)
					pList->ResetContent();
				pList->InsertString(pList->GetCount(), bRes?_T("连接成功"):_T("连接失败"));
				pList->SetCurSel(pList->GetCount() -1);
			}

			strMsg.Format(_T("用户连接入壳%d压力仪连接，连接%s"), nCurSel + 1, bRes?_T("连接成功"):_T("连接失败"));
			WRITELOG(strMsg);
		}
		pList = NULL;
	}
}


void CTestToolDlg::OnBnClickedBtnZeroPressValue()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strMsg;
	int nCurSel = GetCheckedRadioButton(IDC_RADIO_SHELL_STATION1, IDC_RADIO_SHELL_STATION2) - IDC_RADIO_SHELL_STATION1;

	if(0 <= nCurSel && nCurSel <= 1 && MACHINE.m_pBatteryCore[nCurSel] != NULL)
	{
		int OZeroOut = MACHINE.m_pBatteryCore[nCurSel]->m_OSetZero;

		CListBox *pList = (CListBox *)GetDlgItem(IDC_LIST_PRESS_VALUE);

		if(OZeroOut > -1)
		{
			OUTPUTS[OZeroOut]->On();
			Sleep(300);
			OUTPUTS[OZeroOut]->Off();

			if(pList != NULL)
			{
				if(pList->GetCount() > 10)
					pList->ResetContent();
				pList->InsertString(pList->GetCount(), _T("压力仪已清零"));
				pList->SetCurSel(pList->GetCount() -1);
			}
		}
		pList = NULL;

		strMsg.Format(_T("用户对入壳%d压力仪执行数值清零操作"), nCurSel + 1);
		WRITELOG(strMsg);
	}
}


void CTestToolDlg::OnBnClickedBtnGetPressValue()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strMsg;

	int nCurSel = GetCheckedRadioButton(IDC_RADIO_SHELL_STATION1, IDC_RADIO_SHELL_STATION2) - IDC_RADIO_SHELL_STATION1;

	if(0 <= nCurSel && nCurSel <= 1 && MACHINE.m_pBatteryCore[nCurSel] != NULL)
	{
		CListBox *pList = (CListBox*)GetDlgItem(IDC_LIST_PRESS_VALUE);

		BOOL bConnect = MACHINE.m_pBatteryCore[nCurSel]->m_PressClient.IsConnect();

		if(pList != NULL)
		{
			if(!bConnect)
			{
				if(pList->GetCount() > 10)
					pList->ResetContent();
				pList->InsertString(pList->GetCount(), _T("压力尚未连接，不能获取压力，请先连接"));
				pList->SetCurSel(pList->GetCount() -1);

				strMsg.Format(_T("用户读取入壳%d压力仪当前数值，因压力仪未连接，命令未被发送"), nCurSel + 1);
				WRITELOG(strMsg);
			}
			else
			{
				float fValue = 0.0;
				CString strKey;

				CString strCmd = _T("#01\r\n");
				CString strResult = _T("");
				//BOOL bRes = MACHINE.m_pBatteryCore[nCurSel]->m_PressSenseClient.GetPressValue(1, &fValue);
				BOOL bRes = MACHINE.m_pBatteryCore[nCurSel]->m_PressClient.SendToDeviceAndWait(strCmd,strResult);
				if(pList->GetCount() > 5)
					pList->ResetContent();

				if(!bRes)
				{
					strKey.Format(_T("【%s】获取压力失败，请检查接线"), MACHINE.m_pBatteryCore[nCurSel]->m_strName);
					strMsg.Format(_T("用户读取入壳%d压力仪当前数值，因超时反馈，数值未接收成功"), nCurSel + 1);
				}
				else
				{
					
					int nStart = strResult.Find(_T("-"));
					int nEnd = strResult.Find(_T("@"));
					strResult = strResult.Mid(nStart,nEnd - 1);
					fValue = _wtof(strResult);
					strKey.Format(_T("读取压力数值:%.2f"), fValue);
					strMsg.Format(_T("用户读取入壳%d压力仪当前数值，读取数值:%.2f"), nCurSel + 1, fValue);
					/*strKey.Format(_T("读取压力数值:%.2f"), fValue);
					strMsg.Format(_T("用户读取入壳%d压力仪当前数值，读取数值:%.2f"), nCurSel + 1, fValue);*/
				}

				pList->InsertString(pList->GetCount(), strKey);
				pList->SetCurSel(pList->GetCount() -1);

				WRITELOG(strMsg);
			}
		}
		pList = NULL;
	}
}


void CTestToolDlg::OnBnClickedRadioShellStation1()
{
	// TODO: 在此添加控件通知处理程序代码
	if(MACHINE.m_pBatteryCore[0] != NULL)
	{
		BOOL bConnect = MACHINE.m_pBatteryCore[0]->m_PressClient.IsConnect();

		GetDlgItem(IDC_BTN_OPNE_CLOSE_PRESS)->SetWindowText(bConnect?_T("断开"):_T("连接"));

		CListBox *pList = (CListBox *)GetDlgItem(IDC_LIST_PRESS_VALUE);

		if(pList != NULL)
		{
			pList->ResetContent();
			pList->InsertString(pList->GetCount(), _T("切换到入壳工位#1"));
			pList->SetCurSel(pList->GetCount() -1);
		}

		pList = NULL;
	}
}


void CTestToolDlg::OnBnClickedRadioShellStation2()
{
	// TODO: 在此添加控件通知处理程序代码
	if(MACHINE.m_pBatteryCore[1] != NULL)
	{
		BOOL bConnect = MACHINE.m_pBatteryCore[1]->m_PressClient.IsConnect();

		GetDlgItem(IDC_BTN_OPNE_CLOSE_PRESS)->SetWindowText(bConnect?_T("断开"):_T("连接"));

		CListBox *pList = (CListBox *)GetDlgItem(IDC_LIST_PRESS_VALUE);

		if(pList != NULL)
		{
			pList->ResetContent();
			pList->InsertString(pList->GetCount(), _T("切换到入壳工位#2"));
			pList->SetCurSel(pList->GetCount() -1);
		}

		pList = NULL;
	}
}


void CTestToolDlg::OnBnClickedBtnOpneCloseRobot()
{
	// TODO: 在此添加控件通知处理程序代码
	if(MACHINE.m_pRobotOffload != NULL)
	{
		BOOL bConnect = MACHINE.m_pRobotOffload->m_pRobotClient.IsConnect();

		if(bConnect)
		{
			MACHINE.m_pRobotOffload->m_pRobotClient.Disconnect();
			MACHINE.m_pRobotOffload->DisconnectRobot();
			GetDlgItem(IDC_BTN_OPNE_CLOSE_ROBOT)->SetWindowText(_T("连接"));

			if(m_RobotResponseList.GetCount() > 5)
				m_RobotResponseList.ResetContent();
			m_RobotResponseList.InsertString(m_RobotResponseList.GetCount(), _T("断开连接"));
			m_RobotResponseList.SetCurSel(m_RobotResponseList.GetCount()-1);

			WRITELOG(_T("用户进行机器人断开连接操作"));
		}
		else
		{
			CString strIP = MACHINE.m_pRobotOffload->m_strRobotIp;
			int nPort = MACHINE.m_pRobotOffload->m_nRobotPort;
			BOOL bRes = MACHINE.m_pRobotOffload->ConnectRobot();
			if (!bRes)
			{
				BLMessageBox(_T("检测到机器人的停止、运行信号未配置"),MB_ICONERROR|MB_ICONWARNING);
				return ;
			}
			bRes = MACHINE.m_pRobotOffload->m_pRobotClient.Connect(strIP, nPort);

			GetDlgItem(IDC_LIST_ROBOT_RESPONSE_LIST)->SetWindowText(bRes?_T("断开"):_T("连接"));

			if(m_RobotResponseList.GetCount() > 5)
				m_RobotResponseList.ResetContent();
			m_RobotResponseList.InsertString(m_RobotResponseList.GetCount(), bRes?_T("连接成功"):_T("连接失败"));
			m_RobotResponseList.SetCurSel(m_RobotResponseList.GetCount()-1);

			WRITELOG(bRes?_T("用户进行机器人连接操作，连接成功"):_T("用户进行机器人连接操作，连接失败"));

// 			if(bRes)
// 			{
// 				bRes = MACHINE.m_pRobotOffload->m_pRobotClient.SendToDeviceAndWait(ROBOT_LOW_SPEED);
// 				
// 				if(!bRes)
// 					BLMessageBox(_T("更改机器人速度为低速失败，请注意安全"), MB_ICONERROR);
// 				WRITELOG(bRes?_T("更改机器人速度为低速成功"):_T("更改机器人速度为低速失败"));
// 			}
		}
	}
}


void CTestToolDlg::OnBnClickedBtnSendRobotCmd()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strMsg;
	int nCurSel = m_RobotCmdList.GetCurSel();
	nCurSel += ROBOT_GO_HOME;
	if(ROBOT_GO_HOME <= nCurSel && nCurSel <= ROBOT_OFFLOAD_NG_POS && MACHINE.m_pRobotOffload != NULL)
	{
		BOOL bConnect = MACHINE.m_pRobotOffload->m_pRobotClient.IsConnect();

		if(!bConnect)
		{
			if(m_RobotResponseList.GetCount() > 10)
				m_RobotResponseList.ResetContent();
			m_RobotResponseList.InsertString(m_RobotResponseList.GetCount(), _T("机器人未连接，无法发送命令，请先连接"));
			m_RobotResponseList.SetCurSel(m_RobotResponseList.GetCount()-1);

			strMsg.Format(_T("用户发送命令移动机器人:到【%s】，因机器人未连接，该命令未被发送"), g_robotcmd[nCurSel].strRemark);
			WRITELOG(strMsg);
		}
		else
		{
			BOOL bRes = FALSE;
			int nCmdLevel = g_robotcmd[MACHINE.m_pRobotOffload->m_nCurRobotLoc].nCmdLevel;
		
			if(MACHINE.m_pRobotOffload->m_nCurRobotLoc == nCurSel)
			{
				bRes = MACHINE.m_pRobotOffload->m_pRobotClient.SendToDeviceAndWait(nCurSel);
				MACHINE.m_pRobotOffload->m_nCurRobotLoc = nCurSel;
			}
			else
			{
				if(nCmdLevel == 0)
				{
					// 获取上次指令，与当前指令比较，若当前指令=上次指令+1，并且当前指令是机器人下降，则命令正确
					// 当前指令机器人不会下降，则命令正确
					nCmdLevel = g_robotcmd[nCurSel].nCmdLevel;
					if((nCmdLevel == 1 && nCurSel == MACHINE.m_pRobotOffload->m_nCurRobotLoc + 1) || nCmdLevel == 0)
					{
						bRes = MACHINE.m_pRobotOffload->m_pRobotClient.SendToDeviceAndWait(nCurSel);
						MACHINE.m_pRobotOffload->m_nCurRobotLoc = nCurSel;
					}
					else
					{
						strMsg.Format(_T("当前机器人处于【%s】，不能直接移到【%s】，可先将机器人移到【%s】")
							, g_robotcmd[MACHINE.m_pRobotOffload->m_nCurRobotLoc].strRemark, g_robotcmd[nCurSel].strRemark
							, g_robotcmd[nCurSel -1].strRemark);
						BLMessageBox(strMsg, MB_ICONWARNING);

						strMsg.Format(_T("用户发送命令移动机器人到【%s】,但机器人当前位置在【%s】,该移动命令不安全，系统放弃执行")
							, g_robotcmd[nCurSel].strRemark, g_robotcmd[MACHINE.m_pRobotOffload->m_nCurRobotLoc].strRemark);
						WRITELOG(strMsg);
						return;
					}
				}
				else
				{
					// 获取上次指令，与当前指令比较，若当前指令=上次指令-1，并且当前指令是机器人下降，则命令正确
					if(nCurSel == MACHINE.m_pRobotOffload->m_nCurRobotLoc - 1)
					{
						bRes = MACHINE.m_pRobotOffload->m_pRobotClient.SendToDeviceAndWait(nCurSel);
						MACHINE.m_pRobotOffload->m_nCurRobotLoc = nCurSel;
					}
					else
					{
						strMsg.Format(_T("当前机器人处于【%s】，不能直接移到【%s】，可先将机器人移到【%s】")
							, g_robotcmd[MACHINE.m_pRobotOffload->m_nCurRobotLoc].strRemark, g_robotcmd[nCurSel].strRemark
							, g_robotcmd[MACHINE.m_pRobotOffload->m_nCurRobotLoc -1].strRemark);
						BLMessageBox(strMsg, MB_ICONWARNING);

						strMsg.Format(_T("用户发送命令移动机器人到【%s】,但机器人当前位置在【%s】,该移动命令不安全，系统放弃执行")
							, g_robotcmd[nCurSel].strRemark, g_robotcmd[MACHINE.m_pRobotOffload->m_nCurRobotLoc].strRemark);
						WRITELOG(strMsg);
						return;
					}
				}
			}
			
			if(bRes)
			{
				if(m_RobotResponseList.GetCount() > 10)
					m_RobotResponseList.ResetContent();

				strMsg.Format(_T("机器人移动到【%s】成功"), g_robotcmd[nCurSel].strRemark);
				m_RobotResponseList.InsertString(m_RobotResponseList.GetCount(), strMsg);
				m_RobotResponseList.SetCurSel(m_RobotResponseList.GetCount()-1);

				strMsg.Format(_T("用户发送命令移动机器人到【%s】,命令执行成功") , g_robotcmd[nCurSel].strRemark);
				WRITELOG(strMsg);
			}
			else
			{
				if(m_RobotResponseList.GetCount() > 10)
					m_RobotResponseList.ResetContent();
				m_RobotResponseList.InsertString(m_RobotResponseList.GetCount(), _T("发送改变移动命令，机器人反馈超时"));
				m_RobotResponseList.SetCurSel(m_RobotResponseList.GetCount()-1);

				strMsg.Format(_T("用户发送命令移动机器人到【%s】,命令执行反馈超时") , g_robotcmd[nCurSel].strRemark);
				WRITELOG(strMsg);
			}
		}
	}
}


void CTestToolDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(UPDATE_CONTRL_STATUS == nIDEvent)
	{
		BOOL bEnableCtrl = TRUE;
		int nState = MACHINE.m_pRunCtrl->GetMcState();

		if(nState == MC_INITIALIZING || nState == MC_RUNNING)
			bEnableCtrl = FALSE;

		CWnd* pWnd = GetWindow(GW_CHILD);
		while(pWnd != NULL)
		{
			GetDlgItem(pWnd->GetDlgCtrlID())->EnableWindow(bEnableCtrl);
			pWnd = pWnd->GetWindow(GW_HWNDNEXT);
		}

		BOOL bConnect = FALSE;
		int nCurSel = GetCheckedRadioButton(IDC_RADIO_SHELL_STATION1, IDC_RADIO_SHELL_STATION2) - IDC_RADIO_SHELL_STATION1;

		if(0 <= nCurSel && nCurSel <= 1 && MACHINE.m_pBatteryCore[nCurSel] != NULL)
		{
			bConnect = MACHINE.m_pBatteryCore[nCurSel]->m_PressClient.IsConnect();
			GetDlgItem(IDC_BTN_OPNE_CLOSE_PRESS)->SetWindowText(bConnect?_T("断开"):_T("连接"));
		}
		if(MACHINE.m_pRobotOffload != NULL)
		{
			bConnect = MACHINE.m_pRobotOffload->m_pRobotClient.IsConnect();
			GetDlgItem(IDC_BTN_OPNE_CLOSE_ROBOT)->SetWindowText(bConnect?_T("断开"):_T("连接"));

// 			unsigned int unSpeed = MACHINE.m_pRobotOffload->m_unRobotSpeed;
// 			ASSERT(0 <= unSpeed && unSpeed <= 2);
// 			CheckRadioButton(IDC_RADIO_HIGH_SPEED, IDC_RADIO_SLOW_SPEED, IDC_RADIO_HIGH_SPEED + unSpeed);

			CString strMsg;
			if(ROBOT_GO_HOME <= MACHINE.m_pRobotOffload->m_nCurRobotLoc &&  MACHINE.m_pRobotOffload->m_nCurRobotLoc <= ROBOT_OFFLOAD_NG_POS)
			{
				strMsg.Format(_T("%s，%s"), g_robotcmd[MACHINE.m_pRobotOffload->m_nCurRobotLoc].strRemark, g_robotcmd[MACHINE.m_pRobotOffload->m_unRobotSpeed].strRemark);
				GetDlgItem(IDC_STATIC_CUR_ROBOT_STATUS)->SetWindowText(strMsg);
			}
		}

		if(MACHINE.m_pOnloadCore != NULL)
		{
			bConnect = MACHINE.m_pOnloadCore->m_ScanCodeClient.IsConnect();
			GetDlgItem(IDC_BTN_OPEN_CLOSE_BARCODE)->SetWindowText(bConnect?_T("断开"):_T("连接"));
		}

		if (MACHINE.m_strShowSend.GetLength() > 0)  //发送显示 chen_6.13
		{
			CTime TMIE = CTime::GetCurrentTime();
			CString Shows;
			Shows.Format(_T("%02d:%02d:%02d"),TMIE.GetHour(),TMIE.GetMinute(),TMIE.GetSecond());

			m_ShowSend.SetItemText(m_nCount,1,MACHINE.m_strShowSend);  
			m_ShowSend.SetItemText(m_nCount,2,Shows);
			MACHINE.m_strShowSend.Empty();
			if (++m_nCount > m_ShowSend.GetItemCount())
			{
				m_nCount = 0;
			}
		}

		if (MACHINE.m_strShowRecv.GetLength() > 0)  //接收显示 chen_6.13
		{
			CTime TMIE = CTime::GetCurrentTime();
			CString Shows;
			Shows.Format(_T("%02d:%02d:%02d"),TMIE.GetHour(),TMIE.GetMinute(),TMIE.GetSecond());

			m_ShowRecv.SetItemText(m_nCountEx,1,MACHINE.m_strShowRecv);  
			m_ShowRecv.SetItemText(m_nCountEx,2,Shows);
			MACHINE.m_strShowRecv.Empty();
			if (++m_nCountEx > m_ShowRecv.GetItemCount())
			{
				m_nCountEx = 0;
			}
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CTestToolDlg::AlterRobotSpeed()
{
	CString strMsg;

	int nCurSpeedSel = GetCheckedRadioButton(IDC_RADIO_HIGH_SPEED, IDC_RADIO_SLOW_SPEED) - IDC_RADIO_HIGH_SPEED;

	ASSERT(ROBOT_HIGH_SPEED <= nCurSpeedSel && nCurSpeedSel <= ROBOT_LOW_SPEED);

	if(MACHINE.m_pRobotOffload != NULL)
	{
		BOOL bConnected = MACHINE.m_pRobotOffload->m_pRobotClient.IsConnect();

		if(!bConnected)
		{
 			if(m_RobotResponseList.GetCount() > 10)
 				m_RobotResponseList.ResetContent();
			m_RobotResponseList.InsertString(m_RobotResponseList.GetCount(), _T("机器人未连接，无法发送命令，请先连接"));
			m_RobotResponseList.SetCurSel(m_RobotResponseList.GetCount()-1);

			WRITELOG(_T("用户更改机器人运行速度，因机器人未连接，该命令未被发送"));
		}
		else
		{
			BOOL bRes = MACHINE.m_pRobotOffload->m_pRobotClient.SendToDeviceAndWait(nCurSpeedSel);

			if(bRes)
			{
 				if(m_RobotResponseList.GetCount() > 10)
 					m_RobotResponseList.ResetContent();

				MACHINE.m_pRobotOffload->m_unRobotSpeed = nCurSpeedSel;
				strMsg.Format(_T("改变机器人速度成功，当前速度：%s"), g_robotcmd[nCurSpeedSel].strRemark);
				m_RobotResponseList.InsertString(m_RobotResponseList.GetCount(), strMsg);
				m_RobotResponseList.SetCurSel(m_RobotResponseList.GetCount()-1);

				WRITELOG(strMsg);
			}
			else
			{
				if(m_RobotResponseList.GetCount() > 10)
					m_RobotResponseList.ResetContent();

				m_RobotResponseList.InsertString(m_RobotResponseList.GetCount(), _T("发送改变速度命令，机器人反馈超时"));
				m_RobotResponseList.SetCurSel(m_RobotResponseList.GetCount()-1);

				strMsg.Format(_T("发送改变速度命令(%s)，机器人超时反馈"), g_robotcmd[nCurSpeedSel].strRemark);
				WRITELOG(strMsg);
			}
		}
	}
}


void CTestToolDlg::OnBnClickedRadioHighSpeed()
{
	// TODO: 在此添加控件通知处理程序代码
	AlterRobotSpeed();
}


void CTestToolDlg::OnBnClickedRadioMiddleSpeed()
{
	// TODO: 在此添加控件通知处理程序代码
	AlterRobotSpeed();
}


void CTestToolDlg::OnBnClickedRadioSlowSpeed()
{
	// TODO: 在此添加控件通知处理程序代码
	AlterRobotSpeed();
}


void CTestToolDlg::OnBnClickedBtnOpenCloseBarcode()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strMsg;
	int nCurSel = GetCheckedRadioButton(IDC_READ_ONE, IDC_READ_TWO) - IDC_READ_ONE;
	if(MACHINE.m_pReadCode[nCurSel] != NULL)
	{

		BOOL bConnect = MACHINE.m_pReadCode[nCurSel]->m_pReadCode.IsConnect();

		if(bConnect)
		{
			MACHINE.m_pReadCode[nCurSel]->m_pReadCode.Disconnect();
			GetDlgItem(IDC_BTN_OPEN_CLOSE_BARCODE)->SetWindowText(_T("连接"));

			if(m_ScanCodeList.GetCount() > 5)
				m_ScanCodeList.ResetContent();
			m_ScanCodeList.InsertString(m_ScanCodeList.GetCount(), _T("断开连接"));
			m_ScanCodeList.SetCurSel(m_ScanCodeList.GetCount()-1);
			WRITELOG(_T("用户断开扫码枪连接"));
		}
		else
		{
			CString strIP = MACHINE.m_pReadCode[nCurSel]->m_strIp;
			int nPort = MACHINE.m_pReadCode[nCurSel]->m_nPort;
			BOOL bRes = MACHINE.m_pReadCode[nCurSel]->m_pReadCode.Connect(strIP, nPort);

			if (!bRes)
			{
				 MACHINE.m_pReadCode[nCurSel]->m_pReadCode.Disconnect();
			}

			GetDlgItem(IDC_BTN_OPEN_CLOSE_BARCODE)->SetWindowText(bRes?_T("断开"):_T("连接"));

			if(m_ScanCodeList.GetCount() > 5)
				m_ScanCodeList.ResetContent();
			if(bRes)
			{
				m_ScanCodeList.InsertString(m_ScanCodeList.GetCount(), _T("连接成功"));
			}
			else
			{
				m_ScanCodeList.InsertString(m_ScanCodeList.GetCount(),_T("连接失败,请检查串口服务器配置是否正确！"));
			}
			
			m_ScanCodeList.SetCurSel(m_ScanCodeList.GetCount()-1);

			strMsg.Format(_T("用户连接扫码枪，%s"), bRes?_T("连接成功"):_T("连接失败"));
			WRITELOG(strMsg);
		}
	}
}


void CTestToolDlg::OnBnClickedBtnScanBarcode()
{
	// TODO: 在此添加控件通知处理程序代码
	int nCurSel = GetCheckedRadioButton(IDC_READ_ONE, IDC_READ_TWO) - IDC_READ_ONE;
	if(MACHINE.m_pReadCode[nCurSel] != NULL)
	{
		BOOL bConnect = MACHINE.m_pReadCode[nCurSel]->m_pReadCode.IsConnect();

		if(!bConnect)
		{
			if(m_ScanCodeList.GetCount() > 5)
				m_ScanCodeList.ResetContent();
			m_ScanCodeList.InsertString(m_ScanCodeList.GetCount(), _T("扫码枪未连接，请先连接"));
			m_ScanCodeList.SetCurSel(m_ScanCodeList.GetCount()-1);

			WRITELOG(_T("用户执行扫码操作，因扫码枪未连接，命令未被发送"));
		}
		else
		{
			CString strBarCode=_T("");
			CStringA strCmd;

			strCmd = MACHINE.m_pReadCode[nCurSel]->m_strScanCmd;
			strCmd.Trim();
			strCmd += _T("\r\n");
		
			BOOL bRes = MACHINE.m_pReadCode[nCurSel]->m_pReadCode.ReadCode(strCmd.GetBuffer(), strBarCode);
			strCmd.ReleaseBuffer();

			strBarCode.Trim();

			if(m_ScanCodeList.GetCount() > 5)
				m_ScanCodeList.ResetContent();

			CString strMsg;
			if(!bRes)
			{
				
				if(strBarCode.CompareNoCase(_T("NoRead")) == 0)
				{
					strMsg.Format(_T("扫码失败，请检测是否放置条码！"), strBarCode);
					m_ScanCodeList.InsertString(m_ScanCodeList.GetCount(), strMsg);
					strMsg = _T("扫码失败，未读取到条码！");				
				}
				else
				{
					m_ScanCodeList.InsertString(m_ScanCodeList.GetCount(), _T("扫码失败，请检测扫码枪是否正常！"));
					strMsg = _T("用户执行扫码操作，反馈超时");
				}
			}
			else
			{
				strMsg.Format(_T("扫码成功, %s"), strBarCode);
				m_ScanCodeList.InsertString(m_ScanCodeList.GetCount(), strMsg);
				strMsg.Format(_T("用户执行扫码操作, 读取条码%s"), strBarCode);
			}
			WRITELOG(strMsg);
			m_ScanCodeList.SetCurSel(m_ScanCodeList.GetCount() - 1);
		}
	}
}


BOOL CTestToolDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->wParam == VK_SPACE || pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CTestToolDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	if(MACHINE.m_pOnloadCore != NULL)
	{
		MACHINE.m_pOnloadCore->m_bEnableDocking = TRUE;
	}
}
	

void CTestToolDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	if(MACHINE.m_pOnloadShellPnp != NULL)
		MACHINE.m_pOnloadShellPnp->m_bEnableDocking = TRUE;
}


void CTestToolDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	MACHINE.m_pBatteryOffloadPnp[0]->m_bOffload = TRUE;
	MACHINE.m_pBatteryOffloadPnp[1]->m_bOffload = TRUE;
}


void CTestToolDlg::OnBnClickedButtonServerConnect()
{
	 //TODO: 在此添加控件通知处理程序代码
	if (MACHINE.m_pRobotOffload !=NULL && !MACHINE.m_pRobotOffload->m_Client.IsConnect()) //防止重复连接
	{
		int ServerPort = MACHINE.m_pRobotOffload->m_nServerPort;
		CString ServerIp = MACHINE.m_pRobotOffload->m_strServerIp;
		if (MACHINE.m_pRobotOffload->m_Client.Connect(ServerIp,ServerPort))
		{
			BLMessageBox(_T("连接服务器成功！"));
		}
		else
		{
			BLMessageBox(_T("连接服务器失败"));
		}
	}
}


void CTestToolDlg::OnBnClickedButtonDisconnectServer()
{
	// TODO: 在此添加控件通知处理程序代码
	if (MACHINE.m_pRobotOffload != NULL)
	{
		MACHINE.m_pRobotOffload->m_Client.Disconnect();
		BLMessageBox(_T("断开成功!"));
	}
}


void CTestToolDlg::OnBnClickedButtonTestSend()
{
	// TODO: 在此添加控件通知处理程序代码
	if (MACHINE.m_pRobotOffload != NULL)
	{
		CString Cmd = _T("");
		CString Ret = _T("");
		GetDlgItemText(IDC_EDIT_TEST_CONTENT,Cmd);
		
		if (MACHINE.m_pRobotOffload->m_Client.SendToDeviceAndWait(Cmd,Ret))
		{
			MACHINE.m_strShowSend = Cmd;  //调试界面显示
			MACHINE.m_strShowRecv = Ret;
			BLMessageBox(_T("发送成功"));
		}
		else
		{
			BLMessageBox(_T("发送失败！"));
		}
	}
}


void CTestToolDlg::OnBnClickedButtonClearContent()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ShowSend.DeleteAllItems();
	m_ShowSend.InsertItem(0,_T("1"));
	m_ShowSend.InsertItem(1,_T("2"));
	m_ShowSend.InsertItem(2,_T("3"));
	m_ShowSend.InsertItem(3,_T("4"));
	m_ShowSend.InsertItem(4,_T("5"));
	m_ShowSend.InsertItem(5,_T("6"));
	m_nCount = 0;

	m_ShowRecv.DeleteAllItems();
	m_ShowRecv.InsertItem(0,_T("1"));
	m_ShowRecv.InsertItem(1,_T("2"));
	m_ShowRecv.InsertItem(2,_T("3"));
	m_ShowRecv.InsertItem(3,_T("4"));
	m_nCountEx = 0;

}


void CTestToolDlg::OnBnClickedCheckRebuildCtLog()
{
	// TODO: 在此添加控件通知处理程序代码

	int nCheck = ((CButton*)GetDlgItem(IDC_CHECK_REBUILD_CT_LOG))->GetCheck();

	if (1 == nCheck)
	{
		MACHINE.m_bRebuildCTLog = TRUE;
	}
	else
	{
		MACHINE.m_bRebuildCTLog = FALSE;
	}


}
