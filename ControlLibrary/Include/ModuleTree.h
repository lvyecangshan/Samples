#pragma once
#include "afxcmn.h"
//#include "Machine.h"
struct __declspec(dllexport) TreeInfo
{
	TreeInfo()
	{
		hParentItem = NULL;
		hItem = NULL;
		strName = _T("");
		strClass = _T("");
		bEnable = FALSE;
		bDryRun = FALSE;
		bOnLoad = FALSE;
		bButton = FALSE;
	}
	HTREEITEM hParentItem;
	HTREEITEM hItem;
	CString strName;
	CString strClass;
	BOOL bEnable;
	BOOL bDryRun;
	BOOL bOnLoad;
	BOOL bButton;
};

class __declspec(dllexport) CModuleTree :
	public CTreeCtrl
{
public:
	CModuleTree(void);
	~CModuleTree(void);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	BOOL Init(BOOL bAll = FALSE,CRunCtrl* pRunCtrl = NULL);
	void CheckChange(HTREEITEM hItem, BOOL bValue = TRUE);
	void SelectItems(int nStyle,BOOL bValue = TRUE);
	BOOL SaveValue();
	void SaveConfig();
	void SetOnlyInitEnable(BOOL bEnable = TRUE);
public:
	CArray<TreeInfo> m_ModuleTree;
	CRunCtrl *m_pRunCtrl;
	int m_nType;
	BOOL m_bCheckOk;;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	BOOL m_bOnlyInitEnable;
};
