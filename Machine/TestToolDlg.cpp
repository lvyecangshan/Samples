// TestToolDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Machine.h"
#include "TestToolDlg.h"
#include "afxdialogex.h"
#include <windows.h>  
#include <strsafe.h>  

#define UPDATE_CONTRL_STATUS  111
// CTestToolDlg �Ի���

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
	m_ShowRecv.InsertColumn(0,_T("���"),LVCFMT_CENTER,Rct.Width()/7,0);
	m_ShowRecv.InsertColumn(1,_T("���յ�����"),LVCFMT_CENTER,Rct.Width()/2,1);
	m_ShowRecv.InsertColumn(2,_T("ʱ��"),LVCFMT_CENTER,Rct.Width()/4,2);
	m_ShowRecv.InsertItem(0,_T("1"));
	m_ShowRecv.InsertItem(1,_T("2"));
	m_ShowRecv.InsertItem(2,_T("3"));
	m_ShowRecv.InsertItem(3,_T("4"));

	m_ShowSend.GetClientRect(&Rct);
	m_ShowSend.SetExtendedStyle(m_ShowRecv.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_ShowSend.InsertColumn(0,_T("���"),LVCFMT_CENTER,Rct.Width()/7,0);
	m_ShowSend.InsertColumn(1,_T("���͵�����"),LVCFMT_CENTER,Rct.Width()/1.5,1);
	m_ShowSend.InsertColumn(2,_T("ʱ��"),LVCFMT_CENTER,Rct.Width()/4,2);
	m_ShowSend.InsertItem(0,_T("1"));
	m_ShowSend.InsertItem(1,_T("2"));
	m_ShowSend.InsertItem(2,_T("3"));
	m_ShowSend.InsertItem(3,_T("4"));
	m_ShowSend.InsertItem(4,_T("5"));
	m_ShowSend.InsertItem(5,_T("6"));


	SetTimer(UPDATE_CONTRL_STATUS, 500, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CTestToolDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

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
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
}


void CTestToolDlg::OnBnClickedBtnOpneClosePress()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strMsg;
	int nCurSel = GetCheckedRadioButton(IDC_RADIO_SHELL_STATION1, IDC_RADIO_SHELL_STATION2) - IDC_RADIO_SHELL_STATION1;
	if(0 <= nCurSel && nCurSel <= 1 && MACHINE.m_pBatteryCore[nCurSel] != NULL)
	{
		BOOL bConnect = MACHINE.m_pBatteryCore[nCurSel]->m_PressClient.IsConnect();

		CListBox *pList = (CListBox *)GetDlgItem(IDC_LIST_PRESS_VALUE);

		if(bConnect)
		{
			MACHINE.m_pBatteryCore[nCurSel]->m_PressClient.Disconnect();
			GetDlgItem(IDC_BTN_OPNE_CLOSE_PRESS)->SetWindowText(_T("����"));
			if(pList != NULL)
			{
				if(pList->GetCount() > 10)
					pList->ResetContent();
				pList->InsertString(pList->GetCount(), _T("�Ͽ�����"));
				pList->SetCurSel(pList->GetCount() -1);
			}
			strMsg.Format(_T("�û��Ͽ����%dѹ��������"), nCurSel + 1);
			WRITELOG(strMsg);
		}
		else
		{
			Sleep(1000);
			CString strIP = MACHINE.m_pBatteryCore[nCurSel]->m_strPressSenseIP;
			int nPort = MACHINE.m_pBatteryCore[nCurSel]->m_nPressSensePort;
			BOOL bRes = MACHINE.m_pBatteryCore[nCurSel]->m_PressClient.Connect(strIP, nPort);

			GetDlgItem(IDC_BTN_OPNE_CLOSE_PRESS)->SetWindowText(bRes?_T("�Ͽ�"):_T("����"));

			if(pList != NULL)
			{
				if(pList->GetCount() > 10)
					pList->ResetContent();
				pList->InsertString(pList->GetCount(), bRes?_T("���ӳɹ�"):_T("����ʧ��"));
				pList->SetCurSel(pList->GetCount() -1);
			}

			strMsg.Format(_T("�û��������%dѹ�������ӣ�����%s"), nCurSel + 1, bRes?_T("���ӳɹ�"):_T("����ʧ��"));
			WRITELOG(strMsg);
		}
		pList = NULL;
	}
}


