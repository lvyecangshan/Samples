// MesSetDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Machine.h"
#include "MesSetDlg.h"
#include "afxdialogex.h"


// CMesSetDlg �Ի���
#define  UPDATE_MES_DISPLAY  1000
#define  UPDATE_MES_DATA  2000

IMPLEMENT_DYNAMIC(CMesSetDlg, CDialogEx)

	CMesSetDlg::CMesSetDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMesSetDlg::IDD, pParent)

{

}

CMesSetDlg::~CMesSetDlg()
{
}

void CMesSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST1, m_listMesData);
}

BEGIN_MESSAGE_MAP(CMesSetDlg, CDialogEx)
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_MES_LOGIN, &CMesSetDlg::OnBnClickedBtnMesLogin)
	ON_BN_CLICKED(IDC_BTN_SAVE_CONFIG, &CMesSetDlg::OnBnClickedBtnSaveConfig)
	ON_BN_CLICKED(IDC_BUT_CORE_INBOUND, &CMesSetDlg::OnBnClickedButCoreInbound)
	ON_BN_CLICKED(IDC_BUT_CORE_OUTBOUND, &CMesSetDlg::OnBnClickedButCoreOutbound)
END_MESSAGE_MAP()

BOOL CMesSetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	initGrid();//��ʼ��Ϊ���ģʽ
	InitGridSetting();//��ʼ�������趨ֵ������
	LoadConfig();//����ini�������ļ�
	AdjustLayout();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}





BOOL CMesSetDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->wParam == VK_SPACE || pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}



void CMesSetDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	SCROLLINFO Scrinfo;
	GetScrollInfo(SB_VERT,&Scrinfo,SIF_ALL);
	switch(nSBCode)
	{
	case SB_BOTTOM:
		{
			ScrollWindow(0,(Scrinfo.nPos-Scrinfo.nMax)*20);
			Scrinfo.nPos = Scrinfo.nMax;
			SetScrollInfo(SB_VERT,&Scrinfo,SIF_ALL);
			break;
		}
	case SB_TOP:
		{
			ScrollWindow(0,(Scrinfo.nPos-Scrinfo.nMin)*20);
			Scrinfo.nPos = Scrinfo.nMin;
			SetScrollInfo(SB_VERT,&Scrinfo,SIF_ALL);
			break;
		}
	case SB_LINEUP:
		{
			Scrinfo.nPos -= 2;
			if(Scrinfo.nPos < Scrinfo.nMin)
			{
				Scrinfo.nPos = Scrinfo.nMin;
				break;
			}
			SetScrollInfo(SB_VERT,&Scrinfo,SIF_ALL);
			ScrollWindow(0,20);
			break;
		}
	case SB_LINEDOWN:
		{
			Scrinfo.nPos += 2;
			if(Scrinfo.nPos > Scrinfo.nMax)
			{
				Scrinfo.nPos = Scrinfo.nMax;
				break;
			}
			SetScrollInfo(SB_VERT,&Scrinfo,SIF_ALL);
			ScrollWindow(0,-20);
			break;
		}
	case SB_PAGEUP:
		{
			Scrinfo.nPos -= 10;
			if(Scrinfo.nPos < Scrinfo.nMin)
			{
				Scrinfo.nPos = Scrinfo.nMin;
				break;
			}
			SetScrollInfo(SB_VERT,&Scrinfo,SIF_ALL);
			ScrollWindow(0,20*10);
			break;
		}
	case SB_PAGEDOWN:
		{
			Scrinfo.nPos += 10;
			if(Scrinfo.nPos > Scrinfo.nMax)
			{
				Scrinfo.nPos = Scrinfo.nMax;
				break;
			}
			SetScrollInfo(SB_VERT,&Scrinfo,SIF_ALL);
			ScrollWindow(0,-20*10);
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
			ScrollWindow(0,(Scrinfo.nPos - nPos)*5);
			Scrinfo.nPos = nPos;
			SetScrollInfo(SB_VERT,&Scrinfo,SIF_ALL);
			break;
		}
	default:
		break;
	}
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CMesSetDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	SCROLLINFO Scrinfo;
	GetScrollInfo(SB_HORZ,&Scrinfo,SIF_ALL);
	switch(nSBCode)
	{
	case SB_BOTTOM:
		{
			ScrollWindow((Scrinfo.nPos-Scrinfo.nMax)*20,0);
			Scrinfo.nPos = Scrinfo.nMax;
			SetScrollInfo(SB_HORZ,&Scrinfo,SIF_ALL);
			break;
		}
	case SB_TOP:
		{
			ScrollWindow((Scrinfo.nPos-Scrinfo.nMin)*20,0);
			Scrinfo.nPos = Scrinfo.nMin;
			SetScrollInfo(SB_HORZ,&Scrinfo,SIF_ALL);
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
			SetScrollInfo(SB_HORZ,&Scrinfo,SIF_ALL);
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
			SetScrollInfo(SB_HORZ,&Scrinfo,SIF_ALL);
			ScrollWindow(-10,0);
			break;
		}
	case SB_PAGEUP:
		{
			Scrinfo.nPos -= 10;
			if(Scrinfo.nPos < Scrinfo.nMin)
			{
				Scrinfo.nPos = Scrinfo.nMin;
				break;
			}
			SetScrollInfo(SB_HORZ,&Scrinfo,SIF_ALL);
			ScrollWindow(20*10,0);
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
			SetScrollInfo(SB_HORZ,&Scrinfo,SIF_ALL);
			ScrollWindow(-20*10,0);
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
			ScrollWindow((Scrinfo.nPos - nPos)*15,0);
			Scrinfo.nPos = nPos;
			SetScrollInfo(SB_HORZ,&Scrinfo,SIF_ALL);
			break;
		}
	default:
		break;
	}
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}







void CMesSetDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	AdjustLayout();
}

void CMesSetDlg::initGrid()//ListCtrl��ʼ��Ϊ���ģʽ
{
	//Style
	DWORD dwStyle = ::GetWindowLong(m_listMesData.GetSafeHwnd(), GWL_STYLE); 
	dwStyle |= LVS_SINGLESEL;       //ֻ�ɵ���ѡ��
	dwStyle |= LVS_SHOWSELALWAYS;   //Always show selection
	::SetWindowLong(m_listMesData.GetSafeHwnd(), GWL_STYLE, dwStyle);

	// Extended Style
	DWORD dwStyleEx = m_listMesData.GetExtendedStyle();
	dwStyleEx |= LVS_EX_GRIDLINES;        //������
	dwStyleEx |= LVS_EX_FULLROWSELECT;    //���и���
	//dwStyleEx |= LVS_EX_CHECKBOXES;       //Itemǰ����check box
	m_listMesData.SetExtendedStyle(dwStyleEx);

	// ������
	m_listMesData.InsertColumn(0, TEXT("����"), LVCFMT_LEFT, 200);
	m_listMesData.InsertColumn(1, TEXT("ֵ"), LVCFMT_LEFT, 100);
	m_listMesData.SetColumnCtrlType(1, CCT_EDITBOX);
}
void CMesSetDlg::AddLine(CString strname)
{
	// ������
	int nItem = 0;
	nItem = m_listMesData.InsertItem(m_listMesData.GetItemCount(), strname, 0);
	m_listMesData.SetItemText(nItem, 1, TEXT(""));   //�����1��ʼ,0��������
}
void CMesSetDlg::InitGridSetting()//��ʼ���������
{
	m_listMesData.DeleteAllItems();//��ձ��

	AddLine(L"��о����");
	AddLine(L"����ٶ��趨ֵ");
	AddLine(L"���ѹ���趨ֵ");
	


}
void CMesSetDlg::SaveConfig()//���������ļ�
{
	int count=m_listMesData.GetItemCount();
	CString strRow;
	for(int i=0;i<count;i++)
	{
		strRow.Format(L"row%d_col%d",i,1);
		IniWriteString(L"MESSetting",strRow,m_listMesData.getCell(i,1),GetAbsolutePathName(PARAMETER_CFG));
	}
	int m=0;
	MACHINE.m_ValueSetting.strBatteryType = m_listMesData.getCell(m++,1);//�������
	MACHINE.m_ValueSetting.strSpeed = m_listMesData.getCell(m++,1);//���������¶�
	MACHINE.m_ValueSetting.strPressure = m_listMesData.getCell(m++,1);//��������ʱ��
	
	CString strAddress;
	GetDlgItem(IDC_EDIT_OPC_ADDRESS)->GetWindowTextW(strAddress);
	IniWriteString(L"MESSetting",L"strAddress",strAddress,GetAbsolutePathName(PARAMETER_CFG));
}
void CMesSetDlg::LoadConfig()//���������ļ�
{
	int count=m_listMesData.GetItemCount();
	CString strRow;
	for(int i=0;i<count;i++)
	{
		strRow.Format(L"row%d_col%d",i,1);
		m_listMesData.setCell(i,1,IniReadString(L"MESSetting",strRow,L"",GetAbsolutePathName(PARAMETER_CFG)));
	}
	int m=0;
	MACHINE.m_ValueSetting.strBatteryType = m_listMesData.getCell(m++,1);//�������
	MACHINE.m_ValueSetting.strSpeed = m_listMesData.getCell(m++,1);//���������¶�
	MACHINE.m_ValueSetting.strPressure = m_listMesData.getCell(m++,1);//��������ʱ��
	MACHINE.m_strAddress = IniReadString(L"MESSetting",L"strAddress",L"opc.tcp://10.210.88.172:4862",GetAbsolutePathName(PARAMETER_CFG));
	GetDlgItem(IDC_EDIT_OPC_ADDRESS)->SetWindowText(MACHINE.m_strAddress);
}
void CMesSetDlg::AdjustLayout()
{
	CRect rcView;
	GetClientRect(rcView);
	if (m_listMesData.GetSafeHwnd() != NULL)
	{
		m_listMesData.MoveWindow(rcView.left+20,rcView.top+60,rcView.left+320,rcView.bottom);
	}
}


