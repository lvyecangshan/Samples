#pragma once

// CNonStdFontPropertySheet
class __declspec(dllexport) CNonStdFontPropertySheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CNonStdFontPropertySheet)

public:
	CNonStdFontPropertySheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CNonStdFontPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CNonStdFontPropertySheet();
	virtual BOOL OnInitDialog();
	void SetDesiredLocation(CRect rect) { m_rcDesiredLocation = rect; m_rcDesiredLocation.bottom -= 3; m_rcDesiredLocation.right -= 3; }

protected:
	DECLARE_MESSAGE_MAP()
	virtual void BuildPropPageArray ();
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnResizePage (UINT, LONG);
	afx_msg void OnClose();

    void ChangePropertySheetFont(CWnd *pWnd, int nFlag, CRect rcDesired);
    void ChangeChildWindowFont(CWnd *pWnd, float fRatioX, float fRatioY, BOOL bChangeGrandChildren = TRUE);

protected:
	RECT m_rctPage;
	CFont m_fntPage;
    CRect m_rcDesiredLocation;
};


