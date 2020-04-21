#pragma once


// CMotorParamEditDlg dialog

class CMotorParamEditDlg : public CDialogWithLang
{
	DECLARE_DYNAMIC(CMotorParamEditDlg)

public:
	CMotorParamEditDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMotorParamEditDlg();

// Dialog Data
	enum { IDD = IDD_MOTOR_PARAM_EDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	BOOL   m_bRotaryMotor;

	double m_dNormalStartUp;
	double m_dNormalMax;
	double m_dNormalAcce;
	double m_dNormalDecc;
	
	double m_dSlowStartUp;
	double m_dSlowMax;
	double m_dSlowAcce;
	double m_dSlowDecc;

	double m_dHomeStartUp;
	double m_dHomeMax;
	double m_dHomeAcce;
	double m_dHomeDecc;

	double m_dVirtualHome;
	double m_dHomeGoBackDist;

	int	   m_nSpeedMode;
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnKillfocusMotorParamNormalStart();
	afx_msg void OnEnKillfocusMotorParamNormalMax();
	afx_msg void OnEnKillfocusMotorParamNormalAcce();
	afx_msg void OnEnKillfocusMotorParamNormalDecc();
	afx_msg void OnEnKillfocusMotorParamSlowStart();
	afx_msg void OnEnKillfocusMotorParamSlowMax();
	afx_msg void OnEnKillfocusMotorParamSlowAcce();
	afx_msg void OnEnKillfocusMotorParamSlowDecc();
	afx_msg void OnEnKillfocusMotorParamHomeStart();
	afx_msg void OnEnKillfocusMotorParamHomeMax();
	afx_msg void OnEnKillfocusMotorParamHomeAcce();
	afx_msg void OnEnKillfocusMotorParamHomeDecc();
	afx_msg void OnEnKillfocusMotorParamVirtualHome();
	afx_msg void OnEnKillfocusMotorParamHomeGoBackDist();
	afx_msg void OnClose();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
