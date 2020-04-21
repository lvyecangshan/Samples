#pragma once


// CTabCtrlD

class __declspec(dllexport) CTabCtrlD : public CTabCtrl
{
	DECLARE_DYNAMIC(CTabCtrlD)

public:
	CTabCtrlD();
	virtual ~CTabCtrlD();

protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


