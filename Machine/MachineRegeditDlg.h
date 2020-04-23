#pragma once

// CMachineRegeditDlg 对话框

class CMachineRegeditDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMachineRegeditDlg)

public:
	CMachineRegeditDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMachineRegeditDlg();

// 对话框数据
	enum { IDD = IDD_DLG_MACHINE_REGEDIT };

protected:
	afx_msg void OnBnClickedGetSerialNumber();
	afx_msg void OnBnClickedRegisterMachine();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString	m_strSerialCode;
	CString	m_strRegedit;
	///AutoEncrypt
};
