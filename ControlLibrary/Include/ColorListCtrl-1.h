#pragma once
#include "afxcmn.h"

class __declspec(dllexport) CColorListCtrl : public CListCtrl
{
public:
	CColorListCtrl(void);
	~CColorListCtrl(void);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);

private:
	CMap<DWORD,DWORD&,COLORREF,COLORREF&> MapItemColor;
	int m_nItemCount;
public:
	void SetItemColor(DWORD Item,COLORREF color);
	void SetAllItemColor(DWORD Item,COLORREF TextBKColor);
};
