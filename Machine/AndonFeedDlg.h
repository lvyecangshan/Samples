#pragma once


// CAndonFeedDlg �Ի���

class CAndonFeedDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAndonFeedDlg)

public:
	CAndonFeedDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAndonFeedDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_ANDONG_FEED };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg void OnBnClickedBtnAndonWaitPmcMaterial();
	afx_msg void OnBnClickedBtnAndonWaitWorkroomMaterial();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
public:
	CBrush m_Brush;
};
