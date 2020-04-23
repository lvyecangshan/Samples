#pragma once


#include "afxwin.h"
#include "afxcmn.h"

// CTestToolDlg 对话框

class CTestToolDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTestToolDlg)

public:
	CTestToolDlg(CWnd* pParent = NULL);   // 标准构造函数

	~CTestToolDlg();

	// 对话框数据
	enum { IDD = IDD_DIG_TEST_TOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedBtnOpneClosePress();
	afx_msg void OnBnClickedBtnZeroPressValue();
	afx_msg void OnBnClickedBtnGetPressValue();
	afx_msg void OnBnClickedRadioShellStation1();
	afx_msg void OnBnClickedRadioShellStation2();
	afx_msg void OnBnClickedBtnOpneCloseRobot();
	afx_msg void OnBnClickedBtnSendRobotCmd();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedRadioHighSpeed();
	afx_msg void OnBnClickedRadioMiddleSpeed();
	afx_msg void OnBnClickedRadioSlowSpeed();
	afx_msg void OnBnClickedBtnOpenCloseBarcode();
	afx_msg void OnBnClickedBtnScanBarcode();
	
	DECLARE_MESSAGE_MAP()

public:
	void ErrorExit(LPTSTR lpszFunction);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void AlterRobotSpeed();

	double m_dPressResult;

public:
	CListBox m_RobotCmdList;
	CListBox m_RobotResponseList;
	CListBox m_ScanCodeList;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnLbnSelchangeListRobotCmdList();
	afx_msg void OnBnClickedButtonServerConnect();
	afx_msg void OnBnClickedButtonDisconnectServer();
	afx_msg void OnBnClickedButtonTestSend();
	// //接收列表
	int m_nCountEx;
	CListCtrl m_ShowRecv;
	// 发送的
	int m_nCount;
	CListCtrl m_ShowSend;
	afx_msg void OnBnClickedButtonClearContent();
	afx_msg void OnBnClickedCheckRebuildCtLog();
};
