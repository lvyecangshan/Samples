#pragma once

#include <vector>
// CMaintenanceTabCtrl

class __declspec(dllexport) CMaintenanceTabCtrl : public CTabCtrl
{
	DECLARE_DYNAMIC(CMaintenanceTabCtrl)

public:
	CMaintenanceTabCtrl();
	virtual ~CMaintenanceTabCtrl();
public:
	BOOL Init(CRunCtrl *pRunCtrl = NULL);
	void AddPage(CDialogWithLang * dlgPage,UINT dlgID);
	void InsertTab();
	void InsertTab(CString strTitle);
	void ShowPage(int nCurPage = 0);
	int GetTabIndex(){return m_nTabIndex;}
	void SetCurPorduct(int nPkg);
protected:
	DECLARE_MESSAGE_MAP()
	void AdjustLayout();
	int m_nTabIndex;
vector<CDialogWithLang*> m_vecPage;
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


