#pragma once 
#include "HeaderCtrlCl.h" 
// CListCtrlCl 
 
class __declspec(dllexport) CListCtrlCl : public CListCtrl 
{ 
	DECLARE_DYNAMIC(CListCtrlCl) 
 
public: 
	CHeaderCtrlCl m_Header; 
	CListCtrlCl(); 
	virtual ~CListCtrlCl(); 
 
protected: 
	DECLARE_MESSAGE_MAP() 
	virtual void PreSubclassWindow(); 
public: 
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct); 
public: 
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct); 
	void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct); 
public: 
	// col height 
	int m_nRowHeight; 
	int InsertColumn(int nCol, LPCTSTR lpszColumnHeading, 
		int nFormat = LVCFMT_LEFT, int nWidth = -1, int nSubItem = -1); 
public: 
	// Gradient - 
	void SetHeaderBKColor(int R, int G, int B, int Gradient); 
public: 
	// 
	void SetHeaderHeight(float Height); 
	CPtrList m_ptrListCol;   
	CPtrList m_ptrListItem;  
	CPtrList m_colTextColor;  
	CPtrList m_ItemTextColor; 
	bool FindColColor(int col ,COLORREF &color);  
	bool FindItemColor(int col,int row,COLORREF &color); 
	bool FindColTextColor(int col,COLORREF &color);  
	bool FindItemTextColor(int col,int row,COLORREF &color); 
	void SetColColor(int col,COLORREF color);   
	void SetItemColor(int col,int row,COLORREF color);  
	void SetColTextColor(int col,COLORREF color);  
	void SetItemTextColor(int col,int row,COLORREF color); 
	void SetRowHeigt(int nHeight); 
	void SetHeaderFontHW(int nHeight,int nWith); 
	void SetHeaderTextColor(COLORREF color); 
	COLORREF m_color; 
	BOOL SetTextColor(COLORREF cr); 
	void SetFontHW(int nHeight,int nWith);  
public: 
	int m_fontHeight; 
public: 
	int m_fontWith; 
}; 