void CTestToolDlg::OnBnClickedBtnZeroPressValue()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
				pList->InsertString(pList->GetCount(), _T("ѹ����������"));
				pList->SetCurSel(pList->GetCount() -1);
			}
		}
		pList = NULL;

		strMsg.Format(_T("�û������%dѹ����ִ����ֵ�������"), nCurSel + 1);
		WRITELOG(strMsg);
	}
}


void CTestToolDlg::OnBnClickedBtnGetPressValue()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
				pList->InsertString(pList->GetCount(), _T("ѹ����δ���ӣ����ܻ�ȡѹ������������"));
				pList->SetCurSel(pList->GetCount() -1);

				strMsg.Format(_T("�û���ȡ���%dѹ���ǵ�ǰ��ֵ����ѹ����δ���ӣ�����δ������"), nCurSel + 1);
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
					strKey.Format(_T("��%s����ȡѹ��ʧ�ܣ��������"), MACHINE.m_pBatteryCore[nCurSel]->m_strName);
					strMsg.Format(_T("�û���ȡ���%dѹ���ǵ�ǰ��ֵ����ʱ��������ֵδ���ճɹ�"), nCurSel + 1);
				}
				else
				{
					
					int nStart = strResult.Find(_T("-"));
					int nEnd = strResult.Find(_T("@"));
					strResult = strResult.Mid(nStart,nEnd - 1);
					fValue = _wtof(strResult);
					strKey.Format(_T("��ȡѹ����ֵ:%.2f"), fValue);
					strMsg.Format(_T("�û���ȡ���%dѹ���ǵ�ǰ��ֵ����ȡ��ֵ:%.2f"), nCurSel + 1, fValue);
					/*strKey.Format(_T("��ȡѹ����ֵ:%.2f"), fValue);
					strMsg.Format(_T("�û���ȡ���%dѹ���ǵ�ǰ��ֵ����ȡ��ֵ:%.2f"), nCurSel + 1, fValue);*/
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(MACHINE.m_pBatteryCore[0] != NULL)
	{
		BOOL bConnect = MACHINE.m_pBatteryCore[0]->m_PressClient.IsConnect();

		GetDlgItem(IDC_BTN_OPNE_CLOSE_PRESS)->SetWindowText(bConnect?_T("�Ͽ�"):_T("����"));

		CListBox *pList = (CListBox *)GetDlgItem(IDC_LIST_PRESS_VALUE);

		if(pList != NULL)
		{
			pList->ResetContent();
			pList->InsertString(pList->GetCount(), _T("�л�����ǹ�λ#1"));
			pList->SetCurSel(pList->GetCount() -1);
		}

		pList = NULL;
	}
}


void CTestToolDlg::OnBnClickedRadioShellStation2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(MACHINE.m_pBatteryCore[1] != NULL)
	{
		BOOL bConnect = MACHINE.m_pBatteryCore[1]->m_PressClient.IsConnect();

		GetDlgItem(IDC_BTN_OPNE_CLOSE_PRESS)->SetWindowText(bConnect?_T("�Ͽ�"):_T("����"));

		CListBox *pList = (CListBox *)GetDlgItem(IDC_LIST_PRESS_VALUE);

		if(pList != NULL)
		{
			pList->ResetContent();
			pList->InsertString(pList->GetCount(), _T("�л�����ǹ�λ#2"));
			pList->SetCurSel(pList->GetCount() -1);
		}

		pList = NULL;
	}
}


