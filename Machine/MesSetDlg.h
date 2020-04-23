#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CMesSetDlg �Ի���

#include ".\EditableListCtrl\EditableListCtrl.h"

class CMesSetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMesSetDlg)

public:
	CMesSetDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMesSetDlg();

	// �Ի�������
	enum { IDD = IDD_DIG_MES_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();


	CEditableListCtrl m_listMesData;
	void initGrid();//ListCtrl��ʼ��Ϊ���ģʽ
	void AddLine(CString strname);
	void InitGridSetting();//��ʼ���������
	void SaveConfig();//���������ļ�
	void LoadConfig();//���������ļ�
	void AdjustLayout();

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedBtnMesLogin();
	afx_msg void OnBnClickedBtnSaveConfig();
	afx_msg void OnBnClickedButCoreInbound();
	afx_msg void OnBnClickedButCoreOutbound();
};
