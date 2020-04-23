#pragma once


// CAndonFeedDlg 对话框

class CAndonFeedDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAndonFeedDlg)

public:
	CAndonFeedDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAndonFeedDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_ANDONG_FEED };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg void OnBnClickedBtnAndonWaitPmcMaterial();
	afx_msg void OnBnClickedBtnAndonWaitWorkroomMaterial();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
public:
	CBrush m_Brush;
};
