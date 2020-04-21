#pragma once
#include "afxwin.h"
#include "afxcmn.h"

// CModuleMotorPage dialog
#define MODULE_MOTOR_PAGE_TIMER 117

class __declspec(dllexport) CCPModuleMotorPage : public CDialogWithLang
{
	DECLARE_DYNAMIC(CCPModuleMotorPage)
	
public:
	CCPModuleMotorPage(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCPModuleMotorPage();
	void OnSetActive();
	CRunCtrl *m_pRunCtrl;
// Dialog Data
	enum { IDD = IDD_MODULE_MOTOR_PAGE };
	void UpdateLanguage();
protected:
	CString	GetDoubleValueStr(double dValue);
	void	FillMotorList();
	void FillMotorView(int nCurModue,int nCurProduct,int nMotor);

protected:
    CSensorCtrl m_wndHomeSensor;
    CSensorCtrl m_wndNLimitSensor;
    CSensorCtrl m_wndPLimitSensor;
    CSensorCtrl m_wndReadySensor;
    CSensorCtrl m_wndAlarmSensor;
    CSensorCtrl m_wndInPosSensor;
    CSensorCtrl m_wndServoOnSensor;

	CIconButton m_btnHome;
    CIconButton m_btnMove;
    CIconButton m_btnStop;

    CImageList m_imageList;

	int	m_nMotorListCurSel;

    int m_nSelMotorIndex;
    int m_nSelLocIndex;
    int m_nSelSubLocIndex;
   
	int m_nCurModule;
	int m_nMotorPos;

	int m_nJogMode;

	BOOL m_bModified;

    BOOL m_bAllowUserInteraction;

  	CString m_strMotionStatus[NUM_MOTION_STATUS];

	// for motor offset value backup, modify the offset without saving will be saved here instead save directly to package
	vector<double> m_dMotorPosition;

protected:
	void    InitJogMode();
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnCbnSelchangeModuleMotorList();
	afx_msg void OnEnChangeModuleMotorPosSet();
    afx_msg void OnBnClickedModuleMotorSvon();
    afx_msg void OnBnClickedModuleMotorHomeMove();
    afx_msg void OnBnClickedModuleMotorMove();
    afx_msg void OnBnClickedModuleMotorStop();
    afx_msg void OnBnClickedModuleMotorSave();
	afx_msg void OnTvnSelchangedModuleMotorTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedModuleMotorEditParam();
	afx_msg void OnBnClickedModuleMotorJogForward();
	afx_msg void OnBnClickedModuleMotorJogBackward();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnStnDblclickModuleMotorCurPos();
	afx_msg void OnBnClickedModuleAxisOffset();
	afx_msg void OnCbnSelchangeComboJogMode();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
	void StopExtJog();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	void SetupScrollbars();
	void ResetScrollbars();

	CRect m_ClientRect;
	int m_nVertInc;
	int m_nVscrollMax;
	int m_nVscrollPos;
	afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    CStatic m_stModuleMotorStatus;
    CStatic m_stModuleMotorCurPos;
    CStatic m_stModuleMotorError;
    CStatic m_stModuleMotorLocation;
    CIconButton m_btnModuleMotorJogForward;
    CIconButton m_btnModuleMotorJogBackward;
    CIconButton m_btnModuleMotorSave;
    CIconButton m_btnModuleMotorEditParam;
    CIconButton m_btnModuleMotorSvon;
	CListCtrl m_listMotorPos;
	int m_nCurProduct;
	afx_msg void OnNMClickPageMotorPosList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnStnDblclickModuleMotorLocation();
	afx_msg void OnEnKillfocusModuleMotorPosset();
	afx_msg void OnEnKillfocusModuleMotorJogDist();
};
