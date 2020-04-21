#pragma once
#include "AnimatedModule.h"
#include "afxwin.h"

// CMaintenancePictureDlg dialog

class __declspec(dllexport) CMaintenancePictureDlg : public CDialog
{
	DECLARE_DYNAMIC(CMaintenancePictureDlg)

public:
	CMaintenancePictureDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMaintenancePictureDlg();

// Dialog Data
	enum { IDD = IDD_DLG_MAINTENANCE_PICTURE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CAnimatedModule m_AnimatedModule;
	virtual BOOL OnInitDialog();
	int m_nLeft;
	int m_nTop;
	int m_nRight;
	int m_nBottom;
	CComboBox m_cbOutPut;
	int m_nCurModule;
	CRunCtrl *m_pRunCtrl;
	void Initialize(CRunCtrl *pRunCtrl= NULL);
	afx_msg void OnBnClickedModulePicSet();
	afx_msg void OnBnClickedModulePicSave();
	afx_msg void OnCbnSelchangeCbModulePicOutput();
	afx_msg void OnBnClickedMaintenancePicInput();
	int m_nInput;
	afx_msg void OnBnClickedMaintenancePicOutput();
	CString m_strFileName;
	afx_msg void OnBnClickedModulePicSelect();
};
