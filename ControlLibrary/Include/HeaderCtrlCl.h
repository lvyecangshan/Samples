#pragma once 
 
 
// CHeaderCtrlCl 
 
class __declspec(dllexport) CHeaderCtrlCl : public CHeaderCtrl 
{ 
	DECLARE_DYNAMIC(CHeaderCtrlCl) 
 
public: 
	CHeaderCtrlCl(); 
	virtual ~CHeaderCtrlCl(); 
 
protected: 
	DECLARE_MESSAGE_MAP() 
public: 
	afx_msg void OnPaint(); 
	CStringArray m_HChar; 
	CString m_Format; //0 left 1center 2right 
public: 
	int m_R; 
	int m_G; 
	int m_B; 
	int m_Gradient;	 
	float m_Height;   
	int m_fontHeight; 
	int m_fontWith;  
	COLORREF m_color; 
	LRESULT OnLayout( WPARAM wParam, LPARAM lParam ); 
}; 
