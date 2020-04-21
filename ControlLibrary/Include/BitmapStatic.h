#pragma once


// CBitmapStatic

class  __declspec(dllexport) CBitmapStatic : public CStatic
{
	DECLARE_DYNAMIC(CBitmapStatic)

public:
	CBitmapStatic();
	virtual ~CBitmapStatic();
	BOOL m_bLoadPic;
	void LoadPicture(CString strFileName);
protected:
	DECLARE_MESSAGE_MAP()
	CImage	m_imageBkg;
public:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


