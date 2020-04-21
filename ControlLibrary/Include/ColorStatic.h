// This file was created on March 28th 2001. By Robert Brault
#pragma once

/////////////////////////////////////////////////////////////////////////////
// CColorStatic window
#define COLOR_STATIC_BLINK_TIMER WM_USER+107

class __declspec(dllexport) CColorStatic : public CStatic
{
// Construction
public:
	void SetTextColor(COLORREF color); // This Function is to set the Color for the Text.
	void SetBkColor(COLORREF color); // This Function is to set the BackGround Color for the Text.
	void SetTextFont(int nPoint, LPCTSTR lpszFaceName, BOOL bBold = FALSE, BOOL bItalic = FALSE);
	CColorStatic();

// Overrides
	// ClassWizard generated virtual function overrides
	virtual ~CColorStatic();

	// Generated message map functions
protected:
	CBrush m_brBkgnd; // Holds Brush Color for the Static Text
	COLORREF m_crBkColor; // Holds the Background Color for the Text
	COLORREF m_crTextColor; // Holds the Color for the Text
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT ctlColor);
	CFont m_font;

	DECLARE_MESSAGE_MAP()
};