void CMesSetDlg::OnBnClickedBtnMesLogin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//int strMachineID = IniReadInt(L"MESSetting",L"Machine_ID",0,GetAbsolutePathName(PARAMETER_CFG));
	MACHINE.m_objMes.X_ConnectServer(MACHINE.m_strAddress);
	if (1 == MACHINE.m_objMes.X_isConnected())
	{
		if (1 == MACHINE.m_objMes.X_IsMESOnLine())
		{
			if (1 == MACHINE.m_objMes.X_MesLogin(TRUE))
			{
				//MACHINE.m_objMes.SetAddress(strMachineID);
				BLMessageBox(_T("MES��¼�ɹ�"));
				OnBnClickedBtnSaveConfig();
				return;
			}
		}
		else
		{
			BLMessageBox(_T("OPC���������ӳɹ���������MES��̨�����ߣ���¼ʧ�ܣ�����MES����ʦ����"));
			return;
		}
	}
	BLMessageBox(_T("MES��¼ʧ��"));
}


void CMesSetDlg::OnBnClickedBtnSaveConfig()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SaveConfig();

	if (1 == MACHINE.m_objMes.X_UpdateSetting(MACHINE.m_ValueSetting,TRUE))
	{
		if (1 == MACHINE.m_objMes.X_DownloadParameter(MACHINE.m_ValueRange))
		{
			if (1 == MACHINE.m_objMes.X_CompareParameter(MACHINE.m_ValueSetting,MACHINE.m_ValueRange))
			{
				BLMessageBox(_T("�����ϴ��ɹ�"));
				return;
			}
			else
			{
				if (MACHINE.m_objMes.strNG != _T(""))
				{
					BLMessageBox(MACHINE.m_objMes.strNG);
					return;
				}
			}
		}
	}
	BLMessageBox(_T("�����ϴ�ʧ�ܣ����������Ƿ�ͨ��"));
}


void CMesSetDlg::OnBnClickedButCoreInbound()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (1 != MACHINE.m_objMes.X_isConnected())
	{
		BLMessageBox(_T("���ȵ�¼MES"));
		return;
	}
	if (1 != MACHINE.m_objMes.X_IsMESOnLine())
	{
		BLMessageBox(_T("������MES��̨�����ߣ�����MES����ʦ����"));
		return;
	}
	CString strCoreCode = _T("");
	CString strResult = _T("");
	GetDlgItem(IDC_EDIT_BAR_CODE)->GetWindowText(strCoreCode);
	if (MACHINE.m_objMes.X_UpdateBarCode(strCoreCode,strResult))
	{
		if (strResult == _T("1"))
		{
			BLMessageBox(_T("��վ�ɹ�"));
			return;
		}
	}
	BLMessageBox(_T("��վʧ��"));
}


void CMesSetDlg::OnBnClickedButCoreOutbound()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (1 != MACHINE.m_objMes.X_isConnected())
	{
		BLMessageBox(_T("���ȵ�¼MES"));
		return;
	}
	if (1 != MACHINE.m_objMes.X_IsMESOnLine())
	{
		BLMessageBox(_T("������MES��̨�����ߣ�����MES����ʦ����"));
		return;
	}
	CString strCoreCode = _T("");
	GetDlgItem(IDC_EDIT_BAR_CODE)->GetWindowText(strCoreCode);
	X_BatteryData BatteryData;
	BatteryData.strBatteryCode = strCoreCode;

	SYSTEMTIME currentTime;
	GetLocalTime(&currentTime);
	CString Time = _T("");
	Time.Format(_T("%4d-%02d-%02d-%02d:%02d:%02d"),currentTime.wYear, currentTime.wMonth, currentTime.wDay, currentTime.wHour, currentTime.wMinute, currentTime.wSecond);    
	BatteryData.strTime = Time;

	BatteryData.strSpeed = MACHINE.m_ValueSetting.strSpeed;
	BatteryData.strPressure = MACHINE.m_ValueSetting.strPressure;

	if (MACHINE.m_objMes.X_UpdateData(BatteryData,MACHINE.m_ValueRange))
	{
		BLMessageBox(_T("��վ�ɹ�"));
		return;
	}
	BLMessageBox(_T("��վʧ��"));
}
