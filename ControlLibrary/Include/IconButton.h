#pragma once

class __declspec(dllexport)CIconButton : public CButton
{
	DECLARE_DYNAMIC(CIconButton)

public:
	CIconButton();
	virtual ~CIconButton();

	 enum ICON_STYTLE
	 {	
	    ICON_LEFT_TEXT_CENTER = 0,		
		ICON_LEFT_TEXT_LEFT,
		ICON_RITHT_TEXT_CENTER,
		ICON_TOP_TEXT_BOTTOM,						
	 };
    struct STRUCT_ICONS
	{
		HICON		hIcon;			
		DWORD		dwWidth;		
		DWORD		dwHeight;		
		BOOL        bIcon16;  
	};
protected:
	BOOL m_bOver;	
	BOOL m_bTracking;	
	BOOL m_bSelected;	
	BOOL m_bFocus;
	STRUCT_ICONS m_sIcon;
	ICON_STYTLE m_nIconStyle;
public:
	void SetStyle(ICON_STYTLE style);
	void SetIcon(HICON hIcon);
	void SetIcon(int nIconId,BOOL bIcon16 = TRUE);
protected:
	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void DoGradientFill(CDC *pDC, CRect* rect);
	virtual void DrawInsideBorder(CDC *pDC, CRect* rect);
protected:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);
protected:
	DECLARE_MESSAGE_MAP()
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};


