#pragma once
#include "afxwin.h"

#define NUM_OF_SENSOR_CTRL 12
#define NUM_OF_OUTPUT_CTRL 12
#define MODULE_IO_PAGE_TIMER 107

// CModuleIOPage dialog

class __declspec(dllexport) CModuleIOPage : public CDialogWithLang
{
	DECLARE_DYNAMIC(CModuleIOPage)

public:
	CModuleIOPage(CWnd* pParent = NULL);   // standard constructor
	virtual ~CModuleIOPage();
	CRunCtrl *m_pRunCtrl;
// Dialog Data
	enum { IDD = IDD_MODULE_IO_PAGE };
	void UpdateInputOutputText();
	int m_nPosBar;
	void UpdateLanguage();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CSensorCtrl m_wndSensors[NUM_OF_SENSOR_CTRL];
	//CIconButton  m_btnOutputs[NUM_OF_OUTPUT_CTRL];
	COutputCtrl m_btnOutputs[NUM_OF_OUTPUT_CTRL];

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_stIOInput;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CStatic m_stIOOutput;
	CScrollBar m_scrollbarIO;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg void OnOutputButtonClick(UINT nID);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	void OnSetActive();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnNMThemeChangedScrollbarIoPage(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedStaticIoOutput();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
