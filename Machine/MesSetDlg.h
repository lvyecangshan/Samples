#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CMesSetDlg 对话框

#include ".\EditableListCtrl\EditableListCtrl.h"

class CMesSetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMesSetDlg)

public:
	CMesSetDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMesSetDlg();

	// 对话框数据
	enum { IDD = IDD_DIG_MES_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();


	CEditableListCtrl m_listMesData;
	void initGrid();//ListCtrl初始化为表格模式
	void AddLine(CString strname);
	void InitGridSetting();//初始化表格内容
	void SaveConfig();//保存配置文件
	void LoadConfig();//加载配置文件
	void AdjustLayout();

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedBtnMesLogin();
	afx_msg void OnBnClickedBtnSaveConfig();
	afx_msg void OnBnClickedButCoreInbound();
	afx_msg void OnBnClickedButCoreOutbound();
};
