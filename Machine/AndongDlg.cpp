// AndongDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Machine.h"
#include "AndongDlg.h"
#include "afxdialogex.h"


// CAndongDlg �Ի���

IMPLEMENT_DYNAMIC(CAndongDlg, CDialogEx)

CAndongDlg::CAndongDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAndongDlg::IDD, pParent)
{
	m_strAlarmMes.Empty();
	VERIFY(m_Font.CreateFont(25, 8, 0, 0,  FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("΢���ź�")));
}

CAndongDlg::~CAndongDlg()
{
}

void CAndongDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ALARM_INFO, m_strAlarmMes);
}


BEGIN_MESSAGE_MAP(CAndongDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_NO_PLAN, &CAndongDlg::OnBnClickedBtnNoPlan)
	ON_BN_CLICKED(IDC_BTN_TRAIN_ASSESS, &CAndongDlg::OnBnClickedBtnTrainAssess)
	ON_BN_CLICKED(IDC_BTN_INVENTORY, &CAndongDlg::OnBnClickedBtnInventory)
	ON_BN_CLICKED(IDC_BTN_OTHER_REASON, &CAndongDlg::OnBnClickedBtnOtherReason)
	ON_BN_CLICKED(IDC_BTN_MAINTENANCE_REPAIR, &CAndongDlg::OnBnClickedBtnMaintenanceRepair)
	ON_BN_CLICKED(IDC_BTN_CHANGE_TYPE, &CAndongDlg::OnBnClickedBtnChangeType)
	ON_BN_CLICKED(IDC_BTN_CHANGE_SHIFT, &CAndongDlg::OnBnClickedBtnChangeShift)
	ON_BN_CLICKED(IDC_BTN_DINE_TIME, &CAndongDlg::OnBnClickedBtnDineTime)
	ON_BN_CLICKED(IDC_BTN_MACHINE_JAM, &CAndongDlg::OnBnClickedBtnMachineJam)
	ON_BN_CLICKED(IDC_BTN_CLEAN_AND_CHECK, &CAndongDlg::OnBnClickedBtnCleanAndCheck)
	ON_BN_CLICKED(IDC_BTN_MACHINE_MINOR_FAULT, &CAndongDlg::OnBnClickedBtnMachineMinorFault)
	ON_BN_CLICKED(IDC_BTN_QUALITY_MONITORING, &CAndongDlg::OnBnClickedBtnQualityMonitoring)
	ON_BN_CLICKED(IDC_BTN_MATERIAL_REPLACE, &CAndongDlg::OnBnClickedBtnMaterialReplace)
	ON_BN_CLICKED(IDC_BTN_SAMPLE_MAKING, &CAndongDlg::OnBnClickedBtnSampleMaking)
	ON_BN_CLICKED(IDC_BTN_WAIT_MATERIAL, &CAndongDlg::OnBnClickedBtnWaitMaterial)
	ON_BN_CLICKED(IDC_BTN_ABNORMAL_QUALITY, &CAndongDlg::OnBnClickedBtnAbnormalQuality)
	ON_BN_CLICKED(IDC_BTN_ABNORMAL_MATERIAL, &CAndongDlg::OnBnClickedBtnAbnormalMaterial)
	ON_BN_CLICKED(IDC_BTN_ABNORMAL_IT_ETHERNET, &CAndongDlg::OnBnClickedBtnAbnormalItEthernet)
	ON_BN_CLICKED(IDC_BTN_ABNORMAL_PE, &CAndongDlg::OnBnClickedBtnAbnormalPe)
END_MESSAGE_MAP()


// CAndongDlg ��Ϣ�������


BOOL CAndongDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CWnd* pWnd = GetWindow(GW_CHILD);
	while(pWnd != NULL)
	{
		GetDlgItem(pWnd->GetDlgCtrlID())->SetFont(&m_Font);
		pWnd = pWnd->GetWindow(GW_HWNDNEXT);
	}
	m_strAlarmMes = MACHINE.m_strAlarmMes = _T("����");
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CAndongDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	m_Font.DeleteObject();
	m_brush.DeleteObject();
	// TODO: �ڴ˴������Ϣ����������
}


HBRUSH CAndongDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	// ����ɫ222  210  241

	// TODO:  �ڴ˸��� DC ���κ�����

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	m_brush.DeleteObject();
	pDC->SetBkMode(TRANSPARENT);

	if(nCtlColor == CTLCOLOR_DLG)
	{
		m_brush.CreateSolidBrush(RGB(222, 210, 241));
		return m_brush;
	}
	else if(pWnd->GetDlgCtrlID() == IDC_EDIT_ALARM_INFO)
	{
		m_brush.CreateSolidBrush(RGB(0, 0, 200));
		return m_brush;
	}
	return hbr;
}


void CAndongDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()

	CRect rc;
	GetDlgItem(IDC_STATIC_1)->GetWindowRect(rc);
	ScreenToClient(rc);
	dc.FillSolidRect(rc, RGB(233, 252, 203));

	GetDlgItem(IDC_STATIC_2)->GetWindowRect(rc);
	ScreenToClient(rc);
	dc.FillSolidRect(rc, RGB(49, 113, 201));

	GetDlgItem(IDC_STATIC_3)->GetWindowRect(rc);
	ScreenToClient(rc);
	dc.FillSolidRect(rc, RGB(57, 170, 210));

	GetDlgItem(IDC_STATIC_4)->GetWindowRect(rc);
	ScreenToClient(rc);
	dc.FillSolidRect(rc, RGB(255, 139, 40));

	GetDlgItem(IDC_STATIC_5)->GetWindowRect(rc);
	ScreenToClient(rc);
	dc.FillSolidRect(rc, RGB(194, 242, 255));
}


BOOL CAndongDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->wParam == VK_SPACE || pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CAndongDlg::OnBnClickedBtnNoPlan()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	MACHINE.AndonMachineStatus(MACHINE.m_strAndonResource,20);
	OnCancel();
}


void CAndongDlg::OnBnClickedBtnTrainAssess()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	MACHINE.AndonMachineStatus(MACHINE.m_strAndonResource,44);
	OnCancel();
}


void CAndongDlg::OnBnClickedBtnInventory()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	MACHINE.AndonMachineStatus(MACHINE.m_strAndonResource,41);
	OnCancel();
}


void CAndongDlg::OnBnClickedBtnOtherReason()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	MACHINE.AndonMachineStatus(MACHINE.m_strAndonResource,54);
	OnCancel();
}


void CAndongDlg::OnBnClickedBtnMaintenanceRepair()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	MACHINE.AndonMachineStatus(MACHINE.m_strAndonResource,12);
	OnCancel();
}


void CAndongDlg::OnBnClickedBtnChangeType()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	MACHINE.AndonMachineStatus(MACHINE.m_strAndonResource,21);
	OnCancel();
}


void CAndongDlg::OnBnClickedBtnChangeShift()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	MACHINE.AndonMachineStatus(MACHINE.m_strResource,42);
	OnCancel();
}


void CAndongDlg::OnBnClickedBtnDineTime()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	MACHINE.AndonMachineStatus(MACHINE.m_strResource,46);
	OnCancel();
}


void CAndongDlg::OnBnClickedBtnMachineJam()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	MACHINE.AndonMachineStatus(MACHINE.m_strResource,11);
	OnCancel();
}


void CAndongDlg::OnBnClickedBtnCleanAndCheck()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	MACHINE.AndonMachineStatus(MACHINE.m_strResource,40);
	OnCancel();
}


void CAndongDlg::OnBnClickedBtnMachineMinorFault()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	MACHINE.AndonMachineStatus(MACHINE.m_strResource,10);
	OnCancel();
}


void CAndongDlg::OnBnClickedBtnQualityMonitoring()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	MACHINE.AndonMachineStatus(MACHINE.m_strResource,43);
	OnCancel();
}


void CAndongDlg::OnBnClickedBtnMaterialReplace()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	MACHINE.AndonMachineStatus(MACHINE.m_strResource,32);
	OnCancel();
}


void CAndongDlg::OnBnClickedBtnSampleMaking()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	MACHINE.AndonMachineStatus(MACHINE.m_strResource,22);
	OnCancel();
}


void CAndongDlg::OnBnClickedBtnWaitMaterial()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
	MACHINE.m_DlgAndonFeed.ShowWindow(SW_SHOW);
}



void CAndongDlg::OnBnClickedBtnAbnormalQuality()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	MACHINE.AndonMachineStatus(MACHINE.m_strResource,13);
	OnCancel();
}


void CAndongDlg::OnBnClickedBtnAbnormalMaterial()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	MACHINE.AndonMachineStatus(MACHINE.m_strResource,31);
	OnCancel();
}


void CAndongDlg::OnBnClickedBtnAbnormalItEthernet()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	MACHINE.AndonMachineStatus(MACHINE.m_strResource,55);
	OnCancel();
}


void CAndongDlg::OnBnClickedBtnAbnormalPe()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	MACHINE.AndonMachineStatus(MACHINE.m_strResource,52);
	OnCancel();
}
