#pragma once


class COutputList : public CListBox
{
	// Construction
public:
	COutputList();

	// Implementation
public:
	virtual ~COutputList();

protected:

	DECLARE_MESSAGE_MAP()
};

// COutputView

class COutputView : public CDockablePane
{
	DECLARE_DYNAMIC(COutputView)

public:
	COutputView();
	virtual ~COutputView();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg LRESULT OnListCtrlShowMsg(WPARAM wparam, LPARAM lparam);
	DECLARE_MESSAGE_MAP()

public:
	void UpdateFonts();
	void UpdateLanguage();

public:
	CMFCTabCtrl	m_wndTabs;
	COutputList m_wndOutputCommInfo;
	COutputList m_wndOutputAlarmInfo;
};


