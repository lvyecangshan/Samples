#pragma once

#include <atlimage.h>

// CDigitLED
class __declspec(dllexport) CDigitLED : public CStatic
{
	DECLARE_DYNAMIC(CDigitLED)

public:
	CDigitLED();
	virtual ~CDigitLED();

	void SetError(BOOL bError = TRUE) { m_bError = bError; Invalidate(FALSE); }
	void SetValue(double lfValue) { m_bError = FALSE; m_lfValue = lfValue; Invalidate(FALSE); }
	void SetAfterDotDigit(int numDigit) { m_numAfterDotDigit = numDigit; Invalidate(FALSE); }

protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();

protected:
	void DrawDigit(CPaintDC *pDC, int &x, UINT digit);

protected:
	CImage	m_image;
	int		m_numAfterDotDigit;
	double  m_lfValue;
	BOOL    m_bError;
};

