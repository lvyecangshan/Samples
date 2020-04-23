
#pragma once

#include "ViewTree.h"
#include "ModuleTree.h"
// CModuleViewToolBar

class CModuleViewToolBar : public CMFCToolBar
{
	/*DECLARE_DYNAMIC(CModuleViewToolBar)

public:
	CModuleViewToolBar();
	virtual ~CModuleViewToolBar();

protected:
	DECLARE_MESSAGE_MAP()*/
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(),bDisableIfNoHndler);
	}
	virtual BOOL AllowShowOnList() const { return FALSE;}
};

// CModuleView

class CModuleView : public CDockablePane
{
	DECLARE_DYNAMIC(CModuleView)

public:
	CModuleView();
	virtual ~CModuleView();
	void UpdateLanguage();
	void AdjustLayout();
	void init(BOOL bAll,CRunCtrl *pRunCtrl = NULL){m_wndTreeView.Init(bAll,pRunCtrl);ShowPane(TRUE,FALSE,TRUE);}
protected:
	CModuleViewToolBar m_wndToolBar;
	CModuleTree m_wndTreeView;
protected:


	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnModuleEnable();
	afx_msg void OnModuleDryrun();
	afx_msg void OnModuleSave();
};