void CTestToolDlg::OnBnClickedBtnOpneCloseRobot()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(MACHINE.m_pRobotOffload != NULL)
	{
		BOOL bConnect = MACHINE.m_pRobotOffload->m_pRobotClient.IsConnect();

		if(bConnect)
		{
			MACHINE.m_pRobotOffload->m_pRobotClient.Disconnect();
			MACHINE.m_pRobotOffload->DisconnectRobot();
			GetDlgItem(IDC_BTN_OPNE_CLOSE_ROBOT)->SetWindowText(_T("����"));

			if(m_RobotResponseList.GetCount() > 5)
				m_RobotResponseList.ResetContent();
			m_RobotResponseList.InsertString(m_RobotResponseList.GetCount(), _T("�Ͽ�����"));
			m_RobotResponseList.SetCurSel(m_RobotResponseList.GetCount()-1);

			WRITELOG(_T("�û����л����˶Ͽ����Ӳ���"));
		}
		else
		{
			CString strIP = MACHINE.m_pRobotOffload->m_strRobotIp;
			int nPort = MACHINE.m_pRobotOffload->m_nRobotPort;
			BOOL bRes = MACHINE.m_pRobotOffload->ConnectRobot();
			if (!bRes)
			{
				BLMessageBox(_T("��⵽�����˵�ֹͣ�������ź�δ����"),MB_ICONERROR|MB_ICONWARNING);
				return ;
			}
			bRes = MACHINE.m_pRobotOffload->m_pRobotClient.Connect(strIP, nPort);

			GetDlgItem(IDC_LIST_ROBOT_RESPONSE_LIST)->SetWindowText(bRes?_T("�Ͽ�"):_T("����"));

			if(m_RobotResponseList.GetCount() > 5)
				m_RobotResponseList.ResetContent();
			m_RobotResponseList.InsertString(m_RobotResponseList.GetCount(), bRes?_T("���ӳɹ�"):_T("����ʧ��"));
			m_RobotResponseList.SetCurSel(m_RobotResponseList.GetCount()-1);

			WRITELOG(bRes?_T("�û����л��������Ӳ��������ӳɹ�"):_T("�û����л��������Ӳ���������ʧ��"));

// 			if(bRes)
// 			{
// 				bRes = MACHINE.m_pRobotOffload->m_pRobotClient.SendToDeviceAndWait(ROBOT_LOW_SPEED);
// 				
// 				if(!bRes)
// 					BLMessageBox(_T("���Ļ������ٶ�Ϊ����ʧ�ܣ���ע�ⰲȫ"), MB_ICONERROR);
// 				WRITELOG(bRes?_T("���Ļ������ٶ�Ϊ���ٳɹ�"):_T("���Ļ������ٶ�Ϊ����ʧ��"));
// 			}
		}
	}
}


void CTestToolDlg::OnBnClickedBtnSendRobotCmd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
			m_RobotResponseList.InsertString(m_RobotResponseList.GetCount(), _T("������δ���ӣ��޷����������������"));
			m_RobotResponseList.SetCurSel(m_RobotResponseList.GetCount()-1);

			strMsg.Format(_T("�û����������ƶ�������:����%s�����������δ���ӣ�������δ������"), g_robotcmd[nCurSel].strRemark);
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
					// ��ȡ�ϴ�ָ��뵱ǰָ��Ƚϣ�����ǰָ��=�ϴ�ָ��+1�����ҵ�ǰָ���ǻ������½�����������ȷ
					// ��ǰָ������˲����½�����������ȷ
					nCmdLevel = g_robotcmd[nCurSel].nCmdLevel;
					if((nCmdLevel == 1 && nCurSel == MACHINE.m_pRobotOffload->m_nCurRobotLoc + 1) || nCmdLevel == 0)
					{
						bRes = MACHINE.m_pRobotOffload->m_pRobotClient.SendToDeviceAndWait(nCurSel);
						MACHINE.m_pRobotOffload->m_nCurRobotLoc = nCurSel;
					}
					else
					{
						strMsg.Format(_T("��ǰ�����˴��ڡ�%s��������ֱ���Ƶ���%s�������Ƚ��������Ƶ���%s��")
							, g_robotcmd[MACHINE.m_pRobotOffload->m_nCurRobotLoc].strRemark, g_robotcmd[nCurSel].strRemark
							, g_robotcmd[nCurSel -1].strRemark);
						BLMessageBox(strMsg, MB_ICONWARNING);

						strMsg.Format(_T("�û����������ƶ������˵���%s��,�������˵�ǰλ���ڡ�%s��,���ƶ������ȫ��ϵͳ����ִ��")
							, g_robotcmd[nCurSel].strRemark, g_robotcmd[MACHINE.m_pRobotOffload->m_nCurRobotLoc].strRemark);
						WRITELOG(strMsg);
						return;
					}
				}
				else
				{
					// ��ȡ�ϴ�ָ��뵱ǰָ��Ƚϣ�����ǰָ��=�ϴ�ָ��-1�����ҵ�ǰָ���ǻ������½�����������ȷ
					if(nCurSel == MACHINE.m_pRobotOffload->m_nCurRobotLoc - 1)
					{
						bRes = MACHINE.m_pRobotOffload->m_pRobotClient.SendToDeviceAndWait(nCurSel);
						MACHINE.m_pRobotOffload->m_nCurRobotLoc = nCurSel;
					}
					else
					{
						strMsg.Format(_T("��ǰ�����˴��ڡ�%s��������ֱ���Ƶ���%s�������Ƚ��������Ƶ���%s��")
							, g_robotcmd[MACHINE.m_pRobotOffload->m_nCurRobotLoc].strRemark, g_robotcmd[nCurSel].strRemark
							, g_robotcmd[MACHINE.m_pRobotOffload->m_nCurRobotLoc -1].strRemark);
						BLMessageBox(strMsg, MB_ICONWARNING);

						strMsg.Format(_T("�û����������ƶ������˵���%s��,�������˵�ǰλ���ڡ�%s��,���ƶ������ȫ��ϵͳ����ִ��")
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

				strMsg.Format(_T("�������ƶ�����%s���ɹ�"), g_robotcmd[nCurSel].strRemark);
				m_RobotResponseList.InsertString(m_RobotResponseList.GetCount(), strMsg);
				m_RobotResponseList.SetCurSel(m_RobotResponseList.GetCount()-1);

				strMsg.Format(_T("�û����������ƶ������˵���%s��,����ִ�гɹ�") , g_robotcmd[nCurSel].strRemark);
				WRITELOG(strMsg);
			}
			else
			{
				if(m_RobotResponseList.GetCount() > 10)
					m_RobotResponseList.ResetContent();
				m_RobotResponseList.InsertString(m_RobotResponseList.GetCount(), _T("���͸ı��ƶ���������˷�����ʱ"));
				m_RobotResponseList.SetCurSel(m_RobotResponseList.GetCount()-1);

				strMsg.Format(_T("�û����������ƶ������˵���%s��,����ִ�з�����ʱ") , g_robotcmd[nCurSel].strRemark);
				WRITELOG(strMsg);
			}
		}
	}
}


void CTestToolDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
			GetDlgItem(IDC_BTN_OPNE_CLOSE_PRESS)->SetWindowText(bConnect?_T("�Ͽ�"):_T("����"));
		}
		if(MACHINE.m_pRobotOffload != NULL)
		{
			bConnect = MACHINE.m_pRobotOffload->m_pRobotClient.IsConnect();
			GetDlgItem(IDC_BTN_OPNE_CLOSE_ROBOT)->SetWindowText(bConnect?_T("�Ͽ�"):_T("����"));

// 			unsigned int unSpeed = MACHINE.m_pRobotOffload->m_unRobotSpeed;
// 			ASSERT(0 <= unSpeed && unSpeed <= 2);
// 			CheckRadioButton(IDC_RADIO_HIGH_SPEED, IDC_RADIO_SLOW_SPEED, IDC_RADIO_HIGH_SPEED + unSpeed);

			CString strMsg;
			if(ROBOT_GO_HOME <= MACHINE.m_pRobotOffload->m_nCurRobotLoc &&  MACHINE.m_pRobotOffload->m_nCurRobotLoc <= ROBOT_OFFLOAD_NG_POS)
			{
				strMsg.Format(_T("%s��%s"), g_robotcmd[MACHINE.m_pRobotOffload->m_nCurRobotLoc].strRemark, g_robotcmd[MACHINE.m_pRobotOffload->m_unRobotSpeed].strRemark);
				GetDlgItem(IDC_STATIC_CUR_ROBOT_STATUS)->SetWindowText(strMsg);
			}
		}

		if(MACHINE.m_pOnloadCore != NULL)
		{
			bConnect = MACHINE.m_pOnloadCore->m_ScanCodeClient.IsConnect();
			GetDlgItem(IDC_BTN_OPEN_CLOSE_BARCODE)->SetWindowText(bConnect?_T("�Ͽ�"):_T("����"));
		}

		if (MACHINE.m_strShowSend.GetLength() > 0)  //������ʾ chen_6.13
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

		if (MACHINE.m_strShowRecv.GetLength() > 0)  //������ʾ chen_6.13
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
			m_RobotResponseList.InsertString(m_RobotResponseList.GetCount(), _T("������δ���ӣ��޷����������������"));
			m_RobotResponseList.SetCurSel(m_RobotResponseList.GetCount()-1);

			WRITELOG(_T("�û����Ļ����������ٶȣ��������δ���ӣ�������δ������"));
		}
		else
		{
			BOOL bRes = MACHINE.m_pRobotOffload->m_pRobotClient.SendToDeviceAndWait(nCurSpeedSel);

			if(bRes)
			{
 				if(m_RobotResponseList.GetCount() > 10)
 					m_RobotResponseList.ResetContent();

				MACHINE.m_pRobotOffload->m_unRobotSpeed = nCurSpeedSel;
				strMsg.Format(_T("�ı�������ٶȳɹ�����ǰ�ٶȣ�%s"), g_robotcmd[nCurSpeedSel].strRemark);
				m_RobotResponseList.InsertString(m_RobotResponseList.GetCount(), strMsg);
				m_RobotResponseList.SetCurSel(m_RobotResponseList.GetCount()-1);

				WRITELOG(strMsg);
			}
			else
			{
				if(m_RobotResponseList.GetCount() > 10)
					m_RobotResponseList.ResetContent();

				m_RobotResponseList.InsertString(m_RobotResponseList.GetCount(), _T("���͸ı��ٶ���������˷�����ʱ"));
				m_RobotResponseList.SetCurSel(m_RobotResponseList.GetCount()-1);

				strMsg.Format(_T("���͸ı��ٶ�����(%s)�������˳�ʱ����"), g_robotcmd[nCurSpeedSel].strRemark);
				WRITELOG(strMsg);
			}
		}
	}
}


