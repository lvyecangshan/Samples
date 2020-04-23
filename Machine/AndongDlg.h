#pragma once


// CAndongDlg 对话框

class CAndongDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAndongDlg)

public:
	CAndongDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAndongDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_ANDONG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedBtnNoPlan();
	afx_msg void OnBnClickedBtnTrainAssess();
	afx_msg void OnBnClickedBtnInventory();
	afx_msg void OnBnClickedBtnOtherReason();
	afx_msg void OnBnClickedBtnMaintenanceRepair();
	afx_msg void OnBnClickedBtnChangeType();
	afx_msg void OnBnClickedBtnChangeShift();
	afx_msg void OnBnClickedBtnDineTime();
	afx_msg void OnBnClickedBtnMachineJam();
	afx_msg void OnBnClickedBtnCleanAndCheck();
	afx_msg void OnBnClickedBtnMachineMinorFault();
	afx_msg void OnBnClickedBtnQualityMonitoring();
	afx_msg void OnBnClickedBtnMaterialReplace();
	afx_msg void OnBnClickedBtnSampleMaking();
	afx_msg void OnBnClickedBtnWaitMaterial();
	afx_msg void OnBnClickedBtnAbnormalQuality();
	afx_msg void OnBnClickedBtnAbnormalMaterial();
	afx_msg void OnBnClickedBtnAbnormalItEthernet();
	afx_msg void OnBnClickedBtnAbnormalPe();
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
public:
	CString m_strAlarmMes;
	CFont m_Font;
	CBrush m_brush;
};
