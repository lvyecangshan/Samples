#pragma once
typedef struct tagDjpRECT
{
	LONG    index;
	RECT    rect;
} DRECT;
// CAnimatedModule
class __declspec(dllexport) CAnimatedModule : public CStatic
{
	DECLARE_DYNAMIC(CAnimatedModule)

public:
	CAnimatedModule();
	virtual ~CAnimatedModule();

	BOOL Initialize(CRunCtrl *pRunCtrl = NULL);

protected:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()

protected:
	
	// the images that contains the right picture and the alarm picture
	CImage	m_imageBkg;
	CImage	m_imgParts;
	CImage	m_imgTexts;

	int m_nCurModule;
public:
	CRunCtrl *m_pRunCtrl;
	CString m_strFileName;
	vector<DRECT> m_vecRect;
	vector<DRECT> m_vecInputRect;
	CPoint m_Point;
	CPoint m_StartPoint;
	CPoint m_EndPoint;
	void ReadParameter(int nCurModule);
	void SaveParameter(int nCurModule);
	void LoadPicture(CString strFileName);
	void LoadCombox(CComboBox* ComboBox = NULL,int nInput = 0);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};