void CTestToolDlg::OnBnClickedRadioHighSpeed()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	AlterRobotSpeed();
}


void CTestToolDlg::OnBnClickedRadioMiddleSpeed()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	AlterRobotSpeed();
}


void CTestToolDlg::OnBnClickedRadioSlowSpeed()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	AlterRobotSpeed();
}


void CTestToolDlg::OnBnClickedBtnOpenCloseBarcode()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strMsg;
	int nCurSel = GetCheckedRadioButton(IDC_READ_ONE, IDC_READ_TWO) - IDC_READ_ONE;
	if(MACHINE.m_pReadCode[nCurSel] != NULL)
	{

		BOOL bConnect = MACHINE.m_pReadCode[nCurSel]->m_pReadCode.IsConnect();

		if(bConnect)
		{
			MACHINE.m_pReadCode[nCurSel]->m_pReadCode.Disconnect();
			GetDlgItem(IDC_BTN_OPEN_CLOSE_BARCODE)->SetWindowText(_T("����"));

			if(m_ScanCodeList.GetCount() > 5)
				m_ScanCodeList.ResetContent();
			m_ScanCodeList.InsertString(m_ScanCodeList.GetCount(), _T("�Ͽ�����"));
			m_ScanCodeList.SetCurSel(m_ScanCodeList.GetCount()-1);
			WRITELOG(_T("�û��Ͽ�ɨ��ǹ����"));
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

			GetDlgItem(IDC_BTN_OPEN_CLOSE_BARCODE)->SetWindowText(bRes?_T("�Ͽ�"):_T("����"));

			if(m_ScanCodeList.GetCount() > 5)
				m_ScanCodeList.ResetContent();
			if(bRes)
			{
				m_ScanCodeList.InsertString(m_ScanCodeList.GetCount(), _T("���ӳɹ�"));
			}
			else
			{
				m_ScanCodeList.InsertString(m_ScanCodeList.GetCount(),_T("����ʧ��,���鴮�ڷ����������Ƿ���ȷ��"));
			}
			
			m_ScanCodeList.SetCurSel(m_ScanCodeList.GetCount()-1);

			strMsg.Format(_T("�û�����ɨ��ǹ��%s"), bRes?_T("���ӳɹ�"):_T("����ʧ��"));
			WRITELOG(strMsg);
		}
	}
}


