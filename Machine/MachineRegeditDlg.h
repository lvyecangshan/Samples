#pragma once

// CMachineRegeditDlg �Ի���

class CMachineRegeditDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMachineRegeditDlg)

public:
	CMachineRegeditDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMachineRegeditDlg();

// �Ի�������
	enum { IDD = IDD_DLG_MACHINE_REGEDIT };

protected:
	afx_msg void OnBnClickedGetSerialNumber();
	afx_msg void OnBnClickedRegisterMachine();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString	m_strSerialCode;
	CString	m_strRegedit;
	///AutoEncrypt
};
