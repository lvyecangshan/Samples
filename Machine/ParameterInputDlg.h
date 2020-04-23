#pragma once


// CParameterInputDlg dialog

class CParameterInputDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CParameterInputDlg)

public:
	CParameterInputDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CParameterInputDlg();

// Dialog Data
	enum { IDD = IDD_DLG_PARAMETER_INPUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strValue;
	afx_msg void OnBnClickedParameterInputBtnOk();
	virtual BOOL OnInitDialog();
};