void CTestToolDlg::OnBnClickedBtnScanBarcode()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nCurSel = GetCheckedRadioButton(IDC_READ_ONE, IDC_READ_TWO) - IDC_READ_ONE;
	if(MACHINE.m_pReadCode[nCurSel] != NULL)
	{
		BOOL bConnect = MACHINE.m_pReadCode[nCurSel]->m_pReadCode.IsConnect();

		if(!bConnect)
		{
			if(m_ScanCodeList.GetCount() > 5)
				m_ScanCodeList.ResetContent();
			m_ScanCodeList.InsertString(m_ScanCodeList.GetCount(), _T("ɨ��ǹδ���ӣ���������"));
			m_ScanCodeList.SetCurSel(m_ScanCodeList.GetCount()-1);

			WRITELOG(_T("�û�ִ��ɨ���������ɨ��ǹδ���ӣ�����δ������"));
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
					strMsg.Format(_T("ɨ��ʧ�ܣ������Ƿ�������룡"), strBarCode);
					m_ScanCodeList.InsertString(m_ScanCodeList.GetCount(), strMsg);
					strMsg = _T("ɨ��ʧ�ܣ�δ��ȡ�����룡");				
				}
				else
				{
					m_ScanCodeList.InsertString(m_ScanCodeList.GetCount(), _T("ɨ��ʧ�ܣ�����ɨ��ǹ�Ƿ�������"));
					strMsg = _T("�û�ִ��ɨ�������������ʱ");
				}
			}
			else
			{
				strMsg.Format(_T("ɨ��ɹ�, %s"), strBarCode);
				m_ScanCodeList.InsertString(m_ScanCodeList.GetCount(), strMsg);
				strMsg.Format(_T("�û�ִ��ɨ�����, ��ȡ����%s"), strBarCode);
			}
			WRITELOG(strMsg);
			m_ScanCodeList.SetCurSel(m_ScanCodeList.GetCount() - 1);
		}
	}
}


BOOL CTestToolDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->wParam == VK_SPACE || pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CTestToolDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(MACHINE.m_pOnloadCore != NULL)
	{
		MACHINE.m_pOnloadCore->m_bEnableDocking = TRUE;
	}
}
	

void CTestToolDlg::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(MACHINE.m_pOnloadShellPnp != NULL)
		MACHINE.m_pOnloadShellPnp->m_bEnableDocking = TRUE;
}


void CTestToolDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	MACHINE.m_pBatteryOffloadPnp[0]->m_bOffload = TRUE;
	MACHINE.m_pBatteryOffloadPnp[1]->m_bOffload = TRUE;
}


void CTestToolDlg::OnBnClickedButtonServerConnect()
{
	 //TODO: �ڴ���ӿؼ�֪ͨ����������
	if (MACHINE.m_pRobotOffload !=NULL && !MACHINE.m_pRobotOffload->m_Client.IsConnect()) //��ֹ�ظ�����
	{
		int ServerPort = MACHINE.m_pRobotOffload->m_nServerPort;
		CString ServerIp = MACHINE.m_pRobotOffload->m_strServerIp;
		if (MACHINE.m_pRobotOffload->m_Client.Connect(ServerIp,ServerPort))
		{
			BLMessageBox(_T("���ӷ������ɹ���"));
		}
		else
		{
			BLMessageBox(_T("���ӷ�����ʧ��"));
		}
	}
}


void CTestToolDlg::OnBnClickedButtonDisconnectServer()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (MACHINE.m_pRobotOffload != NULL)
	{
		MACHINE.m_pRobotOffload->m_Client.Disconnect();
		BLMessageBox(_T("�Ͽ��ɹ�!"));
	}
}


void CTestToolDlg::OnBnClickedButtonTestSend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (MACHINE.m_pRobotOffload != NULL)
	{
		CString Cmd = _T("");
		CString Ret = _T("");
		GetDlgItemText(IDC_EDIT_TEST_CONTENT,Cmd);
		
		if (MACHINE.m_pRobotOffload->m_Client.SendToDeviceAndWait(Cmd,Ret))
		{
			MACHINE.m_strShowSend = Cmd;  //���Խ�����ʾ
			MACHINE.m_strShowRecv = Ret;
			BLMessageBox(_T("���ͳɹ�"));
		}
		else
		{
			BLMessageBox(_T("����ʧ�ܣ�"));
		}
	}
}


void CTestToolDlg::OnBnClickedButtonClearContent()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������

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
