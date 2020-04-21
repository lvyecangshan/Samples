#pragma once


// CMachineStateStatic
const CString strMachineStatus[NUM_MC_STATE] =
{
	_T("����"),
	_T("���ڳ�ʼ��"),
	_T("��ʼ�����"),
	_T("������"),
	_T("��ʼ��ֹͣ"),
	_T("ֹͣ"), 
	_T("��ʼ������"),
	_T("����"),
};
const CString strMachineStatusENG[NUM_MC_STATE] =
{
	_T("Begin"),
	_T("Initializing..."),
	_T("Initialized"),
	_T("Running"),
	_T("Stop init"),
	_T("Stop"), 
	_T("Init error"),
	_T("Run jam"),
};
class __declspec(dllexport) CMachineStateStatic : public CStatic
{
	DECLARE_DYNAMIC(CMachineStateStatic)
public:
	CMachineStateStatic();
	virtual ~CMachineStateStatic();
	BOOL Init(CRunCtrl * pRunCtrl = NULL,int nTime = 1000);
protected:
	DECLARE_MESSAGE_MAP()
	CFont m_font;
	BOOL m_bShow;
	CBitmap m_btBackup;
	CBitmap *m_OldBmp;
	int m_nRunState;
	CRunCtrl *m_pRunCtrl;
	CDC *m_pDC;
	CDC *m_cDC;
	HBRUSH m_hrBrush;
public:
	afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